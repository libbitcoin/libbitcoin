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
#include <bitcoin/bitcoin/network/protocol_address.hpp>

#include <functional>
#include <bitcoin/bitcoin/config/authority.hpp>
#include <bitcoin/bitcoin/error.hpp>
#include <bitcoin/bitcoin/message/address.hpp>
#include <bitcoin/bitcoin/message/get_address.hpp>
#include <bitcoin/bitcoin/message/network_address.hpp>
#include <bitcoin/bitcoin/network/channel.hpp>
#include <bitcoin/bitcoin/network/p2p.hpp>
#include <bitcoin/bitcoin/network/protocol_base.hpp>
#include <bitcoin/bitcoin/utility/assert.hpp>
#include <bitcoin/bitcoin/utility/assert.hpp>
#include <bitcoin/bitcoin/utility/threadpool.hpp>

INITIALIZE_TRACK(bc::network::protocol_address);

namespace libbitcoin {
namespace network {

#define NAME "address"

using namespace bc::message;
using std::placeholders::_1;
using std::placeholders::_2;

protocol_address::protocol_address(threadpool& pool, p2p& network,
    const settings& settings, channel::ptr channel)
  : protocol_base(pool, channel, NAME),
    network_(network),
    self_(settings.self),
    disabled_(settings.host_pool_capacity == 0),
    CONSTRUCT_TRACK(protocol_address, LOG_NETWORK)
{
}

void protocol_address::start()
{
    if (self_.port() != 0)
    {
        address self({ { self_.to_network_address() } });
        send(self, &protocol_address::handle_send_address, _1);
    }

    // If we can't store addresses we don't ask for or receive them.
    if (disabled_)
        return;

    protocol_base::start();

    subscribe<get_address>(
        &protocol_address::handle_receive_get_address, _1, _2);
    send(get_address(), &protocol_address::handle_send_get_address, _1);
}

void protocol_address::handle_receive_address(const code& ec,
    const address& message)
{
    if (stopped())
        return;

    if (ec)
    {
        log_debug(LOG_PROTOCOL)
            << "Failure receiving address message from ["
            << authority() << "] " << ec.message();
        stop(ec);
        return;
    }

    // Resubscribe to address messages.
    subscribe<address>(&protocol_address::handle_receive_address, _1, _2);

    log_debug(LOG_PROTOCOL)
        << "Storing addresses from [" << authority() << "] ("
        << message.addresses.size() << ")";

    // TODO: manage timestamps (active channels are connected < 3 hours ago).
    network_.store(message.addresses,
        bind(&protocol_address::handle_store_addresses, _1));
}

void protocol_address::handle_receive_get_address(const code& ec,
    const get_address& message)
{
    if (stopped())
        return;

    if (ec)
    {
        log_debug(LOG_PROTOCOL)
            << "Failure receiving get_address message from ["
            << authority() << "] " << ec.message();
        stop(ec);
        return;
    }

    // TODO: allowing repeated queries can allow a channel to map our history.
    // Resubscribe to get_address messages.
    subscribe<get_address>(
        &protocol_address::handle_receive_get_address, _1, _2);

    // TODO: pull active hosts from host cache (currently just resending self).
    // TODO: need to distort for privacy, don't send currently-connected peers.
    address active({ { self_.to_network_address() } });
    if (active.addresses.empty())
        return;

    log_debug(LOG_PROTOCOL)
        << "Sending addresses to [" << authority() << "] ("
        << active.addresses.size() << ")";

    send(active, &protocol_address::handle_send_address, _1);
}

void protocol_address::handle_send_address(const code& ec)
{
    if (stopped())
        return;

    if (ec)
    {
        log_debug(LOG_PROTOCOL)
            << "Failure sending address [" << authority() << "] "
            << ec.message();
        stop(ec);
    }
}

void protocol_address::handle_send_get_address(const code& ec)
{
    if (stopped())
        return;

    if (ec)
    {
        log_debug(LOG_PROTOCOL)
            << "Failure sending get_address [" << authority() << "] "
            << ec.message();
        stop(ec);
    }
}

void protocol_address::handle_store_addresses(const code& ec)
{
    if (stopped())
        return;

    if (ec)
    {
        log_error(LOG_PROTOCOL)
            << "Failure storing addresses from [" << authority() << "] "
            << ec.message();
        stop(ec);
    }
}

} // namespace network
} // namespace libbitcoin
