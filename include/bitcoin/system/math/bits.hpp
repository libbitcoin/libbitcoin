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
#ifndef LIBBITCOIN_SYSTEM_MATH_BITS_HPP
#define LIBBITCOIN_SYSTEM_MATH_BITS_HPP

#include <cstddef>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/constraints.hpp>

namespace libbitcoin {
namespace system {

/// The number of bits required to store the value.
template <typename Integer, if_integer<Integer> = true>
inline size_t bit_width(Integer value) noexcept;

/// The number of bytes required to store the value.
template <typename Integer, if_integer<Integer> = true>
inline size_t byte_width(Integer value) noexcept;

/// Two's complement (~n), the bit inversion of any integer.
template <typename Integer, if_integer<Integer> = true>
constexpr Integer ones_complement(Integer value) noexcept;

/// Two's complement (~n+1), the absolute value of an unsigned negative value.
template <typename Integer, if_integer<Integer> = true>
constexpr Integer twos_complement(Integer value) noexcept;

/// All bits set (maximum value for signed, minimum value for unsigned).
template <typename Value, if_integer<Value> = true>
constexpr Value bit_all();

/// An instance of value with only the high order bit set (0x8...).
template <typename Value, if_integer<Value> = true>
constexpr Value bit_hi();

/// An instance of value with only the low order bit set (0x...1).
template <typename Value, if_integer<Value> = true>
constexpr Value bit_lo();

/// A single bit bitmask, offset relative to highest order bit.
template <typename Value, if_integer<Value> = true>
constexpr Value bit_left(size_t offset);

/// A single bit bitmask, offset relative to lowest order bit.
template <typename Value, if_integer<Value> = true>
constexpr Value bit_right(size_t offset);

/// Extract a bit from value, offset relative to highest order bit.
template <typename Value, if_integer<Value> = true>
constexpr bool get_left(Value value, size_t offset=zero);

/// Extract a bit from value, offset relative to lowest order bit.
template <typename Value, if_integer<Value> = true>
constexpr bool get_right(Value value, size_t offset=zero);

/// Set a bit in target, offset relative to highest order bit.
template <typename Value, if_integer<Value> = true>
constexpr Value set_left(Value& target, size_t offset=zero, bool state=true);
template <typename Value, if_integer<Value> = true>
constexpr Value set_left(const Value& target, size_t offset=zero,
    bool state=true);

/// Set a bit in target, offset relative to lowest order bit.
template <typename Value, if_integer<Value> = true>
constexpr Value set_right(Value& target, size_t offset=zero, bool state=true);
template <typename Value, if_integer<Value> = true>
constexpr Value set_right(const Value& target, size_t offset=zero,
    bool state=true);

/// A set of bitflags with high order count of bits set.
template <typename Value, if_integer<Value> = true>
constexpr Value flag_left(size_t bits);
template <typename Value, if_integer<Value> = true>
constexpr Value flag_left(Value& target, size_t bits);
template <typename Value, if_integer<Value> = true>
constexpr Value flag_left(const Value& target, size_t bits);

/// A set of bitflags with low order count of bits set.
template <typename Value, if_integer<Value> = true>
constexpr Value flag_right(size_t bits);
template <typename Value, if_integer<Value> = true>
constexpr Value flag_right(Value& target, size_t bits);
template <typename Value, if_integer<Value> = true>
constexpr Value flag_right(const Value& target, size_t bits);

/// A set of bitmasks with high order count of bits unset.
template <typename Value, if_integer<Value> = true>
constexpr Value mask_left(size_t bits);
template <typename Value, if_integer<Value> = true>
constexpr Value mask_left(Value& target, size_t bits);
template <typename Value, if_integer<Value> = true>
constexpr Value mask_left(const Value& target, size_t bits);

/// A set of bitmasks with low order count of bits unset.
template <typename Value, if_integer<Value> = true>
constexpr Value mask_right(size_t bits);
template <typename Value, if_integer<Value> = true>
constexpr Value mask_right(Value& target, size_t bits);
template <typename Value, if_integer<Value> = true>
constexpr Value mask_right(const Value& target, size_t bits);

/// Rotate the bits of Value to the left by amount 'shift'.
template <typename Value, if_integer<Value> = true>
constexpr Value rotate_left(Value& value, size_t shift);
template <typename Value, if_integer<Value> = true>
constexpr Value rotate_left(const Value& value, size_t shift);

/// Rotate the bits of Value to the right by amount 'shift'.
template <typename Value, if_integer<Value> = true>
constexpr Value rotate_right(Value& value, size_t shift);
template <typename Value, if_integer<Value> = true>
constexpr Value rotate_right(const Value& value, size_t shift);

/// Shift the bits of Value to the left by amount 'shift'.
/// Set 'overflow' to false to modulo shift by Value width.
template <typename Value, if_integer<Value> = true>
constexpr Value shift_left(Value& value, size_t shift, bool overflow=true);
template <typename Value, if_integer<Value> = true>
constexpr Value shift_left(const Value& value, size_t shift, bool overflow=true);

/// Shift the bits of Value to the right by amount 'shift'.
/// Set 'overflow' to false to modulo shift by Value width.
template <typename Value, if_integer<Value> = true>
constexpr Value shift_right(Value& value, size_t shift, bool overflow=true);
template <typename Value, if_integer<Value> = true>
constexpr Value shift_right(const Value& value, size_t shift, bool overflow=true);

} // namespace system
} // namespace libbitcoin

#include <bitcoin/system/impl/math/bits.ipp>

#endif
