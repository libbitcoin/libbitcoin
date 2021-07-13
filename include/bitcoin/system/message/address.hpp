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
#ifndef LIBBITCOIN_SYSTEM_MESSAGE_ADDRESS_HPP
#define LIBBITCOIN_SYSTEM_MESSAGE_ADDRESS_HPP

#include <istream>
#include <memory>
#include <string>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/message/network_address.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

class BC_API address
{
public:
    typedef std::shared_ptr<address> ptr;
    typedef std::shared_ptr<const address> const_ptr;

    static address factory(uint32_t version, const data_chunk& data);
    static address factory(uint32_t version, std::istream& stream);
    static address factory(uint32_t version, reader& source);

    address();
    address(const network_address::list& addresses);
    address(network_address::list&& addresses);
    address(const address& other);
    address(address&& other);

    network_address::list& addresses();
    const network_address::list& addresses() const;
    void set_addresses(const network_address::list& value);
    void set_addresses(network_address::list&& value);

    bool from_data(uint32_t version, const data_chunk& data);
    bool from_data(uint32_t version, std::istream& stream);
    bool from_data(uint32_t version, reader& source);
    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, std::ostream& stream) const;
    void to_data(uint32_t version, writer& sink) const;
    bool is_valid() const;
    void reset();
    size_t serialized_size(uint32_t version) const;

    /// This class is move assignable but not copy assignable.
    address& operator=(address&& other);
    void operator=(const address&) = delete;

    bool operator==(const address& other) const;
    bool operator!=(const address& other) const;

    static const std::string command;
    static const uint32_t version_minimum;
    static const uint32_t version_maximum;

private:
    network_address::list addresses_;
};

} // namespace message
} // namespace system
} // namespace libbitcoin

#endif
