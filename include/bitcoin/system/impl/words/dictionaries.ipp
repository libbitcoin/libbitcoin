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
#ifndef LIBBITCOIN_SYSTEM_WORDS_DICTIONARIES_IPP
#define LIBBITCOIN_SYSTEM_WORDS_DICTIONARIES_IPP

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <string>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/words/language.hpp>

namespace libbitcoin {
namespace system {
namespace words {

// Word-not-found sentinel, consistent with dictionary.index.
constexpr int32_t missing = -1;

// Constructor.
// ----------------------------------------------------------------------------

template<size_t Count, size_t Size>
dictionaries<Count, Size>::dictionaries(const list& dictionaries)
  : dictionaries_(dictionaries)
{
}

template<size_t Count, size_t Size>
bool dictionaries<Count, Size>::exists(language identifier) const
{
    return to_dictionary(identifier) != dictionaries_.end();
}

template<size_t Count, size_t Size>
language dictionaries<Count, Size>::to_identifier(const std::string& name) const
{
    const auto it = to_dictionary(name);
    return it != dictionaries_.end() ? it->identifier() : language::none;
}

template<size_t Count, size_t Size>
std::string dictionaries<Count, Size>::to_name(language identifier) const
{
    const auto it = to_dictionary(identifier);
    return it != dictionaries_.end() ? it->name() : "";
}

// Search.
// ----------------------------------------------------------------------------

template<size_t Count, size_t Size>
std::string dictionaries<Count, Size>::at(size_t index,
    language identifier) const
{
    const auto it = to_dictionary(identifier);
    return it != dictionaries_.end() ? it->at(index) : "";
}

template<size_t Count, size_t Size>
string_list dictionaries<Count, Size>::at(const search& indexes,
    language identifier) const
{
    const auto it = to_dictionary(identifier);
    return it != dictionaries_.end() ? it->at(indexes) : string_list{};
}

template<size_t Count, size_t Size>
int32_t dictionaries<Count, Size>::index(const std::string& word,
    language identifier) const
{
    const auto it = to_dictionary(identifier);
    return it != dictionaries_.end() ? it->index(word) : missing;
}

template<size_t Count, size_t Size>
typename dictionaries<Count, Size>::result
dictionaries<Count, Size>::index(const string_list& words,
    language identifier) const
{
    const auto it = to_dictionary(identifier);
    return it != dictionaries_.end() ? it->index(words) : result{};
}

template<size_t Count, size_t Size>
language dictionaries<Count, Size>::contains(const std::string& word,
    language identifier) const
{
    if (identifier != language::none)
    {
        const auto it = to_dictionary(identifier);
        return it != dictionaries_.end() && it->contains(word) ?
            identifier : language::none;
    }

    // std::find_if returns first match, order is guaranteed.
    const auto it = std::find_if(dictionaries_.begin(), dictionaries_.end(),
        [&](const dictionary<Size>& dictionary)
        {
            return dictionary.contains(word);
        });

    return it != dictionaries_.end() ? it->identifier() : language::none;
}

template<size_t Count, size_t Size>
language dictionaries<Count, Size>::contains(const string_list& words,
    language identifier) const
{
    if (identifier != language::none)
    {
        const auto it = to_dictionary(identifier);
        return it != dictionaries_.end() && it->contains(words) ?
            identifier : language::none;
    }

    // std::find_if returns first match, order is guaranteed.
    const auto it = std::find_if(dictionaries_.begin(), dictionaries_.end(),
        [&](const dictionary<Size>& dictionary)
        {
            return dictionary.contains(words);
        });

    return it != dictionaries_.end() ? it->identifier() : language::none;
}

// private
// ----------------------------------------------------------------------------

template<size_t Count, size_t Size>
typename dictionaries<Count, Size>::list::const_iterator
dictionaries<Count, Size>::to_dictionary(language identifier) const
{
    // std::find_if returns first match, order is guaranteed.
    return std::find_if(dictionaries_.begin(), dictionaries_.end(),
        [&](const dictionary<Size>& dictionary)
        {
            return dictionary.identifier() == identifier;
        });
}

template<size_t Count, size_t Size>
typename dictionaries<Count, Size>::list::const_iterator
dictionaries<Count, Size>::to_dictionary(const std::string& name) const
{
    // std::find_if returns first match, order is guaranteed.
    return std::find_if(dictionaries_.begin(), dictionaries_.end(),
        [&](const dictionary<Size>& dictionary)
        {
            return dictionary.name() == name;
        });
}

} // namespace words
} // namespace system
} // namespace libbitcoin

#endif
