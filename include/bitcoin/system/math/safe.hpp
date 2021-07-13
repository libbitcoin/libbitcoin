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
#ifndef LIBBITCOIN_SYSTEM_MATH_SAFE_HPP
#define LIBBITCOIN_SYSTEM_MATH_SAFE_HPP

#include <limits>
#include <bitcoin/system/constraints.hpp>
#include <bitcoin/system/exceptions.hpp>

namespace libbitcoin {
namespace system {

/// Throws overflow_exception on overflow.
template <typename Integer, if_unsigned_integer<Integer> = true>
Integer safe_multiply(Integer left, Integer right) noexcept(false);

/// Throws overflow_exception on overflow.
template <typename Integer, if_unsigned_integer<Integer> = true>
Integer safe_add(Integer left, Integer right) noexcept(false);

/// Throws underflow_exception on underflow.
template <typename Integer, if_unsigned_integer<Integer> = true>
Integer safe_subtract(Integer left, Integer right) noexcept(false);

/// Throws overflow_exception on overflow.
template <typename Integer, if_unsigned_integer<Integer> = true>
void safe_increment(Integer& value) noexcept(false);

/// Throws underflow_exception on underflow.
template <typename Integer, if_unsigned_integer<Integer> = true>
void safe_decrement(Integer& value) noexcept(false);

/// Throws range_exception if From value is above or below To size limits.
template <typename To, typename From,
    if_integer<To> = true, if_integer<From> = true>
To safe_cast(From value) noexcept(false);

} // namespace system
} // namespace libbitcoin

#include <bitcoin/system/impl/math/safe.ipp>

#endif
