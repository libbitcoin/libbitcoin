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
#ifndef LIBBITCOIN_SYSTEM_MESSAGE_FILTER_ADD_HPP
#define LIBBITCOIN_SYSTEM_MESSAGE_FILTER_ADD_HPP

#include <istream>
#include <memory>
#include <string>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

class BC_API filter_add
{
public:
    typedef std::shared_ptr<filter_add> ptr;
    typedef std::shared_ptr<const filter_add> const_ptr;

    static filter_add factory(uint32_t version, const data_chunk& data);
    static filter_add factory(uint32_t version, std::istream& stream);
    static filter_add factory(uint32_t version, reader& source);

    filter_add();
    filter_add(const data_chunk& data);
    filter_add(data_chunk&& data);
    filter_add(const filter_add& other);
    filter_add(filter_add&& other);

    data_chunk& data();
    const data_chunk& data() const;
    void set_data(const data_chunk& value);
    void set_data(data_chunk&& value);

    bool from_data(uint32_t version, const data_chunk& data);
    bool from_data(uint32_t version, std::istream& stream);
    bool from_data(uint32_t version, reader& source);
    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, std::ostream& stream) const;
    void to_data(uint32_t version, writer& sink) const;
    bool is_valid() const;
    void reset();
    size_t serialized_size(uint32_t version) const;

    // This class is move assignable but not copy assignable.
    filter_add& operator=(filter_add&& other);
    void operator=(const filter_add&) = delete;

    bool operator==(const filter_add& other) const;
    bool operator!=(const filter_add& other) const;

    static const std::string command;
    static const uint32_t version_minimum;
    static const uint32_t version_maximum;

private:
    data_chunk data_;
};

} // namespace message
} // namespace system
} // namespace libbitcoin

#endif
