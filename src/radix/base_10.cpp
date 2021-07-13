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
#include <bitcoin/system/radix/base_10.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iterator>
#include <sstream>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/math/addition.hpp>
#include <bitcoin/system/serial/deserialize.hpp>
#include <bitcoin/system/serial/serialize.hpp>

// base10
// Base 10 is an ascii data encoding with a domain of 10 symbols (characters).
// 10 is not a power of 2 so base10 is not a bit mapping.
// base10 is 10^n, with power determined by the parameter 'decimal_places'.
// Conversion range is limited to [0..2^64-1], decoded as uint64_t.
// Encoding uses arabic numeral characters ['0'..'9'] and supports negative
// powers using the '.' character delimiter. Leading zeros and trailing
// delimiters are not emitted. Trailing zeros of a negative power are not
// emitted. Leading zeros or delimiter are allowed. Empty string returns zero.
// Fractional values are rounded up (away from zero). No other textual
// encodings (e.g. "scientific notation") are supported. Integer conversion
// depends on the serialization library, which depends on std::iostream.

namespace libbitcoin {
namespace system {

// Returns true if a character is one of [0-9].
// The C standard library function 'isdigit' depends on the current locale
// and doesn't necessarily correspond to our valid base10 encoding digits.
// msvc++ 'isdigit' includes superscript characters like '²'.
inline bool is_digit(const char character)
{
    return '0' <= character && character <= '9';
}

template <char Character>
inline bool is_character(const char character)
{
    return character == Character;
}

bool decode_base10(uint64_t& out, const std::string& amount,
    uint8_t decimal_places, bool strict)
{
    std::string value(amount);

    // Remove the decimal point.
    auto point = std::find(value.begin(), value.end(), '.');
    if (point != value.end())
        point = value.erase(point);

    // Only digits should remain (this also precludes negatives).
    if (!std::all_of(value.begin(), value.end(), is_digit))
        return false;

    // Add digits to the end if there are too few.
    auto actual_places = std::distance(point, value.end());
    value.append(floored_subtract<size_t>(decimal_places, actual_places), '0');

    // Remove digits from the end if there are too many.
    // Value is truncated if trailing digits are non-zero.
    auto truncated = false;
    if (actual_places > decimal_places)
    {
        auto end = std::next(point, decimal_places);
        truncated = !std::all_of(end, value.end(), is_character<'0'>);
        value.erase(end, value.end());
    }

    if (strict && truncated)
        return false;

    uint64_t number = 0;
    if (!value.empty() && !deserialize(number, value))
        return false;

    if (truncated && (number == max_uint64))
        return false;

    // Round the value up if it was truncated.
    out = number + to_int(truncated);
    return true;
}

std::string encode_base10(uint64_t amount, uint8_t decimal_places)
{
    std::ostringstream stream;
    stream.fill('0');
    stream.width(add1<uint16_t>(decimal_places));
    stream << amount;
    auto text = stream.str();
    text.insert(text.size() - decimal_places, one, '.');
    trim_right(text, { "0" });
    trim_right(text, { "." });
    return text;
}

} // namespace system
} // namespace libbitcoin
