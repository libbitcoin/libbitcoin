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
#include "../../test.hpp"
#include <tuple>
#include <utility>

BOOST_AUTO_TEST_SUITE(checked_tests)

using namespace bc::system::wallet;

typedef checked<0, 0, 0> zero_checked;
typedef checked<1, mini_hash_size, 2> mini_checked;
typedef checked<3, short_hash_size, 4> short_checked;
typedef checked<5, long_hash_size, 8> long_checked;

const data_array<0> zero_prefix{};
const data_array<1> mini_prefix{ 1 };
const data_array<3> short_prefix{ 1, 2, 3 };
const data_array<5> long_prefix{ 1, 2, 3, 4, 5 };

const data_array<0> test_zero_hash = hash_literal("");
const mini_hash test_mini_hash = hash_literal("000102030405");
const short_hash test_short_hash = hash_literal("0908070605040302010009080706050403020100");
const long_hash test_long_hash = hash_literal("09080706050403020100090807060504030201000908070605040302010009080706050403020100090807060504030201000908070605040302010003020100");

const auto zero_valid_value = insert_checksum<zero_checked::value_size, 0>({ zero_prefix,  test_zero_hash });
const auto mini_valid_value = insert_checksum<mini_checked::value_size, 2>({ mini_prefix,  test_mini_hash });
const auto short_valid_value = insert_checksum<short_checked::value_size, 4>({ short_prefix, test_short_hash });
const auto long_valid_value = insert_checksum<long_checked::value_size, 8>({ long_prefix,  test_long_hash });

BOOST_AUTO_TEST_CASE(checked__default_construct__zero__valid)
{
    // Empty checksum and empty value, so no state and always valid.
    BOOST_REQUIRE(zero_checked());
}

BOOST_AUTO_TEST_CASE(checked__default_construct__non_empty_always__invalid)
{
    // Zeroed value and checksum implies invalid checksum.
    BOOST_REQUIRE(!mini_checked());
    BOOST_REQUIRE(!short_checked());
    BOOST_REQUIRE(!long_checked());
}

BOOST_AUTO_TEST_CASE(checked__values_construct__zero__valid)
{
    // Empty checksum and empty value, so no state and always valid.
    const zero_checked zero_instance({}, {});
    BOOST_REQUIRE(zero_instance);

    const mini_checked mini_instance({ 0 }, null_mini_hash);
    BOOST_REQUIRE(mini_instance);

    const short_checked short_instance({ 0, 0, 0 }, null_short_hash);
    BOOST_REQUIRE(short_instance);

    const long_checked long_instance({ 0, 0, 0, 0, 0 }, null_long_hash);
    BOOST_REQUIRE(long_instance);
}

// Excluded by type constraints on split.
//////BOOST_AUTO_TEST_CASE(checked__value_copy_construct__zero__valid)
//////{
//////    // Empty checksum and empty value, so no state and always valid.
//////    const zero_checked instance(zero_valid_value);
//////    BOOST_REQUIRE(instance);
//////}

BOOST_AUTO_TEST_CASE(checked__value_copy_construct__mini_invalid__invalid)
{
    const mini_checked instance(mini_valid_value);
    BOOST_REQUIRE(instance);
}

BOOST_AUTO_TEST_CASE(checked__value_copy_construct__mini__valid)
{
    const mini_checked instance(mini_valid_value);
    BOOST_REQUIRE(instance);
}

BOOST_AUTO_TEST_CASE(checked__copy_construct__long__valid)
{
    const long_checked instance(long_valid_value);
    const long_checked copy(instance);
    BOOST_REQUIRE(copy);
}

BOOST_AUTO_TEST_CASE(checked__value_move_construct__short__valid)
{
    short_checked instance(short_valid_value);
    const short_checked copy(std::move(instance));
    BOOST_REQUIRE(copy);
}

BOOST_AUTO_TEST_CASE(checked__move_construct__long__valid)
{
    long_checked instance(long_valid_value);
    const long_checked copy(std::move(instance));
    BOOST_REQUIRE(copy);
}

BOOST_AUTO_TEST_CASE(checked__copy_assign__short__valid)
{
    const short_checked instance(short_valid_value);
    short_checked copy;
    copy = instance;
    BOOST_REQUIRE(copy);
}

BOOST_AUTO_TEST_CASE(checked__move_assign__short__valid)
{
    short_checked instance(short_valid_value);
    short_checked copy;
    copy = std::move(instance);
    BOOST_REQUIRE(copy);
}

// Whiney compiler (can't test self-assign).
////BOOST_AUTO_TEST_CASE(checked__copy_assign_self__short__valid)
////{
////    short_checked instance(short_valid_value);
////    instance = instance;
////    BOOST_REQUIRE(instance);
////}

// Whiney compiler (can't test self-assign).
////BOOST_AUTO_TEST_CASE(checked__move_assign_self__short__valid)
////{
////    short_checked instance(short_valid_value);
////    instance = std::move(instance);
////    BOOST_REQUIRE(instance);
////}

BOOST_AUTO_TEST_CASE(checked__properies__zero__expected)
{
    const zero_checked zero_instance({}, {});
    BOOST_REQUIRE(zero_instance);
    BOOST_REQUIRE(zero_instance.prefix() == zero_checked::prefix_type{});
    BOOST_REQUIRE(zero_instance.payload() == zero_checked::payload_type{});
    BOOST_REQUIRE_EQUAL(encode_base16(zero_instance.checksum()), "");
}

BOOST_AUTO_TEST_CASE(checked__properies__mini__expected)
{
    const mini_checked mini_instance({ 42 }, test_mini_hash);
    BOOST_REQUIRE(mini_instance);
    const auto expected_prefix = mini_checked::prefix_type{ 42 };
    BOOST_REQUIRE(mini_instance.prefix() == expected_prefix);
    BOOST_REQUIRE(mini_instance.payload() == test_mini_hash);
    BOOST_REQUIRE_EQUAL(encode_base16(mini_instance.checksum()), "6a7b");
}

BOOST_AUTO_TEST_CASE(checked__properies__short__expected)
{
    const short_checked short_instance({ 42, 24, 0 }, test_short_hash);
    BOOST_REQUIRE(short_instance);
    const auto expected_prefix = short_checked::prefix_type{ 42, 24 };
    BOOST_REQUIRE(short_instance.prefix() == expected_prefix);
    BOOST_REQUIRE(short_instance.payload() == test_short_hash);
    BOOST_REQUIRE_EQUAL(encode_base16(short_instance.checksum()), "77a9ccbb");
}

BOOST_AUTO_TEST_CASE(checked__properies__long__expected)
{
    const long_checked long_instance({ 1, 24, 42, 2, 0 }, test_long_hash);
    BOOST_REQUIRE(long_instance);
    const auto expected_prefix = long_checked::prefix_type{ 1, 24, 42, 2, 0 };
    BOOST_REQUIRE(long_instance.prefix() == expected_prefix);
    BOOST_REQUIRE(long_instance.payload() == test_long_hash);
    BOOST_REQUIRE_EQUAL(encode_base16(long_instance.checksum()), "2aed29c75b84cdaf");
}

BOOST_AUTO_TEST_CASE(checked__equality__same__true)
{
    const long_checked long_instance({ 1, 24, 42, 2, 0 }, test_long_hash);
    BOOST_REQUIRE(long_instance);
    const auto expected_prefix = long_checked::prefix_type{ 1, 24, 42, 2, 0 };
    BOOST_REQUIRE(long_instance.prefix() == expected_prefix);
    BOOST_REQUIRE(long_instance.payload() == test_long_hash);
    BOOST_REQUIRE_EQUAL(encode_base16(long_instance.checksum()), "2aed29c75b84cdaf");
}

BOOST_AUTO_TEST_SUITE_END()
