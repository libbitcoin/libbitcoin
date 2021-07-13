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
#ifndef LIBBITCOIN_SYSTEM_WORDS_DICTIONARIES_HPP
#define LIBBITCOIN_SYSTEM_WORDS_DICTIONARIES_HPP

#include <array>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <string>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/words/dictionary.hpp>
#include <bitcoin/system/words/language.hpp>

namespace libbitcoin {
namespace system {
namespace words {

// Search container for a set of dictionaries with POD word lists.
// POD dictionaries wrapper with per dictionary O(n) search and O(1) index.
// Search order is guaranteed, always returns first match.
template<size_t Count, size_t Size>
class dictionaries
{
public:
    /// Use system::cast to convert between search and result.
    typedef typename dictionary<Size>::search search;
    typedef typename dictionary<Size>::result result;
    typedef std::array<dictionary<Size>, Count> list;

    /// The number of dictionaries.
    static constexpr size_t count() { return Count; };

    /// The number of words in each dictionary (all are the same size).
    static constexpr size_t size() { return Size; };

    /// Constructor.
    dictionaries(const list& dictionaries);

    /// True if the specified dictionary exists.
    bool exists(language identifier) const;

    /// The language id of the dictionary name, language::none if not contained.
    language to_identifier(const std::string& name) const;

    /// The name of the specified dictionary, empty string if not contained.
    std::string to_name(language identifier) const;

    /// Search.

    /// The word corresponding to the index in the specified language.
    /// Empty string if language does not exist.
    std::string at(size_t index, language identifier) const;

    /// Empty string for any index > Size.
    /// The word corresponding to each index in the specified language.
    /// The words are returned in the same order as the indexes list.
    /// Empty list if language does not exist.
    string_list at(const search& indexes, language identifier) const;

    /// -1 if word is not found in the specified language.
    int32_t index(const std::string& word, language identifier) const;

    /// -1 for any word that is not found in the specified language.
    /// The word index for each word that is found in the specified language.
    /// The word indexes are returned in the same order as the words list.
    /// Empty list if language does not exist.
    result index(const string_list& words, language identifier) const;

    /// The language that contains the specified word, or language::none.
    /// If language::none is specified all dictionaries are searched.
    /// If any other language is specified the search is limited to it.
    /// Dictionary order is not preserved in the case of conflicts.
    /// BIP39 zh_Hans and zh_Hant dictionaries have 1275 overlapping words.
    language contains(const std::string& word,
        language identifier=language::none) const;

    /// The language that contains all specified words, or language::none.
    /// If language::none is specified all dictionaries are searched.
    /// If any other language is specified the search is limited to it.
    /// Dictionary order is not preserved in the case of conflicts.
    /// BIP39 zh_Hans and zh_Hant dictionaries have 1275 overlapping words.
    language contains(const string_list& words,
        language identifier=language::none) const;

private:
    // Obtain an iterator to the specified dictionary.
    typename list::const_iterator to_dictionary(language identifier) const;
    typename list::const_iterator to_dictionary(const std::string& name) const;

    // This dictionary collection creates only one word of state for each
    // dictionary reference, each which creates only one word of state for the
    // dictionary language identifier. Word lists are not loaded into a vector.
    // Dictionaries are search wrappers around a POD word list. Statically
    // declared word list references are passed through construction here into
    // the dictionary elements, which retain the reference. This dictionaries
    // search wrapper is held by the owner of the word list references.
    const list dictionaries_;
};

} // namespace words
} // namespace system
} // namespace libbitcoin

#include <bitcoin/system/impl/words/dictionaries.ipp>

#endif
