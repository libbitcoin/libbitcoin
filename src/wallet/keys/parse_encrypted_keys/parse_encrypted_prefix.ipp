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
#ifndef LIBBITCOIN_SYSTEM_PARSE_ENCRYPTED_PREFIX_IPP
#define LIBBITCOIN_SYSTEM_PARSE_ENCRYPTED_PREFIX_IPP

#include <cstdint>
#include <cstddef>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/wallet/keys/encrypted_keys.hpp>

namespace libbitcoin {
namespace system {
namespace wallet {

template<uint8_t Size>
parse_encrypted_prefix<Size>::parse_encrypted_prefix(
    const data_array<Size>& value)
  : prefix_(value), valid_(false)
{
}

template<uint8_t Size>
uint8_t parse_encrypted_prefix<Size>::context() const
{
    return prefix_.back();
}

template<uint8_t Size>
data_array<Size> parse_encrypted_prefix<Size>::prefix() const
{
    return prefix_;
}

template<uint8_t Size>
bool parse_encrypted_prefix<Size>::valid() const
{
    return valid_;
}

template<uint8_t Size>
void parse_encrypted_prefix<Size>::valid(bool value)
{
    valid_ = value;
}

} // namespace wallet
} // namespace system
} // namespace libbitcoin

#endif
