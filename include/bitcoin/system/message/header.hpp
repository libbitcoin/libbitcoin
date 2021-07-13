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
#ifndef LIBBITCOIN_SYSTEM_MESSAGE_HEADER_HPP
#define LIBBITCOIN_SYSTEM_MESSAGE_HEADER_HPP

#include <cstdint>
#include <cstddef>
#include <istream>
#include <memory>
#include <bitcoin/system/chain/chain.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

class BC_API header
  : public chain::header
{
public:
    typedef std::vector<header> list;
    typedef std::shared_ptr<header> ptr;
    typedef std::vector<ptr> ptr_list;
    typedef std::shared_ptr<const header> const_ptr;
    typedef std::vector<const_ptr> const_ptr_list;
    typedef std::shared_ptr<const_ptr_list> const_ptr_list_ptr;
    typedef std::shared_ptr<const const_ptr_list> const_ptr_list_const_ptr;

    static header factory(uint32_t version, const data_chunk& data);
    static header factory(uint32_t version, std::istream& stream);
    static header factory(uint32_t version, reader& source);
    static size_t satoshi_fixed_size(uint32_t version);

    header();
    header(uint32_t version, const hash_digest& previous_block_hash,
        const hash_digest& merkle, uint32_t timestamp, uint32_t bits,
        uint32_t nonce);
    header(uint32_t version, hash_digest&& previous_block_hash,
        hash_digest&& merkle, uint32_t timestamp, uint32_t bits,
        uint32_t nonce);
    header(const chain::header& other);
    header(chain::header&& other);
    header(const header& other);
    header(header&& other);

    bool from_data(uint32_t version, const data_chunk& data);
    bool from_data(uint32_t version, std::istream& stream);
    bool from_data(uint32_t version, reader& source);
    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, std::ostream& stream) const;
    void to_data(uint32_t version, writer& sink) const;
    void reset();
    size_t serialized_size(uint32_t version) const;

    header& operator=(chain::header&& other);

    /// This class is move assignable but not copy assignable.
    header& operator=(header&& other);
    header& operator=(const header&) /*= delete*/;

    bool operator==(const chain::header& other) const;
    bool operator!=(const chain::header& other) const;

    bool operator==(const header& other) const;
    bool operator!=(const header& other) const;

    static const std::string command;
    static const uint32_t version_minimum;
    static const uint32_t version_maximum;
};

} // namespace message
} // namespace system
} // namespace libbitcoin

#endif
