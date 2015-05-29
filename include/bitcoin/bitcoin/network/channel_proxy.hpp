/**
 * Copyright (c) 2011-2013 libbitcoin developers (see AUTHORS)
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
#ifndef LIBBITCOIN_CHANNEL_PROXY_HPP
#define LIBBITCOIN_CHANNEL_PROXY_HPP

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <system_error>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/date_time.hpp>
#include <bitcoin/bitcoin/compat.hpp>
#include <bitcoin/bitcoin/constants.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/chain/block.hpp>
#include <bitcoin/bitcoin/math/checksum.hpp>
#include <bitcoin/bitcoin/message/announce_version.hpp>
#include <bitcoin/bitcoin/message/address.hpp>
#include <bitcoin/bitcoin/message/get_address.hpp>
#include <bitcoin/bitcoin/message/get_blocks.hpp>
#include <bitcoin/bitcoin/message/get_data.hpp>
#include <bitcoin/bitcoin/message/header.hpp>
#include <bitcoin/bitcoin/message/inventory.hpp>
#include <bitcoin/bitcoin/message/verack.hpp>
#include <bitcoin/bitcoin/network/channel_stream_loader.hpp>
#include <bitcoin/bitcoin/utility/async_strand.hpp>
#include <bitcoin/bitcoin/utility/data.hpp>
#include <bitcoin/bitcoin/utility/logger.hpp>
#include <bitcoin/bitcoin/utility/threadpool.hpp>
#include <bitcoin/bitcoin/utility/subscriber.hpp>

namespace libbitcoin {
namespace network {

// List of bitcoin messages
// version
// verack
// addr
// getaddr
// inv
// getdata
// getblocks
// tx
// block
// getheaders   [unused]
// headers      [unused]
// checkorder   [deprecated]
// submitorder  [deprecated]
// reply        [deprecated]
// ping         [internal]
// alert        [not supported]

// Defined here because of the central position in the dependency graph.
typedef std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

// TODO: move to serializer|misc|message.hpp/ipp.
template <typename Message>
data_chunk create_raw_message(const Message& packet)
{
    data_chunk payload = packet.to_data();

    // Make the header packet and serialise it
    message::header head {
        magic_value(),
        Message::satoshi_command,
        static_cast<uint32_t>(payload.size()),
        bitcoin_checksum(payload)
    };

    data_chunk raw_header = head.to_data();

    // Construct completed packet with header + payload
    data_chunk whole_message = raw_header;
    extend_data(whole_message, payload);

    // Probably not the right place for this
    // Networking output in an exporter
    log_debug(LOG_NETWORK) << "s: " << head.command
        << " (" << payload.size() << " bytes)";

    return whole_message;
}

class BC_API channel_proxy
  : public std::enable_shared_from_this<channel_proxy>
{
public:
    typedef std::shared_ptr<channel_proxy> pointer;

    typedef std::function<void (const std::error_code&)> send_handler;

    typedef std::function<void (const std::error_code&,
        const message::announce_version&)> receive_version_handler;

    typedef std::function<void (const std::error_code&,
        const message::verack&)> receive_verack_handler;

    typedef std::function<void (const std::error_code&,
        const message::address&)> receive_address_handler;

    typedef std::function<void (const std::error_code&,
        const message::get_address&)> receive_get_address_handler;

    typedef std::function<void (const std::error_code&,
        const message::inventory&)> receive_inventory_handler;

    typedef std::function<void (const std::error_code&,
        const message::get_data&)> receive_get_data_handler;

    typedef std::function<void (const std::error_code&,
        const message::get_blocks&)> receive_get_blocks_handler;

    typedef std::function<void (const std::error_code&,
        const chain::transaction&)> receive_transaction_handler;

    typedef std::function<void (const std::error_code&,
        const chain::block&)> receive_block_handler;

    typedef std::function<void (const std::error_code&,
        const message::header&, const data_chunk&)> receive_raw_handler;

    typedef std::function<void (const std::error_code&)> stop_handler;

    channel_proxy(threadpool& pool, socket_ptr socket);
    ~channel_proxy();

    channel_proxy(const channel_proxy&) = delete;
    void operator=(const channel_proxy&) = delete;

    void start();
    void stop();
    bool stopped() const;

    template <typename Message>
    void send(const Message& packet, send_handler handle_send)
    {
        send_common(create_raw_message(packet), handle_send);
    }

    BC_API void send_raw(const message::header& packet_header,
        const data_chunk& payload, send_handler handle_send);

    void send_common(const data_chunk& whole_message,
        send_handler handle_send);

    void subscribe_version(receive_version_handler handle_receive);
    void subscribe_verack(receive_verack_handler handle_receive);
    void subscribe_address(receive_address_handler handle_receive);
    void subscribe_get_address(
        receive_get_address_handler handle_receive);
    void subscribe_inventory(receive_inventory_handler handle_receive);
    void subscribe_get_data(receive_get_data_handler handle_receive);
    void subscribe_get_blocks(
        receive_get_blocks_handler handle_receive);
    void subscribe_transaction(
        receive_transaction_handler handle_receive);
    void subscribe_block(receive_block_handler handle_receive);
    void subscribe_raw(receive_raw_handler handle_receive);

    void subscribe_stop(stop_handler handle_stop);

private:
    typedef subscriber<const std::error_code&, const message::announce_version&>
        version_subscriber_type;
    typedef subscriber<const std::error_code&, const message::verack&>
        verack_subscriber_type;
    typedef subscriber<const std::error_code&, const message::address&>
        address_subscriber_type;
    typedef subscriber<const std::error_code&, const message::get_address&>
        get_address_subscriber_type;
    typedef subscriber<const std::error_code&, const message::inventory&>
        inventory_subscriber_type;
    typedef subscriber<const std::error_code&, const message::get_data&>
        get_data_subscriber_type;
    typedef subscriber<const std::error_code&, const message::get_blocks&>
        get_blocks_subscriber_type;
    typedef subscriber<const std::error_code&, const chain::transaction&>
        transaction_subscriber_type;
    typedef subscriber<const std::error_code&, const chain::block&>
        block_subscriber_type;

    typedef subscriber<const std::error_code&,
        const message::header&, const data_chunk&> raw_subscriber_type;
    typedef subscriber<const std::error_code&> stop_subscriber_type;

    void do_send_raw(const message::header& packet_header,
        const data_chunk& payload, send_handler handle_send);
    void do_send_common(const data_chunk& whole_message,
        send_handler handle_send);

    template <typename Message, typename Callback, typename SubscriberPtr>
    void generic_subscribe(Callback handle_message,
        SubscriberPtr message_subscribe)
    {
        // Subscribing must be immediate. We cannot switch thread contexts
        if (stopped_)
            handle_message(error::service_stopped, Message());
        else
            message_subscribe->subscribe(handle_message);
    }

    void read_header();
    void read_checksum(const message::header& header_msg);
    void read_payload(const message::header& header_msg);

    void handle_read_header(const boost::system::error_code& ec,
        size_t bytes_transferred);
    void handle_read_checksum(const boost::system::error_code& ec,
        size_t bytes_transferred, message::header& header_msg);
    void handle_read_payload(const boost::system::error_code& ec,
        size_t bytes_transferred, const message::header& header_msg);

    // Calls the send handler after a successful send, translating
    // the boost error_code to std::error_code
    void call_handle_send(const boost::system::error_code& ec,
        send_handler handle_send);

    void handle_timeout(const boost::system::error_code& ec);
    void handle_heartbeat(const boost::system::error_code& ec);

    void set_timeout(const boost::posix_time::time_duration timeout);
    void set_heartbeat(const boost::posix_time::time_duration timeout);
    void reset_timers();

    bool problems_check(const boost::system::error_code& ec);
    void stop_impl();
    void clear_subscriptions();

    async_strand strand_;
    socket_ptr socket_;

    // We keep the service alive for lifetime rules
    boost::asio::deadline_timer timeout_, heartbeat_;
    std::atomic<bool> stopped_;

    channel_stream_loader loader_;

    // Header minus checksum is 4 + 12 + 4 = 20 bytes
    static BC_CONSTEXPR size_t header_chunk_size = 20;
    static BC_CONSTEXPR size_t header_checksum_size = 4;

    // boost1.54/linux/clang/libstdc++-4.8 error if std::array
    // could not match 'boost::array' against 'std::array'
    boost::array<uint8_t, header_chunk_size> inbound_header_;
    boost::array<uint8_t, header_checksum_size> inbound_checksum_;

    std::vector<uint8_t> inbound_payload_;

    // We should be using variadic templates for these
    version_subscriber_type::ptr version_subscriber_;
    verack_subscriber_type::ptr verack_subscriber_;
    address_subscriber_type::ptr address_subscriber_;
    get_address_subscriber_type::ptr get_address_subscriber_;
    inventory_subscriber_type::ptr inventory_subscriber_;
    get_data_subscriber_type::ptr get_data_subscriber_;
    get_blocks_subscriber_type::ptr get_blocks_subscriber_;
    transaction_subscriber_type::ptr transaction_subscriber_;
    block_subscriber_type::ptr block_subscriber_;

    raw_subscriber_type::ptr raw_subscriber_;
    stop_subscriber_type::ptr stop_subscriber_;
};

} // namespace network
} // namespace libbitcoin

#endif

