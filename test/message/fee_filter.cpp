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

BOOST_AUTO_TEST_SUITE(fee_filter_tests)

using namespace bc::system::message;

BOOST_AUTO_TEST_CASE(fee_filter__constructor_1__always__invalid)
{
    const fee_filter instance;
    BOOST_REQUIRE(!instance.is_valid());
}

BOOST_AUTO_TEST_CASE(fee_filter__constructor_2__always__equals_params)
{
    const uint64_t value = 6434u;
    const fee_filter instance(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(value, instance.minimum_fee());
}

BOOST_AUTO_TEST_CASE(fee_filter__constructor_3__always__equals_params)
{
    const uint64_t fee = 6434u;
    const fee_filter value(fee);
    const fee_filter instance(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(fee, instance.minimum_fee());
    BOOST_REQUIRE(value == instance);
}

BOOST_AUTO_TEST_CASE(fee_filter__constructor_4__always__equals_params)
{
    const uint64_t fee = 6434u;
    const fee_filter value(fee);
    const fee_filter instance(std::move(value));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(fee, instance.minimum_fee());
}

BOOST_AUTO_TEST_CASE(fee_filter__from_data__insufficient_bytes_failure)
{
    const data_chunk raw = { 0xab, 0x11 };
    fee_filter instance;
    BOOST_REQUIRE(!instance.from_data(version::level::maximum, raw));
}

BOOST_AUTO_TEST_CASE(fee_filter__from_data__insufficient_version_failure)
{
    const fee_filter expected{ 1 };
    const auto data = expected.to_data(fee_filter::version_maximum);
    fee_filter instance;
    BOOST_REQUIRE(!instance.from_data(filter_add::version_minimum - 1, data));
}

BOOST_AUTO_TEST_CASE(fee_filter__factory_1__roundtrip__success)
{
    const fee_filter expected{ 123 };
    const auto data = expected.to_data(fee_filter::version_maximum);
    const auto result = fee_filter::factory(fee_filter::version_maximum, data);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);

    const auto size = result.serialized_size(version::level::maximum);
    BOOST_REQUIRE_EQUAL(data.size(), size);
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version::level::maximum), size);
}

BOOST_AUTO_TEST_CASE(fee_filter__factory_2__roundtrip__success)
{
    const fee_filter expected{ 325 };
    const auto data = expected.to_data(fee_filter::version_maximum);
    stream::in::copy istream(data);
    const auto result = fee_filter::factory(fee_filter::version_maximum, istream);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);

    const auto size = result.serialized_size(version::level::maximum);
    BOOST_REQUIRE_EQUAL(data.size(), size);
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version::level::maximum), size);
}

BOOST_AUTO_TEST_CASE(fee_filter__factory_3__roundtrip__success)
{
    const fee_filter expected{ 58246 };
    const auto data = expected.to_data(fee_filter::version_maximum);
    read::bytes::copy source(data);
    const auto result = fee_filter::factory(fee_filter::version_maximum, source);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);

    const auto size = result.serialized_size(version::level::maximum);
    BOOST_REQUIRE_EQUAL(data.size(), size);
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version::level::maximum), size);
}

BOOST_AUTO_TEST_CASE(fee_filter__minimum_fee__roundtrip__success)
{
    const uint64_t value = 42134u;
    fee_filter instance;
    BOOST_REQUIRE_NE(instance.minimum_fee(), value);

    instance.set_minimum_fee(value);
    BOOST_REQUIRE_EQUAL(value, instance.minimum_fee());
}

BOOST_AUTO_TEST_CASE(fee_filter__operator_assign_equals__always__matches_equivalent)
{
    fee_filter value(2453u);
    BOOST_REQUIRE(value.is_valid());

    fee_filter instance;
    BOOST_REQUIRE(!instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
}

BOOST_AUTO_TEST_CASE(fee_filter__operator_boolean_equals__duplicates__returns_true)
{
    const fee_filter expected(2453u);
    fee_filter instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(fee_filter__operator_boolean_equals__differs__returns_false)
{
    const fee_filter expected(2453u);
    fee_filter instance;
    BOOST_REQUIRE(!(instance == expected));
}

BOOST_AUTO_TEST_CASE(fee_filter__operator_boolean_not_equals__duplicates__returns_false)
{
    const fee_filter expected(2453u);
    fee_filter instance(expected);
    BOOST_REQUIRE(!(instance != expected));
}

BOOST_AUTO_TEST_CASE(fee_filter__operator_boolean_not_equals__differs__returns_true)
{
    const fee_filter expected(2453u);
    fee_filter instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()
