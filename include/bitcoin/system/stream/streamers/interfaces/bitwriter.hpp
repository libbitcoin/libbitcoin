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

#ifndef LIBBITCOIN_SYSTEM_STREAM_STREAMERS_INTERFACES_BITWRITER_HPP
#define LIBBITCOIN_SYSTEM_STREAM_STREAMERS_INTERFACES_BITWRITER_HPP

#include <cstddef>
#include <cstdint>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/stream/streamers/interfaces/bytewriter.hpp>

namespace libbitcoin {
namespace system {

/// A bit writer interface.
class bitwriter
  : public virtual bytewriter
{
public:
    /// Write one bit (high to low).
    virtual void write_bit(bool value) noexcept = 0;

    /// Write size bits from an integer (high to low)
    virtual void write_bits(uint64_t value, size_t bits) noexcept = 0;
};

} // namespace system
} // namespace libbitcoin

#endif
