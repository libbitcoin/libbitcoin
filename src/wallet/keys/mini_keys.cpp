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
#include <bitcoin/system/wallet/keys/mini_keys.hpp>

#include <cstdint>
#include <string>
#include <bitcoin/system/crypto/crypto.hpp>

namespace libbitcoin {
namespace system {
namespace wallet {

bool check_minikey(const std::string& minikey)
{
    // Legacy minikeys are 22 chars long
    const auto size = minikey.size();
    const auto valid = size == 22u || size == 30u;
    return valid && sha256_hash(to_chunk(minikey + "?")).front() == 0x00;
}

bool minikey_to_secret(ec_secret& out_secret, const std::string& key)
{
    if (!check_minikey(key))
        return false;

    out_secret = sha256_hash(to_chunk(key));
    return true;
}

} // namespace wallet
} // namespace system
} // namespace libbitcoin
