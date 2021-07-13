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
#ifndef LIBBITCOIN_SYSTEM_CHAIN_INPUT_HPP
#define LIBBITCOIN_SYSTEM_CHAIN_INPUT_HPP

#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>
#include <vector>
#include <bitcoin/system/chain/output_point.hpp>
#include <bitcoin/system/chain/script.hpp>
#include <bitcoin/system/chain/witness.hpp>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/mutex.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace chain {

class BC_API input
{
public:
    typedef std::vector<input> list;

    // Constructors.
    //-------------------------------------------------------------------------

    input();

    input(input&& other);
    input(const input& other);

    input(output_point&& previous_output, chain::script&& script,
        uint32_t sequence);

    input(const output_point& previous_output, const chain::script& script,
        uint32_t sequence);

    input(output_point&& previous_output, chain::script&& script,
        chain::witness&& witness, uint32_t sequence);
    input(const output_point& previous_output, const chain::script& script,
        const chain::witness& witness, uint32_t sequence);

    // Operators.
    //-------------------------------------------------------------------------

    input& operator=(input&& other);
    input& operator=(const input& other);

    bool operator==(const input& other) const;
    bool operator!=(const input& other) const;

    // Deserialization.
    //-------------------------------------------------------------------------

    static input factory(const data_chunk& data, bool wire=true, bool witness=false);
    static input factory(std::istream& stream, bool wire=true, bool witness=false);
    static input factory(reader& source, bool wire=true, bool witness=false);

    bool from_data(const data_chunk& data, bool wire=true, bool witness=false);
    bool from_data(std::istream& stream, bool wire=true, bool witness=false);
    bool from_data(reader& source, bool wire=true, bool witness=false);

    bool is_valid() const;

    // Serialization.
    //-------------------------------------------------------------------------

    data_chunk to_data(bool wire=true, bool witness=false) const;
    void to_data(std::ostream& stream, bool wire=true, bool witness=false) const;
    void to_data(writer& sink, bool wire=true, bool witness=false) const;

    // Properties (size, accessors, cache).
    //-------------------------------------------------------------------------

    /// This accounts for wire witness, but does not read or write it.
    size_t serialized_size(bool wire=true, bool witness=false) const;

    output_point& previous_output();
    const output_point& previous_output() const;
    void set_previous_output(const output_point& value);
    void set_previous_output(output_point&& value);

    const chain::script& script() const;
    void set_script(const chain::script& value);
    void set_script(chain::script&& value);

    const chain::witness& witness() const;
    void set_witness(const chain::witness& value);
    void set_witness(chain::witness&& value);

    uint32_t sequence() const;
    void set_sequence(uint32_t value);

    /////// The first payment address extracted (may be invalid).
    ////typename wallet::payment_address address() const;

    /////// The payment addresses extracted from this input as a standard script.
    ////typename wallet::payment_address::list addresses() const;

    // Utilities.
    //-------------------------------------------------------------------------

    /// Clear witness.
    void strip_witness();

    // Validation.
    //-------------------------------------------------------------------------

    bool is_final() const;
    bool is_segregated() const;
    bool is_locked(size_t block_height, uint32_t median_time_past) const;
    size_t signature_operations(bool bip16, bool bip141) const;
    bool extract_reserved_hash(hash_digest& out) const;
    bool extract_embedded_script(chain::script& out) const;
    ////bool extract_witness_script(chain::script& out,
    ////    const chain::script& prevout) const;

protected:
    void reset();
    ////void invalidate_cache() const;

private:
    ////typedef std::shared_ptr<typename wallet::payment_address::list> addresses_ptr;

    ////addresses_ptr addresses_cache() const;

    ////mutable upgrade_mutex mutex_;
    ////mutable addresses_ptr addresses_;

    output_point previous_output_;
    chain::script script_;
    chain::witness witness_;
    uint32_t sequence_;
};

} // namespace chain
} // namespace system
} // namespace libbitcoin

#endif
