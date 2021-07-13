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
#include "electrum.hpp"

BOOST_AUTO_TEST_SUITE(catalogs_electrum_tests)

using namespace test::catalogs_electrum;
using namespace bc::system::words;

const auto dictionary_count = 10u;

// The french combining characters are diacritics in nfkd (normal) form.
const auto combinings_es = 334;
const auto combinings_fr = 366;
const auto combinings_ja = 644;

#ifdef WITH_ICU
const auto abnormals_es = 334;
const auto abnormals_ja = 644;
const auto divergences_es = 334;
const auto divergences_ja = 644;
#else
const auto abnormals_es = 0;
const auto abnormals_ja = 0;
const auto divergences_es = 334;
const auto divergences_ja = 644;
#endif

// count

BOOST_AUTO_TEST_CASE(catalogs_electrum__count__all__expected)
{
    // Any new dictionary must be added below to guarantee lack of normalization.
    // Failure to do so may lead to invalid seed generation, which is very bad.
    BOOST_REQUIRE_MESSAGE(electrum::catalogs::count() == dictionary_count, "new dictionary");
}

// subset

// Identities are the same as the mnemonic dictionaries.
// nfkd testing is performed on the mnemonic dictionaries.
// This test ensures electrum dictionaries are a subset of mnemonic dictionaries.
BOOST_AUTO_TEST_CASE(catalogs_electrum__mnemonic__subset__true)
{
    BOOST_REQUIRE_EQUAL(electrum::en.word, mnemonic::en.word);
    BOOST_REQUIRE_EQUAL(electrum::es.word, mnemonic::es.word);
    BOOST_REQUIRE_EQUAL(electrum::it.word, mnemonic::it.word);
    BOOST_REQUIRE_EQUAL(electrum::fr.word, mnemonic::fr.word);
    BOOST_REQUIRE_EQUAL(electrum::cs.word, mnemonic::cs.word);
    BOOST_REQUIRE_EQUAL(electrum::pt.word, mnemonic::pt.word);
    BOOST_REQUIRE_EQUAL(electrum::ja.word, mnemonic::ja.word);
    BOOST_REQUIRE_EQUAL(electrum::ko.word, mnemonic::ko.word);
    BOOST_REQUIRE_EQUAL(electrum::zh_Hans.word, mnemonic::zh_Hans.word);
    BOOST_REQUIRE_EQUAL(electrum::zh_Hant.word, mnemonic::zh_Hant.word);
}
// combined

// The spanish, french and japanese dictionaries contain combining characters.
// This requires combinings removal in these (only) for wordlist-based seedings.
BOOST_AUTO_TEST_CASE(catalogs_electrum__combinings__combininged_words__true)
{
    BOOST_REQUIRE_EQUAL(combinings(electrum::es), combinings_es);
    BOOST_REQUIRE_EQUAL(combinings(electrum::fr), combinings_fr);
    BOOST_REQUIRE_EQUAL(combinings(electrum::ja), combinings_ja);
}

// No words in these dictionaries contain combining characters.
// So there is no need to normalize combinings these for wordlist-based seedings.
BOOST_AUTO_TEST_CASE(catalogs_electrum__combined__not_combininged_words__false)
{
    BOOST_REQUIRE(!combined(electrum::en));
    BOOST_REQUIRE(!combined(electrum::it));
    BOOST_REQUIRE(!combined(electrum::cs));
    BOOST_REQUIRE(!combined(electrum::pt));
    BOOST_REQUIRE(!combined(electrum::ko));
    BOOST_REQUIRE(!combined(electrum::zh_Hans));
    BOOST_REQUIRE(!combined(electrum::zh_Hant));
}

// compressed_cjk

// No words in any supported dictionaries are uncompressed.
// So there is no need to normalize compression for wordlist-based seeding.
BOOST_AUTO_TEST_CASE(catalogs_electrum__compressed__not_compressed_words__false)
{
    BOOST_REQUIRE(!compressed(electrum::en));
    BOOST_REQUIRE(!compressed(electrum::es));
    BOOST_REQUIRE(!compressed(electrum::it));
    BOOST_REQUIRE(!compressed(electrum::fr));
    BOOST_REQUIRE(!compressed(electrum::cs));
    BOOST_REQUIRE(!compressed(electrum::pt));
    BOOST_REQUIRE(!compressed(electrum::ja));
    BOOST_REQUIRE(!compressed(electrum::ko));
    BOOST_REQUIRE(!compressed(electrum::zh_Hans));
    BOOST_REQUIRE(!compressed(electrum::zh_Hant));
}

// abnormal
// ----------------------------------------------------------------------------
// These dictionaries from the electrum repo are not in nfkd form, but are
// otherwise identical to BIP39 dictionaries. We do not use these words within
// electrum sources, we use BIP39 words. These tests verify that the BIP39
// dictionaries are functionally identical.

// This verifies the number of abnormals in the two divergent dictionaries.
BOOST_AUTO_TEST_CASE(catalogs_electrum__abnormal__unused_words__false)
{
    // The result is definitive only when WITH_ICU is defined.
    BOOST_REQUIRE_EQUAL(abnormals(electrum_es), abnormals_es);
    BOOST_REQUIRE_EQUAL(abnormals(electrum_ja), abnormals_ja);
}

// divergence

// This verifies that the differences are only in nfkd normalization.
// Otherwise there could be differences in word value and/or position.
BOOST_AUTO_TEST_CASE(catalogs_electrum__divergences__unused_words__false)
{
    BOOST_REQUIRE_EQUAL(divergences(electrum::es, electrum_es), divergences_es);
    BOOST_REQUIRE_EQUAL(divergences(electrum::ja, electrum_ja), divergences_ja);
}

BOOST_AUTO_TEST_SUITE_END()
