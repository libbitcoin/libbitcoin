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
#include "../test.hpp"

// Test helpers to verify output signedness.
template <typename Integer, if_signed_integer<Integer> = true>
constexpr bool is_unsigned(Integer) { return false; }
template <typename Integer, if_unsigned_integer<Integer> = true>
constexpr bool is_unsigned(Integer) { return true; }

// to_signed
static_assert(!is_unsigned(to_signed(-1)), "");
static_assert(!is_unsigned(to_signed(0)), "");
static_assert(!is_unsigned(to_signed(0u)), "");
static_assert(!is_unsigned(to_signed(1)), "");
static_assert(!is_unsigned(to_signed(1u)), "");
static_assert(to_signed(-2) == -2, "");
static_assert(to_signed(-1) == -1, "");
static_assert(to_signed(0) == 0, "");
static_assert(to_signed(0u) == 0, "");
static_assert(to_signed(1) == 1, "");
static_assert(to_signed(1u) == 1, "");
static_assert(to_signed(2u) == 2, "");

// to_unsigned
static_assert(is_unsigned(to_unsigned(-1)), "");
static_assert(is_unsigned(to_unsigned(0)), "");
static_assert(is_unsigned(to_unsigned(0u)), "");
static_assert(is_unsigned(to_unsigned(1)), "");
static_assert(is_unsigned(to_unsigned(1u)), "");
static_assert(to_unsigned(-2) == 0xfffffffe, "");
static_assert(to_unsigned(-1) == 0xffffffff, "");
static_assert(to_unsigned(0) == 0u, "");
static_assert(to_unsigned(0u) == 0u, "");
static_assert(to_unsigned(1) == 1u, "");
static_assert(to_unsigned(1u) == 1u, "");
static_assert(to_unsigned(2u) == 2u, "");

// absolute
static_assert(is_unsigned(absolute(-1)), "");
static_assert(is_unsigned(absolute(0)), "");
static_assert(is_unsigned(absolute(1u)), "");
static_assert(absolute(-1) == 1u, "");
static_assert(absolute(-42) == 42u, "");
static_assert(absolute(0) == 0u, "");
static_assert(absolute(0u) == 0u, "");
static_assert(absolute(1) == 1u, "");
static_assert(absolute(1u) == 1u, "");
static_assert(absolute(42) == 42, "");
static_assert(absolute(42u) == 42, "");
static_assert(absolute(max_int32) == max_int32, "");
static_assert(absolute(min_int32 + 1) == max_int32, "");
static_assert(absolute(max_uint32) == max_uint32, "");

// is_negative
static_assert(is_negative(min_int32), "");
static_assert(is_negative(-1), "");
static_assert(is_negative(-42), "");
static_assert(!is_negative(0u), "");
static_assert(!is_negative(1u), "");
static_assert(!is_negative(42u), "");
static_assert(!is_negative(max_int32), "");
static_assert(!is_negative(max_uint32), "");

// to_unsigned (C++14: required for constexpr of differing signs)
static_assert(is_greater(1, 0), "");
////static_assert(is_greater(1u, 0), "");
static_assert(is_greater(1u, 0u), "");
static_assert(is_greater(0, -1), "");
////static_assert(is_greater(0u, -1), "");
static_assert(!is_greater(-1, -1), "");
////static_assert(!is_greater(0u, 0), "");
static_assert(!is_greater(0u, 0u), "");
////static_assert(!is_greater(0, 0u), "");
static_assert(!is_greater(1, 1), "");
////static_assert(!is_greater(1u, 1), "");
static_assert(!is_greater(1u, 1u), "");
////static_assert(!is_greater(1, 1u), "");
static_assert(!is_greater(0, 1), "");
static_assert(!is_greater(-1, 0), "");

// to_unsigned (C++14: required for constexpr of differing signs)
static_assert(!is_lesser(1, 0), "");
////static_assert(!is_lesser(1u, 0), "");
static_assert(!is_lesser(1u, 0u), "");
static_assert(!is_lesser(0, -1), "");
////static_assert(!is_lesser(0u, -1), "");
static_assert(!is_lesser(-1, -1), "");
////static_assert(!is_lesser(0u, 0), "");
static_assert(!is_lesser(0u, 0u), "");
////static_assert(!is_lesser(0, 0u), "");
static_assert(!is_lesser(1, 1), "");
////static_assert(!is_lesser(1u, 1), "");
static_assert(!is_lesser(1u, 1u), "");
////static_assert(!is_lesser(1, 1u), "");
static_assert(is_lesser(0, 1), "");
static_assert(is_lesser(-1, 0), "");

// greater

static_assert(greater<signed>(1, 0) == 1, "");

BOOST_AUTO_TEST_CASE(greater__signed__different__expected)
{
    BOOST_REQUIRE_EQUAL(greater<signed>(1, 0), 1);
    BOOST_REQUIRE_EQUAL(greater<signed>(0, 1), 1);
    BOOST_REQUIRE_EQUAL(greater<signed>(1u, 0u), 1);
    BOOST_REQUIRE_EQUAL(greater<signed>(0u, 1u), 1);
    BOOST_REQUIRE_EQUAL(greater<signed>(1, 0u), 1);
    BOOST_REQUIRE_EQUAL(greater<signed>(0, 1u), 1);
    BOOST_REQUIRE_EQUAL(greater<signed>(1u, 0), 1);
    BOOST_REQUIRE_EQUAL(greater<signed>(0u, 1), 1);
}

BOOST_AUTO_TEST_CASE(greater__signed__equal__expected)
{
    BOOST_REQUIRE_EQUAL(greater<signed>(0, 0), 0);
    BOOST_REQUIRE_EQUAL(greater<signed>(1, 1), 1);
    BOOST_REQUIRE_EQUAL(greater<signed>(0u, 0u), 0);
    BOOST_REQUIRE_EQUAL(greater<signed>(1u, 1u), 1);
    BOOST_REQUIRE_EQUAL(greater<signed>(0u, 0), 0);
    BOOST_REQUIRE_EQUAL(greater<signed>(1u, 1), 1);
    BOOST_REQUIRE_EQUAL(greater<signed>(0, 0u), 0);
    BOOST_REQUIRE_EQUAL(greater<signed>(1, 1u), 1);
}

BOOST_AUTO_TEST_CASE(greater__unsigned__different__expected)
{
    BOOST_REQUIRE_EQUAL(greater<unsigned>(1, 0), 1u);
    BOOST_REQUIRE_EQUAL(greater<unsigned>(0, 1), 1);
    BOOST_REQUIRE_EQUAL(greater<unsigned>(1u, 0u), 1u);
    BOOST_REQUIRE_EQUAL(greater<unsigned>(0u, 1u), 1u);
    BOOST_REQUIRE_EQUAL(greater<unsigned>(1, 0u), 1u);
    BOOST_REQUIRE_EQUAL(greater<unsigned>(0, 1u), 1u);
    BOOST_REQUIRE_EQUAL(greater<unsigned>(1u, 0), 1u);
    BOOST_REQUIRE_EQUAL(greater<unsigned>(0u, 1), 1u);
}

BOOST_AUTO_TEST_CASE(greater__unsigned__equal__expected)
{
    BOOST_REQUIRE_EQUAL(greater<unsigned>(0, 0), 0u);
    BOOST_REQUIRE_EQUAL(greater<unsigned>(1, 1), 1u);
    BOOST_REQUIRE_EQUAL(greater<unsigned>(0u, 0u), 0u);
    BOOST_REQUIRE_EQUAL(greater<unsigned>(1u, 1u), 1u);
    BOOST_REQUIRE_EQUAL(greater<unsigned>(0u, 0), 0u);
    BOOST_REQUIRE_EQUAL(greater<unsigned>(1u, 1), 1u);
    BOOST_REQUIRE_EQUAL(greater<unsigned>(0, 0u), 0u);
    BOOST_REQUIRE_EQUAL(greater<unsigned>(1, 1u), 1u);
}

// lesser

BOOST_AUTO_TEST_CASE(lesser__signed__different__expected)
{
    BOOST_REQUIRE_EQUAL(lesser<signed>(1, 0), 0);
    BOOST_REQUIRE_EQUAL(lesser<signed>(0, 1), 0);
    BOOST_REQUIRE_EQUAL(lesser<signed>(1u, 0u), 0);
    BOOST_REQUIRE_EQUAL(lesser<signed>(0u, 1u), 0);
    BOOST_REQUIRE_EQUAL(lesser<signed>(1, 0u), 0);
    BOOST_REQUIRE_EQUAL(lesser<signed>(0, 1u), 0);
    BOOST_REQUIRE_EQUAL(lesser<signed>(1u, 0), 0);
    BOOST_REQUIRE_EQUAL(lesser<signed>(0u, 1), 0);
}

BOOST_AUTO_TEST_CASE(lesser__signed__equal__expected)
{
    BOOST_REQUIRE_EQUAL(lesser<signed>(0, 0), 0);
    BOOST_REQUIRE_EQUAL(lesser<signed>(1, 1), 1);
    BOOST_REQUIRE_EQUAL(lesser<signed>(0u, 0u), 0);
    BOOST_REQUIRE_EQUAL(lesser<signed>(1u, 1u), 1);
    BOOST_REQUIRE_EQUAL(lesser<signed>(0u, 0), 0);
    BOOST_REQUIRE_EQUAL(lesser<signed>(1u, 1), 1);
    BOOST_REQUIRE_EQUAL(lesser<signed>(0, 0u), 0);
    BOOST_REQUIRE_EQUAL(lesser<signed>(1, 1u), 1);
}

BOOST_AUTO_TEST_CASE(lesser__unsigned__different__expected)
{
    BOOST_REQUIRE_EQUAL(lesser<unsigned>(1, 0), 0u);
    BOOST_REQUIRE_EQUAL(lesser<unsigned>(0, 1), 0u);
    BOOST_REQUIRE_EQUAL(lesser<unsigned>(1u, 0u), 0u);
    BOOST_REQUIRE_EQUAL(lesser<unsigned>(0u, 1u), 0u);
    BOOST_REQUIRE_EQUAL(lesser<unsigned>(1, 0u), 0u);
    BOOST_REQUIRE_EQUAL(lesser<unsigned>(0, 1u), 0u);
    BOOST_REQUIRE_EQUAL(lesser<unsigned>(1u, 0), 0u);
    BOOST_REQUIRE_EQUAL(lesser<unsigned>(0u, 1), 0u);
}

BOOST_AUTO_TEST_CASE(lesser__unsigned__equal__expected)
{
    BOOST_REQUIRE_EQUAL(lesser<unsigned>(0, 0), 0u);
    BOOST_REQUIRE_EQUAL(lesser<unsigned>(1, 1), 1u);
    BOOST_REQUIRE_EQUAL(lesser<unsigned>(0u, 0u), 0u);
    BOOST_REQUIRE_EQUAL(lesser<unsigned>(1u, 1u), 1u);
    BOOST_REQUIRE_EQUAL(lesser<unsigned>(0u, 0), 0u);
    BOOST_REQUIRE_EQUAL(lesser<unsigned>(1u, 1), 1u);
    BOOST_REQUIRE_EQUAL(lesser<unsigned>(0, 0u), 0u);
    BOOST_REQUIRE_EQUAL(lesser<unsigned>(1, 1u), 1u);
}

BOOST_AUTO_TEST_SUITE(sign_tests)

BOOST_AUTO_TEST_SUITE_END()
