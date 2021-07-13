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
#ifndef LIBBITCOIN_SYSTEM_STREAM_DEVICE_IPP
#define LIBBITCOIN_SYSTEM_STREAM_DEVICE_IPP

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <boost/iostreams/stream.hpp>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/math/sign.hpp>

namespace libbitcoin {
namespace system {

// protected
template <typename Container>
device<Container>::device(size_type remaining) noexcept
  : remaining_(remaining)
{
}

// public, device interface
// ----------------------------------------------------------------------------

template <typename Container>
typename device<Container>::sequence
device<Container>::input_sequence() const noexcept
{
    // Required to match output_sequence for read/write devices.
    return do_sequence();
}

template <typename Container>
typename device<Container>::sequence
device<Container>::output_sequence() const noexcept
{
    // Required to match input_sequence for read/write devices.
    return do_sequence();
}

template <typename Container>
typename device<Container>::size_type
device<Container>::read(char_type* buffer, size_type count) noexcept
{
    if (is_null(buffer) || is_negative(count))
        return negative_one;

    const auto size = std::min(remaining_, count);
    do_read(reinterpret_cast<value_type*>(buffer), size);
    remaining_ -= size;

    // eof is indicated by the return of a value less than count.
    return size;
}

template <typename Container>
typename device<Container>::size_type
device<Container>::write(const char_type* buffer, size_type count) noexcept
{
    if (is_null(buffer) || is_negative(count))
        return negative_one;

    const auto size = std::min(remaining_, count);
    do_write(reinterpret_cast<const value_type*>(buffer), size);
    remaining_ -= size;

    // eof is indicated by the return of a value less than count.
    return size;
}

template <typename Container>
typename device<Container>::size_type
device<Container>::optimal_buffer_size() const noexcept
{
    // The buffer size allocated by iostreams upon indirect device construct.
    return do_optimal_buffer_size();
}

// protected virtual
// ----------------------------------------------------------------------------

template <typename Container>
typename device<Container>::sequence
device<Container>::do_sequence() const noexcept
{
    // Not implemented, override.
    return { nullptr, nullptr };
}

template <typename Container>
void device<Container>::do_read(value_type* buffer, size_type size) noexcept
{
    // Not implemented, override.
    std::fill(buffer, std::next(buffer, size), 0x00);
}

template <typename Container>
void device<Container>::do_write(const value_type*, size_type) noexcept
{
    // Not implemented, override.
}

template <typename Container>
typename device<Container>::size_type
device<Container>::do_optimal_buffer_size() const noexcept
{
    // Defaults to 4k bytes, override in indirect devices.
    return BOOST_IOSTREAMS_DEFAULT_DEVICE_BUFFER_SIZE;
}

} // namespace system
} // namespace libbitcoin

#endif
