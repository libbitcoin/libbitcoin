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
#include <boost/optional.hpp>

BOOST_AUTO_TEST_SUITE(bitcoin_uri_tests)

using namespace bc::system::wallet;

// Constructors
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(bitcoin_uri__construct__uninitialized__false)
{
    BOOST_REQUIRE(!bitcoin_uri());
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__construct__initialized__true)
{
    BOOST_REQUIRE(bitcoin_uri("bitcoin:"));
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__construct__scheme_mixed_case__normalized)
{
    BOOST_REQUIRE_EQUAL(bitcoin_uri("bitcOin:").encoded(), "bitcoin:");
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__construct__invalid_scheme__false)
{
    BOOST_REQUIRE(!bitcoin_uri("fedcoin:"));
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__construct__payment_address_only__false)
{
    BOOST_REQUIRE(!bitcoin_uri("113Pfw4sFqN1T5kXUnKbqZHMJHN9oyjtgD"));
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__construct__stealth_address_only__false)
{
    BOOST_REQUIRE(!bitcoin_uri("hfFGUXFPKkQ5M6LC6aEUKMsURdhw93bUdYdacEtBA8XttLv7evZkira2i"));
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__construct__fragment__false)
{
    BOOST_REQUIRE(!bitcoin_uri("bitcoin:#satoshi"));
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__construct__strict__test)
{
    BOOST_REQUIRE(!bitcoin_uri("bitcoin:?label=Some テスト"));
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__construct__not_strict__test)
{
    BOOST_REQUIRE(bitcoin_uri("bitcoin:?label=Some テスト", false));
}

// Setters
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(bitcoin_uri__set_path__payment_address__expected_encoding)
{
    const auto expected_payment = "113Pfw4sFqN1T5kXUnKbqZHMJHN9oyjtgD";
    const auto expected_uri = std::string("bitcoin:") + expected_payment;

    bitcoin_uri uri;
    BOOST_REQUIRE(uri.set_path(expected_payment));
    BOOST_REQUIRE_EQUAL(uri.encoded(), expected_uri);
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__set_path__stealth_address__expected_encoding)
{
    const auto expected_payment = "hfFGUXFPKkQ5M6LC6aEUKMsURdhw93bUdYdacEtBA8XttLv7evZkira2i";
    const auto expected_uri = std::string("bitcoin:") + expected_payment;

    bitcoin_uri uri;
    BOOST_REQUIRE(uri.set_path(expected_payment));
    BOOST_REQUIRE_EQUAL(uri.encoded(), expected_uri);
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__set_path__reset_stealth_after_payment__expected_encoding)
{
    const auto expected_stealth = "hfFGUXFPKkQ5M6LC6aEUKMsURdhw93bUdYdacEtBA8XttLv7evZkira2i";
    const auto expected_uri = std::string("bitcoin:") + expected_stealth;

    bitcoin_uri uri;
    const auto payment = payment_address("113Pfw4sFqN1T5kXUnKbqZHMJHN9oyjtgD");
    BOOST_REQUIRE(payment);
    uri.set_address(payment);
    uri.set_address(stealth_address(expected_stealth));
    BOOST_REQUIRE_EQUAL(uri.encoded(), expected_uri);
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__set_path__reset_payment_after_stealth__expected_encoding)
{
    const auto expected_payment = "113Pfw4sFqN1T5kXUnKbqZHMJHN9oyjtgD";
    const auto expected_uri = std::string("bitcoin:") + expected_payment;

    bitcoin_uri uri;
    const auto stealth = stealth_address("hfFGUXFPKkQ5M6LC6aEUKMsURdhw93bUdYdacEtBA8XttLv7evZkira2i");
    BOOST_REQUIRE(stealth);
    uri.set_address(stealth);
    uri.set_address(payment_address(expected_payment));
    BOOST_REQUIRE_EQUAL(uri.encoded(), expected_uri);
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__set_path__reset_path__false)
{
    const auto expected_payment = "113Pfw4sFqN1T5kXUnKbqZHMJHN9oyjtgD";
    const auto expected_uri = std::string("bitcoin:") + expected_payment;

    bitcoin_uri uri;
    uri.set_address(stealth_address("hfFGUXFPKkQ5M6LC6aEUKMsURdhw93bUdYdacEtBA8XttLv7evZkira2i"));

    // The set_path will not reset a path. This is necessary to catch failures in non-strict parsing.
    BOOST_REQUIRE(!uri.set_path(expected_payment));
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__set_amount__reset_amount__latter_amount)
{
    bitcoin_uri uri;
    uri.set_amount(10000000000);
    uri.set_amount(120000);
    BOOST_REQUIRE_EQUAL(uri.encoded(), "bitcoin:?amount=0.0012");
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__all_setters__complex_uri__expected_encoding)
{
    bitcoin_uri uri;
    uri.set_path("113Pfw4sFqN1T5kXUnKbqZHMJHN9oyjtgD");
    uri.set_amount(120000);
    uri.set_label("&=\n");
    uri.set_message("hello bitcoin");
    uri.set_r("http://example.com?purchase=shoes&user=bob");

    BOOST_REQUIRE_EQUAL(uri.encoded(),
        "bitcoin:113Pfw4sFqN1T5kXUnKbqZHMJHN9oyjtgD?"
        "amount=0.0012&"
        "label=%26%3D%0A&"
        "message=hello%20bitcoin&"
        "r=http://example.com?purchase%3Dshoes%26user%3Dbob");
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__set_parameter__amount_denormalized__normalized)
{
    bitcoin_uri uri;
    BOOST_REQUIRE(uri.set_parameter("amount", ".0012"));
    BOOST_REQUIRE_EQUAL(uri.encoded(), "bitcoin:?amount=0.0012");
}

// Getters
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(bitcoin_uri__amount__set__expected)
{
    BOOST_REQUIRE_EQUAL(bitcoin_uri("bitcoin:?amount=0.0012").amount(), 120000u);
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__label__escaped__expected)
{
    BOOST_REQUIRE_EQUAL(bitcoin_uri("bitcoin:?label=%26%3D%0A").label(), "&=\n");
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__message__escaped__expected)
{
    BOOST_REQUIRE_EQUAL(bitcoin_uri("bitcoin:?message=hello%20bitcoin").message(), "hello bitcoin");
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__r__escaped__expected)
{
    BOOST_REQUIRE_EQUAL(bitcoin_uri("bitcoin:?r=http://example.com?purchase%3Dshoes%26user%3Dbob").r(), "http://example.com?purchase=shoes&user=bob");
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__payment__valid__expected)
{
    const auto expected_payment = "113Pfw4sFqN1T5kXUnKbqZHMJHN9oyjtgD";
    const auto expected_uri = std::string("bitcoin:") + expected_payment;
    BOOST_REQUIRE_EQUAL(bitcoin_uri(expected_uri).payment().encoded(), expected_payment);
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__stealth__valid__expected)
{
    const auto expected_stealth = "hfFGUXFPKkQ5M6LC6aEUKMsURdhw93bUdYdacEtBA8XttLv7evZkira2i";
    const auto expected_uri = std::string("bitcoin:") + expected_stealth;
    BOOST_REQUIRE_EQUAL(bitcoin_uri(expected_uri).stealth().encoded(), expected_stealth);
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__address__payment__expected)
{
    const auto expected_payment = "113Pfw4sFqN1T5kXUnKbqZHMJHN9oyjtgD";
    const auto expected_uri = std::string("bitcoin:") + expected_payment;
    BOOST_REQUIRE_EQUAL(bitcoin_uri(expected_uri).address(), expected_payment);
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__address__stealth__expected)
{
    const auto expected_stealth = "hfFGUXFPKkQ5M6LC6aEUKMsURdhw93bUdYdacEtBA8XttLv7evZkira2i";
    const auto expected_uri = std::string("bitcoin:") + expected_stealth;
    BOOST_REQUIRE_EQUAL(bitcoin_uri(expected_uri).address(), expected_stealth);
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__parameter_amount__denormalized__normalized)
{
    BOOST_REQUIRE_EQUAL(bitcoin_uri("bitcoin:?amount=.0012").parameter("amount"), "0.0012");
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__parameters_all__complex_uri__expected)
{
    bitcoin_uri uri(
        "bitcoin:113Pfw4sFqN1T5kXUnKbqZHMJHN9oyjtgD?"
        "amount=0.0012&"
        "label=%26%3D%0A&"
        "message=hello%20bitcoin&"
        "r=http://example.com?purchase%3Dshoes%26user%3Dbob");

    BOOST_REQUIRE_EQUAL(uri.address(), "113Pfw4sFqN1T5kXUnKbqZHMJHN9oyjtgD");
    BOOST_REQUIRE_EQUAL(uri.parameter("amount"), "0.0012");
    BOOST_REQUIRE_EQUAL(uri.parameter("label"), "&=\n");
    BOOST_REQUIRE_EQUAL(uri.parameter("message"), "hello bitcoin");
    BOOST_REQUIRE_EQUAL(uri.parameter("r"), "http://example.com?purchase=shoes&user=bob");
}

BOOST_AUTO_TEST_SUITE_END()
