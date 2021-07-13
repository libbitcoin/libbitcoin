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

BOOST_AUTO_TEST_SUITE(stream_tests)

// Access the protected constructor.
class device_accessor
  : public device<data_chunk>
{
public:
    device_accessor()
      : device(0)
    {
        // Only zero size remaining is safe to test.
    }
};

// optimal_buffer_size

BOOST_AUTO_TEST_CASE(device__optimal_buffer_size__always__expected)
{
    device_accessor instance;
    BOOST_REQUIRE_EQUAL(instance.optimal_buffer_size(), 4096);
}

// input_sequence

BOOST_AUTO_TEST_CASE(device__input_sequence__not_empty__empty)
{
    device_accessor instance;
    const auto sequence = instance.input_sequence();
    BOOST_REQUIRE_EQUAL(sequence.first, nullptr);
    BOOST_REQUIRE_EQUAL(sequence.second, nullptr);
}

// output_sequence

BOOST_AUTO_TEST_CASE(device__output_sequence__not_empty__empty)
{
    device_accessor instance;
    const auto sequence = instance.output_sequence();
    BOOST_REQUIRE_EQUAL(sequence.first, nullptr);
    BOOST_REQUIRE_EQUAL(sequence.second, nullptr);
}

// write

BOOST_AUTO_TEST_CASE(device__write__nullptr__false)
{
    device_accessor instance;
    BOOST_REQUIRE_EQUAL(instance.write(nullptr, 0), -1);
}

BOOST_AUTO_TEST_CASE(device__write__empty__true)
{
    const std::string from;
    device_accessor instance;
    BOOST_REQUIRE_EQUAL(instance.write(from.data(), 0), 0);
}

BOOST_AUTO_TEST_CASE(device__write__negative__false)
{
    const std::string from{ "a" };
    device_accessor instance;
    BOOST_REQUIRE_EQUAL(instance.write(from.data(), -1), -1);
}

BOOST_AUTO_TEST_CASE(device__write__past_end__zero)
{
    const std::string from{ "a" };
    device_accessor instance;
    BOOST_REQUIRE_EQUAL(instance.write(from.data(), 2), 0);
}

BOOST_AUTO_TEST_CASE(device__write__zero__zero)
{
    const std::string from{ "a" };
    device_accessor instance;
    BOOST_REQUIRE_EQUAL(instance.write(from.data(), 0), 0);
}

BOOST_AUTO_TEST_CASE(device__write__one__zero)
{
    const std::string from{ "a" };
    device_accessor instance;
    BOOST_REQUIRE_EQUAL(instance.write(from.data(), 1), 0);
}

// read (reading from the sink)

BOOST_AUTO_TEST_CASE(device__read__nullptr__false)
{
    device_accessor instance;
    BOOST_REQUIRE_EQUAL(instance.read(nullptr, 0), -1);
}

// zero-size array .data is null pointer.
BOOST_AUTO_TEST_CASE(device__read__empty__false)
{
    std::array<char, 0> to;
    device_accessor instance;
    BOOST_REQUIRE_EQUAL(instance.read(to.data(), 0), -1);
}

BOOST_AUTO_TEST_CASE(device__read__negative__false)
{
    std::array<char, 1> to;
    device_accessor instance;
    BOOST_REQUIRE_EQUAL(instance.read(to.data(), -1), -1);
}

BOOST_AUTO_TEST_CASE(device__read__past_end__zero)
{
    std::array<char, 2> to;
    device_accessor instance;
    BOOST_REQUIRE_EQUAL(instance.read(to.data(), 2), 0);
}

BOOST_AUTO_TEST_CASE(device__read__zero__zero)
{
    std::array<char, 1> to;
    device_accessor instance;
    BOOST_REQUIRE_EQUAL(instance.read(to.data(), 0), 0);
}

BOOST_AUTO_TEST_CASE(device__read__one__zero)
{
    std::array<char, 1> to;
    device_accessor instance;
    BOOST_REQUIRE_EQUAL(instance.read(to.data(), 1), 0);
}

BOOST_AUTO_TEST_SUITE_END()
