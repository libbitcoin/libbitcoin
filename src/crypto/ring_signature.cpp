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
#include <bitcoin/system/crypto/ring_signature.hpp>

#include <algorithm>
#include <map>
#include <numeric>
#include <vector>
#include <secp256k1.h>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/crypto/hash.hpp>
#include <bitcoin/system/stream/stream.hpp>
#include <bitcoin/system/wallet/keys/hd_private.hpp>
#include <bitcoin/system/wallet/keys/ec_point.hpp>
#include <bitcoin/system/wallet/keys/ec_scalar.hpp>

namespace libbitcoin {
namespace system {

typedef std::vector<uint32_t> index_list;
typedef std::map<ec_compressed, ec_secret> secret_keys_map;

static ec_scalar borromean_hash(const hash_digest& M, const data_slice& R,
    uint32_t i, uint32_t j)
{
    // e = H(M || R || i || j)
    data_chunk data(hash_size + R.size() + 2u * sizeof(uint32_t), 0x00);
    write::bytes::copy out(data);
    out.write_bytes(R);
    out.write_bytes(M);
    out.write_4_bytes_big_endian(i);
    out.write_4_bytes_big_endian(j);
    return sha256_hash(data);
}

// Take a list of secret keys and generate a mapping from public key -> secret.
static bool generate_keys_map(secret_keys_map& out, const secret_list& secrets)
{
    for (const auto& secret: secrets)
    {
        ec_compressed public_key;
        if (!secret_to_public(public_key, secret))
            return false;

        out.insert({ public_key, secret });
    }

    return true;
}

// Make a list of public keys for which we have the corresponding secret key
// in a single ring of public keys.
static const compressed_list known_keys_in_ring(
    const secret_keys_map& secret_keys, const compressed_list& ring)
{
    compressed_list known_ring;

    for (const auto& key: ring)
        if (secret_keys.find(key) != secret_keys.end())
            known_ring.push_back(key);

    return known_ring;
}

// For all rings, make a list of known public keys corresponding to each ring.
static key_rings partition_keys_into_rings(const secret_keys_map& secret_keys,
    const key_rings& rings)
{
    key_rings known_keys;
    for (const auto& ring: rings)
        known_keys.push_back(known_keys_in_ring(secret_keys, ring));

    return known_keys;
}

// Make a list of indexes of where our known key occurs in each ring of
// public keys. That is given a ring of {A, B, C} where we know the
// private key of B, it will return 1 (the index in the ring).
// This function computes this for all rings.
static bool create_key_indexes(index_list& out, const key_rings& rings,
    const key_rings& known_keys_by_ring)
{
    BITCOIN_ASSERT(known_keys_by_ring.size() == rings.size());

    for (uint32_t i = 0; i < rings.size(); ++i)
    {
        const auto& ring = rings[i];
        const auto& known = known_keys_by_ring[i];
        const auto it = std::find(ring.begin(), ring.end(), known.back());

        if (it == ring.end())
            return false;

        // The cast is safe because indexes cannot exceed max_uint32.
        out.push_back(static_cast<uint32_t>(std::distance(ring.begin(), it)));
    }

    return true;
}

static bool generate_known_indexes(index_list& out, const key_rings& rings,
    const secret_keys_map& secret_keys)
{
    auto known_keys_by_ring = partition_keys_into_rings(secret_keys, rings);
    BITCOIN_ASSERT(known_keys_by_ring.size() == rings.size());

    const auto empty = [](const compressed_list& ring)
    {
        return ring.empty();
    };

    // Check there is a known secret key in each ring.
    const auto has_empty = std::any_of(known_keys_by_ring.begin(),
        known_keys_by_ring.end(), empty);

    // Compute indexes for known keys inside the rings.
    return !has_empty && create_key_indexes(out, rings, known_keys_by_ring);
}

static ec_point calculate_R(const ec_scalar& s, const ec_scalar& e,
    const ec_point& P)
{
    return s * ec_point::generator + e * P;
}

static ec_point calculate_last_R_signing(const compressed_list& ring,
    uint32_t i, const hash_digest& digest, const ring_signature& signature,
    const uint32_t known_key_index, const secret_list& salts)
{
    auto R_i_j = salts[i] * ec_point::generator;
    if (!R_i_j)
        return {};

    // Start one above index of known key and loop until the end.
    for (uint32_t j = add1(known_key_index); j < ring.size(); ++j)
    {
        BITCOIN_ASSERT(j < signature.proofs[i].size());
        const ec_scalar s(signature.proofs[i][j]);
        if (!s)
            return {};

        // Calculate e and R until the end of this ring.
        const auto e_i_j = borromean_hash(digest, R_i_j.point(), i, j);
        if (!e_i_j)
            return {};

        R_i_j = calculate_R(s, e_i_j, ring[j]);
        if (!R_i_j)
            return {};
    }

    return R_i_j;
}

static bool calculate_e0(ring_signature& out, const key_rings& rings,
    const hash_digest& digest, const secret_list& salts,
    const index_list& known_key_indexes)
{
    data_chunk e0_data;
    e0_data.reserve(ec_compressed_size * rings.size() + hash_size);
    BITCOIN_ASSERT(known_key_indexes.size() == rings.size());
    BITCOIN_ASSERT(out.proofs.size() == rings.size());
    BITCOIN_ASSERT(salts.size() == rings.size());

    for (uint32_t i = 0; i < rings.size(); ++i)
    {
        const auto& ring = rings[i];
        const auto known_key_index = known_key_indexes[i];

        // Calculate the last R value.
        const auto last_R = calculate_last_R_signing(ring, i, digest, out,
            known_key_index, salts);

        if (!last_R)
            return false;

        // Add this ring to e0
        extend(e0_data, last_R.point());
    }

    extend(e0_data, digest);
    out.challenge = sha256_hash(e0_data);
    return true;
}

static bool calculate_e_at_known_key_index(ec_scalar& e_i_j,
    const ring_signature& signature, const compressed_list& ring,
    const hash_digest& digest, const uint32_t i,
    const uint32_t known_key_index)
{
    BITCOIN_ASSERT(signature.proofs[i].size() > known_key_index);
    BITCOIN_ASSERT(ring.size() > known_key_index);

    // Loop until index of known key.
    for (uint32_t j = 0; j < known_key_index; ++j)
    {
        const ec_scalar s{ signature.proofs[i][j] };
        if (!s)
            return false;

        // Calculate e and R until reaching the index.
        const auto R = calculate_R(s, e_i_j, ring[j]);
        if (!R)
            return false;

        e_i_j = borromean_hash(digest, R.point(), i, add1(j));
        if (!e_i_j)
            return false;
    }

    return true;
}

static bool join_rings(ring_signature& out, const key_rings& rings,
    const hash_digest& digest, const secret_list& salts,
    const index_list& known_key_indexes, secret_keys_map& secret_keys)
{
    for (uint32_t i = 0; i < rings.size(); ++i)
    {
        const auto& ring = rings[i];
        const auto known_key_index = known_key_indexes[i];

        // Calculate starting e value of this current ring.
        auto e_i_j = borromean_hash(digest, out.challenge, i, zero);
        if (!e_i_j)
            return false;

        if (!calculate_e_at_known_key_index(e_i_j, out, ring, digest, i,
            known_key_index))
            return false;

        // Find secret key used for calculation in the next step.
        BITCOIN_ASSERT(known_key_index < ring.size());
        const auto& known_public_key = ring[known_key_index];

        BITCOIN_ASSERT(secret_keys.find(known_public_key) != secret_keys.end());
        const auto& secret = secret_keys[known_public_key];

        // Now close the ring using this calculation:
        const auto& k = salts[i];
        const auto& x = secret;

        const auto s = k - e_i_j * x;
        if (!s)
            return false;

        // Close the ring.
        out.proofs[i][known_key_index] = s;
    }

    return true;
}

static ec_point calculate_last_R_verify(const compressed_list& ring,
    ec_scalar e_i_j, uint32_t i, const hash_digest& digest,
    const ring_signature& signature)
{
    ec_point R_i_j;
    BITCOIN_ASSERT(signature.proofs[i].size() == ring.size());

    for (uint32_t j = 0; j < ring.size(); ++j)
    {
        // s_i_j
        const ec_scalar s(signature.proofs[i][j]);

        if (!s || !e_i_j)
            return {};

        // Calculate R and e values until the end.
        R_i_j = calculate_R(s, e_i_j, ring[j]);
        if (!R_i_j)
            return {};

        // Calculate the next e value.
        e_i_j = borromean_hash(digest, R_i_j.point(), i, j + 1u);
        if (!e_i_j)
            return {};
    }

    return R_i_j;
}

// API
//-----------------------------------------------------------------------------

hash_digest digest(const data_slice& message, const key_rings& rings)
{
    const auto sum = [](size_t size, const compressed_list& ring)
    {
        return size + ring.size() * ec_compressed_size;
    };

    const auto size = std::accumulate(rings.begin(), rings.end(),
        message.size(), sum);

    data_chunk data(message);
    data.reserve(size);

    for (const auto& ring: rings)
        for (const auto& key: ring)
            extend(data, key);

    return sha256_hash(data);
}

bool sign(ring_signature& out, const secret_list& secrets,
    const key_rings& rings, const hash_digest& digest,
    const secret_list& salts)
{
    // Guard against overflow.
    if (rings.size() >= max_uint32)
        return false;

    secret_keys_map secret_keys;
    if (!generate_keys_map(secret_keys, secrets))
        return false;

    index_list known_key_indexes;
    if (!generate_known_indexes(known_key_indexes, rings, secret_keys))
        return false;

    if (!calculate_e0(out, rings, digest, salts, known_key_indexes))
        return false;

    // Join each ring at the index where the secret key is known.
    return join_rings(out, rings, digest, salts, known_key_indexes,
        secret_keys);
}

// As compared with signing, we only have to perform a single step.
// The ring has already been computed, so now we just need to verify
// it by calculating e0, and looping all the way until the end R value
// which we use to recalculate e0.
// If the values match then we have a valid ring signature.
bool verify(const key_rings& rings, const hash_digest& digest,
    const ring_signature& signature)
{
    // Guard against overflow.
    if (rings.size() >= max_uint32 || signature.proofs.size() != rings.size())
        return false;

    data_chunk e0_data;
    e0_data.reserve(ec_compressed_size * rings.size() + hash_size);

    for (uint32_t i = 0; i < rings.size(); ++i)
    {
        // Calculate first e value for this ring.
        const auto e_i_0 = borromean_hash(digest, signature.challenge, i, zero);
        if (!e_i_0)
            return false;

        // Calculate the last R value.
        const auto last_R = calculate_last_R_verify(rings[i], e_i_0, i, digest,
            signature);
        if (!last_R)
            return false;

        // Add this ring to e0.
        extend(e0_data, last_R.point());
    }

    extend(e0_data, digest);

    // Hash data to produce e0 value.
    const auto e0_hash = sha256_hash(e0_data);

    // Verification step.
    return e0_hash == signature.challenge;
}

} // namespace system
} // namespace libbitcoin
