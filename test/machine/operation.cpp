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

BOOST_AUTO_TEST_SUITE(operation_tests)

using namespace bc::system::chain;

const auto valid_raw_operation = base16_chunk("0900ff11ee22bb33aa44");

BOOST_AUTO_TEST_CASE(operation__constructor_1__always__returns_default_initialized)
{
    operation instance;

    BOOST_REQUIRE(!instance.is_valid());
    BOOST_REQUIRE(instance.data().empty());
    BOOST_REQUIRE(instance.code() == opcode::disabled_xor);
}

BOOST_AUTO_TEST_CASE(operation__constructor_2__valid_input__returns_input_initialized)
{
    const auto data = base16_chunk("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    auto dup_data = data;
    operation instance(std::move(dup_data));

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(instance.code() == opcode::push_size_32);
    BOOST_REQUIRE(instance.data() == data);
}

BOOST_AUTO_TEST_CASE(operation__constructor_3__valid_input__returns_input_initialized)
{
    const auto data = base16_chunk("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    operation instance(data);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(instance.code() == opcode::push_size_32);
    BOOST_REQUIRE(instance.data() == data);
}

BOOST_AUTO_TEST_CASE(operation__constructor_4__valid_input__returns_input_initialized)
{
    const operation expected(base16_chunk("23156214"));
    operation instance(expected);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(expected == instance);
}

BOOST_AUTO_TEST_CASE(operation__constructor_5__valid_input__returns_input_initialized)
{
    operation expected(base16_chunk("23156214"));
    operation instance(std::move(expected));

    BOOST_REQUIRE(instance.is_valid());
}

BOOST_AUTO_TEST_CASE(operation__from_data__insufficient_bytes__failure)
{
    const data_chunk data;
    operation instance;

    BOOST_REQUIRE(!instance.from_data(data));
    BOOST_REQUIRE(!instance.is_valid());
}

BOOST_AUTO_TEST_CASE(operation__from_data__roundtrip_push_size_0__success)
{
    const auto data0 = base16_chunk("");
    const auto raw_operation = base16_chunk("00");
    operation instance;

    BOOST_REQUIRE(instance.from_data(raw_operation));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(raw_operation == instance.to_data());

    operation duplicate;
    BOOST_REQUIRE(duplicate.from_data(instance.to_data()));
    BOOST_REQUIRE(instance == duplicate);

    BOOST_REQUIRE(instance.code() == opcode::push_size_0);
    BOOST_REQUIRE(instance.data() == data0);
}

BOOST_AUTO_TEST_CASE(operation__from_data__roundtrip_push_size_75__success)
{
    const auto data75 = data_chunk(75, '.');
    const auto raw_operation = splice(base16_literal("4b"), data75);
    operation instance;

    BOOST_REQUIRE(instance.from_data(raw_operation));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(raw_operation == instance.to_data());

    operation duplicate;
    BOOST_REQUIRE(duplicate.from_data(instance.to_data()));
    BOOST_REQUIRE(instance == duplicate);

    BOOST_REQUIRE(instance.code() == opcode::push_size_75);
    BOOST_REQUIRE(instance.data() == data75);
}

BOOST_AUTO_TEST_CASE(operation__from_data__roundtrip_push_negative_1__success)
{
    static const auto op_79 = static_cast<uint8_t>(opcode::push_negative_1);
    const auto data1 = data_chunk{ op_79 };
    const auto raw_operation = data1;
    operation instance;

    // This is read as an encoded operation, not as data.
    // Constructors read (unencoded) data and can select minimal encoding.
    BOOST_REQUIRE(instance.from_data(raw_operation));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(raw_operation == instance.to_data());

    operation duplicate;
    BOOST_REQUIRE(duplicate.from_data(instance.to_data()));
    BOOST_REQUIRE(instance == duplicate);

    // The code is the data for numeric push codes.
    BOOST_REQUIRE(instance.code() == opcode::push_negative_1);
    BOOST_REQUIRE(instance.data() == data_chunk{});
}

BOOST_AUTO_TEST_CASE(operation__from_data__roundtrip_push_positive_1__success)
{
    static const auto op_81 = static_cast<uint8_t>(opcode::push_positive_1);
    const auto data1 = data_chunk{ op_81 };
    const auto raw_operation = data1;
    operation instance;

    // This is read as an encoded operation, not as data.
    // Constructors read (unencoded) data and can select minimal encoding.
    BOOST_REQUIRE(instance.from_data(raw_operation));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(raw_operation == instance.to_data());

    operation duplicate;
    BOOST_REQUIRE(duplicate.from_data(instance.to_data()));
    BOOST_REQUIRE(instance == duplicate);

    // The code is the data for numeric push codes.
    BOOST_REQUIRE(instance.code() == opcode::push_positive_1);
    BOOST_REQUIRE(instance.data() == data_chunk{});
}

BOOST_AUTO_TEST_CASE(operation__from_data__roundtrip_push_positive_16__success)
{
    static const auto op_96 = static_cast<uint8_t>(opcode::push_positive_16);
    const auto data1 = data_chunk{ op_96 };
    const auto raw_operation = data1;
    operation instance;

    // This is read as an encoded operation, not as data.
    // Constructors read (unencoded) data and can select minimal encoding.
    BOOST_REQUIRE(instance.from_data(raw_operation));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(raw_operation == instance.to_data());

    operation duplicate;
    BOOST_REQUIRE(duplicate.from_data(instance.to_data()));
    BOOST_REQUIRE(instance == duplicate);

    // The code is the data for numeric push codes.
    BOOST_REQUIRE(instance.code() == opcode::push_positive_16);
    BOOST_REQUIRE(instance.data() == data_chunk{});
}

BOOST_AUTO_TEST_CASE(operation__from_data__roundtrip_push_one_size__success)
{
    const auto data255 = data_chunk(255, '.');
    const auto raw_operation = splice(base16_literal("4c" "ff"), data255);
    operation instance;

    BOOST_REQUIRE(instance.from_data(raw_operation));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(raw_operation == instance.to_data());

    operation duplicate;
    BOOST_REQUIRE(duplicate.from_data(instance.to_data()));
    BOOST_REQUIRE(instance == duplicate);

    BOOST_REQUIRE(instance.code() == opcode::push_one_size);
    BOOST_REQUIRE(instance.data() == data255);
}

BOOST_AUTO_TEST_CASE(operation__from_data__roundtrip_push_two_size__success)
{
    const auto data520 = data_chunk(520, '.');
    const auto raw_operation = splice(base16_literal("4d" "0802"), data520);
    operation instance;

    BOOST_REQUIRE(instance.from_data(raw_operation));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(raw_operation == instance.to_data());

    operation duplicate;
    BOOST_REQUIRE(duplicate.from_data(instance.to_data()));
    BOOST_REQUIRE(instance == duplicate);

    BOOST_REQUIRE(instance.code() == opcode::push_two_size);
    BOOST_REQUIRE(instance.data() == data520);
}

BOOST_AUTO_TEST_CASE(operation__from_data__roundtrip_push_four_size__success)
{
    const auto data520 = data_chunk(520, '.');
    const auto raw_operation = splice(base16_literal("4e" "08020000"), data520);
    operation instance;

    BOOST_REQUIRE(instance.from_data(raw_operation));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(raw_operation == instance.to_data());

    operation duplicate;
    BOOST_REQUIRE(duplicate.from_data(instance.to_data()));
    BOOST_REQUIRE(instance == duplicate);

    BOOST_REQUIRE(instance.code() == opcode::push_four_size);
    BOOST_REQUIRE(instance.data() == data520);
}

BOOST_AUTO_TEST_CASE(operation__factory_1__roundtrip__success)
{
    auto operation = operation::factory(valid_raw_operation);

    BOOST_REQUIRE(operation.is_valid());
    data_chunk output = operation.to_data();
    BOOST_REQUIRE(output == valid_raw_operation);
}

BOOST_AUTO_TEST_CASE(operation__factory_2__roundtrip__success)
{
    stream::in::copy istream(valid_raw_operation);
    auto operation = operation::factory(istream);

    BOOST_REQUIRE(operation.is_valid());
    data_chunk output = operation.to_data();
    BOOST_REQUIRE(output == valid_raw_operation);
}

BOOST_AUTO_TEST_CASE(operation__factory_3__roundtrip__success)
{
    read::bytes::copy source(valid_raw_operation);
    auto operation = operation::factory(source);

    BOOST_REQUIRE(operation.is_valid());
    data_chunk output = operation.to_data();
    BOOST_REQUIRE(output == valid_raw_operation);
}

BOOST_AUTO_TEST_CASE(operation__operator_assign_equals_1__always__matches_equivalent)
{
    operation expected;
    BOOST_REQUIRE(expected.from_data(valid_raw_operation));
    operation instance;
    operation value;
    BOOST_REQUIRE(value.from_data(valid_raw_operation));
    instance = std::move(value);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(operation__operator_assign_equals_2__always__matches_equivalent)
{
    operation expected;
    BOOST_REQUIRE(expected.from_data(valid_raw_operation));
    operation instance;
    instance = expected;
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(operation__operator_boolean_equals__duplicates__returns_true)
{
    operation alpha;
    operation beta;
    BOOST_REQUIRE(alpha.from_data(valid_raw_operation));
    BOOST_REQUIRE(beta.from_data(valid_raw_operation));
    BOOST_REQUIRE(alpha == beta);
}

BOOST_AUTO_TEST_CASE(operation__operator_boolean_equals__differs__returns_false)
{
    operation alpha;
    operation beta;
    BOOST_REQUIRE(alpha.from_data(valid_raw_operation));
    BOOST_REQUIRE_EQUAL(false, alpha == beta);
}

BOOST_AUTO_TEST_CASE(operation__operator_boolean_not_equals__duplicates__returns_false)
{
    operation alpha;
    operation beta;
    BOOST_REQUIRE(alpha.from_data(valid_raw_operation));
    BOOST_REQUIRE(beta.from_data(valid_raw_operation));
    BOOST_REQUIRE_EQUAL(false, alpha != beta);
}

BOOST_AUTO_TEST_CASE(operation__operator_boolean_not_equals__differs__returns_true)
{
    operation alpha;
    operation beta;
    BOOST_REQUIRE(alpha.from_data(valid_raw_operation));
    BOOST_REQUIRE(alpha != beta);
}

// to_string

BOOST_AUTO_TEST_CASE(operation__to_string__push_size_0__zero)
{
    operation value(opcode::push_size_0);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "zero");
}

BOOST_AUTO_TEST_CASE(operation__to_string__push_size_75__push_75)
{
    // Empty data allows the push code to serialize as an op code.
    operation value(opcode::push_size_75);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "push_75");
}

BOOST_AUTO_TEST_CASE(operation__to_string__push_positive_7__7)
{
    operation value(opcode::push_positive_7);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "7");
}

BOOST_AUTO_TEST_CASE(operation__to_string_minimal__0x07__7)
{
    operation value({ 0x07 }, true);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "7");
}

BOOST_AUTO_TEST_CASE(operation__to_string_nominal__0x07__0x07)
{
    operation value({ 0x07 }, false);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "[07]");
}

BOOST_AUTO_TEST_CASE(operation__to_string__0x42__0x42)
{
    operation value({ 0x42 }, true);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "[42]");
}

BOOST_AUTO_TEST_CASE(operation__to_string__0x112233__0x112233)
{
    operation value({ { 0x11, 0x22, 0x33 } }, true);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "[112233]");
}

BOOST_AUTO_TEST_CASE(operation__to_string__push_size_3__0x112233__0x112233)
{
    static const data_chunk encoded{ { 0x03, 0x11, 0x22, 0x33 } };
    operation value;
    value.from_data(encoded);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "[112233]");
}

BOOST_AUTO_TEST_CASE(operation__to_string__push_one_size_0x112233__1_0x112233)
{
    static const data_chunk encoded{ { 0x4c, 0x03, 0x11, 0x22, 0x33 } };
    operation value;
    value.from_data(encoded);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "[1.112233]");
}

BOOST_AUTO_TEST_CASE(operation__to_string__push_two_size_0x112233__2_0x112233)
{
    static const data_chunk encoded{ { 0x4d, 0x03, 0x00, 0x11, 0x22, 0x33 } };
    operation value;
    value.from_data(encoded);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "[2.112233]");
}

BOOST_AUTO_TEST_CASE(operation__to_string__push_four_size_0x112233__4_0x112233)
{
    static const data_chunk encoded{ { 0x4e, 0x03, 0x00, 0x00, 0x00, 0x11, 0x22, 0x33 } };
    operation value;
    value.from_data(encoded);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "[4.112233]");
}

BOOST_AUTO_TEST_CASE(operation__to_string__nop2_no_rules__nop2)
{
    operation value(opcode::nop2);
    BOOST_REQUIRE_EQUAL(value.to_string(chain::rule_fork::no_rules), "nop2");
}

BOOST_AUTO_TEST_CASE(operation__to_string__nop2_bip65_rule__checklocktimeverify)
{
    operation value(opcode::nop2);
    BOOST_REQUIRE_EQUAL(value.to_string(chain::rule_fork::bip65_rule), "checklocktimeverify");
}

BOOST_AUTO_TEST_CASE(operation__to_string__nop3_no_rules__nop3)
{
    operation value(opcode::nop3);
    BOOST_REQUIRE_EQUAL(value.to_string(chain::rule_fork::no_rules), "nop3");
}

BOOST_AUTO_TEST_CASE(operation__to_string__nop3_bip112_rule__checksequenceverify)
{
    operation value(opcode::nop3);
    BOOST_REQUIRE_EQUAL(value.to_string(chain::rule_fork::bip112_rule), "checksequenceverify");
}

// from_string

BOOST_AUTO_TEST_CASE(operation__from_string__negative_1__push_negative_1_empty)
{
    operation value;
    BOOST_REQUIRE(value.from_string("-1"));
    BOOST_REQUIRE(value.code() == opcode::push_negative_1);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__0__push_size_0_empty)
{
    operation value;
    BOOST_REQUIRE(value.from_string("0"));
    BOOST_REQUIRE(value.code() == opcode::push_size_0);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__1__push_positive_1_empty)
{
    operation value;
    BOOST_REQUIRE(value.from_string("1"));
    BOOST_REQUIRE(value.code() == opcode::push_positive_1);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__16__push_positive_16_empty)
{
    operation value;
    BOOST_REQUIRE(value.from_string("16"));
    BOOST_REQUIRE(value.code() == opcode::push_positive_16);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__17__push_size_1_expected)
{
    static const data_chunk expected{ 0x11 };
    operation value;
    BOOST_REQUIRE(value.from_string("17"));
    BOOST_REQUIRE(value.code() == opcode::push_size_1);
    BOOST_REQUIRE_EQUAL(value.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__from_string__negative_2__push_size_1_expected)
{
    static const data_chunk expected{ 0x82 };
    operation value;
    BOOST_REQUIRE(value.from_string("-2"));
    BOOST_REQUIRE(value.code() == opcode::push_size_1);
    BOOST_REQUIRE_EQUAL(value.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__from_string__9223372036854775807__push_size_8_expected)
{
    static const data_chunk expected{ { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f } };
    operation value;
    BOOST_REQUIRE(value.from_string("9223372036854775807"));
    BOOST_REQUIRE(value.code() == opcode::push_size_8);
    BOOST_REQUIRE_EQUAL(value.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__from_string__negative_9223372036854775807__push_size_8_expected)
{
    static const data_chunk expected{ { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff } };
    operation value;
    BOOST_REQUIRE(value.from_string("-9223372036854775807"));
    BOOST_REQUIRE(value.code() == opcode::push_size_8);
    BOOST_REQUIRE_EQUAL(value.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__from_string__string_empty__push_size_0_empty)
{
    static const data_chunk expected{ 0x61 };
    operation value;
    BOOST_REQUIRE(value.from_string("''"));
    BOOST_REQUIRE(value.code() == opcode::push_size_0);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__string_a__push_size_1_expected_byte)
{
    static const data_chunk expected{ 0x61 };
    operation value;
    BOOST_REQUIRE(value.from_string("'a'"));
    BOOST_REQUIRE(value.code() == opcode::push_size_1);
    BOOST_REQUIRE_EQUAL(value.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__from_string__string_abc__push_size_3_expected_byte_order)
{
    static const data_chunk expected{ { 0x61, 0x62, 0x63 } };
    operation value;
    BOOST_REQUIRE(value.from_string("'abc'"));
    BOOST_REQUIRE(value.code() == opcode::push_size_3);
    BOOST_REQUIRE_EQUAL(value.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__from_string__negative_1_character__push_size_1_nominal_encoding)
{
    static const data_chunk expected{ 0x4f };
    operation value;
    BOOST_REQUIRE(value.from_string("'O'"));
    BOOST_REQUIRE(value.code() == opcode::push_size_1);
    BOOST_REQUIRE_EQUAL(value.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__from_string__push_0__push_size_0)
{
    operation value;
    BOOST_REQUIRE(value.from_string("push_0"));
    BOOST_REQUIRE(value.code() == opcode::push_size_0);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__push_1__false)
{
    operation value;
    BOOST_REQUIRE(!value.from_string("push_1"));
}

BOOST_AUTO_TEST_CASE(operation__from_string__push_75__false)
{
    operation value;
    BOOST_REQUIRE(!value.from_string("push_75"));
}

BOOST_AUTO_TEST_CASE(operation__from_string__push_one__push_one_size_empty)
{
    operation value;
    BOOST_REQUIRE(value.from_string("push_one"));
    BOOST_REQUIRE(value.code() == opcode::push_one_size);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__push_two__push_two_size_empty)
{
    operation value;
    BOOST_REQUIRE(value.from_string("push_two"));
    BOOST_REQUIRE(value.code() == opcode::push_two_size);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__push_four__push_four_size_empty)
{
    operation value;
    BOOST_REQUIRE(value.from_string("push_four"));
    BOOST_REQUIRE(value.code() == opcode::push_four_size);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__7__push_positive_7)
{
    operation value;
    BOOST_REQUIRE(value.from_string("7"));
    BOOST_REQUIRE(value.code() == opcode::push_positive_7);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__0x07__push_size_1)
{
    static const data_chunk expected{ 0x07 };
    operation value;
    BOOST_REQUIRE(value.from_string("[07]"));
    BOOST_REQUIRE(value.code() == opcode::push_size_1);
    BOOST_REQUIRE_EQUAL(value.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__from_string__0x42__push_size_1)
{
    static const data_chunk expected{ 0x42 };
    operation value;
    BOOST_REQUIRE(value.from_string("[42]"));
    BOOST_REQUIRE(value.code() == opcode::push_size_1);
    BOOST_REQUIRE_EQUAL(value.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__from_string__0x112233__push_size_3)
{
    static const data_chunk expected{ { 0x11, 0x22, 0x33 } };
    operation value;
    BOOST_REQUIRE(value.from_string("[112233]"));
    BOOST_REQUIRE(value.code() == opcode::push_size_3);
    BOOST_REQUIRE_EQUAL(value.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__from_string__0_0x112233__push_size_3)
{
    static const data_chunk expected{ { 0x11, 0x22, 0x33 } };
    operation value;
    BOOST_REQUIRE(value.from_string("[0.112233]"));
    BOOST_REQUIRE(value.code() == opcode::push_size_3);
    BOOST_REQUIRE_EQUAL(value.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__from_string__1_0x112233__push_one_size)
{
    static const data_chunk expected{ { 0x11, 0x22, 0x33 } };
    operation value;
    BOOST_REQUIRE(value.from_string("[1.112233]"));
    BOOST_REQUIRE(value.code() == opcode::push_one_size);
    BOOST_REQUIRE_EQUAL(value.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__from_string__2_0x112233__push_two_size)
{
    static const data_chunk expected{ { 0x11, 0x22, 0x33 } };
    operation value;
    BOOST_REQUIRE(value.from_string("[2.112233]"));
    BOOST_REQUIRE(value.code() == opcode::push_two_size);
    BOOST_REQUIRE_EQUAL(value.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__from_string__4_0x112233__push_four_size)
{
    static const data_chunk expected{ { 0x11, 0x22, 0x33 } };
    operation value;
    BOOST_REQUIRE(value.from_string("[4.112233]"));
    BOOST_REQUIRE(value.code() == opcode::push_four_size);
    BOOST_REQUIRE_EQUAL(value.data(), expected);
}

BOOST_AUTO_TEST_CASE(operation__from_string__5_0x112233__false)
{
    operation value;
    BOOST_REQUIRE(!value.from_string("[5.112233]"));
}

BOOST_AUTO_TEST_CASE(operation__from_string__empty_0x112233__false)
{
    operation value;
    BOOST_REQUIRE(!value.from_string("[.112233]"));
}

BOOST_AUTO_TEST_CASE(operation__from_string__nop2__nop2_checklocktimeverify)
{
    operation value;
    BOOST_REQUIRE(value.from_string("nop2"));
    BOOST_REQUIRE(value.code() == opcode::nop2);
    BOOST_REQUIRE(value.code() == opcode::checklocktimeverify);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__checklocktimeverify__nop2_checklocktimeverify)
{
    operation value;
    BOOST_REQUIRE(value.from_string("checklocktimeverify"));
    BOOST_REQUIRE(value.code() == opcode::nop2);
    BOOST_REQUIRE(value.code() == opcode::checklocktimeverify);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__nop3__nop3_checksequenceverify)
{
    operation value;
    BOOST_REQUIRE(value.from_string("nop3"));
    BOOST_REQUIRE(value.code() == opcode::nop3);
    BOOST_REQUIRE(value.code() == opcode::checksequenceverify);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__checklocktimeverify__nop3_checksequenceverify)
{
    operation value;
    BOOST_REQUIRE(value.from_string("checksequenceverify"));
    BOOST_REQUIRE(value.code() == opcode::nop3);
    BOOST_REQUIRE(value.code() == opcode::checksequenceverify);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_SUITE_END()
