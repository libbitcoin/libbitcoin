/*
 * Copyright (c) 2011-2013 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <boost/test/unit_test.hpp>
#include <boost/iostreams/stream.hpp>
#include <bitcoin/bitcoin.hpp>

using namespace bc;

BOOST_AUTO_TEST_SUITE(nonce_tests)

BOOST_AUTO_TEST_CASE(nonce_from_data_insufficient_bytes_failure)
{
    data_chunk raw(1);
    message::nonce_base instance;

    BOOST_REQUIRE_EQUAL(false, instance.from_data(raw));
}

BOOST_AUTO_TEST_CASE(ping_satoshi_fixed_size_returns_sizeof_uint64_t)
{
    BOOST_REQUIRE_EQUAL(sizeof(uint64_t), message::ping::satoshi_fixed_size());
}

BOOST_AUTO_TEST_CASE(ping_roundtrip_to_data_factory_from_data_chunk)
{
    message::ping expected;
    expected.nonce = 16545612u;

    data_chunk data = expected.to_data();

    auto result = message::ping::factory_from_data(data);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.satoshi_size());
    BOOST_REQUIRE_EQUAL(expected.satoshi_size(), result.satoshi_size());
}

BOOST_AUTO_TEST_CASE(ping_roundtrip_to_data_factory_from_data_stream)
{
    message::ping expected;
    expected.nonce = 5087222u;

    data_chunk data = expected.to_data();
    boost::iostreams::stream<byte_source<data_chunk>> istream(data);

    auto result = message::ping::factory_from_data(istream);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.satoshi_size());
    BOOST_REQUIRE_EQUAL(expected.satoshi_size(), result.satoshi_size());
}

BOOST_AUTO_TEST_CASE(ping_roundtrip_to_data_factory_from_data_reader)
{
    message::ping expected;
    expected.nonce = 6456147u;

    data_chunk data = expected.to_data();
    boost::iostreams::stream<byte_source<data_chunk>> istream(data);
    istream_reader source(istream);

    auto result = message::ping::factory_from_data(source);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.satoshi_size());
    BOOST_REQUIRE_EQUAL(expected.satoshi_size(), result.satoshi_size());
}

BOOST_AUTO_TEST_CASE(pong_satoshi_fixed_size_returns_sizeof_uint64_t)
{
    BOOST_REQUIRE_EQUAL(sizeof(uint64_t), message::pong::satoshi_fixed_size());
}

BOOST_AUTO_TEST_CASE(pong_roundtrip_to_data_factory_from_data_chunk)
{
    message::pong expected;
    expected.nonce = 4306550u;

    data_chunk data = expected.to_data();

    auto result = message::pong::factory_from_data(data);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.satoshi_size());
    BOOST_REQUIRE_EQUAL(expected.satoshi_size(), result.satoshi_size());
}

BOOST_AUTO_TEST_CASE(pong_roundtrip_to_data_factory_from_data_stream)
{
    message::pong expected;
    expected.nonce = 3100693u;

    data_chunk data = expected.to_data();
    boost::iostreams::stream<byte_source<data_chunk>> istream(data);

    auto result = message::pong::factory_from_data(istream);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.satoshi_size());
    BOOST_REQUIRE_EQUAL(expected.satoshi_size(), result.satoshi_size());
}

BOOST_AUTO_TEST_CASE(pong_roundtrip_to_data_factory_from_data_reader)
{
    message::pong expected;
    expected.nonce = 4642675u;

    data_chunk data = expected.to_data();
    boost::iostreams::stream<byte_source<data_chunk>> istream(data);
    istream_reader source(istream);

    auto result = message::pong::factory_from_data(source);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.satoshi_size());
    BOOST_REQUIRE_EQUAL(expected.satoshi_size(), result.satoshi_size());
}

BOOST_AUTO_TEST_SUITE_END()
