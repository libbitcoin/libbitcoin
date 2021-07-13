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
#ifndef LIBBITCOIN_SYSTEM_MESSAGE_FILTER_LOAD_HPP
#define LIBBITCOIN_SYSTEM_MESSAGE_FILTER_LOAD_HPP

#include <istream>
#include <memory>
#include <string>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

class BC_API filter_load
{
public:
    typedef std::shared_ptr<filter_load> ptr;
    typedef std::shared_ptr<const filter_load> const_ptr;

    static filter_load factory(uint32_t version, const data_chunk& data);
    static filter_load factory(uint32_t version, std::istream& stream);
    static filter_load factory(uint32_t version, reader& source);

    filter_load();
    filter_load(const data_chunk& filter, uint32_t hash_functions,
        uint32_t tweak, uint8_t flags);
    filter_load(data_chunk&& filter, uint32_t hash_functions,
        uint32_t tweak, uint8_t flags);
    filter_load(const filter_load& other);
    filter_load(filter_load&& other);

    data_chunk& filter();
    const data_chunk& filter() const;
    void set_filter(const data_chunk& value);
    void set_filter(data_chunk&& value);

    uint32_t hash_functions() const;
    void set_hash_functions(uint32_t value);

    uint32_t tweak() const;
    void set_tweak(uint32_t value);

    uint8_t flags() const;
    void set_flags(uint8_t value);

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
    filter_load& operator=(filter_load&& other);
    void operator=(const filter_load&) = delete;

    bool operator==(const filter_load& other) const;
    bool operator!=(const filter_load& other) const;

    static const std::string command;
    static const uint32_t version_minimum;
    static const uint32_t version_maximum;

private:
    data_chunk filter_;
    uint32_t hash_functions_;
    uint32_t tweak_;
    uint8_t flags_;
};

} // namespace message
} // namespace system
} // namespace libbitcoin

#endif
