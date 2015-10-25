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
#include <bitcoin/bitcoin/network/proxy.hpp>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <memory>
#include <boost/date_time.hpp>
#include <boost/format.hpp>
#include <boost/iostreams/stream.hpp>
#include <bitcoin/bitcoin/config/authority.hpp>
#include <bitcoin/bitcoin/error.hpp>
#include <bitcoin/bitcoin/math/checksum.hpp>
#include <bitcoin/bitcoin/messages.hpp>
#include <bitcoin/bitcoin/network/asio.hpp>
#include <bitcoin/bitcoin/network/message_subscriber.hpp>
#include <bitcoin/bitcoin/network/shared_const_buffer.hpp>
#include <bitcoin/bitcoin/utility/assert.hpp>
#include <bitcoin/bitcoin/utility/container_source.hpp>
#include <bitcoin/bitcoin/utility/data.hpp>
#include <bitcoin/bitcoin/utility/deadline.hpp>
#include <bitcoin/bitcoin/utility/endian.hpp>
#include <bitcoin/bitcoin/utility/logger.hpp>
#include <bitcoin/bitcoin/utility/random.hpp>
#include <bitcoin/bitcoin/utility/dispatcher.hpp>
#include <bitcoin/bitcoin/utility/serializer.hpp>
#include <bitcoin/bitcoin/utility/threadpool.hpp>

// This must be declared in the global namespace.
INITIALIZE_TRACK(bc::network::proxy::stop_subscriber)

namespace libbitcoin {
namespace network {

using namespace message;
using std::placeholders::_1;
using std::placeholders::_2;
using boost::format;
using boost::posix_time::time_duration;

// TODO: this is made-up, configure payload size guard for DoS protection.
static constexpr size_t max_payload_size = 10 * 1024 * 1024;

proxy::proxy(threadpool& pool, asio::socket_ptr socket, uint32_t magic)
  : stopped_(true),
    magic_(magic),
    dispatch_(pool),
    socket_(socket),
    message_subscriber_(pool),
    stop_subscriber_(std::make_shared<stop_subscriber>(pool, "stop_subscriber", LOG_NETWORK))
{
    // Cache the address for logging after stop.
    boost_code ec;
    const auto endpoint = socket_->remote_endpoint(ec);
    if (!ec)
        authority_ = config::authority(endpoint);
}

proxy::~proxy()
{
    stop(error::channel_stopped);
}

void proxy::start()
{
    // If we ever allow restart we need to isolate start (stop is already).
    if (!stopped())
        return;

    stopped_ = false;
    read_heading();
}

config::authority proxy::address() const
{
    return authority_;
}

bool proxy::stopped() const
{
    return stopped_;
}

void proxy::stop(const boost_code& ec)
{
    stop(error::boost_to_error_code(ec));
}

void proxy::stop(const code& ec)
{
    BITCOIN_ASSERT_MSG(ec, "The stop code must be an error code.");

    if (stopped())
        return;

    dispatch_.ordered(&proxy::do_stop,
        shared_from_this(), ec);
}

void proxy::subscribe_stop(stop_handler handler)
{
    if (stopped())
        handler(error::channel_stopped);
    else
        stop_subscriber_->subscribe(handler);
}

void proxy::do_stop(const code& ec)
{
    if (stopped())
        return;

    stopped_ = true;
    handle_stopping();

    // Close the socket, ignore the error code.
    boost_code ignore;
    socket_->shutdown(asio::socket::shutdown_both, ignore);
    socket_->close(ignore);

    // All message subscribers relay the channel stop code.
    // This results in all message subscriptions fired with the same code.
    message_subscriber_.broadcast(error::channel_stopped);

    // All stop subscriptions are fired with the channel stop reason code.
    stop_subscriber_->relay(ec);
}

void proxy::read_heading()
{
    if (stopped())
        return;

    using namespace boost::asio;
    async_read(*socket_, buffer(heading_buffer_),
        dispatch_.ordered_delegate(&proxy::handle_read_heading,
            shared_from_this(), _1, _2));
}

void proxy::read_payload(const heading& head)
{
    if (stopped())
        return;

    payload_buffer_.resize(head.payload_size);

    using namespace boost::asio;
    async_read(*socket_, buffer(payload_buffer_, head.payload_size),
        dispatch_.ordered_delegate(&proxy::handle_read_payload,
            shared_from_this(), _1, _2, head));
}

void proxy::handle_read_heading(const boost_code& ec, size_t)
{
    if (stopped())
        return;

    if (ec)
    {
        log_debug(LOG_NETWORK)
            << "Channel failure [" << address() << "] "
            << code(error::boost_to_error_code(ec)).message();
        stop(ec);
        return;
    }

    heading head;
    heading_stream istream(heading_buffer_);
    const auto parsed = head.from_data(istream);
    if (!parsed || head.magic != magic_)
    {
        log_warning(LOG_NETWORK) 
            << "Invalid heading received [" << address() << "]";
        stop(error::bad_stream);
        return;
    }

    if (head.payload_size > max_payload_size)
    {
        log_warning(LOG_NETWORK)
            << "Oversized payload indicated [" << address() << "] ("
            << head.payload_size << " bytes)";
        stop(error::bad_stream);
        return;
    }

    log_debug(LOG_NETWORK)
        << "Receive " << head.command << " [" << address() << "] ("
        << head.payload_size << " bytes)";

    read_payload(head);
    handle_activity();
}

void proxy::handle_read_payload(const boost_code& ec, size_t,
    const heading& heading)
{
    if (stopped())
        return;

    // Ignore read error here, client may have disconnected.

    if (heading.checksum != bitcoin_checksum(payload_buffer_))
    {
        log_warning(LOG_NETWORK) 
            << "Invalid bitcoin checksum from [" << address() << "]";
        stop(error::bad_stream);
        return;
    }

    // We must copy the payload before restarting the reader.
    const auto payload_copy = payload_buffer_;

    // We must restart the reader before firing subscription events.
    if (!ec)
        read_heading();

    handle_activity();

    // Parse and publish the payload to message subscribers.
    payload_source source(payload_copy);
    payload_stream istream(source);
    const auto error = message_subscriber_.load(heading.type(), istream);

    // Warn about unconsumed bytes in the stream.
    if (!error && istream.peek() != std::istream::traits_type::eof())
        log_warning(LOG_NETWORK)
            << "Valid message [" << heading.command
            << "] handled, unused bytes remain in payload.";

    // Stop the channel if there was a read error.
    if (ec)
    {
        log_warning(LOG_NETWORK)
            << "Invalid payload of " << heading.command
            << " from [" << address() << "] (deferred)"
            << code(error::boost_to_error_code(ec)).message();
        stop(ec);
        return;
    }

    // Stop the channel if there was an error from parse.
    if (error)
    {
        log_warning(LOG_NETWORK)
            << "Invalid stream load of " << heading.command
            << " from [" << address() << "] " << error.message();
        stop(error);
    }
}

void proxy::do_send(const data_chunk& message, send_handler handler,
    const std::string& command)
{
    if (stopped())
    {
        handler(error::channel_stopped);
        return;
    }

    log_debug(LOG_NETWORK)
        << "Send " << command << " [" << address() << "] ("
        << message.size() << " bytes)";

    const shared_const_buffer buffer(message);
    async_write(*socket_, buffer,
        std::bind(&proxy::call_handle_send,
            shared_from_this(), _1, handler));
}

void proxy::call_handle_send(const boost_code& ec, send_handler handler)
{
    handler(error::boost_to_error_code(ec));
}

} // namespace network
} // namespace libbitcoin
