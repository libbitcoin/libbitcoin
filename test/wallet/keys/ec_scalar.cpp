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
#include <utility>

BOOST_AUTO_TEST_SUITE(ec_scalar_tests)

const ec_secret secret1a = base16_literal("b9d22ea0ed7e3d53cc9e1b3d436bb9260fc3211634c821f6bb3cb3a73ee7a0b5");
const ec_secret secret1b = base16_literal("f9416cd856d81a00366ad7234a7b5e5dbda61812e56effcb4cde0695bc092380");
const ec_secret sum1 = base16_literal("b3139b79445657540308f2608de7178512ba5c426aee818648485bb02aba82f4");
const ec_secret product1 = base16_literal("aba800424dd618d36a42bf6d6d3156a38a74298b9e4519bbba34cf0d7dce4d4f");

const ec_secret negative1 = base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140");
const ec_secret positive1 = base16_literal("0000000000000000000000000000000000000000000000000000000000000001");
const ec_secret positive2 = base16_literal("0000000000000000000000000000000000000000000000000000000000000002");
const ec_secret positive3 = base16_literal("0000000000000000000000000000000000000000000000000000000000000003");
const ec_secret positive42 = base16_literal("000000000000000000000000000000000000000000000000000000000000002a");

const ec_secret secret2 = base16_literal("8010b1bb119ad37d4b65a1022a314897b1b3614b345974332cb1b9582cf03536");
const ec_compressed compressed2 = base16_literal("0309ba8621aefd3b6ba4ca6d11a4746e8df8d35d9b51b383338f627ba7fc732731");

// construct

BOOST_AUTO_TEST_CASE(ec_scalar__construct__default__false_zero)
{
    const ec_scalar value;
    BOOST_REQUIRE(!value);
    BOOST_REQUIRE_EQUAL(value, int64_t{ 0 });
}

BOOST_AUTO_TEST_CASE(ec_scalar__construct__1__true_1)
{
    const ec_scalar value(1);
    BOOST_REQUIRE(value);
    BOOST_REQUIRE_EQUAL(value, int64_t{ 1 });
}

BOOST_AUTO_TEST_CASE(ec_scalar__construct__copy_ec_scalar__expected)
{
    const ec_scalar value(positive42);
    BOOST_REQUIRE(value);

    const ec_scalar copy(value);
    BOOST_REQUIRE(copy);
    BOOST_REQUIRE_EQUAL(copy, positive42);
}

BOOST_AUTO_TEST_CASE(ec_scalar__construct__copy_int64__expected)
{
    const ec_scalar value(42);
    BOOST_REQUIRE(value);
    BOOST_REQUIRE_EQUAL(value, positive42);

    const ec_scalar copy(value);
    BOOST_REQUIRE(copy);
    BOOST_REQUIRE_EQUAL(copy, value);
}

BOOST_AUTO_TEST_CASE(ec_scalar__construct__move_ec_scalar__moved)
{
    ec_scalar value(positive42);
    BOOST_REQUIRE(value);

    const ec_scalar copy(std::move(value));
    BOOST_REQUIRE(copy);
    BOOST_REQUIRE_EQUAL(copy, positive42);
}

BOOST_AUTO_TEST_CASE(ec_scalar__construct__move_int64__moved)
{
    ec_scalar value(42);
    BOOST_REQUIRE(value);

    const ec_scalar copy(std::move(value));
    BOOST_REQUIRE(copy);
    BOOST_REQUIRE_EQUAL(copy, positive42);
}

BOOST_AUTO_TEST_CASE(ec_scalar__construct__ec_secret_copy__expected)
{
    const ec_scalar value(positive42);
    BOOST_REQUIRE(value);
    BOOST_REQUIRE_EQUAL(value, positive42);
}

BOOST_AUTO_TEST_CASE(ec_scalar__construct__ec_secret_move__moved)
{
    auto copy = positive42;
    const ec_scalar value(std::move(copy));
    BOOST_REQUIRE(value);
    BOOST_REQUIRE_EQUAL(value, positive42);
}

BOOST_AUTO_TEST_CASE(ec_scalar__construct__int64_zero__false_zero)
{
    const ec_scalar value(0);
    BOOST_REQUIRE(!value);
    BOOST_REQUIRE_EQUAL(value, int64_t{ 0 });
}

BOOST_AUTO_TEST_CASE(ec_scalar__construct__int64_42__true_42)
{
    const ec_scalar value(42);
    BOOST_REQUIRE(value);
    BOOST_REQUIRE_EQUAL(value, int64_t{ 42 });
}

BOOST_AUTO_TEST_CASE(ec_scalar__construct__min_int64__true_min_int64)
{
    const ec_scalar value(bc::min_int64);
    BOOST_REQUIRE(value);
    BOOST_REQUIRE_EQUAL(value, bc::min_int64);
}

BOOST_AUTO_TEST_CASE(ec_scalar__construct__max_int64__true_max_int64)
{
    const ec_scalar value(bc::max_int64);
    BOOST_REQUIRE(value);
    BOOST_REQUIRE_EQUAL(value, bc::max_int64);
}

// secret

BOOST_AUTO_TEST_CASE(ec_scalar__secret__secret__expected)
{
    ec_scalar value(positive42);
    BOOST_REQUIRE_EQUAL(value.secret(), positive42);
}

BOOST_AUTO_TEST_CASE(ec_scalar__secret__int64__expected)
{
    ec_scalar value(42);
    BOOST_REQUIRE_EQUAL(value.secret(), positive42);
}

// assignment

// Whiney compiler.
////BOOST_AUTO_TEST_CASE(ec_scalar__copy_assignment__self__expected)
////{
////    ec_scalar value(positive42);
////    value = value;
////    BOOST_REQUIRE_EQUAL(value, positive42);
////}

// Whiney compiler.
////BOOST_AUTO_TEST_CASE(ec_scalar__move_assignment__self__expected)
////{
////    ec_scalar value(positive42);
////    value = std::move(value);
////    BOOST_REQUIRE_EQUAL(value, positive42);
////}

BOOST_AUTO_TEST_CASE(ec_scalar__copy_scalar_assignment__positive__expected)
{
    const ec_scalar value(positive42);
    ec_scalar copy = value;
    BOOST_REQUIRE_EQUAL(copy, value);
}

BOOST_AUTO_TEST_CASE(ec_scalar__move_scalar_assignment__positive__moved)
{
    auto value = ec_scalar(positive42);
    auto copy = std::move(value);
    BOOST_REQUIRE_EQUAL(copy, value);
}

BOOST_AUTO_TEST_CASE(ec_scalar__copy_secret_assignment__positive__expected)
{
    const ec_scalar value(positive42);
    ec_scalar copy = value;
    BOOST_REQUIRE_EQUAL(copy, value);
}

BOOST_AUTO_TEST_CASE(ec_scalar__move_secret_assignment__positive__moved)
{
    auto value = ec_secret(positive42);
    ec_scalar copy;
    copy = std::move(value);
    BOOST_REQUIRE_EQUAL(copy, positive42);
}

BOOST_AUTO_TEST_CASE(ec_scalar__int64_assignment__negative__expected)
{
    ec_scalar value;
    value = -42;
    BOOST_REQUIRE_EQUAL(value, -ec_scalar(positive42));
}

// equality

BOOST_AUTO_TEST_CASE(ec_scalar__equality__negatives__true)
{
    const ec_scalar x(-1);
    const ec_scalar y(negative1);
    BOOST_REQUIRE(x == y);
    BOOST_REQUIRE(!(y != x));
}

BOOST_AUTO_TEST_CASE(ec_scalar__equality__zeros__true)
{
    const ec_scalar x;
    const ec_scalar y(0);
    BOOST_REQUIRE(x == y);
    BOOST_REQUIRE(!(y != x));
}

BOOST_AUTO_TEST_CASE(ec_scalar__equality__positives__true)
{
    const ec_scalar x(42);
    const ec_scalar y(positive42);
    BOOST_REQUIRE(x == y);
    BOOST_REQUIRE(!(y != x));
}

BOOST_AUTO_TEST_CASE(ec_scalar__equality__int64__true)
{
    const int64_t y = 42;
    const ec_scalar x(y);
    BOOST_REQUIRE_EQUAL(x, y);
    BOOST_REQUIRE_EQUAL(y, x);
}

// inequality

BOOST_AUTO_TEST_CASE(ec_scalar__inequality__negative_positive__false)
{
    const ec_scalar x(-1);
    const ec_scalar y(+1);
    BOOST_REQUIRE(!(y == x));
    BOOST_REQUIRE(x != y);
}

BOOST_AUTO_TEST_CASE(ec_scalar__inequality__maximum_minimum__false)
{
    const ec_scalar x(bc::max_int64);
    const ec_scalar y(bc::min_int64);
    BOOST_REQUIRE(!(y == x));
    BOOST_REQUIRE(x != y);
}

BOOST_AUTO_TEST_CASE(ec_scalar__inequality__zero_positive__false)
{
    const ec_scalar x(0);
    const ec_scalar y(42);
    BOOST_REQUIRE(!(x == y));
    BOOST_REQUIRE((y != x));
}

BOOST_AUTO_TEST_CASE(ec_scalar__inequality__negative_zero__false)
{
    const ec_scalar x(-42);
    const ec_scalar y(0);
    BOOST_REQUIRE(!(x == y));
    BOOST_REQUIRE((y != x));
}

BOOST_AUTO_TEST_CASE(ec_scalar__inequality__int64__false)
{
    const ec_scalar x(42);
    const int64_t y = 24;
    BOOST_REQUIRE_NE(x, y);
    BOOST_REQUIRE_NE(y, x);
}

// negation

BOOST_AUTO_TEST_CASE(ec_scalar__negation__of_42__unchanged_operand)
{
    ec_scalar value(42);
    -value;
    BOOST_REQUIRE(value);
    BOOST_REQUIRE_EQUAL(value, int64_t{ 42 });
}

BOOST_AUTO_TEST_CASE(ec_scalar__negation__of_zero__zero)
{
    ec_scalar value;
    BOOST_REQUIRE_EQUAL(value, int64_t{ 0 });

    const auto negated = -value;
    BOOST_REQUIRE_EQUAL(negated, int64_t{ 0 });
}

BOOST_AUTO_TEST_CASE(ec_scalar__negation__of_positive__negative)
{
    ec_scalar value{ positive1 };
    BOOST_REQUIRE(value);

    const auto negated = -value;
    BOOST_REQUIRE(negated);
    BOOST_REQUIRE_EQUAL(negated, negative1);
}

BOOST_AUTO_TEST_CASE(ec_scalar__negation__of_negative__positive)
{
    ec_scalar value{ negative1 };
    BOOST_REQUIRE(value);

    const auto negated = -value;
    BOOST_REQUIRE(negated);
    BOOST_REQUIRE_EQUAL(negated, positive1);
}

// sum

BOOST_AUTO_TEST_CASE(ec_scalar__sum__cool__expected)
{
    BOOST_REQUIRE_EQUAL((secret1a + secret1b), sum1);
}

BOOST_AUTO_TEST_CASE(ec_scalar__sum__secret1a_secret1b__sum1)
{
    const ec_scalar x{ secret1a };
    const ec_scalar y{ secret1b };
    BOOST_REQUIRE_EQUAL((x + y), sum1);
}

BOOST_AUTO_TEST_CASE(ec_scalar__sum__positive1_positive2__positive3)
{
    const ec_scalar one{ positive1 };
    const ec_scalar two{ positive2 };
    BOOST_REQUIRE_EQUAL((one + two), positive3);
}

BOOST_AUTO_TEST_CASE(ec_scalar__sum__positives__expected)
{
    const ec_scalar x(24);
    const ec_scalar y(positive42);
    BOOST_REQUIRE_EQUAL((x + y), int64_t{ 24 + 42 });
}

// 0 + x : failure
BOOST_AUTO_TEST_CASE(ec_scalar__sum__zero_plus__fail)
{
    const ec_scalar x(0);
    const ec_scalar y(42);
    BOOST_REQUIRE(!(x + y));
}

// x + 0 : success
BOOST_AUTO_TEST_CASE(ec_scalar__sum__plus_zero__expected)
{
    const ec_scalar x(42);
    const ec_scalar y(0);
    BOOST_REQUIRE_EQUAL((x + y), positive42);
}

BOOST_AUTO_TEST_CASE(ec_scalar__sum__one_negative__expected)
{
    const ec_scalar x(-1);
    const ec_scalar y(42);
    BOOST_REQUIRE_EQUAL(((x + y)), int64_t{ -1 + 42 });
}

BOOST_AUTO_TEST_CASE(ec_scalar__sum_assignment__positives__expected)
{
    ec_scalar x(1);
    ec_scalar y(2);
    ec_scalar z(39);
    BOOST_REQUIRE_EQUAL(((x += y)), int64_t{ 1 + 2 });
    BOOST_REQUIRE_EQUAL(((y += z)), int64_t{ 2 + 39 });
    BOOST_REQUIRE_EQUAL(((z += x)), positive42);
}

// difference

BOOST_AUTO_TEST_CASE(ec_scalar__difference__cool__expected)
{
    BOOST_REQUIRE_EQUAL((positive1 - positive2), negative1);
}

BOOST_AUTO_TEST_CASE(ec_scalar__difference__positive1_positive2__negative1)
{
    const ec_scalar one{ positive1 };
    const ec_scalar two{ positive2 };
    BOOST_REQUIRE_EQUAL((one - two), negative1);
}

BOOST_AUTO_TEST_CASE(ec_scalar__difference__same_positives__zero)
{
    const ec_scalar x(42);
    const ec_scalar y(positive42);
    BOOST_REQUIRE(!(x - y));
}

// 0 - x : failure
BOOST_AUTO_TEST_CASE(ec_scalar__difference__from_zero__fail)
{
    const ec_scalar x(0);
    const ec_scalar y(42);
    BOOST_REQUIRE(!(x - y));
}

// x - 0 : success
BOOST_AUTO_TEST_CASE(ec_scalar__difference__zero_from__expected)
{
    const ec_scalar x(42);
    const ec_scalar y(0);
    BOOST_REQUIRE_EQUAL((x - y), positive42);
}

BOOST_AUTO_TEST_CASE(ec_scalar__difference__positive_running__expected)
{
    const ec_scalar x(42);
    const ec_scalar y(+1);
    const ec_scalar z(-1);
    BOOST_REQUIRE_EQUAL((x - y - z), positive42);
}

BOOST_AUTO_TEST_CASE(ec_scalar__difference__negative_running__expected)
{
    const ec_scalar x(-42);
    const ec_scalar y(+1);
    const ec_scalar z(-1);
    BOOST_REQUIRE_EQUAL((x - y - z), int64_t{ -42 + 1 - 1 });
}

BOOST_AUTO_TEST_CASE(ec_scalar__difference_assignment__negative_running__expected)
{
    ec_scalar x(1);
    ec_scalar y(2);
    ec_scalar z(41);
    BOOST_REQUIRE_EQUAL(((x -= y)), int64_t{ 1 - 2 });
    BOOST_REQUIRE_EQUAL(((y -= z)), int64_t{ 2 - 41 });
    BOOST_REQUIRE_EQUAL(((z -= x)), positive42);
}

// product

BOOST_AUTO_TEST_CASE(ec_scalar__product__cool__expected)
{
    BOOST_REQUIRE_EQUAL((secret1a * secret1b), product1);
}

BOOST_AUTO_TEST_CASE(ec_scalar__product__secret1a_secret1b__product1)
{
    const ec_scalar x{ secret1a };
    const ec_scalar y{ secret1b };
    BOOST_REQUIRE_EQUAL((x * y), product1);
}

BOOST_AUTO_TEST_CASE(ec_scalar__product__zero_positive2__zero)
{
    const ec_scalar zero;
    const ec_scalar two{ positive2 };
    BOOST_REQUIRE_EQUAL((zero * two), zero);
}

BOOST_AUTO_TEST_CASE(ec_scalar__product__positive1_positive2__positive2)
{
    const ec_scalar one{ positive1 };
    const ec_scalar two{ positive2 };
    BOOST_REQUIRE_EQUAL((one * two), positive2);
}

BOOST_AUTO_TEST_CASE(ec_scalar__product__positives__expected)
{
    const ec_scalar x(24);
    const ec_scalar y(positive42);
    BOOST_REQUIRE_EQUAL((x * y), int64_t{ 24 * 42 });
}

BOOST_AUTO_TEST_CASE(ec_scalar__product__negative__expected)
{
    const ec_scalar x(-1);
    const ec_scalar y(+1);
    const ec_scalar z(42);
    BOOST_REQUIRE_EQUAL((x * y * z), -ec_scalar(positive42));
}

BOOST_AUTO_TEST_CASE(ec_scalar__product_assignment__positives__expected)
{
    ec_scalar x(1);
    ec_scalar y(2);
    ec_scalar z(21);
    BOOST_REQUIRE_EQUAL(((x *= y)), int64_t{ 1 * 2 });
    BOOST_REQUIRE_EQUAL(((y *= z)), int64_t{ 2 * 21 });
    BOOST_REQUIRE_EQUAL(((z *= x)), positive42);
}

// points

BOOST_AUTO_TEST_CASE(ec_scalar__product__ec_secret_by_generator__expected_point)
{
    const auto product = secret2 * ec_point::generator;
    BOOST_REQUIRE_EQUAL(product.point(), compressed2);
}

BOOST_AUTO_TEST_CASE(ec_scalar__product__zero_by_generator__zero)
{
    const auto product = null_hash * ec_point::generator;
    BOOST_REQUIRE(!product);
}

BOOST_AUTO_TEST_SUITE_END()
