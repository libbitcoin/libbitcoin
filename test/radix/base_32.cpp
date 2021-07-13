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

BOOST_AUTO_TEST_SUITE(base_32_tests)

// decode_base32

BOOST_AUTO_TEST_CASE(base_32__decode_base32__vector0__expected)
{
    const auto encoded = "";
    const auto decoded = base16_chunk("");
    data_chunk out;
    BOOST_REQUIRE(decode_base32(out, encoded));
    BOOST_REQUIRE_EQUAL(out, decoded);
}

BOOST_AUTO_TEST_CASE(base_32__decode_base32__vector1__expected)
{
    const auto encoded = "pu";
    const auto decoded = base16_chunk("0f");
    data_chunk out;
    BOOST_REQUIRE(decode_base32(out, encoded));
    BOOST_REQUIRE_EQUAL(out, decoded);
}

BOOST_AUTO_TEST_CASE(base_32__decode_base32__vector2__expected)
{
    const auto encoded = "qyhs";
    const auto decoded = base16_chunk("012f");
    data_chunk out;
    BOOST_REQUIRE(decode_base32(out, encoded));
    BOOST_REQUIRE_EQUAL(out, decoded);
}

BOOST_AUTO_TEST_CASE(base_32__decode_base32__vector3__expected)
{
    const auto encoded = "qy357";
    const auto decoded = base16_chunk("01234f");
    data_chunk out;
    BOOST_REQUIRE(decode_base32(out, encoded));
    BOOST_REQUIRE_EQUAL(out, decoded);
}

BOOST_AUTO_TEST_CASE(base_32__decode_base32__vector4__expected)
{
    const auto encoded = "qy352mc";
    const auto decoded = base16_chunk("0123456f");
    data_chunk out;
    BOOST_REQUIRE(decode_base32(out, encoded));
    BOOST_REQUIRE_EQUAL(out, decoded);
}

// Minimum unpadded (non-zero) size.
BOOST_AUTO_TEST_CASE(base_32__decode_base32__vector5__expected)
{
    const auto encoded = "qy352eu0";
    const auto decoded = base16_chunk("012345678f");
    data_chunk out;
    BOOST_REQUIRE(decode_base32(out, encoded));
    BOOST_REQUIRE_EQUAL(out, decoded);
}

// encode_base32

BOOST_AUTO_TEST_CASE(base_32__encode_base32__vector0__expected)
{
    const auto encoded = "";
    const auto decoded = base16_chunk("");
    BOOST_REQUIRE_EQUAL(encode_base32(decoded), encoded);
}

BOOST_AUTO_TEST_CASE(base_32__encode_base32__vector1__expected)
{
    const auto encoded = "pu";
    const auto decoded = base16_chunk("0f");
    BOOST_REQUIRE_EQUAL(encode_base32(decoded), encoded);
}

BOOST_AUTO_TEST_CASE(base_32__encode_base32__vector2__expected)
{
    const auto encoded = "qyhs";
    const auto decoded = base16_chunk("012f");
    BOOST_REQUIRE_EQUAL(encode_base32(decoded), encoded);
}

BOOST_AUTO_TEST_CASE(base_32__encode_base32__vector3__expected)
{
    const auto encoded = "qy357";
    const auto decoded = base16_chunk("01234f");
    BOOST_REQUIRE_EQUAL(encode_base32(decoded), encoded);
}

BOOST_AUTO_TEST_CASE(base_32__encode_base32__vector4__expected)
{
    const auto encoded = "qy352mc";
    const auto decoded = base16_chunk("0123456f");
    BOOST_REQUIRE_EQUAL(encode_base32(decoded), encoded);
}

// Minimum unpadded (non-zero) size.
BOOST_AUTO_TEST_CASE(base_32__encode_base32__vector5__expected)
{
    const auto encoded = "qy352eu0";
    const auto decoded = base16_chunk("012345678f");
    BOOST_REQUIRE_EQUAL(encode_base32(decoded), encoded);
}

// base32_unpack

BOOST_AUTO_TEST_CASE(base_32__base32_unpack__vector0__expected)
{
    // []=>[]
    const data_chunk packed{};
    const base32_chunk expected{};
    BOOST_REQUIRE_EQUAL(base32_unpack(packed), expected);
}

BOOST_AUTO_TEST_CASE(base_32__base32_unpack__vector1__expected)
{
    // [11111111]=>
    // [11111][111pp]
    const data_chunk packed(1, 0xff);
    const base32_chunk expected{ 0x1f, 0x1c };
    BOOST_REQUIRE_EQUAL(base32_unpack(packed), expected);
}

BOOST_AUTO_TEST_CASE(base_32__base32_unpack__vector2__expected)
{
    // [11111111][11111111]=>
    // [11111][11111][11111][1pppp]
    const data_chunk packed(2, 0xff);
    const base32_chunk expected{ 0x1f, 0x1f, 0x1f, 0x10 };
    BOOST_REQUIRE_EQUAL(base32_unpack(packed), expected);
}

BOOST_AUTO_TEST_CASE(base_32__base32_unpack__vector3__expected)
{
    // [11111111][11111111][11111111]=>
    // [11111][11111][11111][11111][1111p]
    const data_chunk packed(3, 0xff);
    const base32_chunk expected{ 0x1f, 0x1f, 0x1f, 0x1f, 0x1e };
    BOOST_REQUIRE_EQUAL(base32_unpack(packed), expected);
}

BOOST_AUTO_TEST_CASE(base_32__base32_unpack__vector4__expected)
{
    // [11111111][11111111][11111111][11111111]=>
    // [11111][11111][11111][11111][11111][11111][11ppp]
    const data_chunk packed(4, 0xff);
    const base32_chunk expected{ 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x18 };
    BOOST_REQUIRE_EQUAL(base32_unpack(packed), expected);
}

// Minimum unpadded (non-zero) size.
BOOST_AUTO_TEST_CASE(base_32__base32_unpack__vector5__expected)
{
    // [11111111][11111111][11111111][11111111][11111111]=>
    // [11111][11111][11111][11111][11111][11111][11111][11111]
    const data_chunk packed(5, 0xff);
    const base32_chunk expected((packed.size() * 8) / 5, 0x1f);
    BOOST_REQUIRE_EQUAL(base32_unpack(packed), expected);
}

// base32_pack

BOOST_AUTO_TEST_CASE(base_32__base32_pack__vector0__expected)
{
    // []=>[]
    const base32_chunk unpacked{};
    const data_chunk expected{};
    data_chunk out;
    BOOST_REQUIRE_EQUAL(base32_pack(unpacked), expected);
}

BOOST_AUTO_TEST_CASE(base_32__base32_pack__vector1__expected)
{
    // [11111][111xx]=>
    // [11111111][xxpppppp]
    const base32_chunk unpacked{ 0x1f, 0x1c };
    const data_chunk expected(1, 0xff);
    BOOST_REQUIRE_EQUAL(base32_pack(unpacked), expected);
}

BOOST_AUTO_TEST_CASE(base_32__base32_pack__vector2__expected)
{
    // [11111][11111][11111][1xxxx]=>
    // [11111111][11111111][xxxxpppp]
    const base32_chunk unpacked{ 0x1f, 0x1f, 0x1f, 0x10 };
    const data_chunk expected(2, 0xff);
    BOOST_REQUIRE_EQUAL(base32_pack(unpacked), expected);
}

BOOST_AUTO_TEST_CASE(base_32__base32_pack__vector3__expected)
{
    // [11111][11111][11111][11111][1111x]=>
    // [11111111][11111111][11111111][xppppppp]
    const base32_chunk unpacked{ 0x1f, 0x1f, 0x1f, 0x1f, 0x1e };
    const data_chunk expected(3, 0xff);
    BOOST_REQUIRE_EQUAL(base32_pack(unpacked), expected);
}

BOOST_AUTO_TEST_CASE(base_32__base32_pack__vector4__expected)
{
    // [11111][11111][11111][11111][11111][11111][11xxx]=>
    // [11111111][11111111][11111111][11111111][xxxppppp]
    const base32_chunk unpacked{ 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x18 };
    const data_chunk expected(4, 0xff);
    BOOST_REQUIRE_EQUAL(base32_pack(unpacked), expected);
}

// Minimum unpadded (non-zero) size.
BOOST_AUTO_TEST_CASE(base_32__base32_pack__vector5__expected)
{
    // [11111][11111][11111][11111][11111][11111][11111][11111]=>
    // [11111111][11111111][11111111][11111111][11111111]
    const base32_chunk unpacked(8, 0x1f);
    const data_chunk expected((unpacked.size() * 5) / 8, 0xff);
    BOOST_REQUIRE_EQUAL(base32_pack(unpacked), expected);
}

BOOST_AUTO_TEST_SUITE_END()
