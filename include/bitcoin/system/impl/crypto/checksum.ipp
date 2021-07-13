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
#ifndef LIBBITCOIN_CRYPTO_CHECKSUM_IPP
#define LIBBITCOIN_SYSTEM_CHECKSUM_IPP

#include <algorithm>
#include <cstdint>
#include <utility>
#include <bitcoin/system/assert.hpp>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>

namespace libbitcoin {
namespace system {

template <size_t Size, size_t Checksum>
data_array<Size> insert_checksum(const data_loaf& slices)
{
    auto out = build_array<Size>(slices);
    insert_checksum<Size, Checksum>(out);
    return out;
}

template <size_t Size, size_t Checksum>
void insert_checksum(data_array<Size>& data)
{
    static_assert(Checksum <= Size, "insufficient size");
    static_assert(Checksum <= hash_size, "excessive checksum");

    // Obtain the payload iterators.
    const auto payload_begin = data.begin();
    const auto payload_end = std::prev(data.end(), Checksum);

    // Compute the bitcoin hash.
    const auto payload = data_chunk{ payload_begin, payload_end };
    const auto payload_hash = bitcoin_hash(payload);

    // Obtain the hash checksum iterators.
    const auto check_begin = payload_hash.begin();
    const auto check_end = std::next(check_begin, Checksum);

    // Append the checksum from the first hash bytes to after the payload. 
    std::copy(check_begin, check_end, payload_end);
}

template <size_t Size, size_t Checksum>
bool verify_checksum(const data_array<Size>& data)
{
    static_assert(Checksum <= Size, "insufficient size");
    static_assert(Checksum <= hash_size, "excessive checksum");

    // Obtain the payload iterators.
    const auto payload_begin = data.begin();
    const auto payload_end = std::prev(data.end(), Checksum);

    // Compute the bitcoin hash.
    const auto payload = data_chunk{ payload_begin, payload_end };
    const auto payload_hash = bitcoin_hash(payload);

    // Obtain the hash checksum iterators.
    const auto check_begin = payload_hash.begin();
    const auto check_end = std::next(check_begin, Checksum);

    // Compare the checksum from the first hash bytes to those in the payload.
    return std::equal(check_begin, check_end, payload_end);
}

} // namespace system
} // namespace libbitcoin

#endif
