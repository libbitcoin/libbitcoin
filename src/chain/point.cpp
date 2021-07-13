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
#include <bitcoin/system/chain/point.hpp>

#include <cstdint>
#include <utility>
#include <bitcoin/system/assert.hpp>
#include <bitcoin/system/chain/enums/magic_numbers.hpp>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace chain {

// This sentinel is serialized and defined by consensus, not implementation.
const uint32_t point::null_index = no_previous_output;

// Constructors.
//-----------------------------------------------------------------------------

// A default instance is invalid (until modified).
point::point()
  : hash_(null_hash),
    index_(0),
    valid_(false)
{
}

point::point(const hash_digest& hash, uint32_t index)
  : hash_(hash),
    index_(index),
    valid_(true)
{
}

point::point(hash_digest&& hash, uint32_t index)
  : hash_(std::move(hash)),
    index_(index),
    valid_(true)
{
}

// protected
point::point(const hash_digest& hash, uint32_t index, bool valid)
  : hash_(hash),
    index_(index),
    valid_(valid)
{
}

// protected
point::point(hash_digest&& hash, uint32_t index, bool valid)
  : hash_(std::move(hash)),
    index_(index),
    valid_(valid)
{
}

point::point(const point& other)
  : point(other.hash_, other.index_, other.valid_)
{
}

point::point(point&& other)
  : point(std::move(other.hash_), other.index_, other.valid_)
{
}

// Operators.
//-----------------------------------------------------------------------------

point& point::operator=(point&& other)
{
    hash_ = std::move(other.hash_);
    index_ = other.index_;
    return *this;
}

point& point::operator=(const point& other)
{
    hash_ = other.hash_;
    index_ = other.index_;
    return *this;
}

// This arbitrary order is produced to support set uniqueness determinations.
bool point::operator<(const point& other) const
{
    // The index is primary only because its comparisons are simpler.
    return index_ == other.index_ ? hash_ < other.hash_ :
        index_ < other.index_;
}

bool point::operator==(const point& other) const
{
    return (hash_ == other.hash_) && (index_ == other.index_);
}

bool point::operator!=(const point& other) const
{
    return !(*this == other);
}

// Deserialization.
//-----------------------------------------------------------------------------

// static
point point::factory(const data_chunk& data, bool wire)
{
    point instance;
    instance.from_data(data, wire);
    return instance;
}

// static
point point::factory(std::istream& stream, bool wire)
{
    point instance;
    instance.from_data(stream, wire);
    return instance;
}

// static
point point::factory(reader& source, bool wire)
{
    point instance;
    instance.from_data(source, wire);
    return instance;
}

bool point::from_data(const data_chunk& data, bool wire)
{
    stream::in::copy istream(data);
    return from_data(istream, wire);
}

bool point::from_data(std::istream& stream, bool wire)
{
    read::bytes::istream source(stream);
    return from_data(source, wire);
}

bool point::from_data(reader& source, bool wire)
{
    reset();

    valid_ = true;
    hash_ = source.read_hash();

    if (wire)
    {
        index_ = source.read_4_bytes_little_endian();
    }
    else
    {
        index_ = source.read_2_bytes_little_endian();

        // Convert 16 bit sentinel to 32 bit sentinel.
        if (index_ == max_uint16)
            index_ = null_index;
    }

    if (!source)
        reset();

    return source;
}

// protected
void point::reset()
{
    valid_ = false;
    hash_ = null_hash;
    index_ = 0;
}

bool point::is_valid() const
{
    return valid_;
}

// Serialization.
//-----------------------------------------------------------------------------

data_chunk point::to_data(bool wire) const
{
    data_chunk data;
    const auto size = serialized_size(wire);
    data.reserve(size);
    stream::out::data ostream(data);
    to_data(ostream, wire);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void point::to_data(std::ostream& stream, bool wire) const
{
    write::bytes::ostream out(stream);
    to_data(out, wire);
}

void point::to_data(writer& sink, bool wire) const
{
    sink.write_bytes(hash_);

    if (wire)
    {
        sink.write_4_bytes_little_endian(index_);
    }
    else
    {
        BITCOIN_ASSERT(index_ == null_index || index_ < max_uint16);

        // Convert 32 bit sentinel to 16 bit sentinel.
        const auto index = (index_ == null_index) ? max_uint16 :
            static_cast<uint16_t>(index_);

        sink.write_2_bytes_little_endian(index);
    }
}

// Properties.
//-----------------------------------------------------------------------------

size_t point::satoshi_fixed_size(bool wire)
{
    return hash_size + (wire ? sizeof(uint32_t) : sizeof(uint16_t));
}

size_t point::serialized_size(bool wire) const
{
    return satoshi_fixed_size(wire);
}

const hash_digest& point::hash() const
{
    return hash_;
}

void point::set_hash(const hash_digest& value)
{
    // This is no longer a default instance, so valid.
    valid_ = true;
    hash_ = value;
}

void point::set_hash(hash_digest&& value)
{
    // This is no longer a default instance, so valid.
    valid_ = true;
    hash_ = std::move(value);
}

uint32_t point::index() const
{
    return index_;
}

void point::set_index(uint32_t value)
{
    // This is no longer a default instance, so valid.
    valid_ = true;
    index_ = value;
}

// Utilities.
//-----------------------------------------------------------------------------

// Changed in v3.0 and again in v3.1 (3.0 was unmasked, lots of collisions).
// This is used with output_point identification within a set of history rows
// of the same address. Collision will result in miscorrelation of points by
// client callers. This is stored in database. This is NOT a bitcoin checksum.
uint64_t point::checksum() const
{
    // Reserve 49 bits for the tx hash and 15 bits (32768) for the input index.
    static constexpr uint64_t mask = 0xffffffffffff8000;

    // Use an offset to the middle of the hash to avoid coincidental mining
    // of values into the front or back of tx hash (not a security feature).
    // Use most possible bits of tx hash to make intentional collision hard.
    const data_slice slice(std::next(hash_.begin(), 12), hash_.end());

    const auto bits = from_little_endian<uint64_t>(slice);
    const auto index = static_cast<uint64_t>(index_);

    const auto hash_upper_49_bits = bits & mask;
    const auto index_lower_15_bits = index & ~mask;
    return hash_upper_49_bits | index_lower_15_bits;
}

// Validation.
//-----------------------------------------------------------------------------

bool point::is_null() const
{
    return (index_ == null_index) && (hash_ == null_hash);
}

} // namespace chain
} // namespace system
} // namespace libbitcoin
