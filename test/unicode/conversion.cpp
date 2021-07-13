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

// Use of L and U is not recommended as it will only work for ASCII when
// the source file does not have a BOM (which we avoid for other reasons).
// We use it below to simplify creation of ASCII test vectors.

BOOST_AUTO_TEST_SUITE(conversion_tests)

// to_utf8 (char32_t)

BOOST_AUTO_TEST_CASE(conversion__to_utf8_char32__ascii_space__space)
{
    const char32_t space = 0x00000020;
    BOOST_REQUIRE_EQUAL(to_utf8(space), ascii_space);
}

BOOST_AUTO_TEST_CASE(conversion__to_utf8_char32__ideographic_space__space)
{
    const char32_t space = 0x00003000;
    BOOST_REQUIRE_EQUAL(to_utf8(space), ideographic_space);
}

BOOST_AUTO_TEST_CASE(conversion__to_utf__invalid__empty)
{
    // Cause boost::locale::conv::utf_to_utf<>() to throw, which we suppress.
    const char32_t invalid = 0xffffffff;
    BOOST_REQUIRE(to_utf8(invalid).empty());
}

// to_utf8 (u16string)

BOOST_AUTO_TEST_CASE(conversion__to_utf8_16__empty__empty)
{
    BOOST_REQUIRE(to_utf8(L"").empty());
}

BOOST_AUTO_TEST_CASE(conversion__to_utf8_16__ascii__expected)
{
    const auto utf8_ascii = "ascii";
    const auto utf16_ascii = L"ascii";
    const auto converted = to_utf8(utf16_ascii);
    BOOST_REQUIRE_EQUAL(converted, utf8_ascii);
}

// to_utf8 (wstring_list)

BOOST_AUTO_TEST_CASE(conversion__to_utf8_list_16__empty__empty)
{
    BOOST_REQUIRE(to_utf8(wstring_list{}).empty());
}

BOOST_AUTO_TEST_CASE(conversion__to_utf8_list_16__ascii__expected)
{
    const auto utf8_ascii = "ascii";
    const auto utf32_ascii = L"ascii";
    const string_list expected(2, utf8_ascii);
    BOOST_REQUIRE(to_utf8(wstring_list(2, utf32_ascii)) == expected);
}

// to_utf8 (u32string)

BOOST_AUTO_TEST_CASE(conversion__to_utf8_32__empty__empty)
{
    BOOST_REQUIRE(to_utf8(U"").empty());
}

BOOST_AUTO_TEST_CASE(conversion__to_utf8_32__ascii__expected)
{
    const auto utf8_ascii = "ascii";
    const auto utf32_ascii = U"ascii";
    const auto converted = to_utf8(utf32_ascii);
    BOOST_REQUIRE_EQUAL(converted, utf8_ascii);
}

// to_utf8 (u32string_list)

BOOST_AUTO_TEST_CASE(conversion__to_utf8_list_32__empty__empty)
{
    BOOST_REQUIRE(to_utf8(u32string_list{}).empty());
}

BOOST_AUTO_TEST_CASE(conversion__to_utf8_list_32__ascii__expected)
{
    const auto utf8_ascii = "ascii";
    const auto utf32_ascii = U"ascii";
    const string_list expected(2, utf8_ascii);
    BOOST_REQUIRE(to_utf8(u32string_list(2, utf32_ascii)) == expected);
}

// to_utf16 (string)

BOOST_AUTO_TEST_CASE(conversion__to_utf16_8__empty__empty)
{
    BOOST_REQUIRE(to_utf16("").empty());
}

BOOST_AUTO_TEST_CASE(conversion__to_utf16_8__ascii__expected)
{
    const auto utf8_ascii = "ascii";
    const auto utf16_ascii = L"ascii";
    const auto converted = to_utf16(utf8_ascii);
    BOOST_REQUIRE_EQUAL(converted.c_str(), utf16_ascii);
}

// to_utf16 (string_list)

BOOST_AUTO_TEST_CASE(conversion__to_utf16_list_8__empty__empty)
{
    BOOST_REQUIRE(to_utf16(string_list{}).empty());
}

BOOST_AUTO_TEST_CASE(conversion__to_utf16_list_8__ascii__expected)
{
    const auto utf8_ascii = "ascii";
    const auto utf16_ascii = L"ascii";
    const wstring_list expected(2, utf16_ascii);
    BOOST_REQUIRE(to_utf16(string_list(2, utf8_ascii)) == expected);
}

// to_utf16 (u32string)

BOOST_AUTO_TEST_CASE(conversion__to_utf16_32__empty__empty)
{
    BOOST_REQUIRE(to_utf16(U"").empty());
}

BOOST_AUTO_TEST_CASE(conversion__to_utf16_32__ascii__expected)
{
    const auto utf16_ascii = L"ascii";
    const auto utf32_ascii = U"ascii";
    const auto converted = to_utf16(utf32_ascii);
    BOOST_REQUIRE_EQUAL(converted.c_str(), utf16_ascii);
}

// to_utf16 (u32string_list)

BOOST_AUTO_TEST_CASE(conversion__to_utf16_list_32__empty__empty)
{
    BOOST_REQUIRE(to_utf16(u32string_list{}).empty());
}

BOOST_AUTO_TEST_CASE(conversion__to_utf16_list_32__ascii__expected)
{
    const auto utf32_ascii = U"ascii";
    const auto utf16_ascii = L"ascii";
    const auto converted = to_utf16(u32string_list(2, utf32_ascii));
    const wstring_list expected(2, utf16_ascii);
    BOOST_REQUIRE(to_utf16(u32string_list(2, utf32_ascii)) == expected);
}

// to_utf32 (string)

BOOST_AUTO_TEST_CASE(conversion__to_utf32_8__empty__empty)
{
    BOOST_REQUIRE(to_utf32("").empty());
}

BOOST_AUTO_TEST_CASE(conversion__to_utf32_8__ascii__expected)
{
    const auto utf8_ascii = "ascii";
    const auto utf32_ascii = U"ascii";
    const auto converted = to_utf32(utf8_ascii);
    BOOST_REQUIRE(converted == utf32_ascii);
}

// to_utf32 (string_list)

BOOST_AUTO_TEST_CASE(conversion__to_utf32_list_8__empty__empty)
{
    BOOST_REQUIRE(to_utf32(string_list{}).empty());
}

BOOST_AUTO_TEST_CASE(conversion__to_utf32_list_8__ascii__expected)
{
    const auto utf8_ascii = "ascii";
    const auto utf32_ascii = U"ascii";
    const u32string_list expected(2, utf32_ascii);
    BOOST_REQUIRE(to_utf32(string_list(2, utf8_ascii)) == expected);
}

// to_utf32 (wstring)

BOOST_AUTO_TEST_CASE(conversion__to_utf32_16__empty__empty)
{
    BOOST_REQUIRE(to_utf32("").empty());
}

BOOST_AUTO_TEST_CASE(conversion__to_utf32_16__ascii__expected)
{
    const auto utf16_ascii = L"ascii";
    const auto utf32_ascii = U"ascii";
    const auto converted = to_utf32(utf16_ascii);
    BOOST_REQUIRE(converted == utf32_ascii);
}

// to_utf32 (wstring_list)

BOOST_AUTO_TEST_CASE(conversion__to_utf32_list_16__empty__empty)
{
    BOOST_REQUIRE(to_utf32(string_list{}).empty());
}

BOOST_AUTO_TEST_CASE(conversion__to_utf32_list_16__ascii__expected)
{
    const auto utf16_ascii = L"ascii";
    const auto utf32_ascii = U"ascii";
    const u32string_list expected(2, utf32_ascii);
    BOOST_REQUIRE(to_utf32(wstring_list(2, utf16_ascii)) == expected);
}

// Round trip.

BOOST_AUTO_TEST_CASE(conversion__to_utf16__to_utf8_ascii_round_trip__unchanged)
{
    const auto utf8_ascii = "ascii";
    const auto converted = to_utf8(to_utf16(utf8_ascii));
    BOOST_REQUIRE_EQUAL(converted, utf8_ascii);
}

BOOST_AUTO_TEST_CASE(conversion__to_utf16__to_utf8_japanese_round_trip__unchanged)
{
    const auto utf8 = "テスト";
    const auto converted = to_utf8(to_utf16(utf8));
    BOOST_REQUIRE_EQUAL(converted, utf8);
}

BOOST_AUTO_TEST_CASE(conversion__to_utf16__utf8_and_utf16_japanese_literals_round_trip__fails_on_win32)
{
    const auto utf8 = "テスト";
    const auto utf16 = L"テスト";

    const auto widened = to_utf16(utf8);
    const auto narrowed = to_utf8(utf16);

#ifdef _MSC_VER
    // This confirms that the L prefix does not work with non-ascii text when
    // the source file does not have a BOM (which we avoid for other reasons).
    BOOST_REQUIRE_NE(widened.c_str(), utf16);
    BOOST_REQUIRE_NE(narrowed, utf8);
#else
    BOOST_REQUIRE_EQUAL(widened.c_str(), utf16);
    BOOST_REQUIRE_EQUAL(narrowed, utf8);
#endif
}

BOOST_AUTO_TEST_SUITE_END()
