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

BOOST_AUTO_TEST_SUITE(inventory_vector_tests)

using namespace bc::system::message;

BOOST_AUTO_TEST_CASE(inventory_vector__to_number__error__returns_0)
{
    BOOST_REQUIRE_EQUAL(inventory_vector::to_number(inventory_vector::type_id::error), 0u);
}

BOOST_AUTO_TEST_CASE(inventory_vector__to_number__transaction__returns_1)
{
    BOOST_REQUIRE_EQUAL(inventory_vector::to_number(inventory_vector::type_id::transaction), 1u);
}

BOOST_AUTO_TEST_CASE(inventory_vector__to_number__block__returns_2)
{
    BOOST_REQUIRE_EQUAL(inventory_vector::to_number(inventory_vector::type_id::block), 2u);
}

BOOST_AUTO_TEST_CASE(inventory_vector__to_number__filtered_block__returns_3)
{
    BOOST_REQUIRE_EQUAL(inventory_vector::to_number(inventory_vector::type_id::filtered_block), 3u);
}

BOOST_AUTO_TEST_CASE(inventory_vector__to_number__compact_block__returns_4)
{
    BOOST_REQUIRE_EQUAL(inventory_vector::to_number(inventory_vector::type_id::compact_block), 4u);
}

BOOST_AUTO_TEST_CASE(inventory_vector__to_type__0__returns_error)
{
    BOOST_REQUIRE(inventory_vector::type_id::error == inventory_vector::to_type(0));
}

BOOST_AUTO_TEST_CASE(inventory_vector__to_type__1__returns_transaction)
{
    BOOST_REQUIRE(inventory_vector::type_id::transaction == inventory_vector::to_type(1));
}

BOOST_AUTO_TEST_CASE(inventory_vector__to_type__2__returns_block)
{
    BOOST_REQUIRE(inventory_vector::type_id::block == inventory_vector::to_type(2));
}

BOOST_AUTO_TEST_CASE(inventory_vector__to_type__3__returns_filtered_block)
{
    BOOST_REQUIRE(inventory_vector::type_id::filtered_block == inventory_vector::to_type(3));
}

BOOST_AUTO_TEST_CASE(inventory_vector__to_type__4__returns_compact_block)
{
    BOOST_REQUIRE(inventory_vector::type_id::compact_block == inventory_vector::to_type(4));
}

BOOST_AUTO_TEST_CASE(inventory_vector__constructor_1__always__invalid)
{
    inventory_vector instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());
}

BOOST_AUTO_TEST_CASE(inventory_vector__constructor_2__always__equals_params)
{
    inventory_vector::type_id type = inventory_vector::type_id::block;
    hash_digest hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    inventory_vector instance(type, hash);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(type == instance.type());
    BOOST_REQUIRE(hash == instance.hash());
}

BOOST_AUTO_TEST_CASE(inventory_vector__constructor_3__always__equals_params)
{
    inventory_vector::type_id type = inventory_vector::type_id::block;
    hash_digest hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    inventory_vector instance(type, std::move(hash));
    BOOST_REQUIRE(instance.is_valid());
}

BOOST_AUTO_TEST_CASE(inventory_vector__constructor_4__always__equals_params)
{
    inventory_vector::type_id type = inventory_vector::type_id::block;
    hash_digest hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const inventory_vector expected(type, hash);
    BOOST_REQUIRE(expected.is_valid());
    inventory_vector instance(expected);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(expected == instance);
}

BOOST_AUTO_TEST_CASE(inventory_vector__constructor_5__always__equals_params)
{
    inventory_vector::type_id type = inventory_vector::type_id::block;
    hash_digest hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    inventory_vector expected(type, hash);
    BOOST_REQUIRE(expected.is_valid());
    inventory_vector instance(std::move(expected));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(type == instance.type());
    BOOST_REQUIRE(hash == instance.hash());
}

BOOST_AUTO_TEST_CASE(inventory_vector__from_data__insufficient_bytes__failure)
{
    static const data_chunk raw{ 1 };
    inventory_vector instance;
    BOOST_REQUIRE_EQUAL(false, instance.from_data(version::level::minimum, raw));
}

BOOST_AUTO_TEST_CASE(inventory_vector__factory_1__valid_input__success)
{
    static const inventory_vector expected
    {
        inventory_vector::type_id::error,
        {
            {
                0x44, 0x9a, 0x0d, 0x24, 0x9a, 0xd5, 0x39, 0x89,
                0xbb, 0x85, 0x0a, 0x3d, 0x79, 0x24, 0xed, 0x0f,
                0xc3, 0x0d, 0x6f, 0x55, 0x7d, 0x71, 0x12, 0x1a,
                0x37, 0xc0, 0xb0, 0x32, 0xf0, 0xd6, 0x6e, 0xdf
            }
        }
    };

    static const auto version = version::level::minimum;
    const auto data = expected.to_data(version);
    const auto result = inventory_vector::factory(version, data);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(version));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version), result.serialized_size(version));
}

BOOST_AUTO_TEST_CASE(inventory_vector__factory_2__valid_input__success)
{
    static const inventory_vector expected
    {
        inventory_vector::type_id::transaction,
        {
            {
                0x44, 0x9a, 0x0d, 0xee, 0x9a, 0xd5, 0x39, 0xee,
                0xee, 0x85, 0x0a, 0x3d, 0xee, 0x24, 0xed, 0x0f,
                0xc3, 0xee, 0x6f, 0x55, 0x7d, 0xee, 0x12, 0x1a,
                0x37, 0xc0, 0xee, 0x32, 0xf0, 0xd6, 0xee, 0xdf
            }
        }
    };

    static const auto version = version::level::minimum;
    const auto data = expected.to_data(version);
    stream::in::copy istream(data);
    const auto result = inventory_vector::factory(version, istream);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(version));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version), result.serialized_size(version));
}

BOOST_AUTO_TEST_CASE(inventory_vector__factory_3__valid_input__success)
{
    static const inventory_vector expected
    {
        inventory_vector::type_id::block,
        {
            {
                0x66, 0x9a, 0x0d, 0x24, 0x66, 0xd5, 0x39, 0x89,
                0xbb, 0x66, 0x0a, 0x3d, 0x79, 0x66, 0xed, 0x0f,
                0xc3, 0x0d, 0x66, 0x55, 0x7d, 0x71, 0x66, 0x1a,
                0x37, 0xc0, 0xb0, 0x66, 0xf0, 0xd6, 0x6e, 0x66
            }
        }
    };

    static const auto version = version::level::minimum;
    const auto data = expected.to_data(version);
    read::bytes::copy source(data);
    const auto result = inventory_vector::factory(version, source);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(version));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version), result.serialized_size(version));
}

BOOST_AUTO_TEST_CASE(inventory_vector__to_witness__error__unchanged)
{
    static const auto expected = inventory_vector::type_id::error;
    inventory_vector instance{ expected, {} };
    instance.to_witness();
    BOOST_REQUIRE(instance.type() == expected);
}

BOOST_AUTO_TEST_CASE(inventory_vector__to_witness__filtered_block__unchanged)
{
    static const auto expected = inventory_vector::type_id::filtered_block;
    inventory_vector instance{ expected, {} };
    instance.to_witness();
    BOOST_REQUIRE(instance.type() == expected);
}

BOOST_AUTO_TEST_CASE(inventory_vector__to_witness__compact_block__unchanged)
{
    static const auto expected = inventory_vector::type_id::compact_block;
    inventory_vector instance{ expected, {} };
    instance.to_witness();
    BOOST_REQUIRE(instance.type() == expected);
}

BOOST_AUTO_TEST_CASE(inventory_vector__to_witness__witness_transaction__unchanged)
{
    static const auto expected = inventory_vector::type_id::witness_transaction;
    inventory_vector instance{ expected, {} };
    instance.to_witness();
    BOOST_REQUIRE(instance.type() == expected);
}

BOOST_AUTO_TEST_CASE(inventory_vector__to_witness__witness_block__unchanged)
{
    static const auto expected = inventory_vector::type_id::witness_block;
    inventory_vector instance{ expected, {} };
    instance.to_witness();
    BOOST_REQUIRE(instance.type() == expected);
}

BOOST_AUTO_TEST_CASE(inventory_vector__to_witness__block__expected)
{
    inventory_vector instance{ inventory_vector::type_id::block, {} };
    instance.to_witness();
    BOOST_REQUIRE(instance.type() == inventory_vector::type_id::witness_block);
}

BOOST_AUTO_TEST_CASE(inventory_vector__to_witness__transaction__expected)
{
    inventory_vector instance{ inventory_vector::type_id::transaction, {} };
    instance.to_witness();
    BOOST_REQUIRE(instance.type() == inventory_vector::type_id::witness_transaction);
}

BOOST_AUTO_TEST_CASE(inventory_vector__is_block_type__block__returns_true)
{
    inventory_vector instance;
    instance.set_type(inventory_vector::type_id::block);
    BOOST_REQUIRE(instance.is_block_type());
}

BOOST_AUTO_TEST_CASE(inventory_vector__is_block_type__compact_block__returns_true)
{
    inventory_vector instance;
    instance.set_type(inventory_vector::type_id::compact_block);
    BOOST_REQUIRE(instance.is_block_type());
}

BOOST_AUTO_TEST_CASE(inventory_vector__is_block_type__filtered_block__returns_true)
{
    inventory_vector instance;
    instance.set_type(inventory_vector::type_id::filtered_block);
    BOOST_REQUIRE(instance.is_block_type());
}

BOOST_AUTO_TEST_CASE(inventory_vector__is_block_type__non_block_type__returns_false)
{
    inventory_vector instance;
    instance.set_type(inventory_vector::type_id::transaction);
    BOOST_REQUIRE_EQUAL(false, instance.is_block_type());
}

BOOST_AUTO_TEST_CASE(inventory_vector__is_transaction_type__transaction__returns_true)
{
    inventory_vector instance;
    instance.set_type(inventory_vector::type_id::transaction);
    BOOST_REQUIRE(instance.is_transaction_type());
}

BOOST_AUTO_TEST_CASE(inventory_vector__is_transaction_type__non_transaction_type__returns_false)
{
    inventory_vector instance;
    instance.set_type(inventory_vector::type_id::block);
    BOOST_REQUIRE_EQUAL(false, instance.is_transaction_type());
}

BOOST_AUTO_TEST_CASE(inventory_vector__type_accessor__always__returns_initialized_value)
{
    inventory_vector::type_id type = inventory_vector::type_id::transaction;
    hash_digest hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    inventory_vector instance(type, hash);
    BOOST_REQUIRE(type == instance.type());
}

BOOST_AUTO_TEST_CASE(inventory_vector__type_setter__roundtrip__success)
{
    inventory_vector::type_id type = inventory_vector::type_id::transaction;
    inventory_vector instance;
    BOOST_REQUIRE(type != instance.type());
    instance.set_type(type);
    BOOST_REQUIRE(type == instance.type());
}

BOOST_AUTO_TEST_CASE(inventory_vector__hash_accessor__always__returns_initialized_value)
{
    inventory_vector::type_id type = inventory_vector::type_id::transaction;
    hash_digest hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    inventory_vector instance(type, hash);
    BOOST_REQUIRE(hash == instance.hash());
}

BOOST_AUTO_TEST_CASE(inventory_vector__hash_setter_1__roundtrip__success)
{
    hash_digest hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    inventory_vector instance;
    BOOST_REQUIRE(hash != instance.hash());
    instance.set_hash(hash);
    BOOST_REQUIRE(hash == instance.hash());
}

BOOST_AUTO_TEST_CASE(inventory_vector__hash_setter_2__roundtrip__success)
{
    hash_digest duplicate = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    hash_digest hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    inventory_vector instance;
    BOOST_REQUIRE(duplicate != instance.hash());
    instance.set_hash(std::move(hash));
    BOOST_REQUIRE(duplicate == instance.hash());
}

BOOST_AUTO_TEST_CASE(inventory_vector__operator_assign_equals_1__always__matches_equivalent)
{
    inventory_vector value(
        inventory_vector::type_id::compact_block,
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b")
    );

    BOOST_REQUIRE(value.is_valid());

    inventory_vector instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
}

BOOST_AUTO_TEST_CASE(inventory_vector__operator_assign_equals_2__always__matches_equivalent)
{
    inventory_vector value(
        inventory_vector::type_id::compact_block,
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b")
    );

    BOOST_REQUIRE(value.is_valid());

    inventory_vector instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());

    instance = value;
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(value == instance);
}

BOOST_AUTO_TEST_CASE(inventory_vector__operator_boolean_equals__duplicates__returns_true)
{
    const inventory_vector expected(
        inventory_vector::type_id::filtered_block,
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b")
    );

    inventory_vector instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(inventory_vector__operator_boolean_equals__differs__returns_false)
{
    const inventory_vector expected(
        inventory_vector::type_id::filtered_block,
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b")
    );

    inventory_vector instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(reject__operator_boolean_not_equals__duplicates__returns_false)
{
    const inventory_vector expected(
        inventory_vector::type_id::filtered_block,
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b")
    );

    inventory_vector instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(reject__operator_boolean_not_equals__differs__returns_true)
{
    const inventory_vector expected(
        inventory_vector::type_id::filtered_block,
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b")
    );

    inventory_vector instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()
