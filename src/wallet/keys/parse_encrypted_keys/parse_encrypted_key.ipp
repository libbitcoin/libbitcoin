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
#ifndef LIBBITCOIN_SYSTEM_PARSE_ENCRYPTED_KEY_IPP
#define LIBBITCOIN_SYSTEM_PARSE_ENCRYPTED_KEY_IPP

#include <cstdint>
#include <cstddef>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/wallet/keys/encrypted_keys.hpp>
#include "parse_encrypted_prefix.hpp"

namespace libbitcoin {
namespace system {
namespace wallet {

template<size_t PrefixSize>
parse_encrypted_key<PrefixSize>::parse_encrypted_key(
    const data_array<PrefixSize>& prefix, const one_byte& flags,
    const ek_salt& salt, const ek_entropy& entropy)
  : parse_encrypted_prefix<PrefixSize>(prefix),
    flags_(flags), salt_(salt), entropy_(entropy)
{
}

template<size_t PrefixSize>
bool parse_encrypted_key<PrefixSize>::compressed() const
{
    return !is_zero(flags() & ek_flag::ec_compressed_key);
}

template<size_t PrefixSize>
ek_entropy parse_encrypted_key<PrefixSize>::entropy() const
{
    // The owner salt + lot-sequence or owner entropy.
    return entropy_;
}

template<size_t PrefixSize>
uint8_t parse_encrypted_key<PrefixSize>::flags() const
{
    return flags_.front();
}

template<size_t PrefixSize>
bool parse_encrypted_key<PrefixSize>::lot_sequence() const
{
    return !is_zero(flags() & ek_flag::lot_sequence_key);
}

template<size_t PrefixSize>
data_chunk parse_encrypted_key<PrefixSize>::owner_salt() const
{
    // Either 4 or 8 bytes, depending on the lot sequence flags.
    if (lot_sequence())
        return to_chunk(slice<zero, ek_salt_size>(entropy()));
    else
        return to_chunk(entropy());
}

template<size_t PrefixSize>
ek_salt parse_encrypted_key<PrefixSize>::salt() const
{
    // The address hash salt.
    return salt_;
}

} // namespace wallet
} // namespace system
} // namespace libbitcoin

#endif
