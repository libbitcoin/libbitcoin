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

#ifndef LIBBITCOIN_SYSTEM_MESSAGE_COMPACT_FILTER_CHECKPOINT_HPP
#define LIBBITCOIN_SYSTEM_MESSAGE_COMPACT_FILTER_CHECKPOINT_HPP

#include <istream>
#include <memory>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

class BC_API compact_filter_checkpoint
{
public:
    typedef std::shared_ptr<compact_filter_checkpoint> ptr;
    typedef std::shared_ptr<const compact_filter_checkpoint> const_ptr;

    static compact_filter_checkpoint factory(uint32_t version,
        const data_chunk& data);
    static compact_filter_checkpoint factory(uint32_t version,
        std::istream& stream);
    static compact_filter_checkpoint factory(uint32_t version,
        reader& source);

    compact_filter_checkpoint();
    compact_filter_checkpoint(uint8_t filter_type,
        const hash_digest& stop_hash, const hash_list& filter_headers);
    compact_filter_checkpoint(uint8_t filter_type, hash_digest&& stop_hash,
        hash_list&& filter_headers);
    compact_filter_checkpoint(const compact_filter_checkpoint& other);
    compact_filter_checkpoint(compact_filter_checkpoint&& other);

    uint8_t filter_type() const;
    void set_filter_type(uint8_t value);

    hash_digest& stop_hash();
    const hash_digest& stop_hash() const;
    void set_stop_hash(const hash_digest& value);
    void set_stop_hash(hash_digest&& value);

    hash_list& filter_headers();
    const hash_list& filter_headers() const;
    void set_filter_headers(const hash_list& value);
    void set_filter_headers(hash_list&& value);

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
    compact_filter_checkpoint& operator=(compact_filter_checkpoint&& other);
    void operator=(const compact_filter_checkpoint&) = delete;

    bool operator==(const compact_filter_checkpoint& other) const;
    bool operator!=(const compact_filter_checkpoint& other) const;

    static const std::string command;
    static const uint32_t version_minimum;
    static const uint32_t version_maximum;

private:
    uint8_t filter_type_;
    hash_digest stop_hash_;
    hash_list filter_headers_;
};

} // namespace message
} // namespace system
} // namespace libbitcoin

#endif
