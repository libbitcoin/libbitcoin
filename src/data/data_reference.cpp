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
#include <bitcoin/system/data/data_reference.hpp>

#include <string>
#include <bitcoin/system/data/data_chunk.hpp>
#include <bitcoin/system/data/data_slice.hpp>

namespace libbitcoin {
namespace system {

data_reference::data_reference() noexcept
  : data_slice()
{
}

data_reference::data_reference(const data_reference& other) noexcept
  : data_slice(other)
{
}

data_reference::data_reference(const data_slice& data) noexcept
  : data_slice(data)
{
}

data_reference::data_reference(const std::string& text) noexcept
  : data_slice(text)
{
}

data_reference::data_reference(const data_chunk& data) noexcept
  : data_slice(data)
{
}

} // namespace system
} // namespace libbitcoin
