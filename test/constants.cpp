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

static_assert(zero == 0u, "");
static_assert(one == 1u, "");
static_assert(two == 2u, "");
static_assert(byte_bits == 8u, "");
static_assert(negative_one == -1, "");

static_assert(is_zero(0), "");
static_assert(!is_zero(1u), "");
static_assert(!is_zero(0xff), "");
static_assert(std::is_same<decltype(is_zero<int16_t>(0)), bool>::value, "");

static_assert(is_one(1), "");
static_assert(!is_one(0u), "");
static_assert(!is_one(0xff), "");
static_assert(std::is_same<decltype(is_one<int16_t>(0)), bool>::value, "");

static_assert(lo_bit(0) == 0, "");
static_assert(lo_bit(2u) == 0, "");
static_assert(lo_bit(42) == 0, "");
static_assert(lo_bit(max_uint32 - 1u) == 0, "");
static_assert(lo_bit(1) == 1, "");
static_assert(lo_bit(3u) == 1, "");
static_assert(lo_bit(5) == 1, "");
static_assert(lo_bit(max_uint32) == 1, "");
static_assert(std::is_same<decltype(lo_bit<int16_t>(0)), int16_t>::value, "");

static_assert(is_even(0), "");
static_assert(is_even(2u), "");
static_assert(is_even(42), "");
static_assert(is_even(max_uint32 - 1u), "");
static_assert(!is_even(1), "");
static_assert(!is_even(3u), "");
static_assert(!is_even(0xff), "");
static_assert(!is_even(max_uint32), "");
static_assert(std::is_same<decltype(is_even<int16_t>(0)), bool>::value, "");

static_assert(is_odd(1), "");
static_assert(is_odd(3u), "");
static_assert(is_odd(5), "");
static_assert(is_odd(max_uint32), "");
static_assert(!is_odd(0u), "");
static_assert(!is_odd(2), "");
static_assert(!is_odd(10), "");
static_assert(!is_odd(max_uint32 - 1u), "");
static_assert(std::is_same<decltype(is_odd<int16_t>(0)), bool>::value, "");

static_assert(is_integer<uint8_t>(), "");
static_assert(is_integer<uint16_t>(), "");
static_assert(is_integer<uint32_t>(), "");
static_assert(is_integer<uint64_t>(), "");
static_assert(is_integer<size_t>(), "");
static_assert(is_integer<int8_t>(), "");
static_assert(is_integer<int16_t>(), "");
static_assert(is_integer<int32_t>(), "");
static_assert(is_integer<int64_t>(), "");
static_assert(is_integer<char>(), "");
static_assert(is_integer<wchar_t>(), "");
static_assert(!is_integer<bool>(), "");
static_assert(!is_integer<std::string> (), "");

constexpr void* pointer = "";
constexpr void* null_pointer = nullptr;
static_assert(is_null(nullptr), "");
static_assert(is_null(null_pointer), "");
static_assert(is_null(std::nullptr_t{}), "");
static_assert(!is_null(pointer), "");
static_assert(std::is_same<decltype(is_null(nullptr)), bool>::value, "");

static_assert(to_bits(0) == 0 * 8, "");
static_assert(to_bits(1u) == 1u * 8u, "");
static_assert(to_bits(-42) == -42 * 8, "");
static_assert(to_bits(0xff) == 0xff * 8, "");
static_assert(std::is_same<decltype(to_bits<int16_t>(0)), int16_t>::value, "");

static_assert(to_bytes(0) == 0 / 8, "");
static_assert(to_bytes(1u) == 1u / 8u, "");
static_assert(to_bytes(-42) == -42 / 8, "");
static_assert(to_bytes(0xff) == 0xff / 8, "");
static_assert(std::is_same<decltype(to_bytes<int16_t>(0)), int16_t>::value, "");

static_assert(to_half(0) == 0 / 2, "");
static_assert(to_half(1u) == 1u / 2u, "");
static_assert(to_half(-42) == -42 / 2, "");
static_assert(to_half(0xff) == 0xff / 2, "");
static_assert(std::is_same<decltype(to_half<int16_t>(0)), int16_t>::value, "");

static_assert(to_int(true) == 1, "");
static_assert(to_int(false) == 0, "");
static_assert(std::is_same<decltype(to_int<int16_t>(0)), int16_t>::value, "");

static_assert(to_bool(1), "");
static_assert(to_bool(-1), "");
static_assert(to_bool(-42), "");
static_assert(to_bool(42), "");
static_assert(!to_bool(0), "");
static_assert(std::is_same<decltype(to_bool<int16_t>(0)), bool>::value, "");

static_assert(add1(0) == 0 + 1, "");
static_assert(add1(1u) == 1u + 1u, "");
static_assert(add1(-42) == -42 + 1, "");
static_assert(add1(0xff) == 0xff + 1, "");
static_assert(std::is_same<decltype(add1<int16_t>(0)), int16_t>::value, "");

static_assert(sub1(0xff) == 0xff - 1, "");
static_assert(sub1(1u) == 1u - 1u, "");
static_assert(sub1(-42) == -42 - 1, "");
static_assert(sub1(0xff) == 0xff - 1, "");
static_assert(std::is_same<decltype(sub1<int16_t>(0)), int16_t>::value, "");

constexpr uint32_t value = 42;
static_assert(width<bool>() == to_bits(sizeof(char)), "");
static_assert(width<char>() == to_bits(sizeof(char)), "");
static_assert(width<short>() == to_bits(sizeof(short)), "");
static_assert(width<int>() == to_bits(sizeof(int)), "");
static_assert(width<long>() == to_bits(sizeof(long)), "");
static_assert(width<long long>() == to_bits(sizeof(long long)), "");
static_assert(width<signed char>() == to_bits(sizeof(signed char)), "");
static_assert(width<signed short>() == to_bits(sizeof(signed short)), "");
static_assert(width<signed int>() == to_bits(sizeof(signed int)), "");
static_assert(width<signed long>() == to_bits(sizeof(signed long)), "");
static_assert(width<signed long long>() == to_bits(sizeof(signed long long)), "");
static_assert(width<unsigned char>() == to_bits(sizeof(unsigned char)), "");
static_assert(width<unsigned short>() == to_bits(sizeof(unsigned short)), "");
static_assert(width<unsigned int>() == to_bits(sizeof(unsigned int)), "");
static_assert(width<unsigned long>() == to_bits(sizeof(unsigned long)), "");
static_assert(width<unsigned long long>() == to_bits(sizeof(unsigned long long)), "");
static_assert(width<wchar_t>() == to_bits(sizeof(wchar_t)), "");
static_assert(width<int8_t>() == 8, "");
static_assert(width<uint8_t>() == 8, "");
static_assert(width<int16_t>() == 16, "");
static_assert(width<uint16_t>() == 16, "");
static_assert(width<int32_t>() == 32, "");
static_assert(width<uint32_t>() == 32, "");
static_assert(width<int64_t>() == 64, "");
static_assert(width<uint64_t>() == 64, "");
static_assert(width(value) == to_bits(sizeof(value)), "");
static_assert(std::is_same<decltype(width<int32_t>()), size_t>::value, "");

BOOST_AUTO_TEST_SUITE(constants_tests)

BOOST_AUTO_TEST_CASE(constants__width__uintx__expected)
{
    BOOST_REQUIRE_EQUAL(width<uintx>(), 192u);
}

BOOST_AUTO_TEST_CASE(constants__width__uintx_t__expected)
{
    BOOST_REQUIRE_EQUAL(width<uint5_t>(), 8u);
    BOOST_REQUIRE_EQUAL(width<uint11_t>(), 16u);
    BOOST_REQUIRE_EQUAL(width<uint48_t>(), 64u);
    BOOST_REQUIRE_EQUAL(width<uint128_t>(), 192u);
    BOOST_REQUIRE_EQUAL(width<uint160_t>(), 256u);
    BOOST_REQUIRE_EQUAL(width<uint256_t>(), 320u);
    BOOST_REQUIRE_EQUAL(width<uint512_t>(), 576u);
}

BOOST_AUTO_TEST_SUITE_END()

static_assert(variable_size(1) == 1u, "");
static_assert(variable_size(0xfe) == 3u, "");
static_assert(variable_size(0x10000) == 5u, "");
static_assert(variable_size(0x100000000) == 9u, "");