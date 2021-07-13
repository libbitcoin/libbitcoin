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
#include <bitcoin/system/message/pong.hpp>

#include <bitcoin/system/assert.hpp>
#include <bitcoin/system/message/version.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

const std::string pong::command = "pong";
const uint32_t pong::version_minimum = version::level::minimum;
const uint32_t pong::version_maximum = version::level::maximum;

pong pong::factory(uint32_t version, const data_chunk& data)
{
    pong instance;
    instance.from_data(version, data);
    return instance;
}

pong pong::factory(uint32_t version, std::istream& stream)
{
    pong instance;
    instance.from_data(version, stream);
    return instance;
}

pong pong::factory(uint32_t version, reader& source)
{
    pong instance;
    instance.from_data(version, source);
    return instance;
}

size_t pong::satoshi_fixed_size(uint32_t)
{
    return sizeof(nonce_);
}

pong::pong()
  : nonce_(0), valid_(false)
{
}

pong::pong(uint64_t nonce)
  : nonce_(nonce), valid_(true)
{
}

pong::pong(const pong& other)
  : nonce_(other.nonce_), valid_(other.valid_)
{
}

bool pong::from_data(uint32_t version, const data_chunk& data)
{
    stream::in::copy istream(data);
    return from_data(version, istream);
}

bool pong::from_data(uint32_t version, std::istream& stream)
{
    read::bytes::istream source(stream);
    return from_data(version, source);
}

bool pong::from_data(uint32_t, reader& source)
{
    reset();

    valid_ = true;
    nonce_ = source.read_8_bytes_little_endian();

    if (!source)
        reset();

    return source;
}

data_chunk pong::to_data(uint32_t version) const
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

void pong::to_data(uint32_t version, std::ostream& stream) const
{
    write::bytes::ostream out(stream);
    to_data(version, out);
}

void pong::to_data(uint32_t, writer& sink) const
{
    sink.write_8_bytes_little_endian(nonce_);
}

bool pong::is_valid() const
{
    return valid_ || (nonce_ != 0);
}

void pong::reset()
{
    nonce_ = 0;
    valid_ = false;
}

size_t pong::serialized_size(uint32_t version) const
{
    return satoshi_fixed_size(version);
}

uint64_t pong::nonce() const
{
    return nonce_;
}

void pong::set_nonce(uint64_t value)
{
    nonce_ = value;
}

pong& pong::operator=(pong&& other)
{
    nonce_ = other.nonce_;
    return *this;
}

bool pong::operator==(const pong& other) const
{
    return (nonce_ == other.nonce_);
}

bool pong::operator!=(const pong& other) const
{
    return !(*this == other);
}

} // namespace message
} // namespace system
} // namespace libbitcoin
