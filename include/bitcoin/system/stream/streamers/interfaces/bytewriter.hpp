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
#ifndef LIBBITCOIN_SYSTEM_STREAM_STREAMERS_INTERFACES_BYTEWRITER_HPP
#define LIBBITCOIN_SYSTEM_STREAM_STREAMERS_INTERFACES_BYTEWRITER_HPP

#include <cstddef>
#include <cstdint>
#include <string>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/constraints.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/error.hpp>

namespace libbitcoin {
namespace system {
    
/// A byte writer interface.
class bytewriter
{
public:
    /// Write big endian integers.
    virtual void write_2_bytes_big_endian(uint16_t value) noexcept = 0;
    virtual void write_4_bytes_big_endian(uint32_t value) noexcept = 0;
    virtual void write_8_bytes_big_endian(uint64_t value) noexcept = 0;

    /// Write little endian integers.
    virtual void write_2_bytes_little_endian(uint16_t value) noexcept = 0;
    virtual void write_4_bytes_little_endian(uint32_t value) noexcept = 0;
    virtual void write_8_bytes_little_endian(uint64_t value) noexcept = 0;

    /// Write Bitcoin variable integer (3, 5, or 9 bytes, little-endian).
    virtual void write_variable(uint64_t value) noexcept = 0;

    /// Call write_4_bytes_little_endian with integer value of error code.
    virtual void write_error_code(const code& ec) noexcept = 0;

    /// Write until reader is exhausted.
    virtual std::istream& write(std::istream& in) noexcept = 0;

    /// Write one byte.
    virtual void write_byte(uint8_t value) noexcept = 0;

    /// Write all bytes.
    virtual void write_bytes(const data_slice& data) noexcept = 0;

    /// Write size bytes.
    virtual void write_bytes(const uint8_t* data, size_t size) noexcept = 0;

    /// Write Bitcoin length-prefixed string (prefixed by write_variable).
    virtual void write_string(const std::string& value, size_t size) noexcept = 0;

    /// Write string to specified length, padded with nulls as required.
    virtual void write_string(const std::string& value) noexcept = 0;

    /// Flush the buffer.
    virtual void flush() noexcept = 0;

    /// The stream is valid.
    virtual operator bool() const noexcept = 0;

    /// The stream is invalid.
    virtual bool operator!() const noexcept = 0;
};

} // namespace system
} // namespace libbitcoin

#endif
