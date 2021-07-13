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
#ifndef LIBBITCOIN_SYSTEM_CRYPTO_ENCRYPTION_HPP
#define LIBBITCOIN_SYSTEM_CRYPTO_ENCRYPTION_HPP

#include <cstdint>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {

/**
 * The secret for aes256 block cypher.
 */
constexpr uint8_t aes256_key_size = 32;
typedef data_array<aes256_key_size> aes_secret;

/**
 * The data block for use with aes256 block cypher.
 */
constexpr uint8_t aes256_block_size = 16;
typedef data_array<aes256_block_size> aes_block;

/**
 * Perform aes256 encryption on the specified data block.
 */
BC_API void aes256_encrypt(const aes_secret& key, aes_block& block);

/**
 * Perform aes256 decryption on the specified data block.
 */
BC_API void aes256_decrypt(const aes_secret& key, aes_block& block);

} // namespace system
} // namespace libbitcoin

#endif

