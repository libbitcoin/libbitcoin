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
#include <bitcoin/system/message/inventory_vector.hpp>

#include <cstdint>
#include <string>
#include <bitcoin/system/assert.hpp>
#include <bitcoin/system/message/inventory.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

uint32_t inventory_vector::to_number(type_id inventory_type)
{
    return static_cast<uint32_t>(inventory_type);
}

inventory_vector::type_id inventory_vector::to_type(uint32_t value)
{
    return static_cast<type_id>(value);
}

std::string inventory_vector::to_string(type_id inventory_type)
{
    switch (inventory_type)
    {
        case type_id::transaction:
            return "transaction";
        case type_id::block:
            return "block";
        case type_id::filtered_block:
            return "filtered_block";
        case type_id::compact_block:
            return "compact_block";
        case type_id::witness_transaction:
            return "witness_transaction";
        case type_id::witness_block:
            return "witness_block";
        case type_id::reserved:
            return "reserved";
        case type_id::error:
        default:
            return "error";
    }
}

inventory_vector inventory_vector::factory(uint32_t version,
    const data_chunk& data)
{
    inventory_vector instance;
    instance.from_data(version, data);
    return instance;
}

inventory_vector inventory_vector::factory(uint32_t version,
    std::istream& stream)
{
    inventory_vector instance;
    instance.from_data(version, stream);
    return instance;
}

inventory_vector inventory_vector::factory(uint32_t version,
    reader& source)
{
    inventory_vector instance;
    instance.from_data(version, source);
    return instance;
}

inventory_vector::inventory_vector()
  : inventory_vector(type_id::error, null_hash)
{
}

inventory_vector::inventory_vector(type_id type, const hash_digest& hash)
  : type_(type), hash_(hash)
{
}

inventory_vector::inventory_vector(type_id type, hash_digest&& hash)
  : type_(type), hash_(std::move(hash))
{
}

inventory_vector::inventory_vector(const inventory_vector& other)
  : inventory_vector(other.type_, other.hash_)
{
}

inventory_vector::inventory_vector(inventory_vector&& other)
  : inventory_vector(other.type_, std::move(other.hash_))
{
}

bool inventory_vector::is_valid() const
{
    return (type_ != type_id::error) || (hash_ != null_hash);
}

void inventory_vector::reset()
{
    type_ = type_id::error;
    hash_.fill(0);
}

void inventory_vector::to_witness()
{
    if (type_ == type_id::block || type_ == type_id::transaction)
        type_ = to_type(to_number(type_) | to_number(type_id::witness));
}

bool inventory_vector::from_data(uint32_t version,
    const data_chunk& data)
{
    stream::in::copy istream(data);
    return from_data(version, istream);
}

bool inventory_vector::from_data(uint32_t version,
    std::istream& stream)
{
    read::bytes::istream source(stream);
    return from_data(version, source);
}

bool inventory_vector::from_data(uint32_t ,
    reader& source)
{
    reset();

    const auto raw_type = source.read_4_bytes_little_endian();
    type_ = inventory_vector::to_type(raw_type);
    hash_ = source.read_hash();

    if (!source)
        reset();

    return source;
}

data_chunk inventory_vector::to_data(uint32_t version) const
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

void inventory_vector::to_data(uint32_t version, std::ostream& stream) const
{
    write::bytes::ostream out(stream);
    to_data(version, out);
}

void inventory_vector::to_data(uint32_t, writer& sink) const
{
    const auto raw_type = inventory_vector::to_number(type_);
    sink.write_4_bytes_little_endian(raw_type);
    sink.write_bytes(hash_);
}

size_t inventory_vector::serialized_size(uint32_t version) const
{
    return inventory_vector::satoshi_fixed_size(version);
}

size_t inventory_vector::satoshi_fixed_size(uint32_t)
{
    return sizeof(hash_) + sizeof(uint32_t);
}

bool inventory_vector::is_block_type() const
{
    return type_ == type_id::witness_block
        || type_ == type_id::block
        || type_ == type_id::compact_block
        || type_ == type_id::filtered_block;
}

bool inventory_vector::is_transaction_type() const
{
    return type_ == type_id::witness_transaction
        || type_ == type_id::transaction;
}

inventory_vector::type_id inventory_vector::type() const
{
    return type_;
}

void inventory_vector::set_type(type_id value)
{
    type_ = value;
}

hash_digest& inventory_vector::hash()
{
    return hash_;
}

const hash_digest& inventory_vector::hash() const
{
    return hash_;
}

void inventory_vector::set_hash(const hash_digest& value)
{
    hash_ = value;
}

void inventory_vector::set_hash(hash_digest&& value)
{
    hash_ = std::move(value);
}

inventory_vector& inventory_vector::operator=(inventory_vector&& other)
{
    type_ = other.type_;
    hash_ = std::move(other.hash_);
    return *this;
}

void inventory_vector::operator=(const inventory_vector& other)
{
    type_ = other.type_;
    hash_ = other.hash_;
}

bool inventory_vector::operator==(const inventory_vector& other) const
{
    return (hash_ == other.hash_) && (type_ == other.type_);
}

bool inventory_vector::operator!=(const inventory_vector& other) const
{
    return !(*this == other);
}

} // namespace message
} // namespace system
} // namespace libbitcoin
