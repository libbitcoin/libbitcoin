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
#include "test.hpp"

BOOST_AUTO_TEST_SUITE(properties_tests)

BOOST_AUTO_TEST_CASE(properties__property_tree__value__expected_json)
{
    const std::string json
    {
        "{\"sequence\":\"101\",\"command\":\"query fetch-tx\",\"arguments\":\"4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b\"}"
    };
    boost::property_tree::ptree tree;
    const auto result = property_tree(tree, json);
    BOOST_REQUIRE_EQUAL(result, true);
    BOOST_REQUIRE_EQUAL(tree.get<uint32_t>("sequence"), 101u);
    BOOST_REQUIRE_EQUAL(tree.get<std::string>("command"), std::string("query fetch-tx"));
    BOOST_REQUIRE_EQUAL(tree.get<std::string>("arguments"), std::string("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"));
}

BOOST_AUTO_TEST_CASE(properties__property_tree__value__expected_empty)
{
    const std::string invalid_json
    {
        "{\"sequence\":\"101\" \"arguments\";\"test\"}"
    };
    boost::property_tree::ptree tree;
    const auto result = property_tree(tree, invalid_json);
    BOOST_REQUIRE_EQUAL(result, false);
}

BOOST_AUTO_TEST_SUITE_END()
