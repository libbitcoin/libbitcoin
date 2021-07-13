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

BOOST_AUTO_TEST_SUITE(elliptic_curve_tests)

// These vectors use hash encoding for sighash values.

// scenario 1
const ec_secret secret1 = base16_literal("8010b1bb119ad37d4b65a1022a314897b1b3614b345974332cb1b9582cf03536");
const ec_compressed compressed1 = base16_literal("0309ba8621aefd3b6ba4ca6d11a4746e8df8d35d9b51b383338f627ba7fc732731");
const ec_uncompressed uncompressed1 = base16_literal("0409ba8621aefd3b6ba4ca6d11a4746e8df8d35d9b51b383338f627ba7fc7327318c3a6ec6acd33c36328b8fb4349b31671bcd3a192316ea4f6236ee1ae4a7d8c9");

// scenario 2
const ec_compressed compressed2 = base16_literal("03bc88a1bd6ebac38e9a9ed58eda735352ad10650e235499b7318315cc26c9b55b");
const hash_digest sighash2 = hash_literal("ed8f9b40c2d349c8a7e58cebe79faa25c21b6bb85b874901f72a1b3f1ad0a67f");
const der_signature der_signature2 = base16_chunk("3045022100bc494fbd09a8e77d8266e2abdea9aef08b9e71b451c7d8de9f63cda33a62437802206b93edd6af7c659db42c579eb34a3a4cb60c28b5a6bc86fd5266d42f6b8bb67d");

// scenario 3
const ec_secret secret3 = base16_literal("33436393f770d9b3f5d11c20be561837300f89515284008965d2fd3f714b8fce");
const hash_digest sighash3 = hash_literal("f89572635651b2e4f89778350616989183c98d1a721c911324bf9f17a0cf5bf0");
const ec_signature signature3 = base16_literal("4832febef8b31c7c922a15cb4063a43ab69b099bba765e24facef50dfbb4d057928ed5c6b6886562c2fe6972fd7c7f462e557129067542cce6b37d72e5ea5037");
const der_signature der_signature3 = base16_chunk("3044022057d0b4fb0df5cefa245e76ba9b099bb63aa46340cb152a927c1cb3f8befe324802203750eae5727db3e6cc4275062971552e467f7cfd7269fec2626588b6c6d58e92");

// generator
const ec_secret one = base16_literal("0000000000000000000000000000000000000000000000000000000000000001");
const ec_compressed generator_point_times_4 = base16_literal("02e493dbf1c10d80f3581e4904930b1404cc6c13900ee0758474fa94abe8c4cd13");

// constants

BOOST_AUTO_TEST_CASE(elliptic_curve__generator__expected)
{
    ec_compressed generator;
    BOOST_REQUIRE(secret_to_public(generator, one));
    BOOST_REQUIRE_EQUAL(generator, ec_compressed_generator);
}

// conversion

BOOST_AUTO_TEST_CASE(elliptic_curve__compress__round_trip__expected)
{
    ec_uncompressed uncompressed;
    BOOST_REQUIRE(decompress(uncompressed, compressed1));

    ec_compressed compressed;
    BOOST_REQUIRE(compress(compressed, uncompressed));
    BOOST_REQUIRE_EQUAL(compressed, compressed1);
}

BOOST_AUTO_TEST_CASE(elliptic_curve__decompress__positive__expected)
{
    ec_uncompressed point;
    BOOST_REQUIRE(decompress(point, compressed1));
    BOOST_REQUIRE_EQUAL(point, uncompressed1);
}

BOOST_AUTO_TEST_CASE(elliptic_curve__secret_to_public__compressed_positive__expected)
{
    ec_compressed point;
    BOOST_REQUIRE(secret_to_public(point, secret1));
    BOOST_REQUIRE_EQUAL(point, compressed1);
}

BOOST_AUTO_TEST_CASE(elliptic_curve__secret_to_public__uncompressed_positive__expected)
{
    ec_uncompressed point;
    BOOST_REQUIRE(secret_to_public(point, secret1));
    BOOST_REQUIRE_EQUAL(point, uncompressed1);
}

// signature

BOOST_AUTO_TEST_CASE(elliptic_curve__sign__positive__expected)
{
    ec_signature signature;
    BOOST_REQUIRE(sign(signature, secret3, sighash3));
    BOOST_REQUIRE_EQUAL(signature, signature3);
}

BOOST_AUTO_TEST_CASE(elliptic_curve__encode_signature__positive__expected)
{
    der_signature signature;
    BOOST_REQUIRE(encode_signature(signature, signature3));
    BOOST_REQUIRE_EQUAL(signature, der_signature3);
}

BOOST_AUTO_TEST_CASE(elliptic_curve__sign__round_trip_positive__expected)
{
    ec_compressed point;
    BOOST_REQUIRE(secret_to_public(point, secret1));

    const auto hash = bitcoin_hash(to_chunk("data"));

    ec_signature signature;
    BOOST_REQUIRE(sign(signature, secret1, hash));
    BOOST_REQUIRE(verify_signature(point, hash, signature));
}

BOOST_AUTO_TEST_CASE(elliptic_curve__sign__round_trip_negative__expected)
{
    ec_compressed point;
    BOOST_REQUIRE(secret_to_public(point, secret1));

    auto hash = bitcoin_hash(to_chunk("data"));

    ec_signature signature;
    BOOST_REQUIRE(sign(signature, secret1, hash));

    // Invalidate the positive test.
    hash[0] = 0;

    BOOST_REQUIRE(!verify_signature(point, hash, signature));
}

BOOST_AUTO_TEST_CASE(elliptic_curve__verify_signature__positive__expected)
{
    ec_signature signature;
    BOOST_REQUIRE(parse_signature(signature, der_signature2, false));
    BOOST_REQUIRE(verify_signature(compressed2, sighash2, signature));
}

BOOST_AUTO_TEST_CASE(elliptic_curve__verify_signature__negative__expected)
{
    ec_signature signature;
    BOOST_REQUIRE(parse_signature(signature, der_signature2, false));

    // Invalidate the positive test.
    signature[10] = 110;
    BOOST_REQUIRE(!verify_signature(compressed2, sighash2, signature));
}

// addition

BOOST_AUTO_TEST_CASE(elliptic_curve__ec_add__positive__expected)
{
    const auto sum = base16_literal("0404040000000000000000000000000000000000000000000000000000000000");

    ec_secret secret1{ { 1, 2, 3 } };
    ec_compressed public1;
    BOOST_REQUIRE(secret_to_public(public1, secret1));

    const ec_secret secret2{ { 3, 2, 1 } };
    BOOST_REQUIRE(ec_add(secret1, secret2));
    BOOST_REQUIRE_EQUAL(secret1, sum);

    ec_compressed public2;
    BOOST_REQUIRE(secret_to_public(public2, secret1));
    BOOST_REQUIRE(ec_add(public1, secret2));
    BOOST_REQUIRE_EQUAL(public1, public2);
}

BOOST_AUTO_TEST_CASE(elliptic_curve__ec_add__negative__expected)
{
    // = n - 1
    auto secret1 = base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140");
    ec_secret secret2{ { 0 } };
    secret2[31] = 1;
    ec_compressed public1;
    BOOST_REQUIRE(secret_to_public(public1, secret1));
    BOOST_REQUIRE(!ec_add(secret1, secret2));
    BOOST_REQUIRE(!ec_add(public1, secret2));
}

BOOST_AUTO_TEST_CASE(elliptic_curve__ec_sum__expected)
{
    const compressed_list points
    {
        ec_compressed_generator,
        ec_compressed_generator,
        ec_compressed_generator,
        ec_compressed_generator
    };

    ec_compressed out;
    BOOST_REQUIRE(ec_sum(out, points));
    BOOST_REQUIRE_EQUAL(out, generator_point_times_4);
}

// multiplication

BOOST_AUTO_TEST_CASE(elliptic_curve__ec_multiply__expected)
{
    ec_secret secret1{ {0} };
    ec_secret secret2{ {0} };
    secret1[31] = 11;
    secret2[31] = 22;
    ec_compressed public1;
    BOOST_REQUIRE(secret_to_public(public1, secret1));
    BOOST_REQUIRE(ec_multiply(secret1, secret2));
    BOOST_REQUIRE_EQUAL(secret1[31], 242u);
    BOOST_REQUIRE(ec_multiply(public1, secret2));

    ec_compressed public2;
    BOOST_REQUIRE(secret_to_public(public2, secret1));
    BOOST_REQUIRE_EQUAL(public1, public2);
}

BOOST_AUTO_TEST_SUITE_END()
