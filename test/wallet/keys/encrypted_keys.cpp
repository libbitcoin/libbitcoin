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
#include "../../test.hpp"
#include <algorithm>
#include <cstddef>
#include <string>

BOOST_AUTO_TEST_SUITE(encrypted_tests)

using namespace bc::system::wallet;

#if defined(SLOW_TESTS) || !defined(NDEBUG)

#ifdef WITH_ICU

BOOST_AUTO_TEST_CASE(encrypted__fixture__unicode_passphrase__matches_encrypted_test_vector)
{
    const auto encoded_password = base16_literal("cf92cc8100f0909080f09f92a9");
    std::string passphrase(encoded_password.begin(), encoded_password.end());

    // This confirms that the passphrase decodes as expected in BIP38.
    auto normal = passphrase;
    BOOST_REQUIRE(to_canonical_composition(normal));
    data_chunk normalized(normal.size());
    std::copy_n(normal.begin(), normal.size(), normalized.begin());
    BOOST_REQUIRE_EQUAL(encode_base16(normalized), "cf9300f0909080f09f92a9");
}

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(encrypted__create_token_lot)

#define BC_REQUIRE_CREATE_TOKEN_LOT(passphrase, bytes, lot, sequence) \
    encrypted_token out_token; \
    BOOST_REQUIRE(create_token(out_token, passphrase, bytes, lot, sequence))

BOOST_AUTO_TEST_CASE(encrypted__create_token_lot__lot_overlow__false)
{
    const size_t lot = 1048575 + 1;
    const size_t sequence = 0;
    const auto passphrase = "";
    const auto salt = base16_literal("baadf00d");
    encrypted_token out_token;
    BOOST_REQUIRE(!create_token(out_token, passphrase, salt, lot, sequence));
}

BOOST_AUTO_TEST_CASE(encrypted__create_token_lot__sequence_overlow__false)
{
    const size_t lot = 0;
    const size_t sequence = 4095 + 1;
    const auto passphrase = "";
    const auto salt = base16_literal("baadf00d");
    encrypted_token out_token;
    BOOST_REQUIRE(!create_token(out_token, passphrase, salt, lot, sequence));
}

BOOST_AUTO_TEST_CASE(encrypted__create_token_lot__defaults__expected)
{
    const size_t lot = 0;
    const size_t sequence = 0;
    const auto passphrase = "";
    const auto salt = base16_literal("baadf00d");
    BC_REQUIRE_CREATE_TOKEN_LOT(passphrase, salt, lot, sequence);
    BOOST_REQUIRE_EQUAL(encode_base58(out_token), "passphrasecpXbDpHuo8F7yQVcg1eQKPuX7rzGwBtEH1YSZnKbyk75x3rugZu1ci4RyF4rEn");
}

BOOST_AUTO_TEST_CASE(encrypted__create_token_lot__passphrase__expected)
{
    const size_t lot = 0;
    const size_t sequence = 0;
    const auto passphrase = "passphrase";
    const auto salt = base16_literal("baadf00d");
    BC_REQUIRE_CREATE_TOKEN_LOT(passphrase, salt, lot, sequence);
    BOOST_REQUIRE_EQUAL(encode_base58(out_token), "passphrasecpXbDpHuo8F7x4pQXMhsJs2j7L8LTV8ujk9jGqgzUrafBeto9VrabP5SmvANvz");
}

BOOST_AUTO_TEST_CASE(encrypted__create_token_lot__passphrase_lot_max__expected)
{
    const size_t lot = 1048575;
    const size_t sequence = 0;
    const auto passphrase = "passphrase";
    const auto salt = base16_literal("baadf00d");
    BC_REQUIRE_CREATE_TOKEN_LOT(passphrase, salt, lot, sequence);
    BOOST_REQUIRE_EQUAL(encode_base58(out_token), "passphrasecpXbDpHuo8FGWnwMTnTFiHSDnqyARArE2YSFQzMHtCZvM2oWg2K3Ua2crKyc11");
}

BOOST_AUTO_TEST_CASE(encrypted__create_token_lot__passphrase_sequence_max__expected)
{
    const size_t lot = 0;
    const size_t sequence = 4095;
    const auto passphrase = "passphrase";
    const auto salt = base16_literal("baadf00d");
    BC_REQUIRE_CREATE_TOKEN_LOT(passphrase, salt, lot, sequence);
    BOOST_REQUIRE_EQUAL(encode_base58(out_token), "passphrasecpXbDpHuo8FGWnwMTnTFiHSDnqyARArE2YSFQzMHtCZvM2oWg2K3Ua2crKyc11");
}

BOOST_AUTO_TEST_CASE(encrypted__create_token_lot__passphrase_lot_sequence__expected)
{
    const size_t lot = 42;
    const size_t sequence = 42;
    const auto passphrase = "passphrase";
    const auto salt = base16_literal("baadf00d");
    BC_REQUIRE_CREATE_TOKEN_LOT(passphrase, salt, lot, sequence);
    BOOST_REQUIRE_EQUAL(encode_base58(out_token), "passphrasecpXbDpHuo8FGWnwMTnTFiHSDnqyARArE2YSFQzMHtCZvM2oWg2K3Ua2crKyc11");
}

BOOST_AUTO_TEST_SUITE_END()

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(encrypted__create_token_entropy)

#define BC_CREATE_TOKEN_ENTROPY(passphrase, bytes) \
    encrypted_token out_token; \
    create_token(out_token, passphrase, bytes)

BOOST_AUTO_TEST_CASE(encrypted__create_token_entropy__defaults__expected)
{
    const auto passphrase = "";
    const auto entropy = base16_literal("baadf00dbaadf00d");
    BC_CREATE_TOKEN_ENTROPY(passphrase, entropy);
    BOOST_REQUIRE_EQUAL(encode_base58(out_token), "passphraseqVHzjNrYRo5G6yLmJ7TQ49fKnQtsgjybNgNHAKBCQKoFZcTNjNJtg4oCUgtPt3");
}

BOOST_AUTO_TEST_CASE(encrypted__create_token_entropy__passphrase__expected)
{
    const auto passphrase = "passphrase";
    const auto entropy = base16_literal("baadf00dbaadf00d");
    BC_CREATE_TOKEN_ENTROPY(passphrase, entropy);
    BOOST_REQUIRE_EQUAL(encode_base58(out_token), "passphraseqVHzjNrYRo5G6sfRB4YdSaQ2m8URnkBYS1UT6JBju5G5o45YRZKLDpK6J3PEGq");
}

BOOST_AUTO_TEST_SUITE_END()

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(encrypted__encrypt_private)

#define BC_REQUIRE_ENCRYPT(secret, passphrase, version, compressed, expected) \
    encrypted_private out_private; \
    BOOST_REQUIRE(encrypt(out_private, secret, passphrase, version, compressed)); \
    BOOST_REQUIRE_EQUAL(encode_base58(out_private), expected)

// github.com/bitcoin/bips/blob/master/bip-0038.mediawiki#no-compression-no-ec-multiply
BOOST_AUTO_TEST_CASE(encrypted__encrypt_private__vector_0__expected)
{
    auto compression = false;
    const uint8_t version = 0x00;
    const auto expected = "6PRVWUbkzzsbcVac2qwfssoUJAN1Xhrg6bNk8J7Nzm5H7kxEbn2Nh2ZoGg";
    const auto secret = base16_literal("cbf4b9f70470856bb4f40f80b87edb90865997ffee6df315ab166d713af433a5");
    BC_REQUIRE_ENCRYPT(secret, "TestingOneTwoThree", version, compression, expected);
}

// github.com/bitcoin/bips/blob/master/bip-0038.mediawiki#no-compression-no-ec-multiply
BOOST_AUTO_TEST_CASE(encrypted__encrypt_private__vector_1__expected)
{
    auto compression = false;
    const uint8_t version = 0x00;
    const auto expected = "6PRNFFkZc2NZ6dJqFfhRoFNMR9Lnyj7dYGrzdgXXVMXcxoKTePPX1dWByq";
    const auto secret = base16_literal("09c2686880095b1a4c249ee3ac4eea8a014f11e6f986d0b5025ac1f39afbd9ae");
    BC_REQUIRE_ENCRYPT(secret, "Satoshi", version, compression, expected);
}

// github.com/bitcoin/bips/blob/master/bip-0038.mediawiki#compression-no-ec-multiply
BOOST_AUTO_TEST_CASE(encrypted__encrypt_private__vector_2_compressed__expected)
{
    auto compression = true;
    const uint8_t version = 0x00;
    const auto expected = "6PYNKZ1EAgYgmQfmNVamxyXVWHzK5s6DGhwP4J5o44cvXdoY7sRzhtpUeo";
    const auto secret = base16_literal("cbf4b9f70470856bb4f40f80b87edb90865997ffee6df315ab166d713af433a5");
    BC_REQUIRE_ENCRYPT(secret, "TestingOneTwoThree", version, compression, expected);
}

// github.com/bitcoin/bips/blob/master/bip-0038.mediawiki#compression-no-ec-multiply
BOOST_AUTO_TEST_CASE(encrypted__encrypt_private__vector_3_compressed__expected)
{
    auto compression = true;
    const uint8_t version = 0x00;
    const auto expected = "6PYLtMnXvfG3oJde97zRyLYFZCYizPU5T3LwgdYJz1fRhh16bU7u6PPmY7";
    const auto secret = base16_literal("09c2686880095b1a4c249ee3ac4eea8a014f11e6f986d0b5025ac1f39afbd9ae");
    BC_REQUIRE_ENCRYPT(secret, "Satoshi", version, compression, expected);
}

// #3 from: github.com/bitcoin/bips/blob/master/bip-0038.mediawiki#no-compression-no-ec-multiply
BOOST_AUTO_TEST_CASE(encrypted__encrypt_private__vector_unicode__expected)
{
    auto compression = false;
    const uint8_t version = 0x00;
    const auto encoded_password = base16_literal("cf92cc8100f0909080f09f92a9");
    std::string passphrase(encoded_password.begin(), encoded_password.end());
    const auto expected = "6PRW5o9FLp4gJDDVqJQKJFTpMvdsSGJxMYHtHaQBF3ooa8mwD69bapcDQn";
    const auto secret = base16_literal("64eeab5f9be2a01a8365a579511eb3373c87c40da6d2a25f05bda68fe077b66e");
    BC_REQUIRE_ENCRYPT(secret, passphrase, version, compression, expected);
}

BOOST_AUTO_TEST_SUITE_END()

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(encrypted__decrypt1)

// TODO: create compressed+multiplied and altchain/testnet vector(s).

#define BC_REQUIRE_DECRYPT_SECRET(key, passphrase) \
    ec_secret out_secret; \
    uint8_t out_version = 42; \
    bool out_is_compressed = true; \
    BOOST_REQUIRE(decrypt(out_secret, out_version, out_is_compressed, key, passphrase))

// github.com/bitcoin/bips/blob/master/bip-0038.mediawiki#no-compression-no-ec-multiply
BOOST_AUTO_TEST_CASE(encrypted__decrypt_private__vector_0__expected)
{
    const uint8_t expected_version = 0x00;
    const auto key = base58_literal("6PRVWUbkzzsbcVac2qwfssoUJAN1Xhrg6bNk8J7Nzm5H7kxEbn2Nh2ZoGg");
    BC_REQUIRE_DECRYPT_SECRET(key, "TestingOneTwoThree");
    BOOST_REQUIRE_EQUAL(encode_base16(out_secret), "cbf4b9f70470856bb4f40f80b87edb90865997ffee6df315ab166d713af433a5");
    BOOST_REQUIRE_EQUAL(out_version, expected_version);
    BOOST_REQUIRE(!out_is_compressed);
}

// github.com/bitcoin/bips/blob/master/bip-0038.mediawiki#no-compression-no-ec-multiply
BOOST_AUTO_TEST_CASE(encrypted__decrypt_private__vector_1__expected)
{
    const uint8_t expected_version = 0x00;
    const auto key = base58_literal("6PRNFFkZc2NZ6dJqFfhRoFNMR9Lnyj7dYGrzdgXXVMXcxoKTePPX1dWByq");
    BC_REQUIRE_DECRYPT_SECRET(key, "Satoshi");
    BOOST_REQUIRE_EQUAL(encode_base16(out_secret), "09c2686880095b1a4c249ee3ac4eea8a014f11e6f986d0b5025ac1f39afbd9ae");
    BOOST_REQUIRE_EQUAL(out_version, expected_version);
    BOOST_REQUIRE(!out_is_compressed);
}

// github.com/bitcoin/bips/blob/master/bip-0038.mediawiki#compression-no-ec-multiply
BOOST_AUTO_TEST_CASE(encrypted__decrypt_private__vector_2_compressed__expected)
{
    const uint8_t expected_version = 0x00;
    const auto key = base58_literal("6PYNKZ1EAgYgmQfmNVamxyXVWHzK5s6DGhwP4J5o44cvXdoY7sRzhtpUeo");
    BC_REQUIRE_DECRYPT_SECRET(key, "TestingOneTwoThree");
    BOOST_REQUIRE_EQUAL(encode_base16(out_secret), "cbf4b9f70470856bb4f40f80b87edb90865997ffee6df315ab166d713af433a5");
    BOOST_REQUIRE_EQUAL(out_version, expected_version);
    BOOST_REQUIRE(out_is_compressed);
}

// github.com/bitcoin/bips/blob/master/bip-0038.mediawiki#compression-no-ec-multiply
BOOST_AUTO_TEST_CASE(encrypted__decrypt_private__vector_3_compressed__expected)
{
    const uint8_t expected_version = 0x00;
    const auto key = base58_literal("6PYLtMnXvfG3oJde97zRyLYFZCYizPU5T3LwgdYJz1fRhh16bU7u6PPmY7");
    BC_REQUIRE_DECRYPT_SECRET(key, "Satoshi");
    BOOST_REQUIRE_EQUAL(encode_base16(out_secret), "09c2686880095b1a4c249ee3ac4eea8a014f11e6f986d0b5025ac1f39afbd9ae");
    BOOST_REQUIRE_EQUAL(out_version, expected_version);
    BOOST_REQUIRE(out_is_compressed);
}

// github.com/bitcoin/bips/blob/master/bip-0038.mediawiki#no-compression-no-ec-multiply
BOOST_AUTO_TEST_CASE(encrypted__decrypt_private__vector_4_multiplied__expected)
{
    const uint8_t expected_version = 0x00;
    const auto key = base58_literal("6PfQu77ygVyJLZjfvMLyhLMQbYnu5uguoJJ4kMCLqWwPEdfpwANVS76gTX");
    BC_REQUIRE_DECRYPT_SECRET(key, "TestingOneTwoThree");
    BOOST_REQUIRE_EQUAL(encode_base16(out_secret), "a43a940577f4e97f5c4d39eb14ff083a98187c64ea7c99ef7ce460833959a519");
    BOOST_REQUIRE_EQUAL(out_version, expected_version);
    BOOST_REQUIRE(!out_is_compressed);
}

// github.com/bitcoin/bips/blob/master/bip-0038.mediawiki#no-compression-no-ec-multiply
BOOST_AUTO_TEST_CASE(encrypted__decrypt_private__vector_5_multiplied__expected)
{
    const uint8_t expected_version = 0x00;
    const auto key = base58_literal("6PfLGnQs6VZnrNpmVKfjotbnQuaJK4KZoPFrAjx1JMJUa1Ft8gnf5WxfKd");
    BC_REQUIRE_DECRYPT_SECRET(key, "Satoshi");
    BOOST_REQUIRE_EQUAL(encode_base16(out_secret), "c2c8036df268f498099350718c4a3ef3984d2be84618c2650f5171dcc5eb660a");
    BOOST_REQUIRE_EQUAL(out_version, expected_version);
    BOOST_REQUIRE(!out_is_compressed);
}

// github.com/bitcoin/bips/blob/master/bip-0038.mediawiki#ec-multiply-no-compression-lotsequence-numbers
BOOST_AUTO_TEST_CASE(encrypted__decrypt_private__vector_6_multiplied_lot__expected)
{
    const uint8_t expected_version = 0x00;
    const auto key = base58_literal("6PgNBNNzDkKdhkT6uJntUXwwzQV8Rr2tZcbkDcuC9DZRsS6AtHts4Ypo1j");
    BC_REQUIRE_DECRYPT_SECRET(key, "MOLON LABE");
    BOOST_REQUIRE_EQUAL(encode_base16(out_secret), "44ea95afbf138356a05ea32110dfd627232d0f2991ad221187be356f19fa8190");
    BOOST_REQUIRE_EQUAL(out_version, expected_version);
    BOOST_REQUIRE(!out_is_compressed);
}

// github.com/bitcoin/bips/blob/master/bip-0038.mediawiki#ec-multiply-no-compression-lotsequence-numbers
BOOST_AUTO_TEST_CASE(encrypted__decrypt_private__vector_7_multiplied_lot__expected)
{
    const uint8_t expected_version = 0x00;
    const auto key = base58_literal("6PgGWtx25kUg8QWvwuJAgorN6k9FbE25rv5dMRwu5SKMnfpfVe5mar2ngH");
    BC_REQUIRE_DECRYPT_SECRET(key, "ΜΟΛΩΝ ΛΑΒΕ");
    BOOST_REQUIRE_EQUAL(encode_base16(out_secret), "ca2759aa4adb0f96c414f36abeb8db59342985be9fa50faac228c8e7d90e3006");
    BOOST_REQUIRE_EQUAL(out_version, expected_version);
    BOOST_REQUIRE(!out_is_compressed);
}

// generated and verified using bit2factor.com, no lot/sequence
BOOST_AUTO_TEST_CASE(encrypted__decrypt_private__vector_8_multiplied__expected)
{
    const uint8_t expected_version = 0x00;
    const auto key = base58_literal("6PfPAw5HErFdzMyBvGMwSfSWjKmzgm3jDg7RxQyVCSSBJFZLAZ6hVupmpn");
    BC_REQUIRE_DECRYPT_SECRET(key, "libbitcoin test");
    BOOST_REQUIRE_EQUAL(encode_base16(out_secret), "fb4bfb0bfe151d524b0b11983b9f826d6a0bc7f7bdc480864a1b557ff0c59eb4");
    BOOST_REQUIRE_EQUAL(out_version, expected_version);
    BOOST_REQUIRE(!out_is_compressed);
}

// generated and verified using bit2factor.com, no lot/sequence
BOOST_AUTO_TEST_CASE(encrypted__decrypt_private__vector_9_multiplied__expected)
{
    const uint8_t expected_version = 0x00;
    const auto key = base58_literal("6PfU2yS6DUHjgH8wmsJRT1rHWXRofmDV5UJ3dypocew56BDcw5TQJXFYfm");
    BC_REQUIRE_DECRYPT_SECRET(key, "Libbitcoin BIP38 Test Vector");
    BOOST_REQUIRE_EQUAL(encode_base16(out_secret), "97c745cc980e5a070e12d0bff3f539b70748aadb406045fc1b42d4ded559a564");
    BOOST_REQUIRE_EQUAL(out_version, expected_version);
    BOOST_REQUIRE(!out_is_compressed);
}

BOOST_AUTO_TEST_SUITE_END()

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(encrypted__decrypt_public)

// TODO: create compressed and altchain/testnet vector(s).

#define BC_REQUIRE_DECRYPT_POINT(key, passphrase, version) \
    ec_compressed out_point; \
    uint8_t out_version = 42; \
    bool out_is_compressed = true; \
    BOOST_REQUIRE(decrypt(out_point, out_version, out_is_compressed, key, passphrase)); \
    BOOST_REQUIRE_EQUAL(out_version, version); \
    BOOST_REQUIRE(!out_is_compressed); \
    const auto derived_address = payment_address({ out_point, out_is_compressed }, out_version).encoded()

// github.com/bitcoin/bips/blob/master/bip-0038.mediawiki#ec-multiply-no-compression-lotsequence-numbers
BOOST_AUTO_TEST_CASE(encrypted__decrypt_public__vector_6_lot__expected)
{
    const uint8_t version = 0x00;
    const auto key = base58_literal("cfrm38V8aXBn7JWA1ESmFMUn6erxeBGZGAxJPY4e36S9QWkzZKtaVqLNMgnifETYw7BPwWC9aPD");
    BC_REQUIRE_DECRYPT_POINT(key, "MOLON LABE", version);
    BOOST_REQUIRE_EQUAL(encode_base16(out_point), "03e20f3b812f630d0374eefe776e983549d5cdde87780be45cb9def7ee339dfed4");
    BOOST_REQUIRE_EQUAL(derived_address, "1Jscj8ALrYu2y9TD8NrpvDBugPedmbj4Yh");
}

// github.com/bitcoin/bips/blob/master/bip-0038.mediawiki#ec-multiply-no-compression-lotsequence-numbers
BOOST_AUTO_TEST_CASE(encrypted__decrypt_public__vector_7_lot__expected)
{
    const uint8_t version = 0x00;
    const auto key = base58_literal("cfrm38V8G4qq2ywYEFfWLD5Cc6msj9UwsG2Mj4Z6QdGJAFQpdatZLavkgRd1i4iBMdRngDqDs51");
    BC_REQUIRE_DECRYPT_POINT(key, "ΜΟΛΩΝ ΛΑΒΕ", version);
    BOOST_REQUIRE_EQUAL(encode_base16(out_point), "0215fb4e4e62fcec936920dbda69e83facfe2cc5e152fafcf474c8fa0dcf5023f3");
    BOOST_REQUIRE_EQUAL(derived_address, "1Lurmih3KruL4xDB5FmHof38yawNtP9oGf");
}

// generated and verified using bit2factor.com, no lot/sequence
BOOST_AUTO_TEST_CASE(encrypted__decrypt_public__vector_8__expected)
{
    const uint8_t version = 0x00;
    const auto key = base58_literal("cfrm38V5Nm1mn7GxPBAGTXawqXRwE1EbR19GqsvJ9JmF5VKLqi8nETmULpELkQvExCGkTNCH2An");
    BC_REQUIRE_DECRYPT_POINT(key, "libbitcoin test", version);
    BOOST_REQUIRE_EQUAL(encode_base16(out_point), "02c13b65302bbbed4f7ad67bc68e928b58e7748d84091a2d42680dc52e7916079e");
    BOOST_REQUIRE_EQUAL(derived_address, "1NQgLnFz1ZzF6KkCJ4SM3xz3Jy1q2hEEax");
}

// generated and verified using bit2factor.com, no lot/sequence
BOOST_AUTO_TEST_CASE(encrypted__decrypt_public__vector_9__expected)
{
    const uint8_t version = 0x00;
    const auto key = base58_literal("cfrm38V5ec4E5RKwBu46Jf5zfaE54nuB1NWHpHSpgX4GQqfzx7fvqm43mBHvr89pPgykDHts9VC");
    BC_REQUIRE_DECRYPT_POINT(key, "Libbitcoin BIP38 Test Vector", version);
    BOOST_REQUIRE_EQUAL(encode_base16(out_point), "02c3b28a224e38af4219cd782653250d2e4b67ed85ac342201f8f05ff909efdc52");
    BOOST_REQUIRE_EQUAL(derived_address, "1NjjvGqXDrx1DvrhjYJxzrpyopk1ygHTSJ");
}

BOOST_AUTO_TEST_SUITE_END()

#endif // WITH_ICU

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(encrypted__create_key_pair)

// TODO: create compressed vector(s).

#define BC_REQUIRE_CREATE_KEY_PAIR(token, seed, version, compressed) \
    ec_compressed out_point; \
    encrypted_private out_private; \
    BOOST_REQUIRE(create_key_pair(out_private, out_point, token, seed, version, compressed))

BOOST_AUTO_TEST_CASE(encrypted__create_key_pair__bad_checksum__false)
{
    auto compression = false;
    const uint8_t version = 0x00;
    const auto seed = base16_literal("d36d8e703d8bd5445044178f69087657fba73d9f3ff211f7");
    const auto token = base58_literal("passphraseo59BauW85etaRsKpbbTrEa5RRYw6bq5K9yrDf4r4N5fcirPdtDKmfJw9oYNoGN");
    ec_compressed out_point;
    encrypted_private out_private;
    BOOST_REQUIRE(!create_key_pair(out_private, out_point, token, seed, version, compression));
}

// generated and verified using bit2factor.com, no lot/sequence
BOOST_AUTO_TEST_CASE(encrypted__create_key_pair__vector_8__expected)
{
    auto compression = false;
    const uint8_t version = 0x00;
    const auto seed = base16_literal("d36d8e703d8bd5445044178f69087657fba73d9f3ff211f7");
    const auto token = base58_literal("passphraseo59BauW85etaRsKpbbTrEa5RRYw6bq5K9yrDf4r4N5fcirPdtDKmfJw9oYNoGM");
    BC_REQUIRE_CREATE_KEY_PAIR(token, seed, version, compression);
    BOOST_REQUIRE_EQUAL(encode_base58(out_private), "6PfPAw5HErFdzMyBvGMwSfSWjKmzgm3jDg7RxQyVCSSBJFZLAZ6hVupmpn");

    ec_uncompressed decompressed;
    BOOST_REQUIRE(decompress(decompressed, out_point));
    BOOST_REQUIRE_EQUAL(encode_base16(decompressed), "04c13b65302bbbed4f7ad67bc68e928b58e7748d84091a2d42680dc52e7916079e103bd025079e984fb4439177224e48a2d9da5768d9b886d89d22c714169723a6");
}

// generated and verified using bit2factor.com, no lot/sequence
BOOST_AUTO_TEST_CASE(encrypted__create_key_pair__vector_9__expected)
{
    auto compression = false;
    const uint8_t version = 0x00;
    const auto seed = base16_literal("bbeac8b9bb39381520b6873553544b387bcaa19112602230");
    const auto token = base58_literal("passphraseouGLY8yjTZQ5Q2bTo8rtKfdbHz4tme7QuPheRgES8KnT6pX5yxFauYhv3SVPDD");
    BC_REQUIRE_CREATE_KEY_PAIR(token, seed, version, compression);
    BOOST_REQUIRE_EQUAL(encode_base58(out_private), "6PfU2yS6DUHjgH8wmsJRT1rHWXRofmDV5UJ3dypocew56BDcw5TQJXFYfm");

    ec_uncompressed decompressed;
    BOOST_REQUIRE(decompress(decompressed, out_point));
    BOOST_REQUIRE_EQUAL(encode_base16(decompressed), "04c3b28a224e38af4219cd782653250d2e4b67ed85ac342201f8f05ff909efdc52858af96a727252a99c54e871ff7bcf9b53cb74e4da1e15d9e83625e3c91222c0");
}

// generated and verified using bit2factor.com, no lot/sequence
BOOST_AUTO_TEST_CASE(encrypted__create_key_pair__vector_9_compressed__expected)
{
    auto compression = true;
    const uint8_t version = 0x00;
    const auto seed = base16_literal("bbeac8b9bb39381520b6873553544b387bcaa19112602230");
    const auto token = base58_literal("passphraseouGLY8yjTZQ5Q2bTo8rtKfdbHz4tme7QuPheRgES8KnT6pX5yxFauYhv3SVPDD");
    BC_REQUIRE_CREATE_KEY_PAIR(token, seed, version, compression);
    BOOST_REQUIRE_EQUAL(encode_base58(out_private), "6PnQ4ihgH1pxeUWa1SDPZ4xToaTdLtjebd8Qw6KJf8xDCW67ssaAqWuJkw");
    BOOST_REQUIRE_EQUAL(encode_base16(out_point), "02c3b28a224e38af4219cd782653250d2e4b67ed85ac342201f8f05ff909efdc52");
}

// altchain vectors are based on preliminary bidirectional mapping proposal.
BOOST_AUTO_TEST_CASE(encrypted__create_key_pair__vector_9_compressed_testnet__expected)
{
    auto compression = true;
    const uint8_t version = 111;
    const auto seed = base16_literal("bbeac8b9bb39381520b6873553544b387bcaa19112602230");
    const auto token = base58_literal("passphraseouGLY8yjTZQ5Q2bTo8rtKfdbHz4tme7QuPheRgES8KnT6pX5yxFauYhv3SVPDD");
    BC_REQUIRE_CREATE_KEY_PAIR(token, seed, version, compression);
    BOOST_REQUIRE_EQUAL(encode_base58(out_private), "8FELCpEDogaLG3WkLhSVpKKravcNDZ7HAQ7jwHApt1Rn4BHqaLAfo9nrRD");
    BOOST_REQUIRE_EQUAL(encode_base16(out_point), "02c3b28a224e38af4219cd782653250d2e4b67ed85ac342201f8f05ff909efdc52");
}

BOOST_AUTO_TEST_SUITE_END()

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(encrypted__create_key_pair_with_confirmation)

// TODO: create compressed vector(s).

#define BC_REQUIRE_CREATE_KEY_PAIR_CONFIRMATION(token, seed, version, compressed) \
    ec_compressed out_point; \
    encrypted_public out_public; \
    encrypted_private out_private; \
    BOOST_REQUIRE(create_key_pair(out_private, out_public, out_point, token, seed, version, compressed))

BOOST_AUTO_TEST_CASE(encrypted__create_key_pair_with_confirmation__bad_checksum__false)
{
    const auto seed = base16_literal("d36d8e703d8bd5445044178f69087657fba73d9f3ff211f7");
    const auto token = base58_literal("passphraseo59BauW85etaRsKpbbTrEa5RRYw6bq5K9yrDf4r4N5fcirPdtDKmfJw9oYNoGN");
    ec_compressed out_point;
    encrypted_public out_public;
    encrypted_private out_private;
    BOOST_REQUIRE(!create_key_pair(out_private, out_public, out_point, token, seed, 0, false));
}

// generated and verified using bit2factor.com, no lot/sequence
BOOST_AUTO_TEST_CASE(encrypted__create_key_pair_with_confirmation__vector_8__expected)
{
    auto compression = false;
    const uint8_t version = 0x00;
    const auto seed = base16_literal("d36d8e703d8bd5445044178f69087657fba73d9f3ff211f7");
    const auto token = base58_literal("passphraseo59BauW85etaRsKpbbTrEa5RRYw6bq5K9yrDf4r4N5fcirPdtDKmfJw9oYNoGM");
    BC_REQUIRE_CREATE_KEY_PAIR_CONFIRMATION(token, seed, version, compression);
    BOOST_REQUIRE_EQUAL(encode_base58(out_private), "6PfPAw5HErFdzMyBvGMwSfSWjKmzgm3jDg7RxQyVCSSBJFZLAZ6hVupmpn");
    BOOST_REQUIRE_EQUAL(encode_base58(out_public), "cfrm38V5Nm1mn7GxPBAGTXawqXRwE1EbR19GqsvJ9JmF5VKLqi8nETmULpELkQvExCGkTNCH2An");

    ec_uncompressed decompressed;
    BOOST_REQUIRE(decompress(decompressed, out_point));
    BOOST_REQUIRE_EQUAL(encode_base16(decompressed), "04c13b65302bbbed4f7ad67bc68e928b58e7748d84091a2d42680dc52e7916079e103bd025079e984fb4439177224e48a2d9da5768d9b886d89d22c714169723a6");
}

// generated and verified using bit2factor.com, no lot/sequence
BOOST_AUTO_TEST_CASE(encrypted__create_key_pair_with_confirmation__vector_9__expected)
{
    auto compression = false;
    const uint8_t version = 0x00;
    const auto seed = base16_literal("bbeac8b9bb39381520b6873553544b387bcaa19112602230");
    const auto token = base58_literal("passphraseouGLY8yjTZQ5Q2bTo8rtKfdbHz4tme7QuPheRgES8KnT6pX5yxFauYhv3SVPDD");
    BC_REQUIRE_CREATE_KEY_PAIR_CONFIRMATION(token, seed, version, compression);
    BOOST_REQUIRE_EQUAL(encode_base58(out_private), "6PfU2yS6DUHjgH8wmsJRT1rHWXRofmDV5UJ3dypocew56BDcw5TQJXFYfm");
    BOOST_REQUIRE_EQUAL(encode_base58(out_public), "cfrm38V5ec4E5RKwBu46Jf5zfaE54nuB1NWHpHSpgX4GQqfzx7fvqm43mBHvr89pPgykDHts9VC");

    ec_uncompressed decompressed;
    BOOST_REQUIRE(decompress(decompressed, out_point));
    BOOST_REQUIRE_EQUAL(encode_base16(decompressed), "04c3b28a224e38af4219cd782653250d2e4b67ed85ac342201f8f05ff909efdc52858af96a727252a99c54e871ff7bcf9b53cb74e4da1e15d9e83625e3c91222c0");
}

// generated and verified using bit2factor.com, no lot/sequence
BOOST_AUTO_TEST_CASE(encrypted__create_key_pair_with_confirmation__vector_9_compressed__expected)
{
    auto compression = true;
    const uint8_t version = 0x00;
    const auto seed = base16_literal("bbeac8b9bb39381520b6873553544b387bcaa19112602230");
    const auto token = base58_literal("passphraseouGLY8yjTZQ5Q2bTo8rtKfdbHz4tme7QuPheRgES8KnT6pX5yxFauYhv3SVPDD");
    BC_REQUIRE_CREATE_KEY_PAIR_CONFIRMATION(token, seed, version, compression);
    BOOST_REQUIRE_EQUAL(encode_base58(out_private), "6PnQ4ihgH1pxeUWa1SDPZ4xToaTdLtjebd8Qw6KJf8xDCW67ssaAqWuJkw");
    BOOST_REQUIRE_EQUAL(encode_base58(out_public), "cfrm38VUEdzHWKfUjdNjV22wyFNGgtRHYhXdBFT7fWw7cCJbCobryAYUThq4BbTPP15g4SeBsug");
    BOOST_REQUIRE_EQUAL(encode_base16(out_point), "02c3b28a224e38af4219cd782653250d2e4b67ed85ac342201f8f05ff909efdc52");
}

// altchain vectors are based on preliminary bidirectional mapping proposal.
BOOST_AUTO_TEST_CASE(encrypted__create_key_pair_with_confirmation__vector_9_compressed_testnet__expected)
{
    auto compression = true;
    const uint8_t version = 111;
    const auto seed = base16_literal("bbeac8b9bb39381520b6873553544b387bcaa19112602230");
    const auto token = base58_literal("passphraseouGLY8yjTZQ5Q2bTo8rtKfdbHz4tme7QuPheRgES8KnT6pX5yxFauYhv3SVPDD");
    BC_REQUIRE_CREATE_KEY_PAIR_CONFIRMATION(token, seed, version, compression);
    BOOST_REQUIRE_EQUAL(encode_base58(out_private), "8FELCpEDogaLG3WkLhSVpKKravcNDZ7HAQ7jwHApt1Rn4BHqaLAfo9nrRD");
    BOOST_REQUIRE_EQUAL(encode_base58(out_public), "cfrm2zc77zW4FRDALEVBoKmmT79Q7KshtvLZoN62JADnXGPEcPosMx8sM8Ry4ieGW3FXUEoBwk2");
    BOOST_REQUIRE_EQUAL(encode_base16(out_point), "02c3b28a224e38af4219cd782653250d2e4b67ed85ac342201f8f05ff909efdc52");
}

BOOST_AUTO_TEST_SUITE_END()

// ----------------------------------------------------------------------------

#ifdef WITH_ICU

BOOST_AUTO_TEST_SUITE(encrypted__round_trips)

BOOST_AUTO_TEST_CASE(encrypted__encrypt__compressed_testnet__matches_secret_version_and_compression)
{
    const auto secret = base16_literal("09c2686880095b1a4c249ee3ac4eea8a014f11e6f986d0b5025ac1f39afbd9ae");
    const auto passphrase = "passphrase";

    // Encrypt the secret as a private key.
    encrypted_private out_private_key;
    const uint8_t version = 111;
    const auto is_compressed = true;
    BOOST_REQUIRE(encrypt(out_private_key, secret, passphrase, version, is_compressed));

    // Decrypt the secret from the private key.
    const auto& private_key = out_private_key;
    ec_secret out_secret;
    uint8_t out_version = 42;
    bool out_is_compressed = false;
    BOOST_REQUIRE(decrypt(out_secret, out_version, out_is_compressed, private_key, passphrase));
    BOOST_REQUIRE_EQUAL(encode_base16(out_secret), encode_base16(secret));
    BOOST_REQUIRE_EQUAL(out_is_compressed, is_compressed);
    BOOST_REQUIRE_EQUAL(out_version, version);
}

BOOST_AUTO_TEST_CASE(encrypted__create_token_entropy__private_uncompressed_testnet__decrypts_with_matching_version_and_compression)
{
    // Create the token.
    encrypted_token out_token;
    const auto passphrase = "passphrase";
    const auto entropy = base16_literal("baadf00dbaadf00d");
    BOOST_REQUIRE(create_token(out_token, passphrase, entropy));
    BOOST_REQUIRE_EQUAL(encode_base58(out_token), "passphraseqVHzjNrYRo5G6sfRB4YdSaQ2m8URnkBYS1UT6JBju5G5o45YRZKLDpK6J3PEGq");

    // Create the private key.
    const auto& token = out_token;
    ec_compressed out_point;
    encrypted_private out_private_key;
    const uint8_t version = 111;
    const auto is_compressed = false;
    const auto seed = base16_literal("baadf00dbaadf00dbaadf00dbaadf00dbaadf00dbaadf00d");
    BOOST_REQUIRE(create_key_pair(out_private_key, out_point, token, seed, version, is_compressed));

    // Extract the secret from the private key.
    const auto& private_key = out_private_key;
    ec_secret out_secret;
    uint8_t out_version = 42;
    bool out_is_compressed = true;
    BOOST_REQUIRE(decrypt(out_secret, out_version, out_is_compressed, private_key, passphrase));
    BOOST_REQUIRE_EQUAL(out_is_compressed, is_compressed);
    BOOST_REQUIRE_EQUAL(out_version, version);

    // Validate the point derived from key creation against the secret-derived point.
    // The out point is always compressed, since the user can always decompress it.
    ec_compressed compressed;
    BOOST_REQUIRE(secret_to_public(compressed, out_secret));
    BOOST_REQUIRE_EQUAL(encode_base16(out_point), encode_base16(compressed));
}

BOOST_AUTO_TEST_CASE(encrypted__create_token_lot__private_and_public_compressed_testnet__decrypts_with_matching_version_and_compression)
{
    // Create the token.
    encrypted_token out_token;
    const auto passphrase = "passphrase";
    const auto salt = base16_literal("baadf00d");
    BOOST_REQUIRE(create_token(out_token, passphrase, salt, 42, 24));
    BOOST_REQUIRE_EQUAL(encode_base58(out_token), "passphrasecpXbDpHuo8FGWnwMTnTFiHSDnqyARArE2YSFQzMHtCZvM2oWg2K3Ua2crKyc11");

    // Create the public/private key pair.
    const auto& token = out_token;
    ec_compressed out_point;
    encrypted_private out_private_key;
    encrypted_public out_public_key;
    const uint8_t version = 111;
    const auto is_compressed = true;
    const auto seed = base16_literal("baadf00dbaadf00dbaadf00dbaadf00dbaadf00dbaadf00d");
    BOOST_REQUIRE(create_key_pair(out_private_key, out_public_key, out_point, token, seed, version, is_compressed));

    // Extract the secret from the private key.
    const auto& private_key = out_private_key;
    ec_secret out_secret1;
    uint8_t out_version1 = 42;
    bool out_is_compressed1 = false;
    ec_compressed compressed;
    BOOST_REQUIRE(decrypt(out_secret1, out_version1, out_is_compressed1, private_key, passphrase));
    BOOST_REQUIRE_EQUAL(out_is_compressed1, is_compressed);
    BOOST_REQUIRE_EQUAL(out_version1, version);
    BOOST_REQUIRE(secret_to_public(compressed, out_secret1));
    BOOST_REQUIRE_EQUAL(encode_base16(out_point), encode_base16(compressed));

    // Extract the point from the public key.
    const auto& public_key = out_public_key;
    ec_compressed out_point2;
    uint8_t out_version2 = 42;
    bool out_is_compressed2 = false;
    BOOST_REQUIRE(decrypt(out_point2, out_version2, out_is_compressed2, public_key, passphrase));
    BOOST_REQUIRE_EQUAL(out_is_compressed2, is_compressed);
    BOOST_REQUIRE_EQUAL(out_version2, version);
    BOOST_REQUIRE_EQUAL(encode_base16(out_point2), encode_base16(compressed));
}

BOOST_AUTO_TEST_SUITE_END()

#endif // WITH_ICU

#endif // SLOW_TESTS

// ----------------------------------------------------------------------------

// These are not actual tests, just for emitting the version maps.

//BOOST_AUTO_TEST_SUITE(encrypted__altchain_versions)
//
//static std::string hex(uint8_t number)
//{
//    return encode_base16(data_chunk{ number });
//}
//
//BOOST_AUTO_TEST_CASE(encrypted__create_key_pair__all_versions__print_private_and_public_encrypted_keys)
//{
//    encrypted_private out_private_key;
//    const auto secret = base16_literal("09c2686880095b1a4c249ee3ac4eea8a014f11e6f986d0b5025ac1f39afbd9ae");
//
//    const auto compressed = true;
//    for (size_t version = 0x00; version <= 0xFF; ++version)
//    {
//        const auto byte = static_cast<uint8_t>(version);
//        BOOST_REQUIRE(encrypt(out_private_key, secret, "passphrase", byte, compressed));
//
//        auto key = encode_base58(out_private_key);
//        key.resize(2);
//        BOOST_TEST_MESSAGE("0x" + hex(byte) + " " + key);
//    }
//}
//
//BOOST_AUTO_TEST_CASE(encrypted__create_key_pair__all_multiplied_versions__print_private_and_public_encrypted_keys)
//{
//    encrypted_token out_token;
//    create_token(out_token, "passphrase", base16_literal("baadf00dbaadf00d"));
//
//    const auto& token = out_token;
//    ec_compressed unused;
//    encrypted_private out_private_key;
//    encrypted_public out_public_key;
//    const auto seed = base16_literal("baadf00dbaadf00dbaadf00dbaadf00dbaadf00dbaadf00d");
//
//    const auto compressed = true;
//    for (size_t version = 0x00; version <= 0xFF; ++version)
//    {
//        const auto byte = static_cast<uint8_t>(version);
//        BOOST_REQUIRE(create_key_pair(out_private_key, out_public_key, unused, token, seed, byte, compressed));
//
//        auto private_key = encode_base58(out_private_key);
//        auto public_key = encode_base58(out_public_key);
//        private_key.resize(2);
//        public_key.resize(7);
//        BOOST_TEST_MESSAGE("0x" + hex(byte) + " " + private_key + " " + public_key);
//    }
//}
//
//BOOST_AUTO_TEST_SUITE_END()

// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()
