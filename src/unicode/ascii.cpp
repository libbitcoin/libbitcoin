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
#include <bitcoin/system/unicode/ascii.hpp>

#include <algorithm>
#include <cstddef>
#include <string>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/unicode/normalization.hpp>

namespace libbitcoin {
namespace system {

// char32_t functions.

bool is_ascii_character(char32_t character)
{
    return is_zero(character & 0xffffff80);
}

bool is_ascii_separator(char32_t point)
{
    return is_ascii_character(point) && is_separator(point);
}

bool is_ascii_whitespace(char32_t point)
{
    return is_ascii_character(point) && is_whitespace(point);
}

// string functions.

bool is_ascii(const std::string& text)
{
    return std::all_of(text.begin(), text.end(), is_ascii_character);
}
    
std::string ascii_to_lower(const std::string& text)
{
    auto copy = text;

    const auto to_lower = [](char value)
    {
        return 'A' <= value && value <= 'Z' ? value + ('a' - 'A') : value;
    };

    std::transform(text.begin(), text.end(), copy.begin(), to_lower);
    return copy;
}

std::string ascii_to_upper(const std::string& text)
{
    auto copy = text;

    const auto to_upper = [](char value)
    {
        return 'a' <= value && value <= 'z' ? value + ('A' - 'a') : value;
    };

    std::transform(text.begin(), text.end(), copy.begin(), to_upper);
    return copy;
}

bool has_mixed_ascii_case(const std::string& text)
{
    auto lower = false;
    auto upper = false;
    
    const auto set_lower_and_upper = [&](char character)
    {
        lower |= ('a' <= character && character <= 'z');
        upper |= ('A' <= character && character <= 'Z');
    };

    std::for_each(text.begin(), text.end(), set_lower_and_upper);
    return lower && upper;
}

} // namespace system
} // namespace libbitcoin
