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
#ifndef LIBBITCOIN_SYSTEM_TEST_DICTIONARIES_HPP
#define LIBBITCOIN_SYSTEM_TEST_DICTIONARIES_HPP

#include <cstddef>
#include <bitcoin/system.hpp>

 // Avoid using namespace in shared headers, but okay here.
using namespace bc::system::words;

namespace test {
namespace mnemonics_dictionaries {

const size_t test_dictionary_size = 10;
typedef dictionary<test_dictionary_size>::words test_words;
typedef words::dictionary<test_dictionary_size> test_dictionary;
typedef words::dictionaries<5, test_dictionary::size()> test_dictionaries;

const test_words test_words_en
{
    true,
    {
        "abandon",
        "ability",
        "able",
        "about",
        "above",
        "absent",
        "absorb",
        "abstract",
        "absurd",
        "abuse"
    }
};

const test_words test_words_es
{
    false,
    {
        "ábaco",
        "abdomen",
        "abeja",
        "abierto",
        "abogado",
        "abono",
        "aborto",
        "abrazo",
        "abrir",
        "abuelo"
    }
};

const test_words test_words_ja
{
    false,
    {
        "あいこくしん",
        "あいさつ",
        "あいだ",
        "あおぞら",
        "あかちゃん",
        "あきる",
        "あけがた",
        "あける",
        "あこがれる",
        "あさい"
    }
};

const test_words test_words_zh_Hans
{
    false,
    {
        "的",
        "一",
        "是",
        "在",
        "不",
        "了",
        "有",
        "和",
        "人",
        "这"
    }
};

const test_words test_words_zh_Hant
{
    false,
    {
        "的",
        "一",
        "是",
        "在",
        "不",
        "了",
        "有",
        "和",
        "人",
        "這"
    }
};

// This is the instance under test.
const test_dictionaries instance
{
    {
        test_dictionary{ language::en, test_words_en },
        test_dictionary{ language::es, test_words_es },
        test_dictionary{ language::ja, test_words_ja },
        test_dictionary{ language::zh_Hans, test_words_zh_Hans },
        test_dictionary{ language::zh_Hant, test_words_zh_Hant }
    }
};

} // namespace mnemonics_dictionaries
} // namespace test

#endif