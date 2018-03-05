/**
 * Copyright (c) 2011-2016 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see COPYING.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <bitcoin/bitcoin/network/channel.hpp>

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <mutex>
#include <bitcoin/bitcoin/config/authority.hpp>
#include <bitcoin/bitcoin/network/channel_proxy.hpp>
#include <bitcoin/bitcoin/primitives.hpp>

namespace libbitcoin {
namespace network {
    
// Leak tracking.
static std::atomic<size_t> instances_(0);

channel::channel(channel_proxy_ptr proxy)
  : proxy_(proxy),
    nonce_(0),
    own_threshold_(null_hash),
    peer_threshold_(null_hash)
{
    const auto count = ++instances_;

    // Leak tracking.
    log_debug(LOG_NETWORK)
        << "Opened a channel and (" << count << ") are now open";
}

channel::channel(threadpool& pool, socket_ptr socket, const timeout& timeouts)
  : channel(std::make_shared<channel_proxy>(pool, socket, timeouts))
{
}

channel::~channel()
{
    // A proxy reference may be held externally, so ensure the proxy is closed.
    proxy_->stop(error::channel_stopped);

    const auto count = --instances_;

    // Leak tracking.
    log_debug(LOG_NETWORK)
        << "Closed a channel and (" << count << ") remain open";
}

void channel::start()
{
    proxy_->start();
}

void channel::stop(const std::error_code& ec)
{
    proxy_->stop(ec);
}

config::authority channel::address() const
{
    return proxy_->address();
}

// TODO: make private, pass on notfy.
uint64_t channel::nonce() const
{
    return nonce_.load();
}

void channel::set_nonce(uint64_t nonce)
{
    nonce_.store(nonce);
}

hash_digest channel::own_threshold()
{
    // Critical Section
    ///////////////////////////////////////////////////////////////////////////
    std::lock_guard<std::mutex> lock(own_threshold_mutex_);
    return own_threshold_;
    ///////////////////////////////////////////////////////////////////////////
}

void channel::set_own_threshold(const hash_digest& threshold)
{
    // Critical Section
    ///////////////////////////////////////////////////////////////////////////
    std::lock_guard<std::mutex> lock(own_threshold_mutex_);
    own_threshold_ = threshold;
    ///////////////////////////////////////////////////////////////////////////
}

hash_digest channel::peer_threshold()
{
    // Critical Section
    ///////////////////////////////////////////////////////////////////////////
    std::lock_guard<std::mutex> lock(peer_threshold_mutex_);
    return peer_threshold_;
    ///////////////////////////////////////////////////////////////////////////
}

void channel::set_peer_threshold(const hash_digest& threshold)
{
    // Critical Section
    ///////////////////////////////////////////////////////////////////////////
    std::lock_guard<std::mutex> lock(peer_threshold_mutex_);
    peer_threshold_ = threshold;
    ///////////////////////////////////////////////////////////////////////////
}

void channel::reset_poll()
{
    return proxy_->reset_poll();
}

void channel::reset_sync()
{
    return proxy_->reset_sync();
}

void channel::set_poll_handler(channel_proxy::poll_handler handler)
{
    return proxy_->set_poll_handler(handler);
}

void channel::set_sync_handler(channel_proxy::sync_handler handler)
{
    return proxy_->set_sync_handler(handler);
}

void channel::subscribe_version(
    channel_proxy::receive_version_handler handle_receive)
{
    proxy_->subscribe_version(handle_receive);
}

void channel::subscribe_verack(
    channel_proxy::receive_verack_handler handle_receive)
{
    proxy_->subscribe_verack(handle_receive);
}

void channel::subscribe_address(
    channel_proxy::receive_address_handler handle_receive)
{
    proxy_->subscribe_address(handle_receive);
}

void channel::subscribe_get_address(
    channel_proxy::receive_get_address_handler handle_receive)
{
    proxy_->subscribe_get_address(handle_receive);
}

void channel::subscribe_inventory(
    channel_proxy::receive_inventory_handler handle_receive)
{
    proxy_->subscribe_inventory(handle_receive);
}

void channel::subscribe_get_data(
    channel_proxy::receive_get_data_handler handle_receive)
{
    proxy_->subscribe_get_data(handle_receive);
}

void channel::subscribe_get_blocks(
    channel_proxy::receive_get_blocks_handler handle_receive)
{
    proxy_->subscribe_get_blocks(handle_receive);
}

void channel::subscribe_transaction(
    channel_proxy::receive_transaction_handler handle_receive)
{
    proxy_->subscribe_transaction(handle_receive);
}

void channel::subscribe_block(
    channel_proxy::receive_block_handler handle_receive)
{
    proxy_->subscribe_block(handle_receive);
}

void channel::subscribe_ping(
    channel_proxy::receive_ping_handler handle_receive)
{
    proxy_->subscribe_ping(handle_receive);
}

void channel::subscribe_pong(
    channel_proxy::receive_pong_handler handle_receive)
{
    proxy_->subscribe_pong(handle_receive);
}

void channel::subscribe_raw(
    channel_proxy::receive_raw_handler handle_receive)
{
    proxy_->subscribe_raw(handle_receive);
}

void channel::subscribe_stop(
    channel_proxy::stop_handler handle_stop)
{
    proxy_->subscribe_stop(handle_stop);
}

void channel::send_raw(const header_type& packet_header,
    const data_chunk& payload, channel_proxy::send_handler handle_send)
{
    proxy_->send_raw(packet_header, payload, handle_send);
}

} // namespace network
} // namespace libbitcoin