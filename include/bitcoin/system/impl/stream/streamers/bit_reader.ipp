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

// Sponsored in part by Digital Contract Design, LLC

#ifndef LIBBITCOIN_SYSTEM_STREAM_STREAMERS_BIT_READER_IPP
#define LIBBITCOIN_SYSTEM_STREAM_STREAMERS_BIT_READER_IPP

#include <bitcoin/system/stream/streamers/bit_reader.hpp>

#include <cstddef>
#include <cstdint>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/constraints.hpp>
#include <bitcoin/system/math/math.hpp>
#include <bitcoin/system/serial/serial.hpp>

namespace libbitcoin {
namespace system {
    
template <typename IStream>
const uint8_t bit_reader<IStream>::pad = 0x00;

// constructors
//-----------------------------------------------------------------------------

template <typename IStream>
bit_reader<IStream>::bit_reader(IStream& source) noexcept
  : byte_reader(source), byte_(pad), offset_(byte_bits)
{
}

template <typename IStream>
bit_reader<IStream>::~bit_reader() noexcept
{
    byte_reader::~byte_reader();
}

// bits
//-----------------------------------------------------------------------------

template <typename IStream>
bool bit_reader<IStream>::read_bit() noexcept
{
    // load resets offset_ to zero.
    if (is_zero(shift()))
        load();

    // Extract bit from byte.
    return get_left(byte_, offset_++);
}

template <typename IStream>
uint64_t bit_reader<IStream>::read_bits(size_t bits) noexcept
{
    uint64_t out = 0;
    bits = lesser<size_t>(width(out), bits);

    // 'bits' refers to the count of the rightmost bits in 'out'.
    // Those bits are written from left to right using a right-relative offset.
    // Subtract one for size-to-index translation, avoiding iterator underflow.
    for (auto bit = bits; !is_zero(bit); --bit)
        set_right(out, sub1(bit), read_bit());

    return out;
}

template <typename IStream>
void bit_reader<IStream>::skip_bit() noexcept
{
    // load resets offset_ to zero.
    if (is_zero(shift()))
        load();

    ++offset_;
}

template <typename IStream>
void bit_reader<IStream>::skip_bits(size_t bits) noexcept
{
    while (!is_zero(bits--))
        skip_bit();
}

template <typename IStream>
void bit_reader<IStream>::rewind_bit() noexcept
{
    // reload resets offset_ to byte_bits.
    if (is_zero(offset_))
        reload();

    --offset_;
}

template <typename IStream>
void bit_reader<IStream>::rewind_bits(size_t bits) noexcept
{
    while (!is_zero(bits--))
        rewind_bit();
}

// protected overrides
//-----------------------------------------------------------------------------

template <typename IStream>
uint8_t bit_reader<IStream>::do_peek_byte() noexcept
{
    if (is_zero(offset_))
        return peek();

    // Merge the peeked byte with the buffered byte.
    return (byte_ << offset_) | (peek() >> shift());
}

template <typename IStream>
void bit_reader<IStream>::do_read_bytes(uint8_t* buffer, size_t size) noexcept
{
    // Suboptimal becase shifts each bit and reads single bytes, but simple.
    for (size_t byte = 0; byte < size; ++byte)
        buffer[byte] = static_cast<uint8_t>(read_bits(byte_bits));
}

template <typename IStream>
void bit_reader<IStream>::do_skip_bytes(size_t size) noexcept
{
    while (!is_zero(size--))
        skip_bits(byte_bits);
}

template <typename IStream>
void bit_reader<IStream>::do_rewind_bytes(size_t size) noexcept
{
    while (!is_zero(size--))
        rewind_bits(byte_bits);
}

 // This is the only use of the offset apart from peek and read.
template <typename IStream>
bool bit_reader<IStream>::get_exhausted() const noexcept
{
    if (byte_reader::operator!())
        return true;

    return is_zero(shift()) && byte_reader::get_exhausted();
}

// private
//-----------------------------------------------------------------------------

template <typename IStream>
void bit_reader<IStream>::load() noexcept
{
    // The next bit read will be from this byte.
    byte_ = pad;
    offset_ = 0;
    byte_reader::do_read_bytes(&byte_, one);
}

template <typename IStream>
void bit_reader<IStream>::reload() noexcept
{
    byte_ = pad;
    offset_ = byte_bits;
    byte_reader::do_rewind_bytes(two);
    byte_reader::do_read_bytes(&byte_, one);
}

// This is the only byte peek.
template <typename IStream>
uint8_t bit_reader<IStream>::peek() noexcept
{
    return byte_reader::do_peek_byte();
}

template <typename OStream>
constexpr uint8_t bit_reader<OStream>::shift() const noexcept
{
    // If shift is zero then eight bits have been read, so time to load.
    // If offset is zero then no bits have been read since last load.
    return byte_bits - offset_;
}

} // namespace system
} // namespace libbitcoin

#endif
