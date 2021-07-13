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
#ifndef LIBBITCOIN_SYSTEM_MATH_IPP
#define LIBBITCOIN_SYSTEM_MATH_IPP

#include <type_traits>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/constraints.hpp>
#include <bitcoin/system/math/sign.hpp>

namespace libbitcoin {
namespace system {

// TODO: test with uintx.

// local
template <typename Factor1, typename Factor2,
    if_integer<Factor1> = true, if_integer<Factor2> = true>
constexpr bool is_negative(Factor1 factor1, Factor2 factor2) noexcept
{
    return is_negative(factor1) != is_negative(factor2);
}

// local
template <typename Dividend, typename Divisor,
    if_integer<Dividend> = true, if_integer<Divisor> = true>
constexpr bool no_remainder(Dividend dividend, Divisor divisor) noexcept
{
    return is_zero(dividend % divisor);
}

// local
template <typename Dividend, typename Divisor,
    if_integer<Dividend> = true, if_integer<Divisor> = true>
constexpr bool is_ceilinged(Dividend dividend, Divisor divisor) noexcept
{
    return is_negative(dividend, divisor) || no_remainder(dividend, divisor);
}

// local
template <typename Dividend, typename Divisor,
    if_integer<Dividend> = true, if_integer<Divisor> = true>
constexpr bool is_floored(Dividend dividend, Divisor divisor) noexcept
{
    return !is_negative(dividend, divisor) || no_remainder(dividend, divisor);
}

template <typename Dividend, typename Divisor,
    if_integer<Dividend>, if_integer<Divisor>>
constexpr auto ceilinged_divide(Dividend dividend, Divisor divisor) noexcept
    -> decltype(dividend / divisor)
{
    return truncated_divide(dividend, divisor) + 
        (is_ceilinged(dividend, divisor) ? 0 : 1);
}

// Always negative logical result, but operands may be unsigned.
// So convert the result to unsigned type of the same size.
template <typename Dividend, typename Divisor,
    if_integer<Dividend>, if_integer<Divisor>>
constexpr auto ceilinged_modulo(Dividend dividend, Divisor divisor) noexcept
    -> typename std::make_signed<decltype(dividend % divisor)>::type
{
    return truncated_modulo(dividend, divisor) -
        (is_ceilinged(dividend, divisor) ? 0 : divisor);
}

template <typename Dividend, typename Divisor,
    if_integer<Dividend>, if_integer<Divisor>>
constexpr auto floored_divide(Dividend dividend, Divisor divisor) noexcept
    -> decltype(dividend / divisor)
{
    return truncated_divide(dividend, divisor) -
        (is_floored(dividend, divisor) ? 0 : 1);
}

// Always positive result.
template <typename Dividend, typename Divisor,
    if_integer<Dividend>, if_integer<Divisor>>
constexpr auto floored_modulo(Dividend dividend, Divisor divisor) noexcept
    -> decltype(dividend % divisor)
{
    return truncated_modulo(dividend, divisor) +
        (is_floored(dividend, divisor) ? 0 : divisor);
}

template <typename Dividend, typename Divisor,
    if_integer<Dividend>, if_integer<Divisor>>
constexpr auto truncated_divide(Dividend dividend, Divisor divisor) noexcept
    -> decltype(dividend / divisor)
{
    return dividend / divisor;
}

template <typename Dividend, typename Divisor,
    if_integer<Dividend>, if_integer<Divisor>>
constexpr auto truncated_modulo(Dividend dividend, Divisor divisor) noexcept
    -> decltype(dividend % divisor)
{
    return dividend % divisor;
}

} // namespace system
} // namespace libbitcoin

#endif
