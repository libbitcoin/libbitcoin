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

const auto default_buffer_size = 1024;

// The compiler determines capacity, so this may be unreliable to test.
BOOST_AUTO_TEST_CASE(push_sink__optimal_buffer_size__default__default)
{
    data_chunk sink;
    push_sink<data_chunk> instance(sink);
    BOOST_REQUIRE_EQUAL(instance.optimal_buffer_size(), default_buffer_size);
}

// The compiler determines capacity, so this may be unreliable to test.
BOOST_AUTO_TEST_CASE(push_sink__optimal_buffer_size__reserve_empty__reserve)
{
    const auto sink_capacity = 1500;
    data_chunk sink;
    sink.reserve(sink_capacity);
    push_sink<data_chunk> instance(sink);
    BOOST_REQUIRE_EQUAL(instance.optimal_buffer_size(), sink_capacity);
}

// The compiler determines capacity, so this may be unreliable to test.
BOOST_AUTO_TEST_CASE(push_sink__optimal_buffer_size__reserve_not_empty__expected)
{
    const auto sink_size = 42;
    const auto sink_capacity = 1500;
    data_chunk sink(sink_size, 0x00);
    sink.reserve(sink_capacity);
    push_sink<data_chunk> instance(sink);
    BOOST_REQUIRE_EQUAL(instance.optimal_buffer_size(), sink_capacity - sink_size);
}

// The compiler determines capacity, so this may be unreliable to test.
BOOST_AUTO_TEST_CASE(push_sink__optimal_buffer_size__no_reserve_not_empty__default)
{
    const auto sink_size = 42;
    const auto sink_capacity = 42;
    data_chunk sink(sink_size, 0x00);
    sink.reserve(sink_capacity);
    push_sink<data_chunk> instance(sink);
    BOOST_REQUIRE_EQUAL(instance.optimal_buffer_size(), default_buffer_size);
}

BOOST_AUTO_TEST_CASE(push_sink__write__nullptr__false)
{
    data_chunk sink;
    push_sink<data_chunk> instance(sink);
    BOOST_REQUIRE_EQUAL(instance.write(nullptr, 0), -1);
}

BOOST_AUTO_TEST_CASE(push_sink__write__empty__true)
{
    data_chunk sink;
    push_sink<data_chunk> instance(sink);
    BOOST_REQUIRE_EQUAL(instance.write("", 0), 0);
}

BOOST_AUTO_TEST_CASE(push_sink__write__negative__false)
{
    data_chunk sink;
    push_sink<data_chunk> instance(sink);
    BOOST_REQUIRE_EQUAL(instance.write("a", -1), -1);
    BOOST_REQUIRE(sink.empty());
}

// This was tested with a temporary manual hack of data.max_size() to 1.
////BOOST_AUTO_TEST_CASE(push_sink__write__past_end__expected)
////{
////    data_chunk sink;
////    push_sink<data_chunk> instance(sink);
////    BOOST_REQUIRE_EQUAL(instance.write("ab", 2), 1);
////    BOOST_REQUIRE_EQUAL(sink.size(), 1u);
////    BOOST_REQUIRE_EQUAL(sink[0], 'a');
////}

BOOST_AUTO_TEST_CASE(push_sink__write__zero__zero)
{
    data_chunk sink;
    push_sink<data_chunk> instance(sink);
    BOOST_REQUIRE_EQUAL(instance.write("a", 0), 0);
    BOOST_REQUIRE(sink.empty());
}

BOOST_AUTO_TEST_CASE(push_sink__write__one__expected)
{
    data_chunk sink;
    push_sink<data_chunk> instance(sink);
    BOOST_REQUIRE_EQUAL(instance.write("a", 1), 1);
    BOOST_REQUIRE_EQUAL(sink.size(), 1u);
    BOOST_REQUIRE_EQUAL(sink[0], 'a');
}

BOOST_AUTO_TEST_CASE(push_sink__write__multiple__correct_tracking)
{
    data_chunk sink;
    push_sink<data_chunk> instance(sink);
    BOOST_REQUIRE_EQUAL(instance.write("a", 1), 1);
    BOOST_REQUIRE_EQUAL(sink.size(), 1u);
    BOOST_REQUIRE_EQUAL(sink[0], 'a');
    BOOST_REQUIRE_EQUAL(instance.write("bc", 2), 2);
    BOOST_REQUIRE_EQUAL(sink.size(), 3u);
    BOOST_REQUIRE_EQUAL(sink[1], 'b');
    BOOST_REQUIRE_EQUAL(sink[2], 'c');
    BOOST_REQUIRE_EQUAL(instance.write("def", 3), 3);
    BOOST_REQUIRE_EQUAL(sink.size(), 6u);
    BOOST_REQUIRE_EQUAL(sink[3], 'd');
    BOOST_REQUIRE_EQUAL(sink[4], 'e');
    BOOST_REQUIRE_EQUAL(sink[5], 'f');
}

BOOST_AUTO_TEST_SUITE_END()
