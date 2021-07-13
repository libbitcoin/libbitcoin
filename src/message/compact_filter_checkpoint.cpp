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

// Sponsored in part by Digital Contract Design, LLC

#include <bitcoin/system/message/compact_filter_checkpoint.hpp>

#include <initializer_list>
#include <bitcoin/system/message/message.hpp>
#include <bitcoin/system/message/version.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

const std::string compact_filter_checkpoint::command = "cfcheckpt";
const uint32_t compact_filter_checkpoint::version_minimum = version::level::minimum;
const uint32_t compact_filter_checkpoint::version_maximum = version::level::maximum;

compact_filter_checkpoint compact_filter_checkpoint::factory(uint32_t version,
    const data_chunk& data)
{
    compact_filter_checkpoint instance;
    instance.from_data(version, data);
    return instance;
}

compact_filter_checkpoint compact_filter_checkpoint::factory(uint32_t version,
    std::istream& stream)
{
    compact_filter_checkpoint instance;
    instance.from_data(version, stream);
    return instance;
}

compact_filter_checkpoint compact_filter_checkpoint::factory(uint32_t version,
    reader& source)
{
    compact_filter_checkpoint instance;
    instance.from_data(version, source);
    return instance;
}

compact_filter_checkpoint::compact_filter_checkpoint()
  : filter_type_(0u), stop_hash_(null_hash), filter_headers_()
{
}

compact_filter_checkpoint::compact_filter_checkpoint(uint8_t filter_type,
    const hash_digest& stop_hash, const hash_list& filter_headers)
  : filter_type_(filter_type), stop_hash_(stop_hash),
    filter_headers_(filter_headers)
{
}

compact_filter_checkpoint::compact_filter_checkpoint(uint8_t filter_type,
    hash_digest&& stop_hash, hash_list&& filter_headers)
  : filter_type_(filter_type), stop_hash_(std::move(stop_hash)),
    filter_headers_(std::move(filter_headers))
{
}

compact_filter_checkpoint::compact_filter_checkpoint(
    const compact_filter_checkpoint& other)
  : compact_filter_checkpoint(other.filter_type_, other.stop_hash_,
      other.filter_headers_)
{
}

compact_filter_checkpoint::compact_filter_checkpoint(
    compact_filter_checkpoint&& other)
  : compact_filter_checkpoint(other.filter_type_, std::move(other.stop_hash_),
      std::move(other.filter_headers_))
{
}

bool compact_filter_checkpoint::is_valid() const
{
    return stop_hash_ != null_hash && !filter_headers_.empty();
}

void compact_filter_checkpoint::reset()
{
    filter_type_ = 0u;
    stop_hash_.fill(0);
    filter_headers_.clear();
    filter_headers_.shrink_to_fit();
}

bool compact_filter_checkpoint::from_data(uint32_t version,
    const data_chunk& data)
{
    stream::in::copy istream(data);
    return from_data(version, istream);
}

bool compact_filter_checkpoint::from_data(uint32_t version,
    std::istream& stream)
{
    read::bytes::istream source(stream);
    return from_data(version, source);
}

bool compact_filter_checkpoint::from_data(uint32_t version, reader& source)
{
    reset();

    filter_type_ = source.read_byte();
    stop_hash_ = source.read_hash();

    const auto count = source.read_size();

    // TODO: is this the corrected protocol limit?
    // Guard against potential for arbitrary memory allocation.
    if (count > chain::max_block_size)
        source.invalidate();
    else
        filter_headers_.reserve(count);

    // Order is required.
    for (size_t id = 0; id < count && source; ++id)
        filter_headers_.push_back(source.read_hash());

    if (version < compact_filter_checkpoint::version_minimum)
        source.invalidate();

    if (!source)
        reset();

    return source;
}

data_chunk compact_filter_checkpoint::to_data(uint32_t version) const
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

void compact_filter_checkpoint::to_data(uint32_t version,
    std::ostream& stream) const
{
    write::bytes::ostream out(stream);
    to_data(version, out);
}

void compact_filter_checkpoint::to_data(uint32_t , writer& sink) const
{
    sink.write_byte(filter_type_);
    sink.write_bytes(stop_hash_);
    sink.write_variable(filter_headers_.size());

    for (const auto& element: filter_headers_)
        sink.write_bytes(element);
}

size_t compact_filter_checkpoint::serialized_size(uint32_t ) const
{
    return sizeof(filter_type_) + hash_size +
        variable_size(filter_headers_.size()) +
        (filter_headers_.size() * hash_size);
}

uint8_t compact_filter_checkpoint::filter_type() const
{
    return filter_type_;
}

void compact_filter_checkpoint::set_filter_type(uint8_t value)
{
    filter_type_ = value;
}

hash_digest& compact_filter_checkpoint::stop_hash()
{
    return stop_hash_;
}

const hash_digest& compact_filter_checkpoint::stop_hash() const
{
    return stop_hash_;
}

void compact_filter_checkpoint::set_stop_hash(const hash_digest& value)
{
    stop_hash_ = value;
}

void compact_filter_checkpoint::set_stop_hash(hash_digest&& value)
{
    stop_hash_ = std::move(value);
}

hash_list& compact_filter_checkpoint::filter_headers()
{
    return filter_headers_;
}

const hash_list& compact_filter_checkpoint::filter_headers() const
{
    return filter_headers_;
}

void compact_filter_checkpoint::set_filter_headers(const hash_list& value)
{
    filter_headers_ = value;
}

void compact_filter_checkpoint::set_filter_headers(hash_list&& value)
{
    filter_headers_ = std::move(value);
}

compact_filter_checkpoint& compact_filter_checkpoint::operator=(
    compact_filter_checkpoint&& other)
{
    filter_type_ = other.filter_type_;
    stop_hash_ = std::move(other.stop_hash_);
    filter_headers_ = std::move(other.filter_headers_);
    return *this;
}

bool compact_filter_checkpoint::operator==(
    const compact_filter_checkpoint& other) const
{
    return (filter_type_ == other.filter_type_)
        && (stop_hash_ == other.stop_hash_)
        && (filter_headers_ == other.filter_headers_);
}

bool compact_filter_checkpoint::operator!=(
    const compact_filter_checkpoint& other) const
{
    return !(*this == other);
}

} // namespace message
} // namespace system
} // namespace libbitcoin
