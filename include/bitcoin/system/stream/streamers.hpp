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
#ifndef LIBBITCOIN_SYSTEM_STREAM_STREAMERS_HPP
#define LIBBITCOIN_SYSTEM_STREAM_STREAMERS_HPP

#include <iostream>
#include <string>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/stream/binary.hpp>
#include <bitcoin/system/stream/device.hpp>
#include <bitcoin/system/stream/devices/copy_sink.hpp>
#include <bitcoin/system/stream/devices/copy_source.hpp>
#include <bitcoin/system/stream/devices/flip_sink.hpp>
#include <bitcoin/system/stream/devices/push_sink.hpp>
#include <bitcoin/system/stream/make_streamer.hpp>
#include <bitcoin/system/stream/streamers/bit_flipper.hpp>
#include <bitcoin/system/stream/streamers/bit_reader.hpp>
#include <bitcoin/system/stream/streamers/bit_writer.hpp>
#include <bitcoin/system/stream/streamers/byte_flipper.hpp>
#include <bitcoin/system/stream/streamers/byte_reader.hpp>
#include <bitcoin/system/stream/streamers/byte_writer.hpp>
#include <bitcoin/system/stream/streamers/interfaces/bitflipper.hpp>
#include <bitcoin/system/stream/streamers/interfaces/byteflipper.hpp>
#include <bitcoin/system/stream/streamers/interfaces/bitreader.hpp>
#include <bitcoin/system/stream/streamers/interfaces/bytereader.hpp>
#include <bitcoin/system/stream/streamers/interfaces/bitwriter.hpp>
#include <bitcoin/system/stream/streamers/interfaces/bytewriter.hpp>

namespace libbitcoin {
namespace system {
    
namespace read
{
    namespace bytes
    {
        /// A byte reader that reads data from a std::istream.
        using istream = byte_reader<std::istream>;

        /// A byte reader that copies data from a data_reference.
        using copy = make_streamer<copy_source<data_reference>, byte_reader>;
    }

    namespace bits
    {
        /// A bit reader that reads data from a std::istream.
        using istream = bit_reader<std::istream>;

        /// A bit reader that moves data from a data_reference.
        using copy = make_streamer<copy_source<data_reference>, bit_reader>;
    }
}

namespace write
{
    namespace bytes
    {
        /// A byte writer that writes data to a std::ostream.
        using ostream = byte_writer<std::ostream>;

        /// A byte writer that copies data to a data_slab.
        using copy = make_streamer<copy_sink<data_slab>, byte_writer>;

        /// A byte writer that inserts data into a container.
        template <typename Container>
        using push = make_streamer<push_sink<Container>, byte_writer>;
        using text = push<std::string>;
        using data = push<data_chunk>;
    }

    namespace bits
    {
        /// A bit writer that writes data to a std::ostream.
        using ostream = bit_writer<std::ostream>;

        /// A bit writer that copies data to a data_slab.
        using copy = make_streamer<copy_sink<data_slab>, bit_writer>;

        /// A bit writer that inserts data into a container.
        template <typename Container>
        using push = make_streamer<push_sink<Container>, bit_writer>;
        using text = push<std::string>;
        using data = push<data_chunk>;
    }
}

namespace flip
{
    namespace bytes
    {
        /// A byte reader/writer of a std::iostream.
        using iostream = byte_flipper<std::iostream>;

        /// A byte reader/writer of a data_slab (no push and requires own sink).
        using copy = make_streamer<flip_sink<data_slab>, byte_flipper>;
    }

    namespace bits
    {
        /// A bit reader/writer of a std::iostream.
        using iostream = bit_flipper<std::iostream>;

        /// A bit reader/writer of a data_slab (no push and requires own sink).
        using copy = make_streamer<flip_sink<data_slab>, bit_flipper>;
    }
}

/// Interface byte aliases.
using flipper = byteflipper;
using reader = bytereader;
using writer = bytewriter;

} // namespace system
} // namespace libbitcoin

#endif
