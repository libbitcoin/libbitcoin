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

BOOST_AUTO_TEST_SUITE(get_data_tests)

using namespace bc::system::message;

BOOST_AUTO_TEST_CASE(get_data__constructor_1__always__invalid)
{
    const get_data instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());
}

BOOST_AUTO_TEST_CASE(get_data__constructor_2__always__equals_params)
{
    static const inventory_vector::list values =
    {
        inventory_vector
        {
            inventory::type_id::error,
            {
                {
                    0x44, 0x9a, 0x0d, 0x24, 0x9a, 0xd5, 0x39, 0x89,
                    0xbb, 0x85, 0x0a, 0x3d, 0x79, 0x24, 0xed, 0x0f,
                    0xc3, 0x0d, 0x6f, 0x55, 0x7d, 0x71, 0x12, 0x1a,
                    0x37, 0xc0, 0xb0, 0x32, 0xf0, 0xd6, 0x6e, 0xdf
                }
            }
        }
    };

    const get_data instance(values);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(values == instance.inventories());
}

BOOST_AUTO_TEST_CASE(get_data__constructor_3__always__equals_params)
{
    static const auto type = inventory_vector::type_id::error;
    static const auto hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const inventory_vector::list values
    {
        inventory_vector(type, hash)
    };

    const get_data instance(std::move(values));
    BOOST_REQUIRE(instance.is_valid());
    const auto inventories = instance.inventories();
    BOOST_REQUIRE_EQUAL(1u, inventories.size());
    BOOST_REQUIRE(type == inventories[0].type());
    BOOST_REQUIRE(hash == inventories[0].hash());
}

BOOST_AUTO_TEST_CASE(get_data__constructor_4__always__equals_params)
{
    static const auto type = inventory_vector::type_id::error;
    static const auto hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    static const hash_list hashes{ hash };

    const get_data instance(hashes, type);
    BOOST_REQUIRE(instance.is_valid());
    auto inventories = instance.inventories();
    BOOST_REQUIRE_EQUAL(1u, inventories.size());
    BOOST_REQUIRE(type == inventories[0].type());
    BOOST_REQUIRE(hash == inventories[0].hash());
}

BOOST_AUTO_TEST_CASE(get_data__constructor_5__always__equals_params)
{
    static const auto type = inventory_vector::type_id::error;
    static const auto hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    const get_data instance{ { type, hash } };
    BOOST_REQUIRE(instance.is_valid());
    auto inventories = instance.inventories();
    BOOST_REQUIRE_EQUAL(1u, inventories.size());
    BOOST_REQUIRE(type == inventories[0].type());
    BOOST_REQUIRE(hash == inventories[0].hash());
}

BOOST_AUTO_TEST_CASE(get_data__constructor_6__always__equals_params)
{
    static const auto type = inventory_vector::type_id::error;
    static const auto hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    const get_data value{ { type, hash } };
    BOOST_REQUIRE(value.is_valid());
    const get_data instance(value);
    const auto inventories = instance.inventories();
    BOOST_REQUIRE_EQUAL(1u, inventories.size());
    BOOST_REQUIRE(type == inventories[0].type());
    BOOST_REQUIRE(hash == inventories[0].hash());
    BOOST_REQUIRE(value == instance);
}

BOOST_AUTO_TEST_CASE(get_data__constructor_7__always__equals_params)
{
    static const auto type = inventory_vector::type_id::error;
    static const auto hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    const get_data value{ { type, hash } };
    BOOST_REQUIRE(value.is_valid());
    const get_data instance(std::move(value));
    const auto inventories = instance.inventories();
    BOOST_REQUIRE_EQUAL(1u, inventories.size());
    BOOST_REQUIRE(type == inventories[0].type());
    BOOST_REQUIRE(hash == inventories[0].hash());
}

BOOST_AUTO_TEST_CASE(get_data__from_data__insufficient_bytes__failure)
{
    static const data_chunk raw{ 0xab, 0xcd };
    get_data instance;
    static const auto version = version::level::maximum;
    BOOST_REQUIRE_EQUAL(false, instance.from_data(version, raw));
}

BOOST_AUTO_TEST_CASE(get_data__from_data__insufficient_version__failure)
{
    static const get_data expected
    {
        {
            inventory_vector::type_id::error,
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b")
        }
    };

    const auto raw = expected.to_data(version::level::maximum);
    get_data instance;
    BOOST_REQUIRE_EQUAL(false, instance.from_data(get_data::version_minimum - 1, raw));
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());
}

BOOST_AUTO_TEST_CASE(get_data__factory_1__valid_input__success)
{
    static const get_data expected
    {
        {
            inventory_vector::type_id::error,
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b")
        }
    };

    static const auto version = version::level::maximum;
    const auto data = expected.to_data(version);
    const auto result = get_data::factory(version, data);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(version));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version), result.serialized_size(version));
}

BOOST_AUTO_TEST_CASE(get_data__factory_2__valid_input__success)
{
    static const get_data expected
    {
        {
            {
                inventory_vector::type_id::transaction,
                hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b")
            }
        }
    };

    static const auto version = version::level::maximum;
    const auto data = expected.to_data(version);
    stream::in::copy istream(data);
    const auto result = get_data::factory(version, istream);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(version));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version), result.serialized_size(version));
}

BOOST_AUTO_TEST_CASE(get_data__factory_3__valid_input__success)
{
    static const get_data expected
    {
        {
            {
                inventory_vector::type_id::block,
                hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b")
            }
        }
    };

    static const auto version = version::level::maximum;
    const auto data = expected.to_data(version);
    read::bytes::copy source(data);
    const auto result = get_data::factory(version, source);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(version));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version), result.serialized_size(version));
}

BOOST_AUTO_TEST_CASE(get_data__to_witness__error__unchanged)
{
    static const auto expected = inventory_vector::type_id::error;
    get_data instance{ { expected, {} } };
    instance.to_witness();
    BOOST_REQUIRE(instance.inventories()[0].type() == expected);
}

BOOST_AUTO_TEST_CASE(get_data__to_witness__filtered_block__unchanged)
{
    static const auto expected = inventory_vector::type_id::filtered_block;
    get_data instance{ { expected, {} } };
    instance.to_witness();
    BOOST_REQUIRE(instance.inventories()[0].type() == expected);
}

BOOST_AUTO_TEST_CASE(get_data__to_witness__compact_block__unchanged)
{
    static const auto expected = inventory_vector::type_id::compact_block;
    get_data instance{ { expected, {} } };
    instance.to_witness();
    BOOST_REQUIRE(instance.inventories()[0].type() == expected);
}

BOOST_AUTO_TEST_CASE(get_data__to_witness__witness_transaction__unchanged)
{
    static const auto expected = inventory_vector::type_id::witness_transaction;
    get_data instance{ { expected, {} } };
    instance.to_witness();
    BOOST_REQUIRE(instance.inventories()[0].type() == expected);
}

BOOST_AUTO_TEST_CASE(get_data__to_witness__witness_block__unchanged)
{
    static const auto expected = inventory_vector::type_id::witness_block;
    get_data instance{ { expected, {} } };
    instance.to_witness();
    BOOST_REQUIRE(instance.inventories()[0].type() == expected);
}

BOOST_AUTO_TEST_CASE(get_data__to_witness__block__expected)
{
    get_data instance{ { inventory_vector::type_id::block, {} } };
    instance.to_witness();
    BOOST_REQUIRE(instance.inventories()[0].type() == inventory_vector::type_id::witness_block);
}

BOOST_AUTO_TEST_CASE(get_data__to_witness__transaction__expected)
{
    get_data instance{ { inventory_vector::type_id::transaction, {} } };
    instance.to_witness();
    BOOST_REQUIRE(instance.inventories()[0].type() == inventory_vector::type_id::witness_transaction);
}

BOOST_AUTO_TEST_CASE(get_data__to_witness__block_error_transaction__expected)
{
    get_data instance
    {
        { inventory_vector::type_id::block, {} },
        { inventory_vector::type_id::error, {} },
        { inventory_vector::type_id::transaction, {} }
    };
    instance.to_witness();
    BOOST_REQUIRE(instance.inventories()[0].type() == inventory_vector::type_id::witness_block);
    BOOST_REQUIRE(instance.inventories()[1].type() == inventory_vector::type_id::error);
    BOOST_REQUIRE(instance.inventories()[2].type() == inventory_vector::type_id::witness_transaction);
}

BOOST_AUTO_TEST_CASE(get_data__operator_assign_equals__always__matches_equivalent)
{
    static const auto hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    static const inventory_vector::list elements
    {
        inventory_vector(inventory_vector::type_id::error, hash)
    };

    get_data value(elements);
    BOOST_REQUIRE(value.is_valid());

    get_data instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(elements == instance.inventories());
}

BOOST_AUTO_TEST_CASE(get_data__operator_boolean_equals__duplicates__returns_true)
{
    static const auto hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const get_data expected
    {
        inventory_vector(inventory_vector::type_id::error, hash)
    };

    const get_data instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(get_data__operator_boolean_equals__differs__returns_false)
{
    static const auto hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const get_data expected
    {
        inventory_vector(inventory_vector::type_id::error, hash)
    };

    const get_data instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(get_data__operator_boolean_not_equals__duplicates__returns_false)
{
    static const auto hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const get_data expected
    {
        inventory_vector(inventory_vector::type_id::error, hash)
    };

    const  get_data instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(get_data__operator_boolean_not_equals__differs__returns_true)
{
    static const auto hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const get_data expected
    {
        inventory_vector(inventory_vector::type_id::error, hash)
    };

    const get_data instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()
