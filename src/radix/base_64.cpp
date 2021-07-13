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
#include <bitcoin/system/radix/base_64.hpp>

#include <cstdint>
#include <string>
#include <bitcoin/system/data/data.hpp>

// base64
// Base 64 is an ascii data encoding with a domain of 64 symbols (characters).
// 64 is 2^6 so base64 is a 6<=>8 bit mapping.
// 4 characters (64^4) are encoded into one 8 byte (8^8) chunk before mapping.
// So after 8 byte encoding base64 is a 64<=>64 bit mapping.
// The 6 bit encoding is authoritative as byte encoding is padded.
// Invalid padding results in a decoding error.

// This implementation derived from public domain:
// en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64

namespace libbitcoin {
namespace system {

const static char pad = '=';

const static char table[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string encode_base64(const data_slice& unencoded)
{
    std::string encoded;
    const auto size = unencoded.size();
    const auto padding = to_int((!is_zero(size % 3)));
    encoded.reserve((size / 3 + padding) * 4);

    uint32_t value;
    auto cursor = unencoded.begin();
    for (size_t position = 0; position < size / 3u; position++)
    {
        // Convert to big endian.
        value = (*cursor++) << 16;
        value += (*cursor++) << 8;
        value += (*cursor++);

        encoded.append(1, table[(value & 0x00fc0000) >> 18]);
        encoded.append(1, table[(value & 0x0003f000) >> 12]);
        encoded.append(1, table[(value & 0x00000fc0) >> 6]);
        encoded.append(1, table[(value & 0x0000003f) >> 0]);
    }

    switch (size % 3)
    {
        case 1:
            // Convert to big endian.
            value = (*cursor++) << 16;

            encoded.append(1, table[(value & 0x00fc0000) >> 18]);
            encoded.append(1, table[(value & 0x0003f000) >> 12]);
            encoded.append(2, pad);
            break;
        case 2:
            // Convert to big endian.
            value = (*cursor++) << 16;
            value += (*cursor++) << 8;

            encoded.append(1, table[(value & 0x00fc0000) >> 18]);
            encoded.append(1, table[(value & 0x0003f000) >> 12]);
            encoded.append(1, table[(value & 0x00000fc0) >> 6]);
            encoded.append(1, pad);
            break;
    }

    return encoded;
}

bool decode_base64(data_chunk& out, const std::string& in)
{
    const static uint32_t mask = 0x000000ff;

    const auto length = in.length();
    if (!is_zero(length % 4))
        return false;

    size_t padding = 0;
    if (!is_zero(length))
    {
        if (in[length - 1] == pad)
            padding++;
        if (in[length - 2] == pad)
            padding++;
    }

    data_chunk decoded;
    decoded.reserve(((length / 4) * 3) - padding);

    uint32_t value = 0;
    for (auto cursor = in.begin(); cursor < in.end();)
    {
        for (size_t position = 0; position < 4u; position++)
        {
            value <<= 6;
            if (*cursor >= 0x41 && *cursor <= 0x5a)
                value |= *cursor - 0x41;
            else if (*cursor >= 0x61 && *cursor <= 0x7a)
                value |= *cursor - 0x47;
            else if (*cursor >= 0x30 && *cursor <= 0x39)
                value |= *cursor + 0x04;
            else if (*cursor == 0x2b)
                value |= 0x3e;
            else if (*cursor == 0x2f)
                value |= 0x3f;
            else if (*cursor == pad)
            {
                // Handle 1 or 2 pad characters.
                switch (in.end() - cursor)
                {
                    case 1:
                        decoded.push_back((value >> 16) & mask);
                        decoded.push_back((value >> 8) & mask);
                        out = decoded;
                        return true;
                    case 2:
                        decoded.push_back((value >> 10) & mask);
                        out = decoded;
                        return true;
                    default:
                        return false;
                }
            }
            else
                return false;

            cursor++;
        }

        decoded.push_back((value >> 16) & mask);
        decoded.push_back((value >> 8) & mask);
        decoded.push_back((value >> 0) & mask);
    }

    out = decoded;
    return true;
}

} // namespace system
} // namespace libbitcoin

