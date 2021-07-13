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
#include <chrono>

BOOST_AUTO_TEST_SUITE(chain_header_tests)

BOOST_AUTO_TEST_CASE(header__constructor_1__always__initialized_invalid)
{
    chain::header instance;
    BOOST_REQUIRE(!instance.is_valid());
}

BOOST_AUTO_TEST_CASE(header__constructor_2__always__equals_params)
{
    const uint32_t version = 10u;
    const auto previous = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    const auto merkle = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const uint32_t timestamp = 531234u;
    const uint32_t bits = 6523454u;
    const uint32_t nonce = 68644u;

    chain::header instance(version, previous, merkle, timestamp, bits, nonce);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(version, instance.version());
    BOOST_REQUIRE_EQUAL(timestamp, instance.timestamp());
    BOOST_REQUIRE_EQUAL(bits, instance.bits());
    BOOST_REQUIRE_EQUAL(nonce, instance.nonce());
    BOOST_REQUIRE(previous == instance.previous_block_hash());
    BOOST_REQUIRE(merkle == instance.merkle_root());
}

BOOST_AUTO_TEST_CASE(header__constructor_3__always__equals_params)
{
    const uint32_t version = 10u;
    const uint32_t timestamp = 531234u;
    const uint32_t bits = 6523454u;
    const uint32_t nonce = 68644u;

    // These must be non-const.
    auto previous = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    auto merkle = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    chain::header instance(version, std::move(previous), std::move(merkle),
        timestamp, bits, nonce);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(version, instance.version());
    BOOST_REQUIRE_EQUAL(timestamp, instance.timestamp());
    BOOST_REQUIRE_EQUAL(bits, instance.bits());
    BOOST_REQUIRE_EQUAL(nonce, instance.nonce());
    BOOST_REQUIRE(previous == instance.previous_block_hash());
    BOOST_REQUIRE(merkle == instance.merkle_root());
}

BOOST_AUTO_TEST_CASE(header__constructor_4__always__equals_params)
{
    const chain::header expected(
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    chain::header instance(expected);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(expected == instance);
}

BOOST_AUTO_TEST_CASE(header__constructor_5__always__equals_params)
{
    // This must be non-const.
    chain::header expected(
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    chain::header instance(std::move(expected));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(expected == instance);
}

BOOST_AUTO_TEST_CASE(header__from_data__insufficient_bytes__failure)
{
    data_chunk data(10);

    chain::header header;

    BOOST_REQUIRE(!header.from_data(data));
    BOOST_REQUIRE(!header.is_valid());
}

BOOST_AUTO_TEST_CASE(header__factory_1__valid_input__success)
{
    chain::header expected
    {
        10,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234,
        6523454,
        68644,
    };

    const auto data = expected.to_data();

    const auto result = chain::header::factory(data);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
}

BOOST_AUTO_TEST_CASE(header__factory_2__valid_input__success)
{
    chain::header expected
    {
        10,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234,
        6523454,
        68644
    };

    const auto data = expected.to_data();
    stream::in::copy istream(data);

    const auto result = chain::header::factory(istream);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
}

BOOST_AUTO_TEST_CASE(header__factory_3__valid_input__success)
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

    const auto data = expected.to_data();
    read::bytes::copy source(data);

    const auto result = chain::header::factory(source);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
}

BOOST_AUTO_TEST_CASE(header__version_accessor__always__returns_initialized_value)
{
    const uint32_t value = 11234u;
    const chain::header instance(
        value,
        hash_literal("abababababababababababababababababababababababababababababababab"),
        hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe"),
        753234u,
        4356344u,
        34564u);

    BOOST_REQUIRE_EQUAL(value, instance.version());
}

BOOST_AUTO_TEST_CASE(header__version_setter__roundtrip__success)
{
    uint32_t expected = 4521u;
    chain::header instance;
    BOOST_REQUIRE(expected != instance.version());
    instance.set_version(expected);
    BOOST_REQUIRE(expected == instance.version());
}

BOOST_AUTO_TEST_CASE(header__previous_block_hash_accessor_1__always__returns_initialized_value)
{
    const auto value = hash_literal("abababababababababababababababababababababababababababababababab");
    chain::header instance(
        11234u,
        value,
        hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe"),
        753234u,
        4356344u,
        34564u);

    BOOST_REQUIRE(value == instance.previous_block_hash());
}

BOOST_AUTO_TEST_CASE(header__previous_block_hash_accessor_2__always__returns_initialized_value)
{
    const auto value = hash_literal("abababababababababababababababababababababababababababababababab");
    const chain::header instance(
        11234u,
        value,
        hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe"),
        753234u,
        4356344u,
        34564u);

    BOOST_REQUIRE(value == instance.previous_block_hash());
}

BOOST_AUTO_TEST_CASE(header__previous_block_hash_setter_1__roundtrip__success)
{
    const auto expected = hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe");
    chain::header instance;
    BOOST_REQUIRE(expected != instance.previous_block_hash());
    instance.set_previous_block_hash(expected);
    BOOST_REQUIRE(expected == instance.previous_block_hash());
}

BOOST_AUTO_TEST_CASE(header__previous_block_hash_setter_2__roundtrip__success)
{
    const auto expected = hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe");

    // This must be non-const.
    auto duplicate = expected;

    chain::header instance;
    BOOST_REQUIRE(expected != instance.previous_block_hash());
    instance.set_previous_block_hash(std::move(duplicate));
    BOOST_REQUIRE(expected == instance.previous_block_hash());
}

BOOST_AUTO_TEST_CASE(header__merkle_accessor_1__always__returns_initialized_value)
{
    const auto value = hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe");
    chain::header instance(
        11234u,
        hash_literal("abababababababababababababababababababababababababababababababab"),
        value,
        753234u,
        4356344u,
        34564u);

    BOOST_REQUIRE(value == instance.merkle_root());
}

BOOST_AUTO_TEST_CASE(header__merkle_accessor_2__always__returns_initialized_value)
{
    const auto value = hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe");
    const chain::header instance(
        11234u,
        hash_literal("abababababababababababababababababababababababababababababababab"),
        value,
        753234u,
        4356344u,
        34564u);

    BOOST_REQUIRE(value == instance.merkle_root());
}

BOOST_AUTO_TEST_CASE(header__merkle_setter_1__roundtrip__success)
{
    const auto expected = hash_literal("abababababababababababababababababababababababababababababababab");
    chain::header instance;
    BOOST_REQUIRE(expected != instance.merkle_root());
    instance.set_merkle_root(expected);
    BOOST_REQUIRE(expected == instance.merkle_root());
}

BOOST_AUTO_TEST_CASE(header__merkle_setter_2__roundtrip__success)
{
    const auto expected = hash_literal("abababababababababababababababababababababababababababababababab");

    // This must be non-const.
    hash_digest duplicate = expected;

    chain::header instance;
    BOOST_REQUIRE(expected != instance.merkle_root());
    instance.set_merkle_root(std::move(duplicate));
    BOOST_REQUIRE(expected == instance.merkle_root());
}

BOOST_AUTO_TEST_CASE(header__timestamp_accessor__always__returns_initialized_value)
{
    uint32_t value = 753234u;
    chain::header instance(
        11234u,
        hash_literal("abababababababababababababababababababababababababababababababab"),
        hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe"),
        value,
        4356344u,
        34564u);

    BOOST_REQUIRE_EQUAL(value, instance.timestamp());
}

BOOST_AUTO_TEST_CASE(header__timestamp_setter__roundtrip__success)
{
    uint32_t expected = 4521u;
    chain::header instance;
    BOOST_REQUIRE(expected != instance.timestamp());
    instance.set_timestamp(expected);
    BOOST_REQUIRE(expected == instance.timestamp());
}

BOOST_AUTO_TEST_CASE(header__bits_accessor__always__returns_initialized_value)
{
    uint32_t value = 4356344u;
    chain::header instance(
        11234u,
        hash_literal("abababababababababababababababababababababababababababababababab"),
        hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe"),
        753234u,
        value,
        34564u);

    BOOST_REQUIRE_EQUAL(value, instance.bits());
}

BOOST_AUTO_TEST_CASE(header__bits_setter__roundtrip__success)
{
    uint32_t expected = 4521u;
    chain::header instance;
    BOOST_REQUIRE(expected != instance.bits());
    instance.set_bits(expected);
    BOOST_REQUIRE(expected == instance.bits());
}

BOOST_AUTO_TEST_CASE(header__nonce_accessor__always__returns_initialized_value)
{
    uint32_t value = 34564u;
    chain::header instance(
        11234u,
        hash_literal("abababababababababababababababababababababababababababababababab"),
        hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe"),
        753234u,
        4356344u,
        value);

    BOOST_REQUIRE_EQUAL(value, instance.nonce());
}

BOOST_AUTO_TEST_CASE(header__nonce_setter__roundtrip__success)
{
    uint32_t expected = 4521u;
    chain::header instance;
    BOOST_REQUIRE(expected != instance.nonce());
    instance.set_nonce(expected);
    BOOST_REQUIRE(expected == instance.nonce());
}

BOOST_AUTO_TEST_CASE(header__is_valid_timestamp__timestamp_less_than_2_hours_from_now__returns_true)
{
    chain::header instance;
    const auto now = std::chrono::system_clock::now();
    const auto now_time = std::chrono::system_clock::to_time_t(now);
    instance.set_timestamp(static_cast<uint32_t>(now_time));
    BOOST_REQUIRE(instance.is_valid_timestamp(settings().timestamp_limit_seconds));
}

BOOST_AUTO_TEST_CASE(header__is_valid_timestamp__timestamp_greater_than_2_hours_from_now__returns_false)
{
    chain::header instance;
    const auto now = std::chrono::system_clock::now();
    const auto duration = std::chrono::hours(3);
    const auto future = std::chrono::system_clock::to_time_t(now + duration);
    instance.set_timestamp(static_cast<uint32_t>(future));
    BOOST_REQUIRE(!instance.is_valid_timestamp(settings().timestamp_limit_seconds));
}

BOOST_AUTO_TEST_CASE(header__is_valid_proof_of_work__bits_exceeds_maximum__returns_false)
{
    const settings settings(chain::selection::mainnet);
    chain::header instance;
    instance.set_bits(settings.proof_of_work_limit + 1);
    BOOST_REQUIRE(!instance.is_valid_proof_of_work(settings.proof_of_work_limit, false));
}

BOOST_AUTO_TEST_CASE(header__is_valid_proof_of_work__hash_greater_bits__returns_false)
{
    const settings settings(chain::selection::mainnet);
    const chain::header instance(
        11234u,
        hash_literal("abababababababababababababababababababababababababababababababab"),
        hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe"),
        753234u,
        0u,
        34564u);

    BOOST_REQUIRE(!instance.is_valid_proof_of_work(settings.proof_of_work_limit, false));
}

BOOST_AUTO_TEST_CASE(header__is_valid_proof_of_work__hash_less_than_bits__returns_true)
{
    const settings settings(chain::selection::mainnet);
    const chain::header instance(
        4u,
        hash_literal("000000000000000003ddc1e929e2944b8b0039af9aa0d826c480a83d8b39c373"),
        hash_literal("a6cb0b0d6531a71abe2daaa4a991e5498e1b6b0b51549568d0f9d55329b905df"),
        1474388414u,
        402972254u,
        2842832236u);

    BOOST_REQUIRE(instance.is_valid_proof_of_work(settings.proof_of_work_limit, false));
}

BOOST_AUTO_TEST_CASE(header__is_valid_scrypt_proof_of_work__hash_greater_than_bits__returns_false)
{
    const settings settings(chain::selection::mainnet);
    const chain::header instance(
        536870912u,
        hash_literal("abababababababababababababababababababababababababababababababab"),
        hash_literal("5163359dde15eb3f49cbd0926981f065ef1405fc9d4cece8818662b3b65f5dc6"),
        1535119178u,
        436332170u,
        2135224651u);

    BOOST_REQUIRE(!instance.is_valid_proof_of_work(settings.proof_of_work_limit, true));
}

BOOST_AUTO_TEST_CASE(header__is_valid_scrypt_proof_of_work__hash_less_than_bits__returns_true)
{
    const settings settings(chain::selection::mainnet);
    const chain::header instance(
        536870912u,
        hash_literal("313ced849aafeff324073bb2bd31ecdcc365ed215a34e827bb797ad33d158542"),
        hash_literal("5163359dde15eb3f49cbd0926981f065ef1405fc9d4cece8818662b3b65f5dc6"),
        1535119178u,
        436332170u,
        2135224651u);

    BOOST_REQUIRE(instance.is_valid_proof_of_work(settings.proof_of_work_limit, true));
    BOOST_REQUIRE(!instance.is_valid_proof_of_work(settings.proof_of_work_limit, false));
}

BOOST_AUTO_TEST_CASE(header__proof1__genesis_mainnet__expected)
{
    BOOST_REQUIRE_EQUAL(chain::header::proof(0x1d00ffff), 0x0000000100010001);
}

BOOST_AUTO_TEST_CASE(header__proof2__genesis_mainnet__expected)
{
    const chain::block block = settings(chain::selection::mainnet).genesis_block;
    BOOST_REQUIRE_EQUAL(block.header().proof(), 0x0000000100010001);
}

BOOST_AUTO_TEST_CASE(header__operator_assign_equals__always__matches_equivalent)
{
    // This must be non-const.
    chain::header value(
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    BOOST_REQUIRE(value.is_valid());

    chain::header instance;
    BOOST_REQUIRE(!instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
}

BOOST_AUTO_TEST_CASE(header__operator_boolean_equals__duplicates__returns_true)
{
    const chain::header expected(
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    chain::header instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(header__operator_boolean_equals__differs__returns_false)
{
    const chain::header expected(
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    chain::header instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(header__operator_boolean_not_equals__duplicates__returns_false)
{
    const chain::header expected(
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    chain::header instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(header__operator_boolean_not_equals__differs__returns_true)
{
    const chain::header expected(
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    chain::header instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()
