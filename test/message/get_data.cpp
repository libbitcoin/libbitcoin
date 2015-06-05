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

BOOST_AUTO_TEST_SUITE(get_data_tests)

BOOST_AUTO_TEST_CASE(from_data_insufficient_bytes_failure)
{
    data_chunk raw = { 0xab, 0xcd };
    message::get_data instance;

    BOOST_REQUIRE_EQUAL(false, instance.from_data(raw));
}

BOOST_AUTO_TEST_CASE(roundtrip_to_data_factory_from_data_chunk)
{
    message::get_data expected;
    expected.inventories = {
        message::inventory_vector{
            message::inventory_type_id::error,
            hash_digest{
                0x44, 0x9a, 0x0d, 0x24, 0x9a, 0xd5, 0x39, 0x89,
                0xbb, 0x85, 0x0a, 0x3d, 0x79, 0x24, 0xed, 0x0f,
                0xc3, 0x0d, 0x6f, 0x55, 0x7d, 0x71, 0x12, 0x1a,
                0x37, 0xc0, 0xb0, 0x32, 0xf0, 0xd6, 0x6e, 0xdf
            }
        }
    };

    data_chunk data = expected.to_data();

    auto result = message::get_data::factory_from_data(data);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.satoshi_size());
    BOOST_REQUIRE_EQUAL(expected.satoshi_size(), result.satoshi_size());
}

BOOST_AUTO_TEST_CASE(roundtrip_to_data_factory_from_data_stream)
{
    message::get_data expected;
    expected.inventories = {
        message::inventory_vector{
            message::inventory_type_id::transaction,
            hash_digest{
                0x44, 0x9a, 0x0d, 0xee, 0x9a, 0xd5, 0x39, 0xee,
                0xee, 0x85, 0x0a, 0x3d, 0xee, 0x24, 0xed, 0x0f,
                0xc3, 0xee, 0x6f, 0x55, 0x7d, 0xee, 0x12, 0x1a,
                0x37, 0xc0, 0xee, 0x32, 0xf0, 0xd6, 0xee, 0xdf
            }
        }
    };

    data_chunk data = expected.to_data();
    boost::iostreams::stream<byte_source<data_chunk>> istream(data);

    auto result = message::get_data::factory_from_data(istream);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.satoshi_size());
    BOOST_REQUIRE_EQUAL(expected.satoshi_size(), result.satoshi_size());
}

BOOST_AUTO_TEST_CASE(roundtrip_to_data_factory_from_data_reader)
{
    message::get_data expected;
    expected.inventories = {
        message::inventory_vector{
            message::inventory_type_id::block,
            hash_digest{
                0x66, 0x9a, 0x0d, 0x24, 0x66, 0xd5, 0x39, 0x89,
                0xbb, 0x66, 0x0a, 0x3d, 0x79, 0x66, 0xed, 0x0f,
                0xc3, 0x0d, 0x66, 0x55, 0x7d, 0x71, 0x66, 0x1a,
                0x37, 0xc0, 0xb0, 0x66, 0xf0, 0xd6, 0x6e, 0x66
            }
        }
    };

    data_chunk data = expected.to_data();
    boost::iostreams::stream<byte_source<data_chunk>> istream(data);
    istream_reader source(istream);

    auto result = message::get_data::factory_from_data(source);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.satoshi_size());
    BOOST_REQUIRE_EQUAL(expected.satoshi_size(), result.satoshi_size());
}

BOOST_AUTO_TEST_SUITE_END()
