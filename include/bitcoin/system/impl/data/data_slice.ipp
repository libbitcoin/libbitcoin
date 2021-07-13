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
#ifndef LIBBITCOIN_SYSTEM_DATA_DATA_SLICE_IPP
#define LIBBITCOIN_SYSTEM_DATA_DATA_SLICE_IPP

#include <array>
#include <vector>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/constraints.hpp>
#include <bitcoin/system/math/math.hpp>

namespace libbitcoin {
namespace system {
   
// constructors
// ----------------------------------------------------------------------------

template <data_slice::size_type Size>
data_slice::data_slice(const char(&bytes)[Size]) noexcept
  : data_slice(from_literal(bytes))
{
}

template <data_slice::size_type Size, typename Byte, if_byte<Byte>>
data_slice::data_slice(const std::array<Byte, Size>& data) noexcept
  : data_slice(from_size(data.begin(), Size))
{
}

template <typename Byte, if_byte<Byte>>
data_slice::data_slice(const std::vector<Byte>& data) noexcept
  : data_slice(from_size(data.begin(), data.size()))
{
}

template <typename Iterator>
data_slice::data_slice(const Iterator& begin, const Iterator& end) noexcept
  : data_slice(from_iterators(begin, end))
{
}

template <typename Byte, if_byte<Byte>>
data_slice::data_slice(const Byte* begin, const Byte* end) noexcept
  : data_slice(from_iterators(begin, end))
{
}

// factories (private)
// ----------------------------------------------------------------------------

template <data_slice::size_type Size, typename Byte>
data_slice data_slice::from_literal(const Byte(&bytes)[Size]) noexcept
{
    // Guard 0 for lack of null terminator (see below).
    if (is_zero(Size))
        return {};

    // Literals are null terminated but initializer syntax char arrays are not.
    // Those are picked up here and lose their last char. Workaround:
    // data_slice slice({ 'f', 'o', 'o', 'b', 'a', 'r', '\0' });
    // The more common syntax works as expected: data_slice slice("foobar");
    return from_size(&bytes[0], sub1(Size));
}

// static
template <typename Iterator>
data_slice data_slice::from_iterators(const Iterator& begin,
    const Iterator& end) noexcept
{
    // An end iterator can be anything, so convert to size.
    const auto size = std::distance(begin, end);

    // Guard negative against iterators reversed (undefined behavior).
    // Guard 0 because &begin[0] is undefined if size is zero.
    if (is_negative(size) || is_zero(size))
        return {};

    return from_size(&begin[0], static_cast<size_type>(size));
}

// static
template <typename Pointer>
data_slice data_slice::from_size(Pointer begin, size_type size) noexcept
{
    // Guard 0 because &begin[0] is undefined if size is zero.
    if (is_zero(size))
        return {};

    // Pointer may be a char or uin8_t pointer or iterator type.
    const auto start = reinterpret_cast<pointer>(&begin[0]);
    return { start, std::next(start, size), size };
}

// properties
// ----------------------------------------------------------------------------

template <data_slice::size_type Size>
std::array<typename data_slice::value_type, Size>
data_slice::to_array() const noexcept
{
    std::array<data_slice::value_type, Size> out;

    // Array operator safely iterates and emits zeros past end.
    for (size_type index = 0; index < Size; ++index)
        out[index] = (*this)[index];

    return out;
}

// operators
// ----------------------------------------------------------------------------

template <data_slice::size_type Size>
data_slice::operator
std::array<typename data_slice::value_type, Size>() const noexcept
{
    return to_array<Size>();
}

} // namespace system
} // namespace libbitcoin

#endif
