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
#ifndef LIBBITCOIN_SYSTEM_TEST_CATALOGS_MNEMONIC_HPP
#define LIBBITCOIN_SYSTEM_TEST_CATALOGS_MNEMONIC_HPP

#include <algorithm>
#include <cstddef>
#include <bitcoin/system.hpp>

// Avoid using namespace in shared headers, but okay here.
using namespace bc::system;
using namespace bc::system::words;

namespace test {
namespace catalogs_mnemonic {

static ptrdiff_t intersection(const mnemonic::catalog::words& left,
    const mnemonic::catalog::words& right)
{
    return std::count_if(left.word.begin(), left.word.end(),
        [&](const char test[])
        {
            return std::find(right.word.begin(), right.word.end(), test) !=
                right.word.end();
        });
}

static bool intersects(const mnemonic::catalog::words& left,
    const mnemonic::catalog::words& right)
{
    return std::any_of(left.word.begin(), left.word.end(),
        [&](const char test[])
        {
            return std::find(right.word.begin(), right.word.end(), test) !=
                right.word.end();
        });
}

static ptrdiff_t deviation(const mnemonic::catalog::words& left,
    const mnemonic::catalog::words& right)
{
    ptrdiff_t count = 0;

    for (auto outer = left.word.begin(); outer != left.word.end(); ++outer)
    {
        const auto inner = std::find(right.word.begin(), right.word.end(),
            *outer);

        if (inner != right.word.end())
        {
            const auto inner_index = std::distance(right.word.begin(), inner);
            const auto outer_index = std::distance(left.word.begin(), outer);
            count += (inner_index == outer_index ? 0 : 1);
        }
    }

    return count;
}

static bool abnormal(const mnemonic::catalog::words& words)
{
    return std::all_of(words.word.begin(), words.word.end(),
        [&](const char test[])
        {
            std::string copy = test;
            to_compatibility_decomposition(copy);
            to_lower(copy);
            return test != copy;
        });
}

static string_list to_string_list(const mnemonic::catalog::words& words)
{
    // Convert dictionary to string, otherwise pointers are compared.
    string_list tokens(mnemonic::catalog::size());
    std::transform(words.word.begin(), words.word.end(), tokens.begin(),
        [](const char* token)
        {
            return token;
        });

    return tokens;
}

static bool sorted8(const mnemonic::catalog::words& words)
{
    return is_sorted(to_string_list(words));
}

static bool sorted16(const mnemonic::catalog::words& words)
{
    return is_sorted(to_utf16(to_string_list(words)));
}

static bool sorted32(const mnemonic::catalog::words& words)
{
    return is_sorted(to_utf32(to_string_list(words)));
}

static bool distinct(const mnemonic::catalog::words& words)
{
    return is_distinct(to_string_list(words));
}

static hash_digest identity(const mnemonic::catalog::words& words)
{
    return sha256_hash(join(to_string_list(words)));
}

} // catalogs_mnemonic
} // test

#endif
