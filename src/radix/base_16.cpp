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
#include <bitcoin/system/radix/base_16.hpp>

#include <algorithm>
#include <cstdint>
#include <string>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/data/data.hpp>

// base16 (hexidecimal):
// Base 16 is an ascii data encoding with a domain of 16 symbols (characters).
// 16 is 2^4 so base16 is a 4<=>4 bit mapping.
// Invalid padding in byte packing is not detectable as it is byte-aligned.
// So this implementation is limited to an 8<=>8 bit mapping.
// There is no need for bit streaming or padding, and both sides are
// authoritative. Therefore base16 is our canonical byte encoding.

namespace libbitcoin {
namespace system {

inline bool is_between(uint8_t value, uint8_t low, uint8_t high)
{
    return low <= value && value <= high;
}

inline uint8_t from_base16_characters(char high, char low)
{
    const auto from_base16_digit = [](char character)
    {
        if (is_between(character, 'A', 'F'))
            return character - 'A' + '\xA';

        if (is_between(character, 'a', 'f'))
            return character - 'a' + '\xa';

        return character - '0' + '\x0';
    };

    return (from_base16_digit(high) << to_half(byte_bits)) |
        from_base16_digit(low);
}

inline char to_base16_character(char digit)
{
    return (is_between(digit, 0, 9) ? '0' : 'a' - '\xa') + digit;
}

// The C standard library function 'isxdigit' depends on the current locale,
// and does not necessarily match the base16 encoding.
bool is_base16(char character)
{
    return
        (is_between(character, '0', '9')) ||
        (is_between(character, 'a', 'f')) ||
        (is_between(character, 'A', 'F'));
}

// Undefined (but safe) behavior if characters are not base16. 
uint8_t encode_octet(const char(&string)[add1(octet_width)])
{
    return from_base16_characters(string[0], string[1]);
}

std::string encode_base16(const data_slice& data)
{
    std::string out;
    out.resize(data.size() * octet_width);
    auto digit = out.begin();

    for (const auto byte: data)
    {
        *digit++ = to_base16_character(byte >> to_half(byte_bits));
        *digit++ = to_base16_character(byte & 0x0f);
    }

    return out;
}

std::string encode_hash(const data_slice& hash)
{
    data_chunk data(hash.size());

    // TODO: avoid this copy.
    std::reverse_copy(hash.begin(), hash.end(), data.begin());
    return encode_base16(data);
}

bool decode_base16(data_chunk& out, const std::string& in)
{
    if (is_odd(in.size()))
        return false;

    if (!std::all_of(in.begin(), in.end(), is_base16))
        return false;

    out.resize(to_half(in.size()));
    auto data = out.begin();

    for (auto digit = in.begin(); digit != in.end();)
    {
        const auto hi = *digit++;
        const auto lo = *digit++;
        *data++ = from_base16_characters(hi, lo);
    }

    return true;
}

} // namespace system
} // namespace libbitcoin
