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
#include "../test.hpp"
#include <cstdint>

BOOST_AUTO_TEST_SUITE(checksum_tests)

// BIP173: All examples use public key:
const uint8_t bip173_program_version { 0 };
const std::string bip173_mainnet_prefix{ "bc" };
const std::string bip173_testnet_prefix{ "tb" };
const auto bip173_mainnet_p2wkh = "qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t4";
const auto bip173_mainnet_p2wsh = "qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3qccfmv3";
const auto bip173_testnet_p2wkh = "qw508d6qejxtdg4y5r3zarvary0c5xw7kxpjzsx";
const auto bip173_testnet_p2wsh = "qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3q0sl5k7";

// BIP173: All examples use public key:
const auto bip173_ec_compressed = base16_literal("0279BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798");

// BIP141: The HASH160 of the pubkey in witness must match the 20 byte witness program.
const auto bip173_p2wkh_program = ripemd160_hash_chunk(sha256_hash(bip173_ec_compressed));

// BIP173: The P2WSH examples use key OP_CHECKSIG (to_pay_public_key_pattern) as script.
const auto bip173_p2wsh_opcodes = chain::script::to_pay_public_key_pattern(bip173_ec_compressed);

// BIP141: The SHA256 of the script must match the 32 byte witness program.
const auto bip173_p2wsh_program = sha256_hash_chunk(chain::script(bip173_p2wsh_opcodes).to_data(false));

// insert_checksum

BOOST_AUTO_TEST_CASE(checksum__insert_checksum__empty__expected)
{
    data_array<checksum_default_size> data;
    insert_checksum(data);
    BOOST_REQUIRE_EQUAL(data[0], 0x5du);
    BOOST_REQUIRE_EQUAL(data[1], 0xf6u);
    BOOST_REQUIRE_EQUAL(data[2], 0xe0u);
    BOOST_REQUIRE_EQUAL(data[3], 0xe2u);
}

BOOST_AUTO_TEST_CASE(checksum__insert_checksum__not_empty__expected)
{
    data_array<5 + checksum_default_size> data{ 0, 0, 0, 0, 0 };
    insert_checksum(data);
    BOOST_REQUIRE_EQUAL(data[0], 0u);
    BOOST_REQUIRE_EQUAL(data[1], 0u);
    BOOST_REQUIRE_EQUAL(data[2], 0u);
    BOOST_REQUIRE_EQUAL(data[3], 0u);
    BOOST_REQUIRE_EQUAL(data[4], 0u);
    BOOST_REQUIRE_EQUAL(data[5], 0x79u);
    BOOST_REQUIRE_EQUAL(data[6], 0x01u);
    BOOST_REQUIRE_EQUAL(data[7], 0xafu);
    BOOST_REQUIRE_EQUAL(data[8], 0x93u);
}

BOOST_AUTO_TEST_CASE(checksum__insert_checksum_data_loaf__empty__expected)
{
    data_array<0> data;
    const auto out = insert_checksum<checksum_default_size>({ { data } });
    BOOST_REQUIRE_EQUAL(out[0], 0x5du);
    BOOST_REQUIRE_EQUAL(out[1], 0xf6u);
    BOOST_REQUIRE_EQUAL(out[2], 0xe0u);
    BOOST_REQUIRE_EQUAL(out[3], 0xe2u);
}

BOOST_AUTO_TEST_CASE(checksum__insert_checksum_data_loaf__not_empty__expected)
{
    data_array<5 + checksum_default_size> data{ { 0, 0, 0, 0, 0 } };
    const auto out = insert_checksum<5 + checksum_default_size>({ { data } });
    BOOST_REQUIRE_EQUAL(out[0], 0u);
    BOOST_REQUIRE_EQUAL(out[1], 0u);
    BOOST_REQUIRE_EQUAL(out[2], 0u);
    BOOST_REQUIRE_EQUAL(out[3], 0u);
    BOOST_REQUIRE_EQUAL(out[4], 0u);
    BOOST_REQUIRE_EQUAL(out[5], 0x79u);
    BOOST_REQUIRE_EQUAL(out[6], 0x01u);
    BOOST_REQUIRE_EQUAL(out[7], 0xafu);
    BOOST_REQUIRE_EQUAL(out[8], 0x93u);
}

BOOST_AUTO_TEST_CASE(checksum__insert_checksum_data_loaf__maximum_checksum__expected)
{
    constexpr size_t checksum_maximum = 32;
    data_array<5 + checksum_maximum> data{ { 0, 0, 0, 0, 0 } };
    const auto out = insert_checksum<5 + checksum_maximum, checksum_maximum>({ { data } });
    BOOST_REQUIRE_EQUAL(out[0], 0u);
    BOOST_REQUIRE_EQUAL(out[1], 0u);
    BOOST_REQUIRE_EQUAL(out[2], 0u);
    BOOST_REQUIRE_EQUAL(out[3], 0u);
    BOOST_REQUIRE_EQUAL(out[4], 0u);
    BOOST_REQUIRE_EQUAL(out[5], 0x79u);
    BOOST_REQUIRE_EQUAL(out[36], 0xf3);
}

// append_checksum

BOOST_AUTO_TEST_CASE(checksum__append_checksum__empty__expected_size)
{
    data_chunk data{};
    append_checksum(data);
    BOOST_REQUIRE_EQUAL(data.size(), sizeof(uint32_t));
}

BOOST_AUTO_TEST_CASE(checksum__append_checksum__not_empty__expected_size)
{
    data_chunk data{ 0, 0, 0, 0, 0 };
    const auto size = data.size();
    append_checksum(data);
    BOOST_REQUIRE_EQUAL(data.size(), size + sizeof(uint32_t));
}

BOOST_AUTO_TEST_CASE(checksum__append_checksum__empty__expected)
{
    data_chunk out;
    append_checksum(out);
    BOOST_REQUIRE_EQUAL(out[0], 0x5du);
    BOOST_REQUIRE_EQUAL(out[1], 0xf6u);
    BOOST_REQUIRE_EQUAL(out[2], 0xe0u);
    BOOST_REQUIRE_EQUAL(out[3], 0xe2u);
}

BOOST_AUTO_TEST_CASE(checksum__append_checksum__zeros__expected)
{
    data_chunk out{ 0, 0, 0, 0, 0 };
    append_checksum(out);
    BOOST_REQUIRE_EQUAL(out[0], 0u);
    BOOST_REQUIRE_EQUAL(out[1], 0u);
    BOOST_REQUIRE_EQUAL(out[2], 0u);
    BOOST_REQUIRE_EQUAL(out[3], 0u);
    BOOST_REQUIRE_EQUAL(out[4], 0u);
    BOOST_REQUIRE_EQUAL(out[5], 0x79u);
    BOOST_REQUIRE_EQUAL(out[6], 0x01u);
    BOOST_REQUIRE_EQUAL(out[7], 0xafu);
    BOOST_REQUIRE_EQUAL(out[8], 0x93u);
}

BOOST_AUTO_TEST_CASE(checksum__append_checksum_data_loaf__zeros__expected)
{
    data_chunk data1{ 0, 0 };
    data_chunk data2{ 0, 0, 0 };
    const auto value = append_checksum({ data1 , data2 });
    BOOST_REQUIRE_EQUAL(value[0], 0u);
    BOOST_REQUIRE_EQUAL(value[1], 0u);
    BOOST_REQUIRE_EQUAL(value[2], 0u);
    BOOST_REQUIRE_EQUAL(value[3], 0u);
    BOOST_REQUIRE_EQUAL(value[4], 0u);
    BOOST_REQUIRE_EQUAL(value[5], 0x79u);
    BOOST_REQUIRE_EQUAL(value[6], 0x01u);
    BOOST_REQUIRE_EQUAL(value[7], 0xafu);
    BOOST_REQUIRE_EQUAL(value[8], 0x93u);
}

// verify_checksum<>

BOOST_AUTO_TEST_CASE(checksum__verify_checksum_array__round_trip__true)
{
    data_array<3 + checksum_default_size> data{ 0, 0, 0 };
    insert_checksum(data);
    BOOST_REQUIRE(verify_checksum(data));
}

BOOST_AUTO_TEST_CASE(checksum__verify_checksum_array__invalidated__false)
{
    data_array<5 + checksum_default_size> data{ 0, 0, 0, 0, 0 };
    insert_checksum(data);
    data[0] = 42;
    BOOST_REQUIRE(!verify_checksum(data));
}

// verify_checksum

BOOST_AUTO_TEST_CASE(checksum__verify_checksum_slice__underflow__false)
{
    static const data_chunk data{ 0, 0, 0 };
    BOOST_REQUIRE(!verify_checksum(data));
}

BOOST_AUTO_TEST_CASE(checksum__verify_checksum_slice__not_set__false)
{
    static const data_chunk data{ 0, 0, 0, 0, 0 };
    BOOST_REQUIRE(!verify_checksum(data));
}

BOOST_AUTO_TEST_CASE(checksum__verify_checksum_slice__round_trip__true)
{
    data_chunk data{ 0, 0, 0, 0, 0 };
    append_checksum(data);
    BOOST_REQUIRE(verify_checksum(data));
}

BOOST_AUTO_TEST_CASE(checksum__verify_checksum_slice__invalidated__false)
{
    data_chunk data{ 0, 0, 0, 0, 0 };
    append_checksum(data);
    data[0] = 42;
    BOOST_REQUIRE(!verify_checksum(data));
}

// bech32_build_checked

BOOST_AUTO_TEST_CASE(checksum__bech32_build_checked__version_one_empty__expected_size)
{
    const auto checked = bech32_build_checked(1, {}, "");
    BOOST_REQUIRE_EQUAL(checked.size(), 1 + 0 + 6);
    BOOST_REQUIRE_EQUAL(encode_base32(checked), "p2gdwpf");
}

BOOST_AUTO_TEST_CASE(checksum__bech32_build_checked__version_overflow__empty)
{
    BOOST_REQUIRE(bech32_build_checked(32, {}, "").empty());
}

BOOST_AUTO_TEST_CASE(checksum__bech32_build_checked__prefix_empty_payload__size_same_as_empty_prefix)
{
    const auto checked = bech32_build_checked(0, {}, "abcdef");
    BOOST_REQUIRE_EQUAL(checked.size(), 1 + 0 + 6);
    BOOST_REQUIRE_EQUAL(encode_base32(checked), "qgfl9ah");
}

BOOST_AUTO_TEST_CASE(checksum__bech32_build_checked__five_program_bytes__expected_size)
{
    const data_chunk program{ 1, 2, 3, 4, 5 };
    const auto checked = bech32_build_checked(0, program, "");
    BOOST_REQUIRE_EQUAL(checked.size(), 1 + (program.size() * 8) / 5 + 6);
    BOOST_REQUIRE_EQUAL(encode_base32(checked), "qqypqxpq939vyak");
}

// bech32_build_checked - BIP173

BOOST_AUTO_TEST_CASE(checksum__bech32_build_checked__mainnet_p2wkh__expected)
{
    const auto checked = bech32_build_checked(bip173_program_version, bip173_p2wkh_program, bip173_mainnet_prefix);
    BOOST_REQUIRE_EQUAL(encode_base32(checked), bip173_mainnet_p2wkh);
}

BOOST_AUTO_TEST_CASE(checksum__bech32_build_checked__testnet_p2wkh__expected)
{
    const auto checked = bech32_build_checked(bip173_program_version, bip173_p2wkh_program, bip173_testnet_prefix);
    BOOST_REQUIRE_EQUAL(encode_base32(checked), bip173_testnet_p2wkh);
}

BOOST_AUTO_TEST_CASE(checksum__bech32_build_checked__mainnet_p2wsh__expected)
{
    const auto checked = bech32_build_checked(bip173_program_version, bip173_p2wsh_program, bip173_mainnet_prefix);
    BOOST_REQUIRE_EQUAL(encode_base32(checked), bip173_mainnet_p2wsh);
}

BOOST_AUTO_TEST_CASE(checksum__bech32_build_checked__testnet_p2wsh__expected)
{
    const auto checked = bech32_build_checked(bip173_program_version, bip173_p2wsh_program, bip173_testnet_prefix);
    BOOST_REQUIRE_EQUAL(encode_base32(checked), bip173_testnet_p2wsh);
}

// bech32_verify_checked

base32_chunk checked;
uint8_t out_version;
data_chunk out_program;

BOOST_AUTO_TEST_CASE(checksum__bech32_verify_checked__version_one_empty__round_trips)
{
    const auto checked = bech32_build_checked(1, {}, "");
    BOOST_REQUIRE(bech32_verify_checked(out_version, out_program, "", checked));
    BOOST_REQUIRE_EQUAL(out_version, 1u);
    BOOST_REQUIRE(out_program.empty());
}

BOOST_AUTO_TEST_CASE(checksum__bech32_verify_checked__prefix_empty_payload__round_trips)
{
    const auto checked = bech32_build_checked(0, {}, "abcdef");
    BOOST_REQUIRE(bech32_verify_checked(out_version, out_program, "abcdef", checked));
    BOOST_REQUIRE_EQUAL(out_version, 0u);
    BOOST_REQUIRE(out_program.empty());
}

BOOST_AUTO_TEST_CASE(checksum__bech32_verify_checked__five_program_bytes__round_trips)
{
    const data_chunk program{ 1, 2, 3, 4, 5 };
    const auto checked = bech32_build_checked(0, program, "");
    BOOST_REQUIRE(bech32_verify_checked(out_version, out_program, "", checked));
    BOOST_REQUIRE_EQUAL(out_version, 0u);
    BOOST_REQUIRE_EQUAL(out_program, program);
}

// bech32_verify_checked - BIP173

BOOST_AUTO_TEST_CASE(checksum__bech32_verify_checked__mainnet_p2wkh__true_expected_version_and_program)
{
    BOOST_REQUIRE(decode_base32(checked, bip173_mainnet_p2wkh));
    BOOST_REQUIRE(bech32_verify_checked(out_version, out_program, bip173_mainnet_prefix, checked));
    BOOST_REQUIRE_EQUAL(out_version, bip173_program_version);
    BOOST_REQUIRE_EQUAL(out_program, bip173_p2wkh_program);
}

BOOST_AUTO_TEST_CASE(checksum__bech32_verify_checked__testnet_p2wkh__true_expected_version_and_program)
{
    BOOST_REQUIRE(decode_base32(checked, bip173_testnet_p2wkh));
    BOOST_REQUIRE(bech32_verify_checked(out_version, out_program, bip173_testnet_prefix, checked));
    BOOST_REQUIRE_EQUAL(out_version, bip173_program_version);
    BOOST_REQUIRE_EQUAL(out_program, bip173_p2wkh_program);
}

BOOST_AUTO_TEST_CASE(checksum__bech32_verify_checked__mainnet_p2wsh__true_expected_version_and_program)
{
    BOOST_REQUIRE(decode_base32(checked, bip173_mainnet_p2wsh));
    BOOST_REQUIRE(bech32_verify_checked(out_version, out_program, bip173_mainnet_prefix, checked));
    BOOST_REQUIRE_EQUAL(out_version, bip173_program_version);
    BOOST_REQUIRE_EQUAL(out_program, bip173_p2wsh_program);
}

BOOST_AUTO_TEST_CASE(checksum__bech32_verify_checked__testnet_p2wsh__true_expected_version_and_program)
{
    BOOST_REQUIRE(decode_base32(checked, bip173_testnet_p2wsh));
    BOOST_REQUIRE(bech32_verify_checked(out_version, out_program, bip173_testnet_prefix, checked));
    BOOST_REQUIRE_EQUAL(out_version, bip173_program_version);
    BOOST_REQUIRE_EQUAL(out_program, bip173_p2wsh_program);
}

BOOST_AUTO_TEST_CASE(checksum__bech32_verify_checked__empty__false)
{
    BOOST_REQUIRE(!bech32_verify_checked(out_version, out_program, bip173_testnet_prefix, {}));
}

BOOST_AUTO_TEST_CASE(checksum__bech32_verify_checked__mismatched_prefix__false)
{
    BOOST_REQUIRE(decode_base32(checked, bip173_testnet_p2wsh));
    BOOST_REQUIRE(!bech32_verify_checked(out_version, out_program, bip173_mainnet_prefix, checked));
}

BOOST_AUTO_TEST_CASE(checksum__bech32_verify_checked__invalid_checksum__false)
{
    BOOST_REQUIRE(decode_base32(checked, bip173_testnet_p2wsh));

    // Invalidate checksum.
    checked[checked.size() - 1] = 31;
    BOOST_REQUIRE(!bech32_verify_checked(out_version, out_program, bip173_testnet_prefix, checked));
}

BOOST_AUTO_TEST_SUITE_END()
