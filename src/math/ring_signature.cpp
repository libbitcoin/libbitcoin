/**
 * Copyright (c) 2011-2018 libbitcoin developers (see AUTHORS)
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
#include <bitcoin/bitcoin/math/ring_signature.hpp>

#include <map>
#include <boost/range/combine.hpp>
#include <secp256k1.h>
#include <bitcoin/bitcoin/utility/serializer.hpp>
#include <bitcoin/bitcoin/wallet/hd_private.hpp>

namespace libbitcoin {

typedef std::map<ec_compressed, ec_secret> secret_keys_map;

typedef std::vector<size_t> index_list;

// Take a list of secret keys and generate a mapping from public key -> secret
secret_keys_map generate_keys_map(const secret_list& secrets)
{
    secret_keys_map keys;
    for (const auto& secret: secrets)
    {
        // Compute public key
        ec_compressed public_key;
        if (!secret_to_public(public_key, secret))
            return secret_keys_map();

        // Insert into map
        keys.insert({ public_key, secret });
    }

    return keys;
}

// Make a list of public keys for which we have the corresponding secret key
// in a single ring of public keys.
const point_list known_keys_in_ring(
    const secret_keys_map& secret_keys, const point_list& ring)
{
    point_list known_ring;
    for (const auto& key: ring)
    {
        // Is this public key in our secret keys map?
        if (secret_keys.find(key) != secret_keys.end())
            known_ring.push_back(key);
    }
    return known_ring;
}

// For all rings, make a list of known public keys corresponding to each ring.
const key_rings partition_keys_into_rings(
    const secret_keys_map& secret_keys, const key_rings& rings)
{
    key_rings known_keys;
    // For each ring in our rings...
    for (const auto& ring: rings)
    {
        // ... find known keys corresponding to our private keys
        const auto known_ring = known_keys_in_ring(secret_keys, ring);
        // And add this list to our rings of known public keys.
        known_keys.push_back(known_ring);
    }
    return known_keys;
}

// Make a list of indexes of where our known key occurs in each ring of
// public keys. That is given a ring of {A, B, C} where we know the
// private key of B, it will return 1 (the index in the ring).
// This function computes this for all rings.
index_list search_key_indexes(
    const key_rings& rings, const key_rings& known_keys_by_ring)
{
    index_list known_key_indexes;
    BITCOIN_ASSERT(known_keys_by_ring.size() == rings.size());
    // Iterate all our rings.
    for (size_t i = 0; i < rings.size(); ++i)
    {
        const auto& ring = rings[i];
        const auto& known = known_keys_by_ring[i];
        // Find known key in this current ring.
        const auto it = std::find(ring.begin(), ring.end(), known.back());
        BITCOIN_ASSERT(it != ring.end());
        // Calculate and store the index
        const auto index = std::distance(ring.begin(), it);
        known_key_indexes.push_back(index);
    }
    return known_key_indexes;
}

// Returns message || flatten(rings)
data_chunk concatenate(data_slice message, const key_rings& rings)
{
    data_chunk result(message.begin(), message.end());
    for (const auto& ring: rings)
        for (const auto& key: ring)
            extend_data(result, key);
    return result;
}

// Generate random k and s values.
// There is a single k for each ring.
// Each key in each ring, has a corresponding s value.
// That is, each ring[i][j] should have a corresponding s[i][j]
void random_k_and_s_values(
    secret_list& out_k, ring_signature::s_values_type& out_s,
    const key_rings& rings, data_slice seed)
{
    // This value increments inside this function with every call
    // to random_scalar() lambda.
    uint32_t nonce = 0;
    // Use a hd_private wallet as a random generator for ec_secret values.
    const auto random_scalar = [](data_slice seed, uint32_t& nonce)
    {
        static const wallet::hd_private generate_secret(to_chunk(seed));
        return generate_secret.derive_private(nonce++).secret();
    };

    // Each ring[i][j] should have a corresponding s[i][j]
    for (const auto& ring: rings)
    {
        secret_list s_ring;
        for (const auto& key: ring)
            s_ring.push_back(random_scalar(seed, nonce));
        out_s.push_back(s_ring);

        out_k.push_back(random_scalar(seed, nonce));
    }
}

template <typename R_Type>
ec_secret borromean_hash(const hash_digest& M, const R_Type& R,
    size_t i, size_t j)
{
    // e = H(M || R || i || j)
    data_chunk data(hash_size + R.size() + 4 + 4);
    auto serial = make_unsafe_serializer(data.begin());
    serial.write_hash(M);
    serial.write_bytes(R);
    serial.write_4_bytes_little_endian(i);
    serial.write_4_bytes_little_endian(j);
    return sha256_hash(data);
}

ec_compressed calculate_R(const ec_secret& s, const ec_secret& e,
    const ec_compressed& ring_key)
{
    // R = s G + e P
    //   = (s + ex) G
    //   = k G
    ec_compressed eP = ring_key;
    // eP = e * P
    bool rc = ec_multiply(eP, e);
    BITCOIN_ASSERT(rc);
    // sG = s * G
    ec_compressed sG;
    rc = secret_to_public(sG, s);
    BITCOIN_ASSERT(rc);
    // result = s G + e P
    ec_compressed result;
    rc = ec_sum(result, {sG, eP});
    BITCOIN_ASSERT(rc);
    return result;
}

ec_secret calculate_s(const ec_secret& k, const ec_secret& e,
    const ec_secret& secret)

{
    // s = k - e x
    ec_secret result = e;
    // result = e * x
    bool rc = ec_multiply(result, secret);
    BITCOIN_ASSERT(rc);
    // result = - e x
    rc = ec_negate(result);
    BITCOIN_ASSERT(rc);
    // result = k - e x
    rc = ec_add(result, k);
    BITCOIN_ASSERT(rc);
    return result;
}

bool sign(ring_signature& out, const secret_list& secrets,
    const key_rings& rings, const data_slice message, const data_slice seed)
{
    // Create public key -> secret key map
    auto secret_keys = generate_keys_map(secrets);
    if (secret_keys.empty())
        return false;
    // Organize known public keys into corresponding rings
    const auto known_keys_by_ring =
        partition_keys_into_rings(secret_keys, rings);

    // Check we know a secret key in each ring
    auto non_empty_ring = [](const point_list& ring)
    {
        return !ring.empty();
    };
    const bool all_rings_nonzero_size = std::all_of(
        known_keys_by_ring.begin(), known_keys_by_ring.end(), non_empty_ring);

    if (!all_rings_nonzero_size)
        return false;

    // Compute indexes for known keys inside the rings.
    const auto known_key_indexes =
        search_key_indexes(rings, known_keys_by_ring);
    BITCOIN_ASSERT(known_key_indexes.size() == rings.size());

    // Compute message digest M
    const auto message_data = concatenate(message, rings);
    const auto M = sha256_hash(message_data);

    // Generate random k and s values
    secret_list k;
    random_k_and_s_values(k, out.s, rings, seed);

    // ---------------------------------------------------------------------
    // Step 1: calculate e0
    // ---------------------------------------------------------------------
    data_chunk e0_data;

    for (size_t i = 0; i < rings.size(); ++i)
    {
        BITCOIN_ASSERT(i < known_key_indexes.size());
        BITCOIN_ASSERT(i < k.size());
        BITCOIN_ASSERT(i < out.s.size());

        // Current ring and index of known key
        const auto& ring = rings[i];
        const size_t known_key_index = known_key_indexes[i];

        // Calculate starting R value...
        ec_compressed R_i_j;
        bool rc = secret_to_public(R_i_j, k[i]);

        // ... Start one above index of known key and loop until the end
        for (size_t j = known_key_index + 1; j < ring.size(); ++j)
        {
            BITCOIN_ASSERT(j < out.s[i].size());
            const auto& s = out.s[i][j];

            // Calculate e and R until the end of this ring.
            const auto e_i_j = borromean_hash(M, R_i_j, i, j);
            R_i_j = calculate_R(s, e_i_j, ring[j]);
        }
        // Add this ring to e0
        extend_data(e0_data, R_i_j);
    }
    extend_data(e0_data, M);
    // Hash data to produce e0 value
    out.e = sha256_hash(e0_data);

    // ---------------------------------------------------------------------
    // Step 2: join up each ring at the index where we know the secret key
    // ---------------------------------------------------------------------
    for (size_t i = 0; i < rings.size(); ++i)
    {
        BITCOIN_ASSERT(i < known_key_indexes.size());
        BITCOIN_ASSERT(i < k.size());
        BITCOIN_ASSERT(i < out.s.size());

        // Current ring and index of known key
        const auto& ring = rings[i];
        const size_t known_key_index = known_key_indexes[i];

        // Calculate starting e value of this current ring.
        auto e_i_j = borromean_hash(M, out.e, i, 0);

        // Loop until index of known key.
        for (size_t j = 0; j < known_key_index; ++j)
        {
            BITCOIN_ASSERT(j < out.s[i].size());
            const auto& s = out.s[i][j];

            // Calculate e and R until we reach our index.
            BITCOIN_ASSERT(j < ring.size());
            const auto R_i_j = calculate_R(s, e_i_j, ring[j]);
            e_i_j = borromean_hash(M, R_i_j, i, j + 1);
        }

        // Find secret key used for calculation in the next step.
        BITCOIN_ASSERT(known_key_index < ring.size());
        const auto& known_public_key = ring[known_key_index];
        BITCOIN_ASSERT(secret_keys.find(known_public_key) != secret_keys.end());
        const auto& secret = secret_keys[known_public_key];

        // Now close the ring using this calculation:
        // s = k - e x
        out.s[i][known_key_index] = calculate_s(k[i], e_i_j, secret);
    }

    return true;
}

bool verify(
    const key_rings& rings,
    const data_slice message,
    const ring_signature& signature)
{
    // Compute message digest M
    const data_chunk message_data = concatenate(message, rings);
    const hash_digest M = sha256_hash(message_data);

    // As compared with signing, we only have to perform a single step.
    // The ring has already been computed, so now we just need to verify
    // it by calculating e0, and looping all the way until the end R value
    // which we use to recalculate e0.
    // If the values match then we have a valid ring signature.

    data_chunk e0_data;

    // Loop through rings
    for (size_t i = 0; i < rings.size(); ++i)
    {
        BITCOIN_ASSERT(i < signature.s.size());

        const auto& ring = rings[i];

        // Calculate first e value for this ring.
        auto e_i_j = borromean_hash(M, signature.e, i, 0);

        ec_compressed R_i_j;
        for (size_t j = 0; j < ring.size(); ++j)
        {
            BITCOIN_ASSERT(j < signature.s[i].size());
            const auto& s = signature.s[i][j];

            // Calculate R and e values until the end.
            BITCOIN_ASSERT(j < ring.size());
            R_i_j = calculate_R(s, e_i_j, ring[j]);
            e_i_j = borromean_hash(M, R_i_j, i, j + 1);
        }
        extend_data(e0_data, R_i_j);
    }
    extend_data(e0_data, M);
    // Hash data to produce e0 value
    auto e0_hash = sha256_hash(e0_data);

    // Verification step.
    return e0_hash == signature.e;
}

} // namespace libbitcoin
