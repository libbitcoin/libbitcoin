/**
 * Copyright (c) 2011-2021 libbitcoin developers (see AUTHORS)
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
#ifndef LIBBITCOIN_SYSTEM_MATH_BITS_IPP
#define LIBBITCOIN_SYSTEM_MATH_BITS_IPP

#include <cstddef>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/constraints.hpp>
#include <bitcoin/system/math/division.hpp>
#include <bitcoin/system/math/limits.hpp>
#include <bitcoin/system/math/sign.hpp>

namespace libbitcoin {
namespace system {

// width (see constants.hpp for width<>()).

template <typename Integer, if_integer<Integer>>
inline size_t bit_width(Integer value) noexcept
{
    // C++20: std::bit_width.
    return add1(floored_log2(value));
}

template <typename Integer, if_integer<Integer>>
inline size_t byte_width(Integer value) noexcept
{
    return ceilinged_divide(bit_width(value), byte_bits);
}

// complements

template <typename Integer, if_integer<Integer>>
constexpr Integer ones_complement(Integer value) noexcept
{
    return ~value;
}

template <typename Integer, if_integer<Integer>>
constexpr Integer twos_complement(Integer value) noexcept
{
    return add1(ones_complement(value));
}

// bit

template <typename Value, if_integer<Value>>
constexpr Value bit_all()
{
    return ones_complement<Value>(0);
}

template <typename Value, if_integer<Value>>
constexpr Value bit_hi()
{
    // sub1 for size-to-index translation.
    return to_int<Value>(true) << sub1(width<Value>());
}

template <typename Value, if_integer<Value>>
constexpr Value bit_lo()
{
    return to_int<Value>(true) << zero;
}

template <typename Value, if_integer<Value>>
constexpr Value bit_left(size_t offset)
{
    return bit_hi<Value>() >> offset;
}

template <typename Value, if_integer<Value>>
constexpr Value bit_right(size_t offset)
{
    return bit_lo<Value>() << offset;
}

// get

template <typename Value, if_integer<Value>>
constexpr bool get_right(Value value, size_t offset)
{
    return !is_zero(value & bit_right<Value>(offset));
}

template <typename Value, if_integer<Value>>
constexpr bool get_left(Value value, size_t offset)
{
    return !is_zero(value & bit_left<Value>(offset));
}

// set

template <typename Value, if_integer<Value>>
constexpr Value set_left(Value& target, size_t offset, bool state)
{
    // C++14: local variables allowed in constexpr.
    return state ? ((target |= bit_left<Value>(offset))) :
        ((target &= ones_complement(bit_left<Value>(offset))));
}

template <typename Value, if_integer<Value>>
constexpr Value set_left(const Value& target, size_t offset, bool state)
{
    // C++14: local variables allowed in constexpr.
    return state ? target | bit_left<Value>(offset) :
        target & ones_complement(bit_left<Value>(offset));
}

template <typename Value, if_integer<Value>>
constexpr Value set_right(Value& target, size_t offset, bool state)
{
    // C++14: local variables allowed in constexpr.
    return state ? ((target |= bit_right<Value>(offset))) :
        ((target &= ones_complement(bit_right<Value>(offset))));
}

template <typename Value, if_integer<Value>>
constexpr Value set_right(const Value& target, size_t offset, bool state)
{
    // C++14: local variables allowed in constexpr.
    return state ? target | bit_right<Value>(offset) :
        target & ones_complement(bit_right<Value>(offset));
}

// flag

template <typename Value, if_integer<Value>>
constexpr Value flag_left(size_t bits)
{
    return ones_complement(bit_all<Value>() >> bits);
}

template <typename Value, if_integer<Value>>
constexpr Value flag_left(Value& value, size_t bits)
{
    return ((value |= flag_left<Value>(bits)));
}

template <typename Value, if_integer<Value>>
constexpr Value flag_left(const Value& value, size_t bits)
{
    return value | flag_left<Value>(bits);
}

template <typename Value, if_integer<Value>>
constexpr Value flag_right(size_t bits)
{
    return ones_complement(bit_all<Value>() << bits);
}

template <typename Value, if_integer<Value>>
constexpr Value flag_right(Value& value, size_t bits)
{
    return ((value |= flag_right<Value>(bits)));
}

template <typename Value, if_integer<Value>>
constexpr Value flag_right(const Value& value, size_t bits)
{
    return value | flag_right<Value>(bits);
}

// mask

template <typename Value, if_integer<Value>>
constexpr Value mask_left(size_t bits)
{
    return bit_all<Value>() >> bits;
}

template <typename Value, if_integer<Value>>
constexpr Value mask_left(Value& value, size_t bits)
{
    return ((value &= mask_left<Value>(bits)));
}

template <typename Value, if_integer<Value>>
constexpr Value mask_left(const Value& value, size_t bits)
{
    return value & mask_left<Value>(bits);
}

template <typename Value, if_integer<Value>>
constexpr Value mask_right(size_t bits)
{
    return bit_all<Value>() << bits;
}

template <typename Value, if_integer<Value>>
constexpr Value mask_right(Value& value, size_t bits)
{
    return ((value &= mask_right<Value>(bits)));
}

template <typename Value, if_integer<Value>>
constexpr Value mask_right(const Value& value, size_t bits)
{
    return value & mask_right<Value>(bits);
}

// rotate

// C++20: std::rotl/rotr can replace rotate_left/rotate_right.
// C++20: std::rotl/rotr can replace rotate_left/rotate_right.

template <typename Value, if_integer<Value>>
constexpr Value rotate_left(Value& value, size_t shift)
{
    // C++14: local variables allowed in constexpr.
    constexpr auto bits = width<Value>();
    return ((value = (value << (shift % bits)) |
        (value >> (bits - (shift % bits)))));
}

template <typename Value, if_integer<Value>>
constexpr Value rotate_left(const Value& value, size_t shift)
{
    // C++14: local variables allowed in constexpr.
    constexpr auto bits = width<Value>();
    return (value << (shift % bits)) | (value >> (bits - (shift % bits)));
}

template <typename Value, if_integer<Value>>
constexpr Value rotate_right(Value& value, size_t shift)
{
    // C++14: local variables allowed in constexpr.
    constexpr auto bits = width<Value>();
    return ((value = (value << (bits - (shift % bits)) |
        (value >> (shift % bits)))));
}

template <typename Value, if_integer<Value>>
constexpr Value rotate_right(const Value& value, size_t shift)
{
    // C++14: local variables allowed in constexpr.
    constexpr auto bits = width<Value>();
    return (value << (bits - (shift % bits))) | (value >> (shift % bits));
}

// shift

// C++ standard: "The behavior is undefined if the right operand is negative,
// or greater than or equal to the width of the promoted left operand."
// These functions make explicit the intended behavior in that condition.
// overflow true is logically equivalent to shifting by one the specified
// shift number of times, which results in a zero if shift >= width(value).
// overflow true shifts shift % width(value), which cannot zeroize the value.
// These are both default behaviors of different compilers.

template <typename Value, if_integer<Value>>
constexpr Value shift_left(Value& value, size_t shift, bool overflow)
{
    constexpr auto bits = width<Value>();
    return (overflow && shift >= bits) ? ((value = 0)) : ((value <<= (shift % bits)));
}

template <typename Value, if_integer<Value>>
constexpr Value shift_left(const Value& value, size_t shift, bool overflow)
{
    constexpr auto bits = width<Value>();
    return (overflow && shift >= bits) ? 0 : value << (shift % bits);
}

template <typename Value, if_integer<Value>>
constexpr Value shift_right(Value& value, size_t shift, bool overflow)
{
    constexpr auto bits = width<Value>();
    return (overflow && shift >= bits) ? ((value = 0)) : ((value >>= (shift % bits)));
}

template <typename Value, if_integer<Value>>
constexpr Value shift_right(const Value& value, size_t shift, bool overflow)
{
    constexpr auto bits = width<Value>();
    return (overflow && shift >= bits) ? 0 : value >> (shift % bits);
}

} // namespace system
} // namespace libbitcoin

#endif
