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
#ifndef LIBBITCOIN_SYSTEM_CRYPTO_HASH_HPP
#define LIBBITCOIN_SYSTEM_CRYPTO_HASH_HPP

#include <cstddef>
#include <cstdint>
#include <vector>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {

/// Common bitcoin hash container sizes.
static constexpr size_t hash_size = 32;
static constexpr size_t half_hash_size = hash_size / 2;
static constexpr size_t quarter_hash_size = half_hash_size / 2;
static constexpr size_t long_hash_size = 2 * hash_size;
static constexpr size_t short_hash_size = 20;
static constexpr size_t mini_hash_size = 6;

/// Common bitcoin hash containers.
typedef data_array<mini_hash_size> mini_hash;
typedef data_array<quarter_hash_size> quarter_hash;
typedef data_array<half_hash_size> half_hash;
typedef data_array<short_hash_size> short_hash;
typedef data_array<hash_size> hash_digest;
typedef data_array<long_hash_size> long_hash;

/// Lists of common bitcoin hashes.
typedef std::vector<mini_hash> mini_hash_list;
typedef std::vector<quarter_hash> quarter_hash_list;
typedef std::vector<half_hash> half_hash_list;
typedef std::vector<short_hash> short_hash_list;
typedef std::vector<hash_digest> hash_list;
typedef std::vector<long_hash> long_hash_list;

/// Null-valued common hashes.

constexpr mini_hash null_mini_hash
{
    {
        0, 0, 0, 0, 0, 0
    }
};

constexpr quarter_hash null_quarter_hash
{
    {
        0, 0, 0, 0, 0, 0, 0, 0
    }
};

constexpr half_hash null_half_hash
{
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    }
};

constexpr short_hash null_short_hash
{
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    }
};

constexpr hash_digest null_hash
{
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    }
};

constexpr long_hash null_long_hash
{
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    }
};

/// Hash conversions of corresponding integers.
BC_API mini_hash to_hash(const uint48_t& value);
BC_API quarter_hash to_hash(const uint64_t& value);
BC_API half_hash to_hash(const uint128_t& value);
BC_API short_hash to_hash(const uint160_t& value);
BC_API hash_digest to_hash(const uint256_t& value);
BC_API long_hash to_hash(const uint512_t& value);

/// Integer conversions of corresponding hashes.
BC_API uint48_t to_uint48(const mini_hash& hash);
BC_API uint64_t to_uint64(const quarter_hash& hash);
BC_API uint128_t to_uint128(const half_hash& hash);
BC_API uint160_t to_uint160(const short_hash& hash);
BC_API uint256_t to_uint256(const hash_digest& hash);
BC_API uint512_t to_uint512(const long_hash& hash);

/// Generate a scrypt hash to fill a byte array.
/// Memory required (bytes) = 2 * 64 * work * resources.
template <size_t Size>
data_array<Size> scrypt(const data_slice& data, const data_slice& salt,
    uint64_t work, uint32_t resources, uint32_t parallelism);

/// Generate a scrypt hash.
BC_API hash_digest scrypt_hash(const data_slice& data);

/// Generate a bitcoin hash.
BC_API hash_digest bitcoin_hash(const data_slice& data);

/// Generate a bitcoin short hash.
BC_API short_hash bitcoin_short_hash(const data_slice& data);

/// Generate a ripemd160 hash.
BC_API short_hash ripemd160_hash(const data_slice& data);
BC_API data_chunk ripemd160_hash_chunk(const data_slice& data);

/// Generate a sha1 hash.
BC_API short_hash sha1_hash(const data_slice& data);
BC_API data_chunk sha1_hash_chunk(const data_slice& data);

/// Generate a sha256 hash.
BC_API hash_digest sha256_hash(const data_slice& data);
BC_API data_chunk sha256_hash_chunk(const data_slice& data);

/// Generate a sha256 hash.
/// This hash function is used in electrum seed stretching.
BC_API hash_digest sha256_hash(const data_slice& first,
    const data_slice& second);

// Generate a hmac sha256 hash.
BC_API hash_digest hmac_sha256_hash(const data_slice& data,
    const data_slice& key);

/// Generate a pbkdf2 hmac sha256 hash.
BC_API data_chunk pbkdf2_hmac_sha256_chunk(const data_slice& passphrase,
    const data_slice& salt, size_t iterations, size_t length);

/// Generate a sha512 hash.
BC_API long_hash sha512_hash(const data_slice& data);

/// Generate a hmac sha512 hash.
BC_API long_hash hmac_sha512_hash(const data_slice& data,
    const data_slice& key);

/// Generate a pkcs5 pbkdf2 hmac sha512 hash.
BC_API long_hash pkcs5_pbkdf2_hmac_sha512(const data_slice& passphrase,
    const data_slice& salt, size_t iterations);

/// Generate a scrypt hash of specified length.
BC_API data_chunk scrypt_chunk(const data_slice& data, const data_slice& salt,
    uint64_t work, uint32_t resources, uint32_t parallelism, size_t length);

/// DJB2 hash key algorithm by Dan Bernstein.
BC_API size_t djb2_hash(const data_slice& data);

} // namespace system
} // namespace libbitcoin

// Extend std and boost namespaces with djb2_hash.
//-----------------------------------------------------------------------------
// This allows our data_array to be incorporated into std/boost hash tables.

namespace std
{
template <size_t Size>
struct hash<bc::system::data_array<Size>>
{
    size_t operator()(const bc::system::data_array<Size>& data) const
    {
        return bc::system::djb2_hash(data);
    }
};
} // namespace std

namespace boost
{
template <size_t Size>
struct hash<bc::system::data_array<Size>>
{
    size_t operator()(const bc::system::data_array<Size>& data) const
    {
        return bc::system::djb2_hash(data);
    }
};
} // namespace boost

#include <bitcoin/system/impl/math/hash.ipp>

#endif
