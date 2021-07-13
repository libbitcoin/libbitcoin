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
#ifndef LIBBITCOIN_SYSTEM_MESSAGE_MERKLE_BLOCK_HPP
#define LIBBITCOIN_SYSTEM_MESSAGE_MERKLE_BLOCK_HPP

#include <istream>
#include <memory>
#include <string>
#include <bitcoin/system/chain/chain.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

class BC_API merkle_block
{
public:
    typedef std::vector<merkle_block> list;
    typedef std::shared_ptr<merkle_block> ptr;
    typedef std::shared_ptr<const merkle_block> const_ptr;

    static merkle_block factory(uint32_t version, const data_chunk& data);
    static merkle_block factory(uint32_t version, std::istream& stream);
    static merkle_block factory(uint32_t version, reader& source);

    merkle_block();
    merkle_block(const chain::header& header, size_t total_transactions,
        const hash_list& hashes, const data_chunk& flags);
    merkle_block(chain::header&& header, size_t total_transactions,
        hash_list&& hashes, data_chunk&& flags);
    merkle_block(const chain::block& block);
    merkle_block(const merkle_block& other);
    merkle_block(merkle_block&& other);

    chain::header& header();
    const chain::header& header() const;
    void set_header(const chain::header& value);
    void set_header(chain::header&& value);

    size_t total_transactions() const;
    void set_total_transactions(size_t value);

    hash_list& hashes();
    const hash_list& hashes() const;
    void set_hashes(const hash_list& value);
    void set_hashes(hash_list&& value);

    data_chunk& flags();
    const data_chunk& flags() const;
    void set_flags(const data_chunk& value);
    void set_flags(data_chunk&& value);

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
    merkle_block& operator=(merkle_block&& other);
    void operator=(const merkle_block&) = delete;

    bool operator==(const merkle_block& other) const;
    bool operator!=(const merkle_block& other) const;

    static const std::string command;
    static const uint32_t version_minimum;
    static const uint32_t version_maximum;

private:
    chain::header header_;
    size_t total_transactions_;
    hash_list hashes_;
    data_chunk flags_;
};

} // namespace message
} // namespace system
} // namespace libbitcoin

#endif
