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

#ifndef LIBBITCOIN_SYSTEM_MESSAGE_COMPACT_FILTER_HEADERS_HPP
#define LIBBITCOIN_SYSTEM_MESSAGE_COMPACT_FILTER_HEADERS_HPP

#include <istream>
#include <memory>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

class BC_API compact_filter_headers
{
public:
    typedef std::shared_ptr<compact_filter_headers> ptr;
    typedef std::shared_ptr<const compact_filter_headers> const_ptr;

    static compact_filter_headers factory(uint32_t version,
        const data_chunk& data);
    static compact_filter_headers factory(uint32_t version,
        std::istream& stream);
    static compact_filter_headers factory(uint32_t version,
        reader& source);

    compact_filter_headers();
    compact_filter_headers(uint8_t filter_type, const hash_digest& stop_hash,
        const hash_digest& previous_filter_header,
        const hash_list& filter_hashes);
    compact_filter_headers(uint8_t filter_type, hash_digest&& stop_hash,
        hash_digest&& previous_filter_header, hash_list&& filter_hashes);
    compact_filter_headers(const compact_filter_headers& other);
    compact_filter_headers(compact_filter_headers&& other);

    uint8_t filter_type() const;
    void set_filter_type(uint8_t value);

    hash_digest& stop_hash();
    const hash_digest& stop_hash() const;
    void set_stop_hash(const hash_digest& value);
    void set_stop_hash(hash_digest&& value);

    hash_digest& previous_filter_header();
    const hash_digest& previous_filter_header() const;
    void set_previous_filter_header(const hash_digest& value);
    void set_previous_filter_header(hash_digest&& value);

    hash_list& filter_hashes();
    const hash_list& filter_hashes() const;
    void set_filter_hashes(const hash_list& value);
    void set_filter_hashes(hash_list&& value);

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
    compact_filter_headers& operator=(compact_filter_headers&& other);
    void operator=(const compact_filter_headers&) = delete;

    bool operator==(const compact_filter_headers& other) const;
    bool operator!=(const compact_filter_headers& other) const;

    static const std::string command;
    static const uint32_t version_minimum;
    static const uint32_t version_maximum;

private:
    uint8_t filter_type_;
    hash_digest stop_hash_;
    hash_digest previous_filter_header_;
    hash_list filter_hashes_;
};

} // namespace message
} // namespace system
} // namespace libbitcoin

#endif
