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
#include "test.hpp"

#include <string>
#include <type_traits>
#include <vector>

// The msvc++ compiler dislikes std::is_same outside of template arguments.
// But visual studio intellisense properly evaluations these assertions.

class base : noncopyable { int foo; };
class not_derived { int bar; };
class derived : base { int bar; };

template <typename Type>
constexpr bool is_true()
{
    return !std::is_same<Type, std::enable_if<false, bool>>::value;
}

static_assert(is_true<if_odd<1>>(), "");
static_assert(is_true<if_odd<3>>(), "");
static_assert(is_true<if_odd<5u>>(), "");
static_assert(is_true<if_odd<0xff>>(), "");
////static_assert(!is_true<if_odd<0>>(), "");
////static_assert(!is_true<if_odd<2>>(), "");
////static_assert(!is_true<if_odd<42u>>(), "");
////static_assert(!is_true<if_odd<0xfe>>(), "");

static_assert(is_true<if_even<0>>(), "");
static_assert(is_true<if_even<2>>(), "");
static_assert(is_true<if_even<42u>>(), "");
static_assert(is_true<if_even<0xfe>>(), "");
////static_assert(!is_true<if_even<1>>(), "");
////static_assert(!is_true<if_even<3>>(), "");
////static_assert(!is_true<if_even<5u>>(), "");
////static_assert(!is_true<if_even<0xff>>(), "");

static_assert(is_true<if_non_zero<1>>(), "");
////static_assert(!is_true<if_non_zero<0>>(), "");

static_assert(is_true<if_size<0, 0>>(), "");
static_assert(is_true<if_size<1, 1>>(), "");
static_assert(is_true<if_size<42, 42>>(), "");
////static_assert(!is_true<if_size<0, 1>>(), "");
////static_assert(!is_true<if_size<1, 42>>(), "");
////static_assert(!is_true<if_size<42, 1>>(), "");

static_assert(is_true<if_greater<1, 0>>(), "");
////static_assert(!is_true<if_greater<0, 0>>(), "");
////static_assert(!is_true<if_greater<0, 1>>(), "");

static_assert(is_true<if_not_greater<0, 0>>(), "");
static_assert(is_true<if_not_greater<0, 1>>(), "");
////static_assert(!is_true<if_not_greater<1, 0>>(), "");

static_assert(is_true<if_lesser<0, 1>>(), "");
////static_assert(!is_true<if_lesser<1, 0>>(), "");
////static_assert(!is_true<if_lesser<0, 0>>(), "");

static_assert(is_true<if_not_lesser<0, 0>>(), "");
static_assert(is_true<if_not_lesser<1, 0>>(), "");
////static_assert(!is_true<if_not_lesser<0, 1>>(), "");

static_assert(is_true<if_byte<bool>>(), "");
static_assert(is_true<if_byte<char>>(), "");
static_assert(is_true<if_byte<uint8_t>>(), "");
////static_assert(!is_true<if_byte<float>>(), "");
////static_assert(!is_true<if_byte<double>>(), "");
////static_assert(!is_true<if_byte<int16_t>>(), "");
////static_assert(!is_true<if_byte<uint32_t>>(), "");
////static_assert(!is_true<if_byte<int64_t>>(), "");
////static_assert(!is_true<if_byte<size_t>>(), "");
////static_assert(!is_true<if_byte<base>>(), "");

static_assert(is_true<if_bytes<int16_t>>(), "");
static_assert(is_true<if_bytes<uint32_t>>(), "");
static_assert(is_true<if_bytes<int64_t>>(), "");
static_assert(is_true<if_bytes<size_t>>(), "");
static_assert(is_true<if_bytes<float>>(), "");
static_assert(is_true<if_bytes<double>>(), "");
////static_assert(!is_true<if_bytes<bool>>(), "");
////static_assert(!is_true<if_bytes<char>>(), "");
////static_assert(!is_true<if_bytes<uint8_t>>(), "");
////static_assert(!is_true<if_bytes<base>>(), "");

static_assert(is_true<if_base_of<base, base>>(), "");
static_assert(is_true<if_base_of<base, derived>>(), "");
////static_assert(!is_true<if_base_of<base, not_derived>>(), "");
////static_assert(!is_true<if_base_of<uint8_t, uint8_t>>(), "");
////static_assert(!is_true<if_base_of<uint8_t, uint32_t>>(), "");
////static_assert(!is_true<if_base_of<float, double>>(), "");

static_assert(is_true<if_byte_insertable<std::string>>(), "");
static_assert(is_true<if_byte_insertable<std::vector<uint8_t>>>(), "");
////static_assert(!is_true<if_byte_insertable<std::u32string>>(), "");
////static_assert(!is_true<if_byte_insertable<std::vector<uint32_t>>>(), "");
////static_assert(!is_true<if_byte_insertable<uint32_t>>(), "");

static_assert(is_true<if_integer<char>>(), "");
static_assert(is_true<if_integer<int>>(), "");
static_assert(is_true<if_integer<uint8_t>>(), "");
static_assert(is_true<if_integer<uint32_t>>(), "");
static_assert(is_true<if_integer<int32_t>>(), "");
static_assert(is_true<if_integer<uintx>>(), "");
////static_assert(!is_true<if_integer<bool>>(), "");
////static_assert(!is_true<if_integer<float>>(), "");
////static_assert(!is_true<if_integer<double>>(), "");
////static_assert(!is_true<if_integer<base>>(), "");

static_assert(is_true<if_integral_integer<char>>(), "");
static_assert(is_true<if_integral_integer<int>>(), "");
static_assert(is_true<if_integral_integer<uint8_t>>(), "");
static_assert(is_true<if_integral_integer<uint32_t>>(), "");
static_assert(is_true<if_integral_integer<int32_t>>(), "");
////static_assert(!is_true<if_integral_integer<bool>>(), "");
////static_assert(!is_true<if_integral_integer<uintx>>(), "");
////static_assert(!is_true<if_integral_integer<float>>(), "");
////static_assert(!is_true<if_integral_integer<double>>(), "");
////static_assert(!is_true<if_integral_integer<base>>(), "");

static_assert(is_true<if_non_integral_integer<uintx>>(), "");
static_assert(is_true<if_non_integral_integer<uint256_t>>(), "");
////static_assert(!is_true<if_non_integral_integer<bool>>(), "");
////static_assert(!is_true<if_non_integral_integer<char>>(), "");
////static_assert(!is_true<if_non_integral_integer<int>>(), "");
////static_assert(!is_true<if_non_integral_integer<uint8_t>>(), "");
////static_assert(!is_true<if_non_integral_integer<uint32_t>>(), "");
////static_assert(!is_true<if_non_integral_integer<int32_t>>(), "");
////static_assert(!is_true<if_non_integral_integer<float>>(), "");
////static_assert(!is_true<if_non_integral_integer<double>>(), "");
////static_assert(!is_true<if_non_integral_integer<base>>(), "");

static_assert(is_true<if_signed_integer<char>>(), "");
static_assert(is_true<if_signed_integer<int>>(), "");
static_assert(is_true<if_signed_integer<int16_t>>(), "");
static_assert(is_true<if_signed_integer<int32_t>>(), "");
static_assert(is_true<if_signed_integer<int64_t>>(), "");
////static_assert(!is_true<if_signed_integer<bool>>(), "");
////static_assert(!is_true<if_signed_integer<float>>(), "");
////static_assert(!is_true<if_signed_integer<double>>(), "");
////static_assert(!is_true<if_signed_integer<uint16_t>>(), "");
////static_assert(!is_true<if_signed_integer<size_t>>(), "");
////static_assert(!is_true<if_signed_integer<base>>(), "");

static_assert(is_true<if_unsigned_integer<uint16_t>>(), "");
static_assert(is_true<if_unsigned_integer<size_t>>(), "");
////static_assert(!is_true<if_unsigned_integer<bool>>(), "");
////static_assert(!is_true<if_unsigned_integer<char>>(), "");
////static_assert(!is_true<if_unsigned_integer<int>>(), "");
////static_assert(!is_true<if_unsigned_integer<float>>(), "");
////static_assert(!is_true<if_unsigned_integer<double>>(), "");
////static_assert(!is_true<if_unsigned_integer<int16_t>>(), "");
////static_assert(!is_true<if_unsigned_integer<int32_t>>(), "");
////static_assert(!is_true<if_unsigned_integer<int64_t>>(), "");
////static_assert(!is_true<if_unsigned_integer<base>>(), "");

static_assert(is_true<if_same_signed_integer<int, char>>(), "");
static_assert(is_true<if_same_signed_integer<uint16_t, uint8_t>>(), "");
////static_assert(!is_true<if_same_signed_integer<uint16_t, bool>>(), "");
////static_assert(!is_true<if_same_signed_integer<size_t, bool>>(), "");
////static_assert(!is_true<if_same_signed_integer<bool, bool>>(), "");
////static_assert(!is_true<if_same_signed_integer<char, bool>>(), "");
////static_assert(!is_true<if_same_signed_integer<int, bool>>(), "");
////static_assert(!is_true<if_same_signed_integer<float, bool>>(), "");
////static_assert(!is_true<if_same_signed_integer<double, bool>>(), "");
////static_assert(!is_true<if_same_signed_integer<int16_t, bool>>(), "");
////static_assert(!is_true<if_same_signed_integer<int32_t, bool>>(), "");
////static_assert(!is_true<if_same_signed_integer<int64_t, bool>>(), "");
////static_assert(!is_true<if_same_signed_integer<base, bool>>(), "");

static_assert(is_true<if_same_signed_integer<char, char>>(), "");
static_assert(is_true<if_same_signed_integer<char, int>>(), "");
static_assert(is_true<if_same_signed_integer<char, int16_t>>(), "");
static_assert(is_true<if_same_signed_integer<char, int32_t>>(), "");
static_assert(is_true<if_same_signed_integer<char, int64_t>>(), "");
////static_assert(!is_true<if_same_signed_integer<char, uint16_t>>(), "");
////static_assert(!is_true<if_same_signed_integer<char, size_t>>(), "");
////static_assert(!is_true<if_same_signed_integer<char, float>>(), "");
////static_assert(!is_true<if_same_signed_integer<char, double>>(), "");
////static_assert(!is_true<if_same_signed_integer<char, base>>(), "");

static_assert(std::is_copy_constructible<not_derived>::value, "");
static_assert(!std::is_copy_constructible<noncopyable>::value, "");
static_assert(!std::is_copy_constructible<derived>::value, "");
static_assert(!std::is_copy_constructible<base>::value, "");