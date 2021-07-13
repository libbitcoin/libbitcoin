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
#include <cstdint>
#include <sstream>
#include "../test.hpp"

BOOST_AUTO_TEST_SUITE(endian_tests)

#define ENDIAN_BYTE
#define ENDIAN_NEGATIVE
#define ENDIAN_VARIABLE_SIZE
#define ENDIAN_FIXED_SIZE
#define ENDIAN_STREAM_INTEGER
#define ENDIAN_ARRAY_UINTX
#define ENDIAN_ITERATOR_INTEGER
#define ENDIAN_SYMMETRIC_ROUND_TRIPS
#define ENDIAN_ASYMMETRIC_ROUND_TRIPS

#ifdef ENDIAN_BYTE

// single byte (specialized)

BOOST_AUTO_TEST_CASE(from_big_endian__single_byte__chunk__expected)
{
    const data_chunk data{ 0x42, 0xff };
    BOOST_REQUIRE_EQUAL(from_big_endian<uint8_t>(data), data[0]);
}

BOOST_AUTO_TEST_CASE(from_little_endian__single_byte__chunk___expected)
{
    const data_chunk data{ 0x42, 0xff };
    BOOST_REQUIRE_EQUAL(from_little_endian<uint8_t>(data), data[0]);
}

BOOST_AUTO_TEST_CASE(to_big_endian__single_byte__array__expected)
{
    const data_array<1> data{ { 0x42 } };
    BOOST_REQUIRE_EQUAL(to_big_endian(data[0]), data);
}

BOOST_AUTO_TEST_CASE(to_little_endian__single_byte__array__expected)
{
    const data_array<1> data{ { 0x42 } };
    BOOST_REQUIRE_EQUAL(to_little_endian(data[0]), data);
}

#endif // ENDIAN_BYTE

#ifdef ENDIAN_NEGATIVE

// negatives

BOOST_AUTO_TEST_CASE(to_big_endian__negative__full__expected)
{
    const data_array<mini_hash_size> full_mini_hash{ { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff } };
    BOOST_REQUIRE_EQUAL(to_big_endian<mini_hash_size>(-1), full_mini_hash);
}

BOOST_AUTO_TEST_CASE(to_big_endian__negative__partial__expected)
{
    const data_array<mini_hash_size> half_mini_big{ { 0xff, 0xff, 0xff, 0x00, 0x00, 0x00 } };
    BOOST_REQUIRE_EQUAL(to_big_endian<mini_hash_size>(mask_right(-1, 24)), half_mini_big);
}

BOOST_AUTO_TEST_CASE(to_little_endian__negative__full__expected)
{
    const data_array<mini_hash_size> full_mini_hash{ { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff } };
    BOOST_REQUIRE_EQUAL(to_little_endian<mini_hash_size>(-1), full_mini_hash);
}

BOOST_AUTO_TEST_CASE(to_little_endian__negative__partial__expected)
{
    const data_array<mini_hash_size> half_mini_little{ { 0x00, 0x00, 0x00, 0xff, 0xff, 0xff } };
    BOOST_REQUIRE_EQUAL(to_little_endian<mini_hash_size>(mask_right(-1, 24)), half_mini_little);
}

#endif // ENDIAN_NEGATIVE

#ifdef ENDIAN_VARIABLE_SIZE

constexpr uint32_t value32 = 0x01020304;
const data_chunk data_forward{ 0x01, 0x02, 0x03, 0x04 };
const data_chunk data_reverse{ 0x04, 0x03, 0x02, 0x01 };

// matched data

BOOST_AUTO_TEST_CASE(endian__from_big_endian__chunk_to_integer__expected)
{
    BOOST_REQUIRE(from_big_endian<uintx>(data_forward) == value32);
}

BOOST_AUTO_TEST_CASE(endian__from_little_endian__chunk_to_integer__expected)
{
    BOOST_REQUIRE(from_little_endian<uintx>(data_reverse) == value32);
}

BOOST_AUTO_TEST_CASE(endian__to_big_endian__integer_to_chunk__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian<uintx>(value32), data_forward);
}

BOOST_AUTO_TEST_CASE(endian__to_little_endian__integer_to_chunk__expected)
{
    BOOST_REQUIRE_EQUAL(to_little_endian<uintx>(value32), data_reverse);
}

// short data

BOOST_AUTO_TEST_CASE(endian__from_big_endian__chunk_to_integer_short__expected)
{
    // 4 bytes of data for an 8 byte integer.
    BOOST_REQUIRE_EQUAL(from_big_endian<uint64_t>(data_forward), value32);
}

BOOST_AUTO_TEST_CASE(endian__from_little_endian__chunk_to_integer_short__expected)
{
    // 4 bytes of data for an 8 byte integer.
    BOOST_REQUIRE_EQUAL(from_little_endian<uint64_t>(data_reverse), value32);
}

// long data

BOOST_AUTO_TEST_CASE(endian__from_big_endian__chunk_to_integer_long__expected)
{
    // 4 bytes of data for an 2 byte integer.
    BOOST_REQUIRE_EQUAL(from_big_endian<uint16_t>(data_forward), value32 >> 16);
}

BOOST_AUTO_TEST_CASE(endian__from_little_endian__chunk_to_integer_long__expected)
{
    // 4 bytes of data for an 2 byte integer.
    BOOST_REQUIRE_EQUAL(from_little_endian<uint16_t>(data_reverse), value32 & 0x0000ffff);
}

#endif // ENDIAN_VARIABLE_SIZE

#ifdef ENDIAN_FIXED_SIZE

const data_array<4> array_forward{ { 0x01, 0x02, 0x03, 0x04 } };
const data_array<4> array_reverse{ { 0x04, 0x03, 0x02, 0x01 } };

// matched data

BOOST_AUTO_TEST_CASE(endian__from_big_endian__array_to_integer__expected)
{
    BOOST_REQUIRE_EQUAL(from_big_endian<uint32_t>(array_forward), value32);
}

BOOST_AUTO_TEST_CASE(endian__from_little_endian__array_to_integer__expected)
{
    BOOST_REQUIRE_EQUAL(from_little_endian<uint32_t>(array_reverse), value32);
}

BOOST_AUTO_TEST_CASE(endian__to_big_endian__integer_to_array__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian(value32), array_forward);
}

BOOST_AUTO_TEST_CASE(endian__to_little_endian__integer_to_array__expected)
{
    BOOST_REQUIRE_EQUAL(to_little_endian(value32), array_reverse);
}

// short data

BOOST_AUTO_TEST_CASE(endian__from_big_endian__array_to_integer_short__expected)
{
    // 4 bytes of data for an 8 byte integer.
    BOOST_REQUIRE_EQUAL(from_big_endian<uint64_t>(array_forward), value32);
}

BOOST_AUTO_TEST_CASE(endian__from_little_endian__array_to_integer_short__expected)
{
    // 4 bytes of data for an 8 byte integer.
    BOOST_REQUIRE_EQUAL(from_little_endian<uint64_t>(array_reverse), value32);
}

// long data

BOOST_AUTO_TEST_CASE(endian__from_big_endian__array_to_integer_long__expected)
{
    // 4 bytes of data for an 2 byte integer.
    BOOST_REQUIRE_EQUAL(from_big_endian<uint16_t>(array_forward), value32 >> 16);
}

BOOST_AUTO_TEST_CASE(endian__from_little_endian__array_to_integer_long__expected)
{
    // 4 bytes of data for an 2 byte integer.
    BOOST_REQUIRE_EQUAL(from_little_endian<uint16_t>(array_reverse), value32 & 0x0000ffff);
}

#endif // ENDIAN_FIXED_SIZE

#ifdef ENDIAN_STREAM_INTEGER

const auto alpha_forward = "BITCOIN!";
const auto alpha_reverse = "!NIOCTIB";
const uint64_t value_alpha64 = 0x424954434f494e21;

BOOST_AUTO_TEST_CASE(endian__from_big_endian__stream_to_integer__expected)
{
    std::stringstream stream{ alpha_forward };
    BOOST_REQUIRE_EQUAL(from_big_endian<uint64_t>(stream), value_alpha64);
}

BOOST_AUTO_TEST_CASE(endian__from_little_endian__stream_to_integer__expected)
{
    std::stringstream stream{ alpha_reverse };
    BOOST_REQUIRE_EQUAL(from_little_endian<uint64_t>(stream), value_alpha64);
}

BOOST_AUTO_TEST_CASE(endian__to_big_endian__integer_to_stream__expected)
{
    std::stringstream stream;
    to_big_endian(stream, value_alpha64);
    BOOST_REQUIRE_EQUAL(stream.str(), alpha_forward);
}

BOOST_AUTO_TEST_CASE(endian__to_little_endian__integer_to_stream__expected)
{
    std::stringstream stream;
    to_little_endian(stream, value_alpha64);
    BOOST_REQUIRE_EQUAL(stream.str(), alpha_reverse);
}

#endif // ENDIAN_STREAM_INTEGER

#ifdef ENDIAN_ARRAY_UINTX

BOOST_AUTO_TEST_CASE(endian__from_big_endian__data_to_uintx__expected)
{
    // Populating uintx8
    BOOST_REQUIRE_EQUAL(from_big_endian<1>(data_forward), value32 >> 24);
}

BOOST_AUTO_TEST_CASE(endian__from_little_endian__data_to_uintx__expected)
{
    // Populating uintx800
    BOOST_REQUIRE_EQUAL(from_little_endian<100>(data_reverse), value32);
}

BOOST_AUTO_TEST_CASE(endian__to_big_endian__uintx_to_data__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian<4>(value32), array_forward);
}

BOOST_AUTO_TEST_CASE(endian__to_little_endian__uintx_to_data__expected)
{
    BOOST_REQUIRE_EQUAL(to_little_endian<4>(value32), array_reverse);
}

#endif // ENDIAN_ARRAY_UINTX

#ifdef ENDIAN_SYMMETRIC_ROUND_TRIPS

const mini_hash bytes6 = base16_array(
    "010203040506");
const quarter_hash bytes8 = base16_array(
    "0102030405060708");
const half_hash bytes16 = base16_array(
    "01020304050607080102030405060708");
const short_hash bytes20 = base16_array(
    "0102030405060708010203040506070801020304");
const hash_digest bytes32 = base16_array(
    "0102030405060708010203040506070801020304050607080102030405060708");
const long_hash bytes64 = base16_array(
    "0102030405060708010203040506070801020304050607080102030405060708"
    "0102030405060708010203040506070801020304050607080102030405060708");

// This is not a predefined hash size or uintx_t type.
const data_array<128> bytes128 = base16_array(
    "0102030405060708010203040506070801020304050607080102030405060708"
    "0102030405060708010203040506070801020304050607080102030405060708"
    "0102030405060708010203040506070801020304050607080102030405060708"
    "0102030405060708010203040506070801020304050607080102030405060708");

// big endian

BOOST_AUTO_TEST_CASE(endian__round_trip__uint48_big_endian__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian<6>(from_big_endian<6>(bytes6)), bytes6);
}

// This is uintx64 not uint64_t!
BOOST_AUTO_TEST_CASE(endian__round_trip__uint64_big_endian__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian<8>(from_big_endian<8>(bytes8)), bytes8);
}

// This is uint64_t not uintx64!
BOOST_AUTO_TEST_CASE(endian__round_trip__uint64_t_big_endian__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian(from_big_endian<uint64_t>(bytes8)), bytes8);
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint128_big_endian__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian<16>(from_big_endian<16>(bytes16)), bytes16);
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint160_big_endian__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian<20>(from_big_endian<20>(bytes20)), bytes20);
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint256_big_endian__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian<32>(from_big_endian<32>(bytes32)), bytes32);
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint512_big_endian__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian<64>(from_big_endian<64>(bytes64)), bytes64);
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint1024_big_endian__expected)
{
    // Reading more bytes than provided.
    BOOST_REQUIRE_EQUAL(to_big_endian<128>(from_big_endian<128>(bytes128)), bytes128);
}

// little endian

BOOST_AUTO_TEST_CASE(endian__round_trip__uint48_little_endian__expected)
{
    BOOST_REQUIRE_EQUAL(to_little_endian<6>(from_little_endian<6>(bytes6)), bytes6);
}

// This is uintx64 not uint64_t!
BOOST_AUTO_TEST_CASE(endian__round_trip__uint64_little_endian__expected)
{
    BOOST_REQUIRE_EQUAL(to_little_endian<8>(from_little_endian<8>(bytes8)), bytes8);
}

// This is uint64_t not uintx64!
BOOST_AUTO_TEST_CASE(endian__round_trip__uint64_t_little_endian__expected)
{
    BOOST_REQUIRE_EQUAL(to_little_endian(from_little_endian<uint64_t>(bytes8)), bytes8);
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint128_little_endian__expected)
{
    BOOST_REQUIRE_EQUAL(to_little_endian<16>(from_little_endian<16>(bytes16)), bytes16);
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint160_little_endian__expected)
{
    BOOST_REQUIRE_EQUAL(to_little_endian<20>(from_little_endian<20>(bytes20)), bytes20);
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint256_little_endian__expected)
{
    BOOST_REQUIRE_EQUAL(to_little_endian<32>(from_little_endian<32>(bytes32)), bytes32);
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint512_little_endian__expected)
{
    BOOST_REQUIRE_EQUAL(to_little_endian<64>(from_little_endian<64>(bytes64)), bytes64);
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint1024_little_endian__expected)
{
    // Reading more bytes than provided.
    BOOST_REQUIRE_EQUAL(to_little_endian<128>(from_little_endian<128>(bytes128)), bytes128);
}

#endif // ENDIAN_SYMMETRIC_ROUND_TRIPS

#ifdef ENDIAN_ASYMMETRIC_ROUND_TRIPS

// big endian

BOOST_AUTO_TEST_CASE(endian__round_trip__uint48__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian<6>(from_little_endian<6>(bytes6)), reverse(bytes6));
    BOOST_REQUIRE_EQUAL(to_little_endian<6>(from_big_endian<6>(bytes6)), reverse(bytes6));
}

// This is uintx64 not uint64_t!
BOOST_AUTO_TEST_CASE(endian__round_trip__uint64__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian<8>(from_little_endian<8>(bytes8)), reverse(bytes8));
    BOOST_REQUIRE_EQUAL(to_little_endian<8>(from_big_endian<8>(bytes8)), reverse(bytes8));
}

// This is uint64_t not uintx64!
BOOST_AUTO_TEST_CASE(endian__round_trip__uint64_t__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian(from_little_endian<uint64_t>(bytes8)), reverse(bytes8));
    BOOST_REQUIRE_EQUAL(to_little_endian(from_big_endian<uint64_t>(bytes8)), reverse(bytes8));
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint128__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian<16>(from_little_endian<16>(bytes16)), reverse(bytes16));
    BOOST_REQUIRE_EQUAL(to_little_endian<16>(from_big_endian<16>(bytes16)), reverse(bytes16));
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint160__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian<20>(from_little_endian<20>(bytes20)), reverse(bytes20));
    BOOST_REQUIRE_EQUAL(to_little_endian<20>(from_big_endian<20>(bytes20)), reverse(bytes20));
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint256__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian<32>(from_little_endian<32>(bytes32)), reverse(bytes32));
    BOOST_REQUIRE_EQUAL(to_little_endian<32>(from_big_endian<32>(bytes32)), reverse(bytes32));
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint512__expected)
{
    BOOST_REQUIRE_EQUAL(to_big_endian<64>(from_little_endian<64>(bytes64)), reverse(bytes64));
    BOOST_REQUIRE_EQUAL(to_little_endian<64>(from_big_endian<64>(bytes64)), reverse(bytes64));
}

BOOST_AUTO_TEST_CASE(endian__round_trip__uint1024__expected)
{
    // Reading more bytes than provided.
    BOOST_REQUIRE_EQUAL(to_big_endian<128>(from_little_endian<128>(bytes128)), reverse(bytes128));
    BOOST_REQUIRE_EQUAL(to_little_endian<128>(from_big_endian<128>(bytes128)), reverse(bytes128));
}

#endif // ENDIAN_ASYMMETRIC_ROUND_TRIPS

#ifdef ENDIAN_ITERATOR_INTEGER

BOOST_AUTO_TEST_CASE(endian__from_big_endian_unchecked__iterator_to_integer__expected)
{
    BOOST_REQUIRE_EQUAL(from_big_endian_unchecked<uint32_t>(data_forward.begin()), value32);
}

BOOST_AUTO_TEST_CASE(endian__from_little_endian_unchecked__iterator_to_integer__expected)
{
    BOOST_REQUIRE_EQUAL(from_little_endian_unchecked<uint32_t>(data_reverse.begin()), value32);
}

#endif // ENDIAN_ITERATOR_INTEGER

BOOST_AUTO_TEST_SUITE_END()
