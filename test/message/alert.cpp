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

BOOST_AUTO_TEST_SUITE(alert_tests)

BOOST_AUTO_TEST_CASE(alert__constructor_1__always__invalid)
{
    message::alert instance;
    BOOST_REQUIRE(!instance.is_valid());
}

BOOST_AUTO_TEST_CASE(alert__constructor_2__always__equals_params)
{
    const auto payload = base16_chunk("0123456789abcdef");
    const auto signature = base16_chunk("fedcba9876543210");

    message::alert instance(payload, signature);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(payload == instance.payload());
    BOOST_REQUIRE(signature == instance.signature());
}

BOOST_AUTO_TEST_CASE(alert__constructor_3__always__equals_params)
{
    const auto payload = base16_chunk("0123456789abcdef");
    const auto signature = base16_chunk("fedcba9876543210");
    auto dup_payload = payload;
    auto dup_signature = signature;

    message::alert instance(std::move(dup_payload), std::move(dup_signature));

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(payload == instance.payload());
    BOOST_REQUIRE(signature == instance.signature());
}

BOOST_AUTO_TEST_CASE(alert__constructor_4__always__equals_params)
{
    const auto payload = base16_chunk("0123456789abcdef");
    const auto signature = base16_chunk("fedcba9876543210");

    message::alert value(payload, signature);
    message::alert instance(value);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(value == instance);
    BOOST_REQUIRE(payload == instance.payload());
    BOOST_REQUIRE(signature == instance.signature());
}

BOOST_AUTO_TEST_CASE(alert__constructor_5__always__equals_params)
{
    const auto payload = base16_chunk("0123456789abcdef");
    const auto signature = base16_chunk("fedcba9876543210");

    message::alert value(payload, signature);
    message::alert instance(std::move(value));

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(payload == instance.payload());
    BOOST_REQUIRE(signature == instance.signature());
}

BOOST_AUTO_TEST_CASE(alert__from_data__insufficient_bytes__failure)
{
    const data_chunk raw{ 0xab, 0x11 };
    message::alert instance;

    BOOST_REQUIRE(!instance.from_data(message::version::level::minimum, raw));
    BOOST_REQUIRE(!instance.is_valid());
}

BOOST_AUTO_TEST_CASE(alert__factory_1__wiki_sample__success)
{
    const data_chunk raw_payload
    {
        0x01, 0x00, 0x00, 0x00, 0x37, 0x66, 0x40, 0x4f, 0x00,
        0x00, 0x00, 0x00, 0xb3, 0x05, 0x43, 0x4f, 0x00, 0x00, 0x00,
        0x00, 0xf2, 0x03, 0x00, 0x00, 0xf1, 0x03, 0x00, 0x00, 0x00,
        0x10, 0x27, 0x00, 0x00, 0x48, 0xee, 0x00, 0x00, 0x00, 0x64,
        0x00, 0x00, 0x00, 0x00, 0x46, 0x53, 0x65, 0x65, 0x20, 0x62,
        0x69, 0x74, 0x63, 0x6f, 0x69, 0x6e, 0x2e, 0x6f, 0x72, 0x67,
        0x2f, 0x66, 0x65, 0x62, 0x32, 0x30, 0x20, 0x69, 0x66, 0x20,
        0x79, 0x6f, 0x75, 0x20, 0x68, 0x61, 0x76, 0x65, 0x20, 0x74,
        0x72, 0x6f, 0x75, 0x62, 0x6c, 0x65, 0x20, 0x63, 0x6f, 0x6e,
        0x6e, 0x65, 0x63, 0x74, 0x69, 0x6e, 0x67, 0x20, 0x61, 0x66,
        0x74, 0x65, 0x72, 0x20, 0x32, 0x30, 0x20, 0x46, 0x65, 0x62,
        0x72, 0x75, 0x61, 0x72, 0x79, 0x00
    };

    const data_chunk raw_signature
    {
        0x30, 0x45, 0x02, 0x21, 0x00, 0x83, 0x89, 0xdf, 0x45, 0xf0,
        0x70, 0x3f, 0x39, 0xec, 0x8c, 0x1c, 0xc4, 0x2c, 0x13, 0x81,
        0x0f, 0xfc, 0xae, 0x14, 0x99, 0x5b, 0xb6, 0x48, 0x34, 0x02,
        0x19, 0xe3, 0x53, 0xb6, 0x3b, 0x53, 0xeb, 0x02, 0x20, 0x09,
        0xec, 0x65, 0xe1, 0xc1, 0xaa, 0xee, 0xc1, 0xfd, 0x33, 0x4c,
        0x6b, 0x68, 0x4b, 0xde, 0x2b, 0x3f, 0x57, 0x30, 0x60, 0xd5,
        0xb7, 0x0c, 0x3a, 0x46, 0x72, 0x33, 0x26, 0xe4, 0xe8, 0xa4,
        0xf1
    };

    const data_chunk raw
    {
        0x73, 0x01, 0x00, 0x00, 0x00, 0x37, 0x66, 0x40, 0x4f, 0x00,
        0x00, 0x00, 0x00, 0xb3, 0x05, 0x43, 0x4f, 0x00, 0x00, 0x00,
        0x00, 0xf2, 0x03, 0x00, 0x00, 0xf1, 0x03, 0x00, 0x00, 0x00,
        0x10, 0x27, 0x00, 0x00, 0x48, 0xee, 0x00, 0x00, 0x00, 0x64,
        0x00, 0x00, 0x00, 0x00, 0x46, 0x53, 0x65, 0x65, 0x20, 0x62,
        0x69, 0x74, 0x63, 0x6f, 0x69, 0x6e, 0x2e, 0x6f, 0x72, 0x67,
        0x2f, 0x66, 0x65, 0x62, 0x32, 0x30, 0x20, 0x69, 0x66, 0x20,
        0x79, 0x6f, 0x75, 0x20, 0x68, 0x61, 0x76, 0x65, 0x20, 0x74,
        0x72, 0x6f, 0x75, 0x62, 0x6c, 0x65, 0x20, 0x63, 0x6f, 0x6e,
        0x6e, 0x65, 0x63, 0x74, 0x69, 0x6e, 0x67, 0x20, 0x61, 0x66,
        0x74, 0x65, 0x72, 0x20, 0x32, 0x30, 0x20, 0x46, 0x65, 0x62,
        0x72, 0x75, 0x61, 0x72, 0x79, 0x00, 0x47, 0x30, 0x45, 0x02,
        0x21, 0x00, 0x83, 0x89, 0xdf, 0x45, 0xf0, 0x70, 0x3f, 0x39,
        0xec, 0x8c, 0x1c, 0xc4, 0x2c, 0x13, 0x81, 0x0f, 0xfc, 0xae,
        0x14, 0x99, 0x5b, 0xb6, 0x48, 0x34, 0x02, 0x19, 0xe3, 0x53,
        0xb6, 0x3b, 0x53, 0xeb, 0x02, 0x20, 0x09, 0xec, 0x65, 0xe1,
        0xc1, 0xaa, 0xee, 0xc1, 0xfd, 0x33, 0x4c, 0x6b, 0x68, 0x4b,
        0xde, 0x2b, 0x3f, 0x57, 0x30, 0x60, 0xd5, 0xb7, 0x0c, 0x3a,
        0x46, 0x72, 0x33, 0x26, 0xe4, 0xe8, 0xa4, 0xf1
    };

    const message::alert expected{ raw_payload, raw_signature };
    const auto result = message::alert::factory(
        message::version::level::minimum, raw);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE_EQUAL(raw.size(), result.serialized_size(message::version::level::minimum));
    BOOST_REQUIRE(result == expected);

    const auto data = expected.to_data(message::version::level::minimum);

    BOOST_REQUIRE_EQUAL(raw.size(), data.size());
    BOOST_REQUIRE_EQUAL(data.size(), expected.serialized_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_CASE(alert__factory_1__roundtrip__success)
{
    const message::alert expected
    {
        { 0x07, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 },
        { 0x04, 0xff, 0xab, 0xcd, 0xee }
    };

    const auto data = expected.to_data(message::version::level::minimum);
    const auto result = message::alert::factory(
        message::version::level::minimum, data);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(message::version::level::minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::minimum), result.serialized_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_CASE(alert__factory_2__roundtrip__success)
{
    const message::alert expected
    {
        { 0x07, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 },
        { 0x04, 0xff, 0xab, 0xcd, 0xee }
    };

    const auto data = expected.to_data(message::version::level::minimum);
    stream::in::copy istream(data);
    const auto result = message::alert::factory(message::version::level::minimum, istream);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(message::version::level::minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::minimum), result.serialized_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_CASE(alert__factory_3__roundtrip__success)
{
    const message::alert expected
    {
        { 0x07, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 },
        { 0x04, 0xff, 0xab, 0xcd, 0xee }
    };

    const auto data = expected.to_data(message::version::level::minimum);
    read::bytes::copy source(data);
    const auto result = message::alert::factory(message::version::level::minimum, source);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(message::version::level::minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::minimum), result.serialized_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_CASE(alert__payload_accessor_1__always__returns_initialized)
{
    const auto payload = base16_chunk("0123456789abcdef");
    const auto signature = base16_chunk("fedcba9876543210");

    message::alert instance(payload, signature);
    BOOST_REQUIRE(payload == instance.payload());
}

BOOST_AUTO_TEST_CASE(alert__payload_accessor_2__always__returns_initialized)
{
    const auto payload = base16_chunk("0123456789abcdef");
    const auto signature = base16_chunk("fedcba9876543210");

    const message::alert instance(payload, signature);
    BOOST_REQUIRE(payload == instance.payload());
}

BOOST_AUTO_TEST_CASE(alert__payload_setter_1__roundtrip__success)
{
    const auto value = base16_chunk("aabbccddeeff");
    message::alert instance;
    BOOST_REQUIRE(instance.payload() != value);
    instance.set_payload(value);
    BOOST_REQUIRE(value == instance.payload());
}

BOOST_AUTO_TEST_CASE(alert__payload_setter_2__roundtrip__success)
{
    const auto value = base16_chunk("aabbccddeeff");
    auto dup_value = value;
    message::alert instance;
    BOOST_REQUIRE(instance.payload() != value);
    instance.set_payload(std::move(dup_value));
    BOOST_REQUIRE(value == instance.payload());
}

BOOST_AUTO_TEST_CASE(alert__signature_accessor_1__always__returns_initialized)
{
    const auto payload = base16_chunk("0123456789abcdef");
    const auto signature = base16_chunk("fedcba9876543210");

    message::alert instance(payload, signature);
    BOOST_REQUIRE(signature == instance.signature());
}

BOOST_AUTO_TEST_CASE(alert__signature_accessor_2__always__returns_initialized)
{
    const auto payload = base16_chunk("0123456789abcdef");
    const auto signature = base16_chunk("fedcba9876543210");

    const message::alert instance(payload, signature);
    BOOST_REQUIRE(signature == instance.signature());
}

BOOST_AUTO_TEST_CASE(alert__signature_setter_1__roundtrip__success)
{
    const auto value = base16_chunk("aabbccddeeff");
    message::alert instance;
    BOOST_REQUIRE(instance.signature() != value);
    instance.set_signature(value);
    BOOST_REQUIRE(value == instance.signature());
}

BOOST_AUTO_TEST_CASE(alert__signature_setter_2__roundtrip__success)
{
    const auto value = base16_chunk("aabbccddeeff");
    auto dup_value = value;
    message::alert instance;
    BOOST_REQUIRE(instance.signature() != value);
    instance.set_signature(std::move(dup_value));
    BOOST_REQUIRE(value == instance.signature());
}

BOOST_AUTO_TEST_CASE(alert__operator_assign_equals__always__matches_equivalent)
{
    const auto payload = base16_chunk("0123456789abcdef");
    const auto signature = base16_chunk("fedcba9876543210");

    message::alert value(payload, signature);

    BOOST_REQUIRE(value.is_valid());

    message::alert instance;
    BOOST_REQUIRE(!instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(payload == instance.payload());
    BOOST_REQUIRE(signature == instance.signature());
}

BOOST_AUTO_TEST_CASE(alert__operator_boolean_equals__duplicates__returns_true)
{
    const message::alert expected(
        base16_chunk("0123456789abcdef"),
        base16_chunk("fedcba9876543210"));

    message::alert instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(alert__operator_boolean_equals__differs__returns_false)
{
    const message::alert expected(
        base16_chunk("0123456789abcdef"),
        base16_chunk("fedcba9876543210"));

    message::alert instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(alert__operator_boolean_not_equals__duplicates__returns_false)
{
    const message::alert expected(
        base16_chunk("0123456789abcdef"),
        base16_chunk("fedcba9876543210"));

    message::alert instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(alert__operator_boolean_not_equals__differs__returns_true)
{
    const message::alert expected(
        base16_chunk("0123456789abcdef"),
        base16_chunk("fedcba9876543210"));

    message::alert instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()
