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
#ifndef LIBBITCOIN_SYSTEM_MATH_DIVISION_HPP
#define LIBBITCOIN_SYSTEM_MATH_DIVISION_HPP

#include <type_traits>
#include <bitcoin/system/constraints.hpp>

namespace libbitcoin {
namespace system {

/// All operations below support signed and unsigned parameters.
/// github.com/libbitcoin/libbitcoin-system/wiki/Integer-Division-Unraveled

/// Obtain the ceilinged (rounded up) integer modulo quotient.
/// This is equivalent to c++ % for negative quotients.
template <typename Dividend, typename Divisor,
    if_integer<Dividend> = true, if_integer<Divisor> = true>
constexpr auto ceilinged_divide(Dividend dividend, Divisor divisor) noexcept
    -> decltype(dividend / divisor);

/// Obtain the ceilinged (rounded up) integer modulo quotient.
/// This is equivalent to c++ % for negative quotients.
/// The result is signed because positive operands yield a negative result.
/// Native operators would convert a negative logical result to its two's
/// complement representation, which is not generally useful as the negative.
template <typename Dividend, typename Divisor,
    if_integer<Dividend> = true, if_integer<Divisor> = true>
constexpr auto ceilinged_modulo(Dividend dividend, Divisor divisor) noexcept
    -> typename std::make_signed<decltype(dividend % divisor)>::type;

/// Obtain the floored (rounded down) integer modulo quotient.
/// This is equivalent to c++ % for positive quotients.
template <typename Dividend, typename Divisor,
    if_integer<Dividend> = true, if_integer<Divisor> = true>
constexpr auto floored_divide(Dividend dividend, Divisor divisor) noexcept
    -> decltype(dividend / divisor);

/// Obtain the floorded (rounded down) integer modulo quotient.
/// This is equivalent to c++ % for positive quotients.
template <typename Dividend, typename Divisor,
    if_integer<Dividend> = true, if_integer<Divisor> = true>
constexpr auto floored_modulo(Dividend dividend, Divisor divisor) noexcept
    -> decltype(dividend % divisor);

/// Obtain the truncated (rounded toward zero) integer quotient.
/// This is equivalent to c++ /.
template <typename Dividend, typename Divisor,
    if_integer<Dividend> = true, if_integer<Divisor> = true>
constexpr auto truncated_divide(Dividend dividend, Divisor divisor) noexcept
    -> decltype(dividend / divisor);

/// Obtain the truncated (rounded toward zero) integer divide remainder.
/// This is equivalent to c++ %.
template <typename Dividend, typename Divisor,
    if_integer<Dividend> = true, if_integer<Divisor> = true>
constexpr auto truncated_modulo(Dividend dividend, Divisor divisor) noexcept
    -> decltype(dividend % divisor);

} // namespace system
} // namespace libbitcoin

#include <bitcoin/system/impl/math/division.ipp>

#endif

