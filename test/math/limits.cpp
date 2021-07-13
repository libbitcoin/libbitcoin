/**
 * Copyright (c) 2011-2021 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation == either version 3 of the License == or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not == see <http://www.gnu.org/licenses/>.
 */
#include "../test.hpp"
#include <cstddef>
#include <cstdint>

constexpr uint64_t min_uint64 = 0;

static_assert(limit< uint8_t>(min_uint64) == 0u, "");
static_assert(limit< int16_t>(min_uint64) == 0, "");
static_assert(limit<uint16_t>(min_uint64) == 0u, "");
static_assert(limit< int32_t>(min_uint64) == 0, "");
static_assert(limit<uint32_t>(min_uint64) == 0u, "");
static_assert(limit< int64_t>(min_uint64) == 0, "");
static_assert(limit<uint64_t>(min_uint64) == 0u, "");

static_assert(limit< uint8_t>(min_int64) == 0u, "");
static_assert(limit< int16_t>(min_int64) == min_int16, "");
static_assert(limit<uint16_t>(min_int64) == 0u, "");
static_assert(limit< int32_t>(min_int64) == min_int32, "");
static_assert(limit<uint32_t>(min_int64) == 0u, "");
static_assert(limit< int64_t>(min_int64) == min_int64, "");
static_assert(limit<uint64_t>(min_int64) == 0u, "");

static_assert(limit< uint8_t>(max_int64) == max_uint8, "");
static_assert(limit< int16_t>(max_int64) == max_int16, "");
static_assert(limit<uint16_t>(max_int64) == max_uint16, "");
static_assert(limit< int32_t>(max_int64) == max_int32, "");
static_assert(limit<uint32_t>(max_int64) == max_uint32, "");
static_assert(limit< int64_t>(max_int64) == max_int64, "");
static_assert(limit<uint64_t>(max_int64) == max_int64, "");

static_assert(limit< uint8_t>(max_uint64) == max_uint8, "");
static_assert(limit< int16_t>(max_uint64) == max_int16, "");
static_assert(limit<uint16_t>(max_uint64) == max_uint16, "");
static_assert(limit< int32_t>(max_uint64) == max_int32, "");
static_assert(limit<uint32_t>(max_uint64) == max_uint32, "");
static_assert(limit< int64_t>(max_uint64) == max_int64, "");
static_assert(limit<uint64_t>(max_uint64) == max_uint64, "");

static_assert(std::is_same<decltype(limit<uint8_t>(0)), uint8_t>::value, "");

static_assert(limit(size_t(0), size_t(0)) == size_t(0), "");
static_assert(limit(size_t(25), size_t(60)) == size_t(25), "");
static_assert(limit(size_t(75), size_t(60)) == size_t(60), "");

static_assert(std::is_same<decltype(limit<uint8_t>(0, 0)), uint8_t>::value, "");

static_assert(limit(size_t(0),   size_t(0), size_t(0))  == size_t(0),  "");
static_assert(limit(size_t(25), size_t(50), size_t(60)) == size_t(50), "");
static_assert(limit(size_t(50), size_t(50), size_t(60)) == size_t(50), "");
static_assert(limit(size_t(55), size_t(50), size_t(60)) == size_t(55), "");
static_assert(limit(size_t(60), size_t(50), size_t(60)) == size_t(60), "");
static_assert(limit(size_t(75), size_t(50), size_t(60)) == size_t(60), "");

static_assert(limit( 0, size_t(0) ,  size_t(0)) == size_t(0),  "");
static_assert(limit(25, size_t(50), size_t(60)) == size_t(50), "");
static_assert(limit(50, size_t(50), size_t(60)) == size_t(50), "");
static_assert(limit(55, size_t(50), size_t(60)) == size_t(55), "");
static_assert(limit(60, size_t(50), size_t(60)) == size_t(60), "");
static_assert(limit(75, size_t(50), size_t(60)) == size_t(60), "");

static_assert(limit(size_t(0),   0,  0) ==  0, "");
static_assert(limit(size_t(25), 50, 60) == 50, "");
static_assert(limit(size_t(50), 50, 60) == 50, "");
static_assert(limit(size_t(55), 50, 60) == 55, "");
static_assert(limit(size_t(60), 50, 60) == 60, "");
static_assert(limit(size_t(75), 50, 60) == 60, "");

static_assert(limit( 0,  0,  0) ==  0, "");
static_assert(limit(25, 50, 60) == 50, "");
static_assert(limit(50, 50, 60) == 50, "");
static_assert(limit(55, 50, 60) == 55, "");
static_assert(limit(60, 50, 60) == 60, "");
static_assert(limit(75, 50, 60) == 60, "");

static_assert(limit(-25, -60, -50) == -50, "");
static_assert(limit(-50, -60, -50) == -50, "");
static_assert(limit(-55, -60, -50) == -55, "");
static_assert(limit(-60, -60, -50) == -60, "");
static_assert(limit(-75, -60, -50) == -60, "");

static_assert(limit(-25, -60, 50) == -25, "");
static_assert(limit(-50, -60, 50) == -50, "");
static_assert(limit(-55, -60, 50) == -55, "");
static_assert(limit(-60, -60, 50) == -60, "");
static_assert(limit(-75, -60, 50) == -60, "");

static_assert(std::is_same<decltype(limit<uint8_t>(0, 0, 0)), uint8_t>::value, "");
