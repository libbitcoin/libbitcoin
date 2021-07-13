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
#include <sstream>
#include <utility>
#include "electrum_v1.hpp"

BOOST_AUTO_TEST_SUITE(electrum_v1_tests)

using namespace test::mnemonics_electrum_v1;
using namespace bc::system::wallet;

#define TODO_TESTS true
#define DECODING_CLASS 1
#define PUBLIC_STATIC 1
#define PROTECTED_STATIC 1
#define CONSTRUCTORS 1
#define PUBLIC_METHODS 1
#define PROTECTED_METHODS 1
#define OPERATORS 1
#define VERIFIED_VECTORS 1

#ifdef DECODING_CLASS

BOOST_AUTO_TEST_CASE(v1_decoding__construct_default__always__empty)
{
    v1_decoding instance;
    BOOST_REQUIRE(instance.entropy().empty());
    BOOST_REQUIRE(instance.overflows().empty());
    BOOST_REQUIRE(instance.seed_entropy().empty());
}

BOOST_AUTO_TEST_CASE(v1_decoding__construct_entropy__empty__empty)
{
    v1_decoding instance(data_chunk{});
    BOOST_REQUIRE(instance.entropy().empty());
    BOOST_REQUIRE(instance.overflows().empty());
    BOOST_REQUIRE(instance.seed_entropy().empty());
}

BOOST_AUTO_TEST_CASE(v1_decoding__construct_entropy__non_empty__expected)
{
    const data_chunk entropy(4, 0x42);
    const auto expected_seed = to_chunk("42424242");
    v1_decoding instance(entropy);
    BOOST_REQUIRE_EQUAL(instance.entropy(), entropy);
    BOOST_REQUIRE(instance.overflows().empty());
    BOOST_REQUIRE_EQUAL(instance.seed_entropy(), expected_seed);
}

BOOST_AUTO_TEST_CASE(v1_decoding__construct__empty__expected)
{
    v1_decoding instance({}, {});
    BOOST_REQUIRE(instance.entropy().empty());
    BOOST_REQUIRE(instance.overflows().empty());
    BOOST_REQUIRE(instance.seed_entropy().empty());
}

BOOST_AUTO_TEST_CASE(v1_decoding__construct__empty_entropy__expected)
{
    const v1_decoding::overflow overflows{ true, false, true };
    v1_decoding instance({}, overflows);
    BOOST_REQUIRE(instance.entropy().empty());
    BOOST_REQUIRE_EQUAL(instance.overflows(), overflows);
    BOOST_REQUIRE(instance.seed_entropy().empty());
}

BOOST_AUTO_TEST_CASE(v1_decoding__seed_entropy__mismatched_overflow__expected)
{
    const auto entropy = data_chunk(3 * sizeof(uint32_t), 0x42);
    const v1_decoding::overflow overflows(2, false);
    v1_decoding instance(entropy, overflows);
    BOOST_REQUIRE_EQUAL(instance.entropy(), entropy);
    BOOST_REQUIRE_EQUAL(instance.overflows(), overflows);
    BOOST_REQUIRE(instance.seed_entropy().empty());
}

BOOST_AUTO_TEST_CASE(v1_decoding__seed_entropy__false_overflows__expected)
{
    const data_chunk entropy(3 * sizeof(uint32_t), 0x42);

    // to_chunk(text) is not decode_base16(out, text)
    // It casts char directly to uint8_t.
    // This is the value that electrum hashes to obtain the seed.
    // We refer to this as the "seed entropy", as opposed to the "entropy".
    const auto expected_seed_entropy = to_chunk(encode_base16(entropy));

    const v1_decoding::overflow overflows(3, false);
    v1_decoding instance(entropy, overflows);
    BOOST_REQUIRE_EQUAL(instance.entropy(), entropy);
    BOOST_REQUIRE_EQUAL(instance.overflows(), overflows);
    BOOST_REQUIRE_EQUAL(instance.seed_entropy(), expected_seed_entropy);
}

BOOST_AUTO_TEST_CASE(v1_decoding__construct__true_overflows__expected)
{
    const data_chunk entropy
    {
        0xaa, 0xbb, 0xcc, 0xdd,
        0xaa, 0xbb, 0xcc, 0xdd,
        0xaa, 0xbb, 0xcc, 0xdd
    };
    const auto expected_seed_entropy = to_chunk("1""aabbccdd""1""aabbccdd""1""aabbccdd");

    const v1_decoding::overflow overflows(3, true);
    v1_decoding instance(entropy, overflows);
    BOOST_REQUIRE_EQUAL(instance.entropy(), entropy);
    BOOST_REQUIRE_EQUAL(instance.overflows(), overflows);
    BOOST_REQUIRE_EQUAL(instance.seed_entropy(), expected_seed_entropy);
}

BOOST_AUTO_TEST_CASE(v1_decoding__construct__mixed_overflows__expected)
{
    const data_chunk entropy
    {
        0xaa, 0xbb, 0xcc, 0xdd,
        0xaa, 0xbb, 0xcc, 0xdd,
        0xaa, 0xbb, 0xcc, 0xdd
    };
    const auto expected_seed_entropy = to_chunk("aabbccdd""1""aabbccddaabbccdd");
    const v1_decoding::overflow overflows{ false, true, false };
    v1_decoding instance(entropy, overflows);
    BOOST_REQUIRE_EQUAL(instance.entropy(), entropy);
    BOOST_REQUIRE_EQUAL(instance.overflows(), overflows);
    BOOST_REQUIRE_EQUAL(instance.seed_entropy(), expected_seed_entropy);
}

BOOST_AUTO_TEST_CASE(v1_decoding__construct__even_overflows__expected)
{
    const data_chunk entropy
    {
        0xaa, 0xbb, 0xcc, 0xdd,
        0xaa, 0xbb, 0xcc, 0xdd,
        0xaa, 0xbb, 0xcc, 0xdd
    };

    // This is a byte encoded example of an even number of overflow bits.
    // Note that a single overflow bit cannot be represened in byte encoding.
    // Note also that a single overflow bit consumes 4 bits in the electrum
    // encoding because of electrum's concatenation of presumed 4 byte hex
    // strings. The overflow consumes a full hex character in the string
    // encoding is then concatenated with the next chunks. Electrum's own
    // entropy decoder cannot decode an overflowed value as it successively
    // divides the entropy by 1626 (the dictionary size), which is based on the
    // assumption that each chunk is limited to [0-1625]. But overflowed values
    // violate this assumption. For these reasons we carry overflow flags in a
    // distinct data structure and truncate the overflow for entropy.
    // Fortunately the overflows are limited to one bit, which simplifies this
    // techique. For this reason overflowed entropy will not round trip to the
    // original seed words (which were invalid - manually generated).
    const data_chunk malformed_entropy
    {
        0xaa, 0xbb, 0xcc, 0xdd,
        0x1a, 0xab, 0xbc, 0xcd,
        0xd1, 0xaa, 0xbb, 0xcc, 0xdd
    };

    // This is the true true.
    const v1_decoding::overflow overflows{ false, true, true };
    v1_decoding instance(entropy, overflows);
    BOOST_REQUIRE_EQUAL(instance.entropy(), entropy);
    BOOST_REQUIRE_EQUAL(instance.overflows(), overflows);
    BOOST_REQUIRE_EQUAL(instance.seed_entropy(), to_chunk(encode_base16(malformed_entropy)));
}

#endif // DECODING_CLASS

#ifdef PUBLIC_STATIC

// contained_by

BOOST_AUTO_TEST_CASE(electrum_v1__contained_by__empty_any__true)
{
    // This is based on the order of dictionary insertion (first).
    BOOST_REQUIRE(electrum_v1::contained_by({}) == language::en);
}

BOOST_AUTO_TEST_CASE(electrum_v1__contained_by__invalid_any__none)
{
    BOOST_REQUIRE(electrum_v1::contained_by({ "foo" }) == language::none);
}

BOOST_AUTO_TEST_CASE(electrum_v1__contained_by__yellow_lotus__none)
{
    // en and pt words
    BOOST_REQUIRE(electrum_v1::contained_by({ "yellow", "lotus" }) == language::none);
}

BOOST_AUTO_TEST_CASE(electrum_v1__contained_by__blind_faith__en)
{
    BOOST_REQUIRE(electrum_v1::contained_by(words2) == language::en);
}

BOOST_AUTO_TEST_CASE(electrum_v1__contained_by__explicit_language__expected)
{
    BOOST_REQUIRE(electrum_v1::contained_by(words2, language::en) == language::en);
}

BOOST_AUTO_TEST_CASE(electrum_v1__contained_by__incorrect_explicit_language__none)
{
    BOOST_REQUIRE(electrum_v1::contained_by(words2, language::pt) == language::none);
}

BOOST_AUTO_TEST_CASE(electrum_v1__contained_by__invalid_explicit_language__none)
{
    BOOST_REQUIRE(electrum_v1::contained_by(words2, language::ko) == language::none);
}

// is_valid_dictionary

BOOST_AUTO_TEST_CASE(electrum_v1__is_valid_dictionary__contained__true)
{
    BOOST_REQUIRE(electrum_v1::is_valid_dictionary(language::en));
    BOOST_REQUIRE(electrum_v1::is_valid_dictionary(language::pt));
}

BOOST_AUTO_TEST_CASE(electrum_v1__is_valid_dictionary__uncontained__false)
{
    BOOST_REQUIRE(!electrum_v1::is_valid_dictionary(language::es));
    BOOST_REQUIRE(!electrum_v1::is_valid_dictionary(language::it));
    BOOST_REQUIRE(!electrum_v1::is_valid_dictionary(language::fr));
    BOOST_REQUIRE(!electrum_v1::is_valid_dictionary(language::cs));
    BOOST_REQUIRE(!electrum_v1::is_valid_dictionary(language::ja));
    BOOST_REQUIRE(!electrum_v1::is_valid_dictionary(language::ko));
    BOOST_REQUIRE(!electrum_v1::is_valid_dictionary(language::zh_Hans));
    BOOST_REQUIRE(!electrum_v1::is_valid_dictionary(language::zh_Hant));
    BOOST_REQUIRE(!electrum_v1::is_valid_dictionary(language::none));
}

// is_valid_entropy_size

BOOST_AUTO_TEST_CASE(electrum_v1__is_valid_entropy_size__valid__true)
{
    BOOST_REQUIRE(electrum_v1::is_valid_entropy_size(16));
    BOOST_REQUIRE(electrum_v1::is_valid_entropy_size(32));
}

BOOST_AUTO_TEST_CASE(electrum_v1__is_valid_entropy_size__invalid__false)
{
    BOOST_REQUIRE(!electrum_v1::is_valid_entropy_size(0));
    BOOST_REQUIRE(!electrum_v1::is_valid_entropy_size(8));
    BOOST_REQUIRE(!electrum_v1::is_valid_entropy_size(15));
    BOOST_REQUIRE(!electrum_v1::is_valid_entropy_size(17));
    BOOST_REQUIRE(!electrum_v1::is_valid_entropy_size(31));
    BOOST_REQUIRE(!electrum_v1::is_valid_entropy_size(33));
    BOOST_REQUIRE(!electrum_v1::is_valid_entropy_size(64));
}

// is_valid_word_count

BOOST_AUTO_TEST_CASE(electrum_v1__is_valid_word_count__valid__true)
{
    BOOST_REQUIRE(electrum_v1::is_valid_word_count(12));
    BOOST_REQUIRE(electrum_v1::is_valid_word_count(24));
}

BOOST_AUTO_TEST_CASE(electrum_v1__is_valid_word_count__invalid__false)
{
    BOOST_REQUIRE(!electrum_v1::is_valid_word_count(0));
    BOOST_REQUIRE(!electrum_v1::is_valid_word_count(3));
    BOOST_REQUIRE(!electrum_v1::is_valid_word_count(6));
    BOOST_REQUIRE(!electrum_v1::is_valid_word_count(9));
    BOOST_REQUIRE(!electrum_v1::is_valid_word_count(15));
    BOOST_REQUIRE(!electrum_v1::is_valid_word_count(18));
    BOOST_REQUIRE(!electrum_v1::is_valid_word_count(27));
}

#endif // PUBLIC_STATIC

#ifdef PROTECTED_STATIC

// coders
// ----------------------------------------------------------------------------

// encoder

BOOST_AUTO_TEST_CASE(electrum_v1__encoder__empty__empty)
{
    BOOST_REQUIRE(accessor::encoder({}, language::en).empty());
}

BOOST_AUTO_TEST_CASE(electrum_v1__encoder__size_and_language__expected)
{
    const auto size = 7u;
    const data_chunk entropy(size * sizeof(uint32_t), 0x00);
    const string_list expected(size * electrum_v1::word_multiple, "abaular");
    const auto words = accessor::encoder(entropy, language::pt);
    BOOST_REQUIRE_EQUAL(words.size(), size * electrum_v1::word_multiple);
    BOOST_REQUIRE_EQUAL(words, expected);
}

BOOST_AUTO_TEST_CASE(electrum_v1__encoder__overflow__expected)
{
    const auto entropy = base16_chunk("003ca789");
    const auto expected = split("ship baby bright");
    const auto words = accessor::encoder(entropy, language::en);
    BOOST_REQUIRE_EQUAL(words, expected);
}

BOOST_AUTO_TEST_CASE(electrum_v1__encoder__non_overflow__expected)
{
    const auto entropy = base16_chunk("025d2f2d");
    const auto expected = split("hurry idiot prefer");
    const auto words = accessor::encoder(entropy, language::en);
    BOOST_REQUIRE_EQUAL(words, expected);
}

// decoder

BOOST_AUTO_TEST_CASE(electrum_v1__decoder__empty__empty)
{
    const auto decoding = accessor::decoder({}, language::en);
    BOOST_REQUIRE(decoding.entropy().empty());
    BOOST_REQUIRE(decoding.overflows().empty());
    BOOST_REQUIRE(decoding.seed_entropy().empty());
}

BOOST_AUTO_TEST_CASE(electrum_v1__decoder__size_and_language__expected)
{
    const auto size = 7u;
    const data_chunk entropy(size * sizeof(uint32_t), 0x00);
    const auto expected = to_chunk(encode_base16(entropy));
    const v1_decoding::overflow overflows(size, false);
    const string_list words(size * electrum_v1::word_multiple, "abaular");
    const auto decoding = accessor::decoder(words, language::pt);
    BOOST_REQUIRE_EQUAL(decoding.entropy(), entropy);
    BOOST_REQUIRE_EQUAL(decoding.overflows(), overflows);
    BOOST_REQUIRE_EQUAL(decoding.seed_entropy(), expected);
}

BOOST_AUTO_TEST_CASE(electrum_v1__decoder__overflow__expected)
{
    const auto entropy = base16_chunk("025d2f2d00503691003ca78900ca155c");
    const auto expected = to_chunk("025d2f2d00503691""1""003ca78900ca155c");
    const auto words = split("hurry idiot prefer sunset mention mist jaw inhale impossible kingdom rare squeeze");
    const v1_decoding::overflow overflows{ false, false, true, false };
    const auto decoding = accessor::decoder(words, language::en);
    BOOST_REQUIRE_EQUAL(decoding.entropy(), entropy);
    BOOST_REQUIRE_EQUAL(decoding.overflows(), overflows);
    BOOST_REQUIRE_EQUAL(decoding.seed_entropy(), expected);
}

BOOST_AUTO_TEST_CASE(electrum_v1__decoder__non_overflow__expected)
{
    const auto entropy = base16_chunk("acb740e454c3134901d7c8f16497cc1c");
    const auto expected = to_chunk(encode_base16(entropy));
    const auto words = split("powerful random nobody notice nothing important anyway look away hidden message over");
    const v1_decoding::overflow overflows(4, false);
    const auto decoding = accessor::decoder(words, language::en);
    BOOST_REQUIRE_EQUAL(decoding.entropy(), entropy);
    BOOST_REQUIRE_EQUAL(decoding.overflows(), overflows);
    BOOST_REQUIRE_EQUAL(decoding.seed_entropy(), expected);
}

// strecher

BOOST_AUTO_TEST_CASE(electrum_v1__strecher__empty__expected)
{
    const auto expected = base16_array("52f429563ecbf164efe23f9f77cd00073f5677600d721a6c754dc4e41124d645");
    BOOST_REQUIRE_EQUAL(accessor::strecher({}), expected);
}

BOOST_AUTO_TEST_CASE(electrum_v1__strecher__non_empty__expected)
{
    const auto expected = base16_array("922538904d8070da6a98a44f9a19661724c775e312c4c67e2a67c0a316f7efa6");
    BOOST_REQUIRE_EQUAL(accessor::strecher(to_chunk(null_hash)), expected);
}

// sizers
// ----------------------------------------------------------------------------

// entropy_bits entropy

BOOST_AUTO_TEST_CASE(electrum_v1__entropy_bits__entropy__expected)
{
    for (size_t bytes = 0; bytes < electrum_v1::entropy_maximum; ++bytes)
    {
        BOOST_REQUIRE_EQUAL(accessor::entropy_bits(data_chunk(bytes, 0x00)), bytes * 8u);
    }
}

// entropy_bits words

BOOST_AUTO_TEST_CASE(electrum_v1__entropy_bits__words__expected)
{
    for (size_t words = 0; words < electrum_v1::word_maximum; ++words)
    {
        BOOST_REQUIRE_EQUAL(accessor::entropy_bits(string_list(words, "")), (words * 4u) / 3u);
    }
}

// entropy_size

BOOST_AUTO_TEST_CASE(electrum_v1__entropy_size__various__expected)
{
    for (size_t words = 0; words < electrum_v1::word_maximum; ++words)
    {
        BOOST_REQUIRE_EQUAL(accessor::entropy_size(string_list(words, "")), ((words * 4u) / 3u) / 8u);
    }
}

// word_count

BOOST_AUTO_TEST_CASE(electrum_v1__word_count__various__expected)
{
    for (size_t bytes = 0; bytes < electrum_v1::entropy_maximum; ++bytes)
    {
        BOOST_REQUIRE_EQUAL(accessor::word_count(data_chunk(bytes, 0x00)), (bytes * 8u) / 4u);
    }
}

// factories
// ----------------------------------------------------------------------------

// from_entropy

BOOST_AUTO_TEST_CASE(electrum_v1__from_entropy__todo__todo)
{
    BOOST_REQUIRE(TODO_TESTS);
}

// from_words

BOOST_AUTO_TEST_CASE(electrum_v1__from_words__todo__todo)
{
    BOOST_REQUIRE(TODO_TESTS);
}

#endif // PROTECTED_STATIC

#ifdef CONSTRUCTORS

// construct default

BOOST_AUTO_TEST_CASE(electrum_v1__construct_default__always__invalid)
{
    BOOST_REQUIRE(!electrum_v1());
}

// construct copy

BOOST_AUTO_TEST_CASE(electrum_v1__construct_copy__always__expected)
{
    electrum_v1 instance1(two_overflows12);
    const electrum_v1 instance2(instance1);
    BOOST_REQUIRE(instance2);
    BOOST_REQUIRE_EQUAL(instance2.words(), two_overflows12);
    BOOST_REQUIRE_EQUAL(instance2.entropy(), instance1.entropy());
    BOOST_REQUIRE(instance2.lingo() == instance1.lingo());
    BOOST_REQUIRE(instance2.overflow() == instance1.overflow());

    // Overflow entropy does not round trip words.
    const electrum_v1 instance3(instance2.entropy());
    BOOST_REQUIRE_EQUAL(instance3.entropy(), instance2.entropy());
    BOOST_REQUIRE_NE(instance3.words(), instance2.words());
    BOOST_REQUIRE(instance3.overflow() != instance2.overflow());
}

// construct move (default)

BOOST_AUTO_TEST_CASE(electrum_v1__construct_move__always__expected)
{
    electrum_v1 instance1(two_overflows12);
    const electrum_v1 instance1_copy(instance1);
    const electrum_v1 instance2(std::move(instance1));
    BOOST_REQUIRE(instance2);
    BOOST_REQUIRE_EQUAL(instance2.words(), two_overflows12);
    BOOST_REQUIRE_EQUAL(instance2.entropy(), instance1_copy.entropy());
    BOOST_REQUIRE(instance2.lingo() == instance1_copy.lingo());
    BOOST_REQUIRE(instance2.overflow() == instance1_copy.overflow());

    // Overflow entropy does not round trip words.
    const electrum_v1 instance3(instance2.entropy());
    BOOST_REQUIRE_EQUAL(instance3.entropy(), instance2.entropy());
    BOOST_REQUIRE_NE(instance3.words(), instance2.words());
    BOOST_REQUIRE(instance3.overflow() != instance2.overflow());
}

// construct sentence

BOOST_AUTO_TEST_CASE(electrum_v1__construct_sentence__empty_sentence__invalid)
{
    BOOST_REQUIRE(!electrum_v1(""));
}

BOOST_AUTO_TEST_CASE(electrum_v1__construct_sentence__2_word_sentence__invalid_empty)
{
    const electrum_v1 instance(join(words2));
    BOOST_REQUIRE(!instance);
    BOOST_REQUIRE(instance.sentence().empty());
}

BOOST_AUTO_TEST_CASE(electrum_v1__construct_sentence__12_word_sentence__valid_expected)
{
    const electrum_v1 instance(join(words12));
    BOOST_REQUIRE(instance);
    BOOST_REQUIRE_EQUAL(instance.sentence(), join(words12));
}

BOOST_AUTO_TEST_CASE(electrum_v1__construct_sentence__24_word_sentence__valid_expected)
{
    const electrum_v1 instance(join(words24));
    BOOST_REQUIRE(instance);
    BOOST_REQUIRE_EQUAL(instance.sentence(), join(words24));
}

BOOST_AUTO_TEST_CASE(electrum_v1__construct_sentence__26_word_sentence__valid_expected)
{
    const electrum_v1 instance(words26);
    BOOST_REQUIRE(!instance);
    BOOST_REQUIRE(instance.sentence().empty());
}

BOOST_AUTO_TEST_CASE(electrum_v1__construct_sentence__explicit_language__valid_expected)
{
    const electrum_v1 instance(join(words12), language::en);
    BOOST_REQUIRE(instance);
    BOOST_REQUIRE_EQUAL(instance.sentence(), join(words12));
}

BOOST_AUTO_TEST_CASE(electrum_v1__construct_sentence__incorrect_language__invalid)
{
    const electrum_v1 instance(join(words24), language::pt);
    BOOST_REQUIRE(!instance);
}

BOOST_AUTO_TEST_CASE(electrum_v1__construct_sentence__invalid_language__invalid)
{
    const electrum_v1 instance(join(words24), language::ko);
    BOOST_REQUIRE(!instance);
}

BOOST_AUTO_TEST_CASE(electrum_v1__construct_sentence__mixed_sentence__invalid)
{
    const electrum_v1 instance(join(mixed_words12), language::none);
    BOOST_REQUIRE(!instance);
}

// construct words

BOOST_AUTO_TEST_CASE(electrum_v1__construct_words__empty__invalid)
{
    BOOST_REQUIRE(!electrum_v1(string_list{}));
}

BOOST_AUTO_TEST_CASE(electrum_v1__construct_words__2_words__invalid)
{
    electrum_v1 instance(words2);
    BOOST_REQUIRE(!instance);
    BOOST_REQUIRE(instance.words().empty());
}

BOOST_AUTO_TEST_CASE(electrum_v1__construct_words__12_words__valid_expected)
{
    electrum_v1 instance(words12);
    BOOST_REQUIRE(instance);
    BOOST_REQUIRE_EQUAL(instance.words(), words12);
}

BOOST_AUTO_TEST_CASE(electrum_v1__construct_words__24_words__valid_expected)
{
    electrum_v1 instance(words24);
    BOOST_REQUIRE(instance);
    BOOST_REQUIRE_EQUAL(instance.words(), words24);
}

BOOST_AUTO_TEST_CASE(electrum_v1__construct_words__26_words__valid_expected)
{
    electrum_v1 instance(words26);
    BOOST_REQUIRE(!instance);
    BOOST_REQUIRE(instance.words().empty());
}

BOOST_AUTO_TEST_CASE(electrum_v1__construct_words__explicit_language__valid_expected)
{
    electrum_v1 instance(words12, language::en);
    BOOST_REQUIRE(instance);
    BOOST_REQUIRE_EQUAL(instance.words(), words12);
}

BOOST_AUTO_TEST_CASE(electrum_v1__construct_words__incorrect_language__invalid)
{
    electrum_v1 instance(words24, language::pt);
    BOOST_REQUIRE(!instance);
}

BOOST_AUTO_TEST_CASE(electrum_v1__construct_words__invalid_language__invalid)
{
    electrum_v1 instance(words24, language::ko);
    BOOST_REQUIRE(!instance);
}

BOOST_AUTO_TEST_CASE(electrum_v1__construct_words__mixed_words__invalid)
{
    electrum_v1 instance(mixed_words12, language::none);
    BOOST_REQUIRE(!instance);
}

// construct entropy

BOOST_AUTO_TEST_CASE(electrum_v1__construct_entropy__empty__invalid)
{
    BOOST_REQUIRE(!electrum_v1(data_chunk{}));
}

BOOST_AUTO_TEST_CASE(electrum_v1__construct_entropy__8_bytes__invalid)
{
    const data_chunk entropy(8, 0x42);
    electrum_v1 instance(entropy);
    BOOST_REQUIRE(!instance);
    BOOST_REQUIRE(instance.entropy().empty());
}

BOOST_AUTO_TEST_CASE(electrum_v1__construct_entropy__16_bytes__valid_expected)
{
    const data_chunk entropy(16, 0x42);
    electrum_v1 instance(entropy);
    BOOST_REQUIRE(instance);
    BOOST_REQUIRE_EQUAL(instance.entropy(), entropy);
}

BOOST_AUTO_TEST_CASE(electrum_v1__construct_entropy__32_bytes__valid_expected)
{
    const data_chunk entropy(32, 0x42);
    electrum_v1 instance(entropy);
    BOOST_REQUIRE(instance);
    BOOST_REQUIRE_EQUAL(instance.entropy(), entropy);
}

BOOST_AUTO_TEST_CASE(electrum_v1__construct_entropy__64_bytes__invalid)
{
    const data_chunk entropy(64, 0x42);
    electrum_v1 instance(entropy);
    BOOST_REQUIRE(!instance);
    BOOST_REQUIRE(instance.entropy().empty());
}

BOOST_AUTO_TEST_CASE(electrum_v1__construct_entropy__explicit_language__valid_expected)
{
    const data_chunk entropy(16, 0x42);
    electrum_v1 instance(entropy, language::pt);
    BOOST_REQUIRE(instance);
    BOOST_REQUIRE_EQUAL(instance.entropy(), entropy);
}

BOOST_AUTO_TEST_CASE(electrum_v1__construct_entropy__invalid_language__invalid)
{
    const data_chunk entropy(16, 0x42);
    electrum_v1 instance(entropy, language::ko);
    BOOST_REQUIRE(!instance);
}

// construct minimum_entropy

BOOST_AUTO_TEST_CASE(electrum_v1__construct_minimum_entropy__always__valid_expected)
{
    const electrum_v1::minimum_entropy entropy{ 0x42 };
    electrum_v1 instance(entropy);
    BOOST_REQUIRE(instance);
    BOOST_REQUIRE_EQUAL(instance.entropy(), to_chunk(entropy));
}

// construct maximum_entropy

BOOST_AUTO_TEST_CASE(electrum_v1__construct_maximum_entropy__always__valid_expected)
{
    const electrum_v1::maximum_entropy entropy{ 0x42 };
    electrum_v1 instance(entropy);
    BOOST_REQUIRE(instance);
    BOOST_REQUIRE_EQUAL(instance.entropy(), to_chunk(entropy));
}

// construct protected entropy

BOOST_AUTO_TEST_CASE(electrum_v1__construct_protected_entropy__empty__expected)
{
    const accessor instance(data_chunk{}, {}, language::ja);
    BOOST_REQUIRE(!instance);
    BOOST_REQUIRE(instance.entropy().empty());
    BOOST_REQUIRE(instance.words().empty());
    BOOST_REQUIRE(instance.lingo() == language::ja);
}

BOOST_AUTO_TEST_CASE(electrum_v1__construct_protected_entropy__not_empty__expected)
{
    const data_chunk entropy(5, 0x42);
    const string_list words(5, "word");
    const accessor instance(entropy, words, language::ja);
    BOOST_REQUIRE(instance);
    BOOST_REQUIRE_EQUAL(instance.entropy(), entropy);
    BOOST_REQUIRE_EQUAL(instance.words(), words);
    BOOST_REQUIRE(instance.lingo() == language::ja);
}

// construct protected result

BOOST_AUTO_TEST_CASE(electrum_v1__construct_protected_decoding__true_overflows__expected)
{
    const data_chunk entropy(5, 0x42);
    const string_list words(5, "word");
    const v1_decoding::overflow overflows(5, true);
    const accessor instance(v1_decoding(entropy, overflows), words, language::ja);
    BOOST_REQUIRE(instance);
    BOOST_REQUIRE_EQUAL(instance.overflow(), true);
    BOOST_REQUIRE_EQUAL(instance.overflows(), overflows);
    BOOST_REQUIRE_EQUAL(instance.entropy(), entropy);
    BOOST_REQUIRE_EQUAL(instance.words(), words);
    BOOST_REQUIRE(instance.lingo() == language::ja);
}

BOOST_AUTO_TEST_CASE(electrum_v1__construct_protected_decoding__false_overflows__expected)
{
    const data_chunk entropy(5, 0x42);
    const string_list words(5, "word");
    const v1_decoding::overflow overflows(5, false);
    const accessor instance(v1_decoding(entropy, overflows), words, language::ja);
    BOOST_REQUIRE(instance);
    BOOST_REQUIRE_EQUAL(instance.overflow(), false);
    BOOST_REQUIRE_EQUAL(instance.overflows(), overflows);
    BOOST_REQUIRE_EQUAL(instance.entropy(), entropy);
    BOOST_REQUIRE_EQUAL(instance.words(), words);
    BOOST_REQUIRE(instance.lingo() == language::ja);
}

#endif // CONSTRUCTORS

#ifdef PUBLIC_METHODS

// to_public_key

BOOST_AUTO_TEST_CASE(electrum_v1__to_public_key__invalid__invalid)
{
    electrum_v1 instance;
    BOOST_REQUIRE(!instance);
    BOOST_REQUIRE(!instance.to_public_key());
}

BOOST_AUTO_TEST_CASE(electrum_v1__to_public_key__valid__default_context__valid)
{
    const electrum_v1 instance(words12);
    BOOST_REQUIRE(instance);
    BOOST_REQUIRE(instance.to_public_key());
}

BOOST_AUTO_TEST_CASE(electrum_v1__to_public_key__valid_explicit_context__valid)
{
    const electrum_v1 instance(words12);
    BOOST_REQUIRE(instance);
    BOOST_REQUIRE(instance.to_public_key(btc_mainnet_p2kh));
}

BOOST_AUTO_TEST_CASE(electrum_v1__to_public_key__todo__todo)
{
    BOOST_REQUIRE(TODO_TESTS);
}

// to_seed

BOOST_AUTO_TEST_CASE(electrum_v1__to_seed__todo__todo)
{
    BOOST_REQUIRE(TODO_TESTS);
}

// overflow

BOOST_AUTO_TEST_CASE(electrum_v1__overflow__entropy_invalid__false)
{
    const accessor instance(data_chunk{});
    BOOST_REQUIRE(!instance);
    BOOST_REQUIRE(!instance.overflow());
}

BOOST_AUTO_TEST_CASE(electrum_v1__overflow__entropy__false)
{
    const accessor instance(data_chunk(16, 0x00));
    BOOST_REQUIRE(instance);
    BOOST_REQUIRE(!instance.overflow());
}

BOOST_AUTO_TEST_CASE(electrum_v1__overflow__words_invalid__false)
{
    const accessor instance(words2);
    BOOST_REQUIRE(!instance);
    BOOST_REQUIRE(!instance.overflow());
}

BOOST_AUTO_TEST_CASE(electrum_v1__overflow__words_not_overflowed__false)
{
    const accessor instance(words12);
    BOOST_REQUIRE(instance);
    BOOST_REQUIRE(!instance.overflow());
}

BOOST_AUTO_TEST_CASE(electrum_v1__overflow__words_overflowed__true)
{
    const accessor instance(two_overflows12);
    BOOST_REQUIRE(instance);
    BOOST_REQUIRE(instance.overflow());
}

#endif // PUBLIC_METHODS

#ifdef PROTECTED_METHODS

// overflows

BOOST_AUTO_TEST_CASE(electrum_v1__overflows__entropy_invalid__empty)
{
    const accessor instance(data_chunk{});
    BOOST_REQUIRE(!instance);
    BOOST_REQUIRE(instance.overflows().empty());
}

BOOST_AUTO_TEST_CASE(electrum_v1__overflows__entropy__empty)
{
    const accessor instance(data_chunk(32, 0x00));
    BOOST_REQUIRE(instance);
    BOOST_REQUIRE(instance.overflows().empty());
}

BOOST_AUTO_TEST_CASE(electrum_v1__overflow__words_invalid__empty)
{
    const accessor instance(words2);
    BOOST_REQUIRE(!instance);
    BOOST_REQUIRE(instance.overflows().empty());
}

BOOST_AUTO_TEST_CASE(electrum_v1__overflows__words_not_overflowed__expected)
{
    const accessor instance(words24);
    BOOST_REQUIRE(instance);
    BOOST_REQUIRE(!instance.overflow());
    const auto& overflows = instance.overflows();
    BOOST_REQUIRE_EQUAL(overflows.size(), 24u / 3u);
    BOOST_REQUIRE(!overflows[0]);
    BOOST_REQUIRE(!overflows[1]);
    BOOST_REQUIRE(!overflows[2]);
    BOOST_REQUIRE(!overflows[3]);
    BOOST_REQUIRE(!overflows[4]);
    BOOST_REQUIRE(!overflows[5]);
    BOOST_REQUIRE(!overflows[6]);
    BOOST_REQUIRE(!overflows[7]);
}

BOOST_AUTO_TEST_CASE(electrum_v1__overflows__words_overflowed__expected)
{
    const accessor instance(two_overflows12);
    BOOST_REQUIRE(instance);
    BOOST_REQUIRE(instance.overflow());
    const auto& overflows = instance.overflows();
    BOOST_REQUIRE_EQUAL(overflows.size(), 12u / 3u);
    BOOST_REQUIRE(!overflows[0]);
    BOOST_REQUIRE(overflows[1]);
    BOOST_REQUIRE(!overflows[2]);
    BOOST_REQUIRE(overflows[3]);
}

// seed_entropy

BOOST_AUTO_TEST_CASE(electrum_v1__seed_entropy__todo__todo)
{
    BOOST_REQUIRE(TODO_TESTS);
}

#endif // PROTECTED METHODS

#ifdef OPERATORS

// All operators are implemented in the languages base class.

// assign

BOOST_AUTO_TEST_CASE(electrum_v1__assign_copy__always__expected)
{
    const electrum_v1 instance1(two_overflows12);
    electrum_v1 instance2;
    instance2 = instance1;
    BOOST_REQUIRE(instance2);
    BOOST_REQUIRE_EQUAL(instance2.words(), two_overflows12);
    BOOST_REQUIRE_EQUAL(instance2.entropy(), instance1.entropy());
    BOOST_REQUIRE(instance2.lingo() == instance1.lingo());
    BOOST_REQUIRE(instance2.overflow() == instance1.overflow());

    // Overflow entropy does not round trip words.
    const electrum_v1 instance3(instance2.entropy());
    BOOST_REQUIRE_EQUAL(instance3.entropy(), instance2.entropy());
    BOOST_REQUIRE_NE(instance3.words(), instance2.words());
    BOOST_REQUIRE(instance3.overflow() != instance2.overflow());
}

BOOST_AUTO_TEST_CASE(electrum_v1__assign_move__always__expected)
{
    electrum_v1 instance1(two_overflows12);
    const electrum_v1 instance1_copy(two_overflows12);
    electrum_v1 instance2;
    instance2 = std::move(instance1);
    BOOST_REQUIRE(instance2);
    BOOST_REQUIRE_EQUAL(instance2.words(), two_overflows12);
    BOOST_REQUIRE_EQUAL(instance2.entropy(), instance1_copy.entropy());
    BOOST_REQUIRE(instance2.lingo() == instance1_copy.lingo());
    BOOST_REQUIRE(instance2.overflow() == instance1_copy.overflow());

    // Overflow entropy does not round trip words.
    const electrum_v1 instance3(instance2.entropy());
    BOOST_REQUIRE_EQUAL(instance3.entropy(), instance2.entropy());
    BOOST_REQUIRE_NE(instance3.words(), instance2.words());
    BOOST_REQUIRE(instance3.overflow() != instance2.overflow());
}

// in/equality

BOOST_AUTO_TEST_CASE(electrum_v1__equality__always__expected)
{
    const electrum_v1 instance1(words12);
    const electrum_v1 instance2(words12);
    BOOST_REQUIRE(instance1);
    BOOST_REQUIRE(instance1 == instance2);
    BOOST_REQUIRE(electrum_v1(instance2.entropy()) == instance2);
}

BOOST_AUTO_TEST_CASE(electrum_v1__inequality__always__expected)
{
    const electrum_v1 instance1(words12);
    const electrum_v1 instance2(two_overflows12);
    BOOST_REQUIRE(instance1);
    BOOST_REQUIRE(instance2);
    BOOST_REQUIRE(instance1 != instance2);
    BOOST_REQUIRE(electrum_v1(instance2.entropy()) != instance2);
}

// operator>>

BOOST_AUTO_TEST_CASE(electrum_v1__deserialize__valid__expected)
{
    std::istringstream in{ join(words12) };
    electrum_v1 instance;
    in >> instance;
    BOOST_REQUIRE(instance);
    BOOST_REQUIRE_EQUAL(instance.words(), words12);
}

BOOST_AUTO_TEST_CASE(electrum_v1__deserialize__invalid__invalid)
{
    std::istringstream in{ join(words2) };
    electrum_v1 instance;
    in >> instance;
    BOOST_REQUIRE(!instance);
}

// operator<<

BOOST_AUTO_TEST_CASE(electrum_v1__serialize__valid__expected)
{
    std::ostringstream out;
    electrum_v1 instance(words12);
    out << instance;
    BOOST_REQUIRE_EQUAL(out.str(), join(words12));
}

BOOST_AUTO_TEST_CASE(electrum_v1__serialize__invalid__invalid)
{
    std::ostringstream out;
    electrum_v1 instance;
    BOOST_REQUIRE(!instance);
    out << instance;
    BOOST_REQUIRE(out.str().empty());
}

#endif // OPERATORS

#ifdef VERIFIED_VECTORS

BOOST_AUTO_TEST_CASE(electrum_v1__construct__vectors__expected)
{
    for (const auto& vector: vectors)
    {
        const accessor words_instance(vector.mnemonic, vector.lingo);
        BOOST_REQUIRE(words_instance);
        BOOST_REQUIRE(words_instance.lingo() == vector.lingo);
        BOOST_REQUIRE_EQUAL(words_instance.entropy(), vector.entropy);
        BOOST_REQUIRE_EQUAL(words_instance.to_public_key(vector.network), vector.key);

        // The mnemonic always round trips from words/sentence.
        BOOST_REQUIRE_EQUAL(words_instance.sentence(), vector.mnemonic);
        BOOST_REQUIRE_EQUAL(words_instance.words(), vector.words());

        // Overflow is always populated when constructed from words/sentence.
        BOOST_REQUIRE_EQUAL(words_instance.overflow(), vector.overflow());
        BOOST_REQUIRE_EQUAL(words_instance.overflows(), vector.overflows());
        BOOST_REQUIRE_EQUAL(words_instance.seed_entropy(), vector.overflowed_seed_entropy());

        const accessor entropy_instance(vector.entropy, vector.lingo);
        BOOST_REQUIRE(entropy_instance);
        BOOST_REQUIRE(entropy_instance.lingo() == vector.lingo);
        BOOST_REQUIRE_EQUAL(entropy_instance.entropy(), vector.entropy);

        // The mnemonic does not round trip from entropy when overflowed.
        BOOST_REQUIRE_EQUAL(entropy_instance.sentence(), vector.trimmed_mnemonic());
        BOOST_REQUIRE_EQUAL(entropy_instance.words(), vector.trimmed_words());

        // Overflow is never populated when constructed from entropy.
        BOOST_REQUIRE(!entropy_instance.overflow());
        BOOST_REQUIRE(entropy_instance.overflows().empty());
        BOOST_REQUIRE_EQUAL(entropy_instance.seed_entropy(), vector.seed_entropy());
    }
}

#endif // VERIFIED_VECTORS

BOOST_AUTO_TEST_SUITE_END()
