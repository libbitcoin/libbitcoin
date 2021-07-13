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
#ifndef LIBBITCOIN_SYSTEM_WALLET_KEYS_ENCRYPTED_KEYS_HPP
#define LIBBITCOIN_SYSTEM_WALLET_KEYS_ENCRYPTED_KEYS_HPP

#include <string>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/wallet/addresses/payment_address.hpp>

namespace libbitcoin {
namespace system {
namespace wallet {

/**
 * The maximum lot and sequence values for encrypted key token creation.
 */
static constexpr uint32_t ek_max_lot = 1048575;
static constexpr uint32_t ek_max_sequence = 4095;

/**
 * A seed for use in creating an intermediate passphrase (token).
 */
static constexpr size_t ek_salt_size = 4;
typedef data_array<ek_salt_size> ek_salt;

/**
 * A seed for use in creating an intermediate passphrase (token).
 */
static constexpr size_t ek_entropy_size = 8;
typedef data_array<ek_entropy_size> ek_entropy;

/**
 * A seed for use in creating a key pair.
 */
static constexpr size_t ek_seed_size = 24;
typedef data_array<ek_seed_size> ek_seed;

/**
 * An intermediate passphrase (token) type (checked but not base58 encoded).
 */
static constexpr size_t encrypted_token_encoded_size = 72;
static constexpr size_t encrypted_token_decoded_size = 53;
typedef data_array<encrypted_token_decoded_size> encrypted_token;

/**
 * An encrypted private key type (checked but not base58 encoded).
 */
static constexpr size_t ek_private_encoded_size = 58;
static constexpr size_t ek_private_decoded_size = 43;
typedef data_array<ek_private_decoded_size> encrypted_private;

/**
 * DEPRECATED
 * An encrypted public key type (checked but not base58 encoded).
 * This is refered to as a confirmation code in bip38.
 */
static constexpr size_t encrypted_public_encoded_size = 75;
static constexpr size_t encrypted_public_decoded_size = 55;
typedef data_array<encrypted_public_decoded_size> encrypted_public;

// BIP38
// It is requested that the unused flag bytes NOT be used for denoting that
// the key belongs to an alt-chain [This shoud read "flag bits"].
enum ek_flag : uint8_t
{
    lot_sequence_key = 1 << 2,
    ec_compressed_key = 1 << 5,
    ec_non_multiplied_low = 1 << 6,
    ec_non_multiplied_high = 1 << 7,

    /// Two bits are used to represent "not multiplied".
    ec_non_multiplied = (ec_non_multiplied_low | ec_non_multiplied_high)
};

/**
 * Create an encrypted private key from an intermediate passphrase.
 * The `out_point` parameter is always compressed, so to use it it should be
 * decompressed as necessary to match the state of the `compressed` parameter.
 * @param[out] out_private  The new encrypted private key.
 * @param[out] out_point    The ec compressed public key of the new key pair.
 * @param[in]  token        An intermediate passphrase string.
 * @param[in]  seed         A random value for use in the encryption.
 * @param[in]  version      The coin address version byte.
 * @param[in]  compressed   Set true to associate ec public key compression.
 * @return false if the token checksum is not valid.
 */
BC_API bool create_key_pair(encrypted_private& out_private,
    ec_compressed& out_point, const encrypted_token& token,
    const ek_seed& seed, uint8_t version, bool compressed=true);

/**
 * DEPRECATED (scenario)
 * Create an encrypted key pair from an intermediate passphrase.
 * The `out_point` parameter is always compressed, so to use it it should be
 * decompressed as necessary to match the state of the `compressed` parameter.
 * @param[out] out_private  The new encrypted private key.
 * @param[out] out_public   The new encrypted public key.
 * @param[out] out_point    The compressed ec public key of the new key pair.
 * @param[in]  token        An intermediate passphrase string.
 * @param[in]  seed         A random value for use in the encryption.
 * @param[in]  version      The coin address version byte.
 * @param[in]  compressed   Set true to associate ec public key compression.
 * @return false if the token checksum is not valid.
 */
BC_API bool create_key_pair(encrypted_private& out_private,
    encrypted_public& out_public, ec_compressed& out_point,
    const encrypted_token& token, const ek_seed& seed, uint8_t version,
    bool compressed=true);

/**
 * Create an intermediate passphrase for subsequent key pair generation.
 * @param[out] out_token   The new intermediate passphrase.
 * @param[in]  passphrase  A passphrase for use in the encryption.
 * @param[in]  entropy     A random value for use in the encryption.
 * @return false if the token could not be created from the entropy.
 */
BC_API bool create_token(encrypted_token& out_token,
    const std::string& passphrase, const ek_entropy& entropy);

/**
 * Create an intermediate passphrase for subsequent key pair generation.
 * @param[out] out_token   The new intermediate passphrase.
 * @param[in]  passphrase  A passphrase for use in the encryption.
 * @param[in]  salt        A random value for use in the encryption.
 * @param[in]  lot         A lot, max allowed value 1048575 (2^20-1).
 * @param[in]  sequence    A sequence, max allowed value 4095 (2^12-1).
 * @return false if the lot and/or sequence are out of range or the token
 * could not be created from the entropy.
 */
BC_API bool create_token(encrypted_token& out_token,
    const std::string& passphrase, const ek_salt& salt, uint32_t lot,
    uint32_t sequence);

/**
 * Encrypt the ec secret to an encrypted public key using the passphrase.
 * @param[out] out_private  The new encrypted private key.
 * @param[in]  secret       An ec secret to encrypt.
 * @param[in]  passphrase   A passphrase for use in the encryption.
 * @param[in]  version      The coin address version byte.
 * @param[in]  compressed   Set true to associate ec public key compression.
 * @return false if the secret could not be converted to a public key.
 */
BC_API bool encrypt(encrypted_private& out_private, const ec_secret& secret,
    const std::string& passphrase, uint8_t version, bool compressed=true);

/**
 * Decrypt the ec secret associated with the encrypted private key.
 * @param[out] out_secret      The decrypted ec secret.
 * @param[out] out_version     The coin address version.
 * @param[out] out_compressed  The compression of the associated ec public key.
 * @param[in]  key             An encrypted private key.
 * @param[in]  passphrase      The passphrase from the encryption or token.
 * @return false if the key checksum or passphrase is not valid.
 */
BC_API bool decrypt(ec_secret& out_secret, uint8_t& out_version,
    bool& out_compressed, const encrypted_private& key,
    const std::string& passphrase);

/**
 * DEPRECATED (scenario)
 * Decrypt the ec point associated with the encrypted public key.
 * @param[out] out_point       The decrypted ec compressed point.
 * @param[out] out_version     The coin address version of the public key.
 * @param[out] out_compressed  The public key specified compression state.
 * @param[in]  key             An encrypted public key.
 * @param[in]  passphrase      The passphrase of the associated token.
 * @return false if the key checksum or passphrase is not valid.
 */
BC_API bool decrypt(ec_compressed& out_point, uint8_t& out_version,
    bool& out_compressed, const encrypted_public& key,
    const std::string& passphrase);

} // namespace wallet
} // namespace system
} // namespace libbitcoin

#endif
