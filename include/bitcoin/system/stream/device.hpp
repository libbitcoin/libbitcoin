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
#ifndef LIBBITCOIN_SYSTEM_STREAM_DEVICE_HPP
#define LIBBITCOIN_SYSTEM_STREAM_DEVICE_HPP

#include <iostream>
#include <utility>
#include <boost/iostreams/stream.hpp>

namespace libbitcoin {
namespace system {
    
namespace ios = boost::iostreams;

/// Virtual base class for ios::stream devices.
template <typename Container>
class device
{
public:
    /// device types
    typedef char char_type;
    typedef std::streamsize size_type;
    typedef typename Container::value_type value_type;

    /// seekable types
    typedef std::streampos position;
    typedef std::ios_base::seekdir direction;
    typedef std::iostream::off_type offset;
    typedef std::pair<char_type*, char_type*> sequence;

    /// seekable input/output (direct devices only).
    sequence input_sequence() const noexcept;
    sequence output_sequence() const noexcept;

    /// input/output devices (indirect devices only).
    size_type read(char_type* buffer, size_type count) noexcept;
    size_type write(const char_type* buffer, size_type count) noexcept;

    /// Buffer allocation (indirect devices only).
    size_type optimal_buffer_size() const noexcept;

protected:
    device(size_type remaining) noexcept;
    virtual sequence do_sequence() const noexcept;
    virtual void do_read(value_type* to, size_type size) noexcept;
    virtual void do_write(const value_type* from, size_type size) noexcept;
    virtual size_type do_optimal_buffer_size() const noexcept;

    size_type remaining_;
};

} // namespace system
} // namespace libbitcoin

#include <bitcoin/system/impl/stream/device.ipp>

#endif
