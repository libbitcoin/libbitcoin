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

BOOST_AUTO_TEST_SUITE(base_85_tests)

#define BASE85_ENCODED "HelloWorld"
#define BASE85_DECODED \
{ \
    0x86, 0x4F, 0xD2, 0x6F, 0xB5, 0x59, 0xF7, 0x5B \
}

#define BASE85_ENCODED_INVALID_CHAR "Test\n"
#define BASE85_ENCODED_INVALID_LENGTH "Hello World"
#define BASE85_DECODED_INVALID \
{ \
    0x86, 0x4F, 0xD2, 0x6F, 0xB5, 0x59, 0xF7, 0x5B, 0x42 \
}

BOOST_AUTO_TEST_CASE(encode_base85_empty_test)
{
    std::string encoded;
    BOOST_REQUIRE(encode_base85(encoded, data_chunk{}));
    BOOST_REQUIRE(encoded.empty());
}

BOOST_AUTO_TEST_CASE(decode_base85_empty_test)
{
    data_chunk result;
    BOOST_REQUIRE(decode_base85(result, ""));
    BOOST_REQUIRE(result.empty());
}

BOOST_AUTO_TEST_CASE(encode_base85_valid_test)
{
    std::string encoded;
    data_chunk decoded(BASE85_DECODED);
    BOOST_REQUIRE(encode_base85(encoded, decoded));
    BOOST_REQUIRE_EQUAL(encoded, BASE85_ENCODED);
}

BOOST_AUTO_TEST_CASE(encode_base85_invalid_test)
{
    std::string encoded;
    data_chunk decoded(BASE85_DECODED_INVALID);
    BOOST_REQUIRE(!encode_base85(encoded, decoded));
    BOOST_REQUIRE(encoded.empty());
}

BOOST_AUTO_TEST_CASE(decode_base85_valid_test)
{
    data_chunk result;
    BOOST_REQUIRE(decode_base85(result, BASE85_ENCODED));
    BOOST_REQUIRE(result == data_chunk(BASE85_DECODED));
}

BOOST_AUTO_TEST_CASE(decode_base85_invalid_char_test)
{
    data_chunk result;
    BOOST_REQUIRE(!decode_base85(result, BASE85_ENCODED_INVALID_CHAR));
    BOOST_REQUIRE(result.empty());
}

BOOST_AUTO_TEST_CASE(decode_base85_invalid_length_test)
{
    data_chunk result;
    BOOST_REQUIRE(!decode_base85(result, BASE85_ENCODED_INVALID_LENGTH));
    BOOST_REQUIRE(result.empty());
}

// The semicolon is not in the Z85 alphabet, and such characters are treated as
// valid but with zero value in the reference implementation.
BOOST_AUTO_TEST_CASE(decode_base85_outside_alphabet_test)
{
    data_chunk result;
    BOOST_REQUIRE(decode_base85(result, ";;;;;"));
    BOOST_REQUIRE(result == data_chunk({ 0, 0, 0, 0 }));
}

BOOST_AUTO_TEST_SUITE_END()
