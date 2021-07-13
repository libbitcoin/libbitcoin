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
#ifndef LIBBITCOIN_SYSTEM_CONSTANTS_HPP
#define LIBBITCOIN_SYSTEM_CONSTANTS_HPP

#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {

// Guard assumptions within the codebase.
static_assert(sizeof(size_t) == sizeof(uint32_t) ||
    sizeof(size_t) == sizeof(uint64_t), "unsupported size_t");

// Integral value limits.
constexpr int64_t min_int64 = std::numeric_limits<int64_t>::min();
constexpr int64_t max_int64 = std::numeric_limits<int64_t>::max();
constexpr int32_t min_int32 = std::numeric_limits<int32_t>::min();
constexpr int32_t max_int32 = std::numeric_limits<int32_t>::max();
constexpr int16_t min_int16 = std::numeric_limits<int16_t>::min();
constexpr int16_t max_int16 = std::numeric_limits<int16_t>::max();
constexpr uint64_t max_uint64 = std::numeric_limits<uint64_t>::max();
constexpr uint32_t max_uint32 = std::numeric_limits<uint32_t>::max();
constexpr uint16_t max_uint16 = std::numeric_limits<uint16_t>::max();
constexpr uint8_t max_uint8 = std::numeric_limits<uint8_t>::max();
constexpr uint64_t max_size_t = std::numeric_limits<size_t>::max();

// The number of bits in a byte (uint8_t)
constexpr uint8_t byte_bits = 8;

// Use zero, one, two when any unsigned or size_t value is required.
constexpr size_t zero = 0;
constexpr size_t one = 1;
constexpr size_t two = 2;

// Use negative_one when returning negative one (or any negative) as sentinel.
constexpr int32_t negative_one = -1;

template <typename Type>
constexpr bool is_zero(Type value) noexcept
{
    return value == 0;
}

template <typename Type>
constexpr bool is_one(Type value) noexcept
{
    return value == 1;
}

template <typename Type>
constexpr Type lo_bit(Type value) noexcept
{
    return value % 2;
}

template <typename Type>
constexpr bool is_even(Type value) noexcept
{
    return is_zero(lo_bit(value));
}

template <typename Type>
constexpr bool is_odd(Type value) noexcept
{
    return !is_even(value);
}

template <typename Type>
constexpr bool is_integer()
{
    return std::numeric_limits<Type>::is_integer &&
        !std::is_same<bool, Type>::value;
}

template <typename Type>
constexpr bool is_null(Type value) noexcept
{
    return value == nullptr;
}

template <typename Type>
constexpr Type to_bits(Type bytes) noexcept
{
    return bytes * 8;
}

template <typename Type>
constexpr Type to_bytes(Type bits) noexcept
{
    return bits / 8;
}

template <typename Type>
constexpr Type to_half(Type value) noexcept
{
    return value / 2;
}

template <typename Type = int>
constexpr Type to_int(bool value) noexcept
{
    return value ? 1 : 0;
}

template <typename Type>
constexpr bool to_bool(Type value) noexcept
{
    return !is_zero(value);
}

template <typename Type>
constexpr Type add1(Type value) noexcept
{
    return value + 1;
}

template <typename Type>
constexpr Type sub1(Type value) noexcept
{
    return value - 1;
}

template <typename Type>
constexpr size_t width() noexcept
{
    // This is not always a logical size for non-integral types.
    return to_bits(sizeof(Type));
}

template <typename Type>
constexpr size_t width(Type value) noexcept
{
    // This is not always a logical size for non-integral types.
    return to_bits(sizeof(value));
}

/// Variable integer prefix sentinels.
constexpr uint8_t varint_two_bytes = 0xfd;
constexpr uint8_t varint_four_bytes = 0xfe;
constexpr uint8_t varint_eight_bytes = 0xff;

/// Determine the bitcoin variable-serialized size of a given value.
constexpr size_t variable_size(uint64_t value)
{
    // C++11: single return required for constexpr.
    return (value < varint_two_bytes) ? sizeof(uint8_t) :
        ((value <= max_uint16) ? sizeof(uint8_t) + sizeof(uint16_t) :
            ((value <= max_uint32) ? sizeof(uint8_t) + sizeof(uint32_t) :
                sizeof(uint8_t) + sizeof(uint64_t)));
}

} // namespace libbitcoin

#endif
