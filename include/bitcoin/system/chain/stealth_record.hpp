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
#ifndef LIBBITCOIN_SYSTEM_CHAIN_STEALTH_RECORD_HPP
#define LIBBITCOIN_SYSTEM_CHAIN_STEALTH_RECORD_HPP

#include <cstddef>
#include <cstdint>
#include <istream>
#include <vector>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace chain {
    
/// This class models blockchain storage of a stealth record.
/// The store excludes assumed sign byte (0x02) of the ephemeral public key.
class BC_API stealth_record
{
public:
    typedef std::vector<stealth_record> list;
    typedef hash_digest unsigned_ephemeral_key;

    // Constructors.
    //-------------------------------------------------------------------------

    stealth_record();

    stealth_record(chain::stealth_record&& other);
    stealth_record(const chain::stealth_record& other);

    stealth_record(size_t height, uint32_t prefix,
        hash_digest&& ephemeral_public_key, short_hash&& public_key_hash,
        hash_digest&& transaction_hash);
    stealth_record(size_t height, uint32_t prefix,
        const hash_digest& ephemeral_public_key,
        const short_hash& public_key_hash, const hash_digest& transaction_hash);

    stealth_record(size_t height, uint32_t prefix,
        const ec_compressed& ephemeral_public_key,
        short_hash&& public_key_hash, hash_digest&& transaction_hash);
    stealth_record(size_t height, uint32_t prefix,
        const ec_compressed& ephemeral_public_key,
        const short_hash& public_key_hash, const hash_digest& transaction_hash);

    // Operators.
    //-------------------------------------------------------------------------

    stealth_record& operator=(stealth_record&& other);
    stealth_record& operator=(const stealth_record& other);

    bool operator==(const stealth_record& other) const;
    bool operator!=(const stealth_record& other) const;

    // Deserialization.
    //-------------------------------------------------------------------------

    static stealth_record factory(const data_chunk& data, bool wire=true);
    static stealth_record factory(std::istream& stream, bool wire=true);
    static stealth_record factory(reader& source, bool wire=true);

    bool from_data(const data_chunk& data, bool wire=true);
    bool from_data(std::istream& stream, bool wire=true);
    bool from_data(reader& source, bool wire=true);

    /// Skip read of records below this height or not matching (non-wire only).
    bool from_data(reader& source, size_t start_height, const binary& filter);

    bool is_valid() const;

    // Serialization.
    //-------------------------------------------------------------------------

    data_chunk to_data(bool wire=true) const;
    void to_data(std::ostream& stream, bool wire=true) const;
    void to_data(writer& sink, bool wire=true) const;

    // Properties (size, accessors).
    //-------------------------------------------------------------------------

    static size_t satoshi_fixed_size(bool wire=true);
    size_t serialized_size(bool wire=true) const;

    size_t height() const;
    uint32_t prefix() const;
    ec_compressed ephemeral_public_key() const;
    const hash_digest& unsigned_ephemeral_public_key() const;
    const short_hash& public_key_hash() const;
    const hash_digest& transaction_hash() const;

protected:
    void reset();

private:
    uint32_t height_;
    uint32_t prefix_;
    hash_digest unsigned_ephemeral_;
    short_hash public_key_hash_;
    hash_digest transaction_hash_;
};

} // namespace chain
} // namespace system
} // namespace libbitcoin

#endif
