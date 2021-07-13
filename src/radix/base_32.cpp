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
#include <bitcoin/system/radix/base_32.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/stream/stream.hpp>
#include <bitcoin/system/unicode/unicode.hpp>

// base32
// Base 32 is an ascii data encoding with a domain of 32 symbols (characters).
// 32 is 2^5 so this is a 5<=>8 bit mapping.
// The 5 bit encoding is authoritative as byte encoding is padded.
// Invalid padding results in a decoding error.

namespace libbitcoin {
namespace system {

constexpr char encode[] = "qpzry9x8gf2tvdw0s3jn54khce6mua7l";
constexpr uint8_t decode[] =
{
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    15,   0xff, 10,   17,   21,   20,   26,   30,
    7,    5,    0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 29,   0xff, 24,   13,   25,   9,    8,
    23,   0xff, 18,   22,   31,   27,   19,   0xff,
    1,    0,    3,    16,   11,   28,   12,   14,
    6,    4,    2,    0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 29,   0xff, 24,   13,   25,   9,    8,
    23,   0xff, 18,   22,   31,   27,   19,   0xff,
    1,    0,    3,    16,   11,   28,   12,   14,
    6,    4,    2,    0xff, 0xff, 0xff, 0xff, 0xff
};

// encode

std::string encode_base32(const base32_chunk& data)
{
    std::string out;
    out.reserve(data.size());

    // encode[] cannot be out of bounds because expanded bytes are < 32.
    for (auto value: data)
        out.push_back(encode[static_cast<uint8_t>(value)]);

    return out;
}

std::string encode_base32(const data_chunk& data)
{
    return encode_base32(base32_unpack(data));
}

// decode

bool decode_base32(base32_chunk& out, const std::string& in)
{
    if (has_mixed_ascii_case(in))
        return false;

    out.clear();
    out.reserve(in.size());

    // decode[] cannot be out of bounds because char are < 256.
    for (auto character: in)
    {
        const auto value = decode[static_cast<uint8_t>(character)];

        if (value == 0xff)
            return false;

        out.push_back(static_cast<uint5_t>(value));
    }

    return true;
}

bool decode_base32(data_chunk& out, const std::string& in)
{
    base32_chunk expanded;
    if (!decode_base32(expanded, in))
        return false;

    out = base32_pack(expanded);
    return true;
}

// pack/unpack

data_chunk base32_pack(const base32_chunk& unpacked)
{
    data_chunk packed;
    write::bits::data sink(packed);

    // This is how c++ developers do it. :)
    for (const auto& value: unpacked)
        sink.write_bits(value.convert_to<uint8_t>(), 5);

    sink.flush();

    // Remove an element that is only padding, assumes base32_unpack encoding.
    // The bit writer writes zeros past end as padding.
    // This is a ((n * 5) / 8) operation, so (8 - ((n * 5) % 8)) are pad.
    // This padding is in addition to that added by unpacking. When unpacked
    // and then packed this will always result in either no pad bits or a full
    // element of zeros that is padding. This should be apparent from the fact
    // that the number of used bits is unchanged. Remainder indicates padding.
    if (!is_zero((unpacked.size() * 5) % 8))
    {
        // If pad byte is non-zero the unpacking was not base32_unpack.
        // So we return an failure where the condition is detecable.
        packed.resize(packed.back() == 0x00 ? sub1(packed.size()) : 0);
    }

    return packed;
}

base32_chunk base32_unpack(const data_chunk& packed)
{
    base32_chunk unpacked;
    read::bits::copy source(packed);

    // This is how c++ developers do it. :)
    while (!source.is_exhausted())
        unpacked.push_back(source.read_bits(5));

    // The bit reader reads zeros past end as padding.
    // This is a ((n * 8) / 5) operation, so ((n * 8) % 5)) bits are pad.
    ////const auto padded = (packed.size() * 8) % 5 != 0;
    return unpacked;
}

} // namespace system
} // namespace libbitcoin
