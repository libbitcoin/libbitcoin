/**
 * Copyright (c) 2011-2019 libbitcoin developers (see AUTHORS)
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
#include <bitcoin/system/data/string.hpp>

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <sstream>
#include <string>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/data/data_slice.hpp>
#include <bitcoin/system/unicode/unicode.hpp>

namespace libbitcoin {
namespace system {

std::string to_string(const data_slice& bytes) noexcept
{
    return bytes.to_string();
}

std::string join(const string_list& tokens,
    const std::string& delimiter) noexcept
{
    if (tokens.empty())
        return {};

    // Start with the first token.
    std::ostringstream sentence;
    sentence << tokens.front();

    // Add remaining tokens preceded by delimiters.
    for (auto token = std::next(tokens.begin()); token != tokens.end(); ++token)
        sentence << delimiter << *token;

    return sentence.str();
}

void reduce(string_list& tokens, const string_list& trim_tokens,
    bool compress) noexcept
{
    static const std::string empty{};

    if (tokens.empty())
        return;

    for (auto& token: tokens)
        trim(token, trim_tokens);

    if (compress)
        tokens.erase(std::remove(tokens.begin(), tokens.end(), empty),
            tokens.end());

    if (tokens.empty())
        tokens.push_back({});
}

string_list reduce_copy(const string_list& tokens,
    const string_list& trim_tokens, bool compress) noexcept
{
    auto copy = tokens;
    reduce(copy, trim_tokens, compress);
    return copy;
}

size_t replace(std::string& text, const std::string& from,
    const std::string& to) noexcept
{
    size_t count = 0;

    for (auto position = text.find(from);
        position != std::string::npos;
        position = text.find(from, position + to.length()))
    {
        ++count;
        text.replace(position, from.length(), to);
    }

    return count;
}

std::string replace_copy(const std::string& text, const std::string& from,
    const std::string& to) noexcept
{
    auto copy = text;
    replace(copy, from, to);
    return copy;
}

static string_list splitter(const std::string& text, const std::string& delimiter,
    const string_list& trim_tokens, bool compress) noexcept
{
    size_t start = 0;
    string_list tokens;

    // Push all but the last token.
    for (auto position = text.find(delimiter);
        position != std::string::npos;
        position = text.find(delimiter, start))
    {
        tokens.push_back(text.substr(start, position - start));
        start = position + delimiter.length();
    }

    // Push last token (delimiter not found).
    tokens.push_back(text.substr(start, text.length() - start));
    reduce(tokens, trim_tokens, compress);
    return tokens;
}

string_list split(const std::string& text, const string_list& delimiters,
    const string_list& trim_tokens, bool compress) noexcept
{
    // Nothing to do.
    if (delimiters.empty() && trim_tokens.empty())
        return { text };

    // Trim first to preclude outer empty otherwise trimmable outer tokens.
    auto trimmed = trim_copy(text, trim_tokens);

    // Nothing more to do.
    if (delimiters.empty())
        return { trimmed };

    // Replace all other delimiters with the first delimiter.
    for (auto it = std::next(delimiters.begin()); it != delimiters.end(); ++it)
        replace(trimmed, *it, delimiters.front());

    // Split on the first delimiter and conditionally compress empty tokens.
    return splitter(trimmed, delimiters.front(), trim_tokens, compress);
}

string_list split(const std::string& text, const std::string& delimiter,
    bool trim, bool compress) noexcept
{
    const auto trim_tokens = trim ? ascii_whitespace : string_list{};
    return split(text, string_list{ delimiter }, trim_tokens, compress);
}

string_list split(const std::string& text, bool compress) noexcept
{
    // Splitting is prioritized over trimming, because each token is trimmed.
    // So trimming is not an option when splitting on the trim characters.
    return split(text, ascii_whitespace, ascii_whitespace, compress);
}

bool trim_left(std::string& text, const std::string& token) noexcept
{
    auto found = false;
    const auto length = token.length();
    while (starts_with(text, token))
    {
        found = true;
        text.erase(0, length);
    }
    text.shrink_to_fit();
    return found;
}

bool trim_right(std::string& text, const std::string& token) noexcept
{
    auto found = false;
    const auto length = token.length();
    while (ends_with(text, token))
    {
        found = true;
        text.erase(text.length() - length);
    }
    text.shrink_to_fit();
    return found;
}

bool trim_left(std::string& text, const string_list& trim_tokens) noexcept
{
    bool found;
    do
    {
        found = false;
        for (const auto& token: trim_tokens)
            if (trim_left(text, token))
                found = true;
    }
    while (found);
    return found;
}

bool trim_right(std::string& text, const string_list& trim_tokens) noexcept
{
    bool found;
    do
    {
        found = false;
        for (const auto& token: trim_tokens)
            if (trim_right(text, token))
                found = true;
    }
    while (found);
    return found;
}

void trim(std::string& text, const string_list& trim_tokens) noexcept
{
    trim_left(text, trim_tokens);
    trim_right(text, trim_tokens);
}

std::string trim_left_copy(const std::string& text,
    const string_list& trim_tokens) noexcept
{
    auto copy = text;
    trim_left(copy, trim_tokens);
    return copy;
}

std::string trim_right_copy(const std::string& text,
    const string_list& trim_tokens) noexcept
{
    auto copy = text;
    trim_right(copy, trim_tokens);
    return copy;
}

std::string trim_copy(const std::string& text,
    const string_list& trim_tokens) noexcept
{
    auto copy = text;
    trim(copy, trim_tokens);
    return copy;
}

bool ends_with(const std::string& text, const std::string& suffix) noexcept
{
    const auto at = text.rfind(suffix);
    return at != std::string::npos && (at + suffix.length()) == text.length();
}

bool starts_with(const std::string& text, const std::string& prefix) noexcept
{
    return is_zero(text.find(prefix));
}

} // namespace system
} // namespace libbitcoin
