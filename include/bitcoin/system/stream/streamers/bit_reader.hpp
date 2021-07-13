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

#ifndef LIBBITCOIN_SYSTEM_STREAM_STREAMERS_BIT_READER_HPP
#define LIBBITCOIN_SYSTEM_STREAM_STREAMERS_BIT_READER_HPP

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <bitcoin/system/stream/streamers/byte_reader.hpp>
#include <bitcoin/system/stream/streamers/interfaces/bitreader.hpp>

namespace libbitcoin {
namespace system {

// Suppress multiple inheritance warnings.
// The inheritance is virtual, so not actually multiple.
// But the boost type constraint 'is_virtual_base_of' triggers the warning.
#pragma warning(push)
#pragma warning(disable : 4250)

/// A bit reader that accepts an istream.
template <typename IStream = std::istream>
class bit_reader
  : public byte_reader<IStream>,
    public virtual bitreader
{
public:
    /// Constructors.
    bit_reader(IStream& source) noexcept;
    ~bit_reader() noexcept override;

    /// Read one bit (high to low).
    virtual bool read_bit() noexcept;

    /// Read size bits into an integer (high to low).
    virtual uint64_t read_bits(size_t bits) noexcept;

    /// Advance the iterator.
    virtual void skip_bit() noexcept;
    virtual void skip_bits(size_t bits) noexcept;

    /// Rewind the iterator.
    virtual void rewind_bit() noexcept;
    virtual void rewind_bits(size_t bits) noexcept;

protected:
    uint8_t do_peek_byte() noexcept override;
    void do_read_bytes(uint8_t* buffer, size_t size) noexcept override;
    void do_skip_bytes(size_t size) noexcept override;
    void do_rewind_bytes(size_t size) noexcept override;
    bool get_exhausted() const noexcept override;

private:
    static const uint8_t pad;

    void load() noexcept;
    void reload() noexcept;
    uint8_t peek() noexcept;
    constexpr uint8_t shift() const noexcept;

    uint8_t byte_;
    uint8_t offset_;
};

#pragma warning(pop)

} // namespace system
} // namespace libbitcoin

#include <bitcoin/system/impl/stream/streamers/bit_reader.ipp>

#endif
