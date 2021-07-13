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
#include <bitcoin/system/wallet/keys/stealth.hpp>

#include <algorithm>
#include <utility>
#include <bitcoin/system/chain/chain.hpp>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/serial/serial.hpp>

namespace libbitcoin {
namespace system {

using namespace bc::system::chain;

bool is_stealth_script(const script& script)
{
    return script::is_pay_null_data_pattern(script.operations()) &&
        (script[1].data().size() >= hash_size);
}

bool to_stealth_prefix(uint32_t& out_prefix, const script& script)
{
    if (!is_stealth_script(script))
        return false;

    // A stealth prefix is the full 32 bits (prefix to the hash).
    // A stealth filter is a leftmost substring of the stealth prefix.

    const auto script_hash = bitcoin_hash(script.to_data(false));
    out_prefix = from_little_endian<uint32_t>(script_hash);
    return true;
}

// TODO: this can be implemented using libsecp256k1 without iteration.
// The public key must have a sign value of 0x02 (i.e. must be even y-valued).
bool create_ephemeral_key(ec_secret& out_secret, const data_chunk& seed)
{
    auto nonced_seed = splice({ 0x00 }, seed);
    ec_compressed point;

    // Iterate up to 255 times before giving up on finding a valid key pair.
    // This gives extremely high success probability given even distribution.
    for (uint8_t nonce = 0; nonce < max_uint8; ++nonce)
    {
        nonced_seed[0] = nonce;
        out_secret = hmac_sha256_hash(nonced_seed, "Stealth seed");

        if (secret_to_public(point, out_secret) && is_even_key(point))
            return true;
    }

    out_secret = null_hash;
    return false;
}

// Mine a filter into the leftmost bytes of sha256(sha256(output-script)).
bool create_stealth_data(script& out_null_data, ec_secret& out_secret,
    const binary& filter, const data_chunk& seed)
{
    // Create a valid ephemeral key pair using the seed and then the script.
    return create_ephemeral_key(out_secret, seed) &&
        create_stealth_script(out_null_data, out_secret, filter, seed);
}

// Mine a filter into the leftmost bytes of sha256(sha256(output-script)).
bool create_stealth_script(script& out_null_data, const ec_secret& secret,
    const binary& filter, const data_chunk& seed)
{
    // [ephemeral-public-key-hash:32][pad:0-44][nonce:4]
    static constexpr size_t max_pad = max_null_data_size - hash_size -
        sizeof(uint32_t);

    // Derive our initial nonce and pad from the provided seed.
    const auto bytes = sha512_hash(seed);

    // Create a pad size of 0-44 using the last of bytes (avoiding pad/nonce).
    const auto pad_size = bytes.back() % max_pad;

    // Allocate data of target size (36-80 bytes)
    data_chunk data(hash_size + pad_size + sizeof(uint32_t));

    // Obtain the ephemeral public key from the provided ephemeral secret key.
    ec_compressed point;
    if (!secret_to_public(point, secret) || !is_even_key(point))
        return false;

    // Copy the unsigned portion of the ephemeral public key into data.
    std::copy_n(std::next(point.begin()), sub1(ec_compressed_size), data.begin());

    // Copy arbitrary pad bytes into data.
    const auto pad_begin = data.begin() + hash_size;
    std::copy_n(bytes.begin(), pad_size, pad_begin);

    // Create an initial 32 bit nonce value from last word (avoiding pad).
    const data_slice slice(std::next(bytes.begin(), max_pad), bytes.end());
    const auto start = from_little_endian<uint32_t>(slice);

    // Mine a prefix into the double sha256 hash of the stealth script.
    // This will iterate up to 2^32 times before giving up.
    uint32_t out;
    for (uint32_t nonce = add1(start); nonce != start; ++nonce)
    {
        // Fill the nonce into the data buffer.
        const auto fill = to_little_endian(nonce);
        std::copy_n(fill.begin(), sizeof(nonce),
            std::prev(data.end(), sizeof(nonce)));

        // Create the stealth script with the current data.
        out_null_data = script(script::to_pay_null_data_pattern(data));

        // Test for match of filter to stealth script hash prefix.
        if (to_stealth_prefix(out, out_null_data) &&
            (filter == binary(filter.bits(), to_little_endian(out))))
            return true;
    }

    out_null_data.clear();
    return false;
}

bool extract_ephemeral_key(ec_compressed& out_ephemeral_public_key,
    const script& script)
{
    if (!is_stealth_script(script))
        return false;

    // The sign of the ephemeral public key is fixed by convention.
    // This requires the spender to generate a compliant (y-even) key.
    // That requires iteration with probability of 1 in 2 chance of success.
    out_ephemeral_public_key[0] = ec_even_sign;

    const auto& to = script[1].data();
    const auto from = std::next(out_ephemeral_public_key.begin());
    std::copy_n(to.begin(), hash_size, from);
    return true;
}

bool extract_ephemeral_key(hash_digest& out_unsigned_ephemeral_key,
    const script& script)
{
    if (!is_stealth_script(script))
        return false;

    const auto& data = script[1].data();
    std::copy_n(data.begin(), hash_size, out_unsigned_ephemeral_key.begin());
    return true;
}

bool shared_secret(ec_secret& out_shared, const ec_secret& secret,
    const ec_compressed& point)
{
    auto copy = point;
    if (!ec_multiply(copy, secret))
        return false;

    out_shared = sha256_hash(copy);
    return true;
}

bool uncover_stealth(ec_compressed& out_stealth,
    const ec_compressed& ephemeral_or_scan, const ec_secret& scan_or_ophemeral,
    const ec_compressed& spend)
{
    ec_secret shared;
    if (!shared_secret(shared, scan_or_ophemeral, ephemeral_or_scan))
        return false;

    auto copy = spend;
    if (!ec_add(copy, shared))
        return false;

    out_stealth = copy;
    return true;
}

bool uncover_stealth(ec_secret& out_stealth,
    const ec_compressed& ephemeral_or_scan, const ec_secret& scan_or_ephemeral,
    const ec_secret& spend)
{
    ec_secret shared;
    if (!shared_secret(shared, scan_or_ephemeral, ephemeral_or_scan))
        return false;

    auto copy = spend;
    if (!ec_add(copy, shared))
        return false;

    out_stealth = copy;
    return true;
}

} // namespace system
} // namespace libbitcoin
