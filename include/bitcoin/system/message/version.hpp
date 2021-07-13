/**
 * Copyright (c) 2011-2019 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBBITCOIN_SYSTEM_MESSAGE_VERSION_HPP
#define LIBBITCOIN_SYSTEM_MESSAGE_VERSION_HPP

#include <cstdint>
#include <istream>
#include <memory>
#include <string>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/message/network_address.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

// The checksum is ignored by the version command.
class BC_API version
{
public:
    typedef std::shared_ptr<version> ptr;
    typedef std::shared_ptr<const version> const_ptr;

    enum level: uint32_t
    {
        // compact blocks protocol
        bip152 = 70014,

        // fee_filter
        bip133 = 70013,

        // send_headers
        bip130 = 70012,

        // node_bloom service bit
        bip111 = 70011,

        // node_utxo service bit (draft)
        bip64 = 70004,

        // reject (satoshi node writes version.relay starting here)
        bip61 = 70002,

        // filters, merkle_block, not_found, version.relay
        bip37 = 70001,

        // memory_pool
        bip35 = 60002,

        // ping.nonce, pong
        bip31 = 60001,

        // Don't request blocks from nodes of versions 32000-32400.
        no_blocks_end = 32400,

        // Don't request blocks from nodes of versions 32000-32400.
        no_blocks_start = 32000,

        // This preceded the BIP system.
        headers = 31800,

        // We require at least this of peers, address.time fields.
        minimum = 31402,

        // We support at most this internally (bound to settings default).
        maximum = bip152,

        // Used to generate canonical size required by consensus checks.
        canonical = 0
    };

    enum service: uint64_t
    {
        // The node exposes no services.
        none = 0,

        // The node is capable of serving the block chain (full node).
        node_network = (1u << 0),

        // Requires version.value >= level::bip64 (BIP64 is draft only).
        // The node is capable of responding to the getutxo protocol request.
        node_utxo = (1u << 1),

        // Requires version.value >= level::bip111
        // The node is capable and willing to handle bloom-filtered connections.
        node_bloom = (1u << 2),

        // Independent of network protocol level.
        // The node is capable of responding to witness inventory requests.
        node_witness = (1u << 3),

        // The node is capable of responding to getcfilters, getcfheaders,
        // and getcfcheckpt protocol requests.
        node_compact_filters = (1u << 6)
    };

    static version factory(uint32_t version, const data_chunk& data);
    static version factory(uint32_t version, std::istream& stream);
    static version factory(uint32_t version, reader& source);

    version();
    version(uint32_t value, uint64_t services, uint64_t timestamp,
        const network_address& address_receiver,
        const network_address& address_sender,
        uint64_t nonce, const std::string& user_agent, uint32_t start_height,
        bool relay);
    version(uint32_t value, uint64_t services, uint64_t timestamp,
        network_address&& address_receiver, network_address&& address_sender,
        uint64_t nonce, std::string&& user_agent, uint32_t start_height,
        bool relay);
    version(const version& other);
    version(version&& other);

    uint32_t value() const;
    void set_value(uint32_t value);

    uint64_t services() const;
    void set_services(uint64_t services);

    uint64_t timestamp() const;
    void set_timestamp(uint64_t timestamp);

    network_address& address_receiver();
    const network_address& address_receiver() const;
//    void set_address_receiver(const network_address& address);
    void set_address_receiver(network_address&& address);

    network_address& address_sender();
    const network_address& address_sender() const;
//    void set_address_sender(const network_address& address);
    void set_address_sender(network_address&& address);

    uint64_t nonce() const;
    void set_nonce(uint64_t nonce);

    std::string& user_agent();
    const std::string& user_agent() const;
    void set_user_agent(const std::string& agent);
    void set_user_agent(std::string&& agent);

    uint32_t start_height() const;
    void set_start_height(uint32_t height);

    // version >= 70001
    bool relay() const;
    void set_relay(bool relay);

    bool from_data(uint32_t version, const data_chunk& data);
    bool from_data(uint32_t version, std::istream& stream);
    bool from_data(uint32_t version, reader& source);
    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, std::ostream& stream) const;
    void to_data(uint32_t version, writer& sink) const;
    bool is_valid() const;
    void reset();
    size_t serialized_size(uint32_t version) const;

    // This class is move assignable but not copy assignable.
    version& operator=(version&& other);
    void operator=(const version&) = delete;

    bool operator==(const version& other) const;
    bool operator!=(const version& other) const;

    static const std::string command;
//    static const bounds version;
    static const uint32_t version_minimum;
    static const uint32_t version_maximum;

private:
    uint32_t value_;
    uint64_t services_;
    uint64_t timestamp_;
    network_address address_receiver_;
    network_address address_sender_;
    uint64_t nonce_;
    std::string user_agent_;
    uint32_t start_height_;

    // version >= 70001
    bool relay_;
};

} // namespace message
} // namespace system
} // namespace libbitcoin

#endif
