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
#ifndef LIBBITCOIN_SYSTEM_DATA_DATA_ARRAY_HPP
#define LIBBITCOIN_SYSTEM_DATA_DATA_ARRAY_HPP

#include <array>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/constraints.hpp>
#include <bitcoin/system/data/data_chunk.hpp>
#include <bitcoin/system/data/data_slice.hpp>

namespace libbitcoin {
namespace system {

/// Byte array of a specified length.
template <size_t Size>
using data_array = std::array<uint8_t, Size>;

/// Return type for splitters.
template <size_t Size>
using split_parts = std::pair<data_array<Size>, data_array<Size>>;

/// Arbitrary byte storage types.
typedef data_array<one> one_byte;

/// Create a single byte arrray with given element value.
inline one_byte to_array(uint8_t byte) noexcept;

/// Convert the data slice to an array.
/// Underfill is padded with 0x00, excess is truncated.
/// to_array(to_string(data)) == data.
template <size_t Size>
data_array<Size> to_array(const data_slice& bytes) noexcept;

/// Create a data stack from vector of data array.
template <size_t Size>
data_stack to_stack(const std::vector<data_array<Size>>& values) noexcept;

/// Concatenate several data slices into a single array.
/// Underfill is padded with 0x00, excess is truncated.
template <size_t Size>
data_array<Size> build_array(const data_loaf& slices) noexcept;

/// Extend insertable target by copying extension.
template <class Target>
Target& extend(Target& target, const data_slice& extension) noexcept;

/// Extend insertable target by moving extension.
template <class Target, class Extension>
Target& extend(Target& target, Extension&& extension) noexcept;

/// Extract a byte subarray from start position with length end minus start.
template <size_t Start, size_t End, size_t Size,
    if_not_greater<Start, Size> = true, if_not_greater<End, Size> = true,
    if_not_lesser<End, Start> = true>
data_array<End - Start> slice(const data_array<Size>& bytes) noexcept;

/// Break an evenly-sized byte array array into two equal length parts.
template <size_t Size, if_even<Size> = true>
split_parts<to_half(Size)> split(const data_array<Size>& bytes) noexcept;

/// Concatenate two byte arrays into a new array.
template <size_t Left, size_t Right>
data_array<Left + Right> splice(const data_array<Left>& left,
    const data_array<Right>& right) noexcept;

/// Concatenate three byte arrays into a new array.
template <size_t Left, size_t Middle, size_t Right>
data_array<Left + Middle + Right> splice(const data_array<Left>& left,
    const data_array<Middle>& middle, const data_array<Right>& right) noexcept;

/// Perform an exclusive or (xor) on two arrays to specified length.
template <size_t Size, size_t Size1, size_t Size2,
    if_not_lesser<Size1, Size> = true, if_not_lesser<Size2, Size> = true>
data_array<Size> xor_data(const data_array<Size1>& bytes1,
    const data_array<Size2>& bytes2) noexcept;

/// Perform an exclusive or (xor) on two arrays at specified offsets and length.
template <size_t Size, size_t Offset1, size_t Offset2, size_t Size1, size_t Size2,
    if_not_lesser<Size1, Offset1 + Size> = true,
    if_not_lesser<Size2, Offset2 + Size> = true>
data_array<Size> xor_offset(const data_array<Size1>& bytes1,
    const data_array<Size2>& bytes2) noexcept;

} // namespace system
} // namespace libbitcoin

#include <bitcoin/system/impl/data/data_array.ipp>

#endif
