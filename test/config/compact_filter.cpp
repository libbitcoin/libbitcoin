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

// Sponsored in part by Digital Contract Design, LLC

#include "../test.hpp"
#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>

BOOST_AUTO_TEST_SUITE(compact_filter_tests)

using namespace bc::system::config;

static const std::string encoded_genesis_filter =
    "00"
    "000000000933ea01ad0ee984209779baaec3ced90fa3f408719526f8d77f4943"
    "04"
    "019dfca8";

static const auto genesis_filter = compact_filter(encoded_genesis_filter);

// ------------------------------------------------------------------------- //

BOOST_AUTO_TEST_SUITE(compact_filter__construct)

BOOST_AUTO_TEST_CASE(compact_filter__construct__default)
{
    const compact_filter filter;
    BOOST_REQUIRE_EQUAL(filter.to_string(), std::string(68, '0'));
}

BOOST_AUTO_TEST_CASE(compact_filter__construct__copy__expected)
{
    const compact_filter filter(genesis_filter);
    BOOST_REQUIRE_EQUAL(filter, genesis_filter);
}

BOOST_AUTO_TEST_CASE(compact_filter__copy_assign__always__expected)
{
    compact_filter filter;
    filter = genesis_filter;
    BOOST_REQUIRE_EQUAL(filter, genesis_filter);
}

BOOST_AUTO_TEST_CASE(compact_filter__construct__string__expected)
{
    const compact_filter filter(encoded_genesis_filter);
    BOOST_REQUIRE_EQUAL(filter, genesis_filter);
}

BOOST_AUTO_TEST_SUITE_END()

// ------------------------------------------------------------------------- //

BOOST_AUTO_TEST_SUITE(compact_filter__istream)

BOOST_AUTO_TEST_CASE(compact_filter__istream__populated__expected)
{
    compact_filter deserialized;
    std::stringstream serialized(encoded_genesis_filter);
    serialized >> deserialized;
    BOOST_REQUIRE_EQUAL(deserialized.to_string(), encoded_genesis_filter);
}

BOOST_AUTO_TEST_SUITE_END()

// ------------------------------------------------------------------------- //

BOOST_AUTO_TEST_SUITE(compact_filter__ostream)

BOOST_AUTO_TEST_CASE(compact_filter__ostream__empty__expected)
{
    std::stringstream serialized;
    serialized << compact_filter();
    BOOST_REQUIRE_EQUAL(serialized.str(), std::string(68, '0'));
}

BOOST_AUTO_TEST_CASE(compact_filter__ostream__populated__expected)
{
    std::stringstream serialized;
    serialized << genesis_filter;
    BOOST_REQUIRE_EQUAL(serialized.str(), encoded_genesis_filter);
}

BOOST_AUTO_TEST_CASE(compact_filter__ostream__boost_lexical_cast__expected)
{
    const auto serialized = boost::lexical_cast<std::string>(genesis_filter);
    BOOST_REQUIRE_EQUAL(serialized, encoded_genesis_filter);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
