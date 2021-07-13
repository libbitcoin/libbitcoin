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
#ifndef LIBBITCOIN_SYSTEM_SERIAL_ENDIAN_IPP
#define LIBBITCOIN_SYSTEM_SERIAL_ENDIAN_IPP

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <string>
#include <boost/range/adaptor/reversed.hpp>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/constraints.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/math/math.hpp>

namespace libbitcoin {
namespace system {

// These conversions are efficient, performing no data copies or reversals.
// array/uintx_t sizes are inferred by type, and vector/uintx by value.
// High order bits are padded when read (from) is insufficient.
// High order bits are ignored when write (to) is insufficient.

// byte convertors
// ----------------------------------------------------------------------------
// C++ standard: "The [shift] behavior is undefined if ... greater than or
// equal to the width of the promoted left operand." So we must be careful to
// not shift bytes by byte_bits (8). For this reason we specialize on byte-size
// integers. This also presents a performance optimization for byte conversion,
// which is close to a no-op.

template <typename Integer, if_byte<Integer> = true>
static Integer from_big(size_t size, const data_slice& data) noexcept
{
    return data.empty() ? 0 : data.front();
}

template <typename Integer, if_byte<Integer> = true>
static Integer from_little(size_t size, const data_slice& data) noexcept
{
    return data.empty() ? 0 : data.front();
}

template <typename Data, typename Integer, if_byte<Integer> = true>
static Data to_big(Data&& bytes, Integer value) noexcept
{
    bytes.front() = static_cast<uint8_t>(value);
    return bytes;
}

template <typename Data, typename Integer, if_byte<Integer> = true>
static Data to_little(Data&& bytes, Integer value) noexcept
{
    bytes.front() = static_cast<uint8_t>(value);
    return bytes;
}

// integer convertors
// ----------------------------------------------------------------------------
// C++ standard: "Right-shift on signed integral types is an arithmetic right
// shift, which performs sign-extension". In other words, repeatedly shifting
// -1 of any integer width will produce "1" bits, indefinitely.

template <typename Integer, if_bytes<Integer> = true>
static Integer from_big(size_t size, const data_slice& data) noexcept
{
    // read msb (forward), shift in the byte (no shift on first)
    // data[0] is most significant
    // { 0x01, 0x02 } => 0x0102
    // 0x0000 << 8 => 0x0000
    // 0x0001 << 8 => 0x0100
    // 0x0000 |= 0x01[0] => 0x0001
    // 0x0100 |= 0x02[1] => 0x0102

    Integer value(0);
    const auto bytes = std::min(size, data.size());

    for (size_t byte = 0; byte < bytes; ++byte)
    {
        value <<= byte_bits;
        value |= Integer{ data[byte] };
    }

    return value;
}

template <typename Integer, if_bytes<Integer> = true>
static Integer from_little(size_t size, const data_slice& data) noexcept
{
    // read msb (reverse), shift in the byte (no shift on first)
    // data[0] is least significant
    // { 0x01, 0x02 } => 0x0201
    // 0x0000 << 8 => 0x0000
    // 0x0002 << 8 => 0x0200
    // 0x0000 |= 0x02[1] => 0x0002
    // 0x0200 |= 0x01[0] => 0x0201

    Integer value(0);
    const auto bytes = std::min(size, data.size());

    for (auto byte = bytes; byte > 0; --byte)
    {
        value <<= byte_bits;
        value |= Integer{ data[sub1(byte)] };
    }

    return value;
}

template <typename Data, typename Integer, if_bytes<Integer> = true>
static Data to_big(Data&& bytes, Integer value) noexcept
{
    // read and shift out lsb, set byte in reverse order
    // data[0] is most significant
    // 0x0102 => { 0x01, 0x02 }
    // (uint8_t)0x0102 => 0x02[1]
    // (uint8_t)0x0001 => 0x01[0]
    // 0x0102 >> 8 => 0x0001
    // 0x0001 >> 8 => 0x0000

    for (auto& byte: boost::adaptors::reverse(bytes))
    {
        byte = static_cast<uint8_t>(value);
        value >>= byte_bits;
    }

    return bytes;
}

template <typename Data, typename Integer, if_bytes<Integer> = true>
static Data to_little(Data&& bytes, Integer value) noexcept
{
    // read and shift out lsb, set byte in forward order
    // data[0] is least significant
    // 0x0102 => { 0x02, 0x01 }
    // (uint8_t)0x0102 => 0x02[0]
    // (uint8_t)0x0001 => 0x01[1]
    // 0x0102 >> 8 => 0x0001
    // 0x0001 >> 8 => 0x0000

    for (auto& byte: bytes)
    {
        byte = static_cast<uint8_t>(value);
        value >>= byte_bits;
    }

    return bytes;
}

// integer => data (value-sized) or array (explicit size)
// ----------------------------------------------------------------------------
// These allocate the to-endian outgoing buffer and forward the call.

template <size_t Size, typename Integer, if_integer<Integer>>
data_array<Size> to_big_endian_array(Integer value) noexcept
{
    return to_big(data_array<Size>{}, value);
}

template <size_t Size, typename Integer, if_integer<Integer>>
data_array<Size> to_little_endian_array(Integer value) noexcept
{
    return to_little(data_array<Size>{}, value);
}

template <typename Integer, if_integer<Integer>>
data_chunk to_big_endian_chunk(Integer value) noexcept
{
    return to_big(data_chunk(byte_width(value), no_fill_allocator), value);
}

template <typename Integer, if_integer<Integer>>
data_chunk to_little_endian_chunk(Integer value) noexcept
{
    return to_little(data_chunk(byte_width(value), no_fill_allocator), value);
}

// data => integral, integral => byte_array
// ----------------------------------------------------------------------------

template <typename Integer, if_integral_integer<Integer>>
Integer from_big_endian(const data_slice& data) noexcept
{
    // Limit sizeof to integral integers.
    return from_big<Integer>(sizeof(Integer), data);
}

template <typename Integer, if_integral_integer<Integer>>
Integer from_little_endian(const data_slice& data) noexcept
{
    // Limit sizeof to integral integers.
    return from_little<Integer>(sizeof(Integer), data);
}

template <typename Integer, if_integral_integer<Integer>>
data_array<sizeof(Integer)> to_big_endian(Integer value) noexcept
{
    // Limit sizeof to integral integers.
    return to_big_endian_array<sizeof(Integer)>(value);
}

template <typename Integer, if_integral_integer<Integer>>
data_array<sizeof(Integer)> to_little_endian(Integer value) noexcept
{
    // Limit sizeof to integral integers.
    return to_little_endian_array<sizeof(Integer)>(value);
}

// data => uintx
// uintx => data_chunk
// ----------------------------------------------------------------------------
// Overload for uintx, as from_big_endian<0> reads zero bytes and uintx is a
// signed type (though otherwise would be declared as uintx_t<0>).

template <typename Integer, if_base_of<Integer, uintx>>
Integer from_big_endian(const data_slice& data) noexcept
{
    // uintx return can handle data of arbitrary size.
    return from_big<uintx>(data.size(), data);
}

template <typename Integer, if_base_of<Integer, uintx>>
Integer from_little_endian(const data_slice& data) noexcept
{
    // uintx return can handle data of arbitrary size.
    return from_little<uintx>(data.size(), data);
}

template <typename Integer, if_base_of<Integer, uintx>>
data_chunk to_big_endian(const Integer& value) noexcept
{
    // data_chunk size determined by uintx (vs. type).
    return to_big_endian_chunk(value);
}

template <typename Integer, if_base_of<Integer, uintx>>
data_chunk to_little_endian(const Integer& value) noexcept
{
    // data_chunk size determined by uintx (vs. type).
    return to_little_endian_chunk(value);
}

// data => uintx_t<to_bits(Bytes)>
// uintx_t (or other integer) => data_array
// ----------------------------------------------------------------------------

template <size_t Bytes>
uintx_t<to_bits(Bytes)> from_big_endian(const data_slice& data) noexcept
{
    return from_big<uintx_t<to_bits(Bytes)>>(Bytes, data);
}

template <size_t Bytes>
uintx_t<to_bits(Bytes)> from_little_endian(const data_slice& data) noexcept
{
    return from_little<uintx_t<to_bits(Bytes)>>(Bytes, data);
}

template <size_t Bytes, typename Integer, if_integer<Integer>>
data_array<Bytes> to_big_endian(const Integer& value) noexcept
{
    return to_big_endian_array<Bytes>(value);
}

template <size_t Bytes, typename Integer, if_integer<Integer>>
data_array<Bytes> to_little_endian(const Integer& value) noexcept
{
    return to_little_endian_array<Bytes>(value);
}

// stream <=> integral
// ----------------------------------------------------------------------------

template <typename Integer, if_integral_integer<Integer>>
Integer from_big_endian(std::istream& stream) noexcept
{
    // Limit sizeof to integral integers.
    std::vector<char> buffer(sizeof(Integer));
    stream.read(buffer.data(), buffer.size());
    buffer.resize(stream.gcount());
    return from_big_endian<Integer>(buffer);
}

template <typename Integer, if_integral_integer<Integer>>
Integer from_little_endian(std::istream& stream) noexcept
{
    // Limit sizeof to integral integers.
    std::vector<char> buffer(sizeof(Integer));
    stream.read(buffer.data(), buffer.size());
    buffer.resize(stream.gcount());
    return from_little_endian<Integer>(buffer);
}

template <typename Integer, if_integral_integer<Integer>>
void to_big_endian(std::ostream& stream, Integer value) noexcept
{
    // Limit sizeof (in above override) to integral integers.
    const auto buffer = to_big_endian(value);
    stream.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
}

template <typename Integer, if_integral_integer<Integer>>
void to_little_endian(std::ostream& stream, Integer value) noexcept
{
    // Limit sizeof (in above override) to integral integers.
    const auto buffer = to_little_endian(value);
    stream.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
}

// iterator => integral
// ----------------------------------------------------------------------------
// size is unguarded.

template <typename Integer, typename Iterator, if_integral_integer<Integer>>
Integer from_big_endian_unchecked(const Iterator& data) noexcept
{
    // Limit sizeof to integral integers.
    const auto slice = data_slice(data, std::next(data, sizeof(Integer)));
    return from_big_endian<Integer>(slice);
}

template <typename Integer, typename Iterator, if_integral_integer<Integer>>
Integer from_little_endian_unchecked(const Iterator& data) noexcept
{
    // Limit sizeof to integral integers.
    const auto slice = data_slice(data, std::next(data, sizeof(Integer)));
    return from_little_endian<Integer>(slice);
}

} // namespace system
} // namespace libbitcoin

#endif
