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
#include <array>
#include <sstream>
#include <string>
#include <vector>

BOOST_AUTO_TEST_SUITE(deserialize_tests)

// <string> is overloaded to avoid trimming (straight pass through).

BOOST_AUTO_TEST_CASE(deserialize__string__empty__unchanged)
{
    std::string out;
    const auto value = "";
    BOOST_REQUIRE(deserialize(out, value));
    BOOST_REQUIRE_EQUAL(out, value);
}

BOOST_AUTO_TEST_CASE(deserialize__string__text__unchanged)
{
    std::string out;
    const auto value = "01234 567890";
    BOOST_REQUIRE(deserialize(out, value));
    BOOST_REQUIRE_EQUAL(out, value);
}

BOOST_AUTO_TEST_CASE(deserialize__string__text_padded__untrimmed)
{
    std::string out;
    const auto value = " \r\t\n\v01234 567890\r\t\n\v ";
    BOOST_REQUIRE(deserialize(out, value));
    BOOST_REQUIRE_EQUAL(out, value);
}

BOOST_AUTO_TEST_CASE(deserialize__string__istream_padded__untrimmed)
{
    std::string out;
    const auto value = " \r\t\n\v01234 567890\r\t\n\v ";
    std::istringstream in{ value };
    BOOST_REQUIRE(deserialize(out, in));
    BOOST_REQUIRE_EQUAL(out, value);
}

// <uint8_t>

// The template specialization must be observed before the template
// or the specialization will be lost and this will return char value.
BOOST_AUTO_TEST_CASE(deserialize__uint8__empty__false)
{
    uint8_t out;
    BOOST_REQUIRE(!deserialize(out, ""));
}

BOOST_AUTO_TEST_CASE(deserialize__uint8__uint8__base10)
{
    uint8_t out;
    BOOST_REQUIRE(deserialize(out, "0"));
    BOOST_REQUIRE_EQUAL(out, 0u);
    BOOST_REQUIRE(deserialize(out, "255"));
    BOOST_REQUIRE_EQUAL(out, 0xff);
}

BOOST_AUTO_TEST_CASE(deserialize__uint8__uchar__base10)
{
    unsigned char out;
    BOOST_REQUIRE(deserialize(out, "0"));
    BOOST_REQUIRE_EQUAL(out, 0u);
    BOOST_REQUIRE(deserialize(out, "255"));
    BOOST_REQUIRE_EQUAL(out, 0xff);
}

BOOST_AUTO_TEST_CASE(deserialize__uint8__istream__base10)
{
    uint8_t out;
    std::istringstream in{ "255" };
    BOOST_REQUIRE(deserialize(out, in));
    BOOST_REQUIRE_EQUAL(out, 0xff);
}

// <data_array<Size>>

BOOST_AUTO_TEST_CASE(deserialize__data_array__empty__empty)
{
    std::array<uint8_t, 0> out;
    const std::array<uint8_t, 0> expected{};
    BOOST_REQUIRE(deserialize(out, ""));
    BOOST_REQUIRE_EQUAL(out, expected);
}

BOOST_AUTO_TEST_CASE(deserialize__data_array__bytes__base16)
{
    std::array<uint8_t, 3> out;
    const std::array<uint8_t, 3> expected{ { 7, 42, 11 } };
    BOOST_REQUIRE(deserialize(out, "072a0b"));
    BOOST_REQUIRE_EQUAL(out, expected);
}

BOOST_AUTO_TEST_CASE(deserialize__data_array__istream__base16)
{
    std::array<uint8_t, 3> out;
    const std::array<uint8_t, 3> expected{ { 7, 42, 11 } };
    std::istringstream in{ "072a0b" };
    BOOST_REQUIRE(deserialize(out, in));
    BOOST_REQUIRE_EQUAL(out, expected);
}

// <data_chunk>

BOOST_AUTO_TEST_CASE(deserialize__data_chunk__empty__empty)
{
    std::vector<uint8_t> out;
    const std::vector<uint8_t> expected{};
    BOOST_REQUIRE(deserialize(out, ""));
    BOOST_REQUIRE_EQUAL(out, expected);
}

BOOST_AUTO_TEST_CASE(deserialize__data_chunk__bytes__base16)
{
    std::vector<uint8_t> out;
    const std::vector<uint8_t> expected{ 7, 42, 11 };
    BOOST_REQUIRE(deserialize(out, "072a0b"));
    BOOST_REQUIRE_EQUAL(out, expected);
}

BOOST_AUTO_TEST_CASE(deserialize__data_chunk__istream__base16)
{
    std::vector<uint8_t> out;
    const std::vector<uint8_t> expected{ 7, 42, 11 };
    std::istringstream in{ "072a0b" };
    BOOST_REQUIRE(deserialize(out, in));
    BOOST_REQUIRE_EQUAL(out, expected);
}

// <array<Value>>

BOOST_AUTO_TEST_CASE(serialize__array__overflow_base10_delimited__false)
{
    std::array<uint16_t, 3> out;
    BOOST_REQUIRE(!deserialize(out, "42 9999999 42"));
}

BOOST_AUTO_TEST_CASE(serialize__array__untrimmed_base10_delimited__true)
{
    std::array<int, 3> out;
    const std::array<int, 3> expected{ { 7, -42, 11 } };
    BOOST_REQUIRE(deserialize(out, " 7  -42  11 "));
    BOOST_REQUIRE_EQUAL(out, expected);
}

BOOST_AUTO_TEST_CASE(serialize__array__int_base10_delimited__ints_true)
{
    std::array<int, 3> out;
    const std::array<int, 3> expected{ { 7, 42, 11 } };
    BOOST_REQUIRE(deserialize(out, "7 42 11"));
    BOOST_REQUIRE_EQUAL(out, expected);
}

BOOST_AUTO_TEST_CASE(serialize__array__int_negative_base10_delimited__ints_true)
{
    std::array<int, 3> out;
    const std::array<int, 3> expected{ { -7, 42, -11 } };
    BOOST_REQUIRE(deserialize(out, "-7 42 -11"));
    BOOST_REQUIRE_EQUAL(out, expected);
}

BOOST_AUTO_TEST_CASE(serialize__array__char_delimited__chars_true)
{
    std::array<char, 3> out;
    const std::array<char, 3> expected{ { 'a', 'b', 'c' } };
    BOOST_REQUIRE(deserialize(out, "a b c"));
    BOOST_REQUIRE_EQUAL(out, expected);
}

BOOST_AUTO_TEST_CASE(serialize__array__string_delimited__tokens_true)
{
    std::array<std::string, 3> out;
    const std::array<std::string, 3> expected{ { "abc", "def", "ghi" } };
    BOOST_REQUIRE(deserialize(out, "abc def ghi"));
    BOOST_REQUIRE_EQUAL(out, expected);
}

// Splitting on whitespace trims string collections before string override.
BOOST_AUTO_TEST_CASE(serialize__array__string_delimited_padded__tokens_true)
{
    std::array<std::string, 3> out;
    const std::array<std::string, 3> expected{ { "abc", "def", "ghi" } };
    BOOST_REQUIRE(deserialize(out, " abc \n def \n ghi "));
    BOOST_REQUIRE_EQUAL(out, expected);
}

BOOST_AUTO_TEST_CASE(deserialize__array__istream__tokens_true)
{
    std::array<std::string, 3> out;
    const std::array<std::string, 3> expected{ { "abc", "def", "ghi" } };
    std::istringstream in{ " abc \n def \n ghi " };
    BOOST_REQUIRE(deserialize(out, in));
    BOOST_REQUIRE_EQUAL(out, expected);
}

// <vector<Value>>

BOOST_AUTO_TEST_CASE(serialize__vector__overflow_base10_delimited__false)
{
    std::vector<uint16_t> out;
    BOOST_REQUIRE(!deserialize(out, "42 9999999 42"));
}

BOOST_AUTO_TEST_CASE(serialize__vector__untrimmed_base10_delimited__true)
{
    std::vector<int> out;
    const std::vector<int> expected{ 7, -42, 11 };
    BOOST_REQUIRE(deserialize(out, " 7  -42  11 "));
    BOOST_REQUIRE_EQUAL(out, expected);
}

BOOST_AUTO_TEST_CASE(serialize__vector__int_base10_delimited__ints_true)
{
    std::vector<int> out;
    const std::vector<int> expected{ 7, 42, 11 };
    BOOST_REQUIRE(deserialize(out, "7 42 11"));
    BOOST_REQUIRE_EQUAL(out, expected);
}

BOOST_AUTO_TEST_CASE(serialize__vector__int_negative_base10_delimited__ints_true)
{
    std::vector<int> out;
    const std::vector<int> expected{ -7, 42, -11 };
    BOOST_REQUIRE(deserialize(out, "-7 42 -11"));
    BOOST_REQUIRE_EQUAL(out, expected);
}

BOOST_AUTO_TEST_CASE(serialize__vector__char_delimited__chars_true)
{
    std::vector<char> out;
    const std::vector<char> expected{ 'a', 'b', 'c' };
    BOOST_REQUIRE(deserialize(out, "a b c"));
    BOOST_REQUIRE_EQUAL(out, expected);
}

BOOST_AUTO_TEST_CASE(serialize__vector__string_delimited__tokens_true)
{
    std::vector<std::string> out;
    const std::vector<std::string> expected{ "abc", "def", "ghi" };
    BOOST_REQUIRE(deserialize(out, "abc def ghi"));
    BOOST_REQUIRE_EQUAL(out, expected);
}

// Splitting on whitespace trims string collections before string override.
BOOST_AUTO_TEST_CASE(serialize__vector__string_delimited_padded__tokens_true)
{
    std::vector<std::string> out;
    const std::vector<std::string> expected{ "abc", "def", "ghi" };
    BOOST_REQUIRE(deserialize(out, " abc \n def \n ghi "));
    BOOST_REQUIRE_EQUAL(out, expected);
}

BOOST_AUTO_TEST_CASE(serialize__vector__istream__tokens_true)
{
    std::vector<std::string> out;
    const std::vector<std::string> expected{ "abc", "def", "ghi" };
    std::istringstream in{ " abc \n def \n ghi " };
    BOOST_REQUIRE(deserialize(out, in));
    BOOST_REQUIRE_EQUAL(out, expected);
}

// <Value>

BOOST_AUTO_TEST_CASE(deserialize__integer__empty__false)
{
    uint16_t out;
    BOOST_REQUIRE(!deserialize(out, ""));
}

BOOST_AUTO_TEST_CASE(deserialize__char__empty__false)
{
    char out;
    BOOST_REQUIRE(!deserialize(out, ""));
}

BOOST_AUTO_TEST_CASE(deserialize__integer__overflow_base10__false)
{
    uint16_t out;
    BOOST_REQUIRE(!deserialize(out, "9999999"));
}

BOOST_AUTO_TEST_CASE(deserialize__integer__invalid_base10__false)
{
    uint16_t out;
    BOOST_REQUIRE(!deserialize(out, "????"));
}

BOOST_AUTO_TEST_CASE(deserialize__int__untrimmed_base10__true)
{
    int out;
    BOOST_REQUIRE(deserialize(out, " 42 "));
    BOOST_REQUIRE_EQUAL(out, 42);
}

BOOST_AUTO_TEST_CASE(deserialize__int__unsigned_base10__true)
{
    int out;
    BOOST_REQUIRE(deserialize(out, "42"));
    BOOST_REQUIRE_EQUAL(out, 42);
}

BOOST_AUTO_TEST_CASE(deserialize__int__negative_base10__true)
{
    int out;
    BOOST_REQUIRE(deserialize(out, "-42"));
    BOOST_REQUIRE_EQUAL(out, -42);
}

BOOST_AUTO_TEST_CASE(deserialize__float__base10__true)
{
    float out;
    BOOST_REQUIRE(deserialize(out, ".42"));
    BOOST_REQUIRE_EQUAL(out, 0.42f);
}

BOOST_AUTO_TEST_CASE(deserialize__double__zero_padded_base10__true)
{
    double out;
    BOOST_REQUIRE(deserialize(out, "00.4200"));
    BOOST_REQUIRE_EQUAL(out, 0.42);
}

BOOST_AUTO_TEST_CASE(deserialize__char__text__char)
{
    char out;
    BOOST_REQUIRE(deserialize(out, "*"));
    BOOST_REQUIRE_EQUAL(out, '*');
}

BOOST_AUTO_TEST_CASE(deserialize__char__numeric_chars__first_char)
{
    char out;
    BOOST_REQUIRE(deserialize(out, "1234567890"));
    BOOST_REQUIRE_EQUAL(out, '1');
}

BOOST_AUTO_TEST_CASE(deserialize__uint16__min_max__true)
{
    uint16_t out;
    BOOST_REQUIRE(deserialize(out, "0"));
    BOOST_REQUIRE_EQUAL(out, 0u);
    BOOST_REQUIRE(deserialize(out, "65535"));
    BOOST_REQUIRE_EQUAL(out, 0xffff);
}

BOOST_AUTO_TEST_CASE(deserialize__uint32__min_max__true)
{
    uint32_t out;
    BOOST_REQUIRE(deserialize(out, "0"));
    BOOST_REQUIRE_EQUAL(out, 0u);
    BOOST_REQUIRE(deserialize(out, "4294967295"));
    BOOST_REQUIRE_EQUAL(out, 0xffffffff);
}

BOOST_AUTO_TEST_CASE(deserialize__uint64__min_max__true)
{
    uint64_t out;
    BOOST_REQUIRE(deserialize(out, "0"));
    BOOST_REQUIRE_EQUAL(out, 0u);
    BOOST_REQUIRE(deserialize(out, "18446744073709551615"));
    BOOST_REQUIRE_EQUAL(out, 0xffffffffffffffff);
}

BOOST_AUTO_TEST_CASE(deserialize__size_t__min_uint32_max__true)
{
    size_t out;
    BOOST_REQUIRE(deserialize(out, "0"));
    BOOST_REQUIRE_EQUAL(out, 0u);
    BOOST_REQUIRE(deserialize(out, "4294967295"));
    BOOST_REQUIRE_EQUAL(out, 0xffffffff);
}

BOOST_AUTO_TEST_CASE(deserialize__size_t__stringstream__true)
{
    size_t out;
    std::stringstream in{ "4294967295" };
    BOOST_REQUIRE(deserialize(out, in));
    BOOST_REQUIRE_EQUAL(out, 0xffffffff);
}

BOOST_AUTO_TEST_SUITE_END()
