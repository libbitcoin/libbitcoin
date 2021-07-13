/**
 * Copyright (c) 2011-2021 libbitcoin developers (see AUTHORS)
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

BOOST_AUTO_TEST_SUITE(chain_check_point_tests)

using namespace bc::system::chain;

BOOST_AUTO_TEST_CASE(check_point__constructor_default__always__null_hash_zero)
{
    const check_point instance;
    BOOST_REQUIRE_EQUAL(instance.hash(), null_hash);
    BOOST_REQUIRE_EQUAL(instance.height(), zero);
}

BOOST_AUTO_TEST_CASE(check_point__constructor_copy__always__expected)
{
    const check_point instance1("0102030405060708090a0102030405060708090a0102030405060708090a0b0c", 42);
    const check_point instance2(instance1);
    BOOST_REQUIRE_EQUAL(instance1, instance2);
    BOOST_REQUIRE_EQUAL(instance1.hash(), instance2.hash());
    BOOST_REQUIRE_EQUAL(instance1.height(), instance2.height());
}

BOOST_AUTO_TEST_CASE(check_point__constructor_move__always__expected)
{
    check_point instance1("0102030405060708090a0102030405060708090a0102030405060708090a0b0c", 42);
    const check_point instance2(instance1);
    const check_point instance3(std::move(instance1));
    BOOST_REQUIRE_EQUAL(instance2.hash(), instance3.hash());
    BOOST_REQUIRE_EQUAL(instance2.height(), instance3.height());
}

BOOST_AUTO_TEST_CASE(check_point__constructor_text__valid__expected)
{
    const auto height = 42u;
    const char text[] = "0102030405060708090a0102030405060708090a0102030405060708090a0b0c";
    const check_point instance(text, height);
    BOOST_REQUIRE_EQUAL(instance.hash(), base16_hash(text));
    BOOST_REQUIRE_EQUAL(instance.height(), height);
}

BOOST_AUTO_TEST_CASE(check_point__constructor_text__invalid__expected)
{
    const auto height = 42u;
    const char text[] = "zzzz030405060708090a0102030405060708090a0102030405060708090a0b0c";
    const check_point instance(text, height);
    BOOST_REQUIRE_EQUAL(instance.hash(), null_hash);
    BOOST_REQUIRE_EQUAL(instance.height(), zero);
}

BOOST_AUTO_TEST_CASE(check_point__constructor_string__valid__expected)
{
    const auto height = 42u;
    const char text[] = "0102030405060708090a0102030405060708090a0102030405060708090a0b0c";
    const std::string value{ text };
    const check_point instance(value, height);
    BOOST_REQUIRE_EQUAL(instance.hash(), base16_hash(text));
    BOOST_REQUIRE_EQUAL(instance.height(), height);
}

BOOST_AUTO_TEST_CASE(check_point__constructor_string__invalid__expected)
{
    const auto height = 42u;
    const std::string value{ "dfjgsdrhugidsnfvinsdfvi" };
    const check_point instance(value, height);
    BOOST_REQUIRE_EQUAL(instance.hash(), null_hash);
    BOOST_REQUIRE_EQUAL(instance.height(), zero);
}

BOOST_AUTO_TEST_CASE(check_point__constructor_copy_hash__always__expected)
{
    const auto height = 42u;
    const char text[] = "0102030405060708090a0102030405060708090a0102030405060708090a0b0c";
    const auto hash = base16_hash(text);
    const check_point instance(hash, height);
    BOOST_REQUIRE_EQUAL(instance.hash(), hash);
    BOOST_REQUIRE_EQUAL(instance.height(), height);
}

BOOST_AUTO_TEST_CASE(check_point__constructor_move_hash__always__expected)
{
    const auto height = 42u;
    const char text[] = "0102030405060708090a0102030405060708090a0102030405060708090a0b0c";
    const check_point instance(base16_hash(text), height);
    BOOST_REQUIRE_EQUAL(instance.hash(), base16_hash(text));
    BOOST_REQUIRE_EQUAL(instance.height(), height);
}

BOOST_AUTO_TEST_CASE(check_point__assign_copy__always__expected)
{
    const check_point instance1("0102030405060708090a0102030405060708090a0102030405060708090a0b0c", 42);
    const check_point instance2 = instance1;
    BOOST_REQUIRE_EQUAL(instance1, instance2);
    BOOST_REQUIRE_EQUAL(instance1.hash(), instance2.hash());
    BOOST_REQUIRE_EQUAL(instance1.height(), instance2.height());
}

BOOST_AUTO_TEST_CASE(check_point__assign_move__always__expected)
{
    check_point instance1("0102030405060708090a0102030405060708090a0102030405060708090a0b0c", 42);
    const check_point instance2(instance1);
    const check_point instance3 = std::move(instance1);
    BOOST_REQUIRE_EQUAL(instance2.hash(), instance3.hash());
    BOOST_REQUIRE_EQUAL(instance2.height(), instance3.height());
}

BOOST_AUTO_TEST_CASE(check_point__lesser__same__false)
{
    const check_point instance1("0102030405060708090a0102030405060708090a0102030405060708090a0b0c", 42);
    const check_point instance2("0102030405060708090a0102030405060708090a0102030405060708090a0b0c", 42);
    BOOST_REQUIRE(!(instance1 < instance2));
}

BOOST_AUTO_TEST_CASE(check_point__lesser__lesser_hash_same_height__false)
{
    const check_point instance1(null_hash, 42);
    const check_point instance2("0102030405060708090a0102030405060708090a0102030405060708090a0b0c", 42);
    BOOST_REQUIRE(!(instance1 < instance2));
}

BOOST_AUTO_TEST_CASE(check_point__lesser__same_hash_lesser_height__true)
{
    const check_point instance1("0102030405060708090a0102030405060708090a0102030405060708090a0b0c", 41);
    const check_point instance2("0102030405060708090a0102030405060708090a0102030405060708090a0b0c", 42);
    BOOST_REQUIRE(instance1 < instance2);
}

BOOST_AUTO_TEST_CASE(check_point__lesser__same_hash_greater_height__false)
{
    const check_point instance1("0102030405060708090a0102030405060708090a0102030405060708090a0b0c", 43);
    const check_point instance2("0102030405060708090a0102030405060708090a0102030405060708090a0b0c", 42);
    BOOST_REQUIRE(!(instance1 < instance2));
}

BOOST_AUTO_TEST_CASE(check_point__equality__same__expected)
{
    const check_point instance1("0102030405060708090a0102030405060708090a0102030405060708090a0b0c", 42);
    const check_point instance2("0102030405060708090a0102030405060708090a0102030405060708090a0b0c", 42);
    BOOST_REQUIRE(instance1 == instance2);
    BOOST_REQUIRE(!(instance1 != instance2));
}

BOOST_AUTO_TEST_CASE(check_point__equality__different__expected)
{
    const check_point instance1("0102030405060708090a0102030405060708090a0102030405060708090a0b0c", 42);
    const check_point instance2;
    BOOST_REQUIRE(instance1 != instance2);
    BOOST_REQUIRE(!(instance1 == instance2));
}

BOOST_AUTO_TEST_SUITE_END()
