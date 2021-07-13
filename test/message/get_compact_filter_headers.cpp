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

BOOST_AUTO_TEST_SUITE(get_compact_filter_headers_tests)

BOOST_AUTO_TEST_CASE(get_compact_filter_headers__constructor_1__always__invalid)
{
    message::get_compact_filter_headers instance;
    BOOST_REQUIRE(!instance.is_valid());
}

BOOST_AUTO_TEST_CASE(get_compact_filter_headers__constructor_2__always__equals_params)
{
    const uint8_t filter_type = 16u;
    const uint32_t start_height = 62334u;
    const hash_digest stop_hash = hash_literal("bb5b26270e07d26283238bcbefb622682a43e9c5bb51a8276b3309f7553d4040");

    message::get_compact_filter_headers instance(filter_type, start_height, stop_hash);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(filter_type == instance.filter_type());
    BOOST_REQUIRE(start_height == instance.start_height());
    BOOST_REQUIRE(stop_hash == instance.stop_hash());
}

BOOST_AUTO_TEST_CASE(get_compact_filter_headers__constructor_3__always__equals_params)
{
    const uint8_t filter_type = 16u;
    const uint32_t start_height = 62334u;
    const hash_digest stop_hash = hash_literal("bb5b26270e07d26283238bcbefb622682a43e9c5bb51a8276b3309f7553d4040");
    hash_digest copy_stop = stop_hash;

    message::get_compact_filter_headers instance(filter_type, start_height, std::move(copy_stop));

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(filter_type == instance.filter_type());
    BOOST_REQUIRE(start_height == instance.start_height());
    BOOST_REQUIRE(stop_hash == instance.stop_hash());
}

BOOST_AUTO_TEST_CASE(get_compact_filter_headers__constructor_4__always__equals_params)
{
    const uint8_t filter_type = 16u;
    const uint32_t start_height = 62334u;
    const hash_digest stop_hash = hash_literal("bb5b26270e07d26283238bcbefb622682a43e9c5bb51a8276b3309f7553d4040");

    message::get_compact_filter_headers value(filter_type, start_height, stop_hash);

    message::get_compact_filter_headers instance(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(filter_type == instance.filter_type());
    BOOST_REQUIRE(start_height == instance.start_height());
    BOOST_REQUIRE(stop_hash == instance.stop_hash());
}

BOOST_AUTO_TEST_CASE(get_compact_filter_headers__constructor_5__always__equals_params)
{
    const uint8_t filter_type = 16u;
    const uint32_t start_height = 62334u;
    const hash_digest stop_hash = hash_literal("bb5b26270e07d26283238bcbefb622682a43e9c5bb51a8276b3309f7553d4040");

    message::get_compact_filter_headers value(filter_type, start_height, stop_hash);

    message::get_compact_filter_headers instance(std::move(value));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(filter_type == instance.filter_type());
    BOOST_REQUIRE(start_height == instance.start_height());
    BOOST_REQUIRE(stop_hash == instance.stop_hash());
}

BOOST_AUTO_TEST_CASE(get_compact_filter_headers__from_data__insufficient_bytes__failure)
{
    const auto raw = to_chunk(base16_literal(
        "0b"
        "000032"));

    message::get_compact_filter_headers instance;
    BOOST_REQUIRE_EQUAL(false, instance.from_data(message::get_compact_filter_headers::version_minimum, raw));
}

BOOST_AUTO_TEST_CASE(get_compact_filter_headers__from_data__insufficient_version__failure)
{
    const auto raw = to_chunk(base16_literal(
        "0b"
        "00000FFF"
        "bb5b26270e07d26283238bcbefb622682a43e9c5bb51a8276b3309f7553d4040"));

    message::get_compact_filter_headers expected;
    const auto result = expected.from_data(message::get_compact_filter_headers::version_minimum, raw);
    BOOST_REQUIRE(result);
    BOOST_REQUIRE(expected.is_valid());

    const auto data = expected.to_data(message::get_compact_filter_headers::version_minimum);
    BOOST_REQUIRE(raw == data);

    message::get_compact_filter_headers instance;
    BOOST_REQUIRE_EQUAL(false, instance.from_data(message::get_compact_filter_headers::version_minimum - 1, data));
}

BOOST_AUTO_TEST_CASE(get_compact_filter_headers__factory_1__valid_input__success)
{
    const auto raw = to_chunk(base16_literal(
        "0b"
        "00000FFF"
        "bb5b26270e07d26283238bcbefb622682a43e9c5bb51a8276b3309f7553d4040"));

    message::get_compact_filter_headers expected;
    expected.from_data(message::get_compact_filter_headers::version_minimum, raw);
    const auto data = expected.to_data(message::get_compact_filter_headers::version_minimum);
    BOOST_REQUIRE(raw == data);

    const auto result = message::get_compact_filter_headers::factory(message::get_compact_filter_headers::version_minimum, data);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(message::get_compact_filter_headers::version_minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::get_compact_filter_headers::version_minimum), result.serialized_size(message::get_compact_filter_headers::version_minimum));
}

BOOST_AUTO_TEST_CASE(get_compact_filter_headers__factory_2__valid_input__success)
{
    const auto raw = to_chunk(base16_literal(
        "0b"
        "00000FFF"
        "bb5b26270e07d26283238bcbefb622682a43e9c5bb51a8276b3309f7553d4040"));

    message::get_compact_filter_headers expected;
    expected.from_data(message::get_compact_filter_headers::version_minimum, raw);
    const auto data = expected.to_data(message::get_compact_filter_headers::version_minimum);
    BOOST_REQUIRE(raw == data);

    stream::in::copy istream(data);
    const auto result = message::get_compact_filter_headers::factory(message::get_compact_filter_headers::version_minimum, istream);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(message::get_compact_filter_headers::version_minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::get_compact_filter_headers::version_minimum), result.serialized_size(message::get_compact_filter_headers::version_minimum));
}

BOOST_AUTO_TEST_CASE(get_compact_filter_headers__factory_3__valid_input__success)
{
    const auto raw = to_chunk(base16_literal(
        "0b"
        "00000FFF"
        "bb5b26270e07d26283238bcbefb622682a43e9c5bb51a8276b3309f7553d4040"));

    message::get_compact_filter_headers expected;
    expected.from_data(message::get_compact_filter_headers::version_minimum, raw);
    const auto data = expected.to_data(message::get_compact_filter_headers::version_minimum);
    BOOST_REQUIRE(raw == data);

    read::bytes::copy source(data);
    const auto result = message::get_compact_filter_headers::factory(message::get_compact_filter_headers::version_minimum, source);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(message::get_compact_filter_headers::version_minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::get_compact_filter_headers::version_minimum), result.serialized_size(message::get_compact_filter_headers::version_minimum));
}

BOOST_AUTO_TEST_CASE(get_compact_filter_headers__filter_type_accessor__always__returns_initialized_value)
{
    const uint8_t filter_type = 55u;
    const uint32_t start_height = 64323u;
    const hash_digest stop_hash = hash_literal("bb5b26270e07d26283238bcbefb622682a43e9c5bb51a8276b3309f7553d4040");

    message::get_compact_filter_headers instance(filter_type, start_height, stop_hash);

    BOOST_REQUIRE(filter_type == instance.filter_type());
}

BOOST_AUTO_TEST_CASE(get_compact_filter_headers__filter_type_setter__roundtrip__success)
{
    const uint8_t filter_type = 38u;

    message::get_compact_filter_headers instance;
    BOOST_REQUIRE(filter_type != instance.filter_type());

    instance.set_filter_type(filter_type);
    BOOST_REQUIRE(filter_type == instance.filter_type());
}

BOOST_AUTO_TEST_CASE(get_compact_filter_headers__start_height_accessor__always__returns_initialized_value)
{
    const uint8_t filter_type = 55u;
    const uint32_t start_height = 64323u;
    const hash_digest stop_hash = hash_literal("bb5b26270e07d26283238bcbefb622682a43e9c5bb51a8276b3309f7553d4040");

    message::get_compact_filter_headers instance(filter_type, start_height, stop_hash);
    BOOST_REQUIRE(start_height == instance.start_height());
}

BOOST_AUTO_TEST_CASE(get_compact_filter_headers__start_height_setter__roundtrip__success)
{
    const uint32_t start_height = 38u;

    message::get_compact_filter_headers instance;
    BOOST_REQUIRE(start_height != instance.start_height());

    instance.set_start_height(start_height);
    BOOST_REQUIRE(start_height == instance.start_height());
}

BOOST_AUTO_TEST_CASE(get_compact_filter_headers__stop_hash_accessor_1__always__returns_initialized_value)
{
    const uint8_t filter_type = 55u;
    const uint32_t start_height = 64323u;
    const hash_digest stop_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    message::get_compact_filter_headers instance(filter_type, start_height, stop_hash);
    BOOST_REQUIRE(stop_hash == instance.stop_hash());
}

BOOST_AUTO_TEST_CASE(get_compact_filter_headers__stop_hash_accessor_2__always__returns_initialized_value)
{
    const uint8_t filter_type = 55u;
    const uint32_t start_height = 64323u;
    const hash_digest stop_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    const message::get_compact_filter_headers instance(filter_type, start_height, stop_hash);
    BOOST_REQUIRE(stop_hash == instance.stop_hash());
}

BOOST_AUTO_TEST_CASE(get_compact_filter_headers__stop_hash_setter_1__roundtrip__success)
{
    const hash_digest stop_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    message::get_compact_filter_headers instance;
    BOOST_REQUIRE(stop_hash != instance.stop_hash());

    instance.set_stop_hash(stop_hash);
    BOOST_REQUIRE(stop_hash == instance.stop_hash());
}

BOOST_AUTO_TEST_CASE(get_compact_filter_headers__stop_hash_setter_2__roundtrip__success)
{
    const hash_digest stop_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    hash_digest copy = stop_hash;

    message::get_compact_filter_headers instance;
    BOOST_REQUIRE(stop_hash != instance.stop_hash());

    instance.set_stop_hash(std::move(copy));
    BOOST_REQUIRE(stop_hash == instance.stop_hash());
}

BOOST_AUTO_TEST_CASE(get_compact_filter_headers__operator_assign_equals__always__matches_equivalent)
{
    const uint8_t filter_type = 55u;
    const uint32_t start_height = 64323u;
    const hash_digest stop_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    message::get_compact_filter_headers value(filter_type, start_height, stop_hash);
    BOOST_REQUIRE(value.is_valid());

    message::get_compact_filter_headers instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(filter_type == instance.filter_type());
    BOOST_REQUIRE(start_height == instance.start_height());
    BOOST_REQUIRE(stop_hash == instance.stop_hash());
}

BOOST_AUTO_TEST_CASE(get_compact_filter_headers__operator_boolean_equals__duplicates__returns_true)
{
    const message::get_compact_filter_headers expected(
        19u,
        2135u,
        hash_literal("01cf1a02915dc0137cae81ea04161dbebc0d7ec445c2704840b05ca52fce8484"));

    message::get_compact_filter_headers instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(get_compact_filter_headers__operator_boolean_equals__differs__returns_false)
{
    const message::get_compact_filter_headers expected(
        19u,
        2341u,
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"));

    message::get_compact_filter_headers instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(get_compact_filter_headers__operator_boolean_not_equals__duplicates__returns_false)
{
    const message::get_compact_filter_headers expected(
        19u,
        3546u,
        hash_literal("01cf1a02915dc0137cae81ea04161dbebc0d7ec445c2704840b05ca52fce8484"));

    message::get_compact_filter_headers instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(get_compact_filter_headers__operator_boolean_not_equals__differs__returns_true)
{
    const message::get_compact_filter_headers expected(
        19u,
        549u,
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"));

    message::get_compact_filter_headers instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()
