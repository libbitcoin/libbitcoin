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

BOOST_AUTO_TEST_SUITE(not_found_tests)

using namespace bc::system::message;

BOOST_AUTO_TEST_CASE(not_found__constructor_1__always__invalid)
{
    message::not_found instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());
}

BOOST_AUTO_TEST_CASE(not_found__constructor_2__always__equals_params)
{
    const message::inventory_vector::list values =
    {
        message::inventory_vector(
            inventory::type_id::error,
            {
                {
                    0x44, 0x9a, 0x0d, 0x24, 0x9a, 0xd5, 0x39, 0x89,
                    0xbb, 0x85, 0x0a, 0x3d, 0x79, 0x24, 0xed, 0x0f,
                    0xc3, 0x0d, 0x6f, 0x55, 0x7d, 0x71, 0x12, 0x1a,
                    0x37, 0xc0, 0xb0, 0x32, 0xf0, 0xd6, 0x6e, 0xdf
                }
            }
        )
    };

    message::not_found instance(values);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(values == instance.inventories());
}

BOOST_AUTO_TEST_CASE(not_found__constructor_3__always__equals_params)
{
    message::inventory_vector::type_id type = message::inventory_vector::type_id::error;
    auto hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    message::inventory_vector::list values =
    {
        message::inventory_vector(type, hash)
    };

    message::not_found instance(std::move(values));
    BOOST_REQUIRE(instance.is_valid());
    auto inventories = instance.inventories();
    BOOST_REQUIRE_EQUAL(1u, inventories.size());
    BOOST_REQUIRE(type == inventories[0].type());
    BOOST_REQUIRE(hash == inventories[0].hash());
}

BOOST_AUTO_TEST_CASE(not_found__constructor_4__always__equals_params)
{
    message::inventory_vector::type_id type = message::inventory_vector::type_id::error;
    auto hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const hash_list hashes = { hash };

    message::not_found instance(hashes, type);
    BOOST_REQUIRE(instance.is_valid());
    auto inventories = instance.inventories();
    BOOST_REQUIRE_EQUAL(1u, inventories.size());
    BOOST_REQUIRE(type == inventories[0].type());
    BOOST_REQUIRE(hash == inventories[0].hash());
}

BOOST_AUTO_TEST_CASE(not_found__constructor_5__always__equals_params)
{
    message::inventory_vector::type_id type = message::inventory_vector::type_id::error;
    auto hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    message::not_found instance{ { type, hash } };
    BOOST_REQUIRE(instance.is_valid());
    auto inventories = instance.inventories();
    BOOST_REQUIRE_EQUAL(1u, inventories.size());
    BOOST_REQUIRE(type == inventories[0].type());
    BOOST_REQUIRE(hash == inventories[0].hash());
}

BOOST_AUTO_TEST_CASE(not_found__constructor_6__always__equals_params)
{
    message::inventory_vector::type_id type = message::inventory_vector::type_id::error;
    auto hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    const message::not_found value{ { type, hash } };
    BOOST_REQUIRE(value.is_valid());
    message::not_found instance(value);
    auto inventories = instance.inventories();
    BOOST_REQUIRE_EQUAL(1u, inventories.size());
    BOOST_REQUIRE(type == inventories[0].type());
    BOOST_REQUIRE(hash == inventories[0].hash());
    BOOST_REQUIRE(value == instance);
}

BOOST_AUTO_TEST_CASE(not_found__constructor_7__always__equals_params)
{
    message::inventory_vector::type_id type = message::inventory_vector::type_id::error;
    auto hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    message::not_found value{ { type, hash } };
    BOOST_REQUIRE(value.is_valid());
    message::not_found instance(std::move(value));
    auto inventories = instance.inventories();
    BOOST_REQUIRE_EQUAL(1u, inventories.size());
    BOOST_REQUIRE(type == inventories[0].type());
    BOOST_REQUIRE(hash == inventories[0].hash());
}

BOOST_AUTO_TEST_CASE(not_found__from_data__insufficient_bytes__failure)
{
    static const data_chunk raw{ 0xab, 0xcd };
    not_found instance;
    BOOST_REQUIRE_EQUAL(false, instance.from_data(version::level::minimum, raw));
}

BOOST_AUTO_TEST_CASE(not_found__from_data__insufficient_version__failure)
{
    static const not_found expected
    {
        {
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
            }
        }
    };

    const auto version = version::level::maximum;
    const data_chunk raw = expected.to_data(version);
    not_found instance;
    BOOST_REQUIRE_EQUAL(false, instance.from_data(not_found::version_minimum - 1, raw));
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());
}

BOOST_AUTO_TEST_CASE(not_found__factory_1__valid_input__success)
{
    static const not_found expected
    {
        {
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
            }
        }
    };

    const auto version = version::level::maximum;
    const auto data = expected.to_data(version);
    const auto result = not_found::factory(version, data);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(version));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version), result.serialized_size(version));
}

BOOST_AUTO_TEST_CASE(not_found__factory_2__valid_input__success)
{
    static const not_found expected
    {
        {
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
            }
        }
    };

    const auto version = version::level::maximum;
    const auto data = expected.to_data(version);
    stream::in::copy istream(data);
    const auto result = not_found::factory(version, istream);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(version));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version), result.serialized_size(version));
}

BOOST_AUTO_TEST_CASE(not_found__factory_3__valid_input__success)
{
    static const not_found expected
    {
        {
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
            }
        }
    };

    const auto version = version::level::maximum;
    const auto data = expected.to_data(version);
    read::bytes::copy source(data);
    const auto result = not_found::factory(version, source);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(version));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version), result.serialized_size(version));
}

BOOST_AUTO_TEST_CASE(not_found__operator_assign_equals__always__matches_equivalent)
{
    const message::inventory_vector::list elements =
    {
        message::inventory_vector(message::inventory_vector::type_id::error,
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"))
    };

    message::not_found value(elements);
    BOOST_REQUIRE(value.is_valid());

    message::not_found instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(elements == instance.inventories());
}

BOOST_AUTO_TEST_CASE(not_found__operator_boolean_equals__duplicates__returns_true)
{
    const message::not_found expected(
    {
        message::inventory_vector(message::inventory_vector::type_id::error,
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"))
    });

    message::not_found instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(not_found__operator_boolean_equals__differs__returns_false)
{
    const message::not_found expected(
    {
        message::inventory_vector(message::inventory_vector::type_id::error,
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"))
    });

    message::not_found instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(not_found__operator_boolean_not_equals__duplicates__returns_false)
{
    const message::not_found expected(
    {
        message::inventory_vector(message::inventory_vector::type_id::error,
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"))
    });

    message::not_found instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(not_found__operator_boolean_not_equals__differs__returns_true)
{
    const message::not_found expected(
    {
        message::inventory_vector(message::inventory_vector::type_id::error,
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"))
    });

    message::not_found instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()
