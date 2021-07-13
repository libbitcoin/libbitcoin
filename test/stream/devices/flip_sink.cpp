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

BOOST_AUTO_TEST_SUITE(stream_tests)

// input_sequence

BOOST_AUTO_TEST_CASE(flip_sink__input_sequence__empty__empty)
{
    data_chunk sink;
    flip_sink<data_slab> instance(sink);
    const auto sequence = instance.input_sequence();
    const auto first = reinterpret_cast<char*>(sink.data());
    const auto second = std::next(first, sink.size());
    BOOST_REQUIRE_EQUAL(first, sequence.first);
    BOOST_REQUIRE_EQUAL(second, sequence.second);
    BOOST_REQUIRE_EQUAL(std::distance(sequence.first, sequence.second), 0);
}

BOOST_AUTO_TEST_CASE(flip_sink__input_sequence__not_empty__expected)
{
    data_chunk sink(42, 0x00);
    flip_sink<data_slab> instance(sink);
    const auto sequence = instance.input_sequence();
    BOOST_REQUIRE_EQUAL(std::distance(sequence.first, sequence.second), 42);
}

// output_sequence

BOOST_AUTO_TEST_CASE(flip_sink__output_sequence__empty__empty)
{
    data_chunk sink;
    flip_sink<data_slab> instance(sink);
    const auto sequence = instance.output_sequence();
    const auto first = reinterpret_cast<char*>(sink.data());
    const auto second = std::next(first, sink.size());
    BOOST_REQUIRE_EQUAL(first, sequence.first);
    BOOST_REQUIRE_EQUAL(second, sequence.second);
    BOOST_REQUIRE_EQUAL(std::distance(sequence.first, sequence.second), 0);
}

BOOST_AUTO_TEST_CASE(flip_sink__output_sequence__not_empty__expected)
{
    data_chunk sink(42, 0x00);
    flip_sink<data_slab> instance(sink);
    const auto sequence = instance.output_sequence();
    BOOST_REQUIRE_EQUAL(std::distance(sequence.first, sequence.second), 42);
}

// read() and write() are not required for direct devices.
#ifdef UNDEFINED

// read (reading from the sink)

BOOST_AUTO_TEST_CASE(flip_sink__read__nullptr__false)
{
    data_chunk sink{ 0x42 };
    flip_sink<data_slab> instance(sink);
    BOOST_REQUIRE_EQUAL(instance.read(nullptr, 0), -1);
}

// zero-size array .data is null pointer.
BOOST_AUTO_TEST_CASE(flip_sink__read__empty__false)
{
    std::array<char, 0> to;
    data_chunk sink{ 0x42 };
    flip_sink<data_slab> instance(sink);
    BOOST_REQUIRE_EQUAL(instance.read(to.data(), 0), -1);
}

BOOST_AUTO_TEST_CASE(flip_sink__read__negative__false)
{
    std::array<char, 1> to{ { 0x00 } };
    data_chunk sink{ 0x42 };
    flip_sink<data_slab> instance(sink);
    BOOST_REQUIRE_EQUAL(instance.read(to.data(), -1), -1);
    BOOST_REQUIRE_EQUAL(to[0], 0x00);
}

BOOST_AUTO_TEST_CASE(flip_sink__read__past_end__expected)
{
    std::array<char, 1> to{ { 0x00 } };
    data_chunk sink{ 0x42 };
    flip_sink<data_slab> instance(sink);
    BOOST_REQUIRE_EQUAL(instance.read(to.data(), 2), 1);
    BOOST_REQUIRE_EQUAL(to[0], sink[0]);
}

// This was tested with a temporary manual hack of size_type to uint8_t.
////BOOST_AUTO_TEST_CASE(flip_sink__read__overflow__expected)
////{
////    std::array<char, max_uint8> to;
////    data_chunk sink(add1<int>(max_uint8), 0x42);
////    flip_sink<data_slab> instance(sink);
////    BOOST_REQUIRE_EQUAL(instance.read(to.data(), from.size()), max_uint8);
////}

BOOST_AUTO_TEST_CASE(flip_sink__read__zero__zero)
{
    std::array<char, 1> to{ { 0x00 } };
    data_chunk sink{ 0x42 };
    flip_sink<data_slab> instance(sink);
    BOOST_REQUIRE_EQUAL(instance.read(to.data(), 0), 0);
    BOOST_REQUIRE_EQUAL(to[0], 0x00);
}

BOOST_AUTO_TEST_CASE(flip_sink__read__one__expected)
{
    std::array<char, 1> to;
    data_chunk sink{ 0x42 };
    flip_sink<data_slab> instance(sink);
    BOOST_REQUIRE_EQUAL(instance.read(to.data(), 1), 1);
    BOOST_REQUIRE_EQUAL(to[0], sink[0]);
}

BOOST_AUTO_TEST_CASE(flip_sink__read__multiple__correct_tracking)
{
    std::array<char, 1> to1;
    std::array<char, 2> to2;
    std::array<char, 3> to3;
    std::array<char, 42> to0;
    data_chunk sink{ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 };
    flip_sink<data_slab> instance(sink);
    BOOST_REQUIRE_EQUAL(instance.read(to1.data(), 1), 1);
    BOOST_REQUIRE_EQUAL(to1[0], sink[0]);
    BOOST_REQUIRE_EQUAL(instance.read(to2.data(), 2), 2);
    BOOST_REQUIRE_EQUAL(to2[0], sink[1]);
    BOOST_REQUIRE_EQUAL(to2[1], sink[2]);
    BOOST_REQUIRE_EQUAL(instance.read(to3.data(), 3), 3);
    BOOST_REQUIRE_EQUAL(to3[0], sink[3]);
    BOOST_REQUIRE_EQUAL(to3[1], sink[4]);
    BOOST_REQUIRE_EQUAL(to3[2], sink[5]);
    BOOST_REQUIRE_EQUAL(instance.read(to0.data(), 42), 0);
}

// write

BOOST_AUTO_TEST_CASE(flip_sink__write__nullptr__false)
{
    data_chunk sink;
    flip_sink<data_slab> instance(sink);
    BOOST_REQUIRE_EQUAL(instance.write(nullptr, 0), -1);
}

BOOST_AUTO_TEST_CASE(flip_sink__write__empty__true)
{
    const std::string from;
    data_chunk sink{ 0x00 };
    flip_sink<data_slab> instance(sink);
    BOOST_REQUIRE_EQUAL(instance.write(from.data(), 0), 0);
}

BOOST_AUTO_TEST_CASE(flip_sink__write__negative__false)
{
    const std::string from{ "a" };
    data_chunk sink{ 0x00 };
    flip_sink<data_slab> instance(sink);
    BOOST_REQUIRE_EQUAL(instance.write(from.data(), -1), -1);
    BOOST_REQUIRE_EQUAL(sink[0], 0x00);
}

BOOST_AUTO_TEST_CASE(flip_sink__write__past_end__expected)
{
    const std::string from{ "a" };
    data_chunk sink{ 0x00 };
    flip_sink<data_slab> instance(sink);
    BOOST_REQUIRE_EQUAL(instance.write(from.data(), 2), 1);
    BOOST_REQUIRE_EQUAL(sink[0], from[0]);
}

BOOST_AUTO_TEST_CASE(flip_sink__write__zero__zero)
{
    const std::string from{ "a" };
    data_chunk sink{ 0x00 };
    flip_sink<data_slab> instance(sink);
    BOOST_REQUIRE_EQUAL(instance.write(from.data(), 0), 0);
    BOOST_REQUIRE_EQUAL(sink[0], 0x00);
}

BOOST_AUTO_TEST_CASE(flip_sink__write__one__expected)
{
    const std::string from{ "a" };
    data_chunk sink{ 0x00 };
    flip_sink<data_slab> instance(sink);
    BOOST_REQUIRE_EQUAL(instance.write(from.data(), 1), 1);
    BOOST_REQUIRE_EQUAL(sink[0], from[0]);
}

BOOST_AUTO_TEST_CASE(flip_sink__write__multiple__correct_tracking)
{
    const std::string from{ "abcdef" };
    data_chunk sink(6, 0x00);
    flip_sink<data_slab> instance(sink);
    BOOST_REQUIRE_EQUAL(instance.write(&from[0], 1), 1);
    BOOST_REQUIRE_EQUAL(sink[0], from[0]);
    BOOST_REQUIRE_EQUAL(instance.write(&from[1], 2), 2);
    BOOST_REQUIRE_EQUAL(sink[1], from[1]);
    BOOST_REQUIRE_EQUAL(sink[2], from[2]);
    BOOST_REQUIRE_EQUAL(instance.write(&from[3], 3), 3);
    BOOST_REQUIRE_EQUAL(sink[3], from[3]);
    BOOST_REQUIRE_EQUAL(sink[4], from[4]);
    BOOST_REQUIRE_EQUAL(sink[5], from[5]);
    BOOST_REQUIRE_EQUAL(instance.write(&from[6], 42), 0);
}

// read/write (single head)

BOOST_AUTO_TEST_CASE(flip_sink__read_write__read_first__expected)
{
    std::array<char, 1> to1;
    std::array<char, 1> to2;
    const std::string from{ "b" };
    data_chunk sink{ 'a', 0x00, 'c' };
    flip_sink<data_slab> instance(sink);
    BOOST_REQUIRE_EQUAL(instance.read(to1.data(), 1), 1);
    BOOST_REQUIRE_EQUAL(instance.write(from.data(), 1), 1);
    BOOST_REQUIRE_EQUAL(instance.read(to2.data(), 1), 1);
    BOOST_REQUIRE_EQUAL(to1[0], 'a');
    BOOST_REQUIRE_EQUAL(to2[0], 'c');
    BOOST_REQUIRE_EQUAL(sink[0], 'a');
    BOOST_REQUIRE_EQUAL(sink[1], 'b');
    BOOST_REQUIRE_EQUAL(sink[2], 'c');
}

BOOST_AUTO_TEST_CASE(flip_sink__read_write__write_first__expected)
{
    std::array<char, 1> to;
    const std::string from1{ "a" };
    const std::string from2{ "c" };
    data_chunk sink{ 0x00, 'b', 0x00 };
    flip_sink<data_slab> instance(sink);
    BOOST_REQUIRE_EQUAL(instance.write(from1.data(), 1), 1);
    BOOST_REQUIRE_EQUAL(instance.read(to.data(), 1), 1);
    BOOST_REQUIRE_EQUAL(instance.write(from2.data(), 1), 1);
    BOOST_REQUIRE_EQUAL(to[0], 'b');
    BOOST_REQUIRE_EQUAL(sink[0], 'a');
    BOOST_REQUIRE_EQUAL(sink[1], 'b');
    BOOST_REQUIRE_EQUAL(sink[2], 'c');
}

#endif // UNDEFINED

BOOST_AUTO_TEST_SUITE_END()
