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
#ifndef LIBBITCOIN_SYSTEM_WORDS_LANGUAGES_HPP
#define LIBBITCOIN_SYSTEM_WORDS_LANGUAGES_HPP

#include <iostream>
#include <string>
#include <bitcoin/system/constraints.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/unicode/unicode.hpp>
#include <bitcoin/system/words/language.hpp>

namespace libbitcoin {
namespace system {
namespace words {

class BC_API languages
{
public:
    // static methods
    // ------------------------------------------------------------------------

    /// The language id of the language name, language::none if not found.
    static language from_name(const std::string& name);

    /// The name of the specified language, empty string if not found.
    static std::string to_name(language identifier);

    /// The mnemonic delimiter for the given language.
    static std::string to_delimiter(language identifier);

    /// All languages except reference::ja are joined by an ASCII space.
    static std::string join(const string_list& words, language identifier);

    /// There is no trimming or token compression for reference::ja.
    /// All other languages are split and trimmed on ASCII whitespace.
    static string_list split(const std::string& sentence, language identifier);

    // constructors
    // ------------------------------------------------------------------------

    /// Class is not constructable but this is exposed for derived assignment.
    languages(const languages& other);

    // public methods
    // ------------------------------------------------------------------------

    /// The entropy of the mnemonic (not to be confused with the seed).
    const data_chunk& entropy() const;

    /// The dictionary language of the mnemonic.
    language lingo() const;

    /// The mnemonic sentence, Japanese joined by an ideographic space.
    std::string sentence() const;

    /// The individual words of the mnemonic.
    const string_list& words() const;

    // operators
    // ------------------------------------------------------------------------

    /// True if the object is a valid mnemonic.
    operator bool() const;

    /// Lexical compares of mnemonic sentences.
    bool operator<(const languages& other) const;
    bool operator==(const languages& other) const;
    bool operator!=(const languages& other) const;

    /// Assignment.
    languages& operator=(const languages& other);

protected:
    languages();
    languages(const data_chunk& entropy, const string_list& words,
        language identifier);

    // Normalizes ascii whitespace and attempts nfkd and case lowering.
    // Both nfkd and case lowering are skipped if WITH_ICU undefined.
    // This is only used to improve the chance of wordlist matching.
    static string_list try_normalize(const string_list& words);

    // These should be const, apart from the need to implement assignment.
    data_chunk entropy_;
    string_list words_;
    language identifier_;
};

template <class Type, if_base_of<languages, Type> = true>
std::istream& operator>>(std::istream& in, Type& out)
{
    std::istreambuf_iterator<char> begin(in), end;
    const auto value = std::string(begin, end);
    out = Type(value);

    ////if (!out)
    ////    throw istream_exception(value);

    return in;
}

template <class Type, if_base_of<languages, Type> = true>
std::ostream& operator<<(std::ostream& out, const Type& in)
{
    out << in.sentence();
    return out;
}

} // namespace words
} // namespace system
} // namespace libbitcoin

#endif
