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
#ifndef LIBBITCOIN_SYSTEM_CRYPTO_ELLIPTIC_CURVE_HPP
#define LIBBITCOIN_SYSTEM_CRYPTO_ELLIPTIC_CURVE_HPP

#include <cstddef>
#include <bitcoin/system/crypto/hash.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/radix/radix.hpp>

namespace libbitcoin {
namespace system {

/// The sign byte value for an even (y-valued) key.
static constexpr uint8_t ec_even_sign = 2;

/// Private key:
static constexpr size_t ec_secret_size = 32;
typedef data_array<ec_secret_size> ec_secret;

typedef std::vector<ec_secret> secret_list;

/// Compressed public key:
static constexpr size_t ec_compressed_size = 33;
typedef data_array<ec_compressed_size> ec_compressed;
typedef std::vector<ec_compressed> compressed_list;

/// Uncompressed public key:
static constexpr size_t ec_uncompressed_size = 65;
typedef data_array<ec_uncompressed_size> ec_uncompressed;
typedef std::vector<ec_uncompressed> uncompressed_list;

// Parsed ECDSA signature:
static constexpr size_t ec_signature_size = 64;
typedef data_array<ec_signature_size> ec_signature;

// DER encoded signature:
static constexpr size_t max_der_signature_size = 72;
typedef data_chunk der_signature;

/// DER encoded signature with sighash byte for contract endorsement:
static constexpr size_t min_endorsement_size = 9;
static constexpr size_t max_endorsement_size = 73;
typedef data_chunk endorsement;

// secg.org/sec2-v2.pdf
static const ec_compressed ec_compressed_generator = base16_array("02"
    "79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798");

// secg.org/sec2-v2.pdf
static const ec_uncompressed ec_uncompressed_generator = base16_array("04"
    "79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798"
    "483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8");

static const ec_compressed null_ec_compressed = base16_array("00"
    "0000000000000000000000000000000000000000000000000000000000000000");

static const ec_uncompressed null_ec_uncompressed = base16_array("00"
    "0000000000000000000000000000000000000000000000000000000000000000"
    "0000000000000000000000000000000000000000000000000000000000000000");

/// Recoverable ecdsa signature for message signing.
struct BC_API recoverable_signature
{
    ec_signature signature;
    uint8_t recovery_id;
};

// Add EC values
// ----------------------------------------------------------------------------

/// Compute the sum a += G * b.
BC_API bool ec_add(ec_compressed& point, const ec_secret& scalar);

/// Compute the sum a += G * b.
BC_API bool ec_add(ec_uncompressed& point, const ec_secret& scalar);

/// Compute the sum a = (a + b) % n.
BC_API bool ec_add(ec_secret& left, const ec_secret& right);

/// Compute the sum a += b.
BC_API bool ec_add(ec_compressed& left, const ec_compressed& right);

/// Compute the sum a += b.
BC_API bool ec_add(ec_compressed& left, const ec_uncompressed& right);

/// Compute the sum of compressed point values.
BC_API bool ec_sum(ec_compressed& out, const compressed_list& values);

// Multiply EC values
// ----------------------------------------------------------------------------

/// Compute the product point *= secret.
BC_API bool ec_multiply(ec_compressed& point, const ec_secret& scalar);

/// Compute the product point *= secret.
BC_API bool ec_multiply(ec_uncompressed& point, const ec_secret& scalar);

/// Compute the product a = (a * b) % n.
BC_API bool ec_multiply(ec_secret& left, const ec_secret& right);

// Negate EC values
// ----------------------------------------------------------------------------

/// Negate scalar.
BC_API bool ec_negate(ec_secret& scalar);

/// Invert point (flip on Y axis).
BC_API bool ec_negate(ec_compressed& point);

/// Invert point (flip on Y axis).
BC_API bool ec_negate(ec_uncompressed& point);

// Convert keys
// ----------------------------------------------------------------------------

/// Convert uncompressed point to compressed.
BC_API bool compress(ec_compressed& out, const ec_uncompressed& point);

/// Convert compressed point to decompressed.
BC_API bool decompress(ec_uncompressed& out, const ec_compressed& point);

/// Convert secret to a compressed point.
BC_API bool secret_to_public(ec_compressed& out, const ec_secret& secret);

/// Convert secret to an uncompressed point.
BC_API bool secret_to_public(ec_uncompressed& out, const ec_secret& secret);

// Verify keys
// ----------------------------------------------------------------------------

/// Verify secret.
BC_API bool verify(const ec_secret& secret);

/// Verify point.
BC_API bool verify(const ec_compressed& point);

/// Verify point.
BC_API bool verify(const ec_uncompressed& point);

// Detect public keys
// ----------------------------------------------------------------------------

/// Determine if the compressed public key is even (y-valued).
BC_API bool is_even_key(const ec_compressed& point);

/// Fast detection of compressed public key structure.
BC_API bool is_compressed_key(const data_slice& point);

/// Fast detection of uncompressed public key structure.
BC_API bool is_uncompressed_key(const data_slice& point);

/// Fast detection of compressed or uncompressed public key structure.
BC_API bool is_public_key(const data_slice& point);

/// Fast detection of endorsement structure (DER with signature hash type).
BC_API bool is_endorsement(const endorsement& endorsement);

// DER parse/encode
// ----------------------------------------------------------------------------

/// Parse an endorsement into signature hash type and DER signature.
BC_API bool parse_endorsement(uint8_t& sighash_type,
    der_signature& der_signature, const endorsement& endorsement);

/// Parse a DER encoded signature with optional strict DER enforcement.
/// Treat an empty DER signature as invalid, in accordance with BIP66.
BC_API bool parse_signature(ec_signature& out,
    const der_signature& der_signature, bool strict);

/// Encode an EC signature as DER (strict).
BC_API bool encode_signature(der_signature& out, const ec_signature& signature);

// EC sign/verify
// ----------------------------------------------------------------------------

/// Create a deterministic ECDSA signature using a private key.
BC_API bool sign(ec_signature& out, const ec_secret& secret,
    const hash_digest& hash);

/// Verify an EC signature using a compressed point.
BC_API bool verify_signature(const ec_compressed& point,
    const hash_digest& hash, const ec_signature& signature);

/// Verify an EC signature using an uncompressed point.
BC_API bool verify_signature(const ec_uncompressed& point,
    const hash_digest& hash, const ec_signature& signature);

/// Verify an EC signature using a potential point.
BC_API bool verify_signature(const data_slice& point, const hash_digest& hash,
    const ec_signature& signature);

// Recoverable sign/recover
// ----------------------------------------------------------------------------

/// Create a recoverable signature for use in message signing.
BC_API bool sign_recoverable(recoverable_signature& out,
    const ec_secret& secret, const hash_digest& hash);

/// Recover the compressed point from a recoverable message signature.
BC_API bool recover_public(ec_compressed& out,
    const recoverable_signature& recoverable, const hash_digest& hash);

/// Recover the uncompressed point from a recoverable message signature.
BC_API bool recover_public(ec_uncompressed& out,
    const recoverable_signature& recoverable, const hash_digest& hash);

} // namespace system
} // namespace libbitcoin

#endif
