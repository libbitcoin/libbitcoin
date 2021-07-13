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
#ifndef LIBBITCOIN_SYSTEM_TEST_DICTIONARY_HPP
#define LIBBITCOIN_SYSTEM_TEST_DICTIONARY_HPP

#include <cstddef>
#include <bitcoin/system.hpp>

// Avoid using namespace in shared headers, but okay here.
using namespace bc::system::words;

namespace test {
namespace mnemonics_dictionary {

const size_t test_dictionary_size = 10;
typedef dictionary<test_dictionary_size>::words test_words;

// First 10 words of BIP39 es, first word not sorted for C compare (<).
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

// This is the instance under test.
dictionary<test_dictionary_size> instance(language::es, test_words_es);

} // namespace mnemonics_dictionary
} // namespace test

#endif