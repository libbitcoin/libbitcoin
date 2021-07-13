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
#include <bitcoin/system/data/data_slab.hpp>

#include <array>
#include <string>
#include <vector>
#include <bitcoin/system/data/data_slice.hpp>
#include <bitcoin/system/constants.hpp>

namespace libbitcoin {
namespace system {

// constructors
// ----------------------------------------------------------------------------

data_slab::data_slab() noexcept
  : data_slab(nullptr, nullptr, zero)
{
}

data_slab::data_slab(const data_slab& other) noexcept
  : begin_(other.begin_), end_(other.end_), size_(other.size_)
{
}

data_slab::data_slab(std::string& text) noexcept
  : data_slab(from_size(text.begin(), text.size()))
{
}

// private
data_slab::data_slab(pointer begin, pointer end, size_type size) noexcept
  : begin_(begin), end_(end), size_(size)
{
}

// methods
// ----------------------------------------------------------------------------

std::vector<data_slab::value_type> data_slab::to_chunk() const noexcept
{
    return { begin_, end_ };
}

data_slice data_slab::to_slice() const noexcept
{
    return { begin_, end_ };
}

std::string data_slab::to_string() const noexcept
{
    return { begin_, end_ };
}

// Cannot provide a "decode" factory since the data is not owned.
std::string data_slab::encoded() const noexcept
{
    return to_slice().encoded();
}

bool data_slab::resize(size_t size) noexcept
{
    if (size >= size_)
        return false;

    end_ = std::next(begin_, size);
    size_ = size;
    return true;
}

// properties
// ----------------------------------------------------------------------------

// Undefined to dereference >= end.
data_slab::pointer data_slab::data() const noexcept
{
    return begin_;
}

// Undefined to dereference >= end.
data_slab::pointer data_slab::begin() const noexcept
{
    return begin_;
}

// Undefined to dereference >= end.
data_slab::pointer data_slab::end() const noexcept
{
    return end_;
}

data_slab::value_type data_slab::front() const noexcept
{
    // Guard against end overrun (return zero).
    return empty() ? 0x00 : *begin_;
}

data_slab::value_type data_slab::back() const noexcept
{
    // Guard against begin underrun (return zero).
    return empty() ? 0x00 : *std::prev(end_);
}

data_slab::size_type data_slab::size() const noexcept
{
    return size_;
}

bool data_slab::empty() const noexcept
{
    return is_zero(size_);
}

// operators
// ----------------------------------------------------------------------------

data_slab::operator std::vector<data_slab::value_type>() const noexcept
{
    return data_slab::to_chunk();
}

data_slab::operator data_slice() const noexcept
{
    return data_slab::to_slice();
}

data_slab::value_type data_slab::operator[](size_type index) const noexcept
{
    // Guard against end overrun (return zero).
    return index < size_ ? *std::next(begin_, index) : 0x00;
}

bool operator==(const data_slab& left, const data_slab& right) noexcept
{
    return left.to_slice() == right.to_slice();
}

bool operator!=(const data_slab& left, const data_slab& right) noexcept
{
    return !(left == right);
}

} // namespace system
} // namespace libbitcoin
