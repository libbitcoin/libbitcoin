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
#include "../../test.hpp"
#include "electrum_v1.hpp"

BOOST_AUTO_TEST_SUITE(catalogs_electrum_v1_tests)

using namespace test::catalogs_electrum_v1;
using namespace bc::system::words;

const auto dictionary_count = 2u;
const auto dictionary_size = 1626;
const auto intersection_en_pt = 5;
const auto intersection_en_electrum_en = 861;
const auto intersection_en_electrum_es = 7;
const auto intersection_en_electrum_fr = 50;
const auto intersection_en_electrum_cs = 2;
const auto intersection_en_electrum_pt = 1;
const auto intersection_pt_electrum_en = 14;
const auto intersection_pt_electrum_es = 69;
const auto intersection_pt_electrum_it = 42;
const auto intersection_pt_electrum_fr = 7;
const auto intersection_pt_electrum_cs = 2;
const auto intersection_pt_electrum_pt = 185;

// These are not "reversed" as with typical bitcoin hash presentations.
const auto identity_en = base16_array("430ce8fc15a42a6097782a10144d1277e2596b7591be338254a257dfcf1f57d6");
const auto identity_pt = base16_array("43da9ad5a079483e224bcdd61cd1728a68c2b2647f0f1b87832dbaab5e159a12");

// count

BOOST_AUTO_TEST_CASE(catalogs_electrum_v1__count__all__expected)
{
    // Any new dictionary must be added below to guarantee lack of normalization.
    // Failure to do so may lead to invalid seed generation, which is very bad.
    BOOST_REQUIRE_MESSAGE(electrum_v1::catalogs::count() == dictionary_count, "new dictionary");
}

// identity

BOOST_AUTO_TEST_CASE(catalogs_electrum_v1__all__identity__expected)
{
    BOOST_REQUIRE_EQUAL(identity(electrum_v1::en), identity_en);
    BOOST_REQUIRE_EQUAL(identity(electrum_v1::pt), identity_pt);
}

// distinct

BOOST_AUTO_TEST_CASE(catalogs_electrum_v1__all__distinct__true)
{
    BOOST_REQUIRE(distinct(electrum_v1::en));
    BOOST_REQUIRE(distinct(electrum_v1::pt));
}

// sorted

BOOST_AUTO_TEST_CASE(catalogs_electrum_v1__sorted__unsorted__false)
{
    // Compare is not locale-aware, but these are consistent across encodings.
    BOOST_REQUIRE(!sorted8(electrum_v1::pt) && !electrum_v1::pt.sorted);
    BOOST_REQUIRE(!sorted8(electrum_v1::en) && !electrum_v1::en.sorted);
    BOOST_REQUIRE(!sorted16(electrum_v1::pt) && !electrum_v1::pt.sorted);
    BOOST_REQUIRE(!sorted16(electrum_v1::en) && !electrum_v1::en.sorted);
    BOOST_REQUIRE(!sorted32(electrum_v1::pt) && !electrum_v1::pt.sorted);
    BOOST_REQUIRE(!sorted32(electrum_v1::en) && !electrum_v1::en.sorted);
}

// abnormal

// These dictionaries are in normal form.
// So there is no need to nfkd normalize these for wordlist-based seedings.
// This also removes the ICU dependency for these language.
BOOST_AUTO_TEST_CASE(catalogs_electrum_v1__normal__normal_words__true)
{
    // The result is definitive only when WITH_ICU is defined.
    BOOST_REQUIRE(!abnormal(electrum_v1::en));
    BOOST_REQUIRE(!abnormal(electrum_v1::pt));
}

// intersection

BOOST_AUTO_TEST_CASE(catalogs_electrum_v1__intersections__en__expected)
{
    BOOST_REQUIRE_EQUAL(intersection(electrum_v1::en, electrum_v1::en), dictionary_size);
    BOOST_REQUIRE_EQUAL(intersection(electrum_v1::en, electrum_v1::pt), intersection_en_pt);
}

BOOST_AUTO_TEST_CASE(catalogs_electrum_v1__intersections__pt__expected)
{
    ////BOOST_REQUIRE_EQUAL(intersection(electrum_v1::pt, electrum_v1::en), intersection_en_pt);
    BOOST_REQUIRE_EQUAL(intersection(electrum_v1::pt, electrum_v1::pt), dictionary_size);
}

BOOST_AUTO_TEST_CASE(catalogs_electrum_v1__intersections__en_electrum__expected)
{
    BOOST_REQUIRE_EQUAL(intersection(electrum_v1::en, electrum::en), intersection_en_electrum_en);
    BOOST_REQUIRE_EQUAL(intersection(electrum_v1::en, electrum::es), intersection_en_electrum_es);
    BOOST_REQUIRE(!intersects(electrum_v1::en, electrum::it));
    BOOST_REQUIRE_EQUAL(intersection(electrum_v1::en, electrum::fr), intersection_en_electrum_fr);
    BOOST_REQUIRE_EQUAL(intersection(electrum_v1::en, electrum::cs), intersection_en_electrum_cs);
    BOOST_REQUIRE_EQUAL(intersection(electrum_v1::en, electrum::pt), intersection_en_electrum_pt);
    BOOST_REQUIRE(!intersects(electrum_v1::en, electrum::ja));
    BOOST_REQUIRE(!intersects(electrum_v1::en, electrum::ko));
    BOOST_REQUIRE(!intersects(electrum_v1::en, electrum::zh_Hans));
    BOOST_REQUIRE(!intersects(electrum_v1::en, electrum::zh_Hant));
}

BOOST_AUTO_TEST_CASE(catalogs_electrum_v1__intersections__pt_electrum__expected)
{
    BOOST_REQUIRE_EQUAL(intersection(electrum_v1::pt, electrum::en), intersection_pt_electrum_en);
    BOOST_REQUIRE_EQUAL(intersection(electrum_v1::pt, electrum::es), intersection_pt_electrum_es);
    BOOST_REQUIRE_EQUAL(intersection(electrum_v1::pt, electrum::it), intersection_pt_electrum_it);
    BOOST_REQUIRE_EQUAL(intersection(electrum_v1::pt, electrum::fr), intersection_pt_electrum_fr);
    BOOST_REQUIRE_EQUAL(intersection(electrum_v1::pt, electrum::cs), intersection_pt_electrum_cs);
    BOOST_REQUIRE_EQUAL(intersection(electrum_v1::pt, electrum::pt), intersection_pt_electrum_pt);
    BOOST_REQUIRE(!intersects(electrum_v1::pt, electrum::ja));
    BOOST_REQUIRE(!intersects(electrum_v1::pt, electrum::ko));
    BOOST_REQUIRE(!intersects(electrum_v1::pt, electrum::zh_Hans));
    BOOST_REQUIRE(!intersects(electrum_v1::pt, electrum::zh_Hant));
}

BOOST_AUTO_TEST_SUITE_END()
