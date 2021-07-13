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
#ifndef LIBBITCOIN_SYSTEM_WALLET_MNEMONICS_ELECTRUM_HPP
#define LIBBITCOIN_SYSTEM_WALLET_MNEMONICS_ELECTRUM_HPP

#include <cstddef>
#include <cstdint>
#include <string>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/radix/radix.hpp>
#include <bitcoin/system/wallet/context.hpp>
#include <bitcoin/system/wallet/keys/ec_private.hpp>
#include <bitcoin/system/wallet/keys/hd_private.hpp>
#include <bitcoin/system/wallet/mnemonics/electrum_v1.hpp>
#include <bitcoin/system/wallet/mnemonics/mnemonic.hpp>
#include <bitcoin/system/words/words.hpp>

namespace libbitcoin {
namespace system {
namespace wallet {

/// A wallet mnemonic, as defined by the second Electrum implementation.
/// Generates menemonic with desired prefix from entropy.
/// Converts menemonic to entropy and prefix.
/// Converts entropy and valid prefix to menemonic.
/// Derives master keys from menemonic or entropy, and optional passphrase.
class BC_API electrum
  : public electrum_v1
{
public:
    /// Seed prefix types.
    /// The values for old, bip39 and none are not actual prefixes but 
    /// here for consistency when handling electrum exceptional conditions.
    enum class seed_prefix: uint8_t
    {
        old,
        bip39,
        standard,
        witness,
        two_factor_authentication,
        two_factor_authentication_witness,
        none
    };

    typedef words::electrum::catalog dictionary;
    typedef words::electrum::catalogs dictionaries;

    // Supports 132 to 506 bits of entropy (12 to 46 words).
    // Entropy byte limits are rounded up to 17 to 64 bytes.
    static constexpr size_t strength_minimum = 132;
    static constexpr size_t strength_maximum = 506;

    /// The dictionary, limited by identifier, that contains all words.
    /// If 'none' is specified all dictionaries are searched.
    /// If 'none' is specified and contained by both en and fr, returns en.
    static language contained_by(const string_list& words,
        language identifier=language::none);

    /// Valid dictionaries (en, es, it, fr, cs, pt, ja, ko, zh_Hans, zh_Hant).
    static bool is_valid_dictionary(language identifier);

    /// Valid entropy values (17 to 64 bytes).
    static bool is_valid_entropy_size(size_t size);
    
    /// Valid word counts (12 to 46 words).
    static bool is_valid_word_count(size_t count);

    /// Returns true if the seed of the words has the given prefix.
    /// Will also match 'old', 'bip39' and 'none', as specified.
    /// Non-ascii words must be nfkd/lower prenormalized if WITH_ICU undefind.
    static bool is_prefix(const string_list& words, seed_prefix prefix);
    static bool is_prefix(const std::string& sentence, seed_prefix prefix);

    /// Obtain the enumerated prefix corresponding to the words.
    /// Returns 'old', 'bip39' or 'none' if not a valid electrum v2 seed.
    /// Non-ascii words must be nfkd/lower prenormalized if WITH_ICU undefind.
    /// A prefix other than 'none' implies the words represent a valid seed.
    static seed_prefix to_prefix(const string_list& words);
    static seed_prefix to_prefix(const std::string& sentence);

    /// Convert a raw for "root seed" to its hd form.
    /// The "root seed" is also referred to by electrum as the "master key".
    /// There is no way to determine if this is a valid "root seed".
    /// The prefix cannot be verified, which requires the "recovery seed".
    static hd_private to_key(const long_hash& seed, const context& context);

    /// Convert an hd form "root seed" to its raw form (loses context).
    /// The "root seed" is also referred to by electrum as the "master key".
    /// There is no way to determine if this is a valid "root seed".
    /// The prefix cannot be verified, which requires the "recovery seed".
    static long_hash to_seed(const hd_private& key);

    electrum();
    electrum(const electrum& other);
    electrum(const electrum_v1& old);

    /// Construct from the "recovery seed" (mnemonic phrase).
    /// Validity and prefix should be checked after construction.
    /// Any valid length of words from a single dictionary will be valid.
    electrum(const string_list& words, language identifier=language::none);
    electrum(const std::string& sentence, language identifier=language::none);

    /// Construct from the "recovery seed" (mnemonic entropy).
    /// Validity should be checked after construction.
    /// By default this only verifies entropy against the prefix.
    /// Set grind limit to allow entropy mutation for prefix discovery.
    /// The instance will be invalid if the prefix not found within the limit.
    electrum(const data_chunk& entropy, seed_prefix prefix, language lexicon,
        size_t grind_limit=0);

    /// The prefix indicates the intended use of the seed.
    seed_prefix prefix() const;

    /// Derive raw form "root seed" from mnemonic entropy and passphrase.
    /// The "root seed" is also referred to by electrum as the "master key".
    /// Returns null result if current prefix is 'none', 'bip39, or 'old'.
    /// Returns null result with non-ascii passphrase and WITH_ICU undefind.
    long_hash to_seed(const std::string& passphrase="") const;

    /// Derive hd form "root seed" from mnemonic entropy and passphrase.
    /// The "root seed" is also referred to as the "master private key".
    /// hd_private.to_public() is the "master public key".
    /// hd_private.secret() + .chain_code() is the raw form "root seed".
    /// Context affects the hd form but does not affect the contained seed.
    /// Returns invalid result if current prefix is 'none', 'bip39, or 'old'.
    /// Returns invalid result with non-ascii passphrase and WITH_ICU undefind.
    hd_private to_key(const std::string& passphrase="",
        const context& context=btc_mainnet_p2kh) const;

protected:
    typedef struct
    {
        data_chunk entropy;
        string_list words;
        size_t iterations;
    } grinding;

    /// Constructors.
    electrum(const data_chunk& entropy, const string_list& words,
        language identifier, seed_prefix prefix);

    /// Map entropy to entropy bit count (132 to 506 bits).
    static size_t entropy_bits(const data_slice& entropy);
    
    /// Map words to entropy bit count (132 to 506 bits).
    static size_t entropy_bits(const string_list& words);

    /// Map entropy bits to sufficient entropy size (17 to 64 bytes).
    static size_t entropy_size(size_t bit_strength);

    /// Map words to entropy size (17 to 64 bytes).
    static size_t entropy_size(const string_list& words);
    
    /// Map entropy size to word count (12 to 46 words).
    static size_t word_count(const data_slice& entropy);

    /// Map entropy bits to sufficient word count (12 to 46 words).
    static size_t word_count(size_t bit_strength);

    /// Map entropy size to number of unused bits.
    static uint8_t unused_bits(const data_slice& entropy);

    /// Determine the number of unused bytes of entropy.
    static uint8_t unused_bytes(const data_slice& entropy);

    /// Map entropy size to usable bytes (unused bits may remain).
    static size_t usable_size(const data_slice& entropy);

    static bool is_conflict(const string_list& words);
    static seed_prefix to_conflict(const string_list& words);

    static seed_prefix normalized_to_prefix(const string_list& words);
    static bool normalized_is_prefix(const string_list& words,
        seed_prefix prefix);

    static bool is_ambiguous(size_t count, seed_prefix prefix);
    static bool is_ambiguous(const string_list& words, language requested,
        language derived);

    static bool is_seedable(seed_prefix prefix);
    static std::string to_version(seed_prefix prefix);

    static string_list encoder(const data_chunk& entropy, language identifier);
    static data_chunk decoder(const string_list& words, language identifier);
    static grinding grinder(const data_chunk& entropy, seed_prefix prefix,
        language identifier, size_t limit);
    static bool validator(const string_list& words, seed_prefix prefix);
    static long_hash seeder(const string_list& words,
        const std::string& passphrase);

    static electrum from_words(const string_list& words, language identifier);
    static electrum from_entropy(const data_chunk& entropy, seed_prefix prefix,
        language identifier, size_t grind_limit);

private:
    // All Electrum dictionaries, subset of <dictionaries/mnemonic.cpp>.
    static const dictionaries dictionaries_;

    // Unique to Electrum v2.
    seed_prefix prefix_;
};

} // namespace wallet
} // namespace system
} // namespace libbitcoin

#endif
