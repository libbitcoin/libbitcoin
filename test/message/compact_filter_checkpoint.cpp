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

BOOST_AUTO_TEST_SUITE(compact_filter_checkpoint_tests)

BOOST_AUTO_TEST_CASE(compact_filter_checkpoint__constructor_1__always__invalid)
{
    message::compact_filter_checkpoint instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());
}

BOOST_AUTO_TEST_CASE(compact_filter_checkpoint__constructor_2__always__equals_params)
{
    const uint8_t filter_type = 16u;
    const hash_digest stop_hash = hash_literal("bb5b26270e07d26283238bcbefb622682a43e9c5bb51a8276b3309f7553d4040");
    const hash_list filter_headers
    {
        hash_literal("00000000000000000018a65ff0bbbc2a93493c693d05dd65c6a8dcbb881f55fb"),
        hash_literal("00000000000000000026fe62acfabe9fae5f986e0eaeda21d5830c15ac4bdf89"),
        hash_literal("00000000000000000000eecdc8cec3ed73c2298ced2e0fbca3e7c46b033b58d0"),
        hash_literal("0000000000000000001f5f2b512fbf0b4538398852857455eb8d02dfdc042eeb"),
        hash_literal("00000000000000000020f5d0a7cb16e4946c9dbb7c62416ea292d6a10229c83c")
    };

    message::compact_filter_checkpoint instance(filter_type, stop_hash, filter_headers);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(filter_type == instance.filter_type());
    BOOST_REQUIRE(stop_hash == instance.stop_hash());
    BOOST_REQUIRE(filter_headers == instance.filter_headers());
}

BOOST_AUTO_TEST_CASE(compact_filter_checkpoint__constructor_3__always__equals_params)
{
    const uint8_t filter_type = 16u;
    const hash_digest stop_hash = hash_literal("bb5b26270e07d26283238bcbefb622682a43e9c5bb51a8276b3309f7553d4040");
    hash_digest dup_stop = stop_hash;

    const hash_list filter_headers
    {
        hash_literal("00000000000000000018a65ff0bbbc2a93493c693d05dd65c6a8dcbb881f55fb"),
        hash_literal("00000000000000000026fe62acfabe9fae5f986e0eaeda21d5830c15ac4bdf89"),
        hash_literal("00000000000000000000eecdc8cec3ed73c2298ced2e0fbca3e7c46b033b58d0"),
        hash_literal("0000000000000000001f5f2b512fbf0b4538398852857455eb8d02dfdc042eeb"),
        hash_literal("00000000000000000020f5d0a7cb16e4946c9dbb7c62416ea292d6a10229c83c")
    };

    hash_list dup_filter_headers = filter_headers;

    message::compact_filter_checkpoint instance(filter_type, std::move(dup_stop), std::move(dup_filter_headers));

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(filter_type == instance.filter_type());
    BOOST_REQUIRE(stop_hash == instance.stop_hash());
    BOOST_REQUIRE(filter_headers == instance.filter_headers());
}

BOOST_AUTO_TEST_CASE(compact_filter_checkpoint__constructor_4__always__equals_params)
{
    const uint8_t filter_type = 16u;
    const hash_digest stop_hash = hash_literal("bb5b26270e07d26283238bcbefb622682a43e9c5bb51a8276b3309f7553d4040");
    const hash_list filter_headers
    {
        hash_literal("00000000000000000018a65ff0bbbc2a93493c693d05dd65c6a8dcbb881f55fb"),
        hash_literal("00000000000000000026fe62acfabe9fae5f986e0eaeda21d5830c15ac4bdf89"),
        hash_literal("00000000000000000000eecdc8cec3ed73c2298ced2e0fbca3e7c46b033b58d0"),
        hash_literal("0000000000000000001f5f2b512fbf0b4538398852857455eb8d02dfdc042eeb"),
        hash_literal("00000000000000000020f5d0a7cb16e4946c9dbb7c62416ea292d6a10229c83c")
    };

    message::compact_filter_checkpoint value(filter_type, stop_hash, filter_headers);

    message::compact_filter_checkpoint instance(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(filter_type == instance.filter_type());
    BOOST_REQUIRE(stop_hash == instance.stop_hash());
    BOOST_REQUIRE(filter_headers == instance.filter_headers());
}

BOOST_AUTO_TEST_CASE(compact_filter_checkpoint__constructor_5__always__equals_params)
{
    const uint8_t filter_type = 16u;
    const hash_digest stop_hash = hash_literal("bb5b26270e07d26283238bcbefb622682a43e9c5bb51a8276b3309f7553d4040");
    const hash_list filter_headers = {
        hash_literal("00000000000000000018a65ff0bbbc2a93493c693d05dd65c6a8dcbb881f55fb"),
        hash_literal("00000000000000000026fe62acfabe9fae5f986e0eaeda21d5830c15ac4bdf89"),
        hash_literal("00000000000000000000eecdc8cec3ed73c2298ced2e0fbca3e7c46b033b58d0"),
        hash_literal("0000000000000000001f5f2b512fbf0b4538398852857455eb8d02dfdc042eeb"),
        hash_literal("00000000000000000020f5d0a7cb16e4946c9dbb7c62416ea292d6a10229c83c")
    };

    message::compact_filter_checkpoint value(filter_type, stop_hash, filter_headers);

    message::compact_filter_checkpoint instance(std::move(value));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(filter_type == instance.filter_type());
    BOOST_REQUIRE(stop_hash == instance.stop_hash());
    BOOST_REQUIRE(filter_headers == instance.filter_headers());
}

BOOST_AUTO_TEST_CASE(compact_filter_checkpoint__from_data__insufficient_bytes__failure)
{
    const auto raw = to_chunk(base16_literal(
        "0b"
        "01cf1a02915dc0137cae81ea04161dbebc0d7ec4"));

    message::compact_filter_checkpoint instance;
    BOOST_REQUIRE_EQUAL(false, instance.from_data(message::compact_filter_checkpoint::version_minimum, raw));
}

BOOST_AUTO_TEST_CASE(compact_filter_checkpoint__from_data__insufficient_bytes_mid_list__failure)
{
    const auto raw = to_chunk(base16_literal(
        "0b"
        "01cf1a02915dc0137cae81ea04161dbebc0d7ec445c2704840b05ca52fce8484"
        "05"
        "00000000000000000018a65ff0bbbc2a93493c693d05dd65c6a8dcbb881f55fb"
        "00000000000000000026fe62acfabe9fae5f986e0eaeda21d5830c15ac4bdf89"
        "00000000000000000000eecdc8cec3"));


    message::compact_filter_checkpoint instance;
    BOOST_REQUIRE_EQUAL(false, instance.from_data(message::compact_filter_checkpoint::version_minimum, raw));
}

BOOST_AUTO_TEST_CASE(compact_filter_checkpoint__from_data__insufficient_version__failure)
{
    const auto raw = to_chunk(base16_literal(
        "0b"
        "bb5b26270e07d26283238bcbefb622682a43e9c5bb51a8276b3309f7553d4040"
        "05"
        "00000000000000000018a65ff0bbbc2a93493c693d05dd65c6a8dcbb881f55fb"
        "00000000000000000026fe62acfabe9fae5f986e0eaeda21d5830c15ac4bdf89"
        "00000000000000000000eecdc8cec3ed73c2298ced2e0fbca3e7c46b033b58d0"
        "0000000000000000001f5f2b512fbf0b4538398852857455eb8d02dfdc042eeb"
        "00000000000000000020f5d0a7cb16e4946c9dbb7c62416ea292d6a10229c83c"));

    message::compact_filter_checkpoint expected;
    auto result = expected.from_data(message::compact_filter_checkpoint::version_minimum, raw);
    BOOST_REQUIRE_EQUAL(true, result);
    BOOST_REQUIRE_EQUAL(true, expected.is_valid());

    const auto data = expected.to_data(message::compact_filter_checkpoint::version_minimum);
    BOOST_REQUIRE(raw == data);

    message::compact_filter_checkpoint instance;
    BOOST_REQUIRE_EQUAL(false, instance.from_data(message::compact_filter_checkpoint::version_minimum - 1, data));
}

BOOST_AUTO_TEST_CASE(compact_filter_checkpoint__factory_1__valid_input__success)
{
    const auto raw = to_chunk(base16_literal(
        "0b"
        "bb5b26270e07d26283238bcbefb622682a43e9c5bb51a8276b3309f7553d4040"
        "05"
        "00000000000000000018a65ff0bbbc2a93493c693d05dd65c6a8dcbb881f55fb"
        "00000000000000000026fe62acfabe9fae5f986e0eaeda21d5830c15ac4bdf89"
        "00000000000000000000eecdc8cec3ed73c2298ced2e0fbca3e7c46b033b58d0"
        "0000000000000000001f5f2b512fbf0b4538398852857455eb8d02dfdc042eeb"
        "00000000000000000020f5d0a7cb16e4946c9dbb7c62416ea292d6a10229c83c"));

    message::compact_filter_checkpoint expected;
    expected.from_data(message::compact_filter_checkpoint::version_minimum, raw);
    const auto data = expected.to_data(message::compact_filter_checkpoint::version_minimum);
    BOOST_REQUIRE(raw == data);

    const auto result = message::compact_filter_checkpoint::factory(message::compact_filter_checkpoint::version_minimum, data);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(message::compact_filter_checkpoint::version_minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::compact_filter_checkpoint::version_minimum), result.serialized_size(message::compact_filter_checkpoint::version_minimum));
}

BOOST_AUTO_TEST_CASE(compact_filter_checkpoint__factory_2__valid_input__success)
{
    const auto raw = to_chunk(base16_literal(
        "0b"
        "01cf1a02915dc0137cae81ea04161dbebc0d7ec445c2704840b05ca52fce8484"
        "05"
        "00000000000000000018a65ff0bbbc2a93493c693d05dd65c6a8dcbb881f55fb"
        "00000000000000000026fe62acfabe9fae5f986e0eaeda21d5830c15ac4bdf89"
        "00000000000000000000eecdc8cec3ed73c2298ced2e0fbca3e7c46b033b58d0"
        "0000000000000000001f5f2b512fbf0b4538398852857455eb8d02dfdc042eeb"
        "00000000000000000020f5d0a7cb16e4946c9dbb7c62416ea292d6a10229c83c"));

    message::compact_filter_checkpoint expected;
    expected.from_data(message::compact_filter_checkpoint::version_minimum, raw);
    const auto data = expected.to_data(message::compact_filter_checkpoint::version_minimum);
    BOOST_REQUIRE(raw == data);

    stream::in::copy istream(data);
    auto result = message::compact_filter_checkpoint::factory(message::compact_filter_checkpoint::version_minimum, istream);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(message::compact_filter_checkpoint::version_minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::compact_filter_checkpoint::version_minimum), result.serialized_size(message::compact_filter_checkpoint::version_minimum));
}

BOOST_AUTO_TEST_CASE(compact_filter_checkpoint__factory_3__valid_input__success)
{
    const auto raw = to_chunk(base16_literal(
        "0b"
        "01cf1a02915dc0137cae81ea04161dbebc0d7ec445c2704840b05ca52fce8484"
        "05"
        "00000000000000000018a65ff0bbbc2a93493c693d05dd65c6a8dcbb881f55fb"
        "00000000000000000026fe62acfabe9fae5f986e0eaeda21d5830c15ac4bdf89"
        "00000000000000000000eecdc8cec3ed73c2298ced2e0fbca3e7c46b033b58d0"
        "0000000000000000001f5f2b512fbf0b4538398852857455eb8d02dfdc042eeb"
        "00000000000000000020f5d0a7cb16e4946c9dbb7c62416ea292d6a10229c83c"));

    message::compact_filter_checkpoint expected;
    expected.from_data(message::compact_filter_checkpoint::version_minimum, raw);
    const auto data = expected.to_data(message::compact_filter_checkpoint::version_minimum);
    BOOST_REQUIRE(raw == data);

    read::bytes::copy source(data);
    const auto result = message::compact_filter_checkpoint::factory(message::compact_filter_checkpoint::version_minimum, source);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(message::compact_filter_checkpoint::version_minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::compact_filter_checkpoint::version_minimum), result.serialized_size(message::compact_filter_checkpoint::version_minimum));
}

BOOST_AUTO_TEST_CASE(compact_filter_checkpoint__filter_type_accessor__always__returns_initialized_value)
{
    const uint8_t filter_type = 55u;
    const hash_digest stop_hash = hash_literal("bb5b26270e07d26283238bcbefb622682a43e9c5bb51a8276b3309f7553d4040");
    const hash_list filter_headers
    {
        hash_literal("00000000000000000018a65ff0bbbc2a93493c693d05dd65c6a8dcbb881f55fb"),
        hash_literal("00000000000000000026fe62acfabe9fae5f986e0eaeda21d5830c15ac4bdf89"),
        hash_literal("00000000000000000000eecdc8cec3ed73c2298ced2e0fbca3e7c46b033b58d0"),
        hash_literal("0000000000000000001f5f2b512fbf0b4538398852857455eb8d02dfdc042eeb"),
        hash_literal("00000000000000000020f5d0a7cb16e4946c9dbb7c62416ea292d6a10229c83c")
    };

    message::compact_filter_checkpoint instance(filter_type, stop_hash, filter_headers);

    BOOST_REQUIRE(filter_type == instance.filter_type());
}

BOOST_AUTO_TEST_CASE(compact_filter_checkpoint__filter_type_setter__roundtrip__success)
{
    const uint8_t filter_type = 38u;

    message::compact_filter_checkpoint instance;
    BOOST_REQUIRE(filter_type != instance.filter_type());

    instance.set_filter_type(filter_type);
    BOOST_REQUIRE(filter_type == instance.filter_type());
}

BOOST_AUTO_TEST_CASE(compact_filter_checkpoint__stop_hash_accessor_1__always__returns_initialized_value)
{
    const uint8_t filter_type = 55u;
    const hash_digest stop_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const hash_list filter_headers
    {
        hash_literal("00000000000000000018a65ff0bbbc2a93493c693d05dd65c6a8dcbb881f55fb"),
        hash_literal("00000000000000000026fe62acfabe9fae5f986e0eaeda21d5830c15ac4bdf89"),
        hash_literal("00000000000000000000eecdc8cec3ed73c2298ced2e0fbca3e7c46b033b58d0"),
        hash_literal("0000000000000000001f5f2b512fbf0b4538398852857455eb8d02dfdc042eeb"),
        hash_literal("00000000000000000020f5d0a7cb16e4946c9dbb7c62416ea292d6a10229c83c")
    };

    message::compact_filter_checkpoint instance(filter_type, stop_hash, filter_headers);

    BOOST_REQUIRE(stop_hash == instance.stop_hash());
}

BOOST_AUTO_TEST_CASE(compact_filter_checkpoint__stop_hash_accessor_2__always__returns_initialized_value)
{
    const uint8_t filter_type = 55u;
    const hash_digest stop_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const hash_list filter_headers
    {
        hash_literal("00000000000000000018a65ff0bbbc2a93493c693d05dd65c6a8dcbb881f55fb"),
        hash_literal("00000000000000000026fe62acfabe9fae5f986e0eaeda21d5830c15ac4bdf89"),
        hash_literal("00000000000000000000eecdc8cec3ed73c2298ced2e0fbca3e7c46b033b58d0"),
        hash_literal("0000000000000000001f5f2b512fbf0b4538398852857455eb8d02dfdc042eeb"),
        hash_literal("00000000000000000020f5d0a7cb16e4946c9dbb7c62416ea292d6a10229c83c")
    };

    const message::compact_filter_checkpoint instance(filter_type, stop_hash, filter_headers);

    BOOST_REQUIRE(stop_hash == instance.stop_hash());
}

BOOST_AUTO_TEST_CASE(compact_filter_checkpoint__stop_hash_setter_1__roundtrip__success)
{
    const hash_digest stop_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    message::compact_filter_checkpoint instance;
    BOOST_REQUIRE(stop_hash != instance.stop_hash());

    instance.set_stop_hash(stop_hash);
    BOOST_REQUIRE(stop_hash == instance.stop_hash());
}

BOOST_AUTO_TEST_CASE(compact_filter_checkpoint__stop_hash_setter_2__roundtrip__success)
{
    const hash_digest stop_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    hash_digest copy = stop_hash;

    message::compact_filter_checkpoint instance;
    BOOST_REQUIRE(stop_hash != instance.stop_hash());

    instance.set_stop_hash(std::move(copy));
    BOOST_REQUIRE(stop_hash == instance.stop_hash());
}

BOOST_AUTO_TEST_CASE(compact_filter_checkpoint__filter_headers_accessor_1__always__returns_initialized_value)
{
    const uint8_t filter_type = 55u;
    const hash_digest stop_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const hash_list filter_headers
    {
        hash_literal("00000000000000000018a65ff0bbbc2a93493c693d05dd65c6a8dcbb881f55fb"),
        hash_literal("00000000000000000026fe62acfabe9fae5f986e0eaeda21d5830c15ac4bdf89"),
        hash_literal("00000000000000000000eecdc8cec3ed73c2298ced2e0fbca3e7c46b033b58d0"),
        hash_literal("0000000000000000001f5f2b512fbf0b4538398852857455eb8d02dfdc042eeb"),
        hash_literal("00000000000000000020f5d0a7cb16e4946c9dbb7c62416ea292d6a10229c83c")
    };

    message::compact_filter_checkpoint instance(filter_type, stop_hash, filter_headers);

    BOOST_REQUIRE(filter_headers == instance.filter_headers());
}

BOOST_AUTO_TEST_CASE(compact_filter_checkpoint__filter_headers_accessor_2__always__returns_initialized_value)
{
    const uint8_t filter_type = 55u;
    const hash_digest stop_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const hash_list filter_headers
    {
        hash_literal("00000000000000000018a65ff0bbbc2a93493c693d05dd65c6a8dcbb881f55fb"),
        hash_literal("00000000000000000026fe62acfabe9fae5f986e0eaeda21d5830c15ac4bdf89"),
        hash_literal("00000000000000000000eecdc8cec3ed73c2298ced2e0fbca3e7c46b033b58d0"),
        hash_literal("0000000000000000001f5f2b512fbf0b4538398852857455eb8d02dfdc042eeb"),
        hash_literal("00000000000000000020f5d0a7cb16e4946c9dbb7c62416ea292d6a10229c83c")
    };

    const message::compact_filter_checkpoint instance(filter_type, stop_hash, filter_headers);

    BOOST_REQUIRE(filter_headers == instance.filter_headers());
}

BOOST_AUTO_TEST_CASE(compact_filter_checkpoint__filter_headers_setter_1__roundtrip__success)
{
    const hash_list filter_headers
    {
        hash_literal("00000000000000000018a65ff0bbbc2a93493c693d05dd65c6a8dcbb881f55fb"),
        hash_literal("00000000000000000026fe62acfabe9fae5f986e0eaeda21d5830c15ac4bdf89"),
        hash_literal("00000000000000000000eecdc8cec3ed73c2298ced2e0fbca3e7c46b033b58d0"),
        hash_literal("0000000000000000001f5f2b512fbf0b4538398852857455eb8d02dfdc042eeb"),
        hash_literal("00000000000000000020f5d0a7cb16e4946c9dbb7c62416ea292d6a10229c83c")
    };

    message::compact_filter_checkpoint instance;
    BOOST_REQUIRE(filter_headers != instance.filter_headers());

    instance.set_filter_headers(filter_headers);
    BOOST_REQUIRE(filter_headers == instance.filter_headers());
}

BOOST_AUTO_TEST_CASE(compact_filter_checkpoint__filter_headers_setter_2__roundtrip__success)
{
    const hash_list filter_headers
    {
        hash_literal("00000000000000000018a65ff0bbbc2a93493c693d05dd65c6a8dcbb881f55fb"),
        hash_literal("00000000000000000026fe62acfabe9fae5f986e0eaeda21d5830c15ac4bdf89"),
        hash_literal("00000000000000000000eecdc8cec3ed73c2298ced2e0fbca3e7c46b033b58d0"),
        hash_literal("0000000000000000001f5f2b512fbf0b4538398852857455eb8d02dfdc042eeb"),
        hash_literal("00000000000000000020f5d0a7cb16e4946c9dbb7c62416ea292d6a10229c83c")
    };
    hash_list copy = filter_headers;

    message::compact_filter_checkpoint instance;

    BOOST_REQUIRE(filter_headers != instance.filter_headers());
    instance.set_filter_headers(std::move(copy));
    BOOST_REQUIRE(filter_headers == instance.filter_headers());
}

BOOST_AUTO_TEST_CASE(compact_filter_checkpoint__operator_assign_equals__always__matches_equivalent)
{
    const uint8_t filter_type = 55u;
    const hash_digest stop_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const hash_list filter_headers = {
        hash_literal("00000000000000000018a65ff0bbbc2a93493c693d05dd65c6a8dcbb881f55fb"),
        hash_literal("00000000000000000026fe62acfabe9fae5f986e0eaeda21d5830c15ac4bdf89"),
        hash_literal("00000000000000000000eecdc8cec3ed73c2298ced2e0fbca3e7c46b033b58d0"),
        hash_literal("0000000000000000001f5f2b512fbf0b4538398852857455eb8d02dfdc042eeb"),
        hash_literal("00000000000000000020f5d0a7cb16e4946c9dbb7c62416ea292d6a10229c83c")
    };

    message::compact_filter_checkpoint value(filter_type, stop_hash, filter_headers);

    BOOST_REQUIRE(value.is_valid());

    message::compact_filter_checkpoint instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(filter_type == instance.filter_type());
    BOOST_REQUIRE(stop_hash == instance.stop_hash());
    BOOST_REQUIRE(filter_headers == instance.filter_headers());
}

BOOST_AUTO_TEST_CASE(compact_filter_checkpoint__operator_boolean_equals__duplicates__returns_true)
{
    const message::compact_filter_checkpoint expected(
        19u,
        hash_literal("01cf1a02915dc0137cae81ea04161dbebc0d7ec445c2704840b05ca52fce8484"),
        {
            hash_literal("00000000000000000018a65ff0bbbc2a93493c693d05dd65c6a8dcbb881f55fb"),
            hash_literal("00000000000000000026fe62acfabe9fae5f986e0eaeda21d5830c15ac4bdf89"),
            hash_literal("00000000000000000000eecdc8cec3ed73c2298ced2e0fbca3e7c46b033b58d0"),
            hash_literal("0000000000000000001f5f2b512fbf0b4538398852857455eb8d02dfdc042eeb"),
            hash_literal("00000000000000000020f5d0a7cb16e4946c9dbb7c62416ea292d6a10229c83c")
        });

    message::compact_filter_checkpoint instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(compact_filter_checkpoint__operator_boolean_equals__differs__returns_false)
{
    const message::compact_filter_checkpoint expected(
        19u,
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        {
            hash_literal("00000000000000000018a65ff0bbbc2a93493c693d05dd65c6a8dcbb881f55fb"),
            hash_literal("00000000000000000026fe62acfabe9fae5f986e0eaeda21d5830c15ac4bdf89"),
            hash_literal("00000000000000000000eecdc8cec3ed73c2298ced2e0fbca3e7c46b033b58d0"),
            hash_literal("0000000000000000001f5f2b512fbf0b4538398852857455eb8d02dfdc042eeb"),
            hash_literal("00000000000000000020f5d0a7cb16e4946c9dbb7c62416ea292d6a10229c83c")
        });

    message::compact_filter_checkpoint instance;
    BOOST_REQUIRE(!(instance == expected));
}

BOOST_AUTO_TEST_CASE(compact_filter_checkpoint__operator_boolean_not_equals__duplicates__returns_false)
{
    const message::compact_filter_checkpoint expected(
        19u,
        hash_literal("01cf1a02915dc0137cae81ea04161dbebc0d7ec445c2704840b05ca52fce8484"),
        {
            hash_literal("00000000000000000018a65ff0bbbc2a93493c693d05dd65c6a8dcbb881f55fb"),
            hash_literal("00000000000000000026fe62acfabe9fae5f986e0eaeda21d5830c15ac4bdf89"),
            hash_literal("00000000000000000000eecdc8cec3ed73c2298ced2e0fbca3e7c46b033b58d0"),
            hash_literal("0000000000000000001f5f2b512fbf0b4538398852857455eb8d02dfdc042eeb"),
            hash_literal("00000000000000000020f5d0a7cb16e4946c9dbb7c62416ea292d6a10229c83c")
        });

    message::compact_filter_checkpoint instance(expected);
    BOOST_REQUIRE(!(instance != expected));
}

BOOST_AUTO_TEST_CASE(compact_filter_checkpoint__operator_boolean_not_equals__differs__returns_true)
{
    const message::compact_filter_checkpoint expected(
        19u,
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        {
            hash_literal("00000000000000000018a65ff0bbbc2a93493c693d05dd65c6a8dcbb881f55fb"),
            hash_literal("00000000000000000026fe62acfabe9fae5f986e0eaeda21d5830c15ac4bdf89"),
            hash_literal("00000000000000000000eecdc8cec3ed73c2298ced2e0fbca3e7c46b033b58d0"),
            hash_literal("0000000000000000001f5f2b512fbf0b4538398852857455eb8d02dfdc042eeb"),
            hash_literal("00000000000000000020f5d0a7cb16e4946c9dbb7c62416ea292d6a10229c83c")
        });

    message::compact_filter_checkpoint instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()
