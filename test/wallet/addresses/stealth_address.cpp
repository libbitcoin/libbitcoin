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

BOOST_AUTO_TEST_SUITE(stealth_address_tests)

using namespace bc::system::wallet;

#define SCAN_KEY "03d9e876028f4fc062c19f7097762e4affc2ce4edfffa7d42e3c17cd157ec6d1bc"
#define SPEND_KEY1 "0215a49b55a2ed2a02569cb6c018644211d408caab3aca86d2cc7d6a9e5789b1d2"
#define STEALTH_ADDRESS "vJmzLu29obZcUGXXgotapfQLUpz7dfnZpbr4xg1R75qctf8xaXAteRdi3ZUk3T2ZMSad5KyPbve7uyH6eswYAxLHRVSbWgNUeoGuXp"

BOOST_AUTO_TEST_CASE(stealth_address__construct__string__expected_encoding)
{
    ec_compressed scan;
    ec_compressed spend1;
    BOOST_REQUIRE(decode_base16(scan, SCAN_KEY));
    BOOST_REQUIRE(decode_base16(spend1, SPEND_KEY1));
    stealth_address address({}, scan, { spend1 }, 0, 42);
    BOOST_REQUIRE(address);
    BOOST_REQUIRE_EQUAL(address.encoded(), STEALTH_ADDRESS);
}

BOOST_AUTO_TEST_CASE(stealth_address__construct__decoded__expected_properties)
{
    stealth_address address(STEALTH_ADDRESS);
    BOOST_REQUIRE(address);
    BOOST_REQUIRE_EQUAL((size_t)address.version(), 42);
    BOOST_REQUIRE_EQUAL(encode_base16(address.scan_key()), SCAN_KEY);
    BOOST_REQUIRE_EQUAL(address.spend_keys().size(), 1u);
    BOOST_REQUIRE_EQUAL(encode_base16(address.spend_keys()[0]), SPEND_KEY1);
    BOOST_REQUIRE_EQUAL((size_t)address.signatures(), 1u);
    BOOST_REQUIRE_EQUAL(address.filter().bits(), 0u);
    BOOST_REQUIRE_EQUAL(address.encoded(), STEALTH_ADDRESS);
}

BOOST_AUTO_TEST_CASE(stealth_address__encoding__scan_mainnet__round_trips)
{
    const auto encoded = "vJmzLu29obZcUGXXgotapfQLUpz7dfnZpbr4xg1R75qctf8xaXAteRdi3ZUk3T2ZMSad5KyPbve7uyH6eswYAxLHRVSbWgNUeoGuXp";
    stealth_address address(encoded);
    BOOST_REQUIRE_EQUAL(address.encoded(), encoded);
    BOOST_REQUIRE_EQUAL(address.version(), 42u);
}

BOOST_AUTO_TEST_CASE(stealth_address__encoding__scan_testnet__round_trips)
{
    const std::string encoded = "waPXhQwQE9tDugfgLkvpDs3dnkPx1RsfDjFt4zBq7EeWeATRHpyQpYrFZR8T4BQy91Vpvshm2TDER8b9ZryuZ8VSzz8ywzNzX8NqF4";
    stealth_address address(encoded);
    BOOST_REQUIRE_EQUAL(address.encoded(), encoded);
    BOOST_REQUIRE_EQUAL(address.version(), 43u);
}

BOOST_AUTO_TEST_CASE(stealth_address__encoding__scan_pub_mainnet__round_trips)
{
    const auto encoded = "hfFGUXFPKkQ5M6LC6aEUKMsURdhw93bUdYdacEtBA8XttLv7evZkira2i";
    stealth_address address(encoded);
    BOOST_REQUIRE_EQUAL(address.encoded(), encoded);
    BOOST_REQUIRE_EQUAL(address.version(), 42u);
}

BOOST_AUTO_TEST_CASE(stealth_address__encoding__scan_pub_testnet__round_trip)
{
    const auto encoded = "idPayBqZUpZH7Y5GTaoEyGxDsEmU377JUmhtqG8yoHCkfGfhnAHmGUJbL";
    stealth_address address(encoded);
    BOOST_REQUIRE_EQUAL(address.encoded(), encoded);
    BOOST_REQUIRE_EQUAL(address.version(), 43u);
}

BOOST_AUTO_TEST_SUITE_END()
