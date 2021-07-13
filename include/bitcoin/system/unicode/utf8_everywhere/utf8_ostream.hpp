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
#ifndef LIBBITCOIN_SYSTEM_UNICODE_UTF8_EVERYWHERE_UTF8_OSTREAM_HPP
#define LIBBITCOIN_SYSTEM_UNICODE_UTF8_EVERYWHERE_UTF8_OSTREAM_HPP

#include <cstddef>
#include <iostream>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {

/**
 * Class to expose a widening output stream.
 */
class BC_API unicode_ostream
  : public std::ostream
{
public:
    /**
     * Construct instance of a conditionally-widening output stream.
     * @param[in]  narrow_stream  A narrow output stream such as std::cout.
     * @param[in]  wide_stream    A wide output stream such as std::wcout.
     * @param[in]  size           The wide buffer size.
     */
    unicode_ostream(std::ostream& narrow_stream, std::wostream& wide_stream,
        size_t size);

    /**
     * Delete the unicode_streambuf that wraps wide_stream.
     */
    virtual ~unicode_ostream();
};

} // namespace system
} // namespace libbitcoin

#endif
