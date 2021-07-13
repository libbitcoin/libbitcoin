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
#ifndef LIBBITCOIN_SYSTEM_CHAIN_OUTPUT_HPP
#define LIBBITCOIN_SYSTEM_CHAIN_OUTPUT_HPP

#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>
#include <string>
#include <vector>
#include <bitcoin/system/chain/script.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/mutex.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace chain {

class BC_API output
{
public:
    typedef std::vector<output> list;

    /// This is a sentinel used in .value to indicate not found/populated.
    /// This is a consensus value required by script::generate_signature_hash.
    static const uint64_t not_found;

    // THIS IS FOR LIBRARY USE ONLY, DO NOT CREATE A DEPENDENCY ON IT.
    struct validation
    {
        /// These are non-consensus sentinel values.
        static const uint32_t not_spent;
        static const uint8_t candidate_spent_true;
        static const uint8_t candidate_spent_false;

        /// Stored on output.
        /// The output is spent by a candidate block.
        bool candidate_spent = false;

        /// Stored on output.
        /// The output is spent by a confirmed block at the given height.
        uint32_t confirmed_spent_height = not_spent;
    };

    // Constructors.
    //-------------------------------------------------------------------------

    output();

    output(output&& other);
    output(const output& other);

    output(uint64_t value, chain::script&& script);
    output(uint64_t value, const chain::script& script);

    // Operators.
    //-------------------------------------------------------------------------

    output& operator=(output&& other);
    output& operator=(const output& other);

    bool operator==(const output& other) const;
    bool operator!=(const output& other) const;

    // Deserialization.
    //-------------------------------------------------------------------------

    static output factory(const data_chunk& data, bool wire=true);
    static output factory(std::istream& stream, bool wire=true);
    static output factory(reader& source, bool wire=true);

    bool from_data(const data_chunk& data, bool wire=true);
    bool from_data(std::istream& stream, bool wire=true);
    bool from_data(reader& source, bool wire=true, bool unused=false);

    bool is_valid() const;

    // Serialization.
    //-------------------------------------------------------------------------

    data_chunk to_data(bool wire=true) const;
    void to_data(std::ostream& stream, bool wire=true) const;
    void to_data(writer& sink, bool wire=true, bool unused=false) const;

    // Properties (size, accessors, cache).
    //-------------------------------------------------------------------------

    size_t serialized_size(bool wire=true) const;

    uint64_t value() const;
    void set_value(uint64_t value);

    const chain::script& script() const;
    void set_script(const chain::script& value);
    void set_script(chain::script&& value);

    /////// The first payment address extracted (may be invalid).
    ////typename wallet::payment_address address(
    ////    uint8_t typename p2kh_version=wallet::payment_address::mainnet_p2kh,
    ////    uint8_t typename p2sh_version=wallet::payment_address::mainnet_p2sh) const;

    /////// The payment addresses extracted from this output as a standard script.
    ////typename wallet::payment_address::list addresses(
    ////    uint8_t typename p2kh_version=wallet::payment_address::mainnet_p2kh,
    ////    uint8_t typename p2sh_version=wallet::payment_address::mainnet_p2sh) const;

    // Validation.
    //-------------------------------------------------------------------------

    size_t signature_operations(bool bip141) const;
    bool is_dust(uint64_t minimum_output_value) const;
    bool extract_committed_hash(hash_digest& out) const;

    // THIS IS FOR LIBRARY USE ONLY, DO NOT CREATE A DEPENDENCY ON IT.
    mutable validation metadata;

protected:
    void reset();
    ////void invalidate_cache() const;

private:
    ////typedef std::shared_ptr<typename wallet::payment_address::list> addresses_ptr;

    ////addresses_ptr addresses_cache() const;

    ////mutable upgrade_mutex mutex_;
    ////mutable addresses_ptr addresses_;

    uint64_t value_;
    chain::script script_;
};

} // namespace chain
} // namespace system
} // namespace libbitcoin

#endif
