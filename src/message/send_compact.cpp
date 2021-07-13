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
#include <bitcoin/system/message/send_compact.hpp>

#include <cstdint>
#include <bitcoin/system/assert.hpp>
#include <bitcoin/system/message/version.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

const std::string send_compact::command = "sendcmpct";
const uint32_t send_compact::version_minimum = version::level::bip152;
const uint32_t send_compact::version_maximum = version::level::bip152;

send_compact send_compact::factory(uint32_t version,
    const data_chunk& data)
{
    send_compact instance;
    instance.from_data(version, data);
    return instance;
}

send_compact send_compact::factory(uint32_t version,
    std::istream& stream)
{
    send_compact instance;
    instance.from_data(version, stream);
    return instance;
}

send_compact send_compact::factory(uint32_t version,
    reader& source)
{
    send_compact instance;
    instance.from_data(version, source);
    return instance;
}

size_t send_compact::satoshi_fixed_size(uint32_t)
{
    return 9;
}

send_compact::send_compact()
  : high_bandwidth_mode_(false), version_(0)
{
}

send_compact::send_compact(bool high_bandwidth_mode,
    uint64_t version)
  : high_bandwidth_mode_(high_bandwidth_mode),
    version_(version)
{
}

send_compact::send_compact(const send_compact& other)
  : send_compact(other.high_bandwidth_mode_, other.version_)
{
}

send_compact::send_compact(send_compact&& other)
  : send_compact(other.high_bandwidth_mode_, other.version_)
{
}

bool send_compact::is_valid() const
{
    return (version_ != 0);
}

void send_compact::reset()
{
    high_bandwidth_mode_ = false;
    version_ = 0;
}

bool send_compact::from_data(uint32_t version,
    const data_chunk& data)
{
    stream::in::copy istream(data);
    return from_data(version, istream);
}

bool send_compact::from_data(uint32_t version,
    std::istream& stream)
{
    read::bytes::istream source(stream);
    return from_data(version, source);
}

bool send_compact::from_data(uint32_t version,
    reader& source)
{
    reset();

    const auto mode = source.read_byte();

    if (mode > 1)
        source.invalidate();

    high_bandwidth_mode_ = (mode == 1);
    this->version_ = source.read_8_bytes_little_endian();

    if (version < send_compact::version_minimum)
        source.invalidate();

    if (!source)
        reset();

    return source;
}

data_chunk send_compact::to_data(uint32_t version) const
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

void send_compact::to_data(uint32_t version, std::ostream& stream) const
{
    write::bytes::ostream out(stream);
    to_data(version, out);
}

void send_compact::to_data(uint32_t, writer& sink) const
{
    sink.write_byte(static_cast<uint8_t>(high_bandwidth_mode_));
    sink.write_8_bytes_little_endian(this->version_);
}

size_t send_compact::serialized_size(uint32_t version) const
{
    return send_compact::satoshi_fixed_size(version);
}

bool send_compact::high_bandwidth_mode() const
{
    return high_bandwidth_mode_;
}

void send_compact::set_high_bandwidth_mode(bool mode)
{
    high_bandwidth_mode_ = mode;
}

uint64_t send_compact::version() const
{
    return version_;
}

void send_compact::set_version(uint64_t version)
{
    version_ = version;
}

send_compact& send_compact::operator=(send_compact&& other)
{
    high_bandwidth_mode_ = other.high_bandwidth_mode_;
    version_ = other.version_;
    return *this;
}

bool send_compact::operator==(const send_compact& other) const
{
    return (high_bandwidth_mode_ == other.high_bandwidth_mode_) &&
        (version_ == other.version_);
}

bool send_compact::operator!=(const send_compact& other) const
{
    return !(*this == other);
}

} // namespace message
} // namespace system
} // namespace libbitcoin
