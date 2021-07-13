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
#ifndef LIBBITCOIN_SYSTEM_PARSE_ENCRYPTED_KEY_HPP
#define LIBBITCOIN_SYSTEM_PARSE_ENCRYPTED_KEY_HPP

#include <cstdint>
#include <cstddef>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/wallet/keys/encrypted_keys.hpp>
#include "parse_encrypted_prefix.hpp"

namespace libbitcoin {
namespace system {
namespace wallet {

// Swap not defined.
template<size_t PrefixSize>
class parse_encrypted_key
  : public parse_encrypted_prefix<PrefixSize>
{
public:
    parse_encrypted_key(const data_array<PrefixSize>& prefix,
        const one_byte& flags, const ek_salt& salt, const ek_entropy& entropy);

    bool compressed() const;
    bool lot_sequence() const;
    data_chunk owner_salt() const;

    uint8_t flags() const;
    ek_salt salt() const;
    ek_entropy entropy() const;

private:
    const one_byte flags_;
    const ek_salt salt_;
    const ek_entropy entropy_;
};

} // namespace wallet
} // namespace system
} // namespace libbitcoin

#include "parse_encrypted_key.ipp"

#endif
