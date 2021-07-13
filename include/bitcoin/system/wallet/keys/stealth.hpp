/**
 * Copyright (c) 2011-2021 libbitcoin developers (see AUTHORS)
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
#ifndef LIBBITCOIN_SYSTEM_WALLET_KEYS_STEALTH_HPP
#define LIBBITCOIN_SYSTEM_WALLET_KEYS_STEALTH_HPP

#include <cstdint>
#include <bitcoin/system/chain/chain.hpp>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {

/// Determine if the script is a null-data script of at least 32 data bytes.
BC_API bool is_stealth_script(const chain::script& script);

/// Convert a stealth info script to a prefix usable for stealth.
BC_API bool to_stealth_prefix(uint32_t& out_prefix,
    const chain::script& script);

/// Create a valid stealth ephemeral private key from the provided seed.
BC_API bool create_ephemeral_key(ec_secret& out_secret,
    const data_chunk& seed);

/// Create a stealth null data script the specified filter prefix.
/// Create an ephemeral secret key generated from the seed.
BC_API bool create_stealth_data(chain::script& out_null_data,
    ec_secret& out_secret, const binary& filter, const data_chunk& seed);

/// Create a stealth null data script the specified filter prefix.
/// Use the ephemeral secret key provided by parameter.
BC_API bool create_stealth_script(chain::script& out_null_data,
    const ec_secret& secret, const binary& filter, const data_chunk& seed);

/// Extract the stealth ephemeral public key from an output script.
BC_API bool extract_ephemeral_key(ec_compressed& out_ephemeral_public_key,
    const chain::script& script);

/// Extract the unsigned stealth ephemeral public key from an output script.
BC_API bool extract_ephemeral_key(hash_digest& out_unsigned_ephemeral_key,
    const chain::script& script);

/// Calculate the shared secret.
BC_API bool shared_secret(ec_secret& out_shared, const ec_secret& secret,
    const ec_compressed& point);

/// Uncover the stealth public key.
BC_API bool uncover_stealth(ec_compressed& out_stealth,
    const ec_compressed& ephemeral_or_scan, const ec_secret& scan_or_ephemeral,
    const ec_compressed& spend);

/// Uncover the stealth secret.
BC_API bool uncover_stealth(ec_secret& out_stealth,
    const ec_compressed& ephemeral_or_scan, const ec_secret& scan_or_ephemeral,
    const ec_secret& spend);

} // namespace system
} // namespace libbitcoin

#endif
