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
#ifndef LIBBITCOIN_SYSTEM_CONSTRAINTS_HPP
#define LIBBITCOIN_SYSTEM_CONSTRAINTS_HPP

#include <cstdint>
#include <limits>
#include <string>
#include <type_traits>
#include <vector>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {

// C++14: use enable_if_t.
template <bool Bool, typename Type = void>
using enable_if_type = typename std::enable_if<Bool, Type>::type;

template <size_t Value>
using if_odd = enable_if_type<is_odd(Value), bool>;

template <size_t Value>
using if_even = enable_if_type<is_even(Value), bool>;

template <size_t Value>
using if_non_zero = enable_if_type<!is_zero(Value), bool>;

template <size_t Value, size_t Size>
using if_size = enable_if_type<Value == Size, bool>;

template <size_t Left, size_t Right>
using if_greater = enable_if_type<(Left > Right), bool>;

template <size_t Left, size_t Right>
using if_not_greater = enable_if_type<!(Left > Right), bool>;

template <size_t Left, size_t Right>
using if_lesser = enable_if_type<(Left < Right), bool>;

template <size_t Left, size_t Right>
using if_not_lesser = enable_if_type<!(Left < Right), bool>;

template <typename Type>
using if_byte = enable_if_type<!(width<Type>() > byte_bits), bool>;

template <typename Type>
using if_bytes = enable_if_type<(width<Type>() > byte_bits), bool>;

template <typename Base, typename Type>
using if_base_of = enable_if_type<
    std::is_base_of<Base, Type>::value, bool>;

template <typename Type>
using if_byte_insertable = enable_if_type<
    std::is_base_of<std::vector<uint8_t>, Type>::value ||
    std::is_base_of<std::string, Type>::value, bool>;

template <typename Type>
using if_integer = enable_if_type<is_integer<Type>(), bool>;

template <typename Type>
using if_integral_integer = enable_if_type<is_integer<Type>() &&
    std::is_integral<Type>::value, bool>;

template <typename Type>
using if_non_integral_integer = enable_if_type<is_integer<Type>() &&
    !std::is_integral<Type>::value, bool>;

template <typename Type>
using if_signed_integer = enable_if_type<is_integer<Type>() &&
    std::is_signed<Type>::value, bool>;

template <typename Type>
using if_unsigned_integer = enable_if_type<is_integer<Type>() &&
    !std::is_signed<Type>::value, bool>;

template <typename Left, typename Right>
using if_same_signed_integer = enable_if_type<
    is_integer<Left>() && is_integer<Right>() &&
    (std::is_signed<Left>::value == std::is_signed<Right>::value), bool>;

// Derive from 'noncopyable' to preclude copy construct and assign semantics in
// the derived class. Move semantics are preserved if they are defined.
class BC_API noncopyable
{
public:
    noncopyable(const noncopyable&) = delete;
    void operator=(const noncopyable&) = delete;
protected:
    noncopyable() {}
};

} // namespace system
} // namespace libbitcoin

#endif
