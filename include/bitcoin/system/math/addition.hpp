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
#ifndef LIBBITCOIN_SYSTEM_MATH_ADDITION_HPP
#define LIBBITCOIN_SYSTEM_MATH_ADDITION_HPP

#include <bitcoin/system/constraints.hpp>

namespace libbitcoin {
namespace system {

/// left + right after cast of each to Result type.
template <typename Result, typename Left, typename Right,
    if_same_signed_integer<Left, Right> = true>
constexpr Result add(Left left, Right right) noexcept;

/// left - right after cast of each to Result type.
template <typename Result, typename Left, typename Right,
    if_same_signed_integer<Left, Right> = true>
constexpr Result subtract(Left left, Right right) noexcept;

/// Integer maximum if would overflow return, otherwise the sum.
template <typename Integer, if_unsigned_integer<Integer> = true>
constexpr Integer ceilinged_add(Integer left, Integer right) noexcept;

/// Integer minimum (0) if would underflow, otherwise the difference.
template <typename Integer, if_unsigned_integer<Integer> = true>
constexpr Integer floored_subtract(Integer left, Integer right) noexcept;

} // namespace system
} // namespace libbitcoin

#include <bitcoin/system/impl/math/addition.ipp>

#endif
