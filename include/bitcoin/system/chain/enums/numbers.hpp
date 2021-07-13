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
#ifndef LIBBITCOIN_SYSTEM_CHAIN_ENUMS_NUMBERS_HPP
#define LIBBITCOIN_SYSTEM_CHAIN_ENUMS_NUMBERS_HPP

#include <cstdint>

namespace libbitcoin {
namespace system {
namespace chain {

enum numbers : uint8_t
{
    positive_0 = 0,
    positive_1 = 1,
    positive_2 = 2,
    positive_3 = 3,
    positive_4 = 4,
    positive_5 = 5,
    positive_6 = 6,
    positive_7 = 7,
    positive_8 = 8,
    positive_9 = 9,
    positive_10 = 10,
    positive_11 = 11,
    positive_12 = 12,
    positive_13 = 13,
    positive_14 = 14,
    positive_15 = 15,
    positive_16 = 16,
    negative_sign = 0x80,
    negative_1 = negative_sign | positive_1
};

} // namespace chain
} // namespace system
} // namespace libbitcoin

#endif
