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
#include "parse_encrypted_public.hpp"

#include <cstdint>
#include <cstddef>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/wallet/keys/encrypted_keys.hpp>
#include "parse_encrypted_key.hpp"
#include "parse_encrypted_prefix.hpp"

namespace libbitcoin {
namespace system {
namespace wallet {

// This prefix results in the prefix "cfrm" in the base58 encoding but is
// modified when the payment address is Bitcoin mainnet (0).
const data_array<parse_encrypted_public::magic_size>
parse_encrypted_public::magic_
{
    { 0x64, 0x3b, 0xf6, 0xa8 }
};

data_array<parse_encrypted_public::prefix_size>
parse_encrypted_public::prefix_factory(uint8_t address)
{
    const auto context = default_context_ + address;
    return splice(magic_, to_array(context));
}

parse_encrypted_public::parse_encrypted_public(const encrypted_public& key)
  : parse_encrypted_key<prefix_size>(
        slice<0, 5>(key),
        slice<5, 6>(key),
        slice<6, 10>(key),
        slice<10, 18>(key)),
    sign_(slice<18, 19>(key)),
    data_(slice<19, 51>(key))
{
    valid(verify_magic() && verify_checksum(key));
}

uint8_t parse_encrypted_public::address_version() const
{
    return context() - default_context_;
}

hash_digest parse_encrypted_public::data() const
{
    return data_;
}

one_byte parse_encrypted_public::sign() const
{
    return sign_;
}

bool parse_encrypted_public::verify_magic() const
{
    return slice<zero, magic_size>(prefix()) == magic_;
}

} // namespace wallet
} // namespace system
} // namespace libbitcoin
