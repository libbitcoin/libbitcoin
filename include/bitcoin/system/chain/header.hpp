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
#ifndef LIBBITCOIN_SYSTEM_CHAIN_HEADER_HPP
#define LIBBITCOIN_SYSTEM_CHAIN_HEADER_HPP

#include <cstddef>
#include <cstdint>
#include <istream>
#include <string>
#include <memory>
#include <vector>
#include <bitcoin/system/chain/block_filter.hpp>
#include <bitcoin/system/chain/chain_state.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/error.hpp>
#include <bitcoin/system/mutex.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {

class settings;

namespace chain {

class BC_API header
{
public:
    // THIS IS FOR LIBRARY USE ONLY, DO NOT CREATE A DEPENDENCY ON IT.
    struct validation
    {
        uint64_t originator = 0;
        chain_state::ptr state = nullptr;

        /// Stored in checksum field stored on header (when invalid).
        /// The block validation error code (if validated).
        code error = error::success;

        // Stored on header.
        /// Height also always exists, but we always pass it explicitly.
        /// The median time past of the header, derived from its ancestry.
        uint32_t median_time_past = 0;

        /// False if not found in store.
        /// Header exists, in any state (do not download it).
        bool exists = false;

        /// Derived from header transaction count (non-zero).
        /// Block transactions are populated (do not download block/txs).
        bool populated = false;

        /// Derived from state stored on header (valid or failed).
        /// Block has been validated (do not revalidate).
        bool validated = false;

        /// Derived from state stored on header.
        /// Header is in candidate state and referenced by the candidate index.
        bool candidate = false;

        /// Derived from state stored on header (no fork point considered).
        /// Block is in confirmed state and referenced by the confirmed index.
        bool confirmed = false;

        // Neutrino filter.
        block_filter::ptr neutrino_filter;
    };

    // Constructors.
    //-------------------------------------------------------------------------

    header();

    header(header&& other);
    header(const header& other);

    header(uint32_t version, const hash_digest& previous_block_hash,
        const hash_digest& merkle_root, uint32_t timestamp, uint32_t bits,
        uint32_t nonce);
    header(uint32_t version, hash_digest&& previous_block_hash,
        hash_digest&& merkle_root, uint32_t timestamp, uint32_t bits,
        uint32_t nonce);

    // Operators.
    //-------------------------------------------------------------------------

    header& operator=(header&& other);
    header& operator=(const header& other);

    bool operator==(const header& other) const;
    bool operator!=(const header& other) const;

    // Deserialization.
    //-------------------------------------------------------------------------

    static header factory(const data_chunk& data, bool wire=true);
    static header factory(std::istream& stream, bool wire=true);
    static header factory(reader& source, bool wire=true);
    static header factory(reader& source, hash_digest&& hash, bool wire=true);
    static header factory(reader& source, const hash_digest& hash,
        bool wire=true);

    bool from_data(const data_chunk& data, bool wire=true);
    bool from_data(std::istream& stream, bool wire=true);
    bool from_data(reader& source, bool wire=true);
    bool from_data(reader& source, hash_digest&& hash, bool wire=true);
    bool from_data(reader& source, const hash_digest& hash, bool wire=true);

    bool is_valid() const;

    // Serialization.
    //-------------------------------------------------------------------------

    data_chunk to_data(bool wire=true) const;
    void to_data(std::ostream& stream, bool wire=true) const;
    void to_data(writer& sink, bool wire=true) const;

    // Properties (size, accessors, cache).
    //-------------------------------------------------------------------------

    static size_t satoshi_fixed_size();
    size_t serialized_size(bool wire=true) const;

    uint32_t version() const;
    void set_version(uint32_t value);

    const hash_digest& previous_block_hash() const;
    void set_previous_block_hash(const hash_digest& value);
    void set_previous_block_hash(hash_digest&& value);

    /// This may not match the computed value, validation compares them.
    const hash_digest& merkle_root() const;
    void set_merkle_root(const hash_digest& value);
    void set_merkle_root(hash_digest&& value);

    uint32_t timestamp() const;
    void set_timestamp(uint32_t value);

    uint32_t bits() const;
    void set_bits(uint32_t value);

    uint32_t nonce() const;
    void set_nonce(uint32_t value);

    hash_digest hash() const;

    // Validation.
    //-------------------------------------------------------------------------

    uint256_t proof() const;
    static uint256_t proof(uint32_t bits);

    bool is_valid_timestamp(uint32_t timestamp_limit_seconds) const;
    bool is_valid_proof_of_work(uint32_t proof_of_work_limit,
        bool scrypt=false) const;

    code check(uint32_t timestamp_limit_seconds, uint32_t proof_of_work_limit,
        bool scrypt=false) const;
    code accept() const;
    code accept(const chain_state& state) const;

    // THIS IS FOR LIBRARY USE ONLY, DO NOT CREATE A DEPENDENCY ON IT.
    mutable validation metadata;

protected:
    // So that block may call reset from its own.
    friend class block;

    void reset();
    void invalidate_cache() const;

private:
    typedef std::shared_ptr<hash_digest> hash_ptr;

    hash_ptr hash_cache() const;

    mutable hash_ptr hash_;
    mutable upgrade_mutex mutex_;

    uint32_t version_;
    hash_digest previous_block_hash_;
    hash_digest merkle_root_;
    uint32_t timestamp_;
    uint32_t bits_;
    uint32_t nonce_;
};

} // namespace chain
} // namespace system
} // namespace libbitcoin

#endif
