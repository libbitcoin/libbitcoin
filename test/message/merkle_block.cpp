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

BOOST_AUTO_TEST_SUITE(merkle_block_tests)

BOOST_AUTO_TEST_CASE(merkle_block__constructor_1__always__invalid)
{
    const message::merkle_block instance;
    BOOST_REQUIRE(!instance.is_valid());
}

BOOST_AUTO_TEST_CASE(merkle_block__constructor_2__always__equals_params)
{
    const chain::header header(
        10,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234,
        6523454,
        68644
    );

    const size_t count = 1234u;

    const hash_list hashes
    {
        hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaffffffffffffffffffffffffffffffff"),
        hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"),
        hash_literal("ccccccccccccccccccccccccccccccccdddddddddddddddddddddddddddddddd"),
    };
    const data_chunk flags{ 0xae, 0x56, 0x0f };

    message::merkle_block instance(header, count, hashes, flags);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(header == instance.header());
    BOOST_REQUIRE_EQUAL(instance.total_transactions(), count);
    BOOST_REQUIRE(hashes == instance.hashes());
    BOOST_REQUIRE(flags == instance.flags());
}

BOOST_AUTO_TEST_CASE(merkle_block__constructor_3__always__equals_params)
{
    const message::merkle_block instance(
        chain::header
        {
            10,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234,
            6523454,
            68644
        },
        1234u,
        {
            hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaffffffffffffffffffffffffffffffff"),
            hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"),
            hash_literal("ccccccccccccccccccccccccccccccccdddddddddddddddddddddddddddddddd"),
        },
        { 0xae, 0x56, 0x0f });

    BOOST_REQUIRE(instance.is_valid());
}

BOOST_AUTO_TEST_CASE(merkle_block__constructor_4__always__equals_params)
{
    const message::merkle_block expected(
        chain::header
        {
            10,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234,
            6523454,
            68644
        },
        4321234u,
        {
            hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaffffffffffffffffffffffffffffffff"),
            hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"),
            hash_literal("ccccccccccccccccccccccccccccccccdddddddddddddddddddddddddddddddd"),
        },
        { 0xae, 0x56, 0x0f }
    );

    message::merkle_block instance(expected);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(expected == instance);
}

BOOST_AUTO_TEST_CASE(merkle_block__constructor_5__always__equals_params)
{
    const chain::header header(
        10,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234,
        6523454,
        68644
    );

    const size_t count = 654576u;

    const hash_list hashes
    {
        hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaffffffffffffffffffffffffffffffff"),
        hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"),
        hash_literal("ccccccccccccccccccccccccccccccccdddddddddddddddddddddddddddddddd"),
    };
    const data_chunk flags{ 0xae, 0x56, 0x0f };

    message::merkle_block expected(header, count, hashes, flags);
    message::merkle_block instance(std::move(expected));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(header == instance.header());
    BOOST_REQUIRE_EQUAL(instance.total_transactions(), count);
    BOOST_REQUIRE(hashes == instance.hashes());
    BOOST_REQUIRE(flags == instance.flags());
}

BOOST_AUTO_TEST_CASE(from_data_insufficient_data_fails)
{
    const data_chunk data{ 10 };
    message::merkle_block instance;

    BOOST_REQUIRE(!instance.from_data(message::version::level::maximum, data));
    BOOST_REQUIRE(!instance.is_valid());
}

BOOST_AUTO_TEST_CASE(from_data_insufficient_version_fails)
{
    const message::merkle_block expected
    {
        {
            10,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234,
            6523454,
            68644
        },
        34523u,
        {
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b")
        },
        {
            0x00
        }
    };

    const auto data = expected.to_data(message::version::level::maximum);
    message::merkle_block instance;

    BOOST_REQUIRE(!instance.from_data(
        message::merkle_block::version_minimum - 1, data));
    BOOST_REQUIRE(!instance.is_valid());
}

BOOST_AUTO_TEST_CASE(roundtrip_to_data_factory_chunk)
{
    const message::merkle_block expected
    {
        {
            10,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234,
            6523454,
            68644
        },
        45633u,
        {
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b")
        },
        {
            0x00
        }
    };

    const auto data = expected.to_data(message::version::level::maximum);
    const auto result = message::merkle_block::factory(message::version::level::maximum, data);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
}

BOOST_AUTO_TEST_CASE(roundtrip_to_data_factory_stream)
{
    const  message::merkle_block expected
    {
        {
            10,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234,
            6523454,
            68644
        },
        543563u,
        {
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b")
        },
        {
            0x00
        }
    };

    const auto data = expected.to_data(message::version::level::maximum);
    stream::in::copy istream(data);
    const auto result = message::merkle_block::factory(message::version::level::maximum, istream);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
}

BOOST_AUTO_TEST_CASE(roundtrip_to_data_factory_reader)
{
    const message::merkle_block expected
    {
        {
            10,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234,
            6523454,
            68644
        },
        5324u,
        {
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b")
        },
        {
            0x00
        }
    };

    const auto data = expected.to_data(message::version::level::maximum);
    read::bytes::copy source(data);
    const auto result = message::merkle_block::factory(message::version::level::maximum, source);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
}

BOOST_AUTO_TEST_CASE(merkle_block__header_accessor_1__always__returns_initialized_value)
{
    const chain::header expected
    {
        10,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234,
        6523454,
        68644
    };

    const message::merkle_block instance(
        expected,
        753u,
        {
            hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaffffffffffffffffffffffffffffffff"),
            hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"),
            hash_literal("ccccccccccccccccccccccccccccccccdddddddddddddddddddddddddddddddd"),
        },
        { 0xae, 0x56, 0x0f });

    BOOST_REQUIRE(expected == instance.header());
}

BOOST_AUTO_TEST_CASE(merkle_block__header_accessor_2__always__returns_initialized_value)
{
    const chain::header expected
    {
        10,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234,
        6523454,
        68644
    };

    const message::merkle_block instance(
        expected,
        9542u,
        {
            hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaffffffffffffffffffffffffffffffff"),
            hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"),
            hash_literal("ccccccccccccccccccccccccccccccccdddddddddddddddddddddddddddddddd"),
        },
        { 0xae, 0x56, 0x0f });

    BOOST_REQUIRE(expected == instance.header());
}

BOOST_AUTO_TEST_CASE(merkle_block__header_setter_1__roundtrip__success)
{
    const chain::header expected
    {
        10,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234,
        6523454,
        68644
    };

    message::merkle_block instance;
    BOOST_REQUIRE(expected != instance.header());
    instance.set_header(expected);
    BOOST_REQUIRE(expected == instance.header());
}

BOOST_AUTO_TEST_CASE(merkle_block__header_setter_2__roundtrip__success)
{
    message::merkle_block instance;
    BOOST_REQUIRE(!instance.header().is_valid());
    instance.set_header(
        chain::header
        {
            10,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234,
            6523454,
            68644
        });

    BOOST_REQUIRE(instance.header().is_valid());
}

BOOST_AUTO_TEST_CASE(merkle_block__hashes_accessor_1__always__returns_initialized_value)
{
    const hash_list expected
    {
        hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaffffffffffffffffffffffffffffffff"),
        hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"),
        hash_literal("ccccccccccccccccccccccccccccccccdddddddddddddddddddddddddddddddd"),
    };

    const message::merkle_block instance(
        chain::header
        {
            10,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234,
            6523454,
            68644
        },
        2456u,
        expected,
        { 0xae, 0x56, 0x0f });

    BOOST_REQUIRE(expected == instance.hashes());
}

BOOST_AUTO_TEST_CASE(merkle_block__hashes_accessor_2__always__returns_initialized_value)
{
    const hash_list expected
    {
        hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaffffffffffffffffffffffffffffffff"),
        hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"),
        hash_literal("ccccccccccccccccccccccccccccccccdddddddddddddddddddddddddddddddd"),
    };

    const message::merkle_block instance(
        chain::header
        {
            10,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234,
            6523454,
            68644
        },
        9137u,
        expected,
        { 0xae, 0x56, 0x0f });

    BOOST_REQUIRE(expected == instance.hashes());
}

BOOST_AUTO_TEST_CASE(merkle_block__hashes_setter_1__roundtrip__success)
{
    const hash_list expected
    {
        hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaffffffffffffffffffffffffffffffff"),
        hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"),
        hash_literal("ccccccccccccccccccccccccccccccccdddddddddddddddddddddddddddddddd"),
    };

    message::merkle_block instance;
    BOOST_REQUIRE(expected != instance.hashes());
    instance.set_hashes(expected);
    BOOST_REQUIRE(expected == instance.hashes());
}

BOOST_AUTO_TEST_CASE(merkle_block__hashes_setter_2__roundtrip__success)
{
    message::merkle_block instance;
    BOOST_REQUIRE(instance.hashes().empty());
    instance.set_hashes(hash_list
    {
        hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaffffffffffffffffffffffffffffffff"),
        hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"),
        hash_literal("ccccccccccccccccccccccccccccccccdddddddddddddddddddddddddddddddd"),
    });

    BOOST_REQUIRE(!instance.hashes().empty());
}

BOOST_AUTO_TEST_CASE(merkle_block__flags_accessor_1__always__returns_initialized_value)
{
    const data_chunk expected{ 0xae, 0x56, 0x0f };

    const message::merkle_block instance(
        chain::header
        {
            10,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234,
            6523454,
            68644
        },
        8264u,
        {
            hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaffffffffffffffffffffffffffffffff"),
            hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"),
            hash_literal("ccccccccccccccccccccccccccccccccdddddddddddddddddddddddddddddddd"),
        },
        expected);

    BOOST_REQUIRE(expected == instance.flags());
}

BOOST_AUTO_TEST_CASE(merkle_block__flags_accessor_2__always__returns_initialized_value)
{
    const data_chunk expected{ 0xae, 0x56, 0x0f };

    const message::merkle_block instance(
        chain::header{
            10,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234,
            6523454,
            68644
        },
        6428u,
        {
            hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaffffffffffffffffffffffffffffffff"),
            hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"),
            hash_literal("ccccccccccccccccccccccccccccccccdddddddddddddddddddddddddddddddd"),
        },
        expected);

    BOOST_REQUIRE(expected == instance.flags());
}

BOOST_AUTO_TEST_CASE(merkle_block__flags_setter_1__roundtrip__success)
{
    const data_chunk expected{ 0xae, 0x56, 0x0f };
    message::merkle_block instance;
    BOOST_REQUIRE(expected != instance.flags());
    instance.set_flags(expected);
    BOOST_REQUIRE(expected == instance.flags());
}

BOOST_AUTO_TEST_CASE(merkle_block__flags_setter_2__roundtrip__success)
{
    message::merkle_block instance;
    BOOST_REQUIRE(instance.flags().empty());
    instance.set_flags(data_chunk{ 0xae, 0x56, 0x0f });
    BOOST_REQUIRE(!instance.flags().empty());
}

BOOST_AUTO_TEST_CASE(merkle_block__operator_assign_equals__always__matches_equivalent)
{
    message::merkle_block value(
        chain::header
        {
            10,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234,
            6523454,
            68644
        },
        3197u,
        {
            hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaffffffffffffffffffffffffffffffff"),
            hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"),
            hash_literal("ccccccccccccccccccccccccccccccccdddddddddddddddddddddddddddddddd"),
        },
        { 0xae, 0x56, 0x0f }
    );

    BOOST_REQUIRE(value.is_valid());

    message::merkle_block instance;
    BOOST_REQUIRE(!instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
}

BOOST_AUTO_TEST_CASE(merkle_block__operator_boolean_equals__duplicates__returns_true)
{
    const message::merkle_block expected(
        chain::header
        {
            10,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234,
            6523454,
            68644
        },
        9821u,
        {
            hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaffffffffffffffffffffffffffffffff"),
            hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"),
            hash_literal("ccccccccccccccccccccccccccccccccdddddddddddddddddddddddddddddddd"),
        },
        { 0xae, 0x56, 0x0f }
    );

    message::merkle_block instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(merkle_block__operator_boolean_equals__differs__returns_false)
{
    const message::merkle_block expected(
        chain::header
        {
            10,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234,
            6523454,
            68644
        },
        1469u,
        {
            hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaffffffffffffffffffffffffffffffff"),
            hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"),
            hash_literal("ccccccccccccccccccccccccccccccccdddddddddddddddddddddddddddddddd"),
        },
        { 0xae, 0x56, 0x0f }
    );

    message::merkle_block instance;
    BOOST_REQUIRE(!(instance == expected));
}

BOOST_AUTO_TEST_CASE(merkle_block__operator_boolean_not_equals__duplicates__returns_false)
{
    const message::merkle_block expected(
        chain::header
        {
            10,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234,
            6523454,
            68644
        },
        3524u,
        {
            hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaffffffffffffffffffffffffffffffff"),
            hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"),
            hash_literal("ccccccccccccccccccccccccccccccccdddddddddddddddddddddddddddddddd"),
        },
        { 0xae, 0x56, 0x0f }
    );

    message::merkle_block instance(expected);
    BOOST_REQUIRE(!(instance != expected));
}

BOOST_AUTO_TEST_CASE(merkle_block__operator_boolean_not_equals__differs__returns_true)
{
    const message::merkle_block expected(
        chain::header
        {
            10,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234,
            6523454,
            68644
        },
        8642u,
        {
            hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaffffffffffffffffffffffffffffffff"),
            hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"),
            hash_literal("ccccccccccccccccccccccccccccccccdddddddddddddddddddddddddddddddd"),
        },
        { 0xae, 0x56, 0x0f }
    );

    message::merkle_block instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()
