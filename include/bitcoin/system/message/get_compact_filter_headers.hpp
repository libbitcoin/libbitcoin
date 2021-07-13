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

// Sponsored in part by Digital Contract Design, LLC

#ifndef LIBBITCOIN_SYSTEM_MESSAGE_GET_COMPACT_FILTER_HEADERS_HPP
#define LIBBITCOIN_SYSTEM_MESSAGE_GET_COMPACT_FILTER_HEADERS_HPP

#include <istream>
#include <memory>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

class BC_API get_compact_filter_headers
{
public:
    typedef std::shared_ptr<get_compact_filter_headers> ptr;
    typedef std::shared_ptr<const get_compact_filter_headers> const_ptr;

    static get_compact_filter_headers factory(uint32_t version,
        const data_chunk& data);
    static get_compact_filter_headers factory(uint32_t version,
        std::istream& stream);
    static get_compact_filter_headers factory(uint32_t version,
        reader& source);

    get_compact_filter_headers();
    get_compact_filter_headers(uint8_t filter_type, uint32_t start_height,
        const hash_digest& stop_hash);
    get_compact_filter_headers(uint8_t filter_type, uint32_t start_height,
        hash_digest&& stop_hash);
    get_compact_filter_headers(const get_compact_filter_headers& other);
    get_compact_filter_headers(get_compact_filter_headers&& other);

    uint8_t filter_type() const;
    void set_filter_type(uint8_t value);

    uint32_t start_height() const;
    void set_start_height(uint32_t value);

    hash_digest& stop_hash();
    const hash_digest& stop_hash() const;
    void set_stop_hash(const hash_digest& value);
    void set_stop_hash(hash_digest&& value);

    bool from_data(uint32_t version, const data_chunk& data);
    bool from_data(uint32_t version, std::istream& stream);
    bool from_data(uint32_t version, reader& source);
    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, std::ostream& stream) const;
    void to_data(uint32_t version, writer& sink) const;
    bool is_valid() const;
    void reset();

    static size_t satoshi_fixed_size();
    size_t serialized_size(uint32_t version) const;

    // This class is move assignable but not copy assignable.
    get_compact_filter_headers& operator=(get_compact_filter_headers&& other);
    void operator=(const get_compact_filter_headers&) = delete;

    bool operator==(const get_compact_filter_headers& other) const;
    bool operator!=(const get_compact_filter_headers& other) const;

    static const std::string command;
    static const uint32_t version_minimum;
    static const uint32_t version_maximum;

private:
    uint8_t filter_type_;
    uint32_t start_height_;
    hash_digest stop_hash_;
};

} // namespace message
} // namespace system
} // namespace libbitcoin

#endif
