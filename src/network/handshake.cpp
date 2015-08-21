/**
 * Copyright (c) 2011-2015 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <bitcoin/bitcoin/network/handshake.hpp>

#include <cstdint>
#include <cstdlib>
#include <mutex>
#include <functional>
#include <system_error>
#include <bitcoin/bitcoin/config/authority.hpp>
#include <bitcoin/bitcoin/constants.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/error.hpp>
#include <bitcoin/bitcoin/network/channel.hpp>
#include <bitcoin/bitcoin/network/timeout.hpp>
#include <bitcoin/bitcoin/utility/assert.hpp>
#include <bitcoin/bitcoin/utility/random.hpp>
#include <bitcoin/bitcoin/version.hpp>

namespace libbitcoin {
namespace network {

using std::placeholders::_1;
using std::placeholders::_2;

#define BC_USER_AGENT "/libbitcoin:" LIBBITCOIN_VERSION "/"

enum services: uint64_t
{
    node_network = 1
};

static constexpr uint32_t no_timestamp = 0;
static constexpr uint16_t unspecified_ip_port = 0;
static constexpr message::ip_address unspecified_ip_address
{
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00
    }
};

const message::network_address handshake::unspecified
{
    no_timestamp,
    services::node_network,
    unspecified_ip_address,
    unspecified_ip_port
};

handshake::handshake(threadpool& pool, const config::authority& self,
    const timeout& timeouts)
  : strand_(pool), timeouts_(timeouts), timer_(pool.service())
{
    // relay and address_you are set in ready().
    template_version_.address_you = unspecified;
    template_version_.relay = true;

    // start_height is managed dynamically by node.
    template_version_.start_height = 0;

    // Constant values.
    template_version_.address_me = self.to_network_address();
    template_version_.address_me.services = services::node_network;
    template_version_.services = services::node_network;
    template_version_.user_agent = BC_USER_AGENT;
    template_version_.version = bc::protocol_version;
}

// This will not fire the handshake completion until all three
// subscriptions or the timer fires. synchronize guards against the
// possiblity of conflicting timer completion callbacks.
void handshake::start(channel_ptr node, handshake_handler handle_handshake,
    bool relay)
{
    BITCOIN_ASSERT(node);

    // Require three callbacks (or any error) before calling handle_handshake.
    const auto complete = synchronizer<handshake_handler>(
        std::bind(&handshake::handle_synced,
            this, _1, handle_handshake), 3, "handshake");

    // Create a copy of the version template.
    auto session_version = template_version_;

    // Set required transaction relay policy for the connection.
    session_version.relay = relay;

    // TODO: bury the address_you-setting into the proxy.
    // Set the peer's address into the outgoing version.
    // The timestamp should not used here and there's no need to set services.
    session_version.address_you = node->address().to_network_address();

    // TODO: bury the nonce-setting into the proxy.
    // The nonce is used to detect connections to self. It is chosen at random
    // for each connection to minimize persistent collisions.
    // Add nonce to channel state for loopback detection.
    session_version.nonce = pseudo_random();
    node->set_nonce(session_version.nonce);

    // 1 of 3
    node->subscribe_version(
        strand_.wrap(&handshake::receive_version,
            this, _1, _2, node, complete));

    // 2 of 3
    node->subscribe_verack(
        strand_.wrap(&handshake::receive_verack,
            this, _1, _2, node, complete));

    // 3 of 3
    node->send(session_version,
        strand_.wrap(&handshake::handle_version_sent,
            this, _1, node, complete));

    // timeout error
    start_timer(node, complete);
}

void handshake::handle_synced(const std::error_code& ec,
    handshake_handler handle_handshake)
{
    // Speed up the demise of the timer.
    boost::system::error_code code;
    timer_.cancel(code);

    handle_handshake(ec);
}

void handshake::start_timer(channel_ptr node,
    handshake_handler completion_callback)
{
    timer_.expires_from_now(timeouts_.handshake);
    timer_.async_wait(
        std::bind(&handshake::handle_timer,
            this, _1, node, completion_callback));
}

void handshake::handle_timer(const boost::system::error_code& ec,
    channel_ptr node, handshake_handler completion_callback)
{
    // A success code implies that the timer fired.
    if (!ec)
        completion_callback(error::channel_timeout);
}

void handshake::handle_version_sent(const std::error_code& ec,
    channel_ptr node, handshake_handler completion_callback)
{
    completion_callback(ec);
}

void handshake::receive_version(const std::error_code& ec, 
    const message::announce_version& version, channel_ptr node,
    handshake_handler completion_callback)
{
    if (ec)
    {
        completion_callback(ec);
        return;
    }

    // TODO: add loopback detection to the channel.
    // TODO: set the protocol version on node (for feature degradation).
    // TODO: save relay to node and have protocol not relay if false.
    // TODO: trace out version.version|services|user_agent.
    // node->set_version(version);

    if (version.version < bc::peer_minimum_version)
    {
        log_debug(LOG_NETWORK)
            << "Peer version (" << version.version << ") below minimum ("
            << bc::peer_minimum_version << ") [" << node->address() << "]";
        completion_callback(error::accept_failed);
        return;
    }

    node->send(message::verack(),
        strand_.wrap(&handshake::handle_verack_sent,
            this, _1, completion_callback));
}

void handshake::handle_verack_sent(const std::error_code& ec,
    handshake_handler completion_callback)
{
    completion_callback(ec);
}

void handshake::receive_verack(const std::error_code& ec,
    const message::verack&, channel_ptr node,
    handshake_handler completion_callback)
{
    if (!ec)
    {
        // TODO: enable handshake timeout on node construct.
        // TODO: disable handshake timeout.
        // node->reset_handshake();

        // TODO: we don't care what it says about IP addresses but we may want
        // to add inbound connnection addresses to hosts.
        // Find out whether we would add self and/or inbound peers.
    }

    // We may not get this response before timeout, in which case we can
    // only assume that our version wasn't accepted.
    completion_callback(ec);
}

void handshake::set_start_height(uint64_t height, setter_handler handle_set)
{
    strand_.queue(
        std::bind(&handshake::do_set_start_height,
            this, height, handle_set));
}

void handshake::do_set_start_height(uint64_t height, setter_handler handle_set)
{
    // We type this method as uint64_t because that is what is returned by
    // fetch_last_height, which feeds directly into this method. But start_height
    // is uint32_t in the satoshi network protocol.
    BITCOIN_ASSERT(height <= bc::max_uint32);
    template_version_.start_height = static_cast<uint32_t>(height);
    handle_set(error::success);
}

} // namespace network
} // namespace libbitcoin
