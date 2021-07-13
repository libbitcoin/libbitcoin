/**
 * Copyright (c) 2011-2021 libbitcoin developers (see AUTHORS)
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

BOOST_AUTO_TEST_SUITE(binary_tests)

// is_base2

BOOST_AUTO_TEST_CASE(binary__is_base2__empty__true)
{
    BOOST_REQUIRE(binary::is_base2(""));
}

BOOST_AUTO_TEST_CASE(binary__is_base2__non_empty_valid__true)
{
    BOOST_REQUIRE(binary::is_base2("0"));
    BOOST_REQUIRE(binary::is_base2("1"));
    BOOST_REQUIRE(binary::is_base2("1010101000111010011001"));
}

BOOST_AUTO_TEST_CASE(binary__is_base2__invalid__false)
{
    BOOST_REQUIRE(!binary::is_base2("02"));
    BOOST_REQUIRE(!binary::is_base2("30"));
    BOOST_REQUIRE(!binary::is_base2("04"));
    BOOST_REQUIRE(!binary::is_base2("50"));
    BOOST_REQUIRE(!binary::is_base2("06"));
    BOOST_REQUIRE(!binary::is_base2("70"));
    BOOST_REQUIRE(!binary::is_base2("18"));
    BOOST_REQUIRE(!binary::is_base2("91"));
    BOOST_REQUIRE(!binary::is_base2("1a"));
    BOOST_REQUIRE(!binary::is_base2("Z1"));
}

// construct default

BOOST_AUTO_TEST_CASE(binary__construct__default__zero_empty)
{
    const binary instance;
    BOOST_REQUIRE(instance.encoded().empty());
    BOOST_REQUIRE(instance.data().empty());
    BOOST_REQUIRE_EQUAL(instance.bytes(), 0u);
    BOOST_REQUIRE_EQUAL(instance.bits(), 0u);
}

// construct move

BOOST_AUTO_TEST_CASE(binary__construct__move__expected)
{
    const auto expected = "101";
    binary instance1(expected);
    const binary instance2(std::move(instance1));
    BOOST_REQUIRE_EQUAL(instance2.encoded(), expected);
}

// construct copy

BOOST_AUTO_TEST_CASE(binary__construct__copy__expected)
{
    const auto expected = "101";
    const binary instance1(expected);
    const binary instance2(instance1);
    BOOST_REQUIRE_EQUAL(instance2, instance1);
}

// construct string

BOOST_AUTO_TEST_CASE(binary__construct_string__invalid__zero_empty)
{
    const binary instance("42");
    BOOST_REQUIRE(instance.encoded().empty());
    BOOST_REQUIRE(((const data_chunk&)instance).empty());
    BOOST_REQUIRE(instance.data().empty());
    BOOST_REQUIRE_EQUAL(instance.bytes(), 0u);
    BOOST_REQUIRE_EQUAL(instance.bits(), 0u);
}

BOOST_AUTO_TEST_CASE(binary__construct_string__empty__zero_empty)
{
    const binary instance("");
    BOOST_REQUIRE(instance.encoded().empty());
}

BOOST_AUTO_TEST_CASE(binary__construct_string__0__0)
{
    const auto expected = "0";
    const binary instance(expected);
    BOOST_REQUIRE_EQUAL(instance.encoded(), expected);
}

BOOST_AUTO_TEST_CASE(binary__construct_string__1__1)
{
    const auto expected = "1";
    const binary instance(expected);
    BOOST_REQUIRE_EQUAL(instance.encoded(), expected);
}

BOOST_AUTO_TEST_CASE(binary__construct_string__1010__1010)
{
    const auto expected = "1010";
    const binary instance(expected);
    BOOST_REQUIRE_EQUAL(instance.encoded(), expected);
}

BOOST_AUTO_TEST_CASE(binary__construct_string__0101__0100)
{
    const auto expected = "0101";
    const binary instance(expected);
    BOOST_REQUIRE_EQUAL(instance.encoded(), expected);
}

BOOST_AUTO_TEST_CASE(binary__construct_string__bits__expected)
{
    const auto expected = "10111010101011011111000000001101";
    const binary instance(expected);
    BOOST_REQUIRE_EQUAL(instance.encoded(), expected);
}

// construct data

// This data-text relation from original test cases, ensures endianness consistency.
BOOST_AUTO_TEST_CASE(binary__construct_data__32_bits__expected)
{
    const auto expected = "10111010101011011111000000001101";
    const binary instance(32, { 0xba, 0xad, 0xf0, 0x0d });
    BOOST_REQUIRE_EQUAL(instance.encoded(), expected);
}

BOOST_AUTO_TEST_CASE(binary__construct_data__zero_empty__zero_empty)
{
    const binary instance(0, data_chunk{});
    BOOST_REQUIRE(instance.encoded().empty());
    BOOST_REQUIRE(((const data_chunk&)instance).empty());
    BOOST_REQUIRE(instance.data().empty());
    BOOST_REQUIRE_EQUAL(instance.bytes(), 0u);
    BOOST_REQUIRE_EQUAL(instance.bits(), 0u);
}

BOOST_AUTO_TEST_CASE(binary__construct_data__zero_not_empty__zero_empty)
{
    data_chunk data(42, 0x00);
    const binary instance(0, data);
    BOOST_REQUIRE(instance.encoded().empty());
    BOOST_REQUIRE(((const data_chunk&)instance).empty());
    BOOST_REQUIRE(instance.data().empty());
    BOOST_REQUIRE_EQUAL(instance.bytes(), 0u);
    BOOST_REQUIRE_EQUAL(instance.bits(), 0u);
}

BOOST_AUTO_TEST_CASE(binary__construct_data__matched_size__expected)
{
    const data_chunk data{ 0xba, 0xad, 0xf0, 0xfd };
    const auto bytes = data.size();
    const auto bits = to_bits(bytes);
    const binary instance(bits, data);
    BOOST_REQUIRE_EQUAL(instance.encoded(), "10111010101011011111000011111101");
    BOOST_REQUIRE_EQUAL((const data_chunk&)instance, data);
    BOOST_REQUIRE_EQUAL(instance.data(), data);
    BOOST_REQUIRE_EQUAL(instance.bytes(), bytes);
    BOOST_REQUIRE_EQUAL(instance.bits(), bits);
}

BOOST_AUTO_TEST_CASE(binary__construct_data__lower_size__expected)
{
    const data_chunk data{ 0xba, 0xad, 0xf0, 0xfd };
    const data_chunk expected{ 0xba, 0xad, 0xf0, mask_right<uint8_t>(0xfd, 4) };
    const auto bits = to_bits(data.size()) - 4;
    const binary instance(bits, data);
    BOOST_REQUIRE_EQUAL(instance.encoded(), "1011101010101101111100001111");
    BOOST_REQUIRE_EQUAL((const data_chunk&)instance, expected);
    BOOST_REQUIRE_EQUAL(instance.data(), expected);
    BOOST_REQUIRE_EQUAL(instance.bytes(), expected.size());
    BOOST_REQUIRE_EQUAL(instance.bits(), bits);
}

BOOST_AUTO_TEST_CASE(binary__construct_data__higher_size__expected)
{
    const data_chunk data{ 0xba, 0xad, 0xf0, 0x0d };
    const data_chunk expected{ 0xba, 0xad, 0xf0, 0x0d, 0x00 };
    const auto bits = to_bits(data.size()) + 4;
    const binary instance(bits, data);
    BOOST_REQUIRE_EQUAL(instance.encoded(), "101110101010110111110000000011010000");
    BOOST_REQUIRE_EQUAL((const data_chunk&)instance, expected);
    BOOST_REQUIRE_EQUAL(instance.data(), expected);
    BOOST_REQUIRE_EQUAL(instance.bytes(), expected.size());
    BOOST_REQUIRE_EQUAL(instance.bits(), bits);
}

// assign move

BOOST_AUTO_TEST_CASE(binary__assign__move__expected)
{
    const auto expected = "101";
    binary instance1(expected);
    binary instance2;
    instance2 = std::move(instance1);
    BOOST_REQUIRE_EQUAL(instance2.encoded(), expected);
}

// assign copy

BOOST_AUTO_TEST_CASE(binary__assign__copy__expected)
{
    const auto expected = "101";
    binary instance1(expected);
    binary instance2 = instance1;
    BOOST_REQUIRE_EQUAL(instance2, instance1);
}

// equality

BOOST_AUTO_TEST_CASE(binary__equality__same__true)
{
    BOOST_REQUIRE(binary{} == binary{});
    BOOST_REQUIRE(binary("0") == binary("0"));
    BOOST_REQUIRE(binary("101") == binary("101"));
}

BOOST_AUTO_TEST_CASE(binary__equality__different__false)
{
    BOOST_REQUIRE(!(binary("1") == binary("0")));
    BOOST_REQUIRE(!(binary("010") == binary("10")));
    BOOST_REQUIRE(!(binary("01") == binary("010")));
}

// inequality

BOOST_AUTO_TEST_CASE(binary__inequality__different__true)
{
    BOOST_REQUIRE(binary("1") != binary("0"));
    BOOST_REQUIRE(binary("010") != binary("10"));
    BOOST_REQUIRE(binary("01") != binary("010"));
}

BOOST_AUTO_TEST_CASE(binary__inequality__same__false)
{
    BOOST_REQUIRE(!(binary{} != binary{}));
    BOOST_REQUIRE(!(binary("0") != binary("0")));
    BOOST_REQUIRE(!(binary("101") != binary("101")));
}

// operator<

BOOST_AUTO_TEST_CASE(binary__lesser__same__false)
{
    BOOST_REQUIRE(!(binary{} < binary{}));
    BOOST_REQUIRE(!(binary("0") < binary("0")));
    BOOST_REQUIRE(!(binary("101") < binary("101")));
}

// operator<

BOOST_AUTO_TEST_CASE(binary__lesser__lesser__true)
{
    BOOST_REQUIRE(binary{} < binary("0"));
    BOOST_REQUIRE(binary{} < binary("1"));
    BOOST_REQUIRE(binary{} < binary("00"));
    BOOST_REQUIRE(binary{} < binary("01"));
    BOOST_REQUIRE(binary("0") < binary("1"));
    BOOST_REQUIRE(binary("0") < binary("01"));
    BOOST_REQUIRE(binary("1") < binary("11"));
}

BOOST_AUTO_TEST_CASE(binary__lesser__greater__false)
{
    BOOST_REQUIRE(!(binary("0") < binary{}));
    BOOST_REQUIRE(!(binary("1") < binary{}));
    BOOST_REQUIRE(!(binary("1") < binary("00")));
    BOOST_REQUIRE(!(binary("1") < binary("01")));
    BOOST_REQUIRE(!(binary("01") < binary("000")));
    BOOST_REQUIRE(!(binary("10") < binary("011")));
}

// operator[]

BOOST_AUTO_TEST_CASE(binary__index__overflow__false)
{
    const binary instance{};
    BOOST_REQUIRE_EQUAL(instance[0], false);
    BOOST_REQUIRE_EQUAL(instance[42], false);
}

BOOST_AUTO_TEST_CASE(binary__index__contained__expected)
{
    const binary instance{ "1010111010" };
    BOOST_REQUIRE_EQUAL(instance[0], true);
    BOOST_REQUIRE_EQUAL(instance[1], false);
    BOOST_REQUIRE_EQUAL(instance[2], true);
    BOOST_REQUIRE_EQUAL(instance[3], false);
    BOOST_REQUIRE_EQUAL(instance[4], true);
    BOOST_REQUIRE_EQUAL(instance[5], true);
    BOOST_REQUIRE_EQUAL(instance[6], true);
    BOOST_REQUIRE_EQUAL(instance[7], false);
    BOOST_REQUIRE_EQUAL(instance[8], true);
    BOOST_REQUIRE_EQUAL(instance[9], false);
}

// stream >>

BOOST_AUTO_TEST_CASE(binary__stream__in__expected)
{
    const auto expected = "101";
    std::stringstream in{ expected };
    binary instance;
    in >> instance;
    BOOST_REQUIRE_EQUAL(instance.encoded(), expected);
}

// stream <<

BOOST_AUTO_TEST_CASE(binary__stream__out__expected)
{
    const auto expected = "101";
    std::stringstream out;
    const binary instance{ expected };
    out << instance;
    BOOST_REQUIRE_EQUAL(out.str(), expected);
}

BOOST_AUTO_TEST_SUITE_END()
