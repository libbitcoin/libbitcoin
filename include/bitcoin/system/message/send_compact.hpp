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
#ifndef LIBBITCOIN_SYSTEM_MESSAGE_SEND_COMPACT_BLOCKS_HPP
#define LIBBITCOIN_SYSTEM_MESSAGE_SEND_COMPACT_BLOCKS_HPP

#include <istream>
#include <memory>
#include <string>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

class BC_API send_compact
{
public:
    typedef std::shared_ptr<send_compact> ptr;
    typedef std::shared_ptr<const send_compact> const_ptr;

    static send_compact factory(uint32_t version, const data_chunk& data);
    static send_compact factory(uint32_t version, std::istream& stream);
    static send_compact factory(uint32_t version, reader& source);
    static size_t satoshi_fixed_size(uint32_t version);

    send_compact();
    send_compact(bool high_bandwidth_mode, uint64_t version);
    send_compact(const send_compact& other);
    send_compact(send_compact&& other);

    bool high_bandwidth_mode() const;
    void set_high_bandwidth_mode(bool mode);

    uint64_t version() const;
    void set_version(uint64_t version);

    bool from_data(uint32_t version, const data_chunk& data);
    bool from_data(uint32_t version, std::istream& stream);
    bool from_data(uint32_t version, reader& source);
    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, std::ostream& stream) const;
    void to_data(uint32_t version, writer& sink) const;
    bool is_valid() const;
    void reset();
    size_t serialized_size(uint32_t version) const;

    /// This class is move assignable but not copy assignable.
    send_compact& operator=(send_compact&& other);
    void operator=(const send_compact&) = delete;

    bool operator==(const send_compact& other) const;
    bool operator!=(const send_compact& other) const;

    static const std::string command;
    static const uint32_t version_minimum;
    static const uint32_t version_maximum;

private:
    bool high_bandwidth_mode_;
    uint64_t version_;
};

} // namespace message
} // namespace system
} // namespace libbitcoin

#endif
