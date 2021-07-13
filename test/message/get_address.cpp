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

BOOST_AUTO_TEST_SUITE(get_address_tests)

BOOST_AUTO_TEST_CASE(roundtrip_to_data_factory_chunk)
{
    const message::get_address expected{};
    const auto data = expected.to_data(message::version::level::minimum);
    const auto result = message::get_address::factory(
        message::version::level::minimum, data);

    BOOST_REQUIRE_EQUAL(0u, data.size());
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE_EQUAL(0u, result.serialized_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_CASE(roundtrip_to_data_factory_stream)
{
    const message::get_address expected{};
    const auto data = expected.to_data(message::version::level::minimum);
    stream::in::copy istream(data);
    const auto result = message::get_address::factory(
        message::version::level::minimum, istream);

    BOOST_REQUIRE_EQUAL(0u, data.size());
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE_EQUAL(0u, result.serialized_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_CASE(roundtrip_to_data_factory_reader)
{
    const message::get_address expected{};
    const auto data = expected.to_data(message::version::level::minimum);
    read::bytes::copy source(data);
    const auto result = message::get_address::factory(
        message::version::level::minimum, source);

    BOOST_REQUIRE_EQUAL(0u, data.size());
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE_EQUAL(0u, result.serialized_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_SUITE_END()
