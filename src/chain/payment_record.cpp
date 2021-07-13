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
#include <bitcoin/system/chain/payment_record.hpp>

#include <cstddef>
#include <cstdint>
#include <istream>
#include <utility>
#include <bitcoin/system/chain/point.hpp>
#include <bitcoin/system/chain/transaction.hpp>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace chain {

const size_t payment_record::unconfirmed = max_size_t;
static const auto unlinked = transaction::validation::unlinked;

// Constructors.
//-----------------------------------------------------------------------------

// A default instance is invalid (until modified).
payment_record::payment_record()
  : valid_(false),
    output_(false),
    height_(unconfirmed),
    hash_(null_hash),
    index_(point::null_index),
    data_(0),
    link_(unlinked)
{
}

payment_record::payment_record(chain::payment_record&& other)
  : valid_(other.valid_),
    output_(other.output_),
    height_(other.height_),
    hash_(null_hash),
    index_(point::null_index),
    data_(other.data_),
    link_(other.link_)
{
}

payment_record::payment_record(const chain::payment_record& other)
  : valid_(other.valid_),
    output_(other.output_),
    height_(other.height_),
    hash_(other.hash_),
    index_(other.index_),
    data_(other.data_),
    link_(other.link_)
{
}

payment_record::payment_record(uint64_t link, uint32_t index, uint64_t data,
    bool output)
  : valid_(true),
    output_(output),
    height_(unconfirmed),
    hash_(null_hash),
    index_(index),
    data_(data),
    link_(link)

{
}

// Operators.
//-----------------------------------------------------------------------------

payment_record& payment_record::operator=(payment_record&& other)
{
    valid_ = other.valid_;
    output_ = other.output_;
    height_ = other.height_;
    hash_ = other.hash_;
    index_ = other.index_;
    data_ = other.data_;
    link_ = other.link_;
    return *this;
}

payment_record& payment_record::operator=(const payment_record& other)
{
    valid_ = other.valid_;
    output_ = other.output_;
    height_ = other.height_;
    hash_ = other.hash_;
    index_ = other.index_;
    data_ = other.data_;
    link_ = other.link_;
    return *this;
}

bool payment_record::operator==(const payment_record& other) const
{
    return (output_ == other.output_)
        && (height_ == other.height_)
        && (hash_ == other.hash_)
        && (index_ == other.index_)
        && (data_ == other.data_)
        && (link_ == other.link_);
}

bool payment_record::operator!=(const payment_record& other) const
{
    return !(*this == other);
}

// Deserialization.
//-----------------------------------------------------------------------------

payment_record payment_record::factory(const data_chunk& data, bool wire)
{
    payment_record instance;
    instance.from_data(data, wire);
    return instance;
}

payment_record payment_record::factory(std::istream& stream, bool wire)
{
    payment_record instance;
    instance.from_data(stream, wire);
    return instance;
}

payment_record payment_record::factory(reader& source, bool wire)
{
    payment_record instance;
    instance.from_data(source, wire);
    return instance;
}

bool payment_record::from_data(const data_chunk& data, bool wire)
{
    stream::in::copy istream(data);
    return from_data(istream, wire);
}

bool payment_record::from_data(std::istream& stream, bool wire)
{
    read::bytes::istream source(stream);
    return from_data(source, wire);
}

// Avoid point reuse due to affect on store tx serialization.
bool payment_record::from_data(reader& source, bool wire)
{
    valid_ = true;
    output_ = (source.read_byte() == 1);

    if (wire)
    {
        height_ = source.read_4_bytes_little_endian();
        link_ = unlinked;
        hash_ = source.read_hash();
        index_ = source.read_4_bytes_little_endian();
    }
    else
    {
        height_ = unconfirmed;
        link_ = source.read_8_bytes_little_endian();
        hash_ = null_hash;
        index_ = source.read_2_bytes_little_endian();

        // Convert 16 bit sentinel to 32 bit sentinel.
        if (index_ == max_uint16)
            index_ = point::null_index;
    }

    data_ = source.read_8_bytes_little_endian();

    if (!source)
        reset();

    return source;
}

// protected
void payment_record::reset()
{
    valid_ = false;
    output_ = false;
    height_ = unconfirmed;
    hash_ = null_hash;
    index_ = point::null_index;
    data_ = 0;
    link_ = unlinked;
}

bool payment_record::is_valid() const
{
    return valid_;
}

// Serialization.
//-----------------------------------------------------------------------------

data_chunk payment_record::to_data(bool wire) const
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

void payment_record::to_data(std::ostream& stream, bool wire) const
{
    write::bytes::ostream out(stream);
    to_data(out, wire);
}

// Wire assumes height and point.hash population.
void payment_record::to_data(writer& sink, bool wire) const
{
    sink.write_byte(output_ ? 1 : 0);

    if (wire)
    {
        // HACK: Height sentinel is cast from size_t to uint32_t.
        BITCOIN_ASSERT(height_ <= max_uint32 || height_ == unconfirmed);
        const auto height = static_cast<uint32_t>(height_);
        sink.write_4_bytes_little_endian(height);

        sink.write_bytes(hash_);
        sink.write_4_bytes_little_endian(index_);
    }
    else
    {
        sink.write_8_bytes_little_endian(link_);

        BITCOIN_ASSERT(index_ == point::null_index || index_ < max_uint16);

        // Convert 32 bit sentinel to 16 bit sentinel.
        const auto index = (index_ == point::null_index) ? max_uint16 :
            static_cast<uint16_t>(index_);

        sink.write_2_bytes_little_endian(index);
    }

    sink.write_8_bytes_little_endian(data_);
}

// Properties (size, accessors).
//-----------------------------------------------------------------------------

// TODO: should be further generalized into point with link metadata.
// Non-wire point does not currently include the tx offset (hacked in).
// Non-wire payment_record doesn't include height (retrieved from tx store).
size_t payment_record::satoshi_fixed_size(bool wire)
{
    return
        sizeof(uint8_t) +
        (wire ? sizeof(uint32_t) : 0) +
        (wire ? hash_size : sizeof(uint64_t)) +
        (wire ? sizeof(uint32_t) : sizeof(uint16_t)) +
        sizeof(uint64_t);
}

size_t payment_record::serialized_size(bool wire) const
{
    return satoshi_fixed_size(wire);
}

bool payment_record::is_output() const
{
    return output_;
}

uint64_t payment_record::data() const
{
    return data_;
}

uint64_t payment_record::link() const
{
    return link_;
}

size_t payment_record::height() const
{
    return height_;
}

// Set after non-wire deserializaton (distinct store).
void payment_record::set_height(size_t height)
{
    // This is no longer a default instance, so valid.
    valid_ = true;
    height_ = height;
}

hash_digest payment_record::hash() const
{
    return hash_;
}

// Set after non-wire deserialization (distinct store).
void payment_record::set_hash(hash_digest&& hash)
{
    // This is no longer a default instance, so valid.
    valid_ = true;
    hash_ = std::move(hash);
}

uint32_t payment_record::index() const
{
    return index_;
}

////void payment_record::set_index(uint32_t value)
////{
////    // This is no longer a default instance, so valid.
////    valid_ = true;
////    index_ = value;
////}

} // namespace chain
} // namespace system
} // namespace libbitcoin
