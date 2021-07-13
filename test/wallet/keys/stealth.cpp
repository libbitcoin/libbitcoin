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

// TODO: split out individual functions and standardize test names.
BOOST_AUTO_TEST_SUITE(stealth_tests)

#define SCAN_PRIVATE "fa63521e333e4b9f6a98a142680d3aef4d8e7f79723ce0043691db55c36bd905"
#define SCAN_PUBLIC "034ea70b28d607bf3a2493102001cab35689cf2152530bf8bf8a5b594af6ae31d0"

#define SPEND_PRIVATE "dcc1250b51c0f03ae4e978e0256ede51dc1144e345c926262b9717b1bcc9bd1b"
#define SPEND_PUBLIC "03d5b3853bbee336b551ff999b0b1d656e65a7649037ae0dcb02b3c4ff5f29e5be"

#define EPHEMERAL_PRIVATE "5f70a77b32260a7a32c62242381fba2cf40c0e209e665a7959418eae4f2da22b"
#define EPHEMERAL_PUBLIC "0387ff9128d18ddcec0a8119589a62b88bc035cb9cd6db08ce5ff702a78ef8f922"

#define STEALTH_PRIVATE "280a9931c0a7b8f9bed96bad35f69a1431817fb77043fdff641ad48ce1e4411e"
#define STEALTH_PUBLIC "0305f6b99a44a2bdec8b484ffcee561cf9a0c3b7ea92ea8e6334e6fbc4f1c17899"

// $ bx ec-add 03d5b3853bbee336b551ff999b0b1d656e65a7649037ae0dcb02b3c4ff5f29e5be 4b4974266ee6c8bed9eff2cd1087bbc1101f17bad9c37814f8561b67f550c544 | bx ec-to-address
#define P2PKH_ADDRESS "1Gvq8pSTRocNLDyf858o4PL3yhZm5qQDgB"

// $ bx ec-add 03d5b3853bbee336b551ff999b0b1d656e65a7649037ae0dcb02b3c4ff5f29e5be 4b4974266ee6c8bed9eff2cd1087bbc1101f17bad9c37814f8561b67f550c544 | bx ec-to-address - v 111
// #define P2PKH_ADDRESS_TESTNET "mwSnRsXSEq3d7LTGqe7AtJYNqhATwHdhMb"

BOOST_AUTO_TEST_CASE(stealth_round_trip)
{
    ec_secret expected_stealth_private;
    BOOST_REQUIRE(decode_base16(expected_stealth_private, STEALTH_PRIVATE));

    // Receiver generates a new scan private.
    ec_secret scan_private;
    ec_compressed scan_public;
    BOOST_REQUIRE(decode_base16(scan_private, SCAN_PRIVATE));
    BOOST_REQUIRE(secret_to_public(scan_public, scan_private));
    BOOST_REQUIRE_EQUAL(encode_base16(scan_public), SCAN_PUBLIC);

    // Receiver generates a new spend private.
    ec_secret spend_private;
    ec_compressed spend_public;
    BOOST_REQUIRE(decode_base16(spend_private, SPEND_PRIVATE));
    BOOST_REQUIRE(secret_to_public(spend_public, spend_private));
    BOOST_REQUIRE_EQUAL(encode_base16(spend_public), SPEND_PUBLIC);

    // Sender generates a new ephemeral key.
    ec_secret ephemeral_private;
    ec_compressed ephemeral_public;
    BOOST_REQUIRE(decode_base16(ephemeral_private, EPHEMERAL_PRIVATE));
    BOOST_REQUIRE(secret_to_public(ephemeral_public, ephemeral_private));
    BOOST_REQUIRE_EQUAL(encode_base16(ephemeral_public), EPHEMERAL_PUBLIC);

    // Sender derives stealth public, requiring ephemeral private.
    ec_compressed sender_public;
    BOOST_REQUIRE(uncover_stealth(sender_public, scan_public, ephemeral_private, spend_public));
    BOOST_REQUIRE_EQUAL(encode_base16(sender_public), STEALTH_PUBLIC);

    // Receiver derives stealth public, requiring scan private.
    ec_compressed receiver_public;
    BOOST_REQUIRE(uncover_stealth(receiver_public, ephemeral_public, scan_private, spend_public));
    BOOST_REQUIRE_EQUAL(encode_base16(receiver_public), STEALTH_PUBLIC);

    // Only reciever can derive stealth private, as it requires both scan and spend private.
    ec_secret stealth_private;
    BOOST_REQUIRE(uncover_stealth(stealth_private, ephemeral_public, scan_private, spend_private));

    // This shows that both parties have actually generated stealth public.
    ec_compressed stealth_public;
    BOOST_REQUIRE(secret_to_public(stealth_public, stealth_private));
    BOOST_REQUIRE_EQUAL(encode_base16(stealth_public), STEALTH_PUBLIC);

    // Both parties therefore have the ability to generate the p2pkh address.
    // versioning: stealth_address::main corresponds to payment_address::main_p2pkh
    wallet::payment_address address(stealth_public, wallet::payment_address::mainnet_p2kh);
    BOOST_REQUIRE_EQUAL(address.encoded(), P2PKH_ADDRESS);
}

BOOST_AUTO_TEST_CASE(verify_string_constructor)
{
    const std::string value = "01100110000";
    binary prefix(value);
    BOOST_REQUIRE_EQUAL(value.size(), prefix.bits());
    for (unsigned i = 0; i < value.size(); ++i)
    {
        const auto comparison = value[i] == '1';
        BOOST_REQUIRE_EQUAL(prefix[i], comparison);
    }
}

// Binary as a value on the left, padded with zeros to the right.
BOOST_AUTO_TEST_CASE(compare_constructor_results)
{
    std::string value = "01100111000";
    binary prefix(value);
    data_chunk blocks{ { 0x67, 0x00 } };
    binary prefix2(static_cast<unsigned>(value.size()), blocks);
    BOOST_REQUIRE_EQUAL(prefix, prefix2);
}

BOOST_AUTO_TEST_CASE(bitfield_test1)
{
    binary prefix("01100111001");
    data_chunk raw_bitfield{ { 0x67, 0x20, 0x00, 0x0 } };
    BOOST_REQUIRE_GE(raw_bitfield.size() * 8, prefix.bits());
    binary compare(prefix.bits(), raw_bitfield);
    BOOST_REQUIRE_EQUAL(prefix, compare);
}

BOOST_AUTO_TEST_CASE(bitfield_test2)
{
    const data_chunk blocks{ { 0x8b, 0xf4, 0x1c, 0x69 } };
    const binary prefix(27, blocks);
    const data_chunk raw_bitfield{ { 0x8b, 0xf4, 0x1c, 0x79 } };
    BOOST_REQUIRE_GE(raw_bitfield.size() * 8, prefix.bits());
    const binary compare(prefix.bits(), raw_bitfield);
    BOOST_REQUIRE_EQUAL(prefix, compare);
}

BOOST_AUTO_TEST_CASE(bitfield_test3)
{
    const data_chunk blocks{ { 0x69, 0x1c, 0xf4, 0x8b } };
    const binary prefix(32, blocks);
    const data_chunk raw_bitfield{ { 0x69, 0x1c, 0xf4, 0x8b } };
    const binary compare(prefix.bits(), raw_bitfield);
    BOOST_REQUIRE_EQUAL(prefix, compare);
}

BOOST_AUTO_TEST_CASE(bitfield_test4)
{
    const data_chunk blocks{ { 0x69, 0x1c, 0xf4, 0x8b } };
    const binary prefix(29, blocks);
    const data_chunk raw_bitfield{ { 0x69, 0x1c, 0xf4, 0x8b } };
    const binary compare(prefix.bits(), raw_bitfield);
    BOOST_REQUIRE_EQUAL(prefix, compare);
}

BOOST_AUTO_TEST_SUITE_END()
