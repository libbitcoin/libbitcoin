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
#ifndef LIBBITCOIN_SYSTEM_CRYPTO_RING_SIGNATURE_HPP
#define LIBBITCOIN_SYSTEM_CRYPTO_RING_SIGNATURE_HPP

#include <vector>
#include <bitcoin/system/crypto/elliptic_curve.hpp>
#include <bitcoin/system/crypto/hash.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {

/**
 * key_rings is a collection of rings of public keys.
 * Each ring contains several public keys.
 * Creating a valid signature requires at least one private key from each ring.
 * That is given three rings [{A, B, C}, {D, E, F}, {X, Y}], then to create a
 * valid signature, we must use a private key from each of those sets.
 * For example A and E and X. We can summarize this operation as:
 * (A or B or C) and (D or E or F) and (X or Y)
 */
typedef std::vector<compressed_list> key_rings;

/**
 * A borromean ring signature.
 * theta = {e_0, s_{i_j} : 0 <= i <= n, 0 <= j <= m_i}
 */
struct ring_signature
{
    typedef std::vector<secret_list> proof_list;

    ec_secret challenge;
    proof_list proofs;
};

/**
 * Prepare hash digest for use in ring signature signing and verification.
 * Returns sha256(message || flatten(rings))
 * @return     digest   Ring signature digest
 */
BC_API hash_digest digest(const data_slice& message, const key_rings& rings);

/**
 * Create a borromean ring signature.
 * There must exist a valid signing key for each ring of public keys.
 * For example given a ring of [{A, B, C}, {D, E, F}, {X, Y}] then we
 * must have a set of keys that satisfies this constraint:
 * (A or B or C) and (D or E or F) and (X or Y)
 * @param[in]  out      The new signature.
 * @param[in]  secrets  Signing keys. Should be at least one from each ring.
 * @param[in]  rings    The rings each with n_i public keys.
 * @param[in]  digest   The message digest to sign.
 * @param[in]  salts    Randomizing seed values.
 * @return false if the signing operation fails.
 */
BC_API bool sign(ring_signature& out, const secret_list& secrets,
    const key_rings& rings, const hash_digest& digest,
    const secret_list& salts);

/**
 * Verify a borromean ring signature.
 * @param[in]  rings        The rings each with N_i public keys.
 * @param[in]  digest       The message digest to verify.
 * @param[in]  signature    Signature.
 * @return false if the verify operation fails.
 */
BC_API bool verify(const key_rings& rings, const hash_digest& digest,
    const ring_signature& signature);

} // namespace system
} // namespace libbitcoin

#endif

