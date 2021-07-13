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

BOOST_AUTO_TEST_SUITE(uri_tests)

using namespace bc::system::wallet;

BOOST_AUTO_TEST_CASE(uri__parse__http_roundtrip__test)
{
    const auto test = "http://github.com/libbitcoin?good=true#nice";
    uri parsed;
    BOOST_REQUIRE(parsed.decode(test));

    BOOST_REQUIRE(parsed.has_authority());
    BOOST_REQUIRE(parsed.has_query());
    BOOST_REQUIRE(parsed.has_fragment());

    BOOST_REQUIRE_EQUAL(parsed.scheme(), "http");
    BOOST_REQUIRE_EQUAL(parsed.authority(), "github.com");
    BOOST_REQUIRE_EQUAL(parsed.path(), "/libbitcoin");
    BOOST_REQUIRE_EQUAL(parsed.query(), "good=true");
    BOOST_REQUIRE_EQUAL(parsed.fragment(), "nice");

    BOOST_REQUIRE_EQUAL(parsed.encoded(), test);
}

BOOST_AUTO_TEST_CASE(uri__parse__messy_roundtrip__test)
{
    const auto test = "TEST:%78?%79#%7a";
    uri parsed;
    BOOST_REQUIRE(parsed.decode(test));

    BOOST_REQUIRE(!parsed.has_authority());
    BOOST_REQUIRE(parsed.has_query());
    BOOST_REQUIRE(parsed.has_fragment());

    BOOST_REQUIRE_EQUAL(parsed.scheme(), "test");
    BOOST_REQUIRE_EQUAL(parsed.path(), "x");
    BOOST_REQUIRE_EQUAL(parsed.query(), "y");
    BOOST_REQUIRE_EQUAL(parsed.fragment(), "z");

    BOOST_REQUIRE_EQUAL(parsed.encoded(), test);
}

BOOST_AUTO_TEST_CASE(uri__parse__scheme__test)
{
    uri parsed;
    BOOST_REQUIRE(!parsed.decode(""));
    BOOST_REQUIRE(!parsed.decode(":"));
    BOOST_REQUIRE(!parsed.decode("1:"));
    BOOST_REQUIRE(!parsed.decode("%78:"));

    BOOST_REQUIRE(parsed.decode("x:"));
    BOOST_REQUIRE_EQUAL(parsed.scheme(), "x");

    BOOST_REQUIRE(parsed.decode("x::"));
    BOOST_REQUIRE_EQUAL(parsed.scheme(), "x");
    BOOST_REQUIRE_EQUAL(parsed.path(), ":");
}

BOOST_AUTO_TEST_CASE(uri__parsing__non_strict__test)
{
    uri parsed;
    BOOST_REQUIRE(!parsed.decode("test:?テスト"));

    BOOST_REQUIRE(parsed.decode("test:テスト", false));
    BOOST_REQUIRE_EQUAL(parsed.scheme(), "test");
    BOOST_REQUIRE_EQUAL(parsed.path(), "テスト");
}

BOOST_AUTO_TEST_CASE(uri__parse__authority__test)
{
    uri parsed;
    BOOST_REQUIRE(parsed.decode("test:/"));
    BOOST_REQUIRE(!parsed.has_authority());
    BOOST_REQUIRE_EQUAL(parsed.path(), "/");

    BOOST_REQUIRE(parsed.decode("test://"));
    BOOST_REQUIRE(parsed.has_authority());
    BOOST_REQUIRE_EQUAL(parsed.authority(), "");
    BOOST_REQUIRE_EQUAL(parsed.path(), "");

    BOOST_REQUIRE(parsed.decode("test:///"));
    BOOST_REQUIRE(parsed.has_authority());
    BOOST_REQUIRE_EQUAL(parsed.authority(), "");
    BOOST_REQUIRE_EQUAL(parsed.path(), "/");

    BOOST_REQUIRE(parsed.decode("test:/x//"));
    BOOST_REQUIRE(!parsed.has_authority());
    BOOST_REQUIRE_EQUAL(parsed.path(), "/x//");

    BOOST_REQUIRE(parsed.decode("ssh://git@github.com:22/path/"));
    BOOST_REQUIRE(parsed.has_authority());
    BOOST_REQUIRE_EQUAL(parsed.authority(), "git@github.com:22");
    BOOST_REQUIRE_EQUAL(parsed.path(), "/path/");
}

BOOST_AUTO_TEST_CASE(uri__parse__query__test)
{
    uri parsed;
    BOOST_REQUIRE(parsed.decode("test:#?"));
    BOOST_REQUIRE(!parsed.has_query());

    BOOST_REQUIRE(parsed.decode("test:?&&x=y&z"));
    BOOST_REQUIRE(parsed.has_query());
    BOOST_REQUIRE_EQUAL(parsed.query(), "&&x=y&z");

    auto map = parsed.decode_query();
    BOOST_REQUIRE(map.end() != map.find(""));
    BOOST_REQUIRE(map.end() != map.find("x"));
    BOOST_REQUIRE(map.end() != map.find("z"));
    BOOST_REQUIRE(map.end() == map.find("y"));

    BOOST_REQUIRE_EQUAL(map[""], "");
    BOOST_REQUIRE_EQUAL(map["x"], "y");
    BOOST_REQUIRE_EQUAL(map["z"], "");
}

BOOST_AUTO_TEST_CASE(uri__parse__fragment__test)
{
    uri parsed;
    BOOST_REQUIRE(parsed.decode("test:?"));
    BOOST_REQUIRE(!parsed.has_fragment());

    BOOST_REQUIRE(parsed.decode("test:#"));
    BOOST_REQUIRE(parsed.has_fragment());
    BOOST_REQUIRE_EQUAL(parsed.fragment(), "");

    BOOST_REQUIRE(parsed.decode("test:#?"));
    BOOST_REQUIRE(parsed.has_fragment());
    BOOST_REQUIRE_EQUAL(parsed.fragment(), "?");
}

BOOST_AUTO_TEST_CASE(uri__encode__positive__test)
{
    uri out;
    out.set_scheme("test");
    out.set_authority("user@hostname");
    out.set_path("/some/path/?/#");
    out.set_query("tacos=yummy");
    out.set_fragment("good evening");
    BOOST_REQUIRE_EQUAL(out.encoded(), "test://user@hostname/some/path/%3F/%23?tacos=yummy#good%20evening");

    out.remove_authority();
    out.remove_query();
    out.remove_fragment();
    BOOST_REQUIRE_EQUAL(out.encoded(), "test:/some/path/%3F/%23");
}

BOOST_AUTO_TEST_SUITE_END()
