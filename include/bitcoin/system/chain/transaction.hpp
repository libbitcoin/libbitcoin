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
#ifndef LIBBITCOIN_SYSTEM_CHAIN_TRANSACTION_HPP
#define LIBBITCOIN_SYSTEM_CHAIN_TRANSACTION_HPP

#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>
#include <string>
#include <vector>
#include <bitcoin/system/chain/chain_state.hpp>
#include <bitcoin/system/chain/input.hpp>
#include <bitcoin/system/chain/output.hpp>
#include <bitcoin/system/chain/point.hpp>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/error.hpp>
#include <bitcoin/system/mutex.hpp>
#include <bitcoin/system/optional.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace chain {

class BC_API transaction
{
public:
    typedef std::vector<transaction> list;

    // THIS IS FOR LIBRARY USE ONLY, DO NOT CREATE A DEPENDENCY ON IT.
    struct validation
    {
        /// This is a non-consensus sentinel value.
        static const uint64_t unlinked;

        /// Set by node transaction in protocol.
        uint64_t originator = 0;

        /// Set by blockchain tx organizer.
        chain_state::ptr state = nullptr;

        /// Derived from hash table or block reference.
        /// Tx currently exists at the link (in any state).
        uint64_t link = unlinked;

        /// False if read from wire and not found in store.
        /// Tx existed before being written (in the current pass).
        bool existed = false;

        /// Stored on transaction.
        /// Tx is in a candidate chain block (and valid there).
        bool candidate = false;

        /// Derived from tx position (non-sentinel).
        /// Tx is in a confirmed chain block at given height (and valid there).
        bool confirmed = false;

        /// Derived from tx position (non-sentinel).
        /// Tx is not unconfirmed, i.e. has been confirmed once even
        /// if is deconfirmed now.
        bool cataloged = false;
        
        /// Derived from height (when not confirmed).
        /// There is no distiction between a tx that can be valid under some
        /// forks and one that cannot be valid under any forks. The only
        /// criteria for storage is deserialization and DoS protection. The
        /// latter is provided by pool validation or containing block PoW.
        /// This results in revalidation in the case where the tx may be 
        // confirmed again. If true tx is validated relative to fork point.
        bool verified = false;
    };

    // Constructors.
    //-------------------------------------------------------------------------

    transaction();

    transaction(transaction&& other);
    transaction(const transaction& other);

    transaction(uint32_t version, uint32_t locktime, input::list&& inputs,
        output::list&& outputs);
    transaction(uint32_t version, uint32_t locktime, const input::list& inputs,
        const output::list& outputs);

    // Operators.
    //-------------------------------------------------------------------------

    transaction& operator=(transaction&& other);
    transaction& operator=(const transaction& other);

    bool operator==(const transaction& other) const;
    bool operator!=(const transaction& other) const;

    // Deserialization.
    //-------------------------------------------------------------------------

    static transaction factory(const data_chunk& data, bool wire=true, bool witness=false);
    static transaction factory(std::istream& stream, bool wire=true, bool witness=false);
    static transaction factory(reader& source, bool wire=true, bool witness=false);

    // Non-wire store deserializations to preserve hash.
    static transaction factory(reader& source, hash_digest&& hash,bool wire=true, bool witness=false);
    static transaction factory(reader& source, const hash_digest& hash,bool wire=true, bool witness=false);

    bool from_data(const data_chunk& data, bool wire=true, bool witness=false);
    bool from_data(std::istream& stream, bool wire=true, bool witness=false);
    bool from_data(reader& source, bool wire=true, bool witness=false);

    // Non-wire store deserializations to preserve hash.
    bool from_data(reader& source, hash_digest&& hash, bool wire=true, bool witness=false);
    bool from_data(reader& source, const hash_digest& hash, bool wire=true, bool witness=false);

    bool is_valid() const;

    // Serialization.
    //-------------------------------------------------------------------------

    data_chunk to_data(bool wire=true, bool witness=false) const;
    void to_data(std::ostream& stream, bool wire=true, bool witness=false) const;
    void to_data(writer& sink, bool wire=true, bool witness=false) const;

    // Properties (size, accessors, cache).
    //-------------------------------------------------------------------------

    static size_t maximum_size(bool is_coinbase);
    size_t serialized_size(bool wire=true, bool witness=false) const;

    uint32_t version() const;
    void set_version(uint32_t value);

    uint32_t locktime() const;
    void set_locktime(uint32_t value);

    // Deprecated (unsafe).
    input::list& inputs();

    const input::list& inputs() const;
    void set_inputs(const input::list& value);
    void set_inputs(input::list&& value);

    // Deprecated (unsafe).
    output::list& outputs();

    const output::list& outputs() const;
    void set_outputs(const output::list& value);
    void set_outputs(output::list&& value);

    hash_digest outputs_hash() const;
    hash_digest inpoints_hash() const;
    hash_digest sequences_hash() const;
    hash_digest hash(bool witness=false) const;

    // Utilities.
    //-------------------------------------------------------------------------

    /// Clear witness from all inputs (does not change default hash).
    void strip_witness();

    // Validation.
    //-------------------------------------------------------------------------

    uint64_t fees() const;
    point::list previous_outputs() const;
    point::list missing_previous_outputs() const;
    hash_list missing_previous_transactions() const;
    uint64_t total_input_value() const;
    uint64_t total_output_value() const;
    size_t signature_operations() const;
    size_t signature_operations(bool bip16, bool bip141) const;
    size_t weight() const;

    bool is_coinbase() const;
    bool is_null_non_coinbase() const;
    bool is_oversized_coinbase() const;
    bool is_mature(size_t height) const;
    bool is_overspent() const;
    bool is_internal_double_spend() const;
    bool is_confirmed_double_spend() const;
    bool is_dusty(uint64_t minimum_output_value) const;
    bool is_missing_previous_outputs() const;
    bool is_final(size_t block_height, uint32_t block_time) const;
    bool is_locked(size_t block_height, uint32_t median_time_past) const;
    bool is_locktime_conflict() const;
    bool is_segregated() const;

    code check(uint64_t max_money, bool transaction_pool=true) const;
    code accept(bool transaction_pool=true) const;
    code accept(const chain_state& state, bool transaction_pool=true) const;
    code connect() const;
    code connect(const chain_state& state) const;
    code connect_input(const chain_state& state, size_t input_index) const;

    // THIS IS FOR LIBRARY USE ONLY, DO NOT CREATE A DEPENDENCY ON IT.
    mutable validation metadata;

protected:
    void reset();
    void invalidate_cache() const;
    bool all_inputs_final() const;

private:
    typedef std::shared_ptr<hash_digest> hash_ptr;

    hash_ptr hash_cache() const;
    optional64 total_input_value_cache() const;
    optional64 total_output_value_cache() const;

    uint32_t version_;
    uint32_t locktime_;
    input::list inputs_;
    output::list outputs_;

    // Hashing invokes to_data() which invokes is_segregated(), which is
    // guarded by value_mutex_, so hash and value must have distinct mutexes.
    // Hashing does not invoke hashing, and values do not invoke 

    // These share a mutex as they are not expected to contend.
    mutable hash_ptr hash_;
    mutable hash_ptr witness_hash_;
    mutable hash_ptr outputs_hash_;
    mutable hash_ptr inpoints_hash_;
    mutable hash_ptr sequences_hash_;
    mutable upgrade_mutex hash_mutex_;

    // These share a mutex as they are not expected to contend.
    mutable optional64 total_input_value_;
    mutable optional64 total_output_value_;
    mutable optional_flag segregated_;
    mutable upgrade_mutex value_mutex_;
};

} // namespace chain
} // namespace system
} // namespace libbitcoin

#endif
