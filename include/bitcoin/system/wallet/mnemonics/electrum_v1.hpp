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
#ifndef LIBBITCOIN_SYSTEM_WALLET_MNEMONICS_ELECTRUM_V1_HPP
#define LIBBITCOIN_SYSTEM_WALLET_MNEMONICS_ELECTRUM_V1_HPP

#include <cstddef>
#include <cstdint>
#include <string>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/radix/radix.hpp>
#include <bitcoin/system/wallet/addresses/witness_address.hpp>
#include <bitcoin/system/wallet/context.hpp>
#include <bitcoin/system/wallet/keys/ec_private.hpp>
#include <bitcoin/system/wallet/keys/ec_public.hpp>
#include <bitcoin/system/words/words.hpp>

namespace libbitcoin {
namespace system {
namespace wallet {

/// Helper class for managing decoding overflow bug.
/// github.com/spesmilo/electrum/issues/3149
/// =======================================================================

class v1_decoding
{
public:
    typedef std::vector<bool> overflow;

    v1_decoding();
    v1_decoding(const data_chunk& entropy);
    v1_decoding(const data_chunk& entropy, const overflow& overflows);
    const data_chunk& entropy() const;
    const overflow& overflows() const;
    data_chunk seed_entropy() const;

private:
    data_chunk entropy_;
    overflow overflows_;
};

/// =======================================================================

/// A wallet mnemonic, as defined by the first Electrum implementation.
/// Converts menemonic to entropy.
/// Converts entropy to mnemonic.
/// Derives master keys from menemonic or entropy.
class BC_API electrum_v1
  : public words::languages
{
public:
    typedef words::electrum_v1::catalog dictionary;
    typedef words::electrum_v1::catalogs dictionaries;

    /// Supports 128 or 256 bits of entropy.
    static constexpr size_t entropy_multiple = 4;
    static constexpr size_t entropy_minimum = 4u * entropy_multiple;
    static constexpr size_t entropy_maximum = 8u * entropy_multiple;

    /// Supports 12 or 24 words (128 or 256 bits) of entropy.
    static constexpr size_t word_multiple = 3;
    static constexpr size_t word_minimum = 4u * word_multiple;
    static constexpr size_t word_maximum = 8u * word_multiple;

    /// The two arrays of entropy sizes supported by Electrum v1.
    typedef data_array<entropy_minimum> minimum_entropy;
    typedef data_array<entropy_maximum> maximum_entropy;

    /// The dictionary, limited by identifier, that contains all words.
    /// If 'none' is specified all dictionaries are searched.
    static language contained_by(const string_list& words,
        language identifier=language::none);

    /// Valid dictionaries (en, pt).
    static bool is_valid_dictionary(language identifier);

    /// Valid entropy values (16 or 32 bytes).
    static bool is_valid_entropy_size(size_t size);

    /// Valid word counts (12 or 24 words).
    static bool is_valid_word_count(size_t count);

    electrum_v1();
    electrum_v1(const electrum_v1& other);

    /// Construct from the "recovery seed" (mnemonic phrase or entropy).
    /// Validity should be checked after construction.
    electrum_v1(const std::string& sentence, language identifier=language::none);
    electrum_v1(const string_list& words, language identifier=language::none);
    electrum_v1(const data_chunk& entropy, language identifier=language::en);
    electrum_v1(const minimum_entropy& entropy, language identifier=language::en);
    electrum_v1(const maximum_entropy& entropy, language identifier=language::en);

    /// Derive the "wallet seed"/"master private key" from mnemonic entropy.
    /// ec_private.secret() is the "wallet seed"/"master private key".
    /// ec_private.to_public().point() is the compressed "master public key".
    /// Context sets the version byte for derived payment addresses.
    ec_private to_seed(const context& context=btc_mainnet_p2kh) const;

    /// Derive the wallet "master public key" from entropy.
    /// ec_public.point() is the compressed "master public key".
    /// Context sets the version byte for derived payment addresses.
    ec_public to_public_key(const context& context=btc_mainnet_p2kh) const;

    /// True if the mnemonic words were incorrectly generated.
    /// An overflow does not affect the validity of the object.
    /// If true then entropy will not round trip, but the seed is considered
    /// valid by Electrum (for reasons of backward compatibility). This can only
    /// result from manually-generated menmonics, which were inadvertently
    /// accepted by Electrum. github.com/spesmilo/electrum/issues/3149
    bool overflow() const;

    /// Assignment.
    electrum_v1& operator=(electrum_v1 other);

    /// Swap implementation required to properly handle derived class assign.
    friend void swap(electrum_v1& left, electrum_v1& right);

protected:
    /// Helpers for managing decoding overflow state.
    /// github.com/spesmilo/electrum/issues/3149
    /// =======================================================================

    /// One overflow flag for each word triplet. If overflow() is false then
    /// is all false. Can be used to construct actual entropy by prepending 0x1
    /// ("1" in base16 digits) to the corresponding triplet entropy. Value is
    /// populated to the length [word-count / word_multiple] if the instance is
    /// word-initialized and valid, otherwise the value is empty.
    const v1_decoding::overflow& overflows() const;

    /// This is the denormalized entropy, used for seeding.
    /// The base16 entropy encoding is converted to data with overflows.
    data_chunk seed_entropy() const;

    /// =======================================================================

    /// Constructors.
    electrum_v1(const data_chunk& entropy, const string_list& words,
        language identifier);
    electrum_v1(const v1_decoding& decoding, const string_list& words,
        language identifier);

    /// Map entropy to entropy bit count (128 or 256 bits).
    static size_t entropy_bits(const data_slice& entropy);

    /// Map words to entropy bit count (128 or 256 bits).
    static size_t entropy_bits(const string_list& words);

    /// Map words to entropy size (16 or 32 bytes).
    static size_t entropy_size(const string_list& words);

    /// Map entropy size to word count (12 or 24 words).
    static size_t word_count(const data_slice& entropy);

    static v1_decoding decoder(const string_list& words, language identifier);
    static string_list encoder(const data_chunk& entropy, language identifier);
    static ec_secret strecher(const data_chunk& seed_entropy);

    static electrum_v1 from_words(const string_list& words,
        language identifier);
    static electrum_v1 from_entropy(const data_chunk& entropy,
        language identifier);

private:
    // All Electrum v1 dictionaries, from <dictionaries/electrum_v1.cpp>.
    static const dictionaries dictionaries_;

    // Retain the overflow state for a manually-generated mnemonic. 
    v1_decoding::overflow overflows_;
};

} // namespace wallet
} // namespace system
} // namespace libbitcoin

#endif
