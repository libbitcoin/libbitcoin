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

BOOST_AUTO_TEST_SUITE(pseudo_random_tests)

BOOST_AUTO_TEST_CASE(pseudo_random__duration__zero_duration__maximum)
{
    const int max_seconds = 0;
    const std::chrono::seconds maximum(max_seconds);
    const auto result = pseudo_random::duration(maximum, 1);
    BOOST_REQUIRE(result == maximum);
}

BOOST_AUTO_TEST_CASE(pseudo_random__duration__subminute_default_percent__expected)
{
    const int max_seconds = 42;
    const std::chrono::seconds maximum(max_seconds);
    const std::chrono::seconds minimum(max_seconds - max_seconds / 2);
    const auto result = pseudo_random::duration(maximum);
    BOOST_REQUIRE(result <= maximum);
    BOOST_REQUIRE(result >= minimum);
}

BOOST_AUTO_TEST_CASE(pseudo_random__duration__subminute_ratio_0__maximum)
{
    const int max_seconds = 42;
    const std::chrono::seconds maximum(max_seconds);
    const auto result = pseudo_random::duration(maximum, 0);
    BOOST_REQUIRE(result == maximum);
}

BOOST_AUTO_TEST_CASE(pseudo_random__duration__subminute_ratio_1__expected)
{
    const uint8_t ratio = 1;
    const int max_seconds = 42;
    const std::chrono::seconds maximum(max_seconds);
    const std::chrono::seconds minimum(max_seconds - max_seconds / ratio);
    const auto result = pseudo_random::duration(maximum, ratio);
    BOOST_REQUIRE(result <= maximum);
    BOOST_REQUIRE(result >= minimum);
}

BOOST_AUTO_TEST_CASE(pseudo_random__duration__subminute_default_ratio__expected)
{
    const int max_seconds = 42;
    const std::chrono::seconds maximum(max_seconds);
    const std::chrono::seconds minimum(max_seconds - max_seconds / 2);
    const auto result = pseudo_random::duration(maximum);
    BOOST_REQUIRE(result <= maximum);
    BOOST_REQUIRE(result >= minimum);
}

// Use same (ms) resolution as function to prevent test rounding difference.
BOOST_AUTO_TEST_CASE(pseudo_random__duration__superminute_ratio_255__expected)
{
    const uint8_t ratio = 255;
    const int max_seconds = 420;
    const std::chrono::milliseconds maximum(max_seconds);
    const std::chrono::milliseconds minimum(max_seconds - max_seconds / ratio);
    const auto result = pseudo_random::duration(maximum, ratio);
    BOOST_REQUIRE(result <= maximum);
    BOOST_REQUIRE(result >= minimum);
}

BOOST_AUTO_TEST_SUITE_END()
