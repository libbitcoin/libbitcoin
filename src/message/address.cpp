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
#include <bitcoin/system/message/address.hpp>

#include <bitcoin/system/message/message.hpp>
#include <bitcoin/system/message/version.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

const std::string address::command = "addr";
const uint32_t address::version_minimum = version::level::minimum;
const uint32_t address::version_maximum = version::level::maximum;

address address::factory(uint32_t version, const data_chunk& data)
{
    address instance;
    instance.from_data(version, data);
    return instance;
}

address address::factory(uint32_t version, std::istream& stream)
{
    address instance;
    instance.from_data(version, stream);
    return instance;
}

address address::factory(uint32_t version, reader& source)
{
    address instance;
    instance.from_data(version, source);
    return instance;
}

address::address()
  : addresses_()
{
}

address::address(const network_address::list& addresses)
  : addresses_(addresses)
{
}

address::address(network_address::list&& addresses)
  : addresses_(std::move(addresses))
{
}

address::address(const address& other)
  : address(other.addresses_)
{
}

address::address(address&& other)
  : address(std::move(other.addresses_))
{
}

bool address::is_valid() const
{
    return !addresses_.empty();
}

void address::reset()
{
    addresses_.clear();
    addresses_.shrink_to_fit();
}

bool address::from_data(uint32_t version, const data_chunk& data)
{
    stream::in::copy istream(data);
    return from_data(version, istream);
}

bool address::from_data(uint32_t version, std::istream& stream)
{
    read::bytes::istream source(stream);
    return from_data(version, source);
}

bool address::from_data(uint32_t version, reader& source)
{
    reset();

    const auto count = source.read_size();

    // Guard against potential for arbitrary memory allocation.
    if (count > chain::max_address)
        source.invalidate();
    else
        addresses_.resize(count);

    for (auto& address: addresses_)
        if (!address.from_data(version, source, true))
            break;

    if (!source)
        reset();

    return source;
}

data_chunk address::to_data(uint32_t version) const
{
    data_chunk data;
    const auto size = serialized_size(version);
    data.reserve(size);
    stream::out::data ostream(data);
    to_data(version, ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void address::to_data(uint32_t version, std::ostream& stream) const
{
    write::bytes::ostream out(stream);
    to_data(version, out);
}

void address::to_data(uint32_t version, writer& sink) const
{
    sink.write_variable(addresses_.size());

    for (const auto& net_address: addresses_)
        net_address.to_data(version, sink, true);
}

size_t address::serialized_size(uint32_t version) const
{
    return variable_size(addresses_.size()) +
        (addresses_.size() * network_address::satoshi_fixed_size(version, true));
}

network_address::list& address::addresses()
{
    return addresses_;
}

const network_address::list& address::addresses() const
{
    return addresses_;
}

void address::set_addresses(const network_address::list& value)
{
    addresses_ = value;
}

void address::set_addresses(network_address::list&& value)
{
    addresses_ = std::move(value);
}

address& address::operator=(address&& other)
{
    addresses_ = std::move(other.addresses_);
    return *this;
}

bool address::operator==(const address& other) const
{
    return (addresses_ == other.addresses_);
}

bool address::operator!=(const address& other) const
{
    return !(*this == other);
}

} // namespace message
} // namespace system
} // namespace libbitcoin
