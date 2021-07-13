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

#ifndef LIBBITCOIN_SYSTEM_CHAIN_BLOCK_FILTER_HPP
#define LIBBITCOIN_SYSTEM_CHAIN_BLOCK_FILTER_HPP

#include <istream>
#include <memory>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/math/math.hpp>

namespace libbitcoin {
namespace system {
namespace chain {

class BC_API block_filter
{
public:
    typedef std::shared_ptr<block_filter> ptr;
    typedef std::shared_ptr<const block_filter> const_ptr;

    // THIS IS FOR LIBRARY USE ONLY, DO NOT CREATE A DEPENDENCY ON IT.
    struct validation
    {
        /// This is a non-consensus sentinel value.
        static const uint64_t unlinked;

        /// Derived from hash table or block reference.
        /// Filter currently exists at the link (in any state).
        uint64_t link = unlinked;
    };

    // Constructors.
    //-------------------------------------------------------------------------

    block_filter();

    block_filter(block_filter&& other);
    block_filter(const block_filter& other);

    block_filter(uint8_t filter_type, hash_digest&& block_hash,
        hash_digest&& header, data_chunk&& filter);
    block_filter(uint8_t filter_type, const hash_digest& block_hash,
        const hash_digest& header, const data_chunk& filter);

    // Operators.
    //-------------------------------------------------------------------------

    /// This class is move assignable but not copy assignable (performance).
    block_filter& operator=(block_filter&& other);
    void operator=(const block_filter&) = delete;

    bool operator==(const block_filter& other) const;
    bool operator!=(const block_filter& other) const;

    // Properties (size, accessors, cache).
    //-------------------------------------------------------------------------

    uint8_t filter_type() const;
    void set_filter_type(uint8_t value);

    const hash_digest& block_hash() const;
    void set_block_hash(const hash_digest& value);
    void set_block_hash(hash_digest&& value);

    const hash_digest& header() const;
    void set_header(const hash_digest& value);
    void set_header(hash_digest&& value);

    const data_chunk& filter() const;
    void set_filter(const data_chunk& value);
    void set_filter(data_chunk&& value);

    // Validation.
    //-------------------------------------------------------------------------

    bool is_valid() const;
    void reset();

    // THIS IS FOR LIBRARY USE ONLY, DO NOT CREATE A DEPENDENCY ON IT.
    mutable validation metadata;

private:
    uint8_t filter_type_;
    hash_digest block_hash_;
    hash_digest header_;
    data_chunk filter_;
};

} // namespace chain
} // namespace system
} // namespace libbitcoin

#endif
