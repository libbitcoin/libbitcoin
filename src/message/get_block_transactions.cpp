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
#include <bitcoin/system/message/get_block_transactions.hpp>

#include <initializer_list>
#include <bitcoin/system/message/message.hpp>
#include <bitcoin/system/message/version.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

const std::string get_block_transactions::command = "getblocktxn";
const uint32_t get_block_transactions::version_minimum = version::level::bip152;
const uint32_t get_block_transactions::version_maximum = version::level::bip152;

get_block_transactions get_block_transactions::factory(
    uint32_t version, const data_chunk& data)
{
    get_block_transactions instance;
    instance.from_data(version, data);
    return instance;
}

get_block_transactions get_block_transactions::factory(
    uint32_t version, std::istream& stream)
{
    get_block_transactions instance;
    instance.from_data(version, stream);
    return instance;
}

get_block_transactions get_block_transactions::factory(
    uint32_t version, reader& source)
{
    get_block_transactions instance;
    instance.from_data(version, source);
    return instance;
}

get_block_transactions::get_block_transactions()
  : block_hash_(null_hash), indexes_()
{
}

get_block_transactions::get_block_transactions(const hash_digest& block_hash,
    const std::vector<uint64_t>& indexes)
  : block_hash_(block_hash), indexes_(indexes)
{
}

get_block_transactions::get_block_transactions(hash_digest&& block_hash,
    std::vector<uint64_t>&& indexes)
  : block_hash_(std::move(block_hash)), indexes_(std::move(indexes))
{
}

get_block_transactions::get_block_transactions(
    const get_block_transactions& other)
  : get_block_transactions(other.block_hash_, other.indexes_)
{
}

get_block_transactions::get_block_transactions(get_block_transactions&& other)
  : get_block_transactions(std::move(other.block_hash_),
      std::move(other.indexes_))
{
}

bool get_block_transactions::is_valid() const
{
    return (block_hash_ != null_hash);
}

void get_block_transactions::reset()
{
    block_hash_ = null_hash;
    indexes_.clear();
    indexes_.shrink_to_fit();
}

bool get_block_transactions::from_data(uint32_t version,
    const data_chunk& data)
{
    stream::in::copy istream(data);
    return from_data(version, istream);
}

bool get_block_transactions::from_data(uint32_t version,
    std::istream& stream)
{
    read::bytes::istream source(stream);
    return from_data(version, source);
}

bool get_block_transactions::from_data(uint32_t ,
    reader& source)
{
    reset();

    block_hash_ = source.read_hash();
    const auto count = source.read_size();

    // Guard against potential for arbitrary memory allocation.
    if (count > chain::max_block_size)
        source.invalidate();
    else
        indexes_.reserve(count);

    for (size_t position = 0; position < count && source; ++position)
        indexes_.push_back(source.read_size());

    if (!source)
        reset();

    return source;
}

data_chunk get_block_transactions::to_data(uint32_t version) const
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

void get_block_transactions::to_data(uint32_t version,
    std::ostream& stream) const
{
    write::bytes::ostream out(stream);
    to_data(version, out);
}

void get_block_transactions::to_data(uint32_t ,
    writer& sink) const
{
    sink.write_bytes(block_hash_);
    sink.write_variable(indexes_.size());
    for (const auto& element: indexes_)
        sink.write_variable(element);
}

size_t get_block_transactions::serialized_size(uint32_t) const
{
    auto size = hash_size + variable_size(indexes_.size());

    for (const auto& element: indexes_)
        size += variable_size(element);

    return size;
}

hash_digest& get_block_transactions::block_hash()
{
    return block_hash_;
}

const hash_digest& get_block_transactions::block_hash() const
{
    return block_hash_;
}

void get_block_transactions::set_block_hash(const hash_digest& value)
{
    block_hash_ = value;
}

void get_block_transactions::set_block_hash(hash_digest&& value)
{
    block_hash_ = std::move(value);
}

std::vector<uint64_t>& get_block_transactions::indexes()
{
    return indexes_;
}

const std::vector<uint64_t>& get_block_transactions::indexes() const
{
    return indexes_;
}

void get_block_transactions::set_indexes(const std::vector<uint64_t>& values)
{
    indexes_ = values;
}

void get_block_transactions::set_indexes(std::vector<uint64_t>&& values)
{
    indexes_ = values;
}

get_block_transactions& get_block_transactions::operator=(
    get_block_transactions&& other)
{
    block_hash_ = other.block_hash_;
    indexes_ = other.indexes_;
    return *this;
}

bool get_block_transactions::operator==(
    const get_block_transactions& other) const
{
    return (block_hash_ == other.block_hash_)
        && (indexes_ == other.indexes_);
}

bool get_block_transactions::operator!=(
    const get_block_transactions& other) const
{
    return !(*this == other);
}

} // namespace message
} // namespace system
} // namespace libbitcoin
