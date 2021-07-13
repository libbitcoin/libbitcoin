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
#ifndef LIBBITCOIN_SYSTEM_WORDS_DICTIONARY_IPP
#define LIBBITCOIN_SYSTEM_WORDS_DICTIONARY_IPP

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string>
#include <type_traits>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/words/language.hpp>
#include <bitcoin/system/words/languages.hpp>

namespace libbitcoin {
namespace system {
namespace words {

// en.cppreference.com/w/cpp/named_req/PODType
// Ensure that dictionary word lists remain POD types.
static_assert(std::is_pod<dictionary<1>::words>(), "performance");

// Constructor.
// ----------------------------------------------------------------------------

template<size_t Size>
dictionary<Size>::dictionary(language identifier, const words& words)
  : identifier_(identifier), words_(words)
{
}

template <size_t Size>
language dictionary<Size>::identifier() const
{
    return identifier_;
}

template <size_t Size>
std::string dictionary<Size>::name() const
{
    return languages::to_name(identifier_);
}

// Search.
// ----------------------------------------------------------------------------

template <size_t Size>
std::string dictionary<Size>::at(size_t index) const
{
    return index < size() ? words_.word[index] : "";
}

template <size_t Size>
string_list dictionary<Size>::at(const search& indexes) const
{
    string_list out(indexes.size());

    // std::transform can be parallel but maintains order.
    std::transform(indexes.begin(), indexes.end(), out.begin(),
        [&](size_t index)
        {
            // index is signed because we reuse indexes data type. 
            return at(index);
        });

    return out;
}

template <size_t Size>
int32_t dictionary<Size>::index(const std::string& word) const
{
    // Dictionary sort is configured on each dictionary, verified by tests.
    // Dictionary is char* elements but using std::string (word) for compares.
    if (words_.sorted)
        return static_cast<int32_t>(binary_search(words_.word, word));

    // std::find returns first match, though words should be unique.
    const auto it = std::find(words_.word.begin(), words_.word.end(), word);
    return it == words_.word.end() ? -1 : 
        static_cast<int32_t>(std::distance(words_.word.begin(), it));
}

template <size_t Size>
typename dictionary<Size>::result
dictionary<Size>::index(const string_list& words) const
{
    dictionary<Size>::result out(words.size());

    // std::transform can be parallel but maintains order.
    std::transform(words.begin(), words.end(), out.begin(),
        [&](const std::string& word)
        {
            return index(word);
        });

    return out;
}

template <size_t Size>
bool dictionary<Size>::contains(const std::string& word) const
{
    return index(word) >= 0;
}

template <size_t Size>
bool dictionary<Size>::contains(const string_list& words) const
{
    // std::all_of can be parallel and order doesn't matter.
    return std::all_of(words.begin(), words.end(),
        [&](const std::string& word)
        {
            return contains(word);
        });
}

} // namespace words
} // namespace system
} // namespace libbitcoin

#endif
