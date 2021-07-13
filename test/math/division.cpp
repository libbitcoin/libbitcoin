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

 // C++14: required for static_cast to be constexpr.
 // These helpers otherwise create comparison of unsigned types warnings.

template <typename Dividend, typename Divisor>
constexpr bool ceilinged_identity(Dividend x, Divisor y)
{
#ifdef BC_CPP_14
    return ceilinged_divide(x, y) * y + ceilinged_modulo(x, y) ==
        static_cast<decltype((x / y) * y + (x / y))>(x);
#else
    return true;
#endif
}

template <typename Dividend, typename Divisor>
constexpr bool floored_identity(Dividend x, Divisor y)
{
#ifdef BC_CPP_14
    return floored_divide(x, y) * y + floored_modulo(x, y) ==
        static_cast<decltype((x / y) * y + (x / y))>(x);
#else
    return true;
#endif
}

template <typename Dividend, typename Divisor>
constexpr bool truncated_identity(Dividend x, Divisor y)
{
#ifdef BC_CPP_14
    return truncated_divide(x, y) * y + truncated_modulo(x, y) ==
        static_cast<decltype((x / y) * y + (x / y))>(x);
#else
    return true;
#endif
}

#define DIVISION_MODULO
#define DIVISION_DIVIDE
#define DIVISION_MATRIX

#ifdef DIVISION_MODULO

// ceilinged_modulo

// Zero is same as c++.
static_assert(0 % 1 == 0, "");
static_assert(ceilinged_modulo(0, 1) == 0, "");
static_assert(ceilinged_identity(0, 1), "");
static_assert(ceilinged_identity(0u, 1), "");
static_assert(ceilinged_identity(0, 1u), "");
static_assert(ceilinged_identity(0u, 1u), "");

// No remainder and positive quotient are same as c++.
static_assert(42 % 2 == 0, "");
static_assert(ceilinged_modulo(42, 2) == 0, "");
static_assert(ceilinged_identity(42, 2), "");
static_assert(ceilinged_identity(42u, 2), "");;
static_assert(ceilinged_identity(42, 2u), "");
static_assert(ceilinged_identity(42u, 2u), "");

// No remainder is same as c++.
static_assert(-42 % 2 == 0, "");
static_assert(ceilinged_modulo(-42, 2) == 0, "");
static_assert(ceilinged_identity(-42, 2), "");
////static_assert(ceilinged_identity(-42, 2u), "");

// No remainder is same as c++.
static_assert(42 % -2 == 0, "");
static_assert(ceilinged_modulo(42, -2) == 0, "");
static_assert(ceilinged_identity(42, -2), "");
////static_assert(ceilinged_identity(42u, -2), "");

// No remainder is same as c++.
static_assert(-42 % -2 == 0, "");
static_assert(ceilinged_modulo(-42, -2) == 0, "");
static_assert(ceilinged_identity(-42, -2), "");

// Positive quotient is floored in c++.
static_assert(42 % 8 == 2, "");
static_assert(ceilinged_modulo(42, 8) == -6, "");
static_assert(ceilinged_identity(42, 8), "");
////static_assert(ceilinged_identity(42u, 8), "");
////static_assert(ceilinged_identity(42, 8u), "");
////static_assert(ceilinged_identity(42u, 8u), "");

// Negative quotient is ceilinged in c++.
static_assert(-42 % 8 == -2, "");
static_assert(ceilinged_modulo(-42, 8) == -2, "");
static_assert(ceilinged_identity(-42, 8), "");
////static_assert(ceilinged_identity(-42, 8u), "");

// Negative quotient is ceilinged in c++.
static_assert(42 % -8 == 2, "");
static_assert(ceilinged_modulo(42, -8) == 2, "");
static_assert(ceilinged_identity(42, -8), "");
////static_assert(ceilinged_identity(42u, -8), "");

// Positive quotient is floored in c++.
static_assert(-42 % -8 == -2, "");
static_assert(ceilinged_modulo(-42, -8) == 6, "");
static_assert(ceilinged_identity(-42, -8), "");

// floored_modulo
// Same as Python (dividend % divisor).

// Zero is same as c++.
static_assert(0 % 1 == 0, "");
// The python result of 0%1.
static_assert(floored_modulo(0, 1) == 0, "");
static_assert(floored_identity(0, 1), "");
static_assert(floored_identity(0u, 1), "");
static_assert(floored_identity(0, 1u), "");
static_assert(floored_identity(0u, 1u), "");

// No remainder and positive quotient are same as c++.
static_assert(42 % 2 == 0, "");
// The python result of 42%2.
static_assert(floored_modulo(42, 2) == 0, "");
static_assert(floored_identity(42, 2), "");
static_assert(floored_identity(42u, 2), "");
static_assert(floored_identity(42, 2u), "");
static_assert(floored_identity(42u, 2u), "");

// No remainder is same as c++.
static_assert(-42 % 2 == 0, "");
// The python result of -42%2.
static_assert(floored_modulo(-42, 2) == 0, "");
static_assert(floored_identity(-42, 2), "");
////static_assert(floored_identity(-42, 2u), "");

// No remainder is same as c++.
static_assert(42 % -2 == 0, "");
// The python result of 42%-2.
static_assert(floored_modulo(42, -2) == 0, "");
static_assert(floored_identity(42, -2), "");
////static_assert(floored_identity(42u, -2), "");

// No remainder and positive quotient are same as c++.
static_assert(-42 % -2 == 0, "");
// The python result of -42%-2.
static_assert(floored_modulo(-42, -2) == 0, "");
static_assert(floored_identity(-42, -2), "");

// Positive quotient is floored in c++.
static_assert(42 % 8 == 2, "");
// The python result of 42%8.
static_assert(floored_modulo(42, 8) == 2, "");
static_assert(floored_identity(42, 8), "");
static_assert(floored_identity(42u, 8), "");
static_assert(floored_identity(42, 8u), "");
static_assert(floored_identity(42u, 8u), "");

// Negative quotient is ceilinged in c++.
static_assert(-42 % 8 == -2, "");
// The python result of -42%8.
static_assert(floored_modulo(-42, 8) == 6, "");
static_assert(floored_identity(-42, 8), "");
////static_assert(floored_identity(-42, 8u), "");

// Negative quotient is ceilinged in c++.
static_assert(42 % -8 == 2, "");
// The python result of 42%-8.
static_assert(floored_modulo(42, -8) == -6, "");
static_assert(floored_identity(42, -8), "");
////static_assert(floored_identity(42u, -8), "");

// Positive quotient is floored in c++.
static_assert(-42 % -8 == -2, "");
// The python result of -42%-8.
static_assert(floored_modulo(-42, -8) == -2, "");
static_assert(floored_identity(-42, -8), "");

// truncated_modulo
// Same as c++ (dividend % divisor).

static_assert(truncated_modulo(0, 1) == 0 % 1, "");
static_assert(truncated_identity(0, 1), "");
static_assert(truncated_identity(0u, 1), "");
static_assert(truncated_identity(0, 1u), "");
static_assert(truncated_identity(0u, 1u), "");

static_assert(truncated_modulo(42, 2) == 42 % 2, "");
static_assert(truncated_identity(42, 2), "");
static_assert(truncated_identity(42u, 2), "");
static_assert(truncated_identity(42, 2u), "");
static_assert(truncated_identity(42u, 2u), "");

static_assert(truncated_modulo(-42, 2) == -42 % 2, "");
static_assert(truncated_identity(-42, 2), "");
////static_assert(truncated_identity(-42, 2u), "");

static_assert(truncated_modulo(42, -2) == 42 % -2, "");
static_assert(truncated_identity(42, -2), "");
////static_assert(truncated_identity(42u, -2), "");

static_assert(truncated_modulo(-42, -2) == -42 % -2, "");
static_assert(truncated_identity(-42, -2), "");

static_assert(truncated_modulo(42, 8) == 42 % 8, "");
static_assert(truncated_identity(42, 8), "");
static_assert(truncated_identity(42u, 8), "");
static_assert(truncated_identity(42, 8u), "");
static_assert(truncated_identity(42u, 8u), "");

static_assert(truncated_modulo(-42, 8) == -42 % 8, "");
static_assert(truncated_identity(-42, 8), "");
////static_assert(truncated_identity(-42, 8u), "");

static_assert(truncated_modulo(42, -8) == 42 % -8, "");
static_assert(truncated_identity(42, -8), "");
static_assert(truncated_identity(42, -8), "");

static_assert(truncated_modulo(-42, -8) == -42 % -8, "");
static_assert(truncated_identity(-42, -8), "");

#endif // DIVISION_MODULO

#ifdef DIVISION_DIVIDE

// ceilinged_divide
// Same as Python -(numerator//-denominator).

// Zero is same as c++.
static_assert(0 / 1 == 0, "");
static_assert(ceilinged_divide(0, 1) == 0, "");
static_assert(ceilinged_identity(0, 1), "");
static_assert(ceilinged_identity(0u, 1), "");
static_assert(ceilinged_identity(0, 1u), "");
static_assert(ceilinged_identity(0u, 1u), "");

// No remainder and negative quotient are same as c++.
static_assert(1 / -1 == -1, "");
static_assert(ceilinged_divide(1, -1) == -1, "");
static_assert(ceilinged_identity(1, -1), "");
////static_assert(ceilinged_identity(1u, -1), "");

// No remainder and negative quotient are same as c++.
static_assert(-1 / 1 == -1 / 1, "");
static_assert(ceilinged_divide(-1, 1) == -1, "");
static_assert(ceilinged_identity(-1, 1), "");
////static_assert(ceilinged_identity(-1, 1u), "");

// No remainder is same as c++.
static_assert(42 / 2 == 21, "");
static_assert(ceilinged_divide(42, 2) == 21, "");
static_assert(ceilinged_identity(42, 2), "");
static_assert(ceilinged_identity(42u, 2), "");
static_assert(ceilinged_identity(42, 2u), "");
static_assert(ceilinged_identity(42u, 2u), "");

// A common round-up hack works for both positives in c++.
static_assert(ceilinged_divide(42, 8) == (42 + (8 - 1)) / 8, "");
// Positive quotient is floored in c++.
static_assert(42 / 8 == 5, "");
// The python result of -(42//-8).
static_assert(ceilinged_divide(42, 8) == 6, "");
static_assert(ceilinged_identity(42, 8), "");
////static_assert(ceilinged_identity(42u, 8), "");
////static_assert(ceilinged_identity(42, 8u), "");
////static_assert(ceilinged_identity(42u, 8u), "");

// A common round-up hack DOES NOT work for negative dividend in c++.
static_assert(ceilinged_divide(-42, 8) != (-42 + (8 - 1)) / 8, "");
// Negative quotient is ceilinged in c++.
static_assert(-42 / 8 == -5, "");
// The python result of -(-42//-8).
static_assert(ceilinged_divide(-42, 8) == -5, "");
static_assert(ceilinged_identity(-42, 8), "");
////static_assert(ceilinged_identity(-42, 8u), "");

// A common round-up hack DOES NOT work for negative divisor in c++.
static_assert(ceilinged_divide(42, -8) != (42 + (-8 - 1)) / -8, "");
// Negative quotient is ceilinged in c++.
static_assert(42 / -8 == -5, "");
// The python result of -(42//8).
static_assert(ceilinged_divide(42, -8) == -5, "");
static_assert(ceilinged_identity(42, -8), "");
////static_assert(ceilinged_identity(42u, -8), "");

// A common round-up hack works for both negatives in c++.
static_assert(ceilinged_divide(-42, -8) == (-42 + (-8 - 1)) / -8, "");
// Positive quotient is floored in c++.
static_assert(-42 / -8 == 5, "");
// The python result of -(-42//8).
static_assert(ceilinged_divide(-42, -8) == 6, "");
static_assert(ceilinged_identity(-42, -8), "");

// floored_divide
// Same as Python (dividend // divisor).

// Zero is same as c++.
static_assert(0 / 1 == 0, "");
static_assert(floored_divide(0, 1) == 0, "");
static_assert(floored_identity(0, 1), "");
static_assert(floored_identity(0u, 1), "");
static_assert(floored_identity(0, 1u), "");
static_assert(floored_identity(0u, 1u), "");

// No remainder is same as c++.
static_assert(1 / -1 == -1, "");
static_assert(floored_divide(1, -1) == -1, "");
static_assert(floored_identity(1, -1), "");
////static_assert(floored_identity(1u, -1), "");

// No remainder is same as c++.
static_assert(-1 / 1 == -1, "");
////static_assert(floored_divide(-1, 1u) == -1, "");
static_assert(floored_identity(-1, 1), "");
////static_assert(floored_identity(-1, 1u), "");

// Positive quotient is floored in c++.
static_assert(42 / 2 == 21, "");
static_assert(floored_divide(42, 2) == 21, "");
static_assert(floored_identity(42, 2), "");
static_assert(floored_identity(42u, 2), "");
static_assert(floored_identity(42, 2u), "");
static_assert(floored_identity(42u, 2u), "");

// Positive quotient is floored in c++.
static_assert(42 / 8 == 5, "");
static_assert(floored_divide(42, 8) == 5, "");
static_assert(floored_identity(42, 8), "");
static_assert(floored_identity(42u, 8), "");
static_assert(floored_identity(42, 8u), "");
static_assert(floored_identity(42u, 8u), "");

// Negative quotient is ceilinged in c++.
static_assert(-42 / 8 == -5, "");
static_assert(floored_divide(-42, 8) == -6, "");
static_assert(floored_identity(-42, 8), "");
////static_assert(floored_identity(-42, 8u), "");

// Negative quotient is ceilinged in c++.
static_assert(42 / -8 == -5, "");
static_assert(floored_divide(42, -8) == -6, "");
static_assert(floored_identity(42, -8), "");
////static_assert(floored_identity(42u, -8), "");

// Positive quotient is floored in c++.
static_assert(-42 / -8 == 5, "");
static_assert(floored_divide(-42, -8) == 5, "");
static_assert(floored_identity(-42, -8), "");

// truncated_divide
// Same as c++ (dividend / divisor).

static_assert(truncated_divide(0, 1) == 0 / 1, "");
static_assert(truncated_identity(0, 1), "");
static_assert(truncated_identity(0u, 1), "");
static_assert(truncated_identity(0, 1u), "");
static_assert(truncated_identity(0u, 1u), "");

static_assert(truncated_divide(1, -1) == 1 / -1, "");
static_assert(truncated_identity(1, -1), "");
////static_assert(truncated_identity(1u, -1), "");

static_assert(truncated_divide(-1, 1) == -1 / 1, "");
static_assert(truncated_identity(-1, 1), "");
////static_assert(truncated_identity(-1, 1u), "");

static_assert(truncated_divide(42, 2) == 42 / 2, "");
static_assert(truncated_identity(42, 2), "");
static_assert(truncated_identity(42u, 2), "");
static_assert(truncated_identity(42, 2u), "");
static_assert(truncated_identity(42u, 2u), "");

static_assert(truncated_divide(42, 8) == 42 / 8, "");
static_assert(truncated_identity(42, 8), "");
static_assert(truncated_identity(42u, 8), "");
static_assert(truncated_identity(42, 8u), "");
static_assert(truncated_identity(42u, 8u), "");

static_assert(truncated_divide(-42, 8) == -42 / 8, "");
static_assert(truncated_identity(-42, 8), "");
////static_assert(truncated_identity(-42, 8u), "");

static_assert(truncated_divide(42, -8) == 42 / -8, "");
static_assert(truncated_identity(42, 8), "");
static_assert(truncated_identity(42, 8u), "");

static_assert(truncated_divide(-42, -8) == -42 / -8, "");
static_assert(truncated_identity(-42, -8), "");

#endif // DIVISION_DIVIDE

#ifdef DIVISION_MATRIX

// Full matrix of conversions/signs (unsigned types avoided due to warnings).
// ----------------------------------------------------------------------------

// C++11: if the quotient x/y is representable in the type of the result:
// Identity: (x / y) * y + (x % y) = x

// Divide increment magnitude.
constexpr auto i = 1;

// Operand magnitudes.
constexpr auto x = 4;
constexpr auto y = 3;

// Zero to zero.
static_assert(0 / -y == 0, "0");
static_assert(0 / +y == 0, "0");
static_assert(0 % -y == 0, "0");
static_assert(0 % +y == 0, "0");

// ----------------------------------------------------------------------------

// Truncated divide:
static_assert(+x / +y == +1, "+");
static_assert(-x / -y == +1, "+");
static_assert(-x / +y == -1, "-");
static_assert(+x / -y == -1, "-");

// Truncated modulo:
static_assert(+x % +y == +1, "+");
static_assert(-x % -y == -1, "+");
static_assert(-x % +y == -1, "-");
static_assert(+x % -y == +1, "-");

// Truncated identity:
static_assert((+x / +y) * (+y) + (+x % +y) == +x, "+");
static_assert((-x / -y) * (-y) + (-x % -y) == -x, "+");
static_assert((-x / +y) * (+y) + (-x % +y) == -x, "-");
static_assert((+x / -y) * (-y) + (+x % -y) == +x, "-");

// ----------------------------------------------------------------------------

// Ceilinged divide: (increment truncated quotient if positive and remainder)
static_assert((+x / +y) + (((+x < 0) == (+y < 0)) ? ((+x % +y) != 0 ? +i : 0) : 0) == +1 + i, "+");
static_assert((-x / -y) + (((-x < 0) == (-y < 0)) ? ((-x % -y) != 0 ? +i : 0) : 0) == +1 + i, "+");
static_assert((-x / +y) + (((-x < 0) == (+y < 0)) ? ((-x % +y) != 0 ? +i : 0) : 0) == -1 + 0, "-");
static_assert((+x / -y) + (((+x < 0) == (-y < 0)) ? ((+x % -y) != 0 ? +i : 0) : 0) == -1 + 0, "-");

// Ceilinged modulo: (decrease truncated modulo by divisor if positive and remainder)
static_assert((+x % +y) - (((+x < 0) == (+y < 0)) ? +y : 0) == +1 - +y, "+");
static_assert((-x % -y) - (((-x < 0) == (-y < 0)) ? -y : 0) == -1 - -y, "+");
static_assert((-x % +y) - (((-x < 0) == (+y < 0)) ? +y : 0) == -1 - +0, "-");
static_assert((+x % -y) - (((+x < 0) == (-y < 0)) ? -y : 0) == +1 - -0, "-");

// Ceilinged identity
static_assert((+1 + i) * (+y) + (+1 - +y) == +x, "+");
static_assert((+1 + i) * (-y) + (-1 - -y) == -x, "+");
static_assert((-1 + 0) * (+y) + (-1 - +0) == -x, "-");
static_assert((-1 + 0) * (-y) + (+1 - -0) == +x, "-");

// ----------------------------------------------------------------------------

// Floored divide: (decrement truncated quotient if negative and remainder)
static_assert((+x / +y) - (((+x < 0) != (+y < 0)) ? ((+x % +y) != 0 ? +i : 0) : 0) == +1 - 0, "+");
static_assert((-x / -y) - (((-x < 0) != (-y < 0)) ? ((-x % -y) != 0 ? +i : 0) : 0) == +1 - 0, "+");
static_assert((-x / +y) - (((-x < 0) != (+y < 0)) ? ((-x % +y) != 0 ? +i : 0) : 0) == -1 - i, "-");
static_assert((+x / -y) - (((+x < 0) != (-y < 0)) ? ((+x % -y) != 0 ? +i : 0) : 0) == -1 - i, "-");

// Floored modulo: (increase truncated modulo by divisor if negative and remainder)
static_assert((+x % +y) + (((+x < 0) != (+y < 0)) ? +y : 0) == +1 + +0, "+");
static_assert((-x % -y) + (((-x < 0) != (-y < 0)) ? -y : 0) == -1 + +0, "+");
static_assert((-x % +y) + (((-x < 0) != (+y < 0)) ? +y : 0) == -1 + +y, "-");
static_assert((+x % -y) + (((+x < 0) != (-y < 0)) ? -y : 0) == +1 + -y, "-");

// Floored identity:
static_assert((+1 - 0) * (+y) + (+1 + +0) == +x, "+");
static_assert((+1 - 0) * (-y) + (-1 + +0) == -x, "+");
static_assert((-1 - i) * (+y) + (-1 + +y) == -x, "-");
static_assert((-1 - i) * (-y) + (+1 + -y) == +x, "-");

// ----------------------------------------------------------------------------

// Adjustments (+/-1 always corresponds to -/+divisor).
static_assert(((+x / +y) + 1) * +y + ((+x % +y) - +y) == +x, "+1++");
static_assert(((+x / +y) - 1) * +y + ((+x % +y) + +y) == +x, "-1++");
static_assert(((-x / -y) + 1) * -y + ((-x % -y) - -y) == -x, "+1--");
static_assert(((-x / -y) - 1) * -y + ((-x % -y) + -y) == -x, "-1--");
static_assert(((-x / +y) + 1) * +y + ((-x % +y) - +y) == -x, "+1-+");
static_assert(((-x / +y) - 1) * +y + ((-x % +y) + +y) == -x, "-1-+");
static_assert(((+x / -y) + 1) * -y + ((+x % -y) - -y) == +x, "+1+-");
static_assert(((+x / -y) - 1) * -y + ((+x % -y) + -y) == +x, "-1+-");

#endif // DIVISION_MATRIX

