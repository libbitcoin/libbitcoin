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
#include <sstream>

BOOST_AUTO_TEST_SUITE(stream_tests)

// Failed get/peek reads are populated with 0x00 by the reader.
constexpr uint8_t pad = 0x00;

#define BYTE_READER_CONTEXT
#define BYTE_READER_BIG_ENDIAN
#define BYTE_READER_LITTLE_ENDIAN
#define BYTE_READER_BYTES
#define BYTE_READER_STRINGS

#ifdef BYTE_READER_CONTEXT

// bool

BOOST_AUTO_TEST_CASE(byte_reader__bool__stream_default__true)
{
    std::istringstream stream;
    read::bytes::istream reader(stream);
    BOOST_REQUIRE(reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__bool__not_empty__true)
{
    std::istringstream stream{ "*" };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE(reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__bool__stream_end__true)
{
    std::istringstream stream{ "*" };
    stream.get();
    read::bytes::istream reader(stream);
    BOOST_REQUIRE(reader);
}

// !bool

BOOST_AUTO_TEST_CASE(byte_reader__not_bool__stream_invalid__true)
{
    std::istringstream stream{ "*" };
    stream.get();
    stream.get();
    read::bytes::istream reader(stream);
    BOOST_REQUIRE(!reader);
}

// is_exhausted

BOOST_AUTO_TEST_CASE(byte_reader__is_exhausted__default__true)
{
    std::istringstream stream;
    read::bytes::istream reader(stream);
    BOOST_REQUIRE(reader.is_exhausted());
    BOOST_REQUIRE(stream);
}

BOOST_AUTO_TEST_CASE(byte_reader__is_exhausted__not_empty__false)
{
    std::istringstream stream{ "*" };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE(!reader.is_exhausted());
}

BOOST_AUTO_TEST_CASE(byte_reader__is_exhausted__end__true)
{
    std::istringstream stream{ "*" };
    stream.get();
    read::bytes::istream reader(stream);
    BOOST_REQUIRE(reader.is_exhausted());
}

BOOST_AUTO_TEST_CASE(byte_reader__is_exhausted__invalid__true)
{
    std::istringstream stream{ "*" };
    stream.get();
    stream.get();
    read::bytes::istream reader(stream);
    BOOST_REQUIRE(reader.is_exhausted());
}

BOOST_AUTO_TEST_CASE(byte_reader__is_exhausted__valid__does_not_change_state)
{
    std::istringstream stream;
    read::bytes::istream reader(stream);
    const auto state = stream.rdstate();
    BOOST_REQUIRE(reader.is_exhausted());
    BOOST_REQUIRE_EQUAL(stream.rdstate(), state);
    BOOST_REQUIRE(stream);
}

BOOST_AUTO_TEST_CASE(byte_reader__is_exhausted__invalid__does_not_change_state)
{
    std::istringstream stream{ "*" };
    read::bytes::istream reader(stream);
    stream.get();
    stream.get();
    const auto state = stream.rdstate();
    BOOST_REQUIRE(reader.is_exhausted());
    BOOST_REQUIRE_EQUAL(stream.rdstate(), state);
    BOOST_REQUIRE(!stream);
}

// invalidate

BOOST_AUTO_TEST_CASE(byte_reader__invalidate__empty__exhausts_reader)
{
    std::istringstream stream;
    read::bytes::istream reader(stream);
    reader.invalidate();
    BOOST_REQUIRE(reader.is_exhausted());
}

BOOST_AUTO_TEST_CASE(byte_reader__invalidate__empty__invalid)
{
    std::istringstream stream;
    read::bytes::istream reader(stream);
    reader.invalidate();
    BOOST_REQUIRE(!reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__invalidate__not_empty__stream_invalid)
{
    std::istringstream stream{ "*" };
    read::bytes::istream reader(stream);
    reader.invalidate();
    BOOST_REQUIRE(!stream);
}

// skip

BOOST_AUTO_TEST_CASE(byte_reader__skip__default_empty__invalid)
{
    std::istringstream stream;
    read::bytes::istream reader(stream);
    reader.skip_byte();
    BOOST_REQUIRE(!reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__skip__default_single__exhausted)
{
    std::istringstream stream{ "*" };
    read::bytes::istream reader(stream);
    reader.skip_byte();
    BOOST_REQUIRE(reader);
    BOOST_REQUIRE(reader.is_exhausted());
}

BOOST_AUTO_TEST_CASE(byte_reader__skip__end__exhausted)
{
    const auto size = 42;
    std::istringstream stream{ std::string(size, 0x00) };
    read::bytes::istream reader(stream);
    reader.skip_bytes(size);
    BOOST_REQUIRE(reader);
    BOOST_REQUIRE(reader.is_exhausted());
}

BOOST_AUTO_TEST_CASE(byte_reader__skip__past_end__invalid)
{
    const auto size = 42;
    std::istringstream stream{ std::string(size, 0x00) };
    read::bytes::istream reader(stream);
    reader.skip_bytes(add1(size));
    BOOST_REQUIRE(!reader);
}

// rewind

BOOST_AUTO_TEST_CASE(byte_reader__rewind__default_single__expected)
{
    std::istringstream stream{ "*" };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_byte(), '*');
    BOOST_REQUIRE(reader.is_exhausted());
    reader.rewind_byte();
    BOOST_REQUIRE(!reader.is_exhausted());
    BOOST_REQUIRE_EQUAL(reader.read_byte(), '*');
    BOOST_REQUIRE(reader.is_exhausted());
    BOOST_REQUIRE(reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__rewind__skip_to_end_rewind_to_middle__expected)
{
    const auto size = 42;
    const std::string value(size, 0x00);
    std::istringstream stream{ value + "*" + value };
    read::bytes::istream reader(stream);
    reader.skip_bytes(size);
    BOOST_REQUIRE_EQUAL(reader.read_byte(), '*');
    reader.skip_bytes(size);
    BOOST_REQUIRE(reader);
    BOOST_REQUIRE(reader.is_exhausted());
    reader.rewind_bytes(add1(size));
    BOOST_REQUIRE_EQUAL(reader.read_byte(), '*');
    BOOST_REQUIRE(reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__rewind__past_begin__invalid)
{
    const auto size = 42;
    std::istringstream stream{ std::string(size, 0x00) };
    read::bytes::istream reader(stream);
    reader.skip_bytes(size);
    reader.rewind_bytes(add1(size));
    BOOST_REQUIRE(!reader);
}

#endif // BYTE_READER_CONTEXT

#ifdef BYTE_READER_BIG_ENDIAN

// read_big_endian

BOOST_AUTO_TEST_CASE(byte_reader__read_big_endian__empty__zero)
{
    std::istringstream stream;
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_big_endian<size_t>(), 0u);
    BOOST_REQUIRE(!reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_big_endian__full__expected)
{
    const std::string value{ 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01 };
    std::istringstream stream{ value };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_big_endian<uint64_t>(), 0x0807060504030201);
    BOOST_REQUIRE(reader);
}

// read_2_bytes_big_endian

BOOST_AUTO_TEST_CASE(byte_reader__read_2_bytes_big_endian__empty__zero)
{
    std::istringstream stream;
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_2_bytes_big_endian(), 0u);
    BOOST_REQUIRE(!reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_2_bytes_big_endian__full__expected)
{
    const std::string value{ 0x08, 0x07 };
    std::istringstream stream{ value };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_2_bytes_big_endian(), 0x0807);
    BOOST_REQUIRE(reader);
}

// read_4_bytes_big_endian

BOOST_AUTO_TEST_CASE(byte_reader__read_4_bytes_big_endian__empty__zero)
{
    std::istringstream stream;
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_4_bytes_big_endian(), 0u);
    BOOST_REQUIRE(!reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_4_bytes_big_endian__full__expected)
{
    const std::string value{ 0x08, 0x07, 0x06, 0x05 };
    std::istringstream stream{ value };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_4_bytes_big_endian(), 0x08070605);
    BOOST_REQUIRE(reader);
}

// read_8_bytes_big_endian

BOOST_AUTO_TEST_CASE(byte_reader__read_8_bytes_big_endian__empty__zero)
{
    std::istringstream stream;
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_8_bytes_big_endian(), 0u);
    BOOST_REQUIRE(!reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_8_bytes_big_endian__full__expected)
{
    const std::string value{ 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01 };
    std::istringstream stream{ value };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_8_bytes_big_endian(), 0x0807060504030201);
    BOOST_REQUIRE(reader);
}

#endif // BYTE_READER_BIG_ENDIAN

#ifdef BYTE_READER_LITTLE_ENDIAN

// read_little_endian

BOOST_AUTO_TEST_CASE(byte_reader__read_little_endian__empty__zero)
{
    std::istringstream stream;
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_little_endian<size_t>(), 0u);
    BOOST_REQUIRE(!reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_little_endian__full__expected)
{
    const std::string value{ 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01 };
    std::istringstream stream{ value };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_little_endian<size_t>(), 0x0102030405060708);
    BOOST_REQUIRE(reader);
}

// read_2_bytes_little_endian

BOOST_AUTO_TEST_CASE(byte_reader__read_2_bytes_little_endian__empty__zero)
{
    std::istringstream stream;
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_2_bytes_little_endian(), 0u);
    BOOST_REQUIRE(!reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_2_bytes_little_endian__full__expected)
{
    const std::string value{ 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01 };
    std::istringstream stream{ value };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_2_bytes_little_endian(), 0x0708);
    BOOST_REQUIRE(reader);
}

// read_4_bytes_little_endian

BOOST_AUTO_TEST_CASE(byte_reader__read_4_bytes_little_endian__empty__zero)
{
    std::istringstream stream;
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_4_bytes_little_endian(), 0u);
    BOOST_REQUIRE(!reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_4_bytes_little_endian__full__expected)
{
    const std::string value{ 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01 };
    std::istringstream stream{ value };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_4_bytes_little_endian(), 0x05060708);
    BOOST_REQUIRE(reader);
}

// read_8_bytes_little_endian

BOOST_AUTO_TEST_CASE(byte_reader__read_8_bytes_little_endian__empty__zero)
{
    std::istringstream stream;
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_8_bytes_little_endian(), 0u);
    BOOST_REQUIRE(!reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_8_bytes_little_endian__full__expected)
{
    const std::string value{ 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01 };
    std::istringstream stream{ value };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_8_bytes_little_endian(), 0x0102030405060708);
    BOOST_REQUIRE(reader);
}

// read_size/read_variable

BOOST_AUTO_TEST_CASE(byte_reader__read_variable_size__one_byte__expected)
{
    const std::string value{ 0x08 };
    std::istringstream stream{ value };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_size(), 0x08);
    reader.rewind_bytes(1);
    BOOST_REQUIRE_EQUAL(reader.read_variable(), 0x08);
    BOOST_REQUIRE(reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_variable_size__two_bytes__expected)
{
    const std::string value{ (char)varint_two_bytes, 0x08, 0x07 };
    std::istringstream stream{ value };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_size(), 0x0708);
    reader.rewind_bytes(3);
    BOOST_REQUIRE_EQUAL(reader.read_variable(), 0x0708);
    BOOST_REQUIRE(reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_variable_size__four_bytes__expected)
{
    const std::string value{ (char)varint_four_bytes, 0x08, 0x07, 0x06, 0x05 };
    std::istringstream stream{ value };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_size(), 0x05060708);
    reader.rewind_bytes(5);
    BOOST_REQUIRE_EQUAL(reader.read_variable(), 0x05060708);
    BOOST_REQUIRE(reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_variable_size__eight_bytes__expected)
{
    const std::string value{ (char)varint_eight_bytes, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01 };
    std::istringstream stream{ value };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_variable(), 0x0102030405060708);
    reader.rewind_bytes(9);

    if (sizeof(size_t) < sizeof(uint64_t))
    {
        BOOST_REQUIRE_EQUAL(reader.read_size(), 0);
    }
    else
    {
        BOOST_REQUIRE_EQUAL(reader.read_size(), 0x0102030405060708);
    }

    BOOST_REQUIRE(reader);
}


// read_error_code

BOOST_AUTO_TEST_CASE(byte_reader__read_error_code__empty__default_invalid)
{
    std::istringstream stream;
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_error_code(), error::success);
    BOOST_REQUIRE(!reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_error_code__value__expected)
{
    const std::string value{ (char)error::insufficient_work, 0x00, 0x00, 0x00, '*' };
    std::stringstream stream{ value };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_error_code(), error::insufficient_work);
    BOOST_REQUIRE(reader);
}

#endif // BYTE_READER_LITTLE_ENDIAN

#ifdef BYTE_READER_BYTES

// read_forward

BOOST_AUTO_TEST_CASE(byte_reader__read_forward__empty__pad_invalid)
{
    std::istringstream stream;
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_forward<1>()[0], pad);
    BOOST_REQUIRE(!reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_forward__not_empty__expected)
{
    const data_array<4> expected{ 0x01, 0x02, 0x03, 0x04 };
    std::istringstream stream{ "*" + to_string(expected) + "*" };
    read::bytes::istream reader(stream);
    reader.skip_byte();
    BOOST_REQUIRE_EQUAL(reader.read_forward<4>(), expected);
    BOOST_REQUIRE(reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_forward__past_end__padded_invalid)
{
    const data_array<4> expected{ 0x01, 0x02, pad, pad };
    std::istringstream stream{ to_string(expected).substr(0, 2) };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_forward<4>(), expected);
    BOOST_REQUIRE(!reader);
}

// read_reverse

BOOST_AUTO_TEST_CASE(byte_reader__read_reverse__empty__pad_invalid)
{
    std::istringstream stream;
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_reverse<1>()[0], pad);
    BOOST_REQUIRE(!reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_reverse__not_empty__expected)
{
    const data_array<4> value{ 0x01, 0x02, 0x03, 0x04 };
    const data_array<4> expected{ 0x04, 0x03, 0x02, 0x01 };
    std::istringstream stream{ "*" + to_string(value) + "*" };
    read::bytes::istream reader(stream);
    reader.skip_byte();
    BOOST_REQUIRE_EQUAL(reader.read_reverse<4>(), expected);
    BOOST_REQUIRE(reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_reverse__past_end__padded_invalid)
{
    const data_array<2> value{ 0x01, 0x02 };
    const data_array<4> expected{ pad, pad, 0x02, 0x01 };
    std::istringstream stream{ to_string(value) };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_reverse<4>(), expected);
    BOOST_REQUIRE(!reader);
}

// read

BOOST_AUTO_TEST_CASE(byte_reader__read__empty__empty)
{
    std::istringstream in;
    std::ostringstream out;
    read::bytes::istream reader(in);
    BOOST_REQUIRE(reader.read(out));
    BOOST_REQUIRE(out.str().empty());
    BOOST_REQUIRE(reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read__value__expected)
{
    const std::string expected{ "abcdefghijklmnopqrstuvwxyz" };
    std::istringstream in{ expected };
    std::ostringstream out;
    read::bytes::istream reader(in);
    BOOST_REQUIRE(reader.read(out));
    BOOST_REQUIRE_EQUAL(out.str(), expected);
    BOOST_REQUIRE(reader);
}

// read_mini_hash

BOOST_AUTO_TEST_CASE(byte_reader__read_mini_hash__empty__null_hash)
{
    std::istringstream stream{ to_string(null_hash) };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_mini_hash(), null_mini_hash);
    BOOST_REQUIRE(reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_mini_hash__value__expected)
{
    const mini_hash expected{ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 };
    std::istringstream stream{ to_string(expected) };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_mini_hash(), expected);
    BOOST_REQUIRE(reader);
}

// read_short_hash

BOOST_AUTO_TEST_CASE(byte_reader__read_short_hash__value__expected)
{
    const short_hash expected
    {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a
    };

    std::istringstream stream{ to_string(expected) };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_short_hash(), expected);
    BOOST_REQUIRE(reader);
}

// read_hash

BOOST_AUTO_TEST_CASE(byte_reader__read_hash__value__expected)
{
    const hash_digest expected
    {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
        0x01, 0x02
    };

    std::istringstream stream{ to_string(expected) };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_hash(), expected);
    BOOST_REQUIRE(reader);
}

// read_long_hash

BOOST_AUTO_TEST_CASE(byte_reader__read_long_hash__value__expected)
{
    const long_hash expected
    {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
        0x01, 0x02, 0x03, 0x04
    };

    std::istringstream stream{ to_string(expected) };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_long_hash(), expected);
    BOOST_REQUIRE(reader);
}

// peek_byte

BOOST_AUTO_TEST_CASE(byte_reader__peek_byte__empty__pad_invalid)
{
    std::istringstream stream;
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.peek_byte(), pad);
    BOOST_REQUIRE_EQUAL(reader.peek_byte(), pad);
    BOOST_REQUIRE_EQUAL(reader.peek_byte(), pad);
    BOOST_REQUIRE(!reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__peek_byte__end__pad_invalid)
{
    std::istringstream stream{ "abc" };
    read::bytes::istream reader(stream);
    reader.skip_bytes(3);
    BOOST_REQUIRE_EQUAL(reader.peek_byte(), pad);
    BOOST_REQUIRE_EQUAL(reader.peek_byte(), pad);
    BOOST_REQUIRE_EQUAL(reader.peek_byte(), pad);
    BOOST_REQUIRE(!reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__peek_byte__not_empty__expected_not_advanced)
{
    std::istringstream stream{ "abc" };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.peek_byte(), 'a');
    BOOST_REQUIRE_EQUAL(reader.peek_byte(), 'a');
    BOOST_REQUIRE_EQUAL(reader.peek_byte(), 'a');
    BOOST_REQUIRE(reader);
}

// read_byte

BOOST_AUTO_TEST_CASE(byte_reader__read_byte__empty__pad_invalid)
{
    std::istringstream stream;
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_byte(), pad);
    BOOST_REQUIRE_EQUAL(reader.read_byte(), pad);
    BOOST_REQUIRE_EQUAL(reader.read_byte(), pad);
    BOOST_REQUIRE(!reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_byte__end__pad_invalid)
{
    std::istringstream stream{ "abc" };
    read::bytes::istream reader(stream);
    reader.skip_bytes(3);
    BOOST_REQUIRE_EQUAL(reader.read_byte(), pad);
    BOOST_REQUIRE_EQUAL(reader.read_byte(), pad);
    BOOST_REQUIRE_EQUAL(reader.read_byte(), pad);
    BOOST_REQUIRE(!reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_byte__not_empty__expected_advanced)
{
    std::istringstream stream{ "abc" };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_byte(), 'a');
    BOOST_REQUIRE_EQUAL(reader.read_byte(), 'b');
    BOOST_REQUIRE_EQUAL(reader.read_byte(), 'c');
    BOOST_REQUIRE(reader);
}

// read_bytes0

BOOST_AUTO_TEST_CASE(byte_reader__read_bytes0__empty__empty_valid)
{
    std::istringstream stream;
    read::bytes::istream reader(stream);
    BOOST_REQUIRE(reader.read_bytes().empty());
    BOOST_REQUIRE(reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_bytes0__end__empty_valid)
{
    std::istringstream stream{ "abc" };
    read::bytes::istream reader(stream);
    reader.skip_bytes(3);
    BOOST_REQUIRE(reader.read_bytes().empty());
    BOOST_REQUIRE(reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_bytes0__not_empty__expected)
{
    std::istringstream stream{ "*abc" };
    read::bytes::istream reader(stream);
    reader.skip_byte();
    BOOST_REQUIRE_EQUAL(reader.read_bytes(), to_chunk("abc"));
    BOOST_REQUIRE(reader);
}

// read_bytes1

BOOST_AUTO_TEST_CASE(byte_reader__read_bytes1__empty_zero__empty_valid)
{
    std::istringstream stream;
    read::bytes::istream reader(stream);
    BOOST_REQUIRE(reader.read_bytes(0).empty());
    BOOST_REQUIRE(reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_bytes1__end_zero__empty_valid)
{
    std::istringstream stream{ "abc" };
    read::bytes::istream reader(stream);
    reader.skip_bytes(3);
    BOOST_REQUIRE(reader.read_bytes(0).empty());
    BOOST_REQUIRE(reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_bytes1__to_end__expected)
{
    std::istringstream stream{ "*abc" };
    read::bytes::istream reader(stream);
    reader.skip_byte();
    BOOST_REQUIRE_EQUAL(reader.read_bytes(3), to_chunk("abc"));
    BOOST_REQUIRE(reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_bytes1__middle__expected)
{
    std::istringstream stream{ "*abc*" };
    read::bytes::istream reader(stream);
    reader.skip_byte();
    BOOST_REQUIRE_EQUAL(reader.read_bytes(3), to_chunk("abc"));
    BOOST_REQUIRE_EQUAL(stream.get(), '*');
    BOOST_REQUIRE(reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_bytes1__past_end__expected_invalid)
{
    std::istringstream stream{ "abc" };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_bytes(6), to_chunk("abc\0\0\0"));
    BOOST_REQUIRE(!reader);
}

// read_bytes2

BOOST_AUTO_TEST_CASE(byte_reader__read_bytes2__empty_zero__empty_valid)
{
    data_chunk sink;
    std::istringstream stream;
    read::bytes::istream reader(stream);
    reader.read_bytes(sink.data(), 0);
    BOOST_REQUIRE(reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_bytes2__end_zero__empty_valid)
{
    data_chunk sink;
    std::istringstream stream{ "abc" };
    read::bytes::istream reader(stream);
    reader.skip_bytes(3);
    reader.read_bytes(sink.data(), 0);
    BOOST_REQUIRE(reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_bytes2__to_end__expected)
{
    data_chunk sink(3, 0x00);
    std::istringstream stream{ "*abc" };
    read::bytes::istream reader(stream);
    reader.skip_byte();
    reader.read_bytes(sink.data(), sink.size());
    BOOST_REQUIRE_EQUAL(sink, to_chunk("abc"));
    BOOST_REQUIRE(reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_bytes2__middle__expected)
{
    data_chunk sink(3, 0x00);
    std::istringstream stream{ "*abc*" };
    read::bytes::istream reader(stream);
    reader.skip_byte();
    reader.read_bytes(sink.data(), sink.size());
    BOOST_REQUIRE_EQUAL(sink, to_chunk("abc"));
    BOOST_REQUIRE_EQUAL(stream.get(), '*');
    BOOST_REQUIRE(reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_bytes2__past_end__expected_invalid)
{
    data_chunk sink(6, 0x00);
    std::istringstream stream{ "abc" };
    read::bytes::istream reader(stream);
    reader.read_bytes(sink.data(), sink.size());
    BOOST_REQUIRE_EQUAL(sink, to_chunk("abc\0\0\0"));
    BOOST_REQUIRE(!reader);
}

#endif // BYTE_READER_BYTES

#ifdef BYTE_READER_STRINGS

// read_string0

BOOST_AUTO_TEST_CASE(byte_reader__read_string0__one_byte__expected)
{
    const std::string value{ 0x03, 'a', 'b', 'c' };
    std::istringstream stream{ value };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_string(), "abc");
}

BOOST_AUTO_TEST_CASE(byte_reader__read_string0__two_bytes__expected)
{
    const std::string value{ (char)varint_two_bytes, 0x03, 0x00, 'a', 'b', 'c' };
    std::istringstream stream{ value };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_string(), "abc");
}

BOOST_AUTO_TEST_CASE(byte_reader__read_string0__four_bytes__expected)
{
    const std::string value{ (char)varint_four_bytes, 0x03, 0x00, 0x00, 0x00, 'a', 'b', 'c' };
    std::istringstream stream{ value };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_string(), "abc");
}

BOOST_AUTO_TEST_CASE(byte_reader__read_string0__eight_bytes__expected)
{
    const std::string value{ (char)varint_eight_bytes, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 'a', 'b', 'c' };
    std::istringstream stream{ value };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_string(), "abc");
}

// read_string1

BOOST_AUTO_TEST_CASE(byte_reader__read_string1__zero__empty_valid)
{
    std::istringstream stream;
    read::bytes::istream reader(stream);
    BOOST_REQUIRE(reader.read_string(0).empty());
    BOOST_REQUIRE(reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_string1__partial__expected_valid)
{
    const std::string value{ "abcdefghij" };
    const auto length = to_half(value.length());
    std::istringstream stream{ value };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_string(length), value.substr(0, length));
    BOOST_REQUIRE(reader);
}

BOOST_AUTO_TEST_CASE(byte_reader__read_string1__full__expected_valid)
{
    const std::string value{ "abcdefghij" };
    const auto length = value.length();
    std::istringstream stream{ value + "*" };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_string(length), value);
    BOOST_REQUIRE_EQUAL(stream.get(), '*');
    BOOST_REQUIRE(reader);
}

// The full number of bytes are read, but not past end.
BOOST_AUTO_TEST_CASE(byte_reader__read_string1__past_end__truncated_valid)
{
    const std::string value{ "abcdefghij" };
    const auto length = value.length();
    std::istringstream stream{ value };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_string(add1(length)), value);
    BOOST_REQUIRE(reader);
}

// The full number of bytes are read, and the string is terminated at null.
BOOST_AUTO_TEST_CASE(byte_reader__read_string1__full_embedded_null__truncated_exhausted)
{
    const std::string value{ "abcdef\0hij" };
    const auto length = value.length();
    std::istringstream stream{ value };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_string(length), "abcdef");
    BOOST_REQUIRE(reader);
    BOOST_REQUIRE(reader.is_exhausted());
}

// The full number of bytes are read, and the string is terminated at null.
BOOST_AUTO_TEST_CASE(byte_reader__read_string1__partial_embedded_null__truncated_not_exhausted)
{
    const std::string value{ "abcdef\0hij" };
    const auto length = value.length();
    std::istringstream stream{ value + "*" };
    read::bytes::istream reader(stream);
    BOOST_REQUIRE_EQUAL(reader.read_string(length), "abcdef");
    BOOST_REQUIRE(reader);
    BOOST_REQUIRE(!reader.is_exhausted());
    BOOST_REQUIRE_EQUAL(stream.get(), '*');
}

#endif // BYTE_READER_STRINGS

BOOST_AUTO_TEST_SUITE_END()
