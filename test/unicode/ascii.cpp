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

BOOST_AUTO_TEST_SUITE(ascii_tests)

// is_ascii_character

BOOST_AUTO_TEST_CASE(ascii__is_ascii_character__bounds__true)
{
    BOOST_REQUIRE(is_ascii_character(0x00000000));
    BOOST_REQUIRE(is_ascii_character(0x0000007f));
}

BOOST_AUTO_TEST_CASE(ascii__is_ascii_character__out_of_bounds__false)
{
    BOOST_REQUIRE(!is_ascii_character(0x00000080));
    BOOST_REQUIRE(!is_ascii_character(0xffffffff));
}

// is_ascii_separator

BOOST_AUTO_TEST_CASE(ascii__is_ascii_separator__ascii_space__true)
{
    // ASCII separator characters.
    BOOST_REQUIRE(is_ascii_separator(0x00000020));
}

BOOST_AUTO_TEST_CASE(ascii__is_ascii_separator__ideographic_space__false)
{
    BOOST_REQUIRE(!is_ascii_separator(0x00003000));
}

// is_ascii_whitespace

BOOST_AUTO_TEST_CASE(ascii__is_ascii_whitespace__all_ascii_whitespace__true)
{
    // ASCII whitespace characters (C whitespace).
    BOOST_REQUIRE(is_ascii_whitespace(0x00000009));
    BOOST_REQUIRE(is_ascii_whitespace(0x0000000a));
    BOOST_REQUIRE(is_ascii_whitespace(0x0000000b));
    BOOST_REQUIRE(is_ascii_whitespace(0x0000000c));
    BOOST_REQUIRE(is_ascii_whitespace(0x0000000d));
    BOOST_REQUIRE(is_ascii_whitespace(0x00000020));
}

BOOST_AUTO_TEST_CASE(ascii__is_ascii_whitespace__zero__false)
{
    BOOST_REQUIRE(!is_ascii_whitespace(0x00000000));
}

BOOST_AUTO_TEST_CASE(ascii__is_ascii_whitespace__ideographic_space__false)
{
    BOOST_REQUIRE(!is_ascii_whitespace(0x00003000));
}

// is_ascii

BOOST_AUTO_TEST_CASE(ascii__is_ascii__empty__true)
{
    BOOST_REQUIRE(is_ascii(""));
}

BOOST_AUTO_TEST_CASE(ascii__is_ascii__alphanumeric__true)
{
    BOOST_REQUIRE(is_ascii("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"));
}

BOOST_AUTO_TEST_CASE(ascii__is_ascii__below_128__true)
{
    BOOST_REQUIRE(is_ascii(std::string{ '\x00' }));
    BOOST_REQUIRE(is_ascii(std::string{ '\x7f' }));
}

BOOST_AUTO_TEST_CASE(ascii__is_ascii__above_127__false)
{
    BOOST_REQUIRE(!is_ascii(std::string{ '\x80' }));
    BOOST_REQUIRE(!is_ascii(std::string{ '\xff' }));
}

BOOST_AUTO_TEST_CASE(ascii__is_ascii__ideographic_space__false)
{
    BOOST_REQUIRE(!is_ascii(ideographic_space));
}

// ascii_to_lower

BOOST_AUTO_TEST_CASE(ascii__ascii_to_lower__empty__empty)
{
    const std::string value{ "" };
    BOOST_REQUIRE_EQUAL(ascii_to_lower(value), value);
}

BOOST_AUTO_TEST_CASE(ascii__ascii_to_lower__lower__unchanged)
{
    const std::string value{ "abcdefghijklmnopqrstuvwxyz0123456789" };
    BOOST_REQUIRE_EQUAL(ascii_to_lower(value), value);
}

BOOST_AUTO_TEST_CASE(ascii__ascii_to_lower__upper__lowered)
{
    const std::string value{ "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789" };
    const std::string expected{ "abcdefghijklmnopqrstuvwxyz0123456789" };
    BOOST_REQUIRE_EQUAL(ascii_to_lower(value), expected);
}

BOOST_AUTO_TEST_CASE(ascii__ascii_to_lower__mixed__lowered)
{
    const std::string value{ "AbCdEfGhIjKlMnOpQrStUvWxYz0123456789" };
    const std::string expected{ "abcdefghijklmnopqrstuvwxyz0123456789" };
    BOOST_REQUIRE_EQUAL(ascii_to_lower(value), expected);
}

// ascii_to_upper

BOOST_AUTO_TEST_CASE(ascii__ascii_to_upper__empty__empty)
{
    const std::string value{ "" };
    BOOST_REQUIRE_EQUAL(ascii_to_upper(value), value);
}

BOOST_AUTO_TEST_CASE(ascii__ascii_to_upper__lower__raised)
{
    const std::string value{ "abcdefghijklmnopqrstuvwxyz0123456789" };
    const std::string expected{ "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789" };
    BOOST_REQUIRE_EQUAL(ascii_to_upper(value), expected);
}

BOOST_AUTO_TEST_CASE(ascii__ascii_to_upper__upper__unchanged)
{
    const std::string value{ "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789" };
    BOOST_REQUIRE_EQUAL(ascii_to_upper(value), value);
}

BOOST_AUTO_TEST_CASE(ascii__ascii_to_upper__mixed__raised)
{
    const std::string value{ "AbCdEfGhIjKlMnOpQrStUvWxYz0123456789" };
    const std::string expected{ "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789" };
    BOOST_REQUIRE_EQUAL(ascii_to_upper(value), expected);
}

// has_mixed_ascii_case

BOOST_AUTO_TEST_CASE(ascii__has_mixed_ascii_case__empty__false)
{
    BOOST_REQUIRE(!has_mixed_ascii_case(""));
}

BOOST_AUTO_TEST_CASE(ascii__has_mixed_ascii_case__mixed_case_with_non_ascii__true)
{
    BOOST_REQUIRE(has_mixed_ascii_case("\x80xYz"));
}

BOOST_AUTO_TEST_CASE(ascii__has_mixed_ascii_case__lower_ascii__false)
{
    BOOST_REQUIRE(!has_mixed_ascii_case("abcdefghijklmnopqrstuvwxyz0123456789"));
}

BOOST_AUTO_TEST_CASE(ascii__has_mixed_ascii_case__upper_ascii__false)
{
    BOOST_REQUIRE(!has_mixed_ascii_case("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"));
}

BOOST_AUTO_TEST_CASE(ascii__has_mixed_ascii_case__mixed_ascii__true)
{
    BOOST_REQUIRE(has_mixed_ascii_case("AbCdEfGhIjKlMnOpQrStUvWxYz0123456789"));
}

BOOST_AUTO_TEST_CASE(ascii__has_mixed_ascii_case__lower_ascii_with_ideographic_space__false)
{
    const auto unicode = ideographic_space + "abcdefghijklmnopqrstuvwxyz";
    BOOST_REQUIRE(!has_mixed_ascii_case(unicode));
}

BOOST_AUTO_TEST_CASE(ascii__has_mixed_ascii_case__upper_ascii_with_ideographic_space__false)
{
    const auto unicode = ideographic_space + "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    BOOST_REQUIRE(!has_mixed_ascii_case(unicode));
}

BOOST_AUTO_TEST_CASE(ascii__has_mixed_ascii_case__mixed_ascii_with_ideographic_space__true)
{
    const auto unicode = ideographic_space + "AbCdEfGhIjKlMnOpQrStUvWxYz";
    BOOST_REQUIRE(has_mixed_ascii_case(unicode));
}

BOOST_AUTO_TEST_SUITE_END()
