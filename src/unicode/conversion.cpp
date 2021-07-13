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
#include <bitcoin/system/unicode/conversion.hpp>

#include <algorithm>
#include <string>
#include <boost/locale.hpp>

namespace libbitcoin {
namespace system {

template <typename CharOut, typename CharIn>
static std::basic_string<CharOut> to_utf(const std::basic_string<CharIn>& in)
{
    using namespace boost::locale;
    std::basic_string<CharOut> out;

    // method_type::stop throws conv::conversion_error.
    // Other stop methods skip characters, resulting in hidden failure.
    // So use method_type::stop and trap conversion exceptions.
    try
    {
        // This is a mathematical conversion that could be reimplemented here.
        out = conv::utf_to_utf<CharOut>(in, conv::method_type::stop);
    }
    catch (const conv::conversion_error&)
    {
        out.clear();
    }

    return out;
}

template <typename CharOut, typename CharIn>
static std::vector<std::basic_string<CharOut>> to_utf(
    const std::vector<std::basic_string<CharIn>>& in)
{
    std::vector<std::basic_string<CharOut>> out(in.size());
    std::transform(in.begin(), in.end(), out.begin(),
        [](const std::basic_string<CharIn>& word)
        {
            return to_utf<CharOut>(word);
        });

    return out;
}

// char32_t is the only 1:1 char encoding.
std::string to_utf8(char32_t point)
{
    return to_utf8(std::u32string{ point });
}

std::string to_utf8(const std::wstring& text)
{
    return to_utf<char>(text);
}

std::string to_utf8(const std::u32string& text)
{
    return to_utf<char>(text);
}

std::wstring to_utf16(const std::string& text)
{
    return to_utf<wchar_t>(text);
}

std::wstring to_utf16(const std::u32string& text)
{
    return to_utf<wchar_t>(text);
}

std::u32string to_utf32(const std::string& text)
{
    return to_utf<char32_t>(text);
}

std::u32string to_utf32(const std::wstring& text)
{
    return to_utf<char32_t>(text);
}

string_list to_utf8(const wstring_list& text)
{
    return to_utf<char>(text);
}

string_list to_utf8(const u32string_list& text)
{
    return to_utf<char>(text);
}

wstring_list to_utf16(const string_list& text)
{
    return to_utf<wchar_t>(text);
}

wstring_list to_utf16(const u32string_list& text)
{
    return to_utf<wchar_t>(text);
}

u32string_list to_utf32(const string_list& text)
{
    return to_utf<char32_t>(text);
}

u32string_list to_utf32(const wstring_list& text)
{
    return to_utf<char32_t>(text);
}

} // namespace system
} // namespace libbitcoin
