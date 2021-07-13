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

// Sponsored in part by Digital Contract Design, LLC

#include "../test.hpp"

BOOST_AUTO_TEST_SUITE(block_filter_tests)

BOOST_AUTO_TEST_CASE(block_filter__constructor_1__always__invalid)
{
    chain::block_filter instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());
}

BOOST_AUTO_TEST_CASE(block_filter__constructor_2__always__equals_params)
{
    const uint8_t filter_type = 16u;
    const auto block_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const auto header = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    const auto filter = base16_chunk("0123456789abcdef");
    chain::block_filter instance(filter_type, block_hash, header, filter);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(filter_type == instance.filter_type());
    BOOST_REQUIRE(block_hash == instance.block_hash());
    BOOST_REQUIRE(header == instance.header());
    BOOST_REQUIRE(filter == instance.filter());
}

BOOST_AUTO_TEST_CASE(block_filter__constructor_3__always__equals_params)
{
    const uint8_t filter_type = 16u;
    const auto block_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    auto dup_hash = block_hash;
    const auto header = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    auto dup_header = header;
    const auto filter = base16_chunk("0123456789abcdef");
    auto dup_filter = filter;
    chain::block_filter instance(filter_type, std::move(dup_hash), std::move(dup_header), std::move(dup_filter));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(filter_type == instance.filter_type());
    BOOST_REQUIRE(block_hash == instance.block_hash());
    BOOST_REQUIRE(header == instance.header());
    BOOST_REQUIRE(filter == instance.filter());
}

BOOST_AUTO_TEST_CASE(block_filter__constructor_4__always__equals_params)
{
    const uint8_t filter_type = 16u;
    const auto block_hash = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    const auto header = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const auto filter = base16_chunk("fedcba9876543210");
    chain::block_filter value(filter_type, block_hash, header, filter);
    chain::block_filter instance(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(filter_type == instance.filter_type());
    BOOST_REQUIRE(block_hash == instance.block_hash());
    BOOST_REQUIRE(header == instance.header());
    BOOST_REQUIRE(filter == instance.filter());
}

BOOST_AUTO_TEST_CASE(block_filter__constructor_5__always__equals_params)
{
    const uint8_t filter_type = 16u;
    const auto block_hash = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    const auto header = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const auto filter = base16_chunk("fedcba9876543210");
    chain::block_filter value(filter_type, block_hash, header, filter);
    chain::block_filter instance(std::move(value));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(filter_type == instance.filter_type());
    BOOST_REQUIRE(block_hash == instance.block_hash());
    BOOST_REQUIRE(header == instance.header());
    BOOST_REQUIRE(filter == instance.filter());
}

BOOST_AUTO_TEST_CASE(block_filter__filter_type_accessor__always__returns_initialized_value)
{
    const uint8_t filter_type = 55u;
    const auto block_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const auto header = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    const auto filter = base16_chunk("0123456789abcdef");
    chain::block_filter instance(filter_type, block_hash, header, filter);
    BOOST_REQUIRE(filter_type == instance.filter_type());
}

BOOST_AUTO_TEST_CASE(block_filter__filter_type_setter__roundtrip__success)
{
    const uint8_t filter_type = 38u;
    chain::block_filter instance;
    BOOST_REQUIRE(filter_type != instance.filter_type());

    instance.set_filter_type(filter_type);
    BOOST_REQUIRE(filter_type == instance.filter_type());
}

BOOST_AUTO_TEST_CASE(block_filter__block_hash_accessor__always__returns_initialized_value)
{
    const uint8_t filter_type = 55u;
    const auto block_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const auto header = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    const auto filter = base16_chunk("0123456789abcdef");
    const chain::block_filter instance(filter_type, block_hash, header, filter);
    BOOST_REQUIRE(block_hash == instance.block_hash());
}

BOOST_AUTO_TEST_CASE(block_filter__block_hash_setter_1__roundtrip__success)
{
    const auto block_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    chain::block_filter instance;
    BOOST_REQUIRE(block_hash != instance.block_hash());

    instance.set_block_hash(block_hash);
    BOOST_REQUIRE(block_hash == instance.block_hash());
}

BOOST_AUTO_TEST_CASE(block_filter__block_hash_setter_2__roundtrip__success)
{
    const auto block_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    auto dup = block_hash;
    chain::block_filter instance;
    BOOST_REQUIRE(block_hash != instance.block_hash());

    instance.set_block_hash(std::move(dup));
    BOOST_REQUIRE(block_hash == instance.block_hash());
}

BOOST_AUTO_TEST_CASE(block_filter__header_accessor__always__returns_initialized_value)
{
    const uint8_t filter_type = 55u;
    const auto block_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const auto header = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    const auto filter = base16_chunk("0123456789abcdef");
    const chain::block_filter instance(filter_type, block_hash, header, filter);
    BOOST_REQUIRE(header == instance.header());
}

BOOST_AUTO_TEST_CASE(block_filter__header_setter_1__roundtrip__success)
{
    const auto header = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    chain::block_filter instance;
    BOOST_REQUIRE(header != instance.header());

    instance.set_header(header);
    BOOST_REQUIRE(header == instance.header());
}

BOOST_AUTO_TEST_CASE(block_filter__header_setter_2__roundtrip__success)
{
    const auto header = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    auto dup = header;
    chain::block_filter instance;
    BOOST_REQUIRE(header != instance.header());

    instance.set_header(std::move(dup));
    BOOST_REQUIRE(header == instance.header());
}

BOOST_AUTO_TEST_CASE(block_filter__filter_accessor__always__returns_initialized_value)
{
    const uint8_t filter_type = 55u;
    const auto block_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const auto header = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    const auto filter = base16_chunk("0123456789abcdef");
    const chain::block_filter instance(filter_type, block_hash, header, filter);
    BOOST_REQUIRE(filter == instance.filter());
}

BOOST_AUTO_TEST_CASE(block_filter__filter_setter_1__roundtrip__success)
{
    const auto filter = base16_chunk("0123456789abcdef");
    chain::block_filter instance;
    BOOST_REQUIRE(filter != instance.filter());

    instance.set_filter(filter);
    BOOST_REQUIRE(filter == instance.filter());
}

BOOST_AUTO_TEST_CASE(block_filter__filter_setter_2__roundtrip__success)
{
    const auto filter = base16_chunk("0123456789abcdef");
    auto dup = filter;
    chain::block_filter instance;
    BOOST_REQUIRE(filter != instance.filter());

    instance.set_filter(std::move(dup));
    BOOST_REQUIRE(filter == instance.filter());
}

BOOST_AUTO_TEST_CASE(block_filter__operator_assign_equals__always__matches_equivalent)
{
    const uint8_t filter_type = 55u;
    const auto block_hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const auto header = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    const auto filter = base16_chunk("0123456789abcdef");
    chain::block_filter value(filter_type, block_hash, header, filter);
    BOOST_REQUIRE(value.is_valid());

    chain::block_filter instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(filter_type == instance.filter_type());
    BOOST_REQUIRE(block_hash == instance.block_hash());
    BOOST_REQUIRE(header == instance.header());
    BOOST_REQUIRE(filter == instance.filter());
}

BOOST_AUTO_TEST_CASE(block_filter__operator_boolean_equals__duplicates__returns_true)
{
    const chain::block_filter expected
    {
        19u,
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        base16_chunk("0123456789abcdef")
    };
    chain::block_filter instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(block_filter__operator_boolean_equals__differs__returns_false)
{
    const chain::block_filter expected
    {
        19u,
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        base16_chunk("0123456789abcdef")
    };
    chain::block_filter instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(block_filter__operator_boolean_not_equals__duplicates__returns_false)
{
    const chain::block_filter expected
    {
        19u,
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        base16_chunk("0123456789abcdef")
    };
    chain::block_filter instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(block_filter__operator_boolean_not_equals__differs__returns_true)
{
    const chain::block_filter expected
    {
        19u,
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        base16_chunk("0123456789abcdef")
    };
    chain::block_filter instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()
