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
#ifndef LIBBITCOIN_SYSTEM_MATH_SIGN_HPP
#define LIBBITCOIN_SYSTEM_MATH_SIGN_HPP

#include <type_traits>
#include <bitcoin/system/constraints.hpp>

namespace libbitcoin {
namespace system {

/// All operations below support signed and unsigned integer parameters.

/// Cast to smallest signed integer type to hold the maximum unsigned value.
template <typename Integer,
    typename Signed = std::make_signed<Integer>::type,
    if_integer<Integer> = true>
constexpr Signed to_signed(Integer value) noexcept;

/// Cast to smallest unsigned integer type to hold the maximum signed value.
template <typename Integer,
    typename Unsigned = std::make_unsigned<Integer>::type,
    if_integer<Integer> = true>
constexpr Unsigned to_unsigned(Integer value) noexcept;

/// Obtain the absolute value of the integer.
template <typename Integer,
    typename Absolute = std::make_unsigned<Integer>::type,
    if_signed_integer<Integer> = true>
constexpr Absolute absolute(Integer value) noexcept;
template <typename Integer, typename Absolute = Integer,
    if_unsigned_integer<Integer> = true>
constexpr Absolute absolute(Integer value) noexcept;

/// Determine whether the integer is negative.
template <typename Integer, if_signed_integer<Integer> = true>
constexpr bool is_negative(Integer value) noexcept;
template <typename Integer, if_unsigned_integer<Integer> = true>
constexpr bool is_negative(Integer) noexcept;

/// Use !is_lesser(left, right) for (left >= right).
/// Determine whether the left integer is greater than the right.
template <typename Left, typename Right,
    if_same_signed_integer<Left, Right> = true>
constexpr bool is_greater(Left left, Right right) noexcept;
template <typename Left, typename Right,
    if_unsigned_integer<Left> = true, if_signed_integer<Right> = true>
constexpr bool is_greater(Left left, Right right) noexcept;
template <typename Left, typename Right,
    if_signed_integer<Left> = true, if_unsigned_integer<Right> = true>
constexpr bool is_greater(Left left, Right right) noexcept;

/// Use !is_greater(left, right) for (left <= right).
/// Determine whether the left integer is less than the right.
template <typename Left, typename Right,
    if_same_signed_integer<Left, Right> = true>
constexpr bool is_lesser(Left left, Right right) noexcept;
template <typename Left, typename Right,
    if_signed_integer<Left> = true, if_unsigned_integer<Right> = true>
constexpr bool is_lesser(Left left, Right right) noexcept;
template <typename Left, typename Right,
    if_unsigned_integer<Left> = true, if_signed_integer<Right> = true>
constexpr bool is_lesser(Left left, Right right) noexcept;

/// Return the greater of the two values, cast to the Result type.
template<typename Result, typename Left, typename Right,
    if_integer<Left> = true, if_integer<Right> = true>
constexpr Result greater(Left left, Right right) noexcept;

/// Return the lesser of the two values, cast to the Result type.
template<typename Result, typename Left, typename Right,
    if_integer<Left> = true, if_integer<Right> = true>
constexpr Result lesser(Left left, Right right) noexcept;

} // namespace system
} // namespace libbitcoin

#include <bitcoin/system/impl/math/sign.ipp>

#endif

