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
#include <sstream>
#include <boost/program_options.hpp>

BOOST_AUTO_TEST_SUITE(endpoint_tests)

using namespace bc::system::config;
using namespace boost::program_options;

// ------------------------------------------------------------------------- //

BOOST_AUTO_TEST_SUITE(endpoint__construct)

BOOST_AUTO_TEST_CASE(endpoint__construct__empty__throws_invalid_option)
{
    BOOST_REQUIRE_THROW(endpoint url(""), invalid_option_value);
}

BOOST_AUTO_TEST_CASE(endpoint__construct__no_host__throws_invalid_option_value)
{
    BOOST_REQUIRE_THROW(endpoint host("tcp://"), invalid_option_value);
}

BOOST_AUTO_TEST_CASE(endpoint__construct__port_only__throws_invalid_option_value)
{
    BOOST_REQUIRE_THROW(endpoint host(":42"), invalid_option_value);
}

BOOST_AUTO_TEST_CASE(endpoint__construct__default__localhost)
{
    endpoint host;
    BOOST_REQUIRE_EQUAL(host.scheme(), "");
    BOOST_REQUIRE_EQUAL(host.host(), "localhost");
    BOOST_REQUIRE_EQUAL(host.port(), 0u);
}

BOOST_AUTO_TEST_CASE(endpoint__construct__host__expected_values)
{
    endpoint host("foo");
    BOOST_REQUIRE_EQUAL(host.scheme(), "");
    BOOST_REQUIRE_EQUAL(host.host(), "foo");
    BOOST_REQUIRE_EQUAL(host.port(), 0u);
}

BOOST_AUTO_TEST_CASE(endpoint__construct__host_port__expected_values)
{
    endpoint endpoint("foo.bar:42");
    BOOST_REQUIRE_EQUAL(endpoint.scheme(), "");
    BOOST_REQUIRE_EQUAL(endpoint.host(), "foo.bar");
    BOOST_REQUIRE_EQUAL(endpoint.port(), 42u);
}

BOOST_AUTO_TEST_CASE(endpoint__construct__scheme_host_port__expected_values)
{
    endpoint host("tcp://foo.bar:42");
    BOOST_REQUIRE_EQUAL(host.scheme(), "tcp");
    BOOST_REQUIRE_EQUAL(host.host(), "foo.bar");
    BOOST_REQUIRE_EQUAL(host.port(), 42u);
}

BOOST_AUTO_TEST_CASE(endpoint__construct__scheme_host__expected_values)
{
    endpoint host("tcp://foo.bar");
    BOOST_REQUIRE_EQUAL(host.scheme(), "tcp");
    BOOST_REQUIRE_EQUAL(host.host(), "foo.bar");
    BOOST_REQUIRE_EQUAL(host.port(), 0u);
}

BOOST_AUTO_TEST_CASE(endpoint__to_local__scheme_host_port__expected_values)
{
    endpoint original("tcp://*:12345");
    const auto host = original.to_local();
    BOOST_REQUIRE_EQUAL(host.scheme(), "tcp");
    BOOST_REQUIRE_EQUAL(host.host(), "localhost");
    BOOST_REQUIRE_EQUAL(host.port(), 12345u);
}

BOOST_AUTO_TEST_CASE(endpoint__to_local__host_port__expected_values)
{
    endpoint original("*:12345");
    const auto host = original.to_local();
    BOOST_REQUIRE_EQUAL(host.scheme(), "");
    BOOST_REQUIRE_EQUAL(host.host(), "localhost");
    BOOST_REQUIRE_EQUAL(host.port(), 12345u);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
