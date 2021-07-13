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
#ifndef LIBBITCOIN_SYSTEM_UNICODE_UTF8_EVERYWHERE_UTF8_CONSOLE_STREAMBUF_HPP
#define LIBBITCOIN_SYSTEM_UNICODE_UTF8_EVERYWHERE_UTF8_CONSOLE_STREAMBUF_HPP

#include <cstddef>
#include <streambuf>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {

/**
 * Class to patch Windows stdin keyboard input, file input is not a problem.
 * This class and members are no-ops when called in non-MSVC++ builds.
 * When working in Windows console set font to "Lucida Console".
 */
class BC_API console_streambuf
  : public std::wstreambuf
{
public:
    /**
     * Initialize stdio to use utf8 translation on Windows.
     * @param[in]  size  The stream buffer size.
     */
    static void initialize(size_t size);

protected:
    /**
     * Protected construction, use static initialize method.
     * @param[in]  stream_buffer  The stream buffer to patch.
     * @param[in]  size           The stream buffer size.
     */
    console_streambuf(const std::wstreambuf& stream_buffer, size_t size);

    /**
     * Delete stream buffer.
     */
    virtual ~console_streambuf();

    /**
     * Implement alternate console read.
     * @param[in]  buffer  Pointer to the buffer to fill with console reads.
     * @param[in]  size    The size of the buffer that may be populated.
     */
    virtual std::streamsize xsgetn(wchar_t* buffer, std::streamsize size);

    /**
     * Implement alternate console read.
     */
    virtual std::wstreambuf::int_type underflow();

#ifdef _MSC_VER
private:
    // The constructed buffer size.
    size_t buffer_size_;

    // The dynamically-allocated buffers.
    wchar_t* buffer_;
#endif
};

} // namespace system
} // namespace libbitcoin

#endif
