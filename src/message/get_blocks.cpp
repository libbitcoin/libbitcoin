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
#include <bitcoin/system/message/get_blocks.hpp>

#include <bitcoin/system/math/math.hpp>
#include <bitcoin/system/message/message.hpp>
#include <bitcoin/system/message/version.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

const std::string get_blocks::command = "getblocks";
const uint32_t get_blocks::version_minimum = version::level::minimum;
const uint32_t get_blocks::version_maximum = version::level::maximum;

get_blocks get_blocks::factory(uint32_t version,
    const data_chunk& data)
{
    get_blocks instance;
    instance.from_data(version, data);
    return instance;
}

get_blocks get_blocks::factory(uint32_t version,
    std::istream& stream)
{
    get_blocks instance;
    instance.from_data(version, stream);
    return instance;
}

get_blocks get_blocks::factory(uint32_t version,
    reader& source)
{
    get_blocks instance;
    instance.from_data(version, source);
    return instance;
}

// This predicts the size of locator_heights output.
size_t get_blocks::locator_size(size_t top)
{
    size_t size = 0, step = 1;
    for (auto height = top; height > 0; height = floored_subtract(height, step))
        if (++size > 9u)
            step <<= 1;

    return ++size;
}

// This algorithm is a network best practice, not a consensus rule.
get_blocks::indexes get_blocks::locator_heights(size_t top)
{
    size_t step = 1;
    indexes heights;
    heights.reserve(locator_size(top));

    // Start at top block and collect block indexes in reverse.
    for (auto height = top; height > 0; height = floored_subtract(height, step))
    {
        heights.push_back(height);

        // Push top 10 indexes then back off exponentially.
        if (heights.size() > 9u)
            step <<= 1;
    }

    // Push the genesis block index.
    heights.push_back(0);
    return heights;
}

get_blocks::get_blocks()
  : start_hashes_(), stop_hash_(null_hash)
{
}

get_blocks::get_blocks(const hash_list& start, const hash_digest& stop)
  : start_hashes_(start), stop_hash_(stop)
{
}

get_blocks::get_blocks(hash_list&& start, hash_digest&& stop)
  : start_hashes_(std::move(start)), stop_hash_(std::move(stop))
{
}

get_blocks::get_blocks(const get_blocks& other)
  : get_blocks(other.start_hashes_, other.stop_hash_)
{
}

get_blocks::get_blocks(get_blocks&& other)
  : get_blocks(std::move(other.start_hashes_), std::move(other.stop_hash_))
{
}

bool get_blocks::is_valid() const
{
    return !start_hashes_.empty() || (stop_hash_ != null_hash);
}

void get_blocks::reset()
{
    start_hashes_.clear();
    start_hashes_.shrink_to_fit();
    stop_hash_.fill(0);
}

bool get_blocks::from_data(uint32_t version, const data_chunk& data)
{
    stream::in::copy stream(data);
    return from_data(version, stream);
}

bool get_blocks::from_data(uint32_t version, std::istream& stream)
{
    read::bytes::istream source(stream);
    return from_data(version, source);
}

bool get_blocks::from_data(uint32_t, reader& source)
{
    reset();

    // Discard protocol version because it is stupid.
    source.read_4_bytes_little_endian();
    const auto count = source.read_size();

    // Guard against potential for arbitrary memory allocation.
    if (count > chain::max_get_blocks)
        source.invalidate();
    else
        start_hashes_.reserve(count);

    for (size_t hash = 0; hash < count && source; ++hash)
        start_hashes_.push_back(source.read_hash());

    stop_hash_ = source.read_hash();

    if (!source)
        reset();

    return source;
}

data_chunk get_blocks::to_data(uint32_t version) const
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

void get_blocks::to_data(uint32_t version, std::ostream& stream) const
{
    write::bytes::ostream out(stream);
    to_data(version, out);
}

void get_blocks::to_data(uint32_t version, writer& sink) const
{
    sink.write_4_bytes_little_endian(version);
    sink.write_variable(start_hashes_.size());

    for (const auto& start_hash: start_hashes_)
        sink.write_bytes(start_hash);

    sink.write_bytes(stop_hash_);
}

size_t get_blocks::serialized_size(uint32_t) const
{
    return size_t(36) + variable_size(start_hashes_.size()) +
        hash_size * start_hashes_.size();
}

hash_list& get_blocks::start_hashes()
{
    return start_hashes_;
}

const hash_list& get_blocks::start_hashes() const
{
    return start_hashes_;
}

void get_blocks::set_start_hashes(const hash_list& value)
{
    start_hashes_ = value;
}

void get_blocks::set_start_hashes(hash_list&& value)
{
    start_hashes_ = std::move(value);
}

hash_digest& get_blocks::stop_hash()
{
    return stop_hash_;
}

const hash_digest& get_blocks::stop_hash() const
{
    return stop_hash_;
}

void get_blocks::set_stop_hash(const hash_digest& value)
{
    stop_hash_ = value;
}

void get_blocks::set_stop_hash(hash_digest&& value)
{
    stop_hash_ = std::move(value);
}

get_blocks& get_blocks::operator=(get_blocks&& other)
{
    start_hashes_ = std::move(other.start_hashes_);
    stop_hash_ = std::move(other.stop_hash_);
    return *this;
}

bool get_blocks::operator==(const get_blocks& other) const
{
    auto result = (start_hashes_.size() == other.start_hashes_.size()) &&
        (stop_hash_ == other.stop_hash_);

    for (size_t i = 0; i < start_hashes_.size() && result; i++)
        result = (start_hashes_[i] == other.start_hashes_[i]);

    return result;
}

bool get_blocks::operator!=(const get_blocks& other) const
{
    return !(*this == other);
}

} // namespace message
} // namespace system
} // namespace libbitcoin
