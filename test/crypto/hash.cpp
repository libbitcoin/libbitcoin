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
#include "../test.hpp"
#include "hash.hpp"

// TODO: test:

//// /// Hash conversions of corresponding integers.
////BC_API mini_hash to_hash(const uint48_t& value);
////BC_API quarter_hash to_hash(const uint64_t& value);
////BC_API half_hash to_hash(const uint128_t& value);
////BC_API short_hash to_hash(const uint160_t& value);
////BC_API hash_digest to_hash(const uint256_t& value);
////BC_API long_hash to_hash(const uint512_t& value);
////
/////// Integer conversions of corresponding hashes.
////BC_API uint48_t to_uint48(const mini_hash& hash);
////BC_API uint64_t to_uint64(const quarter_hash& hash);
////BC_API uint128_t to_uint128(const half_hash& hash);
////BC_API uint160_t to_uint160(const short_hash& hash);
////BC_API uint256_t to_uint256(const hash_digest& hash);
////BC_API uint512_t to_uint512(const long_hash& hash);

BOOST_AUTO_TEST_SUITE(hash_tests)

BOOST_AUTO_TEST_CASE(sha1_hash_test)
{
    for (const auto& result: sha1_tests)
    {
        data_chunk data;
        BOOST_REQUIRE(decode_base16(data, result.input));
        BOOST_REQUIRE_EQUAL(encode_base16(sha1_hash(data)), result.result);
    }
}

BOOST_AUTO_TEST_CASE(ripemd_hash_test)
{
    for (const auto& result: ripemd_tests)
    {
        data_chunk data;
        BOOST_REQUIRE(decode_base16(data, result.input));
        BOOST_REQUIRE_EQUAL(encode_base16(ripemd160_hash(data)), result.result);
    }

    const auto ripemd_hash1 = bitcoin_short_hash(to_array(110));
    BOOST_REQUIRE_EQUAL(encode_base16(ripemd_hash1), "17d040b739d639c729daaf627eaff88cfe4207f4");

    const auto ripemd_hash2 = bitcoin_short_hash(base16_literal("020641fde3a85beb8321033516de7ec01c35de96e945bf76c3768784a905471986"));
    BOOST_REQUIRE_EQUAL(encode_base16(ripemd_hash2), "c23e37c6fad06deab545f952992c8f28cb02bbe5");
}

BOOST_AUTO_TEST_CASE(sha256_hash_test)
{
    const data_chunk chunk{ 'd', 'a', 't', 'a' };
    const auto hash = sha256_hash(chunk);
    BOOST_REQUIRE_EQUAL(encode_base16(hash), "3a6eb0790f39ac87c94f3856b2dd2c5d110e6811602261a9a923d3bb23adc8b7");
}

BOOST_AUTO_TEST_CASE(sha512_hash_test)
{
    const data_chunk chunk{ 'd', 'a', 't', 'a' };
    const auto long_hash = sha512_hash(chunk);
    BOOST_REQUIRE_EQUAL(encode_base16(long_hash), "77c7ce9a5d86bb386d443bb96390faa120633158699c8844c30b13ab0bf92760b7e4416aea397db91b4ac0e5dd56b8ef7e4b066162ab1fdc088319ce6defc876");
}

BOOST_AUTO_TEST_CASE(hmac_sha256_hash_test)
{
    const data_chunk chunk{ 'd', 'a', 't', 'a' };
    const data_chunk key{ 'k', 'e', 'y' };
    const auto hash = hmac_sha256_hash(chunk, key);
    BOOST_REQUIRE_EQUAL(encode_base16(hash), "5031fe3d989c6d1537a013fa6e739da23463fdaec3b70137d828e36ace221bd0");
}

BOOST_AUTO_TEST_CASE(hmac_sha512_hash_test)
{
    const data_chunk chunk{ 'd', 'a', 't', 'a' };
    const data_chunk key{ 'k', 'e', 'y' };
    const auto long_hash = hmac_sha512_hash(chunk, key);
    BOOST_REQUIRE_EQUAL(encode_base16(long_hash), "3c5953a18f7303ec653ba170ae334fafa08e3846f2efe317b87efce82376253cb52a8c31ddcde5a3a2eee183c2b34cb91f85e64ddbc325f7692b199473579c58");
}

BOOST_AUTO_TEST_CASE(pkcs5_pbkdf2_hmac_sha512_test)
{
    for (const auto& result: pkcs5_pbkdf2_hmac_sha512_tests)
    {
        const auto hash = pkcs5_pbkdf2_hmac_sha512(result.passphrase, result.salt, result.iterations);
        BOOST_REQUIRE_EQUAL(encode_base16(hash), result.result);
    }
}

BOOST_AUTO_TEST_CASE(pbkdf2_hmac_sha256_test)
{
    for (const auto& result: pbkdf2_hmac_sha256_tests)
    {
        const auto data = pbkdf2_hmac_sha256_chunk(result.passphrase, result.salt, result.iterations, result.length);
        BOOST_REQUIRE_EQUAL(encode_base16(data), result.result);
    }
}

BOOST_AUTO_TEST_CASE(scrypt_hash_test)
{
    for (const auto& result: scrypt_hash_tests)
    {
        data_chunk data;
        BOOST_REQUIRE(decode_base16(data, result.input));
        BOOST_REQUIRE_EQUAL(encode_base16(scrypt_hash(data)), result.result);
    }
}

BOOST_AUTO_TEST_CASE(djb2_hash_test)
{
    // djb2_hash is size_t so cast for consistent test expectation.
    const auto hash = djb2_hash("01234567890abcdefghijklmnopqrstuvwxyz");
    BOOST_REQUIRE_EQUAL(static_cast<uint32_t>(hash), 0xe1669c01);
}

BOOST_AUTO_TEST_SUITE_END()
