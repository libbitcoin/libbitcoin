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

BOOST_AUTO_TEST_CASE(copy_source__input_sequence__empty__empty)
{
    data_chunk sink;
    copy_source<data_reference> instance(sink);
    const auto sequence = instance.input_sequence();
    const auto first = reinterpret_cast<char*>(sink.data());
    const auto second = std::next(first, sink.size());
    BOOST_REQUIRE_EQUAL(first, sequence.first);
    BOOST_REQUIRE_EQUAL(second, sequence.second);
    BOOST_REQUIRE_EQUAL(std::distance(sequence.first, sequence.second), 0);
}

BOOST_AUTO_TEST_CASE(copy_source__input_sequence__not_empty__expected)
{
    data_chunk sink(42, 0x00);
    copy_source<data_reference> instance(sink);
    const auto sequence = instance.input_sequence();
    BOOST_REQUIRE_EQUAL(std::distance(sequence.first, sequence.second), 42);
}

// read() is not required for direct devices.
#ifdef UNDEFINED

// read

BOOST_AUTO_TEST_CASE(copy_source__read__nullptr__false)
{
    const data_chunk source{ 0x42 };
    copy_source<data_reference> instance(source);
    BOOST_REQUIRE_EQUAL(instance.read(nullptr, 0), -1);
}

// zero-size array .data is null pointer.
BOOST_AUTO_TEST_CASE(copy_source__read__empty__false)
{
    std::array<char, 0> to;
    const data_chunk source{ 0x42 };
    copy_source<data_reference> instance(source);
    BOOST_REQUIRE_EQUAL(instance.read(to.data(), 0), -1);
}

BOOST_AUTO_TEST_CASE(copy_source__read__negative__false)
{
    std::array<char, 1> to{ { 0x00 } };
    const data_chunk source{ 0x42 };
    copy_source<data_reference> instance(source);
    BOOST_REQUIRE_EQUAL(instance.read(to.data(), -1), -1);
    BOOST_REQUIRE_EQUAL(to[0], 0x00);
}

BOOST_AUTO_TEST_CASE(copy_source__read__past_end__expected)
{
    std::array<char, 1> to{ { 0x00 } };
    const data_chunk source{ 0x42 };
    copy_source<data_reference> instance(source);
    BOOST_REQUIRE_EQUAL(instance.read(to.data(), 2), 1);
    BOOST_REQUIRE_EQUAL(to[0], source[0]);
}

// This was tested with a temporary manual hack of size_type to uint8_t.
////BOOST_AUTO_TEST_CASE(copy_source__read__overflow__expected)
////{
////    std::array<char, max_uint8> to;
////    const data_chunk source(add1<int>(max_uint8), 0x42);
////    copy_source<data_reference> instance(source);
////    BOOST_REQUIRE_EQUAL(instance.read(to.data(), from.size()), max_uint8);
////}

BOOST_AUTO_TEST_CASE(copy_source__read__zero__zero)
{
    std::array<char, 1> to{ { 0x00 } };
    const data_chunk source{ 0x42 };
    copy_source<data_reference> instance(source);
    BOOST_REQUIRE_EQUAL(instance.read(to.data(), 0), 0);
    BOOST_REQUIRE_EQUAL(to[0], 0x00);
}

BOOST_AUTO_TEST_CASE(copy_source__read__one__expected)
{
    std::array<char, 1> to;
    const data_chunk source{ 0x42 };
    copy_source<data_reference> instance(source);
    BOOST_REQUIRE_EQUAL(instance.read(to.data(), 1), 1);
    BOOST_REQUIRE_EQUAL(to[0], source[0]);
}

BOOST_AUTO_TEST_CASE(copy_source__read__multiple__correct_tracking)
{
    std::array<char, 1> to1;
    std::array<char, 2> to2;
    std::array<char, 3> to3;
    std::array<char, 42> to0;
    const data_chunk source{ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 };
    copy_source<data_reference> instance(source);
    BOOST_REQUIRE_EQUAL(instance.read(to1.data(), 1), 1);
    BOOST_REQUIRE_EQUAL(to1[0], source[0]);
    BOOST_REQUIRE_EQUAL(instance.read(to2.data(), 2), 2);
    BOOST_REQUIRE_EQUAL(to2[0], source[1]);
    BOOST_REQUIRE_EQUAL(to2[1], source[2]);
    BOOST_REQUIRE_EQUAL(instance.read(to3.data(), 3), 3);
    BOOST_REQUIRE_EQUAL(to3[0], source[3]);
    BOOST_REQUIRE_EQUAL(to3[1], source[4]);
    BOOST_REQUIRE_EQUAL(to3[2], source[5]);
    BOOST_REQUIRE_EQUAL(instance.read(to0.data(), 42), 0);
}

#endif // UNDEFINED

BOOST_AUTO_TEST_SUITE_END()
