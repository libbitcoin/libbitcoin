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
#ifndef LIBBITCOIN_SYSTEM_DATA_DATA_REFERENCE_IPP
#define LIBBITCOIN_SYSTEM_DATA_DATA_REFERENCE_IPP

#include <array>
#include <vector>
#include <bitcoin/system/constraints.hpp>
#include <bitcoin/system/data/data_array.hpp>
#include <bitcoin/system/data/data_slice.hpp>

namespace libbitcoin {
namespace system {

/// data_array constructor.
template <data_reference::size_type Size>
data_reference::data_reference(const data_array<Size>& data) noexcept
  : data_slice(data)
{
}

/// Byte array constructor (casts Byte to uint8_t).
template <data_reference::size_type Size, typename Byte, if_byte<Byte>>
data_reference::data_reference(const std::array<Byte, Size>& data) noexcept
  : data_slice(data)
{
}

/// Byte vector constructor (casts Byte to uint8_t).
template <typename Byte, if_byte<Byte>>
data_reference::data_reference(const std::vector<Byte>& data) noexcept
  : data_slice(data)
{
}

/// Byte iterators constructor (casts to uint8_t).
template <typename Iterator>
data_reference::data_reference(const Iterator& begin,
    const Iterator& end) noexcept
  : data_slice(begin, end)
{
}

/// Byte pointer to const constructor (casts Byte to uint8_t).
template <typename Byte, if_byte<Byte>>
data_reference::data_reference(const Byte* begin, const Byte* end) noexcept
  : data_slice(begin, end)
{
}

} // namespace system
} // namespace libbitcoin

#endif
