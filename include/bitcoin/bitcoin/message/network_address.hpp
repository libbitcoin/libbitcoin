/*
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
#ifndef LIBBITCOIN_MESSAGE_NETWORK_ADDRESS_HPP
#define LIBBITCOIN_MESSAGE_NETWORK_ADDRESS_HPP

#include <cstdint>
#include <istream>
#include <vector>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/utility/data.hpp>
#include <bitcoin/bitcoin/utility/reader.hpp>
#include <bitcoin/bitcoin/utility/writer.hpp>

namespace libbitcoin {
namespace message {

typedef byte_array<16> ip_address;

class BC_API network_address
{
public:

    uint32_t timestamp;
    uint64_t services;
    ip_address ip;
    uint16_t port;

    bool from_data(const data_chunk& data, bool with_timestamp /*= true*/);

    bool from_data(std::istream& stream, bool with_timestamp /*= true*/);

    bool from_data(reader& source, bool with_timestamp /*= true*/);

    data_chunk to_data(bool with_timestamp /*= true*/) const;

    void to_data(std::ostream& stream, bool with_timestamp /*= true*/) const;

    void to_data(writer& sink, bool with_timestamp /*= true*/) const;

    bool is_valid() const;

    void reset();

    uint64_t satoshi_size(bool with_timestamp /*= false*/) const;

    static network_address factory_from_data(const data_chunk& data,
        bool with_timestamp /*= true*/);

    static network_address factory_from_data(std::istream& stream,
        bool with_timestamp /*= true*/);

    static network_address factory_from_data(reader& source,
        bool with_timestamp /*=true*/);

    static uint64_t satoshi_fixed_size(bool with_timestamp /*= false*/);
};

typedef std::vector<network_address> network_address_list;

} // end message
} // end libbitcoin

#endif
