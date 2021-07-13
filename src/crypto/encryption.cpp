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
#include <bitcoin/system/crypto/encryption.hpp>

#include <bitcoin/system/assert.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/crypto/elliptic_curve.hpp>
#include <bitcoin/system/crypto/hash.hpp>
#include "external/aes256.h"

namespace libbitcoin {
namespace system {

void aes256_encrypt(const aes_secret& key, aes_block& block)
{
    aes256_context context;
    aes256_init(&context, key.data());
    aes256_encrypt_ecb(&context, block.data());
    aes256_done(&context);
}

void aes256_decrypt(const aes_secret& key, aes_block& block)
{
    aes256_context context;
    aes256_init(&context, key.data());
    aes256_decrypt_ecb(&context, block.data());
    aes256_done(&context);
}

} // namespace system
} // namespace libbitcoin
