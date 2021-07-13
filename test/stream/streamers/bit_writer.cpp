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

#define BIT_WRITER_CONTEXT
#define BIT_WRITER_BIG_ENDIAN
#define BIT_WRITER_LITTLE_ENDIAN
#define BIT_WRITER_BYTES
#define BIT_WRITER_STRINGS

#ifdef BIT_WRITER_CONTEXT

// bool

BOOST_AUTO_TEST_CASE(bit_writer__bool__stream_default__true)
{
    std::ostringstream stream;
    write::bits::ostream writer(stream);
    BOOST_REQUIRE(writer);
}

BOOST_AUTO_TEST_CASE(bit_writer__bool__not_empty__true)
{
    std::ostringstream stream{ "*" };
    write::bits::ostream writer(stream);
    BOOST_REQUIRE(writer);
}

BOOST_AUTO_TEST_CASE(bit_writer__bool__stream_end__true)
{
    std::stringstream stream{ "*" };
    stream.get();
    write::bits::ostream writer(stream);
    BOOST_REQUIRE(writer);
}

// !bool

BOOST_AUTO_TEST_CASE(bit_writer__not_bool__stream_invalid__true)
{
    std::stringstream stream{ "*" };
    stream.get();
    stream.get();
    write::bits::ostream writer(stream);
    BOOST_REQUIRE(!writer);
}

// flush

BOOST_AUTO_TEST_CASE(bit_writer__flush__push_sink_empty__valid)
{
    data_chunk sink;
    write::bits::data writer(sink);
    writer.flush();
    BOOST_REQUIRE(writer);
}

BOOST_AUTO_TEST_CASE(bit_writer__flush__empty_sstream__valid)
{
    std::ostringstream stream;
    write::bits::ostream writer(stream);
    writer.flush();
    BOOST_REQUIRE(writer);
}

// Test with our push_sink, since sstream always flushes on read.
BOOST_AUTO_TEST_CASE(bit_writer__flush__push_sink_not_empty__expected)
{
    data_chunk sink;
    write::bits::data writer(sink);
    writer.write_byte('*');
    BOOST_REQUIRE(sink.empty());
    writer.flush();
    BOOST_REQUIRE(!sink.empty());
    BOOST_REQUIRE_EQUAL(sink[0], '*');
    BOOST_REQUIRE(writer);
}

BOOST_AUTO_TEST_CASE(bit_writer__flush__not_empty_sstream__expected)
{
    std::ostringstream stream;
    write::bits::ostream writer(stream);
    writer.write_byte('*');
    writer.flush();
    BOOST_REQUIRE_EQUAL(stream.str(), "*");
    BOOST_REQUIRE(writer);
}

#endif // BIT_WRITER_CONTEXT

#ifdef BIT_WRITER_BIG_ENDIAN

// write_big_endian

BOOST_AUTO_TEST_CASE(bit_writer__write_big_endian__uint8_t__expected)
{
    std::ostringstream stream;
    write::bits::ostream writer(stream);
    writer.write_big_endian<uint8_t>(0x42);
    const std::string expected{ 0x42 };
    BOOST_REQUIRE_EQUAL(stream.str(), expected);
    BOOST_REQUIRE(writer);
}

BOOST_AUTO_TEST_CASE(bit_writer__write_big_endian__size_t__expected)
{
    std::ostringstream stream;
    write::bits::ostream writer(stream);
    writer.write_big_endian<size_t>(0x42);
    std::string expected(sizeof(size_t), 0x00);
    expected.back() = 0x42;
    BOOST_REQUIRE_EQUAL(stream.str(), expected);
    BOOST_REQUIRE(writer);
}

// write_2_bytes_big_endian

BOOST_AUTO_TEST_CASE(bit_writer__write_2_bytes_big_endian__value__expected)
{
    std::ostringstream stream;
    write::bits::ostream writer(stream);
    writer.write_2_bytes_big_endian(0x42);
    const std::string expected{ 0x00, 0x42 };
    BOOST_REQUIRE_EQUAL(stream.str(), expected);
    BOOST_REQUIRE(writer);
}

// write_4_bytes_big_endian

BOOST_AUTO_TEST_CASE(bit_writer__write_4_bytes_big_endian__value__expected)
{
    std::ostringstream stream;
    write::bits::ostream writer(stream);
    writer.write_4_bytes_big_endian(0x42);
    const std::string expected{ 0x00, 0x00, 0x00, 0x42 };
    BOOST_REQUIRE_EQUAL(stream.str(), expected);
    BOOST_REQUIRE(writer);
}

// write_8_bytes_big_endian

BOOST_AUTO_TEST_CASE(bit_writer__write_8_bytes_big_endian__value__expected)
{
    std::ostringstream stream;
    write::bits::ostream writer(stream);
    writer.write_8_bytes_big_endian(0x42);
    const std::string expected{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42 };
    BOOST_REQUIRE_EQUAL(stream.str(), expected);
    BOOST_REQUIRE(writer);
}

#endif // BIT_WRITER_BIG_ENDIAN

#ifdef BIT_WRITER_LITTLE_ENDIAN

// write_little_endian

BOOST_AUTO_TEST_CASE(bit_writer__write_little_endian__uint8_t__expected)
{
    std::ostringstream stream;
    write::bits::ostream writer(stream);
    writer.write_little_endian<uint8_t>(0x42);
    const std::string expected{ 0x42 };
    BOOST_REQUIRE_EQUAL(stream.str(), expected);
    BOOST_REQUIRE(writer);
}

BOOST_AUTO_TEST_CASE(bit_writer__write_little_endian__size_t__expected)
{
    std::ostringstream stream;
    write::bits::ostream writer(stream);
    writer.write_little_endian<size_t>(0x42);
    std::string expected(sizeof(size_t), 0x00);
    expected.front() = 0x42;
    BOOST_REQUIRE_EQUAL(stream.str(), expected);
    BOOST_REQUIRE(writer);
}

// write_2_bytes_little_endian

BOOST_AUTO_TEST_CASE(bit_writer__write_2_bytes_little_endian__value__expected)
{
    std::ostringstream stream;
    write::bits::ostream writer(stream);
    writer.write_2_bytes_little_endian(0x42);
    const std::string expected{ 0x42, 0x00 };
    BOOST_REQUIRE_EQUAL(stream.str(), expected);
    BOOST_REQUIRE(writer);
}

// write_4_bytes_little_endian

BOOST_AUTO_TEST_CASE(bit_writer__write_4_bytes_little_endian__value__expected)
{
    std::ostringstream stream;
    write::bits::ostream writer(stream);
    writer.write_4_bytes_little_endian(0x42);
    const std::string expected{ 0x42, 0x00, 0x00, 0x00 };
    BOOST_REQUIRE_EQUAL(stream.str(), expected);
    BOOST_REQUIRE(writer);
}

// write_8_bytes_little_endian

BOOST_AUTO_TEST_CASE(bit_writer__write_8_bytes_little_endian__value__expected)
{
    std::ostringstream stream;
    write::bits::ostream writer(stream);
    writer.write_8_bytes_little_endian(0x42);
    const std::string expected{ 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    BOOST_REQUIRE_EQUAL(stream.str(), expected);
    BOOST_REQUIRE(writer);
}

// write_variable

BOOST_AUTO_TEST_CASE(bit_writer__write_variable__one_byte__expected)
{
    std::ostringstream stream;
    write::bits::ostream writer(stream);
    writer.write_variable(0x08);
    const std::string expected{ 0x08 };
    BOOST_REQUIRE_EQUAL(stream.str(), expected);
    BOOST_REQUIRE(writer);
}

BOOST_AUTO_TEST_CASE(bit_writer__write_variable__two_bytes__expected)
{
    std::ostringstream stream;
    write::bits::ostream writer(stream);
    writer.write_variable(0x0708);
    const std::string expected{ (char)varint_two_bytes, 0x08, 0x07 };
    BOOST_REQUIRE_EQUAL(stream.str(), expected);
    BOOST_REQUIRE(writer);
}

BOOST_AUTO_TEST_CASE(bit_writer__write_variable__four_bytes__expected)
{
    std::ostringstream stream;
    write::bits::ostream writer(stream);
    writer.write_variable(0x05060708);
    const std::string expected{ (char)varint_four_bytes, 0x08, 0x07, 0x06, 0x05 };
    BOOST_REQUIRE_EQUAL(stream.str(), expected);
    BOOST_REQUIRE(writer);
}

BOOST_AUTO_TEST_CASE(bit_writer__write_variable__eight_bytes__expected)
{
    std::ostringstream stream;
    write::bits::ostream writer(stream);
    writer.write_variable(0x0102030405060708);
    const std::string expected{ (char)varint_eight_bytes, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01 };
    BOOST_REQUIRE_EQUAL(stream.str(), expected);
    BOOST_REQUIRE(writer);
}

// write_error_code

BOOST_AUTO_TEST_CASE(bit_writer__write_error_code__value__expected)
{
    std::ostringstream stream;
    write::bits::ostream writer(stream);
    writer.write_error_code(error::insufficient_work);
    const std::string expected{ (char)error::insufficient_work, 0x00, 0x00, 0x00 };
    BOOST_REQUIRE_EQUAL(stream.str(), expected);
    BOOST_REQUIRE(writer);
}

#endif // BIT_WRITER_LITTLE_ENDIAN

#ifdef BIT_WRITER_BYTES

// write

BOOST_AUTO_TEST_CASE(bit_writer__write__empty__empty)
{
    std::istringstream in;
    std::stringstream out;
    write::bits::ostream writer(out);
    BOOST_REQUIRE(writer.write(in));
    BOOST_REQUIRE(out.str().empty());
    BOOST_REQUIRE(writer);
}

BOOST_AUTO_TEST_CASE(bit_writer__write__value__expected)
{
    const std::string expected{ "abcdefghijklmnopqrstuvwxyz" };
    std::istringstream in{ expected };
    std::stringstream out;
    write::bits::ostream writer(out);
    BOOST_REQUIRE(writer.write(in));
    BOOST_REQUIRE_EQUAL(out.str(), expected);
    BOOST_REQUIRE(writer);
}

// write_byte

BOOST_AUTO_TEST_CASE(bit_writer__write_byte__always__expected)
{
    std::ostringstream stream;
    write::bits::ostream writer(stream);
    writer.write_byte(0x42);
    BOOST_REQUIRE_EQUAL(stream.str().front(), 0x42);
    BOOST_REQUIRE(writer);
}

// write_bytes1

BOOST_AUTO_TEST_CASE(bit_writer__write_bytes1__empty__empty)
{
    std::ostringstream stream;
    write::bits::ostream writer(stream);
    writer.write_bytes({});
    BOOST_REQUIRE(stream.str().empty());
    BOOST_REQUIRE(writer);
}

BOOST_AUTO_TEST_CASE(bit_writer__write_bytes1__value__expected)
{
    std::ostringstream stream;
    write::bits::ostream writer(stream);
    const std::string expected{ "abcdefghijklmnopqrstuvwxyz" };
    writer.write_bytes(expected);
    BOOST_REQUIRE_EQUAL(stream.str(), expected);
    BOOST_REQUIRE(writer);
}

// write_bytes2

BOOST_AUTO_TEST_CASE(bit_writer__write_bytes2__empty__empty)
{
    std::ostringstream stream;
    write::bits::ostream writer(stream);
    data_chunk data;
    writer.write_bytes(data.data(), data.size());
    BOOST_REQUIRE(stream.str().empty());
    BOOST_REQUIRE(writer);
}

BOOST_AUTO_TEST_CASE(bit_writer__write_bytes2__value__expected)
{
    std::ostringstream stream;
    write::bits::ostream writer(stream);
    const std::string expected{ "abcdefghijklmnopqrstuvwxyz" };
    writer.write_bytes(data_slice(expected).data(), expected.size());
    BOOST_REQUIRE_EQUAL(stream.str(), expected);
    BOOST_REQUIRE(writer);
}

#endif // BIT_WRITER_BYTES

#ifdef BIT_WRITER_STRINGS

// write_string1

BOOST_AUTO_TEST_CASE(bit_writer__write_string1__one_byte__expected)
{
    std::ostringstream stream;
    write::bits::ostream writer(stream);
    constexpr auto size = to_half(varint_two_bytes);
    const std::string expected(size, '*');
    writer.write_string(expected);
    BOOST_REQUIRE_EQUAL(stream.str(), std::string{ "\x7e" } + expected);
    BOOST_REQUIRE(writer);
}

BOOST_AUTO_TEST_CASE(bit_writer__write_string1__two_bytes__expected)
{
    std::ostringstream stream;
    write::bits::ostream writer(stream);
    constexpr auto size = varint_two_bytes;
    std::string expected(size, '*');
    writer.write_string(expected);
    expected.insert(expected.begin(), 0x00);
    expected.insert(expected.begin(), '\xfd');
    expected.insert(expected.begin(), '\xfd');
    BOOST_REQUIRE_EQUAL(stream.str(), expected);
    BOOST_REQUIRE(writer);
}

////// Too much memory allocation required to test.
////BOOST_AUTO_TEST_CASE(bit_writer__write_string1__four_bytes__expected)
////{
////    std::ostringstream stream;
////    write::bits::ostream writer(stream);
////    const auto size = add1<uint32_t>(max_uint16);
////    std::string expected(size, '*');
////    writer.write_string(expected);
////    expected.insert(expected.begin(), 0x00);
////    expected.insert(expected.begin(), 0x01);
////    expected.insert(expected.begin(), 0x00);
////    expected.insert(expected.begin(), 0x00);
////    expected.insert(expected.begin(), '\xfe');
////    BOOST_REQUIRE(writer);
////}
////
////// Too much memory allocation required to test.
////BOOST_AUTO_TEST_CASE(bit_writer__write_string1__eight_bytes__expected)
////{
////    std::ostringstream stream;
////    write::bits::ostream writer(stream);
////    const auto size = add1<uint64_t>(max_uint32);
////    std::string expected(size, '*');
////    writer.write_string(expected);
////    expected.insert(expected.begin(), 0x00);
////    expected.insert(expected.begin(), 0x00);
////    expected.insert(expected.begin(), 0x00);
////    expected.insert(expected.begin(), 0x01);
////    expected.insert(expected.begin(), 0x00);
////    expected.insert(expected.begin(), 0x00);
////    expected.insert(expected.begin(), 0x00);
////    expected.insert(expected.begin(), 0x00);
////    expected.insert(expected.begin(), '\xff');
////    BOOST_REQUIRE(writer);
////}

// write_string2

BOOST_AUTO_TEST_CASE(bit_writer__write_string2__empty__empty)
{
    std::ostringstream stream;
    write::bits::ostream writer(stream);
    writer.write_string({}, zero);
    BOOST_REQUIRE(stream.str().empty());
    BOOST_REQUIRE(writer);
}

BOOST_AUTO_TEST_CASE(bit_writer__write_string2__value__expected)
{
    std::ostringstream stream;
    write::bits::ostream writer(stream);
    const std::string expected{ "abcdefghijklmnopqrstuvwxyz" };
    writer.write_string(expected.data(), expected.size());
    BOOST_REQUIRE_EQUAL(stream.str(), expected);
    BOOST_REQUIRE(writer);
}

#endif // BIT_WRITER_STRINGS

BOOST_AUTO_TEST_SUITE_END()
