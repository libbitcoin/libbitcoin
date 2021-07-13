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
#ifndef LIBBITCOIN_SYSTEM_POWER_IPP
#define LIBBITCOIN_SYSTEM_POWER_IPP

#include <cstddef>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/constraints.hpp>
#include <bitcoin/system/math/sign.hpp>

namespace libbitcoin {
namespace system {

// TODO: test all with uintx.

// C++14: All functions below can be made constexpr.
// C++11: no local variables, loops, or multiple returns.

// Returns 0 for undefined (base < 2 or value < 1).
template <typename Exponent, typename Base, typename Value,
    if_integer<Exponent>, if_integer<Base>, if_integer<Value>>
inline Exponent ceilinged_log(Base base, Value value) noexcept
{
    if (base < 2 || value < 1)
        return 0;

    const auto exponent = floored_log<Exponent>(base, value);
    return exponent + ((power<Value>(base, exponent) == value) ? 0 : 1);
}
    
// Returns 0 for undefined (value < 1).
template <typename Exponent, typename Value,
    if_integer<Exponent>, if_integer<Value>>
inline Exponent ceilinged_log2(Value value) noexcept
{
    if (value < 1)
        return 0;

    const auto exponent = floored_log2<Exponent>(value);
    return exponent + ((power2<Value>(exponent) == value) ? 0 : 1);
}

// Returns 0 for undefined (base < 2 or value < 1).
template <typename Exponent, typename Base, typename Value,
    if_integer<Exponent>, if_integer<Base>, if_integer<Value>>
inline Exponent floored_log(Base base, Value value) noexcept
{
    if (base < 2 || value < 1)
        return 0;

    Exponent exponent = 0;
    while (((value /= base)) > 0) { ++exponent; }
    return exponent;
}

// Returns 0 for undefined (value < 1).
template <typename Exponent, typename Value,
    if_integer<Exponent>, if_integer<Value>>
inline Exponent floored_log2(Value value) noexcept
{
    if (value < 1)
        return 0;

    Exponent exponent = 0;
    while (((value >>= 1)) > 0) { ++exponent; };
    return exponent;
}

// Returns 0 for undefined (0^0).
template <typename Value, typename Base, typename Exponent,
    if_integer<Value>, if_integer<Base>, if_integer<Exponent>>
inline Value power(Base base, Exponent exponent) noexcept
{
    if (is_zero(base))
        return 0;

    if (is_zero(exponent))
        return 1;

    if (is_negative(exponent))
        return absolute(base) > 1 ? 0 :
            (is_odd(exponent) && is_negative(base) ? -1 : 1);

    Value value = base;
    while (--exponent > 0) { value *= base; }
    return value;
}

// uintx power2 can be implemented by bit_set(int, exponent) on a new int.
// We could use 64 element lookup table for integrals, instead of shifting.
template <typename Value, typename Exponent,
    if_integer<Value>, if_integer<Exponent>>
inline Value power2(Exponent exponent) noexcept
{
    if (is_zero(exponent))
        return 1;

    if (is_negative(exponent))
        return 0;

    Value value = 2;
    while (--exponent > 0) { value <<= 1; };
    return value;
}

} // namespace system
} // namespace libbitcoin

#endif
