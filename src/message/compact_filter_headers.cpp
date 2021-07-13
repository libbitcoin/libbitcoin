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

#include <bitcoin/system/message/compact_filter_headers.hpp>

#include <initializer_list>
#include <bitcoin/system/message/message.hpp>
#include <bitcoin/system/message/version.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

const std::string compact_filter_headers::command = "cfheaders";
const uint32_t compact_filter_headers::version_minimum = version::level::minimum;
const uint32_t compact_filter_headers::version_maximum = version::level::maximum;

compact_filter_headers compact_filter_headers::factory(uint32_t version,
    const data_chunk& data)
{
    compact_filter_headers instance;
    instance.from_data(version, data);
    return instance;
}

compact_filter_headers compact_filter_headers::factory(uint32_t version,
    std::istream& stream)
{
    compact_filter_headers instance;
    instance.from_data(version, stream);
    return instance;
}

compact_filter_headers compact_filter_headers::factory(uint32_t version,
    reader& source)
{
    compact_filter_headers instance;
    instance.from_data(version, source);
    return instance;
}

compact_filter_headers::compact_filter_headers()
  : filter_type_(0u), stop_hash_(null_hash),
    previous_filter_header_(null_hash), filter_hashes_()
{
}

compact_filter_headers::compact_filter_headers(uint8_t filter_type,
    const hash_digest& stop_hash, const hash_digest& previous_filter_header,
    const hash_list& filter_hashes)
  : filter_type_(filter_type),
    stop_hash_(stop_hash),
    previous_filter_header_(previous_filter_header),
    filter_hashes_(filter_hashes)
{
}

compact_filter_headers::compact_filter_headers(uint8_t filter_type,
    hash_digest&& stop_hash, hash_digest&& previous_filter_header,
    hash_list&& filter_hashes)
  : filter_type_(filter_type),
    stop_hash_(std::move(stop_hash)),
    previous_filter_header_(std::move(previous_filter_header)),
    filter_hashes_(std::move(filter_hashes))
{
}

compact_filter_headers::compact_filter_headers(
    const compact_filter_headers& other)
  : compact_filter_headers(other.filter_type_, other.stop_hash_,
      other.previous_filter_header_, other.filter_hashes_)
{
}

compact_filter_headers::compact_filter_headers(compact_filter_headers&& other)
  : compact_filter_headers(other.filter_type_, std::move(other.stop_hash_),
      std::move(other.previous_filter_header_),
      std::move(other.filter_hashes_))
{
}

bool compact_filter_headers::is_valid() const
{
    return stop_hash_ != null_hash
        && previous_filter_header_ != null_hash
        && !filter_hashes_.empty();
}

void compact_filter_headers::reset()
{
    filter_type_ = 0u;
    stop_hash_.fill(0);
    previous_filter_header_.fill(0);
    filter_hashes_.clear();
    filter_hashes_.shrink_to_fit();
}

bool compact_filter_headers::from_data(uint32_t version,
    const data_chunk& data)
{
    stream::in::copy istream(data);
    return from_data(version, istream);
}

bool compact_filter_headers::from_data(uint32_t version, std::istream& stream)
{
    read::bytes::istream source(stream);
    return from_data(version, source);
}

bool compact_filter_headers::from_data(uint32_t version, reader& source)
{
    reset();

    filter_type_ = source.read_byte();
    stop_hash_ = source.read_hash();
    previous_filter_header_ = source.read_hash();

    const auto count = source.read_size();

    // TODO: is this the corrected protocol limit?
    // Guard against potential for arbitrary memory allocation.
    if (count > chain::max_block_size)
        source.invalidate();
    else
        filter_hashes_.reserve(count);

    // Order is required.
    for (size_t id = 0; id < count && source; ++id)
        filter_hashes_.push_back(source.read_hash());

    if (version < compact_filter_headers::version_minimum)
        source.invalidate();

    if (!source)
        reset();

    return source;
}

data_chunk compact_filter_headers::to_data(uint32_t version) const
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

void compact_filter_headers::to_data(uint32_t version,
    std::ostream& stream) const
{
    write::bytes::ostream out(stream);
    to_data(version, out);
}

void compact_filter_headers::to_data(uint32_t , writer& sink) const
{
    sink.write_byte(filter_type_);
    sink.write_bytes(stop_hash_);
    sink.write_bytes(previous_filter_header_);
    sink.write_variable(filter_hashes_.size());

    for (const auto& element: filter_hashes_)
        sink.write_bytes(element);
}

size_t compact_filter_headers::serialized_size(uint32_t ) const
{
    return sizeof(filter_type_) + hash_size + hash_size +
        variable_size(filter_hashes_.size()) +
        (filter_hashes_.size() * hash_size);
}

uint8_t compact_filter_headers::filter_type() const
{
    return filter_type_;
}

void compact_filter_headers::set_filter_type(uint8_t value)
{
    filter_type_ = value;
}

hash_digest& compact_filter_headers::stop_hash()
{
    return stop_hash_;
}

const hash_digest& compact_filter_headers::stop_hash() const
{
    return stop_hash_;
}

void compact_filter_headers::set_stop_hash(const hash_digest& value)
{
    stop_hash_ = value;
}

void compact_filter_headers::set_stop_hash(hash_digest&& value)
{
    stop_hash_ = std::move(value);
}

hash_digest& compact_filter_headers::previous_filter_header()
{
    return previous_filter_header_;
}

const hash_digest& compact_filter_headers::previous_filter_header() const
{
    return previous_filter_header_;
}

void compact_filter_headers::set_previous_filter_header(
    const hash_digest& value)
{
    previous_filter_header_ = value;
}

void compact_filter_headers::set_previous_filter_header(hash_digest&& value)
{
    previous_filter_header_ = std::move(value);
}

hash_list& compact_filter_headers::filter_hashes()
{
    return filter_hashes_;
}

const hash_list& compact_filter_headers::filter_hashes() const
{
    return filter_hashes_;
}

void compact_filter_headers::set_filter_hashes(const hash_list& value)
{
    filter_hashes_ = value;
}

void compact_filter_headers::set_filter_hashes(hash_list&& value)
{
    filter_hashes_ = std::move(value);
}

compact_filter_headers& compact_filter_headers::operator=(
    compact_filter_headers&& other)
{
    filter_type_ = other.filter_type_;
    stop_hash_ = std::move(other.stop_hash_);
    previous_filter_header_ = std::move(other.previous_filter_header_);
    filter_hashes_ = std::move(other.filter_hashes_);
    return *this;
}

bool compact_filter_headers::operator==(
    const compact_filter_headers& other) const
{
    return (filter_type_ == other.filter_type_)
        && (stop_hash_ == other.stop_hash_)
        && (previous_filter_header_ == other.previous_filter_header_)
        && (filter_hashes_ == other.filter_hashes_);
}

bool compact_filter_headers::operator!=(
    const compact_filter_headers& other) const
{
    return !(*this == other);
}

} // namespace message
} // namespace system
} // namespace libbitcoin
