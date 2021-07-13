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

BOOST_AUTO_TEST_SUITE(block_transactions_tests)

BOOST_AUTO_TEST_CASE(block_transactions__constructor_1__always__invalid)
{
    message::block_transactions instance;
    BOOST_REQUIRE(!instance.is_valid());
}

BOOST_AUTO_TEST_CASE(block_transactions__constructor_2__always__equals_params)
{
    const auto hash = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    const chain::transaction::list transactions
    {
        {
            chain::transaction(1, 48, {}, {}),
            chain::transaction(2, 32, {}, {}),
            chain::transaction(4, 16, {}, {})
        }
    };

    message::block_transactions instance(hash, transactions);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(hash == instance.block_hash());
    BOOST_REQUIRE(transactions == instance.transactions());
}

BOOST_AUTO_TEST_CASE(block_transactions__constructor_3__always__equals_params)
{
    const auto hash = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    const chain::transaction::list transactions
    {
        {
            chain::transaction(1, 48, {}, {}),
            chain::transaction(2, 32, {}, {}),
            chain::transaction(4, 16, {}, {})
        }
    };

    auto dup_hash = hash;
    chain::transaction::list dup_transactions = transactions;
    message::block_transactions instance(std::move(dup_hash), std::move(dup_transactions));

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(hash == instance.block_hash());
    BOOST_REQUIRE(transactions == instance.transactions());
}

BOOST_AUTO_TEST_CASE(block_transactions__constructor_4__always__equals_params)
{
    const auto hash = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    const chain::transaction::list transactions
    {
        {
            chain::transaction(1, 48, {}, {}),
            chain::transaction(2, 32, {}, {}),
            chain::transaction(4, 16, {}, {})
        }
    };

    message::block_transactions value(hash, transactions);
    message::block_transactions instance(value);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(value == instance);
    BOOST_REQUIRE(hash == instance.block_hash());
    BOOST_REQUIRE(transactions == instance.transactions());
}

BOOST_AUTO_TEST_CASE(block_transactions__constructor_5__always__equals_params)
{
    const auto hash = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    const chain::transaction::list transactions
    {
        {
            chain::transaction(1, 48, {}, {}),
            chain::transaction(2, 32, {}, {}),
            chain::transaction(4, 16, {}, {})
        }
    };

    message::block_transactions value(hash, transactions);
    message::block_transactions instance(std::move(value));

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(hash == instance.block_hash());
    BOOST_REQUIRE(transactions == instance.transactions());
}

BOOST_AUTO_TEST_CASE(block_transactions__from_data__insufficient_bytes__failure)
{
    const data_chunk raw{ 0xab, 0xcd };
    message::block_transactions instance{};

    BOOST_REQUIRE_EQUAL(false, instance.from_data(message::block_transactions::version_minimum, raw));
}

BOOST_AUTO_TEST_CASE(block_transactions__from_data__insufficient_transaction_bytes__failure)
{
    const auto raw = to_chunk(base16_literal(
        "3ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a0"
        "20100000001f08e44a96bfb5ae63eda1a6620adae37ee37ee4777fb0336e1bbbc"
        "4de65310fc010000006a473044022050d8368cacf9bf1b8fb1f7cfd9aff632947"
        "89eb1760139e7ef41f083726dadc4022067796354aba8f2e02363c5e510aa7e28"
        "30b115472fb31de67d16972867f13945012103e589480b2f746381fca01a9b12c"
        "517b7a482a203c8b2742985da0ac72cc078f2ffffffff02f0c9c4670000000019"
        "76a914d9d78e26df4e4601cf9b26d09c7b280ee764469f88ac80c4600f0000000"
        "01976a9141ee32412020a324b93b1a1acfdfff6ab9ca8fac288ac000000000100"
        "00000364e62ad837f29617bafeae951776e7a6b3019b2da37827921548d1a5efc"
        "f9e5c010000006b48304502204df0dc9b7f61fbb2e4c8b0e09f3426d625a0191e"
        "56c48c338df3214555180eaf022100f21ac1f632201154f3c69e1eadb59901a34"
        "c40f1127e96adc31fac6ae6b11fb4012103893d5a06201d5cf61400e96fa4a751"
        "4fc12ab45166ace618d68b8066c9c585f9ffffffff54b755c39207d443fd96a8d"
        "12c94446a1c6f66e39c95e894c23418d7501f681b010000006b48304502203267"
        "910f55f2297360198fff57a3631be850965344370f732950b4779573787502210"
        "0f7da90b82d24e6e957264b17d3e5042bab8946ee5fc676d15d915da450151d36"
        "012103893d5a06201d5cf61400e96fa4a7514fc12ab45166ace618d68b8066c9c"
        "585f9ffffffff0aa14d394a1f0eaf0c4496537f8ab9246d9663e26acb5f308fcc"
        "c734b748cc9c010000006c493046022100d64ace8ec2d5feeb3e868e82b894202"
        "db8cb683c414d806b343d02b7ac679de7022100a2dcd39940dd28d4e22cce417a"
        "0829c1b516c471a3d64d11f2c5d754108bdc0b012103893d5a06201d5cf61400e"
        "96fa4a7514fc12ab45166ace618d68b8066c9c585f9ffffffff02c0e1e4000000"
        "00001976a914884c09d7e1f6420976c40e040c30b2b62210c3d488ac203005000"
        "00000001976a914905f933de850988603aafeeb2fd7fce61e66fe5d88ac000000"));

    message::block_transactions instance{};

    BOOST_REQUIRE(!instance.from_data(message::block_transactions::version_minimum, raw));
}

BOOST_AUTO_TEST_CASE(block_transactions__from_data__insufficient_version__failure)
{
    const auto raw = to_chunk(base16_literal(
        "3ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a0"
        "20100000001f08e44a96bfb5ae63eda1a6620adae37ee37ee4777fb0336e1bbbc"
        "4de65310fc010000006a473044022050d8368cacf9bf1b8fb1f7cfd9aff632947"
        "89eb1760139e7ef41f083726dadc4022067796354aba8f2e02363c5e510aa7e28"
        "30b115472fb31de67d16972867f13945012103e589480b2f746381fca01a9b12c"
        "517b7a482a203c8b2742985da0ac72cc078f2ffffffff02f0c9c4670000000019"
        "76a914d9d78e26df4e4601cf9b26d09c7b280ee764469f88ac80c4600f0000000"
        "01976a9141ee32412020a324b93b1a1acfdfff6ab9ca8fac288ac000000000100"
        "00000364e62ad837f29617bafeae951776e7a6b3019b2da37827921548d1a5efc"
        "f9e5c010000006b48304502204df0dc9b7f61fbb2e4c8b0e09f3426d625a0191e"
        "56c48c338df3214555180eaf022100f21ac1f632201154f3c69e1eadb59901a34"
        "c40f1127e96adc31fac6ae6b11fb4012103893d5a06201d5cf61400e96fa4a751"
        "4fc12ab45166ace618d68b8066c9c585f9ffffffff54b755c39207d443fd96a8d"
        "12c94446a1c6f66e39c95e894c23418d7501f681b010000006b48304502203267"
        "910f55f2297360198fff57a3631be850965344370f732950b4779573787502210"
        "0f7da90b82d24e6e957264b17d3e5042bab8946ee5fc676d15d915da450151d36"
        "012103893d5a06201d5cf61400e96fa4a7514fc12ab45166ace618d68b8066c9c"
        "585f9ffffffff0aa14d394a1f0eaf0c4496537f8ab9246d9663e26acb5f308fcc"
        "c734b748cc9c010000006c493046022100d64ace8ec2d5feeb3e868e82b894202"
        "db8cb683c414d806b343d02b7ac679de7022100a2dcd39940dd28d4e22cce417a"
        "0829c1b516c471a3d64d11f2c5d754108bdc0b012103893d5a06201d5cf61400e"
        "96fa4a7514fc12ab45166ace618d68b8066c9c585f9ffffffff02c0e1e4000000"
        "00001976a914884c09d7e1f6420976c40e040c30b2b62210c3d488ac203005000"
        "00000001976a914905f933de850988603aafeeb2fd7fce61e66fe5d88ac000000"
        "00"));

    message::block_transactions expected;
    expected.from_data(message::block_transactions::version_minimum, raw);

    const auto data = expected.to_data(message::block_transactions::version_minimum);

    BOOST_REQUIRE(raw == data);
    message::block_transactions instance;
    BOOST_REQUIRE(!instance.from_data(message::block_transactions::version_minimum - 1, data));
}

BOOST_AUTO_TEST_CASE(block_transactions__factory_1__valid_input__success)
{
    const auto raw = to_chunk(base16_literal(
        "3ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a0"
        "20100000001f08e44a96bfb5ae63eda1a6620adae37ee37ee4777fb0336e1bbbc"
        "4de65310fc010000006a473044022050d8368cacf9bf1b8fb1f7cfd9aff632947"
        "89eb1760139e7ef41f083726dadc4022067796354aba8f2e02363c5e510aa7e28"
        "30b115472fb31de67d16972867f13945012103e589480b2f746381fca01a9b12c"
        "517b7a482a203c8b2742985da0ac72cc078f2ffffffff02f0c9c4670000000019"
        "76a914d9d78e26df4e4601cf9b26d09c7b280ee764469f88ac80c4600f0000000"
        "01976a9141ee32412020a324b93b1a1acfdfff6ab9ca8fac288ac000000000100"
        "00000364e62ad837f29617bafeae951776e7a6b3019b2da37827921548d1a5efc"
        "f9e5c010000006b48304502204df0dc9b7f61fbb2e4c8b0e09f3426d625a0191e"
        "56c48c338df3214555180eaf022100f21ac1f632201154f3c69e1eadb59901a34"
        "c40f1127e96adc31fac6ae6b11fb4012103893d5a06201d5cf61400e96fa4a751"
        "4fc12ab45166ace618d68b8066c9c585f9ffffffff54b755c39207d443fd96a8d"
        "12c94446a1c6f66e39c95e894c23418d7501f681b010000006b48304502203267"
        "910f55f2297360198fff57a3631be850965344370f732950b4779573787502210"
        "0f7da90b82d24e6e957264b17d3e5042bab8946ee5fc676d15d915da450151d36"
        "012103893d5a06201d5cf61400e96fa4a7514fc12ab45166ace618d68b8066c9c"
        "585f9ffffffff0aa14d394a1f0eaf0c4496537f8ab9246d9663e26acb5f308fcc"
        "c734b748cc9c010000006c493046022100d64ace8ec2d5feeb3e868e82b894202"
        "db8cb683c414d806b343d02b7ac679de7022100a2dcd39940dd28d4e22cce417a"
        "0829c1b516c471a3d64d11f2c5d754108bdc0b012103893d5a06201d5cf61400e"
        "96fa4a7514fc12ab45166ace618d68b8066c9c585f9ffffffff02c0e1e4000000"
        "00001976a914884c09d7e1f6420976c40e040c30b2b62210c3d488ac203005000"
        "00000001976a914905f933de850988603aafeeb2fd7fce61e66fe5d88ac000000"
        "00"));

    message::block_transactions expected;
    expected.from_data(message::block_transactions::version_minimum, raw);

    const auto data = expected.to_data(message::block_transactions::version_minimum);

    BOOST_REQUIRE(raw == data);
    const auto result = message::block_transactions::factory(message::block_transactions::version_minimum, data);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(message::block_transactions::version_minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::block_transactions::version_minimum), result.serialized_size(message::block_transactions::version_minimum));
}

BOOST_AUTO_TEST_CASE(block_transactions__factory_2__valid_input__success)
{
    const auto raw = to_chunk(base16_literal(
        "3ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a0"
        "20100000001f08e44a96bfb5ae63eda1a6620adae37ee37ee4777fb0336e1bbbc"
        "4de65310fc010000006a473044022050d8368cacf9bf1b8fb1f7cfd9aff632947"
        "89eb1760139e7ef41f083726dadc4022067796354aba8f2e02363c5e510aa7e28"
        "30b115472fb31de67d16972867f13945012103e589480b2f746381fca01a9b12c"
        "517b7a482a203c8b2742985da0ac72cc078f2ffffffff02f0c9c4670000000019"
        "76a914d9d78e26df4e4601cf9b26d09c7b280ee764469f88ac80c4600f0000000"
        "01976a9141ee32412020a324b93b1a1acfdfff6ab9ca8fac288ac000000000100"
        "00000364e62ad837f29617bafeae951776e7a6b3019b2da37827921548d1a5efc"
        "f9e5c010000006b48304502204df0dc9b7f61fbb2e4c8b0e09f3426d625a0191e"
        "56c48c338df3214555180eaf022100f21ac1f632201154f3c69e1eadb59901a34"
        "c40f1127e96adc31fac6ae6b11fb4012103893d5a06201d5cf61400e96fa4a751"
        "4fc12ab45166ace618d68b8066c9c585f9ffffffff54b755c39207d443fd96a8d"
        "12c94446a1c6f66e39c95e894c23418d7501f681b010000006b48304502203267"
        "910f55f2297360198fff57a3631be850965344370f732950b4779573787502210"
        "0f7da90b82d24e6e957264b17d3e5042bab8946ee5fc676d15d915da450151d36"
        "012103893d5a06201d5cf61400e96fa4a7514fc12ab45166ace618d68b8066c9c"
        "585f9ffffffff0aa14d394a1f0eaf0c4496537f8ab9246d9663e26acb5f308fcc"
        "c734b748cc9c010000006c493046022100d64ace8ec2d5feeb3e868e82b894202"
        "db8cb683c414d806b343d02b7ac679de7022100a2dcd39940dd28d4e22cce417a"
        "0829c1b516c471a3d64d11f2c5d754108bdc0b012103893d5a06201d5cf61400e"
        "96fa4a7514fc12ab45166ace618d68b8066c9c585f9ffffffff02c0e1e4000000"
        "00001976a914884c09d7e1f6420976c40e040c30b2b62210c3d488ac203005000"
        "00000001976a914905f933de850988603aafeeb2fd7fce61e66fe5d88ac000000"
        "00"));

    message::block_transactions expected;
    expected.from_data(message::block_transactions::version_minimum, raw);

    const auto data = expected.to_data(message::block_transactions::version_minimum);

    BOOST_REQUIRE(raw == data);
    stream::in::copy istream(data);
    auto result = message::block_transactions::factory(message::block_transactions::version_minimum, istream);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(message::block_transactions::version_minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::block_transactions::version_minimum), result.serialized_size(message::block_transactions::version_minimum));
}

BOOST_AUTO_TEST_CASE(block_transactions__factory_3__valid_input__success)
{
    const auto raw = to_chunk(base16_literal(
        "3ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a0"
        "20100000001f08e44a96bfb5ae63eda1a6620adae37ee37ee4777fb0336e1bbbc"
        "4de65310fc010000006a473044022050d8368cacf9bf1b8fb1f7cfd9aff632947"
        "89eb1760139e7ef41f083726dadc4022067796354aba8f2e02363c5e510aa7e28"
        "30b115472fb31de67d16972867f13945012103e589480b2f746381fca01a9b12c"
        "517b7a482a203c8b2742985da0ac72cc078f2ffffffff02f0c9c4670000000019"
        "76a914d9d78e26df4e4601cf9b26d09c7b280ee764469f88ac80c4600f0000000"
        "01976a9141ee32412020a324b93b1a1acfdfff6ab9ca8fac288ac000000000100"
        "00000364e62ad837f29617bafeae951776e7a6b3019b2da37827921548d1a5efc"
        "f9e5c010000006b48304502204df0dc9b7f61fbb2e4c8b0e09f3426d625a0191e"
        "56c48c338df3214555180eaf022100f21ac1f632201154f3c69e1eadb59901a34"
        "c40f1127e96adc31fac6ae6b11fb4012103893d5a06201d5cf61400e96fa4a751"
        "4fc12ab45166ace618d68b8066c9c585f9ffffffff54b755c39207d443fd96a8d"
        "12c94446a1c6f66e39c95e894c23418d7501f681b010000006b48304502203267"
        "910f55f2297360198fff57a3631be850965344370f732950b4779573787502210"
        "0f7da90b82d24e6e957264b17d3e5042bab8946ee5fc676d15d915da450151d36"
        "012103893d5a06201d5cf61400e96fa4a7514fc12ab45166ace618d68b8066c9c"
        "585f9ffffffff0aa14d394a1f0eaf0c4496537f8ab9246d9663e26acb5f308fcc"
        "c734b748cc9c010000006c493046022100d64ace8ec2d5feeb3e868e82b894202"
        "db8cb683c414d806b343d02b7ac679de7022100a2dcd39940dd28d4e22cce417a"
        "0829c1b516c471a3d64d11f2c5d754108bdc0b012103893d5a06201d5cf61400e"
        "96fa4a7514fc12ab45166ace618d68b8066c9c585f9ffffffff02c0e1e4000000"
        "00001976a914884c09d7e1f6420976c40e040c30b2b62210c3d488ac203005000"
        "00000001976a914905f933de850988603aafeeb2fd7fce61e66fe5d88ac000000"
        "00"));

    message::block_transactions expected;
    expected.from_data(message::block_transactions::version_minimum, raw);

    const auto data = expected.to_data(message::block_transactions::version_minimum);

    BOOST_REQUIRE(raw == data);
    read::bytes::copy source(data);
    const auto result = message::block_transactions::factory(message::block_transactions::version_minimum, source);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(message::block_transactions::version_minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::block_transactions::version_minimum), result.serialized_size(message::block_transactions::version_minimum));
}

BOOST_AUTO_TEST_CASE(block_transactions__block_hash_accessor_1__always__returns_initialized_value)
{
    const auto hash = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    const chain::transaction::list transactions
    {
        {
            chain::transaction(1, 48, {}, {}),
            chain::transaction(2, 32, {}, {}),
            chain::transaction(4, 16, {}, {})
        }
    };

    message::block_transactions instance(hash, transactions);
    BOOST_REQUIRE(hash == instance.block_hash());
}

BOOST_AUTO_TEST_CASE(block_transactions__block_hash_accessor_2__always__returns_initialized_value)
{
    const auto hash = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    const chain::transaction::list transactions
    {
        {
            chain::transaction(1, 48, {}, {}),
            chain::transaction(2, 32, {}, {}),
            chain::transaction(4, 16, {}, {})
        }
    };

    const message::block_transactions instance(hash, transactions);
    BOOST_REQUIRE(hash == instance.block_hash());
}

BOOST_AUTO_TEST_CASE(block_transactions__block_hash_setter_1__roundtrip__success)
{
    const auto hash = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    message::block_transactions instance;
    BOOST_REQUIRE(hash != instance.block_hash());
    instance.set_block_hash(hash);
    BOOST_REQUIRE(hash == instance.block_hash());
}

BOOST_AUTO_TEST_CASE(block_transactions__block_hash_setter_2__roundtrip__success)
{
    const auto hash = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");

    hash_digest dup_hash = hash;
    message::block_transactions instance;
    BOOST_REQUIRE(hash != instance.block_hash());
    instance.set_block_hash(std::move(dup_hash));
    BOOST_REQUIRE(hash == instance.block_hash());
}

BOOST_AUTO_TEST_CASE(block_transactions__transactions_accessor_1__always__returns_initialized_value)
{
    const auto hash = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    const chain::transaction::list transactions
    {
        {
            chain::transaction(1, 48, {}, {}),
            chain::transaction(2, 32, {}, {}),
            chain::transaction(4, 16, {}, {})
        }
    };

    message::block_transactions instance(hash, transactions);
    BOOST_REQUIRE(transactions == instance.transactions());
}

BOOST_AUTO_TEST_CASE(block_transactions__transactions_accessor_2__always__returns_initialized_value)
{
    const auto hash = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    const chain::transaction::list transactions
    {
        {
            chain::transaction(1, 48, {}, {}),
            chain::transaction(2, 32, {}, {}),
            chain::transaction(4, 16, {}, {})
        }
    };

    const message::block_transactions instance(hash, transactions);
    BOOST_REQUIRE(transactions == instance.transactions());
}

BOOST_AUTO_TEST_CASE(block_transactions__transactions_setter_1__roundtrip__success)
{
    const chain::transaction::list transactions
    {
        {
            chain::transaction(1, 48, {}, {}),
            chain::transaction(2, 32, {}, {}),
            chain::transaction(4, 16, {}, {})
        }
    };

    message::block_transactions instance;
    BOOST_REQUIRE(transactions != instance.transactions());
    instance.set_transactions(transactions);
    BOOST_REQUIRE(transactions == instance.transactions());
}

BOOST_AUTO_TEST_CASE(block_transactions__transactions_setter_2__roundtrip__success)
{
    const chain::transaction::list transactions
    {
        {
            chain::transaction(1, 48, {}, {}),
            chain::transaction(2, 32, {}, {}),
            chain::transaction(4, 16, {}, {})
        }
    };

    chain::transaction::list dup_transactions = transactions;
    message::block_transactions instance;
    BOOST_REQUIRE(transactions != instance.transactions());
    instance.set_transactions(std::move(dup_transactions));
    BOOST_REQUIRE(transactions == instance.transactions());
}

BOOST_AUTO_TEST_CASE(block_transactions__operator_assign_equals__always__matches_equivalent)
{
    const auto hash = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    const chain::transaction::list transactions
    {
        {
            chain::transaction(1, 48, {}, {}),
            chain::transaction(2, 32, {}, {}),
            chain::transaction(4, 16, {}, {})
        }
    };

    message::block_transactions value(hash, transactions);
    BOOST_REQUIRE(value.is_valid());
    message::block_transactions instance;
    BOOST_REQUIRE(!instance.is_valid());
    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(hash == instance.block_hash());
    BOOST_REQUIRE(transactions == instance.transactions());
}

BOOST_AUTO_TEST_CASE(block_transactions__operator_boolean_equals__duplicates__returns_true)
{
    const message::block_transactions expected(
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        {
            chain::transaction(1, 48, {}, {}),
            chain::transaction(2, 32, {}, {}),
            chain::transaction(4, 16, {}, {})
        });

    message::block_transactions instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(block_transactions__operator_boolean_equals__differs__returns_false)
{
    const message::block_transactions expected(
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        {
            chain::transaction(1, 48, {}, {}),
            chain::transaction(2, 32, {}, {}),
            chain::transaction(4, 16, {}, {})
        });

    message::block_transactions instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(block_transactions__operator_boolean_not_equals__duplicates__returns_false)
{
    const message::block_transactions expected(
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        {
            chain::transaction(1, 48, {}, {}),
            chain::transaction(2, 32, {}, {}),
            chain::transaction(4, 16, {}, {})
        });

    message::block_transactions instance(expected);
    BOOST_REQUIRE(!(instance != expected));
}

BOOST_AUTO_TEST_CASE(block_transactions__operator_boolean_not_equals__differs__returns_true)
{
    const message::block_transactions expected(
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        {
            chain::transaction(1, 48, {}, {}),
            chain::transaction(2, 32, {}, {}),
            chain::transaction(4, 16, {}, {})
        });

    message::block_transactions instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()
