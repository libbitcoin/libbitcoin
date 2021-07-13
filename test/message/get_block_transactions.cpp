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

BOOST_AUTO_TEST_SUITE(get_block_transactions_tests)

BOOST_AUTO_TEST_CASE(get_block_transactions__constructor_1__always__invalid)
{
    message::get_block_transactions instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());
}

BOOST_AUTO_TEST_CASE(get_block_transactions__constructor_2__always__equals_params)
{
    const hash_digest hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const std::vector<uint64_t> indexes = { 1u, 3454u, 4234u, 75123u, 455323u };

    message::get_block_transactions instance(hash, indexes);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(hash == instance.block_hash());
    BOOST_REQUIRE(indexes == instance.indexes());
}

BOOST_AUTO_TEST_CASE(get_block_transactions__constructor_3__always__equals_params)
{
    hash_digest hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    auto hash_dup = hash;
    std::vector<uint64_t> indexes = { 1u, 3454u, 4234u, 75123u, 455323u };
    auto indexes_dup = indexes;

    message::get_block_transactions instance(std::move(hash_dup), std::move(indexes_dup));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(hash == instance.block_hash());
    BOOST_REQUIRE(indexes == instance.indexes());
}

BOOST_AUTO_TEST_CASE(get_block_transactions__constructor_4__always__equals_params)
{
    message::get_block_transactions value(
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        { 1u, 3454u, 4234u, 75123u, 455323u });

    message::get_block_transactions instance(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(value == instance);
}

BOOST_AUTO_TEST_CASE(get_block_transactions__constructor_5__always__equals_params)
{
    hash_digest hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    std::vector<uint64_t> indexes = { 1u, 3454u, 4234u, 75123u, 455323u };

    message::get_block_transactions value(hash, indexes);
    message::get_block_transactions instance(std::move(value));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(hash == instance.block_hash());
    BOOST_REQUIRE(indexes == instance.indexes());
}

BOOST_AUTO_TEST_CASE(get_block_transactions__from_data__insufficient_bytes__failure)
{
    const data_chunk raw{ 0xab, 0xcd };
    message::get_block_transactions instance{};

    BOOST_REQUIRE_EQUAL(false, instance.from_data(message::version::level::minimum, raw));
}

BOOST_AUTO_TEST_CASE(get_block_transactions__factory_1__valid_input__success)
{
    const message::get_block_transactions expected
    {
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        {
            16,
            32,
            37,
            44
        }
    };

    const auto data = expected.to_data(message::version::level::minimum);
    const auto result = message::get_block_transactions::factory(
        message::version::level::minimum, data);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(),
        result.serialized_size(message::version::level::minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::minimum),
        result.serialized_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_CASE(get_block_transactions__factory_2__valid_input__success)
{
    const message::get_block_transactions expected
    {
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        {
            16,
            32,
            37,
            44
        }
    };

    const auto data = expected.to_data(message::version::level::minimum);
    stream::in::copy istream(data);
    auto result = message::get_block_transactions::factory(
        message::version::level::minimum, istream);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(),
        result.serialized_size(message::version::level::minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::minimum),
        result.serialized_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_CASE(get_block_transactions__factory_3__valid_input__success)
{
    const message::get_block_transactions expected
    {
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        {
            16,
            32,
            37,
            44
        }
    };

    const auto data = expected.to_data(message::version::level::minimum);
    read::bytes::copy source(data);
    const auto result = message::get_block_transactions::factory(
        message::version::level::minimum, source);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(),
        result.serialized_size(message::version::level::minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::minimum),
        result.serialized_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_CASE(get_block_transactions__block_hash_accessor_1__always__returns_initialized_value)
{
    const hash_digest hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const std::vector<uint64_t> indexes = { 1u, 3454u, 4234u, 75123u, 455323u };
    message::get_block_transactions instance(hash, indexes);
    BOOST_REQUIRE(hash == instance.block_hash());
}

BOOST_AUTO_TEST_CASE(get_block_transactions__block_hash_accessor_2__always__returns_initialized_value)
{
    const hash_digest hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const std::vector<uint64_t> indexes = { 1u, 3454u, 4234u, 75123u, 455323u };
    const message::get_block_transactions instance(hash, indexes);
    BOOST_REQUIRE(hash == instance.block_hash());
}

BOOST_AUTO_TEST_CASE(get_block_transactions__block_hash_setter_1__roundtrip__success)
{
    const hash_digest hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    message::get_block_transactions instance;
    BOOST_REQUIRE(hash != instance.block_hash());
    instance.set_block_hash(hash);
    BOOST_REQUIRE(hash == instance.block_hash());
}

BOOST_AUTO_TEST_CASE(get_block_transactions__block_hash_setter_2__roundtrip__success)
{
    hash_digest hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    auto dup = hash;
    message::get_block_transactions instance;
    BOOST_REQUIRE(hash != instance.block_hash());
    instance.set_block_hash(std::move(dup));
    BOOST_REQUIRE(hash == instance.block_hash());
}

BOOST_AUTO_TEST_CASE(get_block_transactions__indexes_accessor_1__always__returns_initialized_value)
{
    const hash_digest hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const std::vector<uint64_t> indexes = { 1u, 3454u, 4234u, 75123u, 455323u };
    message::get_block_transactions instance(hash, indexes);
    BOOST_REQUIRE(indexes == instance.indexes());
}

BOOST_AUTO_TEST_CASE(get_block_transactions__indexes_accessor_2__always__returns_initialized_value)
{
    const hash_digest hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const std::vector<uint64_t> indexes = { 1u, 3454u, 4234u, 75123u, 455323u };
    const message::get_block_transactions instance(hash, indexes);
    BOOST_REQUIRE(indexes == instance.indexes());
}

BOOST_AUTO_TEST_CASE(get_block_transactions__indexes_setter_1__roundtrip__success)
{
    const std::vector<uint64_t> indexes = { 1u, 3454u, 4234u, 75123u, 455323u };
    message::get_block_transactions instance;
    BOOST_REQUIRE(indexes != instance.indexes());
    instance.set_indexes(indexes);
    BOOST_REQUIRE(indexes == instance.indexes());
}

BOOST_AUTO_TEST_CASE(get_block_transactions__indexes_setter_2__roundtrip__success)
{
    std::vector<uint64_t> indexes = { 1u, 3454u, 4234u, 75123u, 455323u };
    auto dup = indexes;
    message::get_block_transactions instance;
    BOOST_REQUIRE(indexes != instance.indexes());
    instance.set_indexes(std::move(dup));
    BOOST_REQUIRE(indexes == instance.indexes());
}

BOOST_AUTO_TEST_CASE(get_block_transactions__operator_assign_equals__always__matches_equivalent)
{
    const hash_digest hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const std::vector<uint64_t> indexes = { 1u, 3454u, 4234u, 75123u, 455323u };
    message::get_block_transactions value(hash, indexes);

    BOOST_REQUIRE(value.is_valid());

    message::get_block_transactions instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(hash == instance.block_hash());
    BOOST_REQUIRE(indexes == instance.indexes());
}

BOOST_AUTO_TEST_CASE(get_block_transactions__operator_boolean_equals__duplicates__returns_true)
{
    const message::get_block_transactions expected(
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        { 1u, 3454u, 4234u, 75123u, 455323u });

    message::get_block_transactions instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(get_block_transactions__operator_boolean_equals__differs__returns_false)
{
    const message::get_block_transactions expected(
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        { 1u, 3454u, 4234u, 75123u, 455323u });

    message::get_block_transactions instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(get_block_transactions__operator_boolean_not_equals__duplicates__returns_false)
{
    const message::get_block_transactions expected(
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        { 1u, 3454u, 4234u, 75123u, 455323u });

    message::get_block_transactions instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(get_block_transactions__operator_boolean_not_equals__differs__returns_true)
{
    const message::get_block_transactions expected(
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        { 1u, 3454u, 4234u, 75123u, 455323u });

    message::get_block_transactions instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()
