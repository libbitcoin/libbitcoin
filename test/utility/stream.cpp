/*
 * Copyright (c) 2011-2015 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <sstream>
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <bitcoin/bitcoin.hpp>

using namespace bc;

BOOST_AUTO_TEST_SUITE(stream_tests)

BOOST_AUTO_TEST_CASE(roundtrip_byte)
{
    const uint8_t expected = 0xAA;
    std::stringstream stream;
    ostream_writer sink(stream);
    istream_reader source(stream);

    sink.write_byte(expected);
    uint8_t result = source.read_byte();

    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE(stream);
}

BOOST_AUTO_TEST_CASE(roundtrip_2_bytes_little_endian)
{
    const uint16_t expected = 43707;
    std::stringstream stream;
    ostream_writer sink(stream);
    istream_reader source(stream);

    sink.write_2_bytes_little_endian(expected);
    uint16_t result = source.read_2_bytes_little_endian();

    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE(stream);
}

BOOST_AUTO_TEST_CASE(roundtrip_4_bytes_little_endian)
{
    const uint32_t expected = 2898120443;
    std::stringstream stream;
    ostream_writer sink(stream);
    istream_reader source(stream);

    sink.write_4_bytes_little_endian(expected);
    uint32_t result = source.read_4_bytes_little_endian();

    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE(stream);
}

BOOST_AUTO_TEST_CASE(roundtrip_8_bytes_little_endian)
{
    const uint64_t expected = 0xd4b14be5d8f02abe;
    std::stringstream stream;
    ostream_writer sink(stream);
    istream_reader source(stream);

    sink.write_8_bytes_little_endian(expected);
    uint64_t result = source.read_8_bytes_little_endian();

    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE(stream);
}

BOOST_AUTO_TEST_CASE(roundtrip_2_bytes_big_endian)
{
    const uint16_t expected = 43707;
    std::stringstream stream;
    ostream_writer sink(stream);
    istream_reader source(stream);

    sink.write_2_bytes_big_endian(expected);
    uint16_t result = source.read_2_bytes_big_endian();

    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE(stream);
}

BOOST_AUTO_TEST_CASE(roundtrip_4_bytes_big_endian)
{
    const uint32_t expected = 2898120443;
    std::stringstream stream;
    ostream_writer sink(stream);
    istream_reader source(stream);

    sink.write_4_bytes_big_endian(expected);
    uint32_t result = source.read_4_bytes_big_endian();

    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE(stream);
}

BOOST_AUTO_TEST_CASE(roundtrip_8_bytes_big_endian)
{
    const uint64_t expected = 0xd4b14be5d8f02abe;
    std::stringstream stream;
    ostream_writer sink(stream);
    istream_reader source(stream);

    sink.write_8_bytes_big_endian(expected);
    uint64_t result = source.read_8_bytes_big_endian();

    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE(stream);
}


BOOST_AUTO_TEST_CASE(roundtrip_variable_uint_little_endian_1_byte)
{
    const uint64_t expected = 0xAA;
    std::stringstream stream;
    ostream_writer sink(stream);
    istream_reader source(stream);

    sink.write_variable_uint_little_endian(expected);

    BOOST_REQUIRE(stream.str().length() == 1);

    uint64_t result = source.read_variable_uint_little_endian();

    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE(stream);
}

BOOST_AUTO_TEST_CASE(roundtrip_variable_uint_little_endian_2_bytes)
{
    const uint64_t expected = 43707;
    std::stringstream stream;
    ostream_writer sink(stream);
    istream_reader source(stream);

    sink.write_variable_uint_little_endian(expected);

    BOOST_REQUIRE(stream.str().length() == sizeof(uint16_t) + 1);

    uint64_t result = source.read_variable_uint_little_endian();

    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE(stream);
}

BOOST_AUTO_TEST_CASE(roundtrip_variable_uint_little_endian_4_bytes)
{
    const uint64_t expected = 2898120443;
    std::stringstream stream;
    ostream_writer sink(stream);
    istream_reader source(stream);

    sink.write_variable_uint_little_endian(expected);

    BOOST_REQUIRE(stream.str().length() == sizeof(uint32_t) + 1);

    uint64_t result = source.read_variable_uint_little_endian();

    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE(stream);
}

BOOST_AUTO_TEST_CASE(roundtrip_variable_uint_little_endian_8_bytes)
{
    const uint64_t expected = 0xd4b14be5d8f02abe;
    std::stringstream stream;
    ostream_writer sink(stream);
    istream_reader source(stream);

    sink.write_variable_uint_little_endian(expected);

    BOOST_REQUIRE(stream.str().length() == sizeof(uint64_t) + 1);

    uint64_t result = source.read_variable_uint_little_endian();

    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE(stream);
}

BOOST_AUTO_TEST_CASE(roundtrip_variable_uint_big_endian_1_byte)
{
    const uint64_t expected = 0xAA;
    std::stringstream stream;
    ostream_writer sink(stream);
    istream_reader source(stream);

    sink.write_variable_uint_big_endian(expected);

    BOOST_REQUIRE(stream.str().length() == 1);

    uint64_t result = source.read_variable_uint_big_endian();

    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE(stream);
}

BOOST_AUTO_TEST_CASE(roundtrip_variable_uint_big_endian_2_bytes)
{
    const uint64_t expected = 43707;
    std::stringstream stream;
    ostream_writer sink(stream);
    istream_reader source(stream);

    sink.write_variable_uint_big_endian(expected);

    BOOST_REQUIRE(stream.str().length() == sizeof(uint16_t) + 1);

    uint64_t result = source.read_variable_uint_big_endian();

    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE(stream);
}

BOOST_AUTO_TEST_CASE(roundtrip_variable_uint_big_endian_4_bytes)
{
    const uint64_t expected = 2898120443;
    std::stringstream stream;
    ostream_writer sink(stream);
    istream_reader source(stream);

    sink.write_variable_uint_big_endian(expected);

    BOOST_REQUIRE(stream.str().length() == sizeof(uint32_t) + 1);

    uint64_t result = source.read_variable_uint_big_endian();

    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE(stream);
}

BOOST_AUTO_TEST_CASE(roundtrip_variable_uint_big_endian_8_bytes)
{
    const uint64_t expected = 0xd4b14be5d8f02abe;
    std::stringstream stream;
    ostream_writer sink(stream);
    istream_reader source(stream);

    sink.write_variable_uint_big_endian(expected);

    BOOST_REQUIRE(stream.str().length() == sizeof(uint64_t) + 1);

    uint64_t result = source.read_variable_uint_big_endian();

    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE(stream);
}

BOOST_AUTO_TEST_CASE(roundtrip_data_chunk)
{
    const data_chunk expected = {
        0xfb, 0x44, 0x68, 0x84, 0xc6, 0xbf, 0x33, 0xc6, 0x27, 0x54, 0x73, 0x92,
        0x52, 0xa7, 0xb0, 0xf7, 0x47, 0x87, 0x89, 0x28, 0xf2, 0xf4, 0x18, 0x1d,
        0x01, 0x3f, 0xb7, 0xa2, 0xe9, 0x66, 0x69, 0xbf, 0x06, 0x83, 0x45, 0x34,
        0x8e, 0xc2, 0x9b, 0x3c, 0x86, 0xa9, 0xb8, 0x5f, 0xf7, 0x11, 0xa2, 0x00,
        0x5a, 0xa8
    };

    std::stringstream stream;
    ostream_writer sink(stream);
    istream_reader source(stream);

    sink.write_data(expected);

    BOOST_REQUIRE(stream.str().length() == expected.size());

    data_chunk result = source.read_data(expected.size());

    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE(stream);
}

BOOST_AUTO_TEST_CASE(roundtrip_hash)
{
    const hash_digest expected = {
        0x4d, 0xc9, 0x32, 0x18, 0x4d, 0x86, 0xa0, 0xb2, 0xe4, 0xba, 0x65, 0xa8,
        0x36, 0x1f, 0xea, 0x05, 0xf0, 0x26, 0x68, 0xa5, 0x09, 0x69, 0x10, 0x39,
        0x08, 0x95, 0x00, 0x7d, 0xa4, 0x2e, 0x7c, 0x12
    };

    std::stringstream stream;
    ostream_writer sink(stream);
    istream_reader source(stream);

    sink.write_hash(expected);

    BOOST_REQUIRE(stream.str().length() == expected.size());

    hash_digest result = source.read_hash();

    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE(stream);
}

BOOST_AUTO_TEST_CASE(roundtrip_short_hash)
{
    const short_hash expected = {
        0xed, 0x36, 0x48, 0xaf, 0x53, 0xc2, 0x8a, 0x79, 0x90, 0xab, 0x62, 0x04,
        0xb5, 0x2c, 0x6a, 0x40 , 0xdc, 0x6d, 0xa5, 0xfe
    };

    std::stringstream stream;
    ostream_writer sink(stream);
    istream_reader source(stream);

    sink.write_short_hash(expected);

    BOOST_REQUIRE(stream.str().length() == expected.size());

    short_hash result = source.read_short_hash();

    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE(stream);
}

BOOST_AUTO_TEST_CASE(roundtrip_fixed_string)
{
    const std::string expected = "my string data";

    std::stringstream stream;
    ostream_writer sink(stream);
    istream_reader source(stream);

    sink.write_fixed_string(expected, 10);

    BOOST_REQUIRE(stream.str().length() == 10);

    std::string result = source.read_fixed_string(10);

    BOOST_REQUIRE(expected.substr(0, 10) == result);
    BOOST_REQUIRE(stream);
}

BOOST_AUTO_TEST_CASE(roundtrip_string)
{
    const std::string expected = "my string data";

    std::stringstream stream;
    ostream_writer sink(stream);
    istream_reader source(stream);

    sink.write_string(expected);

    BOOST_REQUIRE_EQUAL(stream.str().length(),
        (expected.length() + variable_uint_size(expected.length())));

    std::string result = source.read_string();

    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE(stream);
}

BOOST_AUTO_TEST_CASE(roundtrip_bytes)
{
    const byte_array<32> expected = {
        0x4d, 0xc9, 0x32, 0x18, 0x4d, 0x86, 0xa0, 0xb2, 0xe4, 0xba, 0x65, 0xa8,
        0x36, 0x1f, 0xea, 0x05, 0xf0, 0x26, 0x68, 0xa5, 0x09, 0x69, 0x10, 0x39,
        0x08, 0x95, 0x00, 0x7d, 0xa4, 0x2e, 0x7c, 0x12
    };

    std::stringstream stream;
    ostream_writer sink(stream);
    istream_reader source(stream);

    sink.write_bytes<32>(expected);

    BOOST_REQUIRE(stream.str().length() == expected.size());

    byte_array<32> result = source.read_bytes<32>();

    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE(stream);
}

BOOST_AUTO_TEST_CASE(roundtrip_bytes_reverse)
{
    const byte_array<32> expected = {
        0x4d, 0xc9, 0x32, 0x18, 0x4d, 0x86, 0xa0, 0xb2, 0xe4, 0xba, 0x65, 0xa8,
        0x36, 0x1f, 0xea, 0x05, 0xf0, 0x26, 0x68, 0xa5, 0x09, 0x69, 0x10, 0x39,
        0x08, 0x95, 0x00, 0x7d, 0xa4, 0x2e, 0x7c, 0x12
    };

    std::stringstream stream;
    ostream_writer sink(stream);
    istream_reader source(stream);

    sink.write_bytes_reverse<32>(expected);

    BOOST_REQUIRE(stream.str().length() == expected.size());

    byte_array<32> result = source.read_bytes_reverse<32>();

    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE(stream);
}

BOOST_AUTO_TEST_CASE(write_bytes_read_bytes_reverse)
{
    const byte_array<32> expected = {
        0x4d, 0xc9, 0x32, 0x18, 0x4d, 0x86, 0xa0, 0xb2, 0xe4, 0xba, 0x65, 0xa8,
        0x36, 0x1f, 0xea, 0x05, 0xf0, 0x26, 0x68, 0xa5, 0x09, 0x69, 0x10, 0x39,
        0x08, 0x95, 0x00, 0x7d, 0xa4, 0x2e, 0x7c, 0x12
    };

    std::stringstream stream;
    ostream_writer sink(stream);
    istream_reader source(stream);

    sink.write_bytes<32>(expected);

    BOOST_REQUIRE(stream.str().length() == expected.size());

    byte_array<32> result = source.read_bytes_reverse<32>();

    BOOST_REQUIRE(std::equal(expected.rbegin(), expected.rend(), result.begin()));
    BOOST_REQUIRE(stream);
}

BOOST_AUTO_TEST_CASE(read_data_to_eof)
{
    const data_chunk expected = {
        0x4d, 0xc9, 0x32, 0x18, 0x4d, 0x86, 0xa0, 0xb2, 0xe4, 0xba, 0x65, 0xa8,
        0x36, 0x1f, 0xea, 0x05, 0xf0, 0x26, 0x68, 0xa5, 0x09, 0x69, 0x10, 0x39,
        0x08, 0x95, 0x00, 0x7d, 0xa4, 0x2e, 0x7c, 0x12
    };

    std::stringstream stream;
    ostream_writer sink(stream);
    istream_reader source(stream);

    sink.write_data(expected);

    BOOST_REQUIRE(stream.str().length() == expected.size());

    data_chunk result = source.read_data_to_eof();

    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE(stream);
}

BOOST_AUTO_TEST_SUITE_END()
