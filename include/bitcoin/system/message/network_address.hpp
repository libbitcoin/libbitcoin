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
#ifndef LIBBITCOIN_SYSTEM_MESSAGE_NETWORK_ADDRESS_HPP
#define LIBBITCOIN_SYSTEM_MESSAGE_NETWORK_ADDRESS_HPP

#include <cstdint>
#include <istream>
#include <vector>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

typedef data_array<16> ip_address;

class BC_API network_address
{
public:
    typedef std::vector<network_address> list;

    static network_address factory(uint32_t version, const data_chunk& data,
        bool with_timestamp);
    static network_address factory(uint32_t version, std::istream& stream,
        bool with_timestamp);
    static network_address factory(uint32_t version, reader& source,
        bool with_timestamp);
    static size_t satoshi_fixed_size(uint32_t version, bool with_timestamp);

    network_address();

    // constexpr required for declaration of constexpr address types.
    constexpr network_address(uint32_t timestamp, uint64_t services,
        const ip_address& ip, uint16_t port)
      : timestamp_(timestamp), services_(services), ip_(ip), port_(port)
    {
    }

    network_address(uint32_t timestamp, uint64_t services, ip_address&& ip,
        uint16_t port);

    network_address(const network_address& other);
    network_address(network_address&& other);

    // Starting version 31402, addresses are prefixed with a timestamp.
    uint32_t timestamp() const;
    void set_timestamp(uint32_t value);

    uint64_t services() const;
    void set_services(uint64_t value);

    ip_address& ip();
    const ip_address& ip() const;
    void set_ip(const ip_address& value);
    void set_ip(ip_address&& value);

    uint16_t port() const;
    void set_port(uint16_t value);

    bool from_data(uint32_t version, const data_chunk& data,
        bool with_timestamp);
    bool from_data(uint32_t version, std::istream& stream,
        bool with_timestamp);
    bool from_data(uint32_t version, reader& source, bool with_timestamp);
    data_chunk to_data(uint32_t version, bool with_timestamp) const;
    void to_data(uint32_t version, std::ostream& stream,
        bool with_timestamp) const;
    void to_data(uint32_t version, writer& sink, bool with_timestamp) const;
    bool is_valid() const;
    void reset();
    size_t serialized_size(uint32_t version, bool with_timestamp) const;

    network_address& operator=(network_address&& other);
    network_address& operator=(const network_address& other);

    bool operator==(const network_address& other) const;
    bool operator!=(const network_address& other) const;

private:
    uint32_t timestamp_;
    uint64_t services_;
    ip_address ip_;
    uint16_t port_;
};

// version::services::none
constexpr uint32_t no_services = 0;
constexpr uint32_t no_timestamp = 0;
constexpr uint16_t unspecified_ip_port = 0;
constexpr ip_address unspecified_ip_address
{
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00
    }
};

// Defaults to full node services.
constexpr network_address unspecified_network_address
{
    no_timestamp,
    no_services,
    unspecified_ip_address,
    unspecified_ip_port
};

} // namespace message
} // namespace system
} // namespace libbitcoin

#endif
