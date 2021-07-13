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

#ifndef LIBBITCOIN_SYSTEM_STREAM_STREAMERS_BIT_WRITER_HPP
#define LIBBITCOIN_SYSTEM_STREAM_STREAMERS_BIT_WRITER_HPP

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <bitcoin/system/stream/streamers/byte_writer.hpp>
#include <bitcoin/system/stream/streamers/interfaces/bitwriter.hpp>

namespace libbitcoin {
namespace system {

// Suppress multiple inheritance warnings.
// The inheritance is virtual, so not actually multiple.
// But the boost type constraint 'is_virtual_base_of' triggers the warning.
#pragma warning(push)
#pragma warning(disable : 4250)

/// A bit writer that accepts an ostream.
template <typename OStream = std::ostream>
class bit_writer
  : public byte_writer<OStream>,
    public virtual bitwriter
{
public:
    /// Constructors.
    bit_writer(OStream& sink) noexcept;
    ~bit_writer() noexcept override;

    /// Write one bit (high to low).
    virtual void write_bit(bool value) noexcept;

    /// Write size bits from an integer (high to low).
    void write_bits(uint64_t value, size_t bits) noexcept;

protected:
    void do_write_bytes(const uint8_t* data, size_t size) noexcept override;
    void do_flush() noexcept override;

private:
    static const uint8_t pad;

    void unload() noexcept;
    void flusher() noexcept;
    constexpr uint8_t shift() const noexcept;

    uint8_t byte_;
    uint8_t offset_;
};

#pragma warning(pop)

} // namespace system
} // namespace libbitcoin

#include <bitcoin/system/impl/stream/streamers/bit_writer.ipp>

#endif
