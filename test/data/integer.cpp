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
#include <sstream>
#include <string>

BOOST_AUTO_TEST_SUITE(integer_tests)

#define MAX_HASH \
"0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
static const auto max_hash = uint256_t(MAX_HASH);

#define NEGATIVE1_HASH \
"0x8000000000000000000000000000000000000000000000000000000000000000"
static const auto negative_zero_hash = uint256_t(NEGATIVE1_HASH);

#define MOST_HASH \
"0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
static const auto most_hash = uint256_t(MOST_HASH);

#define ODD_HASH \
"0x8437390223499ab234bf128e8cd092343485898923aaaaabbcbcc4874353fff4"
static const auto odd_hash = uint256_t(ODD_HASH);

#define HALF_HASH \
"0x00000000000000000000000000000000ffffffffffffffffffffffffffffffff"
static const auto half_hash = uint256_t(HALF_HASH);

#define QUARTER_HASH \
"0x000000000000000000000000000000000000000000000000ffffffffffffffff"
static const auto quarter_hash = uint256_t(QUARTER_HASH);

#define UNIT_HASH \
"0x0000000000000000000000000000000000000000000000000000000000000001"
static const auto unit_hash = uint256_t(UNIT_HASH);

#define ONES_HASH \
"0x0000000100000001000000010000000100000001000000010000000100000001"
static const auto ones_hash = uint256_t(ONES_HASH);

#define FIVES_HASH \
"0x5555555555555555555555555555555555555555555555555555555555555555"
static const auto fives_hash = uint256_t(FIVES_HASH);

// constructors
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(integer__uint256_constructor_default__always__equates_to_0)
{
    uint256_t minimum;
    BOOST_REQUIRE_EQUAL(minimum > 0, false);
    BOOST_REQUIRE_EQUAL(minimum < 0, false);
    BOOST_REQUIRE_EQUAL(minimum >= 0, true);
    BOOST_REQUIRE_EQUAL(minimum <= 0, true);
    BOOST_REQUIRE_EQUAL(minimum == 0, true);
    BOOST_REQUIRE_EQUAL(minimum != 0, false);
}


BOOST_AUTO_TEST_CASE(integer__uint256_constructor_move__42__equals_42)
{
    static const auto expected = 42u;
    static const uint256_t value(expected);
    BOOST_REQUIRE_EQUAL(value, expected);
}

BOOST_AUTO_TEST_CASE(integer__uint256_constructor_copy__odd_hash__equals_odd_hash)
{
    static const auto expected = uint256_t(odd_hash);
    static const uint256_t value(expected);
    BOOST_REQUIRE_EQUAL(value, expected);
}

BOOST_AUTO_TEST_CASE(integer__uint256_constructor_uint32__minimum__equals_0)
{
    static const auto expected = 0u;
    static const uint256_t value(expected);
    BOOST_REQUIRE_EQUAL(value, expected);
}

BOOST_AUTO_TEST_CASE(integer__uint256_constructor_uint32__42__equals_42)
{
    static const auto expected = 42u;
    static const uint256_t value(expected);
    BOOST_REQUIRE_EQUAL(value, expected);
}

BOOST_AUTO_TEST_CASE(integer__uint256_constructor_uint32__maximum__equals_maximum)
{
    static const auto expected = bc::max_uint32;
    static const uint256_t value(expected);
    BOOST_REQUIRE_EQUAL(value, expected);
}

// hash
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(integer__uint256_hash__default__returns_null_hash)
{
    static const uint256_t value;
    BOOST_REQUIRE_EQUAL(value, 0);
}

BOOST_AUTO_TEST_CASE(integer__uint256_hash__1__returns_unit_hash)
{
    static const uint256_t value("0x0000000000000000000000000000000000000000000000000000000000000001");
    BOOST_REQUIRE_EQUAL(value, 1);
}

BOOST_AUTO_TEST_CASE(integer__uint256_hash__negative_1__returns_negative_zero_hash)
{
    BOOST_REQUIRE_EQUAL(negative_zero_hash, uint256_t("0x8000000000000000000000000000000000000000000000000000000000000000"));
}

// array operator
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(integer__uint256_array__default__expected)
{
    static const uint256_t value;
    BOOST_REQUIRE_EQUAL(value, uint256_t("0x0000000000000000000000000000000000000000000000000000000000000000"));
}

BOOST_AUTO_TEST_CASE(integer__uint256_array__42__expected)
{
    static const uint256_t value(42);
    BOOST_REQUIRE_EQUAL(value, uint256_t("0x000000000000000000000000000000000000000000000000000000000000002a"));
}

BOOST_AUTO_TEST_CASE(integer__uint256_array__0x87654321__expected)
{
    static const uint256_t value(0x87654321);
    BOOST_REQUIRE_EQUAL(value, uint256_t("0x0000000000000000000000000000000000000000000000000000000087654321"));
}

BOOST_AUTO_TEST_CASE(integer__uint256_array__negative_1__expected)
{
    static const uint256_t value(negative_zero_hash);
    BOOST_REQUIRE_EQUAL(value, uint256_t("0x8000000000000000000000000000000000000000000000000000000000000000"));
}

BOOST_AUTO_TEST_CASE(integer__uint256_array__odd_hash__expected)
{
    static const uint256_t value(odd_hash);
    BOOST_REQUIRE_EQUAL(value, uint256_t("0x8437390223499ab234bf128e8cd092343485898923aaaaabbcbcc4874353fff4"));
}

// comparison operators
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(integer__uint256_comparison_operators__null_hash__expected)
{
    static const uint256_t value(0);

    BOOST_REQUIRE_EQUAL(value > 0, false);
    BOOST_REQUIRE_EQUAL(value < 0, false);
    BOOST_REQUIRE_EQUAL(value >= 0, true);
    BOOST_REQUIRE_EQUAL(value <= 0, true);
    BOOST_REQUIRE_EQUAL(value == 0, true);
    BOOST_REQUIRE_EQUAL(value != 0, false);

    BOOST_REQUIRE_EQUAL(value > 1, false);
    BOOST_REQUIRE_EQUAL(value < 1, true);
    BOOST_REQUIRE_EQUAL(value >= 1, false);
    BOOST_REQUIRE_EQUAL(value <= 1, true);
    BOOST_REQUIRE_EQUAL(value == 1, false);
    BOOST_REQUIRE_EQUAL(value != 1, true);
}

BOOST_AUTO_TEST_CASE(integer__uint256_comparison_operators__unit_hash__expected)
{
    static const uint256_t value(1);

    BOOST_REQUIRE_EQUAL(value > 1, false);
    BOOST_REQUIRE_EQUAL(value < 1, false);
    BOOST_REQUIRE_EQUAL(value >= 1, true);
    BOOST_REQUIRE_EQUAL(value <= 1, true);
    BOOST_REQUIRE_EQUAL(value == 1, true);
    BOOST_REQUIRE_EQUAL(value != 1, false);

    BOOST_REQUIRE_EQUAL(value > 0, true);
    BOOST_REQUIRE_EQUAL(value < 0, false);
    BOOST_REQUIRE_EQUAL(value >= 0, true);
    BOOST_REQUIRE_EQUAL(value <= 0, false);
    BOOST_REQUIRE_EQUAL(value == 0, false);
    BOOST_REQUIRE_EQUAL(value != 0, true);
}

BOOST_AUTO_TEST_CASE(integer__uint256_comparison_operators__negative_zero_hash__expected)
{
    static const uint256_t value(negative_zero_hash);
    static const uint256_t most(most_hash);
    static const uint256_t maximum(max_hash);

    BOOST_REQUIRE_EQUAL(value > 1, true);
    BOOST_REQUIRE_EQUAL(value < 1, false);
    BOOST_REQUIRE_EQUAL(value >= 1, true);
    BOOST_REQUIRE_EQUAL(value <= 1, false);
    BOOST_REQUIRE_EQUAL(value == 1, false);
    BOOST_REQUIRE_EQUAL(value != 1, true);

    BOOST_REQUIRE_GT(value, most);
    BOOST_REQUIRE_LT(value, maximum);

    BOOST_REQUIRE_GE(value, most);
    BOOST_REQUIRE_LE(value, maximum);

    BOOST_REQUIRE_EQUAL(value, value);
    BOOST_REQUIRE_NE(value, most);
    BOOST_REQUIRE_NE(value, maximum);
}

// not
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(integer__uint256_not__minimum__maximum)
{
    BOOST_REQUIRE_EQUAL(~uint256_t(), uint256_t(max_hash));
}

BOOST_AUTO_TEST_CASE(integer__uint256_not__maximum__minimum)
{
    BOOST_REQUIRE_EQUAL(~uint256_t(max_hash), uint256_t());
}

BOOST_AUTO_TEST_CASE(integer__uint256_not__most_hash__negative_zero_hash)
{
    BOOST_REQUIRE_EQUAL(~uint256_t(most_hash), uint256_t(negative_zero_hash));
}

BOOST_AUTO_TEST_CASE(integer__uint256_not__not_odd_hash__odd_hash)
{
    BOOST_REQUIRE_EQUAL(~~uint256_t(odd_hash), uint256_t(odd_hash));
}

BOOST_AUTO_TEST_CASE(integer__uint256_not__odd_hash__expected)
{
    static const uint256_t value(odd_hash);
    static const auto not_value = ~value;
    BOOST_REQUIRE_EQUAL(not_value, uint256_t("-0x8437390223499ab234bf128e8cd092343485898923aaaaabbcbcc4874353fff5"));
}

// two's compliment (negate)
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(integer__uint256_twos_compliment__null_hash__null_hash)
{
    // Note that we use 0 - VALUE to negate, as there is a static
    // assert that fails when you negate a uint256_t alone.
    BOOST_REQUIRE_EQUAL(0 - uint256_t(), uint256_t());
}

BOOST_AUTO_TEST_CASE(integer__uint256_twos_compliment__unit_hash__max_hash)
{
    // Note that we use 0 - VALUE to negate, as there is a static
    // assert that fails when you negate a uint256_t alone.
    BOOST_REQUIRE_EQUAL(0 - uint256_t(unit_hash), uint256_t(max_hash));
}

BOOST_AUTO_TEST_CASE(integer__uint256_twos_compliment__odd_hash__expected)
{
    // Note that we use 0 - VALUE to negate, as there is a static
    // assert that fails when you negate a uint256_t alone.
    static const auto compliment = 0 - uint256_t(odd_hash);
    BOOST_REQUIRE_EQUAL(compliment, uint256_t("-0x8437390223499ab234bf128e8cd092343485898923aaaaabbcbcc4874353fff5") + 1);
}

// shift right
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(integer__uint256_shift_right__null_hash__null_hash)
{
    BOOST_REQUIRE_EQUAL(uint256_t() >> 0, uint256_t());
    BOOST_REQUIRE_EQUAL(uint256_t() >> 1, uint256_t());
    BOOST_REQUIRE_EQUAL(uint256_t() >> bc::max_uint32, uint256_t());
}

BOOST_AUTO_TEST_CASE(integer__uint256_shift_right__unit_hash_0__unit_hash)
{
    static const uint256_t value(unit_hash);
    BOOST_REQUIRE_EQUAL(value >> 0, value);
}

BOOST_AUTO_TEST_CASE(integer__uint256_shift_right__unit_hash_positive__null_hash)
{
    static const uint256_t value(unit_hash);
    BOOST_REQUIRE_EQUAL(value >> 1, uint256_t());
    BOOST_REQUIRE_EQUAL(value >> bc::max_uint32, uint256_t());
}

BOOST_AUTO_TEST_CASE(integer__uint256_shift_right__max_hash_1__most_hash)
{
    static const uint256_t value(max_hash);
    BOOST_REQUIRE_EQUAL(value >> 1, uint256_t(most_hash));
}

BOOST_AUTO_TEST_CASE(integer__uint256_shift_right__odd_hash_32__expected)
{
    static const uint256_t value(odd_hash);
    static const auto shifted = value >> 32;
    BOOST_REQUIRE_EQUAL(shifted, uint256_t("0x000000008437390223499ab234bf128e8cd092343485898923aaaaabbcbcc487"));
}

// add256
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(integer__uint256_add256__0_to_null_hash__null_hash)
{
    BOOST_REQUIRE_EQUAL(uint256_t() + 0, uint256_t());
}

BOOST_AUTO_TEST_CASE(integer__uint256_add256__null_hash_to_null_hash__null_hash)
{
    BOOST_REQUIRE_EQUAL(uint256_t() + uint256_t(), uint256_t());
}

BOOST_AUTO_TEST_CASE(integer__uint256_add256__1_to_max_hash__null_hash)
{
    static const uint256_t value(max_hash);
    static const auto sum = value + 1;
    BOOST_REQUIRE_EQUAL(sum, uint256_t());
}

BOOST_AUTO_TEST_CASE(integer__uint256_add256__ones_hash_to_odd_hash__expected)
{
    static const uint256_t value(odd_hash);
    static const auto sum = value + uint256_t(ones_hash);
    BOOST_REQUIRE_EQUAL(sum, uint256_t("0x8437390323499ab334bf128f8cd092353485898a23aaaaacbcbcc4884353fff5"));
}

BOOST_AUTO_TEST_CASE(integer__uint256_add256__1_to_0xffffffff__0x0100000000)
{
    static const uint256_t value("0xffffffff");
    static const auto sum = value + 1;
    BOOST_REQUIRE_EQUAL(sum, uint256_t("0x0000000000000000000000000000000000000000000000000000000100000000"));
}

BOOST_AUTO_TEST_CASE(integer__uint256_add256__1_to_negative_zero_hash__expected)
{
    static const uint256_t value(negative_zero_hash);
    static const auto sum = value + 1;
    BOOST_REQUIRE_EQUAL(sum, uint256_t("0x8000000000000000000000000000000000000000000000000000000000000001"));
}

// divide256
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(integer__uint256_divide256__unit_hash_by_null_hash__throws_overflow_error)
{
    BOOST_REQUIRE_THROW(uint256_t(unit_hash) / uint256_t(0), overflow_exception);
}

BOOST_AUTO_TEST_CASE(integer__uint256_divide256__null_hash_by_unit_hash__null_hash)
{
    BOOST_REQUIRE_EQUAL(uint256_t(0) / uint256_t(1), uint256_t());
}

BOOST_AUTO_TEST_CASE(integer__uint256_divide256__max_hash_by_3__fives_hash)
{
    BOOST_REQUIRE_EQUAL(uint256_t(max_hash) / uint256_t(3), uint256_t(fives_hash));
}

BOOST_AUTO_TEST_CASE(integer__uint256_divide256__max_hash_by_max_hash__1)
{
    BOOST_REQUIRE_EQUAL(uint256_t(max_hash) / uint256_t(max_hash), uint256_t(1));
}

BOOST_AUTO_TEST_CASE(integer__uint256_divide256__max_hash_by_256__shifts_right_8_bits)
{
    static const uint256_t value(max_hash);
    static const auto quotient = value / uint256_t(256);
    BOOST_REQUIRE_EQUAL(quotient, uint256_t("0x00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));
}

// increment
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(integer__uint256_increment__0__1)
{
    BOOST_REQUIRE_EQUAL(++uint256_t(0), uint256_t(1));
}

BOOST_AUTO_TEST_CASE(integer__uint256_increment__1__2)
{
    BOOST_REQUIRE_EQUAL(++uint256_t(1), uint256_t(2));
}

BOOST_AUTO_TEST_CASE(integer__uint256_increment__max_hash__null_hash)
{
    BOOST_REQUIRE_EQUAL(++uint256_t(max_hash), uint256_t());
}

BOOST_AUTO_TEST_CASE(integer__uint256_increment__0xffffffff__0x0100000000)
{
    static const auto increment = ++uint256_t(0xffffffff);
    BOOST_REQUIRE_EQUAL(increment, uint256_t("0x0000000000000000000000000000000000000000000000000000000100000000"));
}

BOOST_AUTO_TEST_CASE(integer__uint256_increment__negative_zero_hash__expected)
{
    static const auto increment = ++uint256_t(negative_zero_hash);
    BOOST_REQUIRE_EQUAL(increment, uint256_t("0x8000000000000000000000000000000000000000000000000000000000000001"));
}

// assign32
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(integer__uint256_assign__null_hash_0__null_hash)
{
    uint256_t value(0);
    value = 0;
    BOOST_REQUIRE_EQUAL(value, uint256_t());
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign__max_hash_0__null_hash)
{
    uint256_t value(max_hash);
    value = 0;
    BOOST_REQUIRE_EQUAL(value, uint256_t());
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign__odd_hash_to_42__42)
{
    uint256_t value(odd_hash);
    value = 42;
    BOOST_REQUIRE_EQUAL(value, uint256_t("0x000000000000000000000000000000000000000000000000000000000000002a"));
}

// assign shift right
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(integer__uint256_assign_shift_right__null_hash__null_hash)
{
    uint256_t value1;
    uint256_t value2;
    uint256_t value3;
    value1 >>= 0;
    value2 >>= 1;
    value3 >>= bc::max_uint32;
    BOOST_REQUIRE_EQUAL(value1, uint256_t());
    BOOST_REQUIRE_EQUAL(value2, uint256_t());
    BOOST_REQUIRE_EQUAL(value3, uint256_t());
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_shift_right__unit_hash_0__unit_hash)
{
    uint256_t value(unit_hash);
    value >>= 0;
    BOOST_REQUIRE_EQUAL(value, uint256_t(unit_hash));
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_shift_right__unit_hash_positive__null_hash)
{
    uint256_t value1(unit_hash);
    uint256_t value2(unit_hash);
    value1 >>= 1;
    value2 >>= bc::max_uint32;
    BOOST_REQUIRE_EQUAL(value1, uint256_t());
    BOOST_REQUIRE_EQUAL(value2, uint256_t());
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_shift_right__max_hash_1__most_hash)
{
    uint256_t value(max_hash);
    value >>= 1;
    BOOST_REQUIRE_EQUAL(value, uint256_t(most_hash));
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_shift_right__odd_hash_32__expected)
{
    uint256_t value(odd_hash);
    value >>= 32;
    BOOST_REQUIRE_EQUAL(value, uint256_t("0x000000008437390223499ab234bf128e8cd092343485898923aaaaabbcbcc487"));
}

// assign shift left
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(integer__uint256_assign_shift_left__null_hash__null_hash)
{
    uint256_t value1;
    uint256_t value2;
    uint256_t value3;
    value1 <<= 0;
    value2 <<= 1;
    value3 <<= bc::max_uint32;
    BOOST_REQUIRE_EQUAL(value1, uint256_t());
    BOOST_REQUIRE_EQUAL(value2, uint256_t());
    BOOST_REQUIRE_EQUAL(value3, uint256_t());
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_shift_left__unit_hash_0__1)
{
    uint256_t value(unit_hash);
    value <<= 0;
    BOOST_REQUIRE_EQUAL(value, uint256_t(1));
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_shift_left__unit_hash_1__2)
{
    uint256_t value(unit_hash);
    value <<= 1;
    BOOST_REQUIRE_EQUAL(value, uint256_t(2));
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_shift_left__unit_hash_31__0x80000000)
{
    uint256_t value(unit_hash);
    value <<= 31;
    BOOST_REQUIRE_EQUAL(value, uint256_t(0x80000000));
}

/* BOOST_AUTO_TEST_CASE(integer__uint256_assign_shift_left__max_hash_1__expected) */
/* { */
/*     uint256_t value(max_hash); */
/*     value <<= 1; */
/*     BOOST_REQUIRE_EQUAL(value, uint256_t("0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe")); */
/* } */

BOOST_AUTO_TEST_CASE(integer__uint256_assign_shift_left__odd_hash_32__expected)
{
    uint256_t value(odd_hash);
    value <<= 32;
    BOOST_REQUIRE_EQUAL(value, uint256_t("0x23499ab234bf128e8cd092343485898923aaaaabbcbcc4874353fff400000000"));
}

// assign multiply32
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(integer__uint256_assign_multiply32__0_by_0__0)
{
    uint256_t value;
    value *= 0;
    BOOST_REQUIRE_EQUAL(value, uint256_t(0));
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_multiply32__0_by_1__0)
{
    uint256_t value;
    value *= 1;
    BOOST_REQUIRE_EQUAL(value, uint256_t(0));
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_multiply32__1_by_1__1)
{
    uint256_t value(1);
    value *= 1;
    BOOST_REQUIRE_EQUAL(value, uint256_t(1));
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_multiply32__42_by_1__42)
{
    uint256_t value(42);
    value *= 1;
    BOOST_REQUIRE_EQUAL(value, uint256_t(42));
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_multiply32__1_by_42__42)
{
    uint256_t value(1);
    value *= 42;
    BOOST_REQUIRE_EQUAL(value, uint256_t(42));
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_multiply32__fives_hash_by_3__max_hash)
{
    uint256_t value(fives_hash);
    value *= 3;
    BOOST_REQUIRE_EQUAL(value, uint256_t(max_hash));
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_multiply32__ones_hash_by_max_uint32__max_hash)
{
    uint256_t value(ones_hash);
    value *= bc::max_uint32;
    BOOST_REQUIRE_EQUAL(value, uint256_t(max_hash));
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_multiply32__max_hash_by_256__shifts_left_8_bits)
{
    uint256_t value(max_hash);
    value *= 256;
    BOOST_REQUIRE_EQUAL(value, uint256_t("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff00"));
}

// assign divide32
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(integer__uint256_assign_divide32__unit_hash_by_null_hash__throws_overflow_error)
{
    uint256_t value(unit_hash);
    BOOST_REQUIRE_THROW(value /= 0, overflow_exception);
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_divide32__null_hash_by_unit_hash__null_hash)
{
    uint256_t value;
    value /= 1;
    BOOST_REQUIRE_EQUAL(value, uint256_t(0));
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_divide32__max_hash_by_3__fives_hash)
{
    uint256_t value(max_hash);
    value /= 3;
    BOOST_REQUIRE_EQUAL(value, uint256_t(fives_hash));
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_divide32__max_hash_by_max_uint32__ones_hash)
{
    uint256_t value(max_hash);
    value /= bc::max_uint32;
    BOOST_REQUIRE_EQUAL(value, uint256_t(ones_hash));
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_divide32__max_hash_by_256__shifts_right_8_bits)
{
    uint256_t value(max_hash);
    value /= 256;
    BOOST_REQUIRE_EQUAL(value, uint256_t("0x00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));
}

// assign add256
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(integer__uint256_assign_add256__0_to_null_hash__null_hash)
{
    uint256_t value;
    value += 0;
    BOOST_REQUIRE_EQUAL(value, uint256_t(0));
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_add256__null_hash_to_null_hash__null_hash)
{
    uint256_t value;
    value += uint256_t();
    BOOST_REQUIRE_EQUAL(uint256_t() + uint256_t(), uint256_t());
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_add256__1_to_max_hash__null_hash)
{
    uint256_t value(max_hash);
    value += 1;
    BOOST_REQUIRE_EQUAL(value, uint256_t());
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_add256__ones_hash_to_odd_hash__expected)
{
    uint256_t value(odd_hash);
    value += uint256_t(ones_hash);
    BOOST_REQUIRE_EQUAL(value, uint256_t("0x8437390323499ab334bf128f8cd092353485898a23aaaaacbcbcc4884353fff5"));
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_add256__1_to_0xffffffff__0x0100000000)
{
    uint256_t value(0xffffffff);
    value += 1;
    BOOST_REQUIRE_EQUAL(value, uint256_t("0x100000000"));
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_add256__1_to_negative_zero_hash__expected)
{
    uint256_t value(negative_zero_hash);
    value += 1;
    BOOST_REQUIRE_EQUAL(value, uint256_t("0x8000000000000000000000000000000000000000000000000000000000000001"));
}

// assign subtract256
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(integer__uint256_assign_subtract256__0_from_null_hash__null_hash)
{
    uint256_t value;
    value -= 0;
    BOOST_REQUIRE_EQUAL(value, uint256_t(0));
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_subtract256__null_hash_from_null_hash__null_hash)
{
    uint256_t value;
    value -= uint256_t();
    BOOST_REQUIRE_EQUAL(uint256_t() + uint256_t(), uint256_t());
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_subtract256__1_from_null_hash__max_hash)
{
    uint256_t value;
    value -= 1;
    BOOST_REQUIRE_EQUAL(value, uint256_t(max_hash));
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_subtract256__1_from_max_hash__expected)
{
    uint256_t value(max_hash);
    value -= 1;
    BOOST_REQUIRE_EQUAL(value, uint256_t("0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe"));
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_subtract256__ones_hash_from_odd_hash__expected)
{
    uint256_t value(odd_hash);
    value -= uint256_t(ones_hash);
    BOOST_REQUIRE_EQUAL(value, uint256_t("0x8437390123499ab134bf128d8cd092333485898823aaaaaabcbcc4864353fff3"));
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_subtract256__1_from_0xffffffff__0x0100000000)
{
    uint256_t value(0xffffffff);
    value -= 1;
    BOOST_REQUIRE_EQUAL(value, uint256_t(0xfffffffe));
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_subtract256__1_from_negative_zero_hash__most_hash)
{
    uint256_t value(negative_zero_hash);
    value -= 1;
    BOOST_REQUIRE_EQUAL(value, uint256_t(most_hash));
}

// assign divide256
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(integer__uint256_assign_divide__unit_hash_by_null_hash__throws_overflow_error)
{
    uint256_t value(unit_hash);
    BOOST_REQUIRE_THROW(value /= uint256_t(0), overflow_exception);
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_divide__null_hash_by_unit_hash__null_hash)
{
    uint256_t value;
    value /= uint256_t(unit_hash);
    BOOST_REQUIRE_EQUAL(value, uint256_t(0));
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_divide__max_hash_by_3__fives_hash)
{
    uint256_t value(max_hash);
    value /= 3;
    BOOST_REQUIRE_EQUAL(value, uint256_t(fives_hash));
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_divide__max_hash_by_max_hash__1)
{
    uint256_t value(max_hash);
    value /= uint256_t(max_hash);
    BOOST_REQUIRE_EQUAL(value, uint256_t(1));
}

BOOST_AUTO_TEST_CASE(integer__uint256_assign_divide__max_hash_by_256__shifts_right_8_bits)
{
    static const uint256_t value(max_hash);
    static const auto quotient = value / uint256_t(256);
    BOOST_REQUIRE_EQUAL(quotient, uint256_t("0x00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));
}

BOOST_AUTO_TEST_SUITE_END()
