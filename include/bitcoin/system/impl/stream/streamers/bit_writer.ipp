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

#ifndef LIBBITCOIN_SYSTEM_STREAM_STREAMERS_BIT_WRITER_IPP
#define LIBBITCOIN_SYSTEM_STREAM_STREAMERS_BIT_WRITER_IPP

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/constraints.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/math/math.hpp>
#include <bitcoin/system/serial/serial.hpp>

namespace libbitcoin {
namespace system {

template <typename OStream>
const uint8_t bit_writer<OStream>::pad = 0x00;

// constructors
//-----------------------------------------------------------------------------

template <typename OStream>
bit_writer<OStream>::bit_writer(OStream& sink) noexcept
  : byte_writer(sink), byte_(pad), offset_(0)
{
}

template <typename OStream>
bit_writer<OStream>::~bit_writer() noexcept
{
    flusher();
    byte_writer::~byte_writer();
}

// bits
//-----------------------------------------------------------------------------

template <typename OStream>
void bit_writer<OStream>::write_bit(bool value) noexcept
{
    // Insert bit into byte.
    set_left(byte_, offset_++, value);

    // unload resets offset_ to zero.
    if (is_zero(shift()))
        unload();
}

template <typename OStream>
void bit_writer<OStream>::write_bits(uint64_t value, size_t bits) noexcept
{
    bits = lesser<size_t>(width(value), bits);

    // 'bits' refers to the count of the rightmost bits in 'value'.
    // Those bits are read from left to right using a right-relative offset.
    // Subtract one for size-to-index translation, avoiding iterator underflow.
    for (auto bit = bits; !is_zero(bit); --bit)
        write_bit(get_right(value, sub1(bit)));
}

// protected overrides
//-----------------------------------------------------------------------------

template <typename OStream>
void bit_writer<OStream>::do_write_bytes(const uint8_t* data,
    size_t size) noexcept
{
    // Suboptimal becase shifts each bit and writes single bytes, but simple.
    for (size_t byte = 0; byte < size; ++byte)
        write_bits(data[byte], byte_bits);
}

template <typename OStream>
void bit_writer<OStream>::do_flush() noexcept
{
    flusher();
    byte_writer::do_flush();
}

// private
//-----------------------------------------------------------------------------

// This is the only byte write and offset change.
template <typename OStream>
void bit_writer<OStream>::unload() noexcept
{
    byte_writer::do_write_bytes(&byte_, one);
    byte_ = pad;
    offset_ = 0;
}

template <typename OStream>
void bit_writer<OStream>::flusher() noexcept
{
    if (!is_zero(offset_))
        unload();
}

template <typename OStream>
constexpr uint8_t bit_writer<OStream>::shift() const noexcept
{
    // If shift is zero then eight bits have been written, so time to dump.
    // If offset is zero then no bits have been written since last dump.
    return byte_bits - offset_;
}

} // namespace system
} // namespace libbitcoin

#endif
