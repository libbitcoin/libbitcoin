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
#ifndef LIBBITCOIN_SYSTEM_STREAM_STREAMERS_INTERFACES_BYTEFLIPPER_HPP
#define LIBBITCOIN_SYSTEM_STREAM_STREAMERS_INTERFACES_BYTEFLIPPER_HPP

#include <bitcoin/system/stream/streamers/interfaces/bytereader.hpp>
#include <bitcoin/system/stream/streamers/interfaces/bytewriter.hpp>

namespace libbitcoin {
namespace system {
    
/// A byte reader/writer interface.
class byteflipper
  : public virtual bytereader,
    public virtual bytewriter
{
};

} // namespace system
} // namespace libbitcoin

#endif
