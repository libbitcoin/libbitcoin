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
#ifndef LIBBITCOIN_SYSTEM_STREAM_STREAMS_HPP
#define LIBBITCOIN_SYSTEM_STREAM_STREAMS_HPP

#include <string>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/stream/device.hpp>
#include <bitcoin/system/stream/devices/copy_sink.hpp>
#include <bitcoin/system/stream/devices/copy_source.hpp>
#include <bitcoin/system/stream/devices/flip_sink.hpp>
#include <bitcoin/system/stream/devices/push_sink.hpp>
#include <bitcoin/system/stream/make_stream.hpp>

namespace libbitcoin {
namespace system {
    
/// Streams are not copyable, so factory cannot be used for type inference.
/// All sources and sinks (devices) accept only const and non-const references.
/// Insert requires the container type, but there are only two (string/chunk).
/// Container.reserve() may improve performance for all push streaming.
/// push_sink is buffered/indirect (inefficient) and requires flush.
/// For better performance, use Container.resize() and use a copy_sink.
namespace stream
{
    namespace in
    {
        /// An input stream that moves data from a data_reference.
        using copy = make_stream<copy_source<data_reference>>;
    }

    namespace out
    {
        /// An output stream that copies data to a data_slab.
        using copy = make_stream<copy_sink<data_slab>>;

        /// An output stream that inserts data to a container.
        template <typename Container>
        using push = make_stream<push_sink<Container>>;
        using text = push<std::string>;
        using data = push<data_chunk>;
    }

    namespace flip
    {
        /// An input/output stream that copies data to a data_slab.
        using copy = make_stream<flip_sink<data_slab>>;
    }
}

} // namespace system
} // namespace libbitcoin

#endif
