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
#include "../../test.hpp"
#include <string>
#include <sstream>

BOOST_AUTO_TEST_SUITE(utf8_istream_tests)

// Use of L is not recommended as it will only work for ascii.
BOOST_AUTO_TEST_CASE(unicode_ifstream__conditional__test)
{
    std::wstringstream wide_stream(L"windows ...");
    std::stringstream narrow_stream("linux ...");

    unicode_istream input(narrow_stream, wide_stream, 256);
    std::string result;
    input >> result;

#ifdef _MSC_VER
    BOOST_REQUIRE_EQUAL(result, "windows");
#else
    BOOST_REQUIRE_EQUAL(result, "linux");
#endif
}

BOOST_AUTO_TEST_CASE(unicode_ifstream__non_ascii__test)
{
    const auto utf8 = "テスト";
    const auto utf16 = to_utf16(utf8);

    std::wstringstream wide_stream(utf16);
    std::stringstream narrow_stream(utf8);

    unicode_istream input(narrow_stream, wide_stream, 256);
    std::string result;
    input >> result;

    BOOST_REQUIRE_EQUAL(result, utf8);
}

BOOST_AUTO_TEST_CASE(unicode_ifstream__tokenization__test)
{
    const auto utf8 = "テスト\rス\nト\tテス スト";
    const auto utf16 = to_utf16(utf8);

    std::wstringstream wide_stream(utf16);
    std::stringstream narrow_stream(utf8);

    unicode_istream input(narrow_stream, wide_stream, 256);
    std::string result;

    input >> result;
    BOOST_REQUIRE_EQUAL(result, "テスト");
    input >> result;
    BOOST_REQUIRE_EQUAL(result, "ス");
    input >> result;
    BOOST_REQUIRE_EQUAL(result, "ト");
    input >> result;
    BOOST_REQUIRE_EQUAL(result, "テス");
    input >> result;
    BOOST_REQUIRE_EQUAL(result, "スト");
}

BOOST_AUTO_TEST_CASE(unicode_ifstream__overflow__test)
{
    // This is a 20x10 matrix of 3 bytes per character triples (1800 bytes).
    // The buffer is 256 (wide) and 1024 (narrow), resulting in a potential
    // character split because 256 is not a multiple of 3. However sgetn()
    // doesn't split on non-character boundaries as it reads, so this works.
    const auto utf8_1800_bytes =
        "テストテストテストテストテストテストテストテストテストテスト"
        "テストテストテストテストテストテストテストテストテストテスト"
        "テストテストテストテストテストテストテストテストテストテスト"
        "テストテストテストテストテストテストテストテストテストテスト"
        "テストテストテストテストテストテストテストテストテストテスト"
        "テストテストテストテストテストテストテストテストテストテスト"
        "テストテストテストテストテストテストテストテストテストテスト"
        "テストテストテストテストテストテストテストテストテストテスト"
        "テストテストテストテストテストテストテストテストテストテスト"
        "テストテストテストテストテストテストテストテストテストテスト"
        "テストテストテストテストテストテストテストテストテストテスト"
        "テストテストテストテストテストテストテストテストテストテスト"
        "テストテストテストテストテストテストテストテストテストテスト"
        "テストテストテストテストテストテストテストテストテストテスト"
        "テストテストテストテストテストテストテストテストテストテスト"
        "テストテストテストテストテストテストテストテストテストテスト"
        "テストテストテストテストテストテストテストテストテストテスト"
        "テストテストテストテストテストテストテストテストテストテスト"
        "テストテストテストテストテストテストテストテストテストテスト"
        "テストテストテストテストテストテストテストテストテストテスト";

    const auto utf16_600_chars = to_utf16(utf8_1800_bytes);

    std::wstringstream wide_stream(utf16_600_chars);
    std::stringstream narrow_stream(utf8_1800_bytes);

    unicode_istream input(narrow_stream, wide_stream, 256);
    std::string result;
    input >> result;

    BOOST_REQUIRE_EQUAL(result, utf8_1800_bytes);
}

BOOST_AUTO_TEST_SUITE_END()
