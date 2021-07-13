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

#include <bitcoin/system/chain/block_filter.hpp>

#include <utility>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/crypto/crypto.hpp>

namespace libbitcoin {
namespace system {
namespace chain {

// HACK: unlinked must match tx slab_map::not_found.
const uint64_t block_filter::validation::unlinked = max_int64;

block_filter::block_filter()
  : filter_type_(0),
    block_hash_(null_hash),
    header_(null_hash),
    filter_(),
    metadata{}
{
}

block_filter::block_filter(uint8_t filter_type, hash_digest&& block_hash,
    hash_digest&& header, data_chunk&& filter)
  : filter_type_(filter_type),
    block_hash_(std::move(block_hash)),
    header_(std::move(header)),
    filter_(std::move(filter)),
    metadata{}
{
}

block_filter::block_filter(uint8_t filter_type, const hash_digest& block_hash,
    const hash_digest& header, const data_chunk& filter)
  : filter_type_(filter_type),
    block_hash_(block_hash),
    header_(header),
    filter_(filter),
    metadata{}
{
}

block_filter::block_filter(block_filter&& other)
  : filter_type_(other.filter_type_),
    block_hash_(std::move(other.block_hash_)),
    header_(std::move(other.header_)),
    filter_(std::move(other.filter_)),
    metadata(std::move(other.metadata))
{
}

block_filter::block_filter(const block_filter& other)
  : filter_type_(other.filter_type_),
    block_hash_(other.block_hash_),
    header_(other.header_),
    filter_(other.filter_),
    metadata(other.metadata)
{
}

// Operators.
//-----------------------------------------------------------------------------

block_filter& block_filter::operator=(block_filter&& other)
{
    filter_type_ = other.filter_type_;
    block_hash_ = std::move(other.block_hash_);
    header_ = std::move(other.header_);
    filter_ = std::move(other.filter_);
    metadata = std::move(other.metadata);
    return *this;
}

bool block_filter::operator==(const block_filter& other) const
{
    return (filter_type_ == other.filter_type_)
        && (block_hash_ == other.block_hash_)
        && (header_ == other.header_)
        && (filter_ == other.filter_);
}

bool block_filter::operator!=(const block_filter& other) const
{
    return !(*this == other);
}

// Properties.
//-----------------------------------------------------------------------------

uint8_t block_filter::filter_type() const
{
    return filter_type_;
}

void block_filter::set_filter_type(uint8_t value)
{
    filter_type_ = value;
}

const hash_digest& block_filter::block_hash() const
{
    return block_hash_;
}

void block_filter::set_block_hash(const hash_digest& value)
{
    block_hash_ = value;
}

void block_filter::set_block_hash(hash_digest&& value)
{
    block_hash_ = std::move(value);
}

const hash_digest& block_filter::header() const
{
    return header_;
}

void block_filter::set_header(const hash_digest& value)
{
    header_ = value;
}

void block_filter::set_header(hash_digest&& value)
{
    header_ = std::move(value);
}

const data_chunk& block_filter::filter() const
{
    return filter_;
}

void block_filter::set_filter(const data_chunk& value)
{
    filter_ = value;
}

void block_filter::set_filter(data_chunk&& value)
{
    filter_ = std::move(value);
}

// Validation.
//-----------------------------------------------------------------------------

bool block_filter::is_valid() const
{
    return !filter_.empty();
}

void block_filter::reset()
{
    filter_type_ = 0;
    block_hash_.fill(0);
    header_.fill(0);
    filter_.clear();
    filter_.shrink_to_fit();
}

} // namespace chain
} // namespace system
} // namespace libbitcoin
