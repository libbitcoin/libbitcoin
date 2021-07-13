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
#ifndef LIBBITCOIN_SYSTEM_RADIX_BASE_2048_HPP
#define LIBBITCOIN_SYSTEM_RADIX_BASE_2048_HPP

#include <string>
#include <vector>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/words/words.hpp>

namespace libbitcoin {
namespace system {

typedef std::vector<uint11_t> base2048_chunk;

// This is the encoding used by both BIP39 and Electrum mnemonics.
// Dictionaries are unicode lower case and nfkd normalized.
// Words must be space (0x20) delimited and match dictionary (bytewise).
// The packing direction is reversed from base32 encoding, since base2048
// elements are larger than byte elements. This means that round trip must
// start with words, not bytes. Coding in the reverse direction is ambiguous
// and will lead to full pad bytes in 3 out of every 12 bit lengths. Words are
// encoded as bytes, not bytes as words. Words are authoritative for entropy.

/**
 * Convert a base2048 sentence to bytes.
 * False if any word is not from the specified dictionary.
 */
BC_API bool encode_base2048(data_chunk& out, const std::string& in,
    language language =language::en);

/**
* Convert bytes to a base2048 sentence in the specified language.
* Returns empty if language is not a supported dictionary.
*/
BC_API std::string decode_base2048(const data_chunk& data,
    language language=language::en);

/**
 * Convert a base2048 word list to bytes.
 * False if any word is not from the specified dictionary.
 */
BC_API bool encode_base2048_list(data_chunk& out, const string_list& in,
    language language=language::en);

/**
* Convert bytes to a base2048 word list in the specified language.
* Returns empty if language is not a supported dictionary.
*/
BC_API string_list decode_base2048_list(const data_chunk& data,
    language language=language::en);

/**
 * Pack any vector of 8 bit bytes to vector of 11 bit bytes.
 */
BC_API base2048_chunk base2048_pack(const data_chunk& unpacked);

/**
 * Unpack any vector of 11 bit bytes to a vector of 8 bit bytes.
 */
BC_API data_chunk base2048_unpack(const base2048_chunk& packed);

// TODO: en.cppreference.com/w/cpp/language/user_literal

} // namespace system
} // namespace libbitcoin

#endif
