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

using namespace bc::system::message;

BOOST_AUTO_TEST_SUITE(message_block_tests)

BOOST_AUTO_TEST_CASE(block__constructor_1__always__invalid)
{
    block instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());
}

BOOST_AUTO_TEST_CASE(block__constructor_2__always__equals_params)
{
    const chain::header header(10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    const chain::transaction::list transactions
    {
        chain::transaction(1, 48, {}, {}),
        chain::transaction(2, 32, {}, {}),
        chain::transaction(4, 16, {}, {})
    };

    block instance(header, transactions);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(header == instance.header());
    BOOST_REQUIRE(transactions == instance.transactions());
}

BOOST_AUTO_TEST_CASE(block__constructor_3__always__equals_params)
{
    const chain::header header(10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    const chain::transaction::list transactions
    {
        chain::transaction(1, 48, {}, {}),
        chain::transaction(2, 32, {}, {}),
        chain::transaction(4, 16, {}, {})
    };

    chain::header dup_header(header);
    chain::transaction::list dup_transactions = transactions;
    block instance(std::move(dup_header), std::move(dup_transactions));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(header == instance.header());
    BOOST_REQUIRE(transactions == instance.transactions());

}

BOOST_AUTO_TEST_CASE(block__constructor_4__always__equals_params)
{
    const chain::header header(10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    const chain::transaction::list transactions
    {
        chain::transaction(1, 48, {}, {}),
        chain::transaction(2, 32, {}, {}),
        chain::transaction(4, 16, {}, {})
    };

    chain::block value(header, transactions);
    block instance(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(instance == value);
    BOOST_REQUIRE(header == instance.header());
    BOOST_REQUIRE(transactions == instance.transactions());
}

BOOST_AUTO_TEST_CASE(block__constructor_5__always__equals_params)
{
    const chain::header header(10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    const chain::transaction::list transactions
    {
        chain::transaction(1, 48, {}, {}),
        chain::transaction(2, 32, {}, {}),
        chain::transaction(4, 16, {}, {})
    };

    chain::block value(header, transactions);
    block instance(std::move(value));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(header == instance.header());
    BOOST_REQUIRE(transactions == instance.transactions());
}

BOOST_AUTO_TEST_CASE(block__constructor_6__always__equals_params)
{
    const chain::header header(10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    const chain::transaction::list transactions
    {
        chain::transaction(1, 48, {}, {}),
        chain::transaction(2, 32, {}, {}),
        chain::transaction(4, 16, {}, {})
    };

    block value(header, transactions);
    block instance(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(value == instance);
    BOOST_REQUIRE(header == instance.header());
    BOOST_REQUIRE(transactions == instance.transactions());
}

BOOST_AUTO_TEST_CASE(block__constructor_7__always__equals_params)
{
    const chain::header header(10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    const chain::transaction::list transactions
    {
        chain::transaction(1, 48, {}, {}),
        chain::transaction(2, 32, {}, {}),
        chain::transaction(4, 16, {}, {})
    };

    block value(header, transactions);
    block instance(std::move(value));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(header == instance.header());
    BOOST_REQUIRE(transactions == instance.transactions());
}

BOOST_AUTO_TEST_CASE(block__factory_data_1__genesis_mainnet__success)
{
    const chain::block genesis = settings(chain::selection::mainnet).genesis_block;
    BOOST_REQUIRE_EQUAL(genesis.serialized_size(), 285u);
    BOOST_REQUIRE_EQUAL(genesis.header().serialized_size(), 80u);

    // Save genesis block.
    auto raw_block = genesis.to_data();
    BOOST_REQUIRE_EQUAL(raw_block.size(), 285u);

    // Reload genesis block.
    const auto block = block::factory(version::level::minimum, raw_block);

    BOOST_REQUIRE(block.is_valid());
    BOOST_REQUIRE(genesis.header() == block.header());

    // Verify merkle root from transactions.
    BOOST_REQUIRE(block.generate_merkle_root() == genesis.header().merkle_root());

    auto raw_reserialization = block.to_data(version::level::minimum);
    BOOST_REQUIRE(raw_reserialization == raw_block);
    BOOST_REQUIRE_EQUAL(raw_reserialization.size(), block.serialized_size(version::level::minimum));
}

BOOST_AUTO_TEST_CASE(block__factory_data_2__genesis_mainnet__success)
{
    const chain::block genesis = settings(chain::selection::mainnet).genesis_block;
    BOOST_REQUIRE_EQUAL(genesis.serialized_size(), 285u);
    BOOST_REQUIRE_EQUAL(genesis.header().serialized_size(), 80u);

    // Save genesis block.
    auto raw_block = genesis.to_data();
    BOOST_REQUIRE_EQUAL(raw_block.size(), 285u);

    // Reload genesis block.
    stream::in::copy stream(raw_block);
    const auto block = block::factory(version::level::minimum, stream);

    BOOST_REQUIRE(block.is_valid());
    BOOST_REQUIRE(genesis.header() == block.header());

    // Verify merkle root from transactions.
    BOOST_REQUIRE(block.generate_merkle_root() == genesis.header().merkle_root());

    data_chunk raw_reserialization;
    stream::out::data sink(raw_reserialization);
    block.to_data(version::level::minimum, sink);
    sink.flush();
    BOOST_REQUIRE(raw_reserialization == raw_block);
    BOOST_REQUIRE_EQUAL(raw_reserialization.size(), block.serialized_size(version::level::minimum));
}

BOOST_AUTO_TEST_CASE(block__factory_data_3__genesis_mainnet__success)
{
    const chain::block genesis = settings(chain::selection::mainnet).genesis_block;
    BOOST_REQUIRE_EQUAL(genesis.serialized_size(), 285u);
    BOOST_REQUIRE_EQUAL(genesis.header().serialized_size(), 80u);

    // Save genesis block.
    auto raw_block = genesis.to_data();
    BOOST_REQUIRE_EQUAL(raw_block.size(), 285u);

    // Reload genesis block.
    stream::in::copy stream(raw_block);
    read::bytes::istream reader(stream);
    const auto block = block::factory(version::level::minimum + 1, reader);

    BOOST_REQUIRE(block.is_valid());
    BOOST_REQUIRE(genesis.header() == block.header());

    // Verify merkle root from transactions.
    BOOST_REQUIRE(block.generate_merkle_root() == genesis.header().merkle_root());

    data_chunk raw_reserialization;
    write::bytes::data out(raw_reserialization);
    block.to_data(version::level::minimum, out);
    out.flush();
    BOOST_REQUIRE(raw_reserialization == raw_block);
    BOOST_REQUIRE_EQUAL(raw_reserialization.size(), block.serialized_size(version::level::minimum));
}

BOOST_AUTO_TEST_CASE(block__operator_assign_equals_1__always__matches_equivalent)
{
    const chain::header header(10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    const chain::transaction::list transactions
    {
        chain::transaction(1, 48, {}, {}),
        chain::transaction(2, 32, {}, {}),
        chain::transaction(4, 16, {}, {})
    };

    chain::block value(header, transactions);
    BOOST_REQUIRE(value.is_valid());

    message::block instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(instance.header() == header);
    BOOST_REQUIRE(instance.transactions() == transactions);
}

BOOST_AUTO_TEST_CASE(block__operator_assign_equals_2__always__matches_equivalent)
{
    const chain::header header(10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    const chain::transaction::list transactions
    {
        chain::transaction(1, 48, {}, {}),
        chain::transaction(2, 32, {}, {}),
        chain::transaction(4, 16, {}, {})
    };

    message::block value(header, transactions);

    BOOST_REQUIRE(value.is_valid());

    message::block instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(instance.header() == header);
    BOOST_REQUIRE(instance.transactions() == transactions);
}

BOOST_AUTO_TEST_CASE(block__operator_boolean_equals_1__duplicates__returns_true)
{
    const chain::block expected(
        chain::header(10u,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234u,
            6523454u,
            68644u),
        {
            chain::transaction(1, 48, {}, {}),
            chain::transaction(2, 32, {}, {}),
            chain::transaction(4, 16, {}, {})
        });

    message::block instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(block__operator_boolean_equals_1__differs__returns_false)
{
    const chain::block expected(
        chain::header(10u,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234u,
            6523454u,
            68644u),
        {
            chain::transaction(1, 48, {}, {}),
            chain::transaction(2, 32, {}, {}),
            chain::transaction(4, 16, {}, {})
        });


    message::block instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(block__operator_boolean_not_equals_1__duplicates__returns_false)
{
    const chain::block expected(
        chain::header(10u,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234u,
            6523454u,
            68644u),
        {
            chain::transaction(1, 48, {}, {}),
            chain::transaction(2, 32, {}, {}),
            chain::transaction(4, 16, {}, {})
        });

    message::block instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(block__operator_boolean_not_equals_1__differs__returns_true)
{
    const chain::block expected(
        chain::header(10u,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234u,
            6523454u,
            68644u),
        {
            chain::transaction(1, 48, {}, {}),
            chain::transaction(2, 32, {}, {}),
            chain::transaction(4, 16, {}, {})
        });


    chain::block instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_CASE(block__operator_boolean_equals_2__duplicates__returns_true)
{
    const message::block expected(
        chain::header(10u,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234u,
            6523454u,
            68644u),
        {
            chain::transaction(1, 48, {}, {}),
            chain::transaction(2, 32, {}, {}),
            chain::transaction(4, 16, {}, {})
        });

    message::block instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(block__operator_boolean_equals_2__differs__returns_false)
{
    const message::block expected(
        chain::header(10u,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234u,
            6523454u,
            68644u),
        {
            chain::transaction(1, 48, {}, {}),
            chain::transaction(2, 32, {}, {}),
            chain::transaction(4, 16, {}, {})
        });


    message::block instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(block__operator_boolean_not_equals_2__duplicates__returns_false)
{
    const message::block expected(
        chain::header(10u,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234u,
            6523454u,
            68644u),
        {
            chain::transaction(1, 48, {}, {}),
            chain::transaction(2, 32, {}, {}),
            chain::transaction(4, 16, {}, {})
        });

    message::block instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(block__operator_boolean_not_equals_2__differs__returns_true)
{
    const message::block expected(
        chain::header(10u,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234u,
            6523454u,
            68644u),
        {
            chain::transaction(1, 48, {}, {}),
            chain::transaction(2, 32, {}, {}),
            chain::transaction(4, 16, {}, {})
        });


    message::block instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()
