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
#include "../../test.hpp"
#include <string>
#include <sstream>

BOOST_AUTO_TEST_SUITE(utf8_ostream_tests)

// Use of L is not recommended as it will only work for ascii.
BOOST_AUTO_TEST_CASE(unicode_ostream__conditional__test)
{
    std::wstringstream wide_stream;
    std::stringstream narrow_stream;

    unicode_ostream output(narrow_stream, wide_stream, 256);
    output << "ascii";
    output.flush();

#ifdef _MSC_VER
    BOOST_REQUIRE(narrow_stream.str().empty());
    BOOST_REQUIRE_EQUAL(wide_stream.str().c_str(), L"ascii");
#else
    BOOST_REQUIRE(wide_stream.str().empty());
    BOOST_REQUIRE_EQUAL(narrow_stream.str(), "ascii");
#endif
}

BOOST_AUTO_TEST_CASE(unicode_ostream__non_ascii__test)
{
    const auto utf8 = "テスト";
    const auto utf16 = to_utf16(utf8);

    std::wstringstream wide_stream;
    std::stringstream narrow_stream;

    unicode_ostream output(narrow_stream, wide_stream, 256);
    output << utf8;
    output.flush();

#ifdef _MSC_VER
    BOOST_REQUIRE(narrow_stream.str().empty());
    BOOST_REQUIRE_EQUAL(wide_stream.str().c_str(), utf16.c_str());
#else
    BOOST_REQUIRE(wide_stream.str().empty());
    BOOST_REQUIRE_EQUAL(narrow_stream.str(), utf8);
#endif
}

BOOST_AUTO_TEST_CASE(unicode_ostream__overflow__test)
{
    // This is a 20x10 matrix of 3 bytes per character triples (1800 bytes).
    // The buffer is 256 (wide) and 1024 (narrow), resulting in a potential
    // character split because 256 is not a multiple of 3. The overflow
    // method of the output stream buffer must compensate for character
    // splitting as the utf8 stream is not character-oriented, otherwise
    // this will fail.
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

    std::wstringstream wide_stream;
    std::stringstream narrow_stream;

    unicode_ostream output(narrow_stream, wide_stream, 256);
    output << utf8_1800_bytes;
    output.flush();

#ifdef _MSC_VER
    BOOST_REQUIRE(narrow_stream.str().empty());
    BOOST_REQUIRE_EQUAL(wide_stream.str().c_str(), utf16_600_chars.c_str());
#else
    BOOST_REQUIRE(wide_stream.str().empty());
    BOOST_REQUIRE_EQUAL(narrow_stream.str(), utf8_1800_bytes);
#endif
}

BOOST_AUTO_TEST_SUITE_END()
