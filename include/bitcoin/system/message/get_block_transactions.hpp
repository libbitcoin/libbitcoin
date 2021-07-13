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
#ifndef LIBBITCOIN_SYSTEM_MESSAGE_GET_BLOCK_TRANSACTIONS_HPP
#define LIBBITCOIN_SYSTEM_MESSAGE_GET_BLOCK_TRANSACTIONS_HPP

#include <istream>
#include <memory>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

class BC_API get_block_transactions
{
public:
    typedef std::shared_ptr<get_block_transactions> ptr;
    typedef std::shared_ptr<const get_block_transactions> const_ptr;

    static get_block_transactions factory(uint32_t version, const data_chunk& data);
    static get_block_transactions factory(uint32_t version, std::istream& stream);
    static get_block_transactions factory(uint32_t version, reader& source);

    get_block_transactions();
    get_block_transactions(const hash_digest& block_hash,
        const std::vector<uint64_t>& indexes);
    get_block_transactions(hash_digest&& block_hash,
        std::vector<uint64_t>&& indexes);
    get_block_transactions(const get_block_transactions& other);
    get_block_transactions(get_block_transactions&& other);

    hash_digest& block_hash();
    const hash_digest& block_hash() const;
    void set_block_hash(const hash_digest& value);
    void set_block_hash(hash_digest&& value);

    std::vector<uint64_t>& indexes();
    const std::vector<uint64_t>& indexes() const;
    void set_indexes(const std::vector<uint64_t>& values);
    void set_indexes(std::vector<uint64_t>&& values);

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
    get_block_transactions& operator=(get_block_transactions&& other);
    void operator=(const get_block_transactions&) = delete;

    bool operator==(const get_block_transactions& other) const;
    bool operator!=(const get_block_transactions& other) const;

    static const std::string command;
    static const uint32_t version_minimum;
    static const uint32_t version_maximum;

private:
    hash_digest block_hash_;
    std::vector<uint64_t> indexes_;
};

} // namespace message
} // namespace system
} // namespace libbitcoin

#endif
