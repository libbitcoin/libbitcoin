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
#ifndef LIBBITCOIN_SYSTEM_MESSAGE_BLOCK_TRANSACTIONS_HPP
#define LIBBITCOIN_SYSTEM_MESSAGE_BLOCK_TRANSACTIONS_HPP

#include <istream>
#include <bitcoin/system/chain/chain.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

class BC_API block_transactions
{
public:
    typedef std::shared_ptr<block_transactions> ptr;
    typedef std::shared_ptr<const block_transactions> const_ptr;

    static block_transactions factory(uint32_t version, const data_chunk& data);
    static block_transactions factory(uint32_t version, std::istream& stream);
    static block_transactions factory(uint32_t version, reader& source);

    block_transactions();
    block_transactions(const hash_digest& block_hash,
        const chain::transaction::list& transactions);
    block_transactions(hash_digest&& block_hash,
        chain::transaction::list&& transactions);
    block_transactions(const block_transactions& other);
    block_transactions(block_transactions&& other);

    hash_digest& block_hash();
    const hash_digest& block_hash() const;
    void set_block_hash(const hash_digest& value);
    void set_block_hash(hash_digest&& value);

    chain::transaction::list& transactions();
    const chain::transaction::list& transactions() const;
    void set_transactions(const chain::transaction::list& other);
    void set_transactions(chain::transaction::list&& other);

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
    block_transactions& operator=(block_transactions&& other);
    void operator=(const block_transactions&) = delete;

    bool operator==(const block_transactions& other) const;
    bool operator!=(const block_transactions& other) const;

    static const std::string command;
    static const uint32_t version_minimum;
    static const uint32_t version_maximum;

private:
    hash_digest block_hash_;
    chain::transaction::list transactions_;
};

} // namespace message
} // namespace system
} // namespace libbitcoin

#endif
