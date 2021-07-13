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
#include <cstddef>

constexpr uint32_t min_uint32 = 0;
constexpr uint32_t pos_uint32 = 42;
constexpr  int32_t pos_int32 = 42;
constexpr  int32_t zer_int32 = 0;
constexpr  int32_t neg_int32 = -42;

// add
static_assert(add<int64_t>(min_uint32, min_uint32) == (int64_t)min_uint32 + (int64_t)min_uint32, "");
static_assert(add<int32_t>(min_uint32, max_uint32) == (int32_t)min_uint32 + (int32_t)max_uint32, "");
static_assert(add<int16_t>(max_uint32, min_uint32) == (int16_t)max_uint32 + (int16_t)min_uint32, "");
static_assert(add< int8_t>(max_uint32, max_uint32) == ( int8_t)max_uint32 + (int8_t)max_uint32, "");
static_assert(add<int64_t>(pos_uint32, pos_uint32) == (int64_t)pos_uint32 + (int64_t)pos_uint32, "");
static_assert(add<int32_t>(pos_uint32, max_uint32) == (int32_t)pos_uint32 + (int32_t)max_uint32, "");
static_assert(add<int16_t>(max_uint32, pos_uint32) == (int16_t)max_uint32 + (int16_t)pos_uint32, "");
static_assert(add<int64_t>(pos_uint32, min_uint32) == (int64_t)pos_uint32 + (int64_t)min_uint32, "");
static_assert(add<int32_t>(min_uint32, pos_uint32) == (int32_t)min_uint32 + (int32_t)pos_uint32, "");
static_assert(add<int64_t>(zer_int32, zer_int32) == (int64_t)zer_int32 + (int64_t)zer_int32, "");
static_assert(add<int32_t>(zer_int32, max_int32) == (int32_t)zer_int32 + (int32_t)max_int32, "");
static_assert(add<int16_t>(max_int32, zer_int32) == (int16_t)max_int32 + (int16_t)zer_int32, "");
static_assert(add< int8_t>(max_int32, max_int32) == ( int8_t)max_int32 + (int8_t)max_int32, "");
static_assert(add<int64_t>(neg_int32, neg_int32) == (int64_t)neg_int32 + (int64_t)neg_int32, "");
static_assert(add<int32_t>(neg_int32, max_int32) == (int32_t)neg_int32 + (int32_t)max_int32, "");
static_assert(add<int16_t>(max_int32, neg_int32) == (int16_t)max_int32 + (int16_t)neg_int32, "");
static_assert(add<int64_t>(neg_int32, min_int32) == (int64_t)neg_int32 + (int64_t)min_int32, "");
////static_assert(add< int32_t>(min_int32, neg_int32) == (int32_t)min_int32 + (int32_t)neg_int32, "");
static_assert(add<int64_t>(pos_int32, neg_int32) == (int64_t)pos_int32 + (int64_t)neg_int32, "");
static_assert(add<int32_t>(neg_int32, pos_int32) == (int32_t)neg_int32 + (int32_t)pos_int32, "");
static_assert(add<int16_t>(pos_int32, pos_int32) == (int16_t)pos_int32 + (int16_t)pos_int32, "");
static_assert(add<int64_t>(pos_int32, min_int32) == (int64_t)pos_int32 + (int64_t)min_int32, "");
static_assert(add<int32_t>(min_int32, pos_int32) == (int32_t)min_int32 + (int32_t)pos_int32, "");
static_assert(std::is_same<decltype(add<uint16_t>(0, 0)), uint16_t>::value, "");

// subtract
static_assert(subtract<int64_t>(min_uint32, min_uint32) == (int64_t)min_uint32 - (int64_t)min_uint32, "");
static_assert(subtract<int32_t>(min_uint32, max_uint32) == (int32_t)min_uint32 - (int32_t)max_uint32, "");
static_assert(subtract<int16_t>(max_uint32, min_uint32) == (int16_t)max_uint32 - (int16_t)min_uint32, "");
static_assert(subtract< int8_t>(max_uint32, max_uint32) == ( int8_t)max_uint32 - (int8_t)max_uint32, "");
static_assert(subtract<int64_t>(pos_uint32, pos_uint32) == (int64_t)pos_uint32 - (int64_t)pos_uint32, "");
static_assert(subtract<int32_t>(pos_uint32, max_uint32) == (int32_t)pos_uint32 - (int32_t)max_uint32, "");
static_assert(subtract<int16_t>(max_uint32, pos_uint32) == (int16_t)max_uint32 - (int16_t)pos_uint32, "");
static_assert(subtract<int64_t>(pos_uint32, min_uint32) == (int64_t)pos_uint32 - (int64_t)min_uint32, "");
static_assert(subtract<int32_t>(min_uint32, pos_uint32) == (int32_t)min_uint32 - (int32_t)pos_uint32, "");
static_assert(subtract<int64_t>(zer_int32, zer_int32) == (int64_t)zer_int32 - (int64_t)zer_int32, "");
static_assert(subtract<int32_t>(zer_int32, max_int32) == (int32_t)zer_int32 - (int32_t)max_int32, "");
static_assert(subtract<int16_t>(max_int32, zer_int32) == (int16_t)max_int32 - (int16_t)zer_int32, "");
static_assert(subtract< int8_t>(max_int32, max_int32) == ( int8_t)max_int32 - (int8_t)max_int32, "");
static_assert(subtract<int64_t>(neg_int32, neg_int32) == (int64_t)neg_int32 - (int64_t)neg_int32, "");
////static_assert(subtract< int32_t>(neg_int32, max_int32) == (int32_t)neg_int32 - (int32_t)max_int32, "");
static_assert(subtract<int16_t>(max_int32, neg_int32) == (int16_t)max_int32 - (int16_t)neg_int32, "");
static_assert(subtract<int64_t>(neg_int32, min_int32) == (int64_t)neg_int32 - (int64_t)min_int32, "");
static_assert(subtract<int32_t>(min_int32, neg_int32) == (int32_t)min_int32 - (int32_t)neg_int32, "");
static_assert(subtract<int64_t>(pos_int32, neg_int32) == (int64_t)pos_int32 - (int64_t)neg_int32, "");
static_assert(subtract<int32_t>(neg_int32, pos_int32) == (int32_t)neg_int32 - (int32_t)pos_int32, "");
static_assert(subtract<int16_t>(pos_int32, pos_int32) == (int16_t)pos_int32 - (int16_t)pos_int32, "");
static_assert(subtract<int64_t>(pos_int32, min_int32) == (int64_t)pos_int32 - (int64_t)min_int32, "");
////static_assert(subtract<int32_t>(min_int32, pos_int32) == (int32_t)min_int32 - (int32_t)pos_int32, "");
static_assert(std::is_same<decltype(subtract<uint16_t>(0, 0)), uint16_t>::value, "");

constexpr size_t minimum = 0;
constexpr size_t maximum = max_size_t;
constexpr size_t half = to_half(maximum);

// ceilinged_add
static_assert(ceilinged_add(minimum, minimum) == minimum, "");
static_assert(ceilinged_add(minimum, maximum) == maximum, "");
static_assert(ceilinged_add(maximum, minimum) == maximum, "");
static_assert(ceilinged_add(maximum, maximum) == maximum, "");
static_assert(ceilinged_add(maximum, half) == maximum, "");
static_assert(ceilinged_add(half, maximum) == maximum, "");
static_assert(ceilinged_add(half, half) == maximum - lo_bit(maximum), "");
static_assert(std::is_same<decltype(ceilinged_add<uint16_t>(0, 0)), uint16_t>::value, "");

// floor_subtract
static_assert(floored_subtract(minimum, minimum) == minimum, "");
static_assert(floored_subtract(maximum, minimum) == maximum, "");
static_assert(floored_subtract(minimum, maximum) == minimum, "");
static_assert(floored_subtract(maximum, maximum) == minimum, "");
static_assert(floored_subtract(maximum, half) == half + lo_bit(maximum), "");
static_assert(floored_subtract(half, maximum) == minimum, "");
static_assert(floored_subtract(half, half) == minimum, "");
static_assert(std::is_same<decltype(floored_subtract<uint16_t>(0, 0)), uint16_t>::value, "");
