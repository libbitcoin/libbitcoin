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

#include <iterator>
#include <sstream>
#include <string>
#include <utility>

BOOST_AUTO_TEST_SUITE(witness_address_tests)

using namespace chain;
using namespace wallet;

using result = witness_address::parse_result;
using type = witness_address::program_type;

std::string out1;
uint8_t out2;
data_chunk out3;

// construct

BOOST_AUTO_TEST_CASE(witness_address__default_construct__always__invalid)
{
    BOOST_REQUIRE(!witness_address());
}

BOOST_AUTO_TEST_CASE(witness_address__move_construct__null_hash__valid)
{
    auto address = witness_address(null_hash);
    BOOST_REQUIRE(witness_address(std::move(address)));
}

BOOST_AUTO_TEST_CASE(witness_address__copy_construct__null_hash__valid)
{
    const auto address = witness_address(null_hash);
    BOOST_REQUIRE(witness_address(address));
}

BOOST_AUTO_TEST_CASE(witness_address__string_construct__empty__invalid)
{
    BOOST_REQUIRE(!witness_address(""));
}

BOOST_AUTO_TEST_CASE(witness_address__string_construct__invalid_version_0__invalid)
{
    BOOST_REQUIRE(!witness_address("split1checkupstagehandshakeupstreamerranterredcaperred2y9e3w"));
}

BOOST_AUTO_TEST_CASE(witness_address__string_construct__invalid_version_0_lax__invalid)
{
    BOOST_REQUIRE(!witness_address("split1checkupstagehandshakeupstreamerranterredcaperred2y9e3w", false));
}

BOOST_AUTO_TEST_CASE(witness_address__string_construct__valid_version_0_strict__valid)
{
    BOOST_REQUIRE(witness_address("tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3q0sl5k7"));
}

BOOST_AUTO_TEST_CASE(witness_address__parameter_construct__minimal_program_abc_17__invalid)
{
    BOOST_REQUIRE(!witness_address(data_chunk(2, 0x00), "abc", 17));
}

BOOST_AUTO_TEST_CASE(witness_address__parameter_construct__minimal_program_abc_16__valid)
{
    BOOST_REQUIRE(witness_address(data_chunk(2, 0x00), "abc", 16));
}

BOOST_AUTO_TEST_CASE(witness_address__parameter_construct__maximal_program_abc_16__valid)
{
    BOOST_REQUIRE(witness_address(data_chunk(40, 0x00), "abc", 1));
}

BOOST_AUTO_TEST_CASE(witness_address__parameter_construct__oversized_program_abc_16__valid)
{
    BOOST_REQUIRE(!witness_address(data_chunk(40 + 1, 0x00), "abc", 1));
}

BOOST_AUTO_TEST_CASE(witness_address__parameter_construct__minimal_program_abc_1__valid)
{
    BOOST_REQUIRE(witness_address(data_chunk(2, 0x00), "abc", 1));
}

BOOST_AUTO_TEST_CASE(witness_address__parameter_construct__minimal_program_abc_0__invalid)
{
    BOOST_REQUIRE(!witness_address(data_chunk(2, 0x00), "abc", 0));
}

BOOST_AUTO_TEST_CASE(witness_address__parameter_construct__null_short_hash_abc_0__valid)
{
    BOOST_REQUIRE(witness_address(null_short_hash, "abc", 0));
}

BOOST_AUTO_TEST_CASE(witness_address__parameter_construct__null_short_hash_invalid_prefix_0__invalid)
{
    BOOST_REQUIRE(!witness_address(null_short_hash, "\x80", 0));
}

BOOST_AUTO_TEST_CASE(witness_address__short_hash_construct__null_short_hash__valid)
{
    BOOST_REQUIRE(witness_address(null_short_hash));
}

BOOST_AUTO_TEST_CASE(witness_address__parameter_construct__null_hash_abc_0__valid)
{
    BOOST_REQUIRE(witness_address(null_hash, "abc", 0));
}

BOOST_AUTO_TEST_CASE(witness_address__parameter_construct__null_hash_invalid_prefix_0__invalid)
{
    BOOST_REQUIRE(!witness_address(null_hash, "\x80", 0));
}

BOOST_AUTO_TEST_CASE(witness_address__parameter_construct__null_hash__valid)
{
    BOOST_REQUIRE(witness_address(null_hash));
}

BOOST_AUTO_TEST_CASE(witness_address__ec_private_construct__uninitialized__invalid)
{
    BOOST_REQUIRE(!witness_address(ec_private()));
}

BOOST_AUTO_TEST_CASE(witness_address__ec_private_construct__initialized__valid)
{
    BOOST_REQUIRE(witness_address(ec_private("L1WepftUBemj6H4XQovkiW1ARVjxMqaw4oj2kmkYqdG1xTnBcHfC")));
}

BOOST_AUTO_TEST_CASE(witness_address__ec_public_construct__uninitialized__invalid)
{
    BOOST_REQUIRE(!witness_address(ec_public()));
}

BOOST_AUTO_TEST_CASE(witness_address__ec_public_construct__initialized__valid)
{
    BOOST_REQUIRE(witness_address(ec_public(ec_private("L1WepftUBemj6H4XQovkiW1ARVjxMqaw4oj2kmkYqdG1xTnBcHfC"))));
}

BOOST_AUTO_TEST_CASE(witness_address__hash_digest_construct__null_hash__valid)
{
    BOOST_REQUIRE(witness_address(null_hash));
}

BOOST_AUTO_TEST_CASE(witness_address__script_construct__uninitialized__invalid)
{
    BOOST_REQUIRE(!witness_address(script()));
}

BOOST_AUTO_TEST_CASE(witness_address__script_construct__initialized__valid)
{
    BOOST_REQUIRE(witness_address(script(script::to_pay_null_data_pattern(null_hash))));
}

// parse_prefix

BOOST_AUTO_TEST_CASE(witness_address__parse_prefix__not_ascii__prefix_not_ascii)
{
    BOOST_REQUIRE(witness_address::parse_prefix("\x80") == result::prefix_not_ascii);
    BOOST_REQUIRE(witness_address::parse_prefix("\xff") == result::prefix_not_ascii);
}

BOOST_AUTO_TEST_CASE(witness_address__parse_prefix__upper_case__prefix_not_lower_case)
{
    BOOST_REQUIRE(witness_address::parse_prefix("A") == result::prefix_not_lower_case);
    BOOST_REQUIRE(witness_address::parse_prefix("aBc") == result::prefix_not_lower_case);
}

BOOST_AUTO_TEST_CASE(witness_address__parse_prefix__empty__prefix_too_short)
{
    BOOST_REQUIRE(witness_address::parse_prefix("") == result::prefix_too_short);
    BOOST_REQUIRE(witness_address::parse_prefix("\x00") == result::prefix_too_short);
}

BOOST_AUTO_TEST_CASE(witness_address__parse_prefix__upper_case__prefix_too_long)
{
    const std::string prefix((90 - 1 - 1 - 4 - 6) + 1, 'a');
    BOOST_REQUIRE(witness_address::parse_prefix(prefix) == result::prefix_too_long);
}

BOOST_AUTO_TEST_CASE(witness_address__parse_prefix__invalid_ascii__prefix_invalid_character)
{
    // Only ascii characters will be captured here [0x00-0x20, 0x7f].
    BOOST_REQUIRE(witness_address::parse_prefix({ 0x00 }) == result::prefix_invalid_character);
    BOOST_REQUIRE(witness_address::parse_prefix("\x20") == result::prefix_invalid_character);
    BOOST_REQUIRE(witness_address::parse_prefix("\x7f") == result::prefix_invalid_character);
    BOOST_REQUIRE(witness_address::parse_prefix("z\x7fz") == result::prefix_invalid_character);
}

BOOST_AUTO_TEST_CASE(witness_address__parse_prefix__valid_characters_and_lengths__valid)
{
    const std::string prefix((90 - 1 - 1 - 4 - 6), 'a');
    BOOST_REQUIRE(witness_address::parse_prefix(prefix) == result::valid);
    BOOST_REQUIRE(witness_address::parse_prefix("\x21") == result::valid);
    BOOST_REQUIRE(witness_address::parse_prefix("\x7e") == result::valid);
    BOOST_REQUIRE(witness_address::parse_prefix("xyz") == result::valid);
}

BOOST_AUTO_TEST_CASE(witness_address__parse_prefix__known__valid)
{
    BOOST_REQUIRE(witness_address::parse_prefix("bc") == result::valid);
    BOOST_REQUIRE(witness_address::parse_prefix("tb") == result::valid);
    BOOST_REQUIRE(witness_address::parse_prefix("bc", true) == result::valid);
    BOOST_REQUIRE(witness_address::parse_prefix("tb", true) == result::valid);
}

BOOST_AUTO_TEST_CASE(witness_address__parse_prefix__unknown_strict__prefix_unknown)
{
    BOOST_REQUIRE(witness_address::parse_prefix("split") == result::valid);
    BOOST_REQUIRE(witness_address::parse_prefix("split", true) == result::prefix_unknown);
}

// parse_address

BOOST_AUTO_TEST_CASE(witness_address__parse_address__not_ascii__address_not_ascii)
{
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "\x80") == result::address_not_ascii);
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "\xff") == result::address_not_ascii);
}

BOOST_AUTO_TEST_CASE(witness_address__parse_address__mixed_case__address_mixed_case)
{
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "aBc") == result::address_mixed_case);
}

BOOST_AUTO_TEST_CASE(witness_address__parse_address__too_long__address_too_long)
{
    const std::string address(90 + 1, 'a');
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, address) == result::address_too_long);
}

BOOST_AUTO_TEST_CASE(witness_address__parse_address__no_separator__prefix_missing)
{
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "abc") == result::prefix_missing);
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "ABC") == result::prefix_missing);
}

BOOST_AUTO_TEST_CASE(witness_address__parse_address__separator__prefix_errors)
{
    const std::string prefix((90 - 1 - 1 - 4 - 6) + 1, 'a');
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "1") == result::prefix_too_short);
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, prefix + "1") == result::prefix_too_long);
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "\x7f" "1") == result::prefix_invalid_character);
}

BOOST_AUTO_TEST_CASE(witness_address__parse_address__short_payload__payload_too_short)
{
    const std::string payload((1 + 4 + 6) - 1, 'i');
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "a1" + payload) == result::payload_too_short);
}

BOOST_AUTO_TEST_CASE(witness_address__parse_address__long_payload__payload_too_long)
{
    const std::string payload((1 + 64 + 6) + 1, 'a');
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "a1" + payload) == result::payload_too_long);
}

BOOST_AUTO_TEST_CASE(witness_address__parse_address__invalid_payload__payload_not_base32)
{
    const std::string payload(1 + 4 + 6, 'i');
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "a1" + payload) == result::payload_not_base32);
}

BOOST_AUTO_TEST_CASE(witness_address__parse_address__invalid_checksum__checksum_invalid)
{
    const std::string prefix{ "bc" };
    auto address = prefix + "1" + encode_base32(bech32_build_checked(0, data_chunk(2, 0x00), prefix));
    *std::prev(address.end()) = 'x';
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, address) == result::checksum_invalid);
}

BOOST_AUTO_TEST_CASE(witness_address__parse_address__version_17__version_invalid)
{
    const size_t version = 17;
    const data_chunk program(2, 0x00);
    const std::string prefix{ "bc" };
    const auto address = prefix + "1" + encode_base32(bech32_build_checked(version, program, prefix));
    BOOST_REQUIRE_EQUAL(address, "bc13qqqqjggfzq");
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, address) == result::version_invalid);
}

BOOST_AUTO_TEST_CASE(witness_address__parse_address__version_16_minimal_program_strict__program_invalid)
{
    const size_t version = 16;
    const data_chunk program(2, 0x00);
    const std::string prefix{ "bc" };
    const auto address = prefix + "1" + encode_base32(bech32_build_checked(version, program, prefix));
    BOOST_REQUIRE_EQUAL(address, "bc1sqqqqkfw08p");
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, address, true) == result::program_invalid);
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, address, false) == result::valid);
    BOOST_REQUIRE_EQUAL(out1, prefix);
    BOOST_REQUIRE_EQUAL(out2, version);
    BOOST_REQUIRE_EQUAL(out3, program);
}

BOOST_AUTO_TEST_CASE(witness_address__parse_address__version_0_empty_program__payload_too_short)
{
    const std::string prefix{ "bc" };
    const auto address = prefix + "1" + encode_base32(bech32_build_checked(0, {}, prefix));
    BOOST_REQUIRE_EQUAL(address, "bc1q9zpgru");
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, address) == result::payload_too_short);
}

BOOST_AUTO_TEST_CASE(witness_address__parse_address__version_0_minimal_program__program_invalid)
{
    const std::string prefix{ "bc" };
    const auto address = prefix + "1" + encode_base32(bech32_build_checked(0, data_chunk(2, 0x00), prefix));
    BOOST_REQUIRE_EQUAL(address, "bc1qqqqq399cqn");
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, address) == result::program_invalid);
}

BOOST_AUTO_TEST_CASE(witness_address__parse_address__version_0_null_short_hash_program__valid)
{
    const size_t version = 0;
    const data_chunk program = to_chunk(null_short_hash);
    const std::string prefix{ "bc" };
    const auto address = prefix + "1" + encode_base32(bech32_build_checked(version, program, prefix));
    BOOST_REQUIRE_EQUAL(address, "bc1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq9e75rs");
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, address) == result::valid);
    BOOST_REQUIRE_EQUAL(out1, prefix);
    BOOST_REQUIRE_EQUAL(out2, version);
    BOOST_REQUIRE_EQUAL(out3, program);
}

BOOST_AUTO_TEST_CASE(witness_address__parse_address__version_0_null_hash_program__valid)
{
    const size_t version = 0;
    const data_chunk program = to_chunk(null_hash);
    const std::string prefix{ "bc" };
    const auto address = prefix + "1" + encode_base32(bech32_build_checked(version, program, prefix));
    BOOST_REQUIRE_EQUAL(address, "bc1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqthqst8");
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, address) == result::valid);
    BOOST_REQUIRE_EQUAL(out1, prefix);
    BOOST_REQUIRE_EQUAL(out2, version);
    BOOST_REQUIRE_EQUAL(out3, program);
}

// parse_program

BOOST_AUTO_TEST_CASE(witness_address__parse_program__version_0_short_hash_size__version_0_p2kh)
{
    BOOST_REQUIRE(witness_address::parse_program(0, null_short_hash) == type::version_0_p2kh);
}

BOOST_AUTO_TEST_CASE(witness_address__parse_program__version_0_hash_size__version_0_p2sh)
{
    BOOST_REQUIRE(witness_address::parse_program(0, null_hash) == type::version_0_p2sh);
}

BOOST_AUTO_TEST_CASE(witness_address__parse_program__version_16_short__invalid)
{
    BOOST_REQUIRE(witness_address::parse_program(16, data_chunk(1, 0x00)) == type::invalid);
}

BOOST_AUTO_TEST_CASE(witness_address__parse_program__version_16_long__invalid)
{
    BOOST_REQUIRE(witness_address::parse_program(16, data_chunk(40 + 1, 0x00)) == type::invalid);
}

BOOST_AUTO_TEST_CASE(witness_address__parse_program__version_non_zero_valid__unknown)
{
    BOOST_REQUIRE(witness_address::parse_program(1, null_hash) == type::unknown);
    BOOST_REQUIRE(witness_address::parse_program(16, null_hash) == type::unknown);
}

BOOST_AUTO_TEST_CASE(witness_address__parse_program__version_non_zero_valid_strict__invalid)
{
    BOOST_REQUIRE(witness_address::parse_program(1, null_hash, true) == type::invalid);
    BOOST_REQUIRE(witness_address::parse_program(16, null_hash, true) == type::invalid);
}

BOOST_AUTO_TEST_CASE(witness_address__parse_program__version_invalid__invalid)
{
    BOOST_REQUIRE(witness_address::parse_program(17, null_hash) == type::invalid);
    BOOST_REQUIRE(witness_address::parse_program(255, null_hash) == type::invalid);
}

// cast bool

BOOST_AUTO_TEST_CASE(witness_address__bool__invalid__false)
{
    const witness_address instance{};
    BOOST_REQUIRE(!instance);
}

BOOST_AUTO_TEST_CASE(witness_address__bool__valid__true)
{
    const witness_address instance{ null_hash };
    BOOST_REQUIRE(instance);
}

// encoded

BOOST_AUTO_TEST_CASE(witness_address__encoded__empty__empty)
{
    const witness_address instance{};
    BOOST_REQUIRE(instance.encoded().empty());
}

BOOST_AUTO_TEST_CASE(witness_address__encoded__copied__expected)
{
    const auto address = "bc1sqqqqkfw08p";
    const witness_address instance{ address };
    BOOST_REQUIRE_EQUAL(witness_address(instance).encoded(), address);
}

BOOST_AUTO_TEST_CASE(witness_address__encoded__moved__expected)
{
    auto address = "bc1sqqqqkfw08p";
    const witness_address instance{ std::move(address) };
    BOOST_REQUIRE_EQUAL(witness_address(instance).encoded(), address);
}

BOOST_AUTO_TEST_CASE(witness_address__encoded__invalid_address__empty)
{
    const witness_address instance{ "split1bogus" };
    BOOST_REQUIRE(instance.encoded().empty());
}

BOOST_AUTO_TEST_CASE(witness_address__encoded__address__expected)
{
    const auto address = "bc1sqqqqkfw08p";
    const witness_address instance{ address };
    BOOST_REQUIRE_EQUAL(instance.encoded(), address);
}

BOOST_AUTO_TEST_CASE(witness_address__encoded__key_hash__expected)
{
    const witness_address instance{ null_hash };
    BOOST_REQUIRE_EQUAL(instance.encoded(), "bc1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqthqst8");
}

BOOST_AUTO_TEST_CASE(witness_address__encoded__key_hash_prefix__expected)
{
    const witness_address instance{ null_hash, "split" };
    BOOST_REQUIRE_EQUAL(instance.encoded(), "split1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqs8xlkk");
}

BOOST_AUTO_TEST_CASE(witness_address__encoded__invalid_ec_private__empty)
{
    const witness_address instance{ ec_private{} };
    BOOST_REQUIRE(instance.encoded().empty());
}

BOOST_AUTO_TEST_CASE(witness_address__encoded__ec_private__expected)
{
    const auto wif = "L1WepftUBemj6H4XQovkiW1ARVjxMqaw4oj2kmkYqdG1xTnBcHfC";
    const witness_address instance{ ec_private(wif) };
    BOOST_REQUIRE_EQUAL(instance.encoded(), "bc1qkn8up7xm8564fea4rzrn6t6pcv6mk8atnna7pw");
}

BOOST_AUTO_TEST_CASE(witness_address__encoded__ec_private_prefix__expected)
{
    const auto wif = "L1WepftUBemj6H4XQovkiW1ARVjxMqaw4oj2kmkYqdG1xTnBcHfC";
    const witness_address instance{ ec_private(wif), "split" };
    BOOST_REQUIRE_EQUAL(instance.encoded(), "split1qkn8up7xm8564fea4rzrn6t6pcv6mk8atkw7e5r");
}

BOOST_AUTO_TEST_CASE(witness_address__encoded__invalid_ec_public__empty)
{
    const witness_address instance{ ec_public{} };
    BOOST_REQUIRE(instance.encoded().empty());
}

BOOST_AUTO_TEST_CASE(witness_address__encoded__ec_public__expected)
{
    const auto wif = "L1WepftUBemj6H4XQovkiW1ARVjxMqaw4oj2kmkYqdG1xTnBcHfC";
    const witness_address instance{ ec_private(wif).to_public() };
    BOOST_REQUIRE_EQUAL(instance.encoded(), "bc1qkn8up7xm8564fea4rzrn6t6pcv6mk8atnna7pw");
}

BOOST_AUTO_TEST_CASE(witness_address__encoded__ec_public_prefix__expected)
{
    const auto wif = "L1WepftUBemj6H4XQovkiW1ARVjxMqaw4oj2kmkYqdG1xTnBcHfC";
    const witness_address instance{ ec_private(wif).to_public(), "split" };
    BOOST_REQUIRE_EQUAL(instance.encoded(), "split1qkn8up7xm8564fea4rzrn6t6pcv6mk8atkw7e5r");
}

BOOST_AUTO_TEST_CASE(witness_address__encoded__script_hash__expected)
{
    const auto script = chain::script(script::to_pay_null_data_pattern(null_hash));
    const auto script_hash = sha256_hash(script.to_data(false));
    const witness_address instance{ script_hash };
    BOOST_REQUIRE_EQUAL(instance.encoded(), "bc1qpxlkrchnvcz7exzzshh542797nzqx6g4vk5n4sn3m9sf64xlmarsp3t6g2");
}

BOOST_AUTO_TEST_CASE(witness_address__encoded__script_hash_prefix__expected)
{
    const auto script = chain::script(script::to_pay_null_data_pattern(null_hash));
    const auto script_hash = sha256_hash(script.to_data(false));
    const witness_address instance{ script_hash, "split" };
    BOOST_REQUIRE_EQUAL(instance.encoded(), "split1qpxlkrchnvcz7exzzshh542797nzqx6g4vk5n4sn3m9sf64xlmars6pd44m");
}

BOOST_AUTO_TEST_CASE(witness_address__encoded__invalid_script__empty)
{
    const witness_address instance{ chain::script{} };
    BOOST_REQUIRE(instance.encoded().empty());
}

BOOST_AUTO_TEST_CASE(witness_address__encoded__script__expected)
{
    const auto script = chain::script(script::to_pay_null_data_pattern(null_hash));
    const witness_address instance{ script };
    BOOST_REQUIRE_EQUAL(instance.encoded(), "bc1qpxlkrchnvcz7exzzshh542797nzqx6g4vk5n4sn3m9sf64xlmarsp3t6g2");
}

BOOST_AUTO_TEST_CASE(witness_address__encoded__script_prefix__expected)
{
    const auto script = chain::script(script::to_pay_null_data_pattern(null_hash));
    const witness_address instance{ script, "split" };
    BOOST_REQUIRE_EQUAL(instance.encoded(), "split1qpxlkrchnvcz7exzzshh542797nzqx6g4vk5n4sn3m9sf64xlmars6pd44m");
}

// prefix

BOOST_AUTO_TEST_CASE(witness_address__prefix__invalid__empty)
{
    const witness_address instance{};
    BOOST_REQUIRE(instance.prefix().empty());
}

BOOST_AUTO_TEST_CASE(witness_address__prefix__default__bc)
{
    const witness_address instance{ null_hash };
    BOOST_REQUIRE_EQUAL(instance.prefix(), "bc");
}

BOOST_AUTO_TEST_CASE(witness_address__prefix__explicit__expected)
{
    const witness_address instance{ null_hash, "split" };
    BOOST_REQUIRE_EQUAL(instance.prefix(), "split");
}

// version

BOOST_AUTO_TEST_CASE(witness_address__version__invalid__invalid)
{
    const witness_address instance{};
    BOOST_REQUIRE_EQUAL(instance.version(), max_uint8);
}

BOOST_AUTO_TEST_CASE(witness_address__version__17__invalid)
{
    const witness_address instance{ "bc13qqqqjggfzq" };
    BOOST_REQUIRE_EQUAL(instance.version(), max_uint8);
}

BOOST_AUTO_TEST_CASE(witness_address__version__16__16)
{
    const witness_address instance{ "bc1sqqqqkfw08p" };
    BOOST_REQUIRE_EQUAL(instance.version(), 16);
}

BOOST_AUTO_TEST_CASE(witness_address__version__16_strict__invalid)
{
    const witness_address instance{ "bc1sqqqqkfw08p", true };
    BOOST_REQUIRE_EQUAL(instance.version(), max_uint8);
}

BOOST_AUTO_TEST_CASE(witness_address__version__0_empty__invalid)
{
    const witness_address instance{ "bc1q9zpgru" };
    BOOST_REQUIRE_EQUAL(instance.version(), max_uint8);
}

BOOST_AUTO_TEST_CASE(witness_address__version__0__0)
{
    const witness_address instance{ "bc1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq9e75rs" };
    BOOST_REQUIRE_EQUAL(instance.version(), 0);
}

// program

BOOST_AUTO_TEST_CASE(witness_address__program__invalid__empty)
{
    const witness_address instance{};
    BOOST_REQUIRE(instance.program().empty());
}

BOOST_AUTO_TEST_CASE(witness_address__program__null_short_hash__expected)
{
    const witness_address instance{ "bc1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq9e75rs" };
    BOOST_REQUIRE_EQUAL(instance.program(), to_chunk(null_short_hash));
}

BOOST_AUTO_TEST_CASE(witness_address__program__null_hash_address__expected)
{
    const witness_address instance{ "bc1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqthqst8" };
    BOOST_REQUIRE_EQUAL(instance.program(), to_chunk(null_hash));
}

BOOST_AUTO_TEST_CASE(witness_address__program__script__expected)
{
    const auto script = chain::script(script::to_pay_null_data_pattern(null_hash));
    const auto script_hash = sha256_hash(script.to_data(false));
    const witness_address instance{ script };
    BOOST_REQUIRE_EQUAL(instance.program(), to_chunk(script_hash));
}

// identifier

BOOST_AUTO_TEST_CASE(witness_address__identifier__invalid__invalid)
{
    const witness_address instance{};
    BOOST_REQUIRE(instance.identifier() == type::invalid);
}

BOOST_AUTO_TEST_CASE(witness_address__identifier__version_17_address__invalid)
{
    const witness_address instance{ "bc13qqqqjggfzq" };
    BOOST_REQUIRE(instance.identifier() == type::invalid);
}

BOOST_AUTO_TEST_CASE(witness_address__identifier__version_16_address__unknown)
{
    const witness_address instance{ "bc1sqqqqkfw08p" };
    BOOST_REQUIRE(instance.identifier() == type::unknown);
}

BOOST_AUTO_TEST_CASE(witness_address__identifier__version_0_minimal__invalid)
{
    const witness_address instance{ "bc1qqqqq399cqn" };
    BOOST_REQUIRE(instance.identifier() == type::invalid);
}

BOOST_AUTO_TEST_CASE(witness_address__identifier__key_hash__version_0_p2kh)
{
    const witness_address instance{ null_short_hash };
    BOOST_REQUIRE(instance.identifier() == type::version_0_p2kh);
}

BOOST_AUTO_TEST_CASE(witness_address__identifier__script_hash__version_0_p2sh)
{
    const auto script = chain::script(script::to_pay_null_data_pattern(null_hash));
    const witness_address instance{ sha256_hash(script.to_data(false)) };
    BOOST_REQUIRE(instance.identifier() == type::version_0_p2sh);
}

BOOST_AUTO_TEST_CASE(witness_address__identifier__script__version_0_p2sh)
{
    const auto script = chain::script(script::to_pay_null_data_pattern(null_hash));
    const witness_address instance{ script };
    BOOST_REQUIRE(instance.identifier() == type::version_0_p2sh);
}

// script

BOOST_AUTO_TEST_CASE(witness_address__script__invalid__invalid)
{
    const witness_address instance{};
    BOOST_REQUIRE(!instance.script().is_valid());
}

BOOST_AUTO_TEST_CASE(witness_address__script__version_16_address__valid)
{
    const witness_address instance{ "bc1sqqqqkfw08p" };
    BOOST_REQUIRE(chain::script::is_pay_witness_pattern(instance.script().operations()));
}

BOOST_AUTO_TEST_CASE(witness_address__script__key_hash__pay_witness_key_hash)
{
    const witness_address instance{ null_short_hash };
    BOOST_REQUIRE(chain::script::is_pay_witness_key_hash_pattern(instance.script().operations()));
}

BOOST_AUTO_TEST_CASE(witness_address__script__script_hash__pay_witness_script_hash)
{
    const witness_address instance{ null_hash };
    BOOST_REQUIRE(chain::script::is_pay_witness_script_hash_pattern(instance.script().operations()));
}

BOOST_AUTO_TEST_CASE(witness_address__script__null_data_script__pay_witness_script_hash)
{
    const auto script = chain::script(script::to_pay_null_data_pattern(null_hash));
    const witness_address instance{ script };
    BOOST_REQUIRE(chain::script::is_pay_witness_script_hash_pattern(instance.script().operations()));
}

// assignment

BOOST_AUTO_TEST_CASE(witness_address__assignment__copied__expected)
{
    const auto address = "bc1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq9e75rs";
    const witness_address instance1{ address };
    witness_address instance2;
    instance2 = instance1;
    BOOST_REQUIRE_EQUAL(instance2.encoded(), address);
}

BOOST_AUTO_TEST_CASE(witness_address__assignment__moved__expected)
{
    const auto address = "bc1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq9e75rs";
    witness_address instance1{ address };
    witness_address instance2;
    instance2 = std::move(instance1);
    BOOST_REQUIRE_EQUAL(instance2.encoded(), address);
}

// operator<

BOOST_AUTO_TEST_CASE(witness_address__less_than_operator__self__false)
{
    const witness_address instance{ "bc1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq9e75rs" };
    BOOST_REQUIRE(!(instance < instance));
}

BOOST_AUTO_TEST_CASE(witness_address__less_than_operator__equal__false)
{
    const witness_address instance1{ "bc1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq9e75rs" };
    const witness_address instance2{ "bc1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq9e75rs" };
    BOOST_REQUIRE(!(instance1 < instance2));
}

BOOST_AUTO_TEST_CASE(witness_address__less_than_operator__greater__false)
{
    const witness_address instance1{ "bc1qkn8up7xm8564fea4rzrn6t6pcv6mk8atnna7pw" };
    const witness_address instance2{ "bc1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq9e75rs" };
    BOOST_REQUIRE(!(instance2 < instance1));
}

BOOST_AUTO_TEST_CASE(witness_address__less_than_operator__lesser__true)
{
    const witness_address instance1{ "bc1qkn8up7xm8564fea4rzrn6t6pcv6mk8atnna7pw" };
    const witness_address instance2{ "bc1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq9e75rs" };
    BOOST_REQUIRE(instance1 < instance2);
}

// operator==

BOOST_AUTO_TEST_CASE(witness_address__equality__self__true)
{
    const witness_address instance{ "bc1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq9e75rs" };
    BOOST_REQUIRE(instance == instance);
}

BOOST_AUTO_TEST_CASE(witness_address__equality__equal__true)
{
    const witness_address instance1{ "bc1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq9e75rs" };
    const witness_address instance2{ "bc1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq9e75rs" };
    BOOST_REQUIRE(instance1 == instance2);
}

BOOST_AUTO_TEST_CASE(witness_address__equality__distinct__false)
{
    const witness_address instance1{ "bc1qkn8up7xm8564fea4rzrn6t6pcv6mk8atnna7pw" };
    const witness_address instance2{ "bc1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq9e75rs" };
    BOOST_REQUIRE(!(instance1 == instance2));
}

// operator!=

BOOST_AUTO_TEST_CASE(witness_address__inequality__self__false)
{
    const witness_address instance{ "bc1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq9e75rs" };
    BOOST_REQUIRE(!(instance != instance));
}

BOOST_AUTO_TEST_CASE(witness_address__inequality__equal__false)
{
    const witness_address instance1{ "bc1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq9e75rs" };
    const witness_address instance2{ "bc1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq9e75rs" };
    BOOST_REQUIRE(!(instance1 != instance2));
}

BOOST_AUTO_TEST_CASE(witness_address__inequality__distinct__true)
{
    const witness_address instance1{ "bc1qkn8up7xm8564fea4rzrn6t6pcv6mk8atnna7pw" };
    const witness_address instance2{ "bc1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq9e75rs" };
    BOOST_REQUIRE(instance1 != instance2);
}

// operator>>

BOOST_AUTO_TEST_CASE(witness_address__istream__invalid__throws_istream_exception)
{
    std::stringstream in{};
    witness_address instance{};
    BOOST_REQUIRE_THROW(in >> instance, istream_exception);
}

BOOST_AUTO_TEST_CASE(witness_address__istream__valid__expected)
{
    const auto address = "bc1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq9e75rs";
    std::stringstream in{ address };
    witness_address instance{};
    in >> instance;
    BOOST_REQUIRE_EQUAL(instance.encoded(), address);
}

// operator<<

BOOST_AUTO_TEST_CASE(witness_address__ostream__invalid__empty)
{
    std::stringstream out;
    witness_address instance{};
    out << instance;
    BOOST_REQUIRE(out.str().empty());
}

BOOST_AUTO_TEST_CASE(witness_address__ostream__valid__expected)
{
    std::stringstream out;
    const auto address = "bc1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq9e75rs";
    const witness_address instance{ address };
    out << instance;
    BOOST_REQUIRE_EQUAL(out.str(), address);
}

// BIP173 vectors
// See checksum and base_32 for related tests.

// These examples of valid "bech32" require an unversioned (and empty) payload, which
// requires exposure of bech32 internals and has no use case, so is not supported.
// The payload length requires five more characters to be a valid address.
// Note that the casing and character evaluations intended for testing here do in fact pass.
BOOST_AUTO_TEST_CASE(witness_address__bip173__unversioned_empty_bech32_vectors__payload_too_short)
{
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "A12UEL5L") == result::payload_too_short);
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "a12uel5l") == result::payload_too_short);
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "?1ezyfcl") == result::payload_too_short);
}

// This example of valid "bech32" requires an unversioned (and empty) payload, which
// requires exposure of bech32 internals and has no use case, so is not supported.
// The payload length requires five more characters to be a valid address.
// Consequently the prefix length is restricted to one less byte, satisfying the overall length constraint.
BOOST_AUTO_TEST_CASE(witness_address__bip173__unversioned_empty_bech32_vectors__prefix_too_long)
{
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "an83characterlonghumanreadablepartthatcontainsthenumber1andtheexcludedcharactersbio1tt5tgs") == result::prefix_too_long);
}

// This example of valid "bech32" can be interepreted as versioned or otherwise.
// The payload is greater than 71 characters, failing after successful parse of the maximum length tested here.
BOOST_AUTO_TEST_CASE(witness_address__bip173__unversioned_bech32_vectors__payload_too_long)
{
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "11qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqc8247j") == result::payload_too_long);
}

// This example of valid "bech32" can be interepreted as versioned or otherwise.
BOOST_AUTO_TEST_CASE(witness_address__bip173__unversioned_bech32_vectors__checksum_invalid)
{
    // Under bech32 this returns valid, but the non-zero version causes a checksum failure under bech32m.
    ////BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "split1checkupstagehandshakeupstreamerranterredcaperred2y9e3w") == result::valid);
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "split1checkupstagehandshakeupstreamerranterredcaperred2y9e3w") == result::checksum_invalid);
}

// HRP character out of range.
BOOST_AUTO_TEST_CASE(witness_address__bip173__invalid_bech32_prefix_character__invalid_expected)
{
    BOOST_REQUIRE(witness_address::parse_prefix("\x20") == result::prefix_invalid_character);
    BOOST_REQUIRE(witness_address::parse_prefix("\x7f") == result::prefix_invalid_character);
    BOOST_REQUIRE(witness_address::parse_prefix("\x80") == result::prefix_not_ascii);

    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "\x20" "1nwldj5") == result::prefix_invalid_character);
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "\x7f" "1axkwrx") == result::prefix_invalid_character);
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "\x80" "1eym55h") == result::address_not_ascii);
}
// Overall max length exceeded.
BOOST_AUTO_TEST_CASE(witness_address__bip173__invalid_bech32_overall_length__address_too_long)
{
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "an84characterslonghumanreadablepartthatcontainsthenumber1andtheexcludedcharactersbio1569pvx") == result::address_too_long);
}

// No separator character.
BOOST_AUTO_TEST_CASE(witness_address__bip173__no_separator__prefix_missing)
{
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "pzry9x0s0muk") == result::prefix_missing);
}

// Empty HRP.
BOOST_AUTO_TEST_CASE(witness_address__bip173__empty_prefix1__prefix_too_short)
{
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "1pzry9x0s0muk") == result::prefix_too_short);
}

// Invalid data character.
BOOST_AUTO_TEST_CASE(witness_address__bip173__invalid_base32__payload_not_base32)
{
    // This example is too short to be a valid address test (requires 13 characters minimum).
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "x1b4n0q5v") == result::payload_too_short);
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "x1b4n00000q5v") == result::payload_not_base32);
}

// Too short checksum.
BOOST_AUTO_TEST_CASE(witness_address__bip173__short_checksum__payload_too_short)
{
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "li1dgmt3") == result::payload_too_short);
}

// 0xFF: Invalid character in checksum.
BOOST_AUTO_TEST_CASE(witness_address__bip173__invalid_checksum__payload_not_base32)
{
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "de1lg7wt" "\xff") == result::address_not_ascii);
}

// Checksum calculated with uppercase form of HRP.
// This example of valid "bech32" requires an unversioned (and empty) payload, which
// requires exposure of bech32 internals and has no use case, so is not supported.
// The payload length is limited by one more character in an implementation that requires a version.
BOOST_AUTO_TEST_CASE(witness_address__bip173__invalid_checksum__payload_too_short)
{
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "A1G7SGD8") == result::payload_too_short);
}

// Empty HRP.
BOOST_AUTO_TEST_CASE(witness_address__bip173__empty_prefix2__prefix_too_short)
{
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "10a06t8") == result::prefix_too_short);
}

// Empty HRP.
BOOST_AUTO_TEST_CASE(witness_address__bip173__empty_prefix3__prefix_too_short)
{
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "1qzzfhee") == result::prefix_too_short);
}

// BIP173: The following list gives valid segwit addresses and the scriptPubKey that they translate to in hex.

BOOST_AUTO_TEST_CASE(witness_address__bip173__valid_address1__expected)
{
    const witness_address address("BC1QW508D6QEJXTDG4Y5R3ZARVARY0C5XW7KV8F3T4");
    BOOST_REQUIRE_EQUAL(address.version(), 0u);
    BOOST_REQUIRE(address.identifier() == type::version_0_p2kh);
    BOOST_REQUIRE_EQUAL(address.program(), base16_chunk("751e76e8199196d454941c45d1b3a323f1433bd6"));
    BOOST_REQUIRE_EQUAL(address.script().to_string(chain::all_rules), "zero [751e76e8199196d454941c45d1b3a323f1433bd6]");
}

BOOST_AUTO_TEST_CASE(witness_address__bip173__valid_address2__expected)
{
    const witness_address address("tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3q0sl5k7");
    BOOST_REQUIRE_EQUAL(address.version(), 0u);
    BOOST_REQUIRE(address.identifier() == type::version_0_p2sh);
    BOOST_REQUIRE_EQUAL(address.program(), base16_chunk("1863143c14c5166804bd19203356da136c985678cd4d27a1b8c6329604903262"));
    BOOST_REQUIRE_EQUAL(address.script().to_string(chain::all_rules), "zero [1863143c14c5166804bd19203356da136c985678cd4d27a1b8c6329604903262]");
}

BOOST_AUTO_TEST_CASE(witness_address__bip173__valid_address3__expected)
{
    // This returns valid under bech32 and checksum_invalid under bech32m.
    ////const witness_address address("bc1pw508d6qejxtdg4y5r3zarvary0c5xw7kw508d6qejxtdg4y5r3zarvary0c5xw7k7grplx");
    const auto program = base16_literal("751e76e8199196d454941c45d1b3a323f1433bd6751e76e8199196d454941c45d1b3a323f1433bd6");
    const witness_address address(program, "bc", 1);
    BOOST_REQUIRE_EQUAL(address.encoded(), "bc1pw508d6qejxtdg4y5r3zarvary0c5xw7kw508d6qejxtdg4y5r3zarvary0c5xw7kt5nd6y");
    BOOST_REQUIRE_EQUAL(address.version(), 1u);
    BOOST_REQUIRE(address.identifier() == type::unknown);
    BOOST_REQUIRE_EQUAL(address.program(), to_chunk(program));
    BOOST_REQUIRE_EQUAL(address.script().to_string(chain::all_rules), "1 [751e76e8199196d454941c45d1b3a323f1433bd6751e76e8199196d454941c45d1b3a323f1433bd6]");
}

BOOST_AUTO_TEST_CASE(witness_address__bip173__valid_address4__expected)
{
    // This returns valid under bech32 and checksum_invalid under bech32m.
    ////const witness_address address("BC1SW50QA3JX3S");
    const auto program = base16_literal("751e");
    const witness_address address(program, "bc", 16);
    BOOST_REQUIRE_EQUAL(ascii_to_upper(address.encoded()), "BC1SW50QGDZ25J");
    BOOST_REQUIRE_EQUAL(address.version(), 16u);
    BOOST_REQUIRE(address.identifier() == type::unknown);
    BOOST_REQUIRE_EQUAL(address.program(), to_chunk(program));
    BOOST_REQUIRE_EQUAL(address.script().to_string(chain::all_rules), "16 [751e]");
}

BOOST_AUTO_TEST_CASE(witness_address__bip173__valid_address5__expected)
{
    // This returns valid under bech32 and checksum_invalid under bech32m.
    ////const witness_address address("bc1zw508d6qejxtdg4y5r3zarvaryvg6kdaj");
    const auto program = base16_literal("751e76e8199196d454941c45d1b3a323");
    const witness_address address(program, "bc", 2);
    BOOST_REQUIRE_EQUAL(address.encoded(), "bc1zw508d6qejxtdg4y5r3zarvaryvaxxpcs");
    BOOST_REQUIRE_EQUAL(address.version(), 2u);
    BOOST_REQUIRE(address.identifier() == type::unknown);
    BOOST_REQUIRE_EQUAL(address.program(), to_chunk(program));
    BOOST_REQUIRE_EQUAL(address.script().to_string(chain::all_rules), "2 [751e76e8199196d454941c45d1b3a323]");
}

BOOST_AUTO_TEST_CASE(witness_address__bip173__valid_address6__expected)
{
    // Bech32m/Bech32 same (version 0).
    const witness_address address("tb1qqqqqp399et2xygdj5xreqhjjvcmzhxw4aywxecjdzew6hylgvsesrxh6hy");
    BOOST_REQUIRE_EQUAL(address.version(), 0u);
    BOOST_REQUIRE(address.identifier() == type::version_0_p2sh);
    BOOST_REQUIRE_EQUAL(address.program(), base16_chunk("000000c4a5cad46221b2a187905e5266362b99d5e91c6ce24d165dab93e86433"));
    BOOST_REQUIRE_EQUAL(address.script().to_string(chain::all_rules), "zero [000000c4a5cad46221b2a187905e5266362b99d5e91c6ce24d165dab93e86433]");
}

// BIP173: The following list gives invalid segwit addresses and the reason for their invalidity.

// Invalid human-readable part.
BOOST_AUTO_TEST_CASE(witness_address__bip173__invalid_address1__prefix_unknown)
{
    // Strict parsing changes from valid (prefix_unknown) to prefix_unknown.
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "tc1qw508d6qejxtdg4y5r3zarvary0c5xw7kg3g4ty") == result::valid);
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "tc1qw508d6qejxtdg4y5r3zarvary0c5xw7kg3g4ty", true) == result::prefix_unknown);
}

// Invalid checksum.
BOOST_AUTO_TEST_CASE(witness_address__bip173__invalid_address2__checksum_invalid)
{
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t5") == result::checksum_invalid);
}

// Invalid witness version.
BOOST_AUTO_TEST_CASE(witness_address__bip173__invalid_address3__checksum_invalid)
{
    // This returns version_invalid under bech32 and checksum_invalid under bech32m.
    ////BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "BC13W508D6QEJXTDG4Y5R3ZARVARY0C5XW7KN40WF2") == result::version_invalid);
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "BC13W508D6QEJXTDG4Y5R3ZARVARY0C5XW7KN40WF2") == result::checksum_invalid);
}

// Invalid program length.
BOOST_AUTO_TEST_CASE(witness_address__bip173__invalid_address4__payload_too_short)
{
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "bc1rw5uspcuh") == result::payload_too_short);
}

// Invalid program length.
BOOST_AUTO_TEST_CASE(witness_address__bip173__invalid_address5__payload_too_long)
{
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "bc10w508d6qejxtdg4y5r3zarvary0c5xw7kw508d6qejxtdg4y5r3zarvary0c5xw7kw5rljs90") == result::payload_too_long);
}

// Invalid program length for witness version 0.
BOOST_AUTO_TEST_CASE(witness_address__bip173__invalid_address6__program_invalid)
{
    // This is a zero version, so lax parsing has no impact.
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "BC1QR508D6QEJXTDG4Y5R3ZARVARYV98GJ9P") == result::program_invalid);
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "BC1QR508D6QEJXTDG4Y5R3ZARVARYV98GJ9P", true) == result::program_invalid);
}

// Mixed case.
BOOST_AUTO_TEST_CASE(witness_address__bip173__invalid_address7__address_mixed_case)
{
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3q0sL5k7") == result::address_mixed_case);
}

// Zero padding of more than 4 bits.
BOOST_AUTO_TEST_CASE(witness_address__bip173__invalid_address8__checksum_invalid)
{
    // This returns valid/program_invalid under bech32 and checksum_invalid under bech32m.
    ////BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "bc1zw508d6qejxtdg4y5r3zarvaryvqyzf3du") == result::valid);
    ////BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "bc1zw508d6qejxtdg4y5r3zarvaryvqyzf3du", true) == result::program_invalid);
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "bc1zw508d6qejxtdg4y5r3zarvaryvqyzf3du") == result::checksum_invalid);
}

// Non-zero padding in 8-to-5 conversion.
BOOST_AUTO_TEST_CASE(witness_address__bip173__invalid_address9__program_invalid)
{
    // Non-zero padding changes the encoded representation, and would be ignored if not explicitly checked for.
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3pjxtptv") == result::program_invalid);
}

// Empty data section.
BOOST_AUTO_TEST_CASE(witness_address__bip173__invalid_address10__payload_too_short)
{
    BOOST_REQUIRE(witness_address::parse_address(out1, out2, out3, "bc1gmk9yu") == result::payload_too_short);
}

BOOST_AUTO_TEST_SUITE_END()
