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

// Sponsored in part by Digital Contract Design, LLC

#include "../test.hpp"

BOOST_AUTO_TEST_SUITE(compact_filter_tests)

BOOST_AUTO_TEST_CASE(compact_filter__constructor_1__always__invalid)
{
    message::compact_filter instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());
}

BOOST_AUTO_TEST_CASE(compact_filter__constructor_2__always__equals_params)
{
    const uint8_t filter_type = 16u;
    const hash_digest block_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const auto filter = base16_chunk("0123456789abcdef");

    message::compact_filter instance(filter_type, block_hash, filter);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(filter_type == instance.filter_type());
    BOOST_REQUIRE(block_hash == instance.block_hash());
    BOOST_REQUIRE(filter == instance.filter());
}

BOOST_AUTO_TEST_CASE(compact_filter__constructor_3__always__equals_params)
{
    const uint8_t filter_type = 16u;
    const hash_digest block_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    hash_digest dup_hash = block_hash;
    const auto filter = base16_chunk("0123456789abcdef");
    data_chunk dup_filter = filter;

    message::compact_filter instance(filter_type, std::move(dup_hash), std::move(dup_filter));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(filter_type == instance.filter_type());
    BOOST_REQUIRE(block_hash == instance.block_hash());
    BOOST_REQUIRE(filter == instance.filter());
}

BOOST_AUTO_TEST_CASE(compact_filter__constructor_4__always__equals_params)
{
    const uint8_t filter_type = 16u;
    const hash_digest block_hash = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    const auto filter = base16_chunk("fedcba9876543210");

    message::compact_filter value(filter_type, block_hash, filter);
    message::compact_filter instance(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(filter_type == instance.filter_type());
    BOOST_REQUIRE(block_hash == instance.block_hash());
    BOOST_REQUIRE(filter == instance.filter());
}

BOOST_AUTO_TEST_CASE(compact_filter__constructor_5__always__equals_params)
{
    const uint8_t filter_type = 16u;
    const hash_digest block_hash = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    const auto filter = base16_chunk("fedcba9876543210");

    message::compact_filter value(filter_type, block_hash, filter);
    message::compact_filter instance(std::move(value));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(filter_type == instance.filter_type());
    BOOST_REQUIRE(block_hash == instance.block_hash());
    BOOST_REQUIRE(filter == instance.filter());
}

BOOST_AUTO_TEST_CASE(compact_filter__from_data__insufficient_bytes__failure)
{
    const data_chunk raw{ 0xab, 0xcd };
    message::compact_filter instance;
    BOOST_REQUIRE_EQUAL(false, instance.from_data(message::compact_filter::version_minimum, raw));
}

BOOST_AUTO_TEST_CASE(compact_filter__from_data__insufficient_version__failure)
{
    const auto raw = to_chunk(base16_literal(
        "0a0000006fe28c0ab6f1b372c1a6a246ae63f74f931e8365e15a089c68d619000022"
        "00003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a"));

    message::compact_filter expected;
    expected.from_data(message::compact_filter::version_minimum, raw);
    const auto data = expected.to_data(message::compact_filter::version_minimum);
    BOOST_REQUIRE(raw == data);

    message::compact_filter instance;
    BOOST_REQUIRE_EQUAL(false, instance.from_data(message::compact_filter::version_minimum - 1, data));
}

BOOST_AUTO_TEST_CASE(compact_filter__factory_1__valid_input__success)
{
    const auto raw = to_chunk(base16_literal(
        "0a0000006fe28c0ab6f1b372c1a6a246ae63f74f931e8365e15a089c68d619000022"
        "00003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a"));

    message::compact_filter expected;
    expected.from_data(message::compact_filter::version_minimum, raw);
    const auto data = expected.to_data(message::compact_filter::version_minimum);
    BOOST_REQUIRE(raw == data);

    const auto result = message::compact_filter::factory(message::compact_filter::version_minimum, data);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(message::compact_filter::version_minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::compact_filter::version_minimum), result.serialized_size(message::compact_filter::version_minimum));
}

BOOST_AUTO_TEST_CASE(compact_filter__factory_2__valid_input__success)
{
    const auto raw = to_chunk(base16_literal(
        "0a0000006fe28c0ab6f1b372c1a6a246ae63f74f931e8365e15a089c68d619000022"
        "00003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a"));

    message::compact_filter expected;
    expected.from_data(message::compact_filter::version_minimum, raw);
    const auto data = expected.to_data(message::compact_filter::version_minimum);
    BOOST_REQUIRE(raw == data);

    stream::in::copy istream(data);
    const auto result = message::compact_filter::factory(message::compact_filter::version_minimum, istream);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(message::compact_filter::version_minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::compact_filter::version_minimum), result.serialized_size(message::compact_filter::version_minimum));
}

BOOST_AUTO_TEST_CASE(compact_filter__factory_3__valid_input__success)
{
    const auto raw = to_chunk(base16_literal(
        "0a0000006fe28c0ab6f1b372c1a6a246ae63f74f931e8365e15a089c68d619000022"
        "00003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a"));

    message::compact_filter expected;
    expected.from_data(message::compact_filter::version_minimum, raw);
    const auto data = expected.to_data(message::compact_filter::version_minimum);
    BOOST_REQUIRE(raw == data);

    read::bytes::copy source(data);
    const auto result = message::compact_filter::factory(message::compact_filter::version_minimum, source);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(message::compact_filter::version_minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::compact_filter::version_minimum), result.serialized_size(message::compact_filter::version_minimum));
}

BOOST_AUTO_TEST_CASE(compact_filter__filter_type_accessor__always__returns_initialized_value)
{
    const uint8_t filter_type = 55u;
    const hash_digest block_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const auto filter = base16_chunk("0123456789abcdef");

    message::compact_filter instance(filter_type, block_hash, filter);
    BOOST_REQUIRE(filter_type == instance.filter_type());
}

BOOST_AUTO_TEST_CASE(compact_filter__filter_type_setter__roundtrip__success)
{
    const uint8_t filter_type = 38u;

    message::compact_filter instance;
    BOOST_REQUIRE(filter_type != instance.filter_type());
    instance.set_filter_type(filter_type);
    BOOST_REQUIRE(filter_type == instance.filter_type());
}

BOOST_AUTO_TEST_CASE(compact_filter__block_hash_accessor_1__always__returns_initialized_value)
{
    const uint8_t filter_type = 55u;
    const hash_digest block_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const auto filter = base16_chunk("0123456789abcdef");

    message::compact_filter instance(filter_type, block_hash, filter);
    BOOST_REQUIRE(block_hash == instance.block_hash());
}

BOOST_AUTO_TEST_CASE(compact_filter__block_hash_accessor_2__always__returns_initialized_value)
{
    const uint8_t filter_type = 55u;
    const hash_digest block_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const auto filter = base16_chunk("0123456789abcdef");

    const message::compact_filter instance(filter_type, block_hash, filter);
    BOOST_REQUIRE(block_hash == instance.block_hash());
}

BOOST_AUTO_TEST_CASE(compact_filter__block_hash_setter_1__roundtrip__success)
{
    const hash_digest block_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    message::compact_filter instance;
    BOOST_REQUIRE(block_hash != instance.block_hash());
    instance.set_block_hash(block_hash);
    BOOST_REQUIRE(block_hash == instance.block_hash());
}

BOOST_AUTO_TEST_CASE(compact_filter__block_hash_setter_2__roundtrip__success)
{
    const hash_digest block_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    hash_digest dup = block_hash;

    message::compact_filter instance;
    BOOST_REQUIRE(block_hash != instance.block_hash());
    instance.set_block_hash(std::move(dup));
    BOOST_REQUIRE(block_hash == instance.block_hash());
}

BOOST_AUTO_TEST_CASE(compact_filter__filter_accessor_1__always__returns_initialized_value)
{
    const uint8_t filter_type = 55u;
    const hash_digest block_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const auto filter = base16_chunk("0123456789abcdef");

    message::compact_filter instance(filter_type, block_hash, filter);
    BOOST_REQUIRE(filter == instance.filter());
}

BOOST_AUTO_TEST_CASE(compact_filter__filter_accessor_2__always__returns_initialized_value)
{
    const uint8_t filter_type = 55u;
    const hash_digest block_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const auto filter = base16_chunk("0123456789abcdef");

    const message::compact_filter instance(filter_type, block_hash, filter);
    BOOST_REQUIRE(filter == instance.filter());
}

BOOST_AUTO_TEST_CASE(compact_filter__filter_setter_1__roundtrip__success)
{
    const auto filter = base16_chunk("0123456789abcdef");

    message::compact_filter instance;
    BOOST_REQUIRE(filter != instance.filter());
    instance.set_filter(filter);
    BOOST_REQUIRE(filter == instance.filter());
}

BOOST_AUTO_TEST_CASE(compact_filter__filter_setter_2__roundtrip__success)
{
    const auto filter = base16_chunk("0123456789abcdef");
    data_chunk dup = filter;

    message::compact_filter instance;
    BOOST_REQUIRE(filter != instance.filter());
    instance.set_filter(std::move(dup));
    BOOST_REQUIRE(filter == instance.filter());
}

BOOST_AUTO_TEST_CASE(compact_filter__operator_assign_equals__always__matches_equivalent)
{
    const uint8_t filter_type = 55u;
    const hash_digest block_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const auto filter = base16_chunk("0123456789abcdef");

    message::compact_filter value(filter_type, block_hash, filter);
    BOOST_REQUIRE(value.is_valid());

    message::compact_filter instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(filter_type == instance.filter_type());
    BOOST_REQUIRE(block_hash == instance.block_hash());
    BOOST_REQUIRE(filter == instance.filter());
}

BOOST_AUTO_TEST_CASE(compact_filter__operator_boolean_equals__duplicates__returns_true)
{
    const message::compact_filter expected(
        19u,
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        base16_chunk("0123456789abcdef"));

    message::compact_filter instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(compact_filter__operator_boolean_equals__differs__returns_false)
{
    const message::compact_filter expected(
        19u,
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        base16_chunk("0123456789abcdef"));

    message::compact_filter instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(compact_filter__operator_boolean_not_equals__duplicates__returns_false)
{
    const message::compact_filter expected(
        19u,
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        base16_chunk("0123456789abcdef"));

    message::compact_filter instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(compact_filter__operator_boolean_not_equals__differs__returns_true)
{
    const message::compact_filter expected(
        19u,
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        base16_chunk("0123456789abcdef"));

    message::compact_filter instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()
