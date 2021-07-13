/**
 * Copyright (c) 2011-2019 libbitcoin developers (see AUTHORS)
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
#include <cstddef>
#include <cstdint>

BOOST_AUTO_TEST_SUITE(limits_tests)

constexpr uint64_t min_uint64 = 0;
constexpr uint32_t min_uint32 = 0;

constexpr size_t minimum = 0;
constexpr size_t maximum = max_size_t;
constexpr size_t half = maximum / 2;

// safe_add

BOOST_AUTO_TEST_CASE(safe__safe_add__size_t_minimum_plus_minimum__minimum)
{
    BOOST_REQUIRE_EQUAL(safe_add(minimum, minimum), minimum);
}

BOOST_AUTO_TEST_CASE(safe__safe_add__size_t_maximum_plus_maximum__throws_overflow)
{
    BOOST_REQUIRE_THROW(safe_add(maximum, maximum), overflow_exception);
}

BOOST_AUTO_TEST_CASE(safe__safe_add__size_t_minimum_plus_maximum__maximum)
{
    BOOST_REQUIRE_EQUAL(safe_add(minimum, maximum), maximum);
}

BOOST_AUTO_TEST_CASE(safe__safe_add__size_t_maximum_plus_minimum__maximum)
{
    BOOST_REQUIRE_EQUAL(safe_add(maximum, minimum), maximum);
}

BOOST_AUTO_TEST_CASE(safe__safe_add__size_t_half_plus_maximum__throws_overflow)
{
    BOOST_REQUIRE_THROW(safe_add(half, maximum), overflow_exception);
}

// safe_subtract

BOOST_AUTO_TEST_CASE(safe__safe_subtract__size_t_minimum_minus_minimum__minimum)
{
    BOOST_REQUIRE_EQUAL(safe_subtract(minimum, minimum), minimum);
}

BOOST_AUTO_TEST_CASE(safe__safe_subtract__size_t_maximum_minus_maximum__minimum)
{
    BOOST_REQUIRE_EQUAL(safe_subtract(maximum, maximum), minimum);
}

BOOST_AUTO_TEST_CASE(safe__safe_subtract__size_t_maximum_minus_minimum__maximum)
{
    BOOST_REQUIRE_EQUAL(safe_subtract(maximum, minimum), maximum);
}

BOOST_AUTO_TEST_CASE(safe__safe_subtract__size_t_minimum_minus_maximum__throws_underflow)
{
    BOOST_REQUIRE_THROW(safe_subtract(minimum, maximum), underflow_exception);
}

BOOST_AUTO_TEST_CASE(safe__safe_subtract__size_t_half_minus_maximum__throws_underflow)
{
    BOOST_REQUIRE_THROW(safe_subtract(half, maximum), underflow_exception);
}

// safe_multiply

BOOST_AUTO_TEST_CASE(safe__safe_multiply__size_t_minimum_times_minimum__minimum)
{
    BOOST_REQUIRE_EQUAL(safe_multiply(minimum, minimum), minimum);
}

BOOST_AUTO_TEST_CASE(safe__safe_multiply__size_t_maximum_times_maximum__throws_overflow)
{
    BOOST_REQUIRE_THROW(safe_multiply(maximum, maximum), overflow_exception);
}

BOOST_AUTO_TEST_CASE(safe__safe_multiply__size_t_minimum_times_maximum__minimum)
{
    BOOST_REQUIRE_EQUAL(safe_multiply(minimum, maximum), minimum);
}

BOOST_AUTO_TEST_CASE(safe__safe_multiply__size_t_maximum_times_minimum__minimum)
{
    BOOST_REQUIRE_EQUAL(safe_multiply(maximum, minimum), minimum);
}

BOOST_AUTO_TEST_CASE(safe__safe_multiply__size_t_half_times_2__maximum_minus_1)
{
    // The maximum of an unsigned integer is always odd, so half is always rounded down.
    // Therefore 2 * half is always one less than the unsigned integer maximum.
    BOOST_REQUIRE_EQUAL(safe_multiply(half, size_t(2)), sub1(maximum));
}

BOOST_AUTO_TEST_CASE(safe__safe_multiply__size_t_2_times_half_plus_1__throws_overflow)
{
    BOOST_REQUIRE_THROW(safe_multiply(size_t(2), add1(half)), overflow_exception);
}

// safe_increment

BOOST_AUTO_TEST_CASE(safe__safe_increment__size_t_minimum__expected)
{
    auto value = minimum;
    static const auto expected = add1(minimum);
    safe_increment(value);
    BOOST_REQUIRE_EQUAL(value, expected);
}

BOOST_AUTO_TEST_CASE(safe__safe_increment__size_t_half__expected)
{
    auto value = half;
    static const auto expected = add1(half);
    safe_increment(value);
    BOOST_REQUIRE_EQUAL(value, expected);
}

BOOST_AUTO_TEST_CASE(safe__safe_increment__size_t_maximum__throws_overflow)
{
    auto value = maximum;
    BOOST_REQUIRE_THROW(safe_increment(value), overflow_exception);
}

// safe_decrement

BOOST_AUTO_TEST_CASE(safe__safe_decrement__size_t_maximum__expected)
{
    auto value = maximum;
    static const auto expected = sub1(maximum);
    safe_decrement(value);
    BOOST_REQUIRE_EQUAL(value, expected);
}

BOOST_AUTO_TEST_CASE(safe__safe_decrement__size_t_half__expected)
{
    auto value = half;
    static const auto expected = sub1(half);
    safe_decrement(value);
    BOOST_REQUIRE_EQUAL(value, expected);
}

BOOST_AUTO_TEST_CASE(safe__safe_decrement__size_t_minimum__throws_underflow)
{
    auto value = minimum;
    BOOST_REQUIRE_THROW(safe_decrement(value), underflow_exception);
}

// safe_cast (signed)

BOOST_AUTO_TEST_CASE(safe__safe_cast__min_int32_to_int32__min_int32)
{
    BOOST_REQUIRE_EQUAL(safe_cast<int32_t>(min_int32), min_int32);
}

BOOST_AUTO_TEST_CASE(safe__safe_cast__max_int32_to_int32__max_int32)
{
    BOOST_REQUIRE_EQUAL(safe_cast<int32_t>(max_int32), max_int32);
}

BOOST_AUTO_TEST_CASE(safe__safe_cast__min_int64_to_int32__throws_range)
{
    BOOST_REQUIRE_THROW(safe_cast<int32_t>(min_int64), range_exception);
}

// safe_cast (unsigned)

BOOST_AUTO_TEST_CASE(safe__safe_cast__min_uint32_to_uint32__min_uint32)
{
    BOOST_REQUIRE_EQUAL(safe_cast<uint32_t>(min_uint32), min_uint32);
}

BOOST_AUTO_TEST_CASE(safe__safe_cast__max_uint32_to_uint32__max_uint32)
{
    BOOST_REQUIRE_EQUAL(safe_cast<uint32_t>(max_uint32), max_uint32);
}

BOOST_AUTO_TEST_CASE(safe__safe_cast__max_uint64_to_uint32__throws_range)
{
    BOOST_REQUIRE_THROW(safe_cast<uint32_t>(max_uint64), range_exception);
}

// safe_cast (to signed)

BOOST_AUTO_TEST_CASE(safe__safe_cast__min_uint32_to_int32__min_uint32)
{
    BOOST_REQUIRE_EQUAL(safe_cast<int32_t>(min_uint32), (int32_t)min_uint32);
}

BOOST_AUTO_TEST_CASE(safe__safe_cast__max_uint32_to_int32__throws_range)
{
    BOOST_REQUIRE_THROW(safe_cast<int32_t>(max_uint32), range_exception);
}

BOOST_AUTO_TEST_CASE(safe__safe_cast__min_uint64_to_int32__min_uint64)
{
    BOOST_REQUIRE_EQUAL(safe_cast<int32_t>(min_uint64), (int32_t)min_uint64);
}

// safe_cast (to unsigned)

BOOST_AUTO_TEST_CASE(safe__safe_cast__min_int32_to_uint32__throws_range)
{
    BOOST_REQUIRE_THROW(safe_cast<uint32_t>(min_int32), range_exception);
}

BOOST_AUTO_TEST_CASE(safe__safe_cast__max_int32_to_uint32__max_int32)
{
    BOOST_REQUIRE_EQUAL(safe_cast<uint32_t>(max_int32), (uint32_t)max_int32);
}

BOOST_AUTO_TEST_CASE(safe__safe_cast__min_int64_to_uint32__throws_range)
{
    BOOST_REQUIRE_THROW(safe_cast<uint32_t>(min_int64), range_exception);
}

BOOST_AUTO_TEST_SUITE_END()
