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
#include <bitcoin/system/chain/compact.hpp>

#include <cstdint>
#include <bitcoin/system/assert.hpp>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/data/uintx.hpp>

namespace libbitcoin {
namespace system {
namespace chain {

// Bitcoin compact for represents a value in base 256 notation as follows:
// value = (-1^sign) * mantissa * 256^(exponent-3)

// [exponent:8 | sign:1 | mantissa:31]
static constexpr uint32_t exp_byte = 0xff000000;
static constexpr uint32_t sign_bit = 0x00800000;
static constexpr uint32_t mantissa_max = ~(exp_byte | sign_bit);
static constexpr size_t mantissa_bits = to_bits(sub1(sizeof(uint32_t)));

// assertions
DEBUG_ONLY(static constexpr uint32_t mantissa_mask = ~mantissa_max;)
DEBUG_ONLY(static constexpr uint32_t first_byte_mask = 0xffffff00;)

// Inlines
//-----------------------------------------------------------------------------

inline bool is_negated(uint32_t compact)
{
    return !is_zero(compact & sign_bit);
}

inline bool is_nonzero(uint32_t compact)
{
    return !is_zero(compact & mantissa_max);
}

inline uint8_t log_256(uint32_t mantissa)
{
    BITCOIN_ASSERT_MSG(mantissa <= 0x00ffffff, "mantissa log256 is 4");

    return
        (mantissa > 0x0000ffff ? 3 :
        (mantissa > 0x000000ff ? 2 :
        (mantissa > 0x00000000 ? 1 : 0)));
}

inline bool is_overflow(uint8_t exponent, uint32_t mantissa)
{
    // Overflow if exponent would shift the mantissa more than 32 bytes.
    return !is_zero(mantissa) && (exponent > (32 + 3 - log_256(mantissa)));
}

inline uint32_t shift_low(uint8_t exponent)
{
    BITCOIN_ASSERT(exponent <= 3);
    return to_bits(3 - exponent);
}

inline uint32_t shift_high(uint8_t exponent)
{
    BITCOIN_ASSERT(exponent > 3);
    return to_bits(exponent - 3);
}

inline size_t logical_size(uint256_t value)
{
    auto byte = zero;

    for (; !is_zero(value); ++byte)
        value >>= byte_bits;

    return byte;
}

// Constructors
//-----------------------------------------------------------------------------

compact::compact(uint32_t compact)
{
    overflowed_ = !from_compact(big_, compact);
    normal_ = from_big(big_);
}

compact::compact(const uint256_t& value)
  : big_(value), overflowed_(false)
{
    normal_ = from_big(big_);
}

bool compact::is_overflowed() const
{
    return overflowed_;
}

uint32_t compact::normal() const
{
    return normal_;
}

compact::operator const uint256_t&() const
{
    return big_;
}

// Returns false on overflow, negatives are converted to zero.
bool compact::from_compact(uint256_t& out, uint32_t compact)
{
    //*************************************************************************
    // CONSENSUS: The sign bit is not honored and it instead produces zero.
    // This results from having used a signed data structure for unsigned data.
    //*************************************************************************
    if (is_negated(compact))
    {
        out = 0;
        return true;
    }

    // Mask off the exponent byte and sign to get the mantissa.
    auto mantissa = compact & mantissa_max;

    // Shift off the mantissa and sign to get the exponent byte.
    const auto exponent = static_cast<uint8_t>(compact >> mantissa_bits);

    // Shift the mantissa into the big number.
    if (exponent <= 3)
    {
        mantissa >>= shift_low(exponent);
        out = mantissa;
        return true;
    }

    // Compact has space for more exponent bits than can be represented in a
    // base 256 number represented in 32 bytes, so we trap overflow here.
    if (is_overflow(exponent, mantissa))
        return false;

    out = mantissa;
    out <<= shift_high(exponent);
    return true;
}

uint32_t compact::from_big(const uint256_t& big)
{
    // This value is limited to 32, so exponent cannot overflow.
    auto exponent = static_cast<uint8_t>(logical_size(big));

    // Shift the big number significant digits into the mantissa.
    const auto mantissa64 = exponent <= 3 ?
        static_cast<uint64_t>(big) << shift_low(exponent) :
        static_cast<uint64_t>(big >> shift_high(exponent));

    auto mantissa = static_cast<uint32_t>(mantissa64);

    //*************************************************************************
    // CONSENSUS: Satoshi used a signed implementation to represent unsigned.
    // If the sign bit is set drop it from the mantissa into the exponent.
    // This is intended to prevent the appearance of a negative sign bit.
    //*************************************************************************
    if (is_negated(mantissa))
    {
        exponent++;
        mantissa >>= byte_bits;
    }

    BITCOIN_ASSERT_MSG(is_zero(exponent & first_byte_mask), "size excess");
    BITCOIN_ASSERT_MSG(is_zero(mantissa & mantissa_mask), "value excess");

    // Assemble the compact notation.
    return (exponent << mantissa_bits) | mantissa;
}

} // namespace chain
} // namespace system
} // namespace libbitcoin
