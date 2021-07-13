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
#include "../test.hpp"
#include <sstream>

BOOST_AUTO_TEST_SUITE(stream_tests)

// These are a simple single test per object in the stream tree.
// These are not meant to verify the behavior of the devices or streamers but
// instead to perform a simple smoke test for every construction. This ensures
// proper construction and basic behavior to and from boost::iostreams (ios).
// The devices and streamers independently tested in their own directories.

// read::bytes

BOOST_AUTO_TEST_CASE(read__bytes__istream__expected)
{
    std::istringstream istream{ "*" };
    read::bytes::istream reader(istream);
    BOOST_REQUIRE_EQUAL(reader.read_byte(), '*');
}

BOOST_AUTO_TEST_CASE(read__bytes__copy__expected)
{
    const data_chunk source{ '*' };
    read::bytes::copy reader(source);
    BOOST_REQUIRE_EQUAL(reader.read_byte(), '*');
}

// read::bits

BOOST_AUTO_TEST_CASE(read__bits__istream__expected)
{
    std::istringstream istream{ "*" };
    read::bits::istream reader(istream);
    BOOST_REQUIRE_EQUAL(reader.read_byte(), '*');
}

BOOST_AUTO_TEST_CASE(read__bits__copy__expected)
{
    const data_chunk source{ '*' };
    read::bits::copy reader(source);
    BOOST_REQUIRE_EQUAL(reader.read_byte(), '*');
}

// write::bytes
// push_size buffer allocation controlled by reserve.

BOOST_AUTO_TEST_CASE(write__bytes__ostream__expected)
{
    std::ostringstream ostream;
    write::bytes::ostream writer(ostream);
    writer.write_byte('*');
    BOOST_REQUIRE_EQUAL(ostream.str(), "*");
}

BOOST_AUTO_TEST_CASE(write__bytes__copy__expected)
{
    data_chunk sink{ 'x' };
    write::bytes::copy writer(sink);
    writer.write_byte('*');
    BOOST_REQUIRE_EQUAL(sink.front(), '*');
}

BOOST_AUTO_TEST_CASE(write__bytes__into_string__expected)
{
    std::string sink;
    sink.reserve(1);
    write::bytes::push<std::string> writer(sink);
    writer.write_byte('*');
    writer.flush();
    BOOST_REQUIRE_EQUAL(sink, "*");
}

BOOST_AUTO_TEST_CASE(write__bytes__into_data__expected)
{
    data_chunk sink;
    sink.reserve(1);
    write::bytes::push<data_chunk> writer(sink);
    writer.write_byte('*');
    writer.flush();
    BOOST_REQUIRE_EQUAL(sink.front(), '*');
}

// test concatenation (insert position)
BOOST_AUTO_TEST_CASE(write__bytes__text__expected)
{
    std::string sink{ "+/" };
    sink.reserve(3);
    write::bytes::push<std::string> writer(sink);
    writer.write_byte('-');
    writer.flush();
    BOOST_REQUIRE_EQUAL(sink, "+/-");
}

BOOST_AUTO_TEST_CASE(write__bytes__push__expected)
{
    data_chunk sink;
    sink.reserve(1);
    write::bytes::push<data_chunk> writer(sink);
    writer.write_byte('*');
    writer.flush();
    BOOST_REQUIRE_EQUAL(sink.front(), '*');
}

// write::bits
// push_size buffer allocation controlled by reserve.

BOOST_AUTO_TEST_CASE(write__bits__ostream__expected)
{
    std::ostringstream ostream;
    write::bits::ostream writer(ostream);
    writer.write_byte('*');
    BOOST_REQUIRE_EQUAL(ostream.str(), "*");
}

BOOST_AUTO_TEST_CASE(write__bits__copy__expected)
{
    data_chunk sink{ 'x' };
    write::bits::copy writer(sink);
    writer.write_byte('*');
    BOOST_REQUIRE_EQUAL(sink.front(), '*');
}

BOOST_AUTO_TEST_CASE(write__bits__into_string__expected)
{
    std::string sink;
    sink.reserve(1);
    write::bits::push<std::string> writer(sink);
    writer.write_byte('*');
    writer.flush();
    BOOST_REQUIRE_EQUAL(sink, "*");
}

BOOST_AUTO_TEST_CASE(write__bits__into_data__expected)
{
    data_chunk sink;
    sink.reserve(1);
    write::bits::push<data_chunk> writer(sink);
    writer.write_byte('*');
    writer.flush();
    BOOST_REQUIRE_EQUAL(sink.front(), '*');
}

// test concatenation (insert position)
BOOST_AUTO_TEST_CASE(write__bits__text__expected)
{
    std::string sink{ "+/" };
    sink.reserve(3);
    write::bits::push<std::string> writer(sink);
    writer.write_byte('-');
    writer.flush();
    BOOST_REQUIRE_EQUAL(sink, "+/-");
}

BOOST_AUTO_TEST_CASE(write__bits__push__expected)
{
    data_chunk sink;
    sink.reserve(1);
    write::bits::push<data_chunk> writer(sink);
    writer.write_byte('*');
    writer.flush();
    BOOST_REQUIRE_EQUAL(sink.front(), '*');
}

// flip::bytes (write)

BOOST_AUTO_TEST_CASE(flip__bytes__iostream_write__expected)
{
    std::stringstream iostream;
    flip::bytes::iostream flipper(iostream);
    flipper.write_byte('*');
    BOOST_REQUIRE_EQUAL(iostream.str(), "*");
}

BOOST_AUTO_TEST_CASE(flip__bytes__copy_write__expected)
{
    data_chunk data{ 'x' };
    flip::bytes::copy flipper(data);
    flipper.write_byte('*');
    BOOST_REQUIRE_EQUAL(data.front(), '*');
}

// flip::bytes (read)

BOOST_AUTO_TEST_CASE(flip__bytes__iostream_read__expected)
{
    std::stringstream iostream{ "*" };
    flip::bytes::iostream flipper(iostream);
    BOOST_REQUIRE_EQUAL(flipper.read_byte(), '*');
}

BOOST_AUTO_TEST_CASE(flip__bytes__copy_read__expected)
{
    data_chunk data{ '*' };
    flip::bytes::copy flipper(data);
    BOOST_REQUIRE_EQUAL(flipper.read_byte(), '*');
}

// flip::bits (write)

BOOST_AUTO_TEST_CASE(flip__bits__iostream_write__expected)
{
    std::stringstream iostream;
    flip::bits::iostream flipper(iostream);
    flipper.write_byte('*');
    BOOST_REQUIRE_EQUAL(iostream.str(), "*");
}

BOOST_AUTO_TEST_CASE(flip__bits__copy_write__expected)
{
    data_chunk data{ 'x' };
    flip::bits::copy flipper(data);
    flipper.write_byte('*');
    BOOST_REQUIRE_EQUAL(data.front(), '*');
}

// flip::bits (read)

BOOST_AUTO_TEST_CASE(flip__bits__iostream_read__expected)
{
    std::stringstream iostream{ "*" };
    flip::bits::iostream flipper(iostream);
    BOOST_REQUIRE_EQUAL(flipper.read_byte(), '*');
}

BOOST_AUTO_TEST_CASE(flip__bits__copy_read__expected)
{
    data_chunk data{ '*' };
    flip::bits::copy flipper(data);
    BOOST_REQUIRE_EQUAL(flipper.read_byte(), '*');
}

BOOST_AUTO_TEST_SUITE_END()