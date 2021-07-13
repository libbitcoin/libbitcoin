/**
 * Copyright (c) 2011-2019 libbitcoin developers (see AUTHORS)
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
#ifndef LIBBITCOIN_SYSTEM_UNICODE_UTF8_EVERYWHERE_UTF8_STREAMBUF_HPP
#define LIBBITCOIN_SYSTEM_UNICODE_UTF8_EVERYWHERE_UTF8_STREAMBUF_HPP

#include <cstddef>
#include <streambuf>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {

/**
 * Class to translate internal utf8 iostreams to external utf16 iostreams.
 * This uses wide and narrow input and output buffers of 1024 characters/bytes.
 * However because of utf8-utf16 conversion ratios of up to 4:1 the effective
 * wide output buffering may be reduced to as much as 256 characters.
 */
class BC_API unicode_streambuf
  : public std::streambuf
{
public:
    /**
     * Construct unicode stream buffer from a weak reference to a wide buffer.
     * @param[in]  wide_buffer  A wide stream buffer for i/o relay.
     * @param[in]  size         The wide buffer size.
     */
    unicode_streambuf(std::wstreambuf* wide_buffer, size_t size);

    /**
     * Synchronize stream buffer.
     */
    virtual ~unicode_streambuf();

protected:
    /**
     * Implement underflow for support of input streams.
     */
    virtual std::streambuf::int_type underflow();

    /**
     * Implement overflow for support of output streams.
     * @param[in]  character  A single byte to be explicitly put.
     */
    virtual std::streambuf::int_type overflow(
        std::streambuf::int_type character);

    /**
     * Implement sync for support of output streams.
     */
    virtual int sync();

private:
    // The constructed wide buffer size in number of characters.
    size_t wide_size_;

    // The derived narrow buffer size in utf8 (bytes).
    size_t narrow_size_;

    // The dynamically-allocated buffers.
    wchar_t* wide_;
    char* narrow_;

    // The excapsulated wide streambuf.
    std::wstreambuf* wide_buffer_;
};

} // namespace system
} // namespace libbitcoin

#endif
