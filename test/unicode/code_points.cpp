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

BOOST_AUTO_TEST_SUITE(code_points_tests)

BOOST_AUTO_TEST_CASE(code_points__ascii_space__utf32__expected)
{
    BOOST_REQUIRE_EQUAL(ascii_space.size(), 1u);
    BOOST_REQUIRE_EQUAL(to_utf32(ascii_space)[0], 0x00000020);
}

BOOST_AUTO_TEST_CASE(code_points__ascii_separators__utf32__expected)
{
    BOOST_REQUIRE_EQUAL(ascii_separators.size(), char_separators_count);
    BOOST_REQUIRE_EQUAL(to_utf32(ascii_separators[0])[0], 0x00000020);
}

BOOST_AUTO_TEST_CASE(code_points__ascii_whitespace__utf32__expected)
{
    BOOST_REQUIRE_EQUAL(ascii_whitespace.size(), char_whitespace_count);
    BOOST_REQUIRE_EQUAL(to_utf32(ascii_whitespace[0])[0], 0x00000009);
    BOOST_REQUIRE_EQUAL(to_utf32(ascii_whitespace[1])[0], 0x0000000a);
    BOOST_REQUIRE_EQUAL(to_utf32(ascii_whitespace[2])[0], 0x0000000b);
    BOOST_REQUIRE_EQUAL(to_utf32(ascii_whitespace[3])[0], 0x0000000c);
    BOOST_REQUIRE_EQUAL(to_utf32(ascii_whitespace[4])[0], 0x0000000d);
    BOOST_REQUIRE_EQUAL(to_utf32(ascii_whitespace[5])[0], 0x00000020);
}

BOOST_AUTO_TEST_CASE(code_points__ideographic_space__utf32__expected)
{
    BOOST_REQUIRE_EQUAL(ideographic_space.size(), 3u);
    BOOST_REQUIRE_EQUAL(to_utf32(ideographic_space)[0], 0x00003000);
}

BOOST_AUTO_TEST_CASE(code_points__unicode_separators__utf32__expected)
{
    BOOST_REQUIRE_EQUAL(unicode_separators.size(), char32_separators_count);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_separators[0])[0], 0x00000020);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_separators[1])[0], 0x000000a0);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_separators[2])[0], 0x00001680);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_separators[3])[0], 0x00002000);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_separators[4])[0], 0x00002001);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_separators[5])[0], 0x00002002);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_separators[6])[0], 0x00002003);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_separators[7])[0], 0x00002004);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_separators[8])[0], 0x00002005);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_separators[9])[0], 0x00002006);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_separators[10])[0], 0x00002007);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_separators[11])[0], 0x00002008);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_separators[12])[0], 0x00002009);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_separators[13])[0], 0x0000200a);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_separators[14])[0], 0x0000202f);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_separators[15])[0], 0x0000205f);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_separators[16])[0], 0x00003000);
}

BOOST_AUTO_TEST_CASE(code_points__unicode_whitespace__utf32__expected)
{
    BOOST_REQUIRE_EQUAL(unicode_whitespace.size(), char32_whitespace_count);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_whitespace[0])[0], 0x00000009);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_whitespace[1])[0], 0x0000000a);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_whitespace[2])[0], 0x0000000b);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_whitespace[3])[0], 0x0000000c);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_whitespace[4])[0], 0x0000000d);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_whitespace[5])[0], 0x00000020);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_whitespace[6])[0], 0x000000a0);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_whitespace[7])[0], 0x00001680);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_whitespace[8])[0], 0x00002000);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_whitespace[9])[0], 0x00002001);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_whitespace[10])[0], 0x00002002);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_whitespace[11])[0], 0x00002003);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_whitespace[12])[0], 0x00002004);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_whitespace[13])[0], 0x00002005);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_whitespace[14])[0], 0x00002006);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_whitespace[15])[0], 0x00002007);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_whitespace[16])[0], 0x00002008);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_whitespace[17])[0], 0x00002009);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_whitespace[18])[0], 0x0000200a);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_whitespace[19])[0], 0x0000202f);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_whitespace[20])[0], 0x0000205f);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_whitespace[21])[0], 0x00003000);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_whitespace[22])[0], 0x00000085);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_whitespace[23])[0], 0x00002028);
    BOOST_REQUIRE_EQUAL(to_utf32(unicode_whitespace[24])[0], 0x00002029);
}

BOOST_AUTO_TEST_SUITE_END()
