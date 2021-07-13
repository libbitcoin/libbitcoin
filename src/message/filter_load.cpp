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
#include <bitcoin/system/message/filter_load.hpp>

#include <bitcoin/system/assert.hpp>
#include <bitcoin/system/message/message.hpp>
#include <bitcoin/system/message/version.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

const std::string filter_load::command = "filterload";
const uint32_t filter_load::version_minimum = version::level::bip37;
const uint32_t filter_load::version_maximum = version::level::maximum;

filter_load filter_load::factory(uint32_t version,
    const data_chunk& data)
{
    filter_load instance;
    instance.from_data(version, data);
    return instance;
}

filter_load filter_load::factory(uint32_t version,
    std::istream& stream)
{
    filter_load instance;
    instance.from_data(version, stream);
    return instance;
}

filter_load filter_load::factory(uint32_t version,
    reader& source)
{
    filter_load instance;
    instance.from_data(version, source);
    return instance;
}

filter_load::filter_load()
  : filter_(), hash_functions_(0), tweak_(0), flags_(0x00)
{
}

filter_load::filter_load(const data_chunk& filter, uint32_t hash_functions,
    uint32_t tweak, uint8_t flags)
  : filter_(filter), hash_functions_(hash_functions), tweak_(tweak),
    flags_(flags)
{
}

filter_load::filter_load(data_chunk&& filter, uint32_t hash_functions,
    uint32_t tweak, uint8_t flags)
  : filter_(std::move(filter)), hash_functions_(hash_functions), tweak_(tweak),
    flags_(flags)
{
}

filter_load::filter_load(const filter_load& other)
  : filter_load(other.filter_, other.hash_functions_,
      other.tweak_, other.flags_)
{
}

filter_load::filter_load(filter_load&& other)
  : filter_load(std::move(other.filter_), other.hash_functions_, other.tweak_,
      other.flags_)
{
}

bool filter_load::is_valid() const
{
    return !filter_.empty()
        || (hash_functions_ != 0)
        || (tweak_ != 0)
        || (flags_ != 0x00);
}

void filter_load::reset()
{
    filter_.clear();
    filter_.shrink_to_fit();
    hash_functions_ = 0;
    tweak_ = 0;
    flags_ = 0x00;
}

bool filter_load::from_data(uint32_t version, const data_chunk& data)
{
    stream::in::copy istream(data);
    return from_data(version, istream);
}

bool filter_load::from_data(uint32_t version, std::istream& stream)
{
    read::bytes::istream source(stream);
    return from_data(version, source);
}

bool filter_load::from_data(uint32_t version, reader& source)
{
    reset();

    const auto size = source.read_size();

    if (size > chain::max_filter_load)
        source.invalidate();
    else
        filter_ = source.read_bytes(size);

    hash_functions_ = source.read_4_bytes_little_endian();

    if (hash_functions_ > chain::max_filter_functions)
        source.invalidate();

    tweak_ = source.read_4_bytes_little_endian();
    flags_ = source.read_byte();

    if (version < filter_load::version_minimum)
        source.invalidate();

    if (!source)
        reset();

    return source;
}

data_chunk filter_load::to_data(uint32_t version) const
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

void filter_load::to_data(uint32_t version, std::ostream& stream) const
{
    write::bytes::ostream out(stream);
    to_data(version, out);
}

void filter_load::to_data(uint32_t, writer& sink) const
{
    sink.write_variable(filter_.size());
    sink.write_bytes(filter_);
    sink.write_4_bytes_little_endian(hash_functions_);
    sink.write_4_bytes_little_endian(tweak_);
    sink.write_byte(flags_);
}

size_t filter_load::serialized_size(uint32_t) const
{
    return 1u + 4u + 4u + variable_size(filter_.size()) +
        filter_.size();
}

data_chunk& filter_load::filter()
{
    return filter_;
}

const data_chunk& filter_load::filter() const
{
    return filter_;
}

void filter_load::set_filter(const data_chunk& value)
{
    filter_ = value;
}

void filter_load::set_filter(data_chunk&& value)
{
    filter_ = std::move(value);
}

uint32_t filter_load::hash_functions() const
{
    return hash_functions_;
}

void filter_load::set_hash_functions(uint32_t value)
{
    hash_functions_ = value;
}

uint32_t filter_load::tweak() const
{
    return tweak_;
}

void filter_load::set_tweak(uint32_t value)
{
    tweak_ = value;
}

uint8_t filter_load::flags() const
{
    return flags_;
}

void filter_load::set_flags(uint8_t value)
{
    flags_ = value;
}

filter_load& filter_load::operator=(filter_load&& other)
{
    filter_ = std::move(other.filter_);
    hash_functions_ = other.hash_functions_;
    tweak_ = other.tweak_;
    flags_ = other.flags_;
    return *this;
}

bool filter_load::operator==(const filter_load& other) const
{
    return (filter_ == other.filter_)
        && (hash_functions_ == other.hash_functions_)
        && (tweak_ == other.tweak_)
        && (flags_ == other.flags_);
}

bool filter_load::operator!=(const filter_load& other) const
{
    return !(*this == other);
}

} // namespace message
} // namespace system
} // namespace libbitcoin
