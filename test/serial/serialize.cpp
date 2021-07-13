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

BOOST_AUTO_TEST_SUITE(serialize_tests)

// <uint8_t>

// The template specialization must be observed before the template
// or the specialization will be lost and this will return char value.
BOOST_AUTO_TEST_CASE(serialize__uint8__uint8__base10)
{
    uint8_t value = 0;
    BOOST_REQUIRE_EQUAL(serialize(value), "0");
    value = 0xff;
    BOOST_REQUIRE_EQUAL(serialize(value), "255");
}

// The uint8_t override picks up unsigned char.
BOOST_AUTO_TEST_CASE(serialize__uint8__uchar__base10)
{
    unsigned char value = 0;
    BOOST_REQUIRE_EQUAL(serialize(value), "0");
    value = 0xff;
    BOOST_REQUIRE_EQUAL(serialize(value), "255");
}

BOOST_AUTO_TEST_CASE(serialize__uint8__ostream__base10)
{
    const uint8_t value = 0xff;
    std::ostringstream out;
    serialize(out, value, "");
    BOOST_REQUIRE_EQUAL(out.str(), "255");
}

// <data_array<Size>>

BOOST_AUTO_TEST_CASE(serialize__data_array__empty__empty)
{
    const std::array<uint8_t, 0> value{};
    BOOST_REQUIRE_EQUAL(serialize(value), "");
}

BOOST_AUTO_TEST_CASE(serialize__data_array__bytes__base16)
{
    const std::array<uint8_t, 3> value{ { 7, 42, 11 } };
    BOOST_REQUIRE_EQUAL(serialize(value), "072a0b");
}

BOOST_AUTO_TEST_CASE(serialize__data_array__ostream__base16)
{
    const std::array<uint8_t, 3> value{ { 7, 42, 11 } };
    std::ostringstream out;
    serialize(out, value, "");
    BOOST_REQUIRE_EQUAL(out.str(), "072a0b");
}

// <data_chunk>

BOOST_AUTO_TEST_CASE(serialize__data_chunk__empty__empty)
{
    const std::vector<uint8_t> value{};
    BOOST_REQUIRE_EQUAL(serialize(value), "");
}

BOOST_AUTO_TEST_CASE(serialize__data_chunk__bytes__base16)
{
    const std::vector<uint8_t> value{ 7, 42, 11 };
    BOOST_REQUIRE_EQUAL(serialize(value), "072a0b");
}

BOOST_AUTO_TEST_CASE(serialize__data_chunk__ostream__base16)
{
    const std::vector<uint8_t> value{ 7, 42, 11 };
    std::ostringstream out;
    serialize(out, value, "");
    BOOST_REQUIRE_EQUAL(out.str(), "072a0b");
}

// <array<Value>>

BOOST_AUTO_TEST_CASE(serialize__array__int__base10_delimited)
{
    const std::array<int, 3> value{ { 7, 42, 11 } };
    BOOST_REQUIRE_EQUAL(serialize(value), "7 42 11");
}

BOOST_AUTO_TEST_CASE(serialize__array__int_negative__base10_delimited)
{
    const std::array<int, 3> value{ { -7, 42, -11 } };
    BOOST_REQUIRE_EQUAL(serialize(value), "-7 42 -11");
}

BOOST_AUTO_TEST_CASE(serialize__array__char__char_delimited)
{
    const std::array<char, 3> value{ { 'a', 'b', 'c' } };
    BOOST_REQUIRE_EQUAL(serialize(value), "a b c");
}

BOOST_AUTO_TEST_CASE(string__array__string_class__text_delimited)
{
    const std::array<std::string, 3> value{ { "foo", "---", "bar" } };
    BOOST_REQUIRE_EQUAL(serialize(value), "foo --- bar");
}

BOOST_AUTO_TEST_CASE(string__array__string_padded__text_delimited_untrimmed)
{
    const std::array<const char*, 3> value{ { " foo", " --- ", "bar " } };
    BOOST_REQUIRE_EQUAL(serialize(value), " foo  ---  bar ");
}

BOOST_AUTO_TEST_CASE(string__array__string_empty__text_delimited_default_fallback)
{
    const std::array<const char*, 3> value{ { "foo", "", "bar" } };
    BOOST_REQUIRE_EQUAL(serialize(value), "foo ? bar");
}

BOOST_AUTO_TEST_CASE(string__array__string_fallback__text_delimited_specified_fallback)
{
    const std::array<const char*, 3> value{ { "foo", "", "bar" } };
    BOOST_REQUIRE_EQUAL(serialize(value, "*"), "foo * bar");
}

BOOST_AUTO_TEST_CASE(string__array__istream__text_delimited_specified_fallback)
{
    const std::array<const char*, 3> value{ { "foo", "", "bar" } };
    std::ostringstream out;
    serialize(out, value, "*");
    BOOST_REQUIRE_EQUAL(out.str(), "foo * bar");
}

// <vector<Value>>

BOOST_AUTO_TEST_CASE(serialize__vector__int__base10_delimited)
{
    const std::vector<int> value{ 7, 42, 11 };
    BOOST_REQUIRE_EQUAL(serialize(value), "7 42 11");
}

BOOST_AUTO_TEST_CASE(serialize__vector__int_negative__base10_delimited)
{
    const std::vector<int> value{ -7, 42, -11 };
    BOOST_REQUIRE_EQUAL(serialize(value), "-7 42 -11");
}

BOOST_AUTO_TEST_CASE(serialize__vector__char__char_delimited)
{
    const std::vector<char> value{ 'a', 'b', 'c' };
    BOOST_REQUIRE_EQUAL(serialize(value), "a b c");
}

BOOST_AUTO_TEST_CASE(string__vector__string_class__text_delimited)
{
    const std::vector<std::string> value{ "foo", "---", "bar" };
    BOOST_REQUIRE_EQUAL(serialize(value), "foo --- bar");
}

BOOST_AUTO_TEST_CASE(string__vector__string_padded__text_delimited_untrimmed)
{
    const std::vector<const char*> value{ " foo", " --- ", "bar " };
    BOOST_REQUIRE_EQUAL(serialize(value), " foo  ---  bar ");
}

BOOST_AUTO_TEST_CASE(string__vector__string_empty__text_delimited_default_fallback)
{
    const std::vector<const char*> value{ "foo", "", "bar" };
    BOOST_REQUIRE_EQUAL(serialize(value), "foo ? bar");
}

BOOST_AUTO_TEST_CASE(string__vector__string_fallback__text_delimited_specified_fallback)
{
    const std::vector<const char*> value{ "foo", "", "bar" };
    BOOST_REQUIRE_EQUAL(serialize(value, "*"), "foo * bar");
}

BOOST_AUTO_TEST_CASE(string__vector__istream__text_delimited_specified_fallback)
{
    const std::vector<const char*> value{ "foo", "", "bar" };
    std::ostringstream out;
    serialize(out, value, "*");
    BOOST_REQUIRE_EQUAL(out.str(), "foo * bar");
}

// <Value>

BOOST_AUTO_TEST_CASE(serialize__value__int__base10)
{
    const int value = 42;
    BOOST_REQUIRE_EQUAL(serialize(value), "42");
}

BOOST_AUTO_TEST_CASE(serialize__value__int_negative__base10)
{
    const int value = -42;
    BOOST_REQUIRE_EQUAL(serialize(value), "-42");
}

BOOST_AUTO_TEST_CASE(serialize__value__float__base10)
{
    const float value = 42.42f;
    BOOST_REQUIRE_EQUAL(serialize(value), "42.42");
}

BOOST_AUTO_TEST_CASE(serialize__value__float_negative__base10)
{
    const float value = -42.42f;
    BOOST_REQUIRE_EQUAL(serialize(value), "-42.42");
}

BOOST_AUTO_TEST_CASE(serialize__value__double__base10)
{
    const double value = 42.42;
    BOOST_REQUIRE_EQUAL(serialize(value), "42.42");
}

BOOST_AUTO_TEST_CASE(serialize__value__double_negative__base10)
{
    const double value = -42.42;
    BOOST_REQUIRE_EQUAL(serialize(value), "-42.42");
}

BOOST_AUTO_TEST_CASE(serialize__value__double_zero_padded__trimmed_base10)
{
    const double value = -0042.4200;
    BOOST_REQUIRE_EQUAL(serialize(value), "-42.42");
}

BOOST_AUTO_TEST_CASE(serialize__value__char__char)
{
    const char value = 42;
    BOOST_REQUIRE_EQUAL(serialize(value), "*");
}

BOOST_AUTO_TEST_CASE(serialize__value__uint16__base10)
{
    uint16_t value = 0;
    BOOST_REQUIRE_EQUAL(serialize(value), "0");
    value = 0xffff;
    BOOST_REQUIRE_EQUAL(serialize(value), "65535");
}

BOOST_AUTO_TEST_CASE(serialize__value__uint32__base10)
{
    uint32_t value = 0;
    BOOST_REQUIRE_EQUAL(serialize(value), "0");
    value = 0xffffffff;
    BOOST_REQUIRE_EQUAL(serialize(value), "4294967295");
}

BOOST_AUTO_TEST_CASE(serialize__value__uint64__base10)
{
    uint64_t value = 0;
    BOOST_REQUIRE_EQUAL(serialize(value), "0");
    value = 0xffffffffffffffff;
    BOOST_REQUIRE_EQUAL(serialize(value), "18446744073709551615");
}

BOOST_AUTO_TEST_CASE(serialize__value__size_t__base10)
{
    size_t value = 0;
    BOOST_REQUIRE_EQUAL(serialize(value), "0");
    value = 42;
    BOOST_REQUIRE_EQUAL(serialize(value), "42");
}

BOOST_AUTO_TEST_CASE(serialize__value__ostream__base10)
{
    const size_t value = 42;
    std::ostringstream out;
    serialize(out, value, "");
    BOOST_REQUIRE_EQUAL(out.str(), "42");
}

// <string> is overloaded in the deserializer but not in the serializer.

BOOST_AUTO_TEST_CASE(serialize__value__string_class__text)
{
    const std::string value = "foo bar";
    BOOST_REQUIRE_EQUAL(serialize(value), value);
}

BOOST_AUTO_TEST_CASE(serialize__value__text__text)
{
    const auto value = "foo bar";
    BOOST_REQUIRE_EQUAL(serialize(value), value);
}

BOOST_AUTO_TEST_CASE(serialize__value__text_padded__text_untrimmed)
{
    const auto value = "  /n/r/t/vfoo bar/n/r/t/v  ";
    BOOST_REQUIRE_EQUAL(serialize(value), value);
}

BOOST_AUTO_TEST_CASE(serialize__value__text_empty__default_fallback)
{
    BOOST_REQUIRE_EQUAL(serialize(""), "");
}

BOOST_AUTO_TEST_CASE(serialize__value__text_fallback__specified_fallback)
{
    const auto fallback = "***";
    BOOST_REQUIRE_EQUAL(serialize("", fallback), fallback);
}

BOOST_AUTO_TEST_CASE(serialize__value__stringstream__specified_fallback)
{
    std::stringstream out;
    const auto value = "foo bar";
    serialize(out, value, "");
    BOOST_REQUIRE_EQUAL(out.str(), value);
}

BOOST_AUTO_TEST_SUITE_END()
