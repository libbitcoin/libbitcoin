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
#include <bitcoin/system/chain/output_point.hpp>

#include <cstddef>
#include <cstdint>
#include <utility>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/chain/enums/magic_numbers.hpp>
#include <bitcoin/system/chain/point.hpp>
#include <bitcoin/system/math/addition.hpp>

namespace libbitcoin {
namespace system {
namespace chain {

// Constructors.
//-----------------------------------------------------------------------------

output_point::output_point()
  : point{}, metadata{}
{
}

output_point::output_point(point&& value)
  : point(std::move(value)), metadata{}
{
}

output_point::output_point(const point& value)
  : point(value), metadata{}
{
}

output_point::output_point(const output_point& other)
  : point(other), metadata(other.metadata)
{
}

output_point::output_point(output_point&& other)
  : point(std::move(other)), metadata(std::move(other.metadata))
{
}

output_point::output_point(hash_digest&& hash, uint32_t index)
  : point({ std::move(hash), index }), metadata{}
{
}

output_point::output_point(const hash_digest& hash, uint32_t index)
  : point(hash, index), metadata{}
{
}

// Operators.
//-----------------------------------------------------------------------------

output_point& output_point::operator=(point&& other)
{
    reset();
    point::operator=(std::move(other));
    return *this;
}

output_point& output_point::operator=(const point& other)
{
    reset();
    point::operator=(other);
    return *this;
}

output_point& output_point::operator=(output_point&& other)
{
    point::operator=(std::move(other));
    metadata = std::move(other.metadata);
    return *this;
}

output_point& output_point::operator=(const output_point& other)
{
    point::operator=(other);
    metadata = other.metadata;
    return *this;
}

bool output_point::operator==(const point& other) const
{
    return point::operator==(other);
}

bool output_point::operator!=(const point& other) const
{
    return point::operator!=(other);
}

bool output_point::operator==(const output_point& other) const
{
    return point::operator==(other);
}

bool output_point::operator!=(const output_point& other) const
{
    return !(*this == other);
}

// Deserialization.
//-----------------------------------------------------------------------------

output_point output_point::factory(const data_chunk& data, bool wire)
{
    output_point instance;
    instance.from_data(data, wire);
    return instance;
}

output_point output_point::factory(std::istream& stream, bool wire)
{
    output_point instance;
    instance.from_data(stream, wire);
    return instance;
}

output_point output_point::factory(reader& source, bool wire)
{
    output_point instance;
    instance.from_data(source, wire);
    return instance;
}

// Validation.
//-----------------------------------------------------------------------------

// For tx pool validation height is that of the candidate block.
bool output_point::is_mature(size_t height) const
{
    // Coinbase (null) inputs and those with non-coinbase prevouts are mature.
    if (!metadata.coinbase || is_null())
        return true;

    // The (non-coinbase) input refers to a coinbase output, so validate depth.
    return floored_subtract(height, metadata.height) >= coinbase_maturity;
}

} // namespace chain
} // namespace system
} // namespace libbitcoin
