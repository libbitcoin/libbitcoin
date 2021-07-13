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
#ifndef LIBBITCOIN_SYSTEM_MESSAGE_BLOCK_HPP
#define LIBBITCOIN_SYSTEM_MESSAGE_BLOCK_HPP

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

class BC_API block
  : public chain::block
{
public:
    typedef std::shared_ptr<block> ptr;
    typedef std::shared_ptr<const block> const_ptr;
    typedef std::vector<ptr> ptr_list;
    typedef std::vector<const_ptr> const_ptr_list;
    typedef std::shared_ptr<const_ptr_list> const_ptr_list_ptr;
    typedef std::shared_ptr<const const_ptr_list> const_ptr_list_const_ptr;

    static block factory(uint32_t version, const data_chunk& data);
    static block factory(uint32_t version, std::istream& stream);
    static block factory(uint32_t version, reader& source);

    block();

    block(block&& other);
    block(const block& other);

    block(chain::block&& other);
    block(const chain::block& other);

    block(chain::header&& header, chain::transaction::list&& transactions);
    block(const chain::header& header,
        const chain::transaction::list& transactions);

    bool from_data(uint32_t version, const data_chunk& data);
    bool from_data(uint32_t version, std::istream& stream);
    bool from_data(uint32_t version, reader& source);
    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, std::ostream& stream) const;
    void to_data(uint32_t version, writer& sink) const;
    size_t serialized_size(uint32_t version) const;

    block& operator=(chain::block&& other);

    // This class is move assignable but not copy assignable.
    block& operator=(block&& other);
    void operator=(const block&) = delete;

    bool operator==(const chain::block& other) const;
    bool operator!=(const chain::block& other) const;

    bool operator==(const block& other) const;
    bool operator!=(const block& other) const;

    static const std::string command;
    static const uint32_t version_minimum;
    static const uint32_t version_maximum;
};

} // namespace message
} // namespace system
} // namespace libbitcoin

#endif
