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
#ifndef LIBBITCOIN_SYSTEM_STREAM_MAKE_STREAMER_HPP
#define LIBBITCOIN_SYSTEM_STREAM_MAKE_STREAMER_HPP

#include <bitcoin/system/stream/make_stream.hpp>

namespace libbitcoin {
namespace system {

// Suppress multiple inheritance warnings.
// The inheritance is virtual, so not actually multiple.
// But the boost type constraint 'is_virtual_base_of' triggers the warning.
#pragma warning(push)
#pragma warning(disable : 4250)

/// Construct an output stream and feed it to a writer.
/// For std::ostream just pass to writer on construct.
template <typename Device,
    template <typename = make_stream<Device>> class Base,
    typename Stream = make_stream<Device>, typename Streamer = Base<Stream>>
class make_streamer
  : public Streamer
{
public:
    make_streamer(typename Device::container device) noexcept
      : stream_(device), Streamer(stream_)
    {
    }

    ~make_streamer() noexcept override
    {
        Streamer::~Streamer();
    }

protected:
    Stream stream_;
};

#pragma warning(pop)

} // namespace system
} // namespace libbitcoin

#endif
