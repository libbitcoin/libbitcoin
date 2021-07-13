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
#include <boost/utility/in_place_factory.hpp>
#include <bitcoin/system/optional.hpp>

BOOST_AUTO_TEST_SUITE(uri_reader_tests)

using namespace bc::system::wallet;

// Test helper that relies on bitcoin_uri.
static bitcoin_uri parse(const std::string& uri, bool strict=true)
{
    return uri_reader::parse<bitcoin_uri>(uri, strict);
}

// Demonstrate custom uri_reader.
struct custom_reader
  : public uri_reader
{
    custom_reader()
      : strict_(true), authority_(false)
    {
    }

    bool is_valid() const
    {
        return !myscheme.empty() && !authority_;
    }

    void set_strict(bool strict)
    {
        strict_ = strict;
    }

    virtual bool set_scheme(const std::string& scheme)
    {
        myscheme = scheme;
        return true;
    }

    virtual bool set_authority(const std::string& /* authority */)
    {
        // This URI doesn't support an authority component.
        authority_ = true;
        return false;
    }

    virtual bool set_path(const std::string& path)
    {
        mypath = path;
        return true;
    }

    virtual bool set_fragment(const std::string& fragment)
    {
        myfragment = boost::in_place(fragment);
        return true;
    }

    virtual bool set_parameter(const std::string& key, const std::string& value)
    {
        if (key == "myparam1")
            myparam1 = boost::in_place(value);
        else if (key == "myparam2")
            myparam2 = boost::in_place(value);
        else
            return !strict_;

        return true;
    }

    std::string myscheme;
    std::string myauthority;
    std::string mypath;

    // Use optionals when there is a semantic distinction between no value and default value.
    optional<std::string> myfragment;
    optional<std::string> myparam1;
    optional<std::string> myparam2;

private:
    bool strict_;
    bool authority_;
};

BOOST_AUTO_TEST_CASE(uri_reader__parse__typical_uri__test)
{
    const auto uri = parse("bitcoin:113Pfw4sFqN1T5kXUnKbqZHMJHN9oyjtgD?amount=0.1");
    BOOST_REQUIRE(uri);
    BOOST_REQUIRE_EQUAL(uri.payment().encoded(), "113Pfw4sFqN1T5kXUnKbqZHMJHN9oyjtgD");
    BOOST_REQUIRE_EQUAL(uri.amount(), 10000000u);
    BOOST_REQUIRE(uri.label().empty());
    BOOST_REQUIRE(uri.message().empty());
    BOOST_REQUIRE(uri.r().empty());
}

BOOST_AUTO_TEST_CASE(uri_reader__parse__positive_scheme__test)
{
    BOOST_REQUIRE(parse("bitcoin:"));
    BOOST_REQUIRE(parse("Bitcoin:"));
    BOOST_REQUIRE(parse("bitcOin:"));
    BOOST_REQUIRE(parse("BITCOIN:"));
}

BOOST_AUTO_TEST_CASE(uri_reader__parse__negative_scheme__test)
{
    BOOST_REQUIRE(!parse("bitcorn:"));
}

BOOST_AUTO_TEST_CASE(uri_reader__parse__positive_empty_name_parameter__test)
{
    BOOST_REQUIRE(parse("bitcoin:?"));
    BOOST_REQUIRE(parse("bitcoin:?&"));
    BOOST_REQUIRE(parse("bitcoin:?=y"));
    BOOST_REQUIRE(parse("bitcoin:?="));
}

BOOST_AUTO_TEST_CASE(uri_reader__parse__positive_unknown_optional_parameter__test)
{
    BOOST_REQUIRE(parse("bitcoin:?x=y"));
    BOOST_REQUIRE(parse("bitcoin:?x="));
    BOOST_REQUIRE(parse("bitcoin:?x"));

    const auto uri = parse("bitcoin:?ignore=true");
    BOOST_REQUIRE(uri);
    BOOST_REQUIRE(uri.address().empty());
    BOOST_REQUIRE_EQUAL(uri.amount(), 0);
    BOOST_REQUIRE(uri.label().empty());
    BOOST_REQUIRE(uri.message().empty());
    BOOST_REQUIRE(uri.r().empty());
    BOOST_REQUIRE(uri.parameter("ignore").empty());
}

BOOST_AUTO_TEST_CASE(uri_reader__parse__negative_unknown_required_parameter__test)
{
    const auto uri = parse("bitcoin:?req-ignore=false");
    BOOST_REQUIRE(!uri);
}

BOOST_AUTO_TEST_CASE(uri_reader__parse__address__test)
{
    const auto uri = parse("bitcoin:113Pfw4sFqN1T5kXUnKbqZHMJHN9oyjtgD");
    BOOST_REQUIRE(uri);
    BOOST_REQUIRE_EQUAL(uri.payment().encoded(), "113Pfw4sFqN1T5kXUnKbqZHMJHN9oyjtgD");
    BOOST_REQUIRE_EQUAL(uri.amount(), 0);
    BOOST_REQUIRE(uri.label().empty());
    BOOST_REQUIRE(uri.message().empty());
    BOOST_REQUIRE(uri.r().empty());
}

BOOST_AUTO_TEST_CASE(uri_reader__parse__uri_encoded_address__test)
{
    const auto uri = parse("bitcoin:%3113Pfw4sFqN1T5kXUnKbqZHMJHN9oyjtgD");
    BOOST_REQUIRE(uri);
    BOOST_REQUIRE_EQUAL(uri.payment().encoded(), "113Pfw4sFqN1T5kXUnKbqZHMJHN9oyjtgD");
}

BOOST_AUTO_TEST_CASE(uri_reader__parse__negative_address__test)
{
    BOOST_REQUIRE(!parse("bitcoin:&"));
    BOOST_REQUIRE(!parse("bitcoin:19l88"));
    BOOST_REQUIRE(!parse("bitcoin:19z88"));
}

BOOST_AUTO_TEST_CASE(uri_reader__parse__amount_only__test)
{
    const auto uri = parse("bitcoin:?amount=4.2");
    BOOST_REQUIRE(uri);
    BOOST_REQUIRE(!uri.payment());
    BOOST_REQUIRE_EQUAL(uri.amount(), 420000000u);
    BOOST_REQUIRE(uri.label().empty());
    BOOST_REQUIRE(uri.message().empty());
    BOOST_REQUIRE(uri.r().empty());
}

BOOST_AUTO_TEST_CASE(uri_reader__parse__minimal_amount__test)
{
    const auto uri = parse("bitcoin:?amount=.");
    BOOST_REQUIRE(uri);
    BOOST_REQUIRE_EQUAL(uri.amount(), 0);
}

BOOST_AUTO_TEST_CASE(uri_reader__parse__invalid_amount__test)
{
    BOOST_REQUIRE(!parse("bitcoin:amount=4.2.1"));
    BOOST_REQUIRE(!parse("bitcoin:amount=bob"));
}

BOOST_AUTO_TEST_CASE(uri_reader__parse__label_only__test)
{
    const auto uri = parse("bitcoin:?label=test");
    BOOST_REQUIRE(uri);
    BOOST_REQUIRE(!uri.payment());
    BOOST_REQUIRE_EQUAL(uri.amount(), 0);
    BOOST_REQUIRE_EQUAL(uri.label(), "test");
    BOOST_REQUIRE(uri.message().empty());
    BOOST_REQUIRE(uri.r().empty());
}

BOOST_AUTO_TEST_CASE(uri_reader__parse__reserved_symbol_with_lowercase_percent__test)
{
    const auto uri = parse("bitcoin:?label=%26%3d%6b");
    BOOST_REQUIRE_EQUAL(uri.label(), "&=k");
}

BOOST_AUTO_TEST_CASE(uri_reader__parse__negative_percent_encoding__test)
{
    BOOST_REQUIRE(!parse("bitcoin:label=%3"));
    BOOST_REQUIRE(!parse("bitcoin:label=%3G"));
}

BOOST_AUTO_TEST_CASE(uri_reader__parse__encoded_multibyte_utf8__test)
{
    const auto uri = parse("bitcoin:?label=%E3%83%95");
    BOOST_REQUIRE_EQUAL(uri.label(), "フ");
}

BOOST_AUTO_TEST_CASE(uri_reader__parse__non_strict_encoded_multibyte_utf8_with_unencoded_label_space__test)
{
    const auto uri = parse("bitcoin:?label=Some テスト", false);
    BOOST_REQUIRE_EQUAL(uri.label(), "Some テスト");
}

BOOST_AUTO_TEST_CASE(uri_reader__parse__negative_strict_encoded_multibyte_utf8_with_unencoded_label_space__test)
{
    BOOST_REQUIRE(!parse("bitcoin:?label=Some テスト", true));
}

BOOST_AUTO_TEST_CASE(uri_reader__parse__message_only__test)
{
    const auto uri = parse("bitcoin:?message=Hi%20Alice");
    BOOST_REQUIRE(!uri.payment());
    BOOST_REQUIRE_EQUAL(uri.amount(), 0);
    BOOST_REQUIRE(uri.label().empty());
    BOOST_REQUIRE_EQUAL(uri.message(), "Hi Alice");
    BOOST_REQUIRE(uri.r().empty());
}

BOOST_AUTO_TEST_CASE(uri_reader__parse__payment_protocol_only__test)
{
    const auto uri = parse("bitcoin:?r=http://www.example.com?purchase%3Dshoes");
    BOOST_REQUIRE(!uri.payment());
    BOOST_REQUIRE_EQUAL(uri.amount(), 0);
    BOOST_REQUIRE(uri.label().empty());
    BOOST_REQUIRE(uri.message().empty());
    BOOST_REQUIRE_EQUAL(uri.r(), "http://www.example.com?purchase=shoes");
}

BOOST_AUTO_TEST_CASE(uri_reader__parse__custom_reader_optional_parameter_type__test)
{
    const auto custom = uri_reader::parse<custom_reader>("foo:part/abc?myparam1=1&myparam2=2#myfrag");
    BOOST_REQUIRE(custom.is_valid());
    BOOST_REQUIRE_EQUAL(custom.myscheme, "foo");
    BOOST_REQUIRE_EQUAL(custom.mypath, "part/abc");
    BOOST_REQUIRE(custom.myfragment);
    BOOST_REQUIRE_EQUAL(custom.myfragment.get(), "myfrag");
    BOOST_REQUIRE(custom.myparam1);
    BOOST_REQUIRE_EQUAL(custom.myparam1.get(), "1");
    BOOST_REQUIRE(custom.myparam2);
    BOOST_REQUIRE_EQUAL(custom.myparam2.get(), "2");
}

BOOST_AUTO_TEST_CASE(uri_reader__parse__custom_reader_unsupported_component__invalid)
{
    BOOST_REQUIRE(!uri_reader::parse<custom_reader>("foo://bar:42/part/abc?myparam1=1&myparam2=2#myfrag").is_valid());
}

BOOST_AUTO_TEST_CASE(uri_reader__parse__custom_reader_strict__test)
{
    BOOST_REQUIRE(!uri_reader::parse<custom_reader>("foo:?unknown=fail-when-strict").is_valid());
}

BOOST_AUTO_TEST_CASE(uri_reader__parse__custom_reader_not_strict__test)
{
    BOOST_REQUIRE(uri_reader::parse<custom_reader>("foo:?unknown=not-fail-when-not-strict", false).is_valid());
}

BOOST_AUTO_TEST_SUITE_END()
