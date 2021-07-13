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
#ifndef LIBBITCOIN_SYSTEM_WALLET_MNEMONICS_MNEMONIC_HPP
#define LIBBITCOIN_SYSTEM_WALLET_MNEMONICS_MNEMONIC_HPP

#include <cstddef>
#include <cstdint>
#include <string>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/radix/radix.hpp>
#include <bitcoin/system/wallet/context.hpp>
#include <bitcoin/system/wallet/keys/hd_private.hpp>
#include <bitcoin/system/words/words.hpp>

namespace libbitcoin {
namespace system {
namespace wallet {

/// A wallet mnemonic, as defined by BIP39.
/// Generates menemonic from entropy.
/// Converts valid menemonic to entropy.
/// Derives master keys from menemonic or entropy, and optional passphrase.
class BC_API mnemonic
  : public words::languages
{
public:
    typedef words::mnemonic::catalog dictionary;
    typedef words::mnemonic::catalogs dictionaries;

    /// Supports 128 to 256 bits of entropy, in multiples of 32.
    static constexpr size_t entropy_multiple = 4;
    static constexpr size_t entropy_minimum = 4u * entropy_multiple;
    static constexpr size_t entropy_maximum = 8u * entropy_multiple;

    /// Supports 12 to 24 words (128 to 256 bits), in multiples of 3.
    static constexpr size_t word_multiple = 3;
    static constexpr size_t word_minimum = 4u * word_multiple;
    static constexpr size_t word_maximum = 8u * word_multiple;

    /// The dictionary, limited by identifier, that contains all words.
    /// If 'none' is specified all dictionaries are searched.
    /// If 'none' is specified and contained by both en and fr, returns en.
    static language contained_by(const string_list& words,
        language identifier=language::none);

    /// Valid dictionaries (en, es, it, fr, cs, pt, ja, ko, zh_Hans, zh_Hant).
    static bool is_valid_dictionary(language identifier);

    /// Valid entropy values (16, 20, 24, 28, or 32 bytes).
    static bool is_valid_entropy_size(size_t size);

    /// Valid word counts (12, 15, 18, 21, or 24 words).
    static bool is_valid_word_count(size_t count);

    mnemonic();
    mnemonic(const mnemonic& other);

    /// wiki.trezor.io/recovery_seed
    /// Construct from the "recovery seed" (mnemonic phrase or entropy).
    /// Validity and should be checked after construction.
    mnemonic(const std::string& sentence, language identifier=language::none);
    mnemonic(const string_list& words, language identifier=language::none);
    mnemonic(const data_chunk& entropy, language identifier=language::en);

    /// wiki.trezor.io/recovery_seed
    /// Derive the "master binary seed" from the "recovery seed" and passphrase.
    /// Returns null result with non-ascii passphrase and WITH_ICU undefind.
    long_hash to_seed(const std::string& passphrase="") const;

    /// wiki.trezor.io/account_private_key
    /// Derive the "account private key" from the "master binary seed".
    /// This is also known as the wallet "root key" or "master private key".
    /// hd_private.to_public() is the "master public key".
    /// The "master binary seed" cannot be obtained from the key.
    /// Returns invalid result with non-ascii passphrase and WITH_ICU undefind.
    hd_private to_key(const std::string& passphrase="",
        const context& context=btc_mainnet_p2kh) const;

protected:
    // Constructors.
    mnemonic(const data_chunk& entropy, const string_list& words,
        language identifier);

    /// Derive the checksum byte from entropy, stored in high order bits.
    static uint8_t checksum_byte(const data_slice& entropy);

    /// Map entropy to checksum bit count (4, 5, 6, 7, or 8 bits).
    static size_t checksum_bits(const data_slice& entropy);

    /// Map words to checksum bit count (4, 5, 6, 7, or 8 bits).
    static size_t checksum_bits(const string_list& words);

    /// Map entropy to entropy bit count (128, 160, 192, 224, or 256 bits).
    static size_t entropy_bits(const data_slice& entropy);

    /// Map words to entropy bit count (128, 160, 192, 224, or 256 bits).
    static size_t entropy_bits(const string_list& words);

    /// Map words to entropy size (16, 20, 24, 28, or 32 bytes).
    static size_t entropy_size(const string_list& words);

    /// Map entropy size to word count (12, 15, 18, 21, or 24 words).
    static size_t word_count(const data_slice& entropy);

    static bool is_ambiguous(const string_list& words, language requested,
        language derived);

    static string_list encoder(const data_chunk& entropy, language identifier);
    static data_chunk decoder(const string_list& words, language identifier);
    static long_hash seeder(const string_list& words,
        const std::string& passphrase);

    static mnemonic from_words(const string_list& words, language identifier);
    static mnemonic from_entropy(const data_chunk& entropy,
        language identifier);

private:
    // All Electrum v1 dictionaries, from <dictionaries/mnemonic.cpp>.
    static const dictionaries dictionaries_;
};

} // namespace wallet
} // namespace system
} // namespace libbitcoin

#endif
