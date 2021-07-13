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

BOOST_AUTO_TEST_SUITE(send_headers_tests)

BOOST_AUTO_TEST_CASE(roundtrip_to_data_factory_chunk)
{
    const message::send_headers expected{};
    const auto data = expected.to_data(message::version::level::maximum);
    const auto result = message::send_headers::factory(
        message::version::level::maximum, data);

    BOOST_REQUIRE_EQUAL(0u, data.size());
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE_EQUAL(0u, result.serialized_size(message::version::level::maximum));
}

BOOST_AUTO_TEST_CASE(roundtrip_to_data_factory_stream)
{
    const message::send_headers expected{};
    const auto data = expected.to_data(message::version::level::maximum);
    stream::in::copy istream(data);
    const auto result = message::send_headers::factory(
        message::version::level::maximum, istream);

    BOOST_REQUIRE_EQUAL(0u, data.size());
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE_EQUAL(0u, result.serialized_size(message::version::level::maximum));
}

BOOST_AUTO_TEST_CASE(roundtrip_to_data_factory_reader)
{
    const message::send_headers expected{};
    const auto data = expected.to_data(message::version::level::maximum);
    read::bytes::copy source(data);
    const auto result = message::send_headers::factory(
        message::version::level::maximum, source);

    BOOST_REQUIRE_EQUAL(0u, data.size());
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE_EQUAL(0u, result.serialized_size(message::version::level::maximum));
}

BOOST_AUTO_TEST_CASE(from_data_reader_version_prior_bip130_failure)
{
    data_chunk data{};
    read::bytes::copy source(data);
    message::send_headers instance{};
    const auto result = instance.from_data(message::version::level::bip130 - 1, source);

    BOOST_REQUIRE(!result);
    BOOST_REQUIRE(!instance.is_valid());
}

BOOST_AUTO_TEST_CASE(from_data_reader_version_at_least_bip130_success)
{
    data_chunk data{};
    read::bytes::copy source(data);
    message::send_headers instance{};
    const auto result = instance.from_data(message::version::level::bip130, source);

    BOOST_REQUIRE(result);
    BOOST_REQUIRE(instance.is_valid());
}

BOOST_AUTO_TEST_SUITE_END()
