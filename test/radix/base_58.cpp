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

BOOST_AUTO_TEST_SUITE(base_58_tests)

void encode_decode_test(const std::string& base16, const std::string& encoded)
{
    data_chunk data, decoded;
    BOOST_REQUIRE(decode_base16(data, base16));
    BOOST_REQUIRE_EQUAL(encode_base58(data), encoded);
    BOOST_REQUIRE(decode_base58(decoded, encoded));
    BOOST_REQUIRE(decoded == data);
}

// is_base58

BOOST_AUTO_TEST_CASE(base58__is_base58__valid__true)
{
    const std::string base58 = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
    for (char character: base58)
    {
        BOOST_REQUIRE(is_base58(character));
    }
}

BOOST_AUTO_TEST_CASE(base58__is_base58__invalid__false)
{
    const std::string non_base58 = "0OIl+- //#";
    for (char character: non_base58)
    {
        BOOST_REQUIRE(!is_base58(character));
    }
}

BOOST_AUTO_TEST_CASE(base58__is_base58__literals__expected)
{
    BOOST_REQUIRE(is_base58("abcdjkk11"));
    BOOST_REQUIRE(!is_base58("abcdjkk011"));
}

// encode_base58 / decode_base58

BOOST_AUTO_TEST_CASE(base58__encode_base58__various_round_trip__expected)
{
    encode_decode_test("", "");
    encode_decode_test("61", "2g");
    encode_decode_test("626262", "a3gV");
    encode_decode_test("636363", "aPEr");
    encode_decode_test("73696d706c792061206c6f6e6720737472696e67", "2cFupjhnEsSn59qHXstmK2ffpLv2");
    encode_decode_test("00eb15231dfceb60925886b67d065299925915aeb172c06647", "1NS17iag9jJgTHD1VXjvLCEnZuQ3rJDE9L");
    encode_decode_test("516b6fcd0f", "ABnLTmg");
    encode_decode_test("bf4f89001e670274dd", "3SEo3LWLoPntC");
    encode_decode_test("572e4794", "3EFU7m");
    encode_decode_test("ecac89cad93923c02321", "EJDM8drfXA6uyA");
    encode_decode_test("10c8511e", "Rt5zm");
    encode_decode_test("00000000000000000000", "1111111111");
}

BOOST_AUTO_TEST_CASE(base58__decode_base58__address_round_trip__expected)
{
    const data_chunk checked
    {
        {
            0x00, 0x5c, 0xc8, 0x7f, 0x4a, 0x3f, 0xdf, 0xe3,
            0xa2, 0x34, 0x6b, 0x69, 0x53, 0x26, 0x7c, 0xa8,
            0x67, 0x28, 0x26, 0x30, 0xd3, 0xf9, 0xb7, 0x8e,
            0x64
        }
    };
    const auto address = "19TbMSWwHvnxAKy12iNm3KdbGfzfaMFViT";
    BOOST_REQUIRE_EQUAL(encode_base58(checked), address);

    data_chunk decoded;
    BOOST_REQUIRE(decode_base58(decoded, address));
    BOOST_REQUIRE_EQUAL(decoded, checked);
}

BOOST_AUTO_TEST_CASE(base58__decode_base58__array__expected)
{
    const data_array<25> expected
    {
        {
            0x00, 0x5c, 0xc8, 0x7f, 0x4a, 0x3f, 0xdf, 0xe3,
            0xa2, 0x34, 0x6b, 0x69, 0x53, 0x26, 0x7c, 0xa8,
            0x67, 0x28, 0x26, 0x30, 0xd3, 0xf9, 0xb7, 0x8e,
            0x64
        }
    };
    data_array<25> converted;
    BOOST_REQUIRE(decode_base58(converted, "19TbMSWwHvnxAKy12iNm3KdbGfzfaMFViT"));
    BOOST_REQUIRE_EQUAL(converted, expected);
}

BOOST_AUTO_TEST_SUITE_END()
