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
#ifndef LIBBITCOIN_SYSTEM_MESSAGE_COMPACT_BLOCK_HPP
#define LIBBITCOIN_SYSTEM_MESSAGE_COMPACT_BLOCK_HPP

#include <istream>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/chain/chain.hpp>
#include <bitcoin/system/message/prefilled_transaction.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

class BC_API compact_block
{
public:
    typedef std::shared_ptr<compact_block> ptr;
    typedef std::shared_ptr<const compact_block> const_ptr;

    typedef mini_hash short_id;
    typedef mini_hash_list short_id_list;

    static compact_block factory(uint32_t version, const data_chunk& data);
    static compact_block factory(uint32_t version, std::istream& stream);
    static compact_block factory(uint32_t version, reader& source);

    compact_block();
    compact_block(const chain::header& header, uint64_t nonce,
        const short_id_list& short_ids,
        const prefilled_transaction::list& transactions);
    compact_block(chain::header&& header, uint64_t nonce,
        short_id_list&& short_ids,
        prefilled_transaction::list&& transactions);
    compact_block(const compact_block& other);
    compact_block(compact_block&& other);

    chain::header& header();
    const chain::header& header() const;
    void set_header(const chain::header& value);
    void set_header(chain::header&& value);

    uint64_t nonce() const;
    void set_nonce(uint64_t value);

    short_id_list& short_ids();
    const short_id_list& short_ids() const;
    void set_short_ids(const short_id_list& value);
    void set_short_ids(short_id_list&& value);

    prefilled_transaction::list& transactions();
    const prefilled_transaction::list& transactions() const;
    void set_transactions(const prefilled_transaction::list& value);
    void set_transactions(prefilled_transaction::list&& value);

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
    compact_block& operator=(compact_block&& other);
    void operator=(const compact_block&) = delete;

    bool operator==(const compact_block& other) const;
    bool operator!=(const compact_block& other) const;

    static const std::string command;
    static const uint32_t version_minimum;
    static const uint32_t version_maximum;

private:
    chain::header header_;
    uint64_t nonce_;
    short_id_list short_ids_;
    prefilled_transaction::list transactions_;
};

} // namespace message
} // namespace system
} // namespace libbitcoin

#endif
