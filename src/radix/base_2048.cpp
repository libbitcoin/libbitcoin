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
#include <bitcoin/system/radix/base_2048.hpp>

#include <algorithm>
#include <cstdint>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/math/sign.hpp>
#include <bitcoin/system/stream/stream.hpp>
#include <bitcoin/system/words/words.hpp>

// base2048
// Base 2048 is an ascii data encoding with a domain of 2048 symbols (words).
// 2048 is 2^11 so this is a 11<=>8 bit mapping.
// The 5 bit encoding (words) is authoritative as byte encoding is padded.
// Invalid padding results in a decoding error.

namespace libbitcoin {
namespace system {

// Use the BIP39 dictionary set.
// This allows the encoding to map to BIP39 in any language.
static const words::mnemonic::catalogs base2048
{
    {
        words::mnemonic::catalog{ language::en, words::mnemonic::en },
        words::mnemonic::catalog{ language::es, words::mnemonic::es },
        words::mnemonic::catalog{ language::it, words::mnemonic::it },
        words::mnemonic::catalog{ language::fr, words::mnemonic::fr },
        words::mnemonic::catalog{ language::cs, words::mnemonic::cs },
        words::mnemonic::catalog{ language::pt, words::mnemonic::pt },
        words::mnemonic::catalog{ language::ja, words::mnemonic::ja },
        words::mnemonic::catalog{ language::ko, words::mnemonic::ko },
        words::mnemonic::catalog{ language::zh_Hans, words::mnemonic::zh_Hans },
        words::mnemonic::catalog{ language::zh_Hant, words::mnemonic::zh_Hant }
    }
};

// encode

static bool encode_base2048(base2048_chunk& out, const string_list& in,
    language language)
{
    // Empty if words not contained in dictionary.
    const auto indexes = base2048.index(in, language);
    if (indexes.empty() && !in.empty())
        return false;

    if (std::any_of(indexes.begin(), indexes.end(),
        [](const int32_t& index) { return is_negative(index); }))
        return false;

    out.resize(indexes.size());
    std::transform(indexes.begin(), indexes.end(), out.begin(),
        [](const int32_t& index) { return static_cast<uint11_t>(index); });

    return true;
}

bool encode_base2048_list(data_chunk& out, const string_list& in,
    language language)
{
    base2048_chunk packed;
    if (!encode_base2048(packed, in, language))
        return false;

    out = base2048_unpack(packed);
    return true;
}

bool encode_base2048(data_chunk& out, const std::string& in,
    language language)
{
    out.clear();

    // An empty string will split into one empty element, so normalize return.
    return in.empty() || encode_base2048_list(out, split(in), language);
}

// decode

static string_list decode_base2048(const base2048_chunk& data,
    language language)
{
    words::mnemonic::catalogs::search indexes(data.size());
    std::transform(data.begin(), data.end(), indexes.begin(),
        [](const uint11_t& index) { return index.convert_to<size_t>(); });

    // Empty if dictionary not found.
    return base2048.at(indexes, language);
}

string_list decode_base2048_list(const data_chunk& data, language language)
{
    return decode_base2048(base2048_pack(data), language);
}

std::string decode_base2048(const data_chunk& data, language language)
{
    // Empty chunk returns empty string, consistent with encoding empty string.
    return join(decode_base2048_list(data, language));
}

// pack/unpack

base2048_chunk base2048_pack(const data_chunk& unpacked)
{
    base2048_chunk packed;
    read::bits::copy source(unpacked);

    while (!source.is_exhausted())
        packed.push_back(source.read_bits(11));

    // Remove an element that is only padding, assumes base2048_unpack encoding.
    // The bit writer writes zeros past end as padding.
    // This is a ((n * 8) / 11) operation, (11 - ((n * 8) % 11)) bits are pad.
    // This padding is in addition to that added by unpacking. When unpacked
    // and then packed this will always result in either no pad bits or a full
    // element of zeros that is padding. This should be apparent from the fact
    // that the number of used bits is unchanged. Remainder indicates padding.
    if (!is_zero((unpacked.size() * 8) % 11))
    {
        // If pad element is non-zero the unpacking was not base2048_unpack.
        // So we return a failure where the condition is detected.
        packed.resize(is_zero(packed.back()) ? sub1(packed.size()) : zero);
    }

    return packed;
}

data_chunk base2048_unpack(const base2048_chunk& packed)
{
    data_chunk unpacked;
    write::bits::data sink(unpacked);

    for (const auto& value: packed)
        sink.write_bits(value.convert_to<uint16_t>(), 11);

    sink.flush();

    // The bit reader reads zeros past end as padding.
    // This is a ((n * 11) / 8) operation, so (8 - ((n * 11) % 8)) are pad.
    ////const auto padded = (packed.size() * 11) % 8 != 0;
    return unpacked;
}

} // namespace system
} // namespace libbitcoin
