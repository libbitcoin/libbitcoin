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
#include <bitcoin/system/message/network_address.hpp>

#include <algorithm>
#include <cstdint>
#include <bitcoin/system/assert.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

static const ip_address null_address
{
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

// TODO: create derived address that adds the timestamp.
network_address::network_address(uint32_t timestamp, uint64_t services,
    ip_address&& ip, uint16_t port)
  : timestamp_(timestamp), services_(services), ip_(std::move(ip)), port_(port)
{
}

network_address network_address::factory(uint32_t version,
    const data_chunk& data, bool with_timestamp)
{
    network_address instance;
    instance.from_data(version, data, with_timestamp);
    return instance;
}

network_address network_address::factory(uint32_t version,
    std::istream& stream, bool with_timestamp)
{
    network_address instance;
    instance.from_data(version, stream, with_timestamp);
    return instance;
}

network_address network_address::factory(uint32_t version,
    reader& source, bool with_timestamp)
{
    network_address instance;
    instance.from_data(version, source, with_timestamp);
    return instance;
}

network_address::network_address()
  : network_address(0, 0, null_address, 0)
{
}

network_address::network_address(const network_address& other)
  : network_address(other.timestamp_, other.services_, other.ip_, other.port_)
{
}

network_address::network_address(network_address&& other)
  : network_address(other.timestamp_, other.services_,
      std::move(other.ip_), other.port_)
{
}

bool network_address::is_valid() const
{
    return (timestamp_ != 0)
        || (services_ != 0)
        || (port_ != 0)
        || (ip_ != null_address);
}

void network_address::reset()
{
    timestamp_ = 0;
    services_ = 0;
    ip_.fill(0);
    port_ = 0;
}

bool network_address::from_data(uint32_t version,
    const data_chunk& data, bool with_timestamp)
{
    stream::in::copy istream(data);
    return from_data(version, istream, with_timestamp);
}

bool network_address::from_data(uint32_t version,
    std::istream& stream, bool with_timestamp)
{
    read::bytes::istream source(stream);
    return from_data(version, source, with_timestamp);
}

bool network_address::from_data(uint32_t, reader& source,
    bool with_timestamp)
{
    reset();

    if (with_timestamp)
        timestamp_ = source.read_4_bytes_little_endian();

    services_ = source.read_8_bytes_little_endian();
    auto ip = source.read_bytes(ip_.size());
    port_ = source.read_2_bytes_big_endian();

    if (!source)
        reset();

    // TODO: add array to reader interface (can't use template).
    std::move(ip.begin(), ip.end(), ip_.data());
    return source;
}

data_chunk network_address::to_data(uint32_t version,
    bool with_timestamp) const
{
    data_chunk data;
    const auto size = serialized_size(version, with_timestamp);
    data.reserve(size);
    stream::out::data ostream(data);
    to_data(version, ostream, with_timestamp);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void network_address::to_data(uint32_t version,
    std::ostream& stream, bool with_timestamp) const
{
    write::bytes::ostream out(stream);
    to_data(version, out, with_timestamp);
}

void network_address::to_data(uint32_t ,
    writer& sink, bool with_timestamp) const
{
    if (with_timestamp)
        sink.write_4_bytes_little_endian(timestamp_);

    sink.write_8_bytes_little_endian(services_);
    sink.write_bytes(ip_.data(), ip_.size());
    sink.write_2_bytes_big_endian(port_);
}

size_t network_address::serialized_size(uint32_t version,
    bool with_timestamp) const
{
    return network_address::satoshi_fixed_size(version, with_timestamp);
}

size_t network_address::satoshi_fixed_size(uint32_t ,
    bool with_timestamp)
{
    size_t result = 26;

    if (with_timestamp)
        result += 4u;

    return result;
}

uint32_t network_address::timestamp() const
{
    return timestamp_;
}

void network_address::set_timestamp(uint32_t value)
{
    timestamp_ = value;
}

uint64_t network_address::services() const
{
    return services_;
}

void network_address::set_services(uint64_t value)
{
    services_ = value;
}

ip_address& network_address::ip()
{
    return ip_;
}

const ip_address& network_address::ip() const
{
    return ip_;
}

void network_address::set_ip(const ip_address& value)
{
    ip_ = value;
}

void network_address::set_ip(ip_address&& value)
{
    ip_ = std::move(value);
}

uint16_t network_address::port() const
{
    return port_;
}

void network_address::set_port(uint16_t value)
{
    port_ = value;
}

network_address& network_address::operator=(network_address&& other)
{
    timestamp_ = other.timestamp_;
    services_ = other.services_;
    ip_ = std::move(other.ip_);
    port_ = other.port_;
    return *this;
}

network_address& network_address::operator=(const network_address& other)
{
    timestamp_ = other.timestamp_;
    services_ = other.services_;
    ip_ = other.ip_;
    port_ = other.port_;
    return *this;
}

bool network_address::operator==(const network_address& other) const
{
    return (services_ == other.services_) && (port_ == other.port_)
        && (ip_ == other.ip_);
}

bool network_address::operator!=(const network_address& other) const
{
    return !(*this == other);
}

} // namespace message
} // namespace system
} // namespace libbitcoin
