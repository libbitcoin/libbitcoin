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

BOOST_AUTO_TEST_SUITE(filter_load_tests)

BOOST_AUTO_TEST_CASE(filter_load__constructor_1__always__invalid)
{
    message::filter_load instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());
}

BOOST_AUTO_TEST_CASE(filter_load__constructor_2__always__equals_params)
{
    const data_chunk filter = { 0x0f, 0xf0, 0x55, 0xaa };
    uint32_t hash_functions = 48u;
    uint32_t tweak = 36u;
    uint8_t flags = 0xae;

    message::filter_load instance(filter, hash_functions, tweak, flags);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(filter == instance.filter());
    BOOST_REQUIRE_EQUAL(hash_functions, instance.hash_functions());
    BOOST_REQUIRE_EQUAL(tweak, instance.tweak());
    BOOST_REQUIRE_EQUAL(flags, instance.flags());
}

BOOST_AUTO_TEST_CASE(filter_load__constructor_3__always__equals_params)
{
    const data_chunk filter = { 0x0f, 0xf0, 0x55, 0xaa };
    auto dup_filter = filter;
    uint32_t hash_functions = 48u;
    uint32_t tweak = 36u;
    uint8_t flags = 0xae;

    message::filter_load instance(std::move(dup_filter), hash_functions, tweak, flags);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(filter == instance.filter());
    BOOST_REQUIRE_EQUAL(hash_functions, instance.hash_functions());
    BOOST_REQUIRE_EQUAL(tweak, instance.tweak());
    BOOST_REQUIRE_EQUAL(flags, instance.flags());
}

BOOST_AUTO_TEST_CASE(filter_load__constructor_4__always__equals_params)
{
    const data_chunk filter = { 0x0f, 0xf0, 0x55, 0xaa };
    uint32_t hash_functions = 48u;
    uint32_t tweak = 36u;
    uint8_t flags = 0xae;

    const message::filter_load value(filter, hash_functions, tweak, flags);
    message::filter_load instance(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(value == instance);
    BOOST_REQUIRE(filter == instance.filter());
    BOOST_REQUIRE_EQUAL(hash_functions, instance.hash_functions());
    BOOST_REQUIRE_EQUAL(tweak, instance.tweak());
    BOOST_REQUIRE_EQUAL(flags, instance.flags());
}

BOOST_AUTO_TEST_CASE(filter_load__constructor_5__always__equals_params)
{
    const data_chunk filter = { 0x0f, 0xf0, 0x55, 0xaa };
    uint32_t hash_functions = 48u;
    uint32_t tweak = 36u;
    uint8_t flags = 0xae;

    message::filter_load value(filter, hash_functions, tweak, flags);
    message::filter_load instance(std::move(value));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(filter == instance.filter());
    BOOST_REQUIRE_EQUAL(hash_functions, instance.hash_functions());
    BOOST_REQUIRE_EQUAL(tweak, instance.tweak());
    BOOST_REQUIRE_EQUAL(flags, instance.flags());
}

BOOST_AUTO_TEST_CASE(filter_load__from_data__insufficient_bytes__failure)
{
    const data_chunk raw{ 0xab, 0x11 };
    message::filter_load instance;

    BOOST_REQUIRE_EQUAL(false, instance.from_data(message::version::level::maximum, raw));
}

BOOST_AUTO_TEST_CASE(filter_load__from_data__insufficient_version__failure)
{
    const message::filter_load expected
    {
        { 0x05, 0xaa, 0xbb, 0xcc, 0xdd, 0xee },
        25,
        10,
        0xab
    };

    const data_chunk data = expected.to_data(message::version::level::maximum);
    message::filter_load instance;

    BOOST_REQUIRE_EQUAL(false, instance.from_data(
        message::filter_load::version_minimum - 1, data));
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());
}

BOOST_AUTO_TEST_CASE(filter_load__factory_1__valid_input__success)
{
    const message::filter_load expected
    {
        { 0x05, 0xaa, 0xbb, 0xcc, 0xdd, 0xee },
        25,
        10,
        0xab
    };

    const auto data = expected.to_data(message::version::level::maximum);
    const auto result = message::filter_load::factory(
        message::version::level::maximum, data);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(),
        result.serialized_size(message::version::level::maximum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::maximum),
        result.serialized_size(message::version::level::maximum));
}

BOOST_AUTO_TEST_CASE(filter_load__factory_2__valid_input__success)
{
    const message::filter_load expected
    {
        { 0x05, 0xaa, 0xbb, 0xcc, 0xdd, 0xee },
        25,
        10,
        0xab
    };

    const auto data = expected.to_data(message::version::level::maximum);
    stream::in::copy istream(data);
    const auto result = message::filter_load::factory(
        message::version::level::maximum, istream);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(),
        result.serialized_size(message::version::level::maximum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::maximum),
        result.serialized_size(message::version::level::maximum));
}

BOOST_AUTO_TEST_CASE(filter_load__factory_3__valid_input__success)
{
    const message::filter_load expected
    {
        { 0x05, 0xaa, 0xbb, 0xcc, 0xdd, 0xee },
        25,
        10,
        0xab
    };

    const auto data = expected.to_data(message::version::level::maximum);
    read::bytes::copy source(data);
    const auto result = message::filter_load::factory(
        message::version::level::maximum, source);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(message::version::level::maximum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::maximum),
        result.serialized_size(message::version::level::maximum));
}

BOOST_AUTO_TEST_CASE(filter_load__filter_accessor_1__always__returns_initialized_value)
{
    const data_chunk filter = { 0x0f, 0xf0, 0x55, 0xaa };
    uint32_t hash_functions = 48u;
    uint32_t tweak = 36u;
    uint8_t flags = 0xae;

    message::filter_load instance(filter, hash_functions, tweak, flags);
    BOOST_REQUIRE(filter == instance.filter());
}

BOOST_AUTO_TEST_CASE(filter_load__filter_accessor_2__always__returns_initialized_value)
{
    const data_chunk filter = { 0x0f, 0xf0, 0x55, 0xaa };
    uint32_t hash_functions = 48u;
    uint32_t tweak = 36u;
    uint8_t flags = 0xae;

    const message::filter_load instance(filter, hash_functions, tweak, flags);
    BOOST_REQUIRE(filter == instance.filter());
}

BOOST_AUTO_TEST_CASE(filter_load__filter_setter_1__roundtrip__success)
{
    const data_chunk filter = { 0x0f, 0xf0, 0x55, 0xaa };
    message::filter_load instance;
    BOOST_REQUIRE(filter != instance.filter());
    instance.set_filter(filter);
    BOOST_REQUIRE(filter == instance.filter());
}

BOOST_AUTO_TEST_CASE(filter_load__filter_setter_2__roundtrip__success)
{
    const data_chunk filter = { 0x0f, 0xf0, 0x55, 0xaa };
    data_chunk dup = filter;

    message::filter_load instance;
    BOOST_REQUIRE(filter != instance.filter());
    instance.set_filter(std::move(dup));
    BOOST_REQUIRE(filter == instance.filter());
}

BOOST_AUTO_TEST_CASE(filter_load__hash_functions_accessor__always__returns_initialized_value)
{
    const data_chunk filter = { 0x0f, 0xf0, 0x55, 0xaa };
    uint32_t hash_functions = 48u;
    uint32_t tweak = 36u;
    uint8_t flags = 0xae;

    message::filter_load instance(filter, hash_functions, tweak, flags);
    BOOST_REQUIRE_EQUAL(hash_functions, instance.hash_functions());
}

BOOST_AUTO_TEST_CASE(filter_load__hash_functions_setter__roundtrip__success)
{
    uint32_t hash_functions = 48u;
    message::filter_load instance;
    BOOST_REQUIRE(hash_functions != instance.hash_functions());
    instance.set_hash_functions(hash_functions);
    BOOST_REQUIRE(hash_functions == instance.hash_functions());
}

BOOST_AUTO_TEST_CASE(filter_load__tweak_accessor__always__returns_initialized_value)
{
    const data_chunk filter = { 0x0f, 0xf0, 0x55, 0xaa };
    uint32_t hash_functions = 48u;
    uint32_t tweak = 36u;
    uint8_t flags = 0xae;

    message::filter_load instance(filter, hash_functions, tweak, flags);
    BOOST_REQUIRE_EQUAL(tweak, instance.tweak());
}

BOOST_AUTO_TEST_CASE(filter_load__tweak_setter__roundtrip__success)
{
    uint32_t tweak = 36u;
    message::filter_load instance;
    BOOST_REQUIRE(tweak != instance.tweak());
    instance.set_tweak(tweak);
    BOOST_REQUIRE(tweak == instance.tweak());
}

BOOST_AUTO_TEST_CASE(filter_load__flags_accessor__always__returns_initialized_value)
{
    const data_chunk filter = { 0x0f, 0xf0, 0x55, 0xaa };
    uint32_t hash_functions = 48u;
    uint32_t tweak = 36u;
    uint8_t flags = 0xae;

    message::filter_load instance(filter, hash_functions, tweak, flags);
    BOOST_REQUIRE_EQUAL(flags, instance.flags());
}

BOOST_AUTO_TEST_CASE(filter_load__flags_setter__roundtrip__success)
{
    uint8_t flags = 0xae;
    message::filter_load instance;
    BOOST_REQUIRE(flags != instance.flags());
    instance.set_flags(flags);
    BOOST_REQUIRE(flags == instance.flags());
}

BOOST_AUTO_TEST_CASE(filter_load__operator_assign_equals__always__matches_equivalent)
{
    const data_chunk filter = { 0x0f, 0xf0, 0x55, 0xaa };
    uint32_t hash_functions = 48u;
    uint32_t tweak = 36u;
    uint8_t flags = 0xae;
    message::filter_load value(filter, hash_functions, tweak, flags);

    BOOST_REQUIRE(value.is_valid());

    message::filter_load instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(filter == instance.filter());
    BOOST_REQUIRE_EQUAL(hash_functions, instance.hash_functions());
    BOOST_REQUIRE_EQUAL(tweak, instance.tweak());
    BOOST_REQUIRE_EQUAL(flags, instance.flags());
}

BOOST_AUTO_TEST_CASE(filter_load__operator_boolean_equals__duplicates__returns_true)
{
    const message::filter_load expected(
        { 0x0f, 0xf0, 0x55, 0xaa }, 643u, 575u, 0xaa);

    message::filter_load instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(filter_load__operator_boolean_equals__differs__returns_false)
{
    const message::filter_load expected(
        { 0x0f, 0xf0, 0x55, 0xaa }, 643u, 575u, 0xaa);

    message::filter_load instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(filter_load__operator_boolean_not_equals__duplicates__returns_false)
{
    const message::filter_load expected(
        { 0x0f, 0xf0, 0x55, 0xaa }, 643u, 575u, 0xaa);

    message::filter_load instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(filter_load__operator_boolean_not_equals__differs__returns_true)
{
    const message::filter_load expected(
        { 0x0f, 0xf0, 0x55, 0xaa }, 643u, 575u, 0xaa);

    message::filter_load instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()
