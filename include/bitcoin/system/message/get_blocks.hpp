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
#ifndef LIBBITCOIN_SYSTEM_MESSAGE_GET_BLOCKS_HPP
#define LIBBITCOIN_SYSTEM_MESSAGE_GET_BLOCKS_HPP

#include <cstddef>
#include <istream>
#include <memory>
#include <string>
#include <vector>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

class BC_API get_blocks
{
public:
    typedef std::vector<size_t> indexes;
    typedef std::shared_ptr<get_blocks> ptr;
    typedef std::shared_ptr<const get_blocks> const_ptr;

    static get_blocks factory(uint32_t version, const data_chunk& data);
    static get_blocks factory(uint32_t version, std::istream& stream);
    static get_blocks factory(uint32_t version, reader& source);

    static size_t locator_size(size_t top);
    static indexes locator_heights(size_t top);

    get_blocks();
    get_blocks(const hash_list& start, const hash_digest& stop);
    get_blocks(hash_list&& start, hash_digest&& stop);
    get_blocks(const get_blocks& other);
    get_blocks(get_blocks&& other);

    hash_list& start_hashes();
    const hash_list& start_hashes() const;
    void set_start_hashes(const hash_list& value);
    void set_start_hashes(hash_list&& value);

    hash_digest& stop_hash();
    const hash_digest& stop_hash() const;
    void set_stop_hash(const hash_digest& value);
    void set_stop_hash(hash_digest&& value);

    virtual bool from_data(uint32_t version, const data_chunk& data);
    virtual bool from_data(uint32_t version, std::istream& stream);
    virtual bool from_data(uint32_t version, reader& source);
    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, std::ostream& stream) const;
    void to_data(uint32_t version, writer& sink) const;
    bool is_valid() const;
    void reset();
    size_t serialized_size(uint32_t version) const;

    // This class is move assignable but not copy assignable.
    get_blocks& operator=(get_blocks&& other);
    void operator=(const get_blocks&) = delete;

    bool operator==(const get_blocks& other) const;
    bool operator!=(const get_blocks& other) const;

    static const std::string command;
    static const uint32_t version_minimum;
    static const uint32_t version_maximum;

private:
    // 10 sequential hashes, then exponential samples until reaching genesis.
    hash_list start_hashes_;
    hash_digest stop_hash_;
};

} // namespace message
} // namespace system
} // namespace libbitcoin

#endif
