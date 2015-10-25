/**
 * Copyright (c) 2011-2018 libbitcoin developers (see AUTHORS)
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
#include <bitcoin/bitcoin/network/p2p.hpp>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <bitcoin/bitcoin/config/endpoint.hpp>
#include <bitcoin/bitcoin/error.hpp>
#include <bitcoin/bitcoin/network/channel.hpp>
#include <bitcoin/bitcoin/network/hosts.hpp>
#include <bitcoin/bitcoin/network/network_settings.hpp>
#include <bitcoin/bitcoin/network/pending.hpp>
#include <bitcoin/bitcoin/network/protocol_address.hpp>
#include <bitcoin/bitcoin/network/protocol_ping.hpp>
#include <bitcoin/bitcoin/network/protocol_seed.hpp>
#include <bitcoin/bitcoin/network/protocol_version.hpp>
#include <bitcoin/bitcoin/network/session_inbound.hpp>
#include <bitcoin/bitcoin/network/session_manual.hpp>
#include <bitcoin/bitcoin/network/session_outbound.hpp>
#include <bitcoin/bitcoin/network/session_seed.hpp>
#include <bitcoin/bitcoin/utility/assert.hpp>
#include <bitcoin/bitcoin/utility/logger.hpp>
#include <bitcoin/bitcoin/utility/threadpool.hpp>

INITIALIZE_TRACK(bc::network::channel::channel_subscriber);

namespace libbitcoin {
namespace network {

#define NAME "channel::subscriber"

using std::placeholders::_1;

// common settings
#define NETWORK_THREADS                     4
#define NETWORK_INBOUND_CONNECTION_LIMIT    8
#define NETWORK_OUTBOUND_CONNECTIONS        8
#define NETWORK_CONNECT_ATTEMPTS            0
#define NETWORK_CONNECT_TIMEOUT_SECONDS     5
#define NETWORK_CHANNEL_HANDSHAKE_SECONDS   30
#define NETWORK_CHANNEL_REVIVAL_MINUTES     5
#define NETWORK_CHANNEL_HEARTBEAT_MINUTES   5
#define NETWORK_CHANNEL_INACTIVITY_MINUTES  30
#define NETWORK_CHANNEL_EXPIRATION_MINUTES  90
#define NETWORK_CHANNEL_GERMINATION_SECONDS 30
#define NETWORK_HOST_POOL_CAPACITY          1000
#define NETWORK_RELAY_TRANSACTIONS          true
#define NETWORK_HOSTS_FILE                  boost::filesystem::path("hosts.cache")
#define NETWORK_DEBUG_FILE                  boost::filesystem::path("debug.log")
#define NETWORK_ERROR_FILE                  boost::filesystem::path("error.log")
#define NETWORK_SELF                        bc::unspecified_network_address
#define NETWORK_BLACKLISTS                  {}

// mainnet settings
// Seeds based on bitcoinstats.com/network/dns-servers
#define NETWORK_IDENTIFIER_MAINNET          3652501241
#define NETWORK_INBOUND_PORT_MAINNET        8333
#define NETWORK_SEEDS_MAINNET \
{ \
    { "seed.bitchannels.io", 8333 }, \
    { "seed.bitcoinstats.com", 8333 }, \
    { "seed.bitcoin.sipa.be", 8333 }, \
    { "dnsseed.bluematt.me", 8333 }, \
    { "seed.bitcoin.jonasschnelli.ch", 8333 }, \
    { "dnsseed.bitcoin.dashjr.org", 8333 } \
}

// testnet settings
#define NETWORK_IDENTIFIER_TESTNET          118034699
#define NETWORK_INBOUND_PORT_TESTNET        18333
#define NETWORK_SEEDS_TESTNET \
{ \
    { "testnet-seed.alexykot.me", 18333 }, \
    { "testnet-seed.bitcoin.petertodd.org", 18333 }, \
    { "testnet-seed.bluematt.me", 18333 }, \
    { "testnet-seed.bitcoin.schildbach.de", 18333 } \
}

const settings p2p::mainnet
{
    NETWORK_THREADS,
    NETWORK_IDENTIFIER_MAINNET,
    NETWORK_INBOUND_PORT_MAINNET,
    NETWORK_INBOUND_CONNECTION_LIMIT,
    NETWORK_OUTBOUND_CONNECTIONS,
    NETWORK_CONNECT_ATTEMPTS,
    NETWORK_CONNECT_TIMEOUT_SECONDS,
    NETWORK_CHANNEL_HANDSHAKE_SECONDS,
    NETWORK_CHANNEL_REVIVAL_MINUTES,
    NETWORK_CHANNEL_HEARTBEAT_MINUTES,
    NETWORK_CHANNEL_INACTIVITY_MINUTES,
    NETWORK_CHANNEL_EXPIRATION_MINUTES,
    NETWORK_CHANNEL_GERMINATION_SECONDS,
    NETWORK_HOST_POOL_CAPACITY,
    NETWORK_RELAY_TRANSACTIONS,
    NETWORK_HOSTS_FILE,
    NETWORK_DEBUG_FILE,
    NETWORK_ERROR_FILE,
    NETWORK_SELF,
    NETWORK_BLACKLISTS,
    NETWORK_SEEDS_MAINNET
};

const settings p2p::testnet
{
    NETWORK_THREADS,
    NETWORK_IDENTIFIER_TESTNET,
    NETWORK_INBOUND_PORT_TESTNET,
    NETWORK_INBOUND_CONNECTION_LIMIT,
    NETWORK_OUTBOUND_CONNECTIONS,
    NETWORK_CONNECT_ATTEMPTS,
    NETWORK_CONNECT_TIMEOUT_SECONDS,
    NETWORK_CHANNEL_HANDSHAKE_SECONDS,
    NETWORK_CHANNEL_REVIVAL_MINUTES,
    NETWORK_CHANNEL_HEARTBEAT_MINUTES,
    NETWORK_CHANNEL_INACTIVITY_MINUTES,
    NETWORK_CHANNEL_EXPIRATION_MINUTES,
    NETWORK_CHANNEL_GERMINATION_SECONDS,
    NETWORK_HOST_POOL_CAPACITY,
    NETWORK_RELAY_TRANSACTIONS,
    NETWORK_HOSTS_FILE,
    NETWORK_DEBUG_FILE,
    NETWORK_ERROR_FILE,
    NETWORK_SELF,
    NETWORK_BLACKLISTS,
    NETWORK_SEEDS_TESTNET
};

p2p::p2p(const settings& settings)
  : stopped_(true),
    height_(0),
    settings_(settings),
    pool_(settings_.threads),
    dispatch_(pool_),
    pending_(pool_),
    connections_(pool_),
    hosts_(pool_, settings_),
    subscriber_(std::make_shared<channel::channel_subscriber>(pool_, NAME,
        LOG_NETWORK))
{
}

p2p::~p2p()
{
    // This will block until all work ends and threads coalesce.
    stop();
}

// Properties.
// ----------------------------------------------------------------------------

// The blockchain height is set in the version message for handshake.
size_t p2p::height()
{
    return height_;
}

// The height is set externally and is safe as a naturally atomic value.
void p2p::set_height(size_t value)
{
    height_ = value;
}

// Startup processing.
// ----------------------------------------------------------------------------

void p2p::start(result_handler handler)
{
    // If we ever allow restart we need to isolate start/stop.
    if (!stopped())
        return;

    stopped_ = false;

    // This session keeps itself in scope as configured until stop.
    attach<session_inbound>();

    if (settings_.host_pool_capacity == 0)
    {
        handler(error::success);
        return;
    }

    hosts_.load(
        dispatch_.ordered_delegate(&p2p::handle_hosts_loaded,
            this, _1, handler));
}

void p2p::handle_hosts_loaded(const code& ec, result_handler handler)
{
    if (stopped())
        return;

    if (ec)
    {
        handler(ec);
        return;
    }

    const auto handle_complete =
        dispatch_.ordered_delegate(&p2p::handle_hosts_seeded,
            this, _1, handler);

    // This session keeps itself in scope as configured until complete or stop.
    attach<session_seed>(handle_complete);
}

void p2p::handle_hosts_seeded(const code& ec, result_handler handler)
{
    if (stopped())
        return;

    // This is the end of the startup cycle.
    // Inbound calls may still be accepting even if this returns failure.
    handler(ec);

    // If hosts load/seeding was successful, start outbound calls.
    // This session keeps itself in scope as configured until stop.
    if (!ec)
        attach<session_outbound>();
}

// Shutdown processing.
// ----------------------------------------------------------------------------

void p2p::stop()
{
    stopped_ = true;
    relay(error::service_stopped, nullptr);
    connections_.clear(error::service_stopped);
    pool_.shutdown();
    pool_.join();
}

void p2p::stop(result_handler handler)
{
    stopped_ = true;
    relay(error::service_stopped, nullptr);
    connections_.clear(error::service_stopped);
    hosts_.save(
        dispatch_.ordered_delegate(&p2p::handle_stop,
            this, _1, handler));
}

// This will block until all work ends and threads coalesce.
void p2p::handle_stop(const code& ec, result_handler handler)
{
    pool_.shutdown();
    pool_.join();
    handler(ec);
}

bool p2p::stopped() const
{
    return stopped_;
}

// Pending connections collection.
// ----------------------------------------------------------------------------

void p2p::pent(uint64_t version_nonce, truth_handler handler)
{
    pending_.exists(version_nonce, handler);
}

void p2p::pend(channel::ptr channel, result_handler handler)
{
    pending_.store(channel, handler);
}

void p2p::unpend(channel::ptr channel, result_handler handler)
{
    pending_.remove(channel, handler);
}

void p2p::pent_count(count_handler handler)
{
    pending_.count(handler);
}

// Connections collection.
// ----------------------------------------------------------------------------

void p2p::connected(const address& address, truth_handler handler)
{
    connections_.exists(address, handler);
}

void p2p::store(channel::ptr channel, result_handler handler)
{
    connections_.store(channel, handler);
}

void p2p::remove(channel::ptr channel, result_handler handler)
{
    connections_.remove(channel, handler);
}

void p2p::connected_count(count_handler handler)
{
    connections_.count(handler);
}

// Hosts collection.
// ----------------------------------------------------------------------------

void p2p::fetch_address(address_handler handler)
{
    hosts_.fetch_address(handler);
}

void p2p::store(const address& address, result_handler handler)
{
    hosts_.store(address, handler);
}

void p2p::store(const address::list& addresses, result_handler handler)
{
    hosts_.store(addresses, handler);
}

void p2p::remove(const address& address, result_handler handler)
{
    hosts_.remove(address, handler);
}

void p2p::address_count(count_handler handler)
{
    hosts_.count(handler);
}

// Channel management.
// ----------------------------------------------------------------------------

// This can be called without starting the network.
void p2p::connect(const std::string& hostname, uint16_t port)
{
    // This session keeps itself in scope until complete or stop.
    // For frequent connections it would be more efficient to keep the session
    // in a member and connect as necessary, but this is simpler.
    attach<session_manual>()->connect(hostname, port);
}

// This can be called without starting the network.
void p2p::connect(const std::string& hostname, uint16_t port,
    channel_handler handler)
{
    // This session keeps itself in scope until complete or stop.
    // For frequent connections it would be more efficient to keep the session
    // in a member and connect as necessary, but this is simpler.
    attach<session_manual>()->connect(hostname, port, handler);
}

void p2p::subscribe(channel_handler handler)
{
    if (stopped())
        handler(error::service_stopped, nullptr);
    else
        subscriber_->subscribe(handler);
}

void p2p::relay(const code& ec, channel::ptr channel)
{
    subscriber_->relay(ec, channel);
}

} // namespace network
} // namespace libbitcoin
