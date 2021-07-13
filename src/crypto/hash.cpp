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
#include <bitcoin/system/crypto/hash.hpp>

#include <cstddef>
#include <cstdint>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/serial/serial.hpp>
#include "external/crypto_scrypt.h"
#include "external/hmac_sha256.h"
#include "external/hmac_sha512.h"
#include "external/pbkdf2_sha256.h"
#include "external/pkcs5_pbkdf2.h"
#include "external/ripemd160.h"
#include "external/sha1.h"
#include "external/sha256.h"
#include "external/sha512.h"

namespace libbitcoin {
namespace system {

// Hash conversions of corresponding integers.
// ----------------------------------------------------------------------------

mini_hash to_hash(const uint48_t& value)
{
    return to_little_endian<to_bytes(48)>(value);
}

quarter_hash to_hash(const uint64_t& value)
{
    // Use integral uint64_t.
    return to_little_endian(value);
}

half_hash to_hash(const uint128_t& value)
{
    return to_little_endian<to_bytes(128)>(value);
}

short_hash to_hash(const uint160_t& value)
{
    return to_little_endian<to_bytes(160)>(value);
}

hash_digest to_hash(const uint256_t& value)
{
    return to_little_endian<to_bytes(256)>(value);
}

long_hash to_hash(const uint512_t& value)
{
    return to_little_endian<to_bytes(512)>(value);
}

// Integer conversions of corresponding hashes.
// ----------------------------------------------------------------------------

uint48_t to_uint48(const mini_hash& hash)
{
    return from_little_endian<to_bytes(48)>(hash);
}

uint64_t to_uint64(const quarter_hash& hash)
{
    // Use integral uint64_t.
    return from_little_endian<uint64_t>(hash);
}

uint128_t to_uint128(const half_hash& hash)
{
    return from_little_endian<to_bytes(128)>(hash);
}

uint160_t to_uint160(const short_hash& hash)
{
    return from_little_endian<to_bytes(160)>(hash);
}

uint256_t to_uint256(const hash_digest& hash)
{
    return from_little_endian<to_bytes(256)>(hash);
}

uint512_t to_uint512(const long_hash& hash)
{
    return from_little_endian<to_bytes(512)>(hash);
}

// Hash generators.
// ----------------------------------------------------------------------------

hash_digest scrypt_hash(const data_slice& data)
{
    return scrypt<hash_size>(data, data, 1024u, 1u, 1u);
}

hash_digest bitcoin_hash(const data_slice& data)
{
    return sha256_hash(sha256_hash(data));
}

short_hash bitcoin_short_hash(const data_slice& data)
{
    return ripemd160_hash(sha256_hash(data));
}

short_hash ripemd160_hash(const data_slice& data)
{
    short_hash hash;
    RMD160(data.data(), data.size(), hash.data());
    return hash;
}

data_chunk ripemd160_hash_chunk(const data_slice& data)
{
    data_chunk hash(short_hash_size);
    RMD160(data.data(), data.size(), hash.data());
    return hash;
}

short_hash sha1_hash(const data_slice& data)
{
    short_hash hash;
    SHA1_(data.data(), data.size(), hash.data());
    return hash;
}

data_chunk sha1_hash_chunk(const data_slice& data)
{
    data_chunk hash(short_hash_size);
    SHA1_(data.data(), data.size(), hash.data());
    return hash;
}

hash_digest sha256_hash(const data_slice& data)
{
    hash_digest hash;
    SHA256_(data.data(), data.size(), hash.data());
    return hash;
}

data_chunk sha256_hash_chunk(const data_slice& data)
{
    data_chunk hash(hash_size);
    SHA256_(data.data(), data.size(), hash.data());
    return hash;
}

hash_digest sha256_hash(const data_slice& first, const data_slice& second)
{
    hash_digest hash;
    SHA256CTX context;
    SHA256Init(&context);
    SHA256Update(&context, first.data(), first.size());
    SHA256Update(&context, second.data(), second.size());
    SHA256Final(&context, hash.data());
    return hash;
}

hash_digest hmac_sha256_hash(const data_slice& data, const data_slice& key)
{
    hash_digest hash;
    HMACSHA256(data.data(), data.size(), key.data(), key.size(), hash.data());
    return hash;
}

data_chunk pbkdf2_hmac_sha256_chunk(const data_slice& passphrase,
    const data_slice& salt, size_t iterations, size_t length)
{
    data_chunk hash(length);
    pbkdf2_sha256(passphrase.data(), passphrase.size(), salt.data(),
        salt.size(), iterations, hash.data(), length);
    return hash;
}

long_hash sha512_hash(const data_slice& data)
{
    long_hash hash;
    SHA512_(data.data(), data.size(), hash.data());
    return hash;
}

long_hash hmac_sha512_hash(const data_slice& data, const data_slice& key)
{
    long_hash hash;
    HMACSHA512(data.data(), data.size(), key.data(), key.size(), hash.data());
    return hash;
}

long_hash pkcs5_pbkdf2_hmac_sha512(const data_slice& passphrase,
    const data_slice& salt, size_t iterations)
{
    long_hash hash = null_long_hash;
    pkcs5_pbkdf2(passphrase.data(), passphrase.size(),
        salt.data(), salt.size(), hash.data(), hash.size(), iterations);

    // If pkcs5_pbkdf2 returns != 0 then hash will be zeroized.
    // This can only be caused by out-of-memory or invalid parameterization.
    return hash;
}

data_chunk scrypt_chunk(const data_slice& data, const data_slice& salt,
    uint64_t work, uint32_t resources, uint32_t parallelism, size_t length)
{
    data_chunk out(length, 0x00);
    crypto_scrypt(data.data(), data.size(), salt.data(), salt.size(), work,
        resources, parallelism, out.data(), out.size());

    // If crypto_scrypt returns != 0 then out will be zeroized.
    // This can only be caused by out-of-memory or invalid parameterization.
    return out;
}

// Objectives: deterministic, uniform distribution, efficient computation.
size_t djb2_hash(const data_slice& data)
{
    // Nothing special here except that it tested well against collisions.
    size_t hash = 5381;

    // Efficient sum of ((hash * 33) + byte) for all bytes.
    for (const auto byte: data)
        hash = ((hash << 5) + hash) + byte;

    return hash;
}

} // namespace system
} // namespace libbitcoin
