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

BOOST_AUTO_TEST_SUITE(address_tests)

using namespace bc::system::message;

bool equal(const address& left, const address& right)
{
    const auto left_addresses = left.addresses();
    const auto right_addresses = right.addresses();
    auto same = (left_addresses.size() == right_addresses.size());

    for (size_t i = 0; (i < left_addresses.size()) && same; i++)
    {
        same = (left_addresses[i] == right_addresses[i]) && 
            (left_addresses[i].timestamp() == right_addresses[i].timestamp());
    }

    return same;
}

BOOST_AUTO_TEST_CASE(address__constructor_1__always__invalid)
{
    address instance;
    BOOST_REQUIRE(!instance.is_valid());
}

BOOST_AUTO_TEST_CASE(address__constructor_2__always__equals_params)
{
    const network_address::list addresses
    {
        network_address(
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u),
        network_address(
            34654u,
            47653u,
            base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            222u),
        network_address(
            265453u,
            2115325u,
            base16_literal("19573257168426842319857321595126"),
            159u)
    };

    address instance(addresses);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(addresses == instance.addresses());
}

BOOST_AUTO_TEST_CASE(address__constructor_3__always__equals_params)
{
    const network_address::list addresses
    {
        network_address(
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u),
        network_address(
            34654u,
            47653u,
            base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            222u),
        network_address(
            265453u,
            2115325u,
            base16_literal("19573257168426842319857321595126"),
            159u)
    };

    auto dup_addresses = addresses;

    address instance(std::move(dup_addresses));

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(addresses == instance.addresses());
}

BOOST_AUTO_TEST_CASE(address__constructor_4__always__equals_params)
{
    const network_address::list addresses
    {
        network_address(
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u),
        network_address(
            34654u,
            47653u,
            base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            222u),
        network_address(
            265453u,
            2115325u,
            base16_literal("19573257168426842319857321595126"),
            159u)
    };

    address value(addresses);
    address instance(value);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(value == instance);
    BOOST_REQUIRE(addresses == instance.addresses());
}

BOOST_AUTO_TEST_CASE(address__constructor_5__always__equals_params)
{
    const network_address::list addresses
    {
        network_address(
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u),
        network_address(
            34654u,
            47653u,
            base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            222u),
        network_address(
            265453u,
            2115325u,
            base16_literal("19573257168426842319857321595126"),
            159u)
    };

    address value(addresses);
    address instance(std::move(value));

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(addresses == instance.addresses());
}

BOOST_AUTO_TEST_CASE(address__from_data__insufficient_bytes__failure)
{
    const data_chunk raw{ 0xab };
    address instance;

    BOOST_REQUIRE(!instance.from_data(version::level::minimum, raw));
}

BOOST_AUTO_TEST_CASE(address__factory_1__roundtrip__success)
{
    const address expected(
    {
        {
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u
        }
    });

    const auto data = expected.to_data(version::level::minimum);
    const auto result = address::factory(version::level::minimum, data);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(equal(expected, result));
    const auto serialized_size = result.serialized_size(version::level::minimum);
    BOOST_REQUIRE_EQUAL(data.size(), serialized_size);
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version::level::minimum), serialized_size);
}

BOOST_AUTO_TEST_CASE(address__factory_2__roundtrip__success)
{
    const address expected(
    {
        {
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u
        }
    });

    const auto data = expected.to_data(version::level::minimum);
    stream::in::copy istream(data);
    const auto result = address::factory(version::level::minimum, istream);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(equal(expected, result));
    const auto serialized_size = result.serialized_size(version::level::minimum);
    BOOST_REQUIRE_EQUAL(data.size(), serialized_size);
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version::level::minimum), serialized_size);
}

BOOST_AUTO_TEST_CASE(address__factory_3__roundtrip__success)
{
    const address expected(
    {
        {
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u
        }
    });

    const data_chunk data = expected.to_data(version::level::minimum);
    read::bytes::copy source(data);
    const auto result = address::factory(version::level::minimum, source);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(equal(expected, result));
    const auto serialized_size = result.serialized_size(version::level::minimum);
    BOOST_REQUIRE_EQUAL(data.size(), serialized_size);
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version::level::minimum), serialized_size);
}

BOOST_AUTO_TEST_CASE(address__addresses_setter_1__roundtrip__success)
{
    const network_address::list value
    {
        network_address(
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u),
        network_address(
            34654u,
            47653u,
            base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            222u),
        network_address(
            265453u,
            2115325u,
            base16_literal("19573257168426842319857321595126"),
            159u)
    };

    address instance;
    BOOST_REQUIRE(instance.addresses() != value);
    instance.set_addresses(value);
    BOOST_REQUIRE(value == instance.addresses());
}

BOOST_AUTO_TEST_CASE(address__addresses_setter_2__roundtrip__success)
{
    const network_address::list value
    {
        network_address(
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u),
        network_address(
            34654u,
            47653u,
            base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            222u),
        network_address(
            265453u,
            2115325u,
            base16_literal("19573257168426842319857321595126"),
            159u)
    };

    auto dup_value = value;
    address instance;
    BOOST_REQUIRE(instance.addresses() != value);
    instance.set_addresses(std::move(dup_value));
    BOOST_REQUIRE(value == instance.addresses());
}

BOOST_AUTO_TEST_CASE(address__operator_assign_equals__always__matches_equivalent)
{
    const network_address::list addresses
    {
        network_address(
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u),
        network_address(
            34654u,
            47653u,
            base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            222u),
        network_address(
            265453u,
            2115325u,
            base16_literal("19573257168426842319857321595126"),
            159u)
    };

    address value(addresses);

    BOOST_REQUIRE(value.is_valid());

    address instance;
    BOOST_REQUIRE(!instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(addresses == instance.addresses());
}

BOOST_AUTO_TEST_CASE(address__operator_boolean_equals__duplicates__returns_true)
{
    const address expected(
    {
        network_address(
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u),
        network_address(
            34654u,
            47653u,
            base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            222u),
        network_address(
            265453u,
            2115325u,
            base16_literal("19573257168426842319857321595126"),
            159u)
    });

    address instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(address__operator_boolean_equals__differs__returns_false)
{
    const address expected(
    {
        network_address(
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u),
        network_address(
            34654u,
            47653u,
            base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            222u),
        network_address(
            265453u,
            2115325u,
            base16_literal("19573257168426842319857321595126"),
            159u)
    });

    address instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(address__operator_boolean_not_equals__duplicates__returns_false)
{
    const address expected(
    {
        network_address(
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u),
        network_address(
            34654u,
            47653u,
            base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            222u),
        network_address(
            265453u,
            2115325u,
            base16_literal("19573257168426842319857321595126"),
            159u)
    });

    address instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(address__operator_boolean_not_equals__differs__returns_true)
{
    const address expected(
    {
        network_address(
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u),
        network_address(
            34654u,
            47653u,
            base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            222u),
        network_address(
            265453u,
            2115325u,
            base16_literal("19573257168426842319857321595126"),
            159u)
    });

    address instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()
