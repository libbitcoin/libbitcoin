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
#include <bitcoin/system/wallet/mnemonics/electrum.hpp>

#include <cstddef>
#include <cstdint>
#include <string>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/math/math.hpp>
#include <bitcoin/system/radix/radix.hpp>
#include <bitcoin/system/unicode/unicode.hpp>
#include <bitcoin/system/wallet/context.hpp>
#include <bitcoin/system/wallet/keys/ec_private.hpp>
#include <bitcoin/system/wallet/keys/hd_private.hpp>
#include <bitcoin/system/wallet/mnemonics/electrum_v1.hpp>
#include <bitcoin/system/words/words.hpp>

namespace libbitcoin {
namespace system {
namespace wallet {

// local constants
// ----------------------------------------------------------------------------

// Seed prefixes.
// The values for old, bip39 and none are not actual prefixes but included
// here for consistency when handling electrum exceptional conditions.
// The values are selected such that they contain invalid hexidecimal
// characters and therefore cannot match an actual seed prefix.
static const auto version_old = "old";
static const auto version_bip39 = "bip39";
static const auto version_standard = "01";
static const auto version_witness = "100";
static const auto version_two_factor_authentication = "101";
static const auto version_two_factor_authentication_witness = "102";
static const auto version_none = "none";

// 2^11 = 2048 implies 11 bits exactly indexes every possible dictionary word.
static const auto index_bits = static_cast<uint8_t>(floored_log2(
    electrum::dictionary::size()));

// private static
// ----------------------------------------------------------------------------

const electrum::dictionaries electrum::dictionaries_
{
    {
        electrum::dictionary{ language::en, words::electrum::en },
        electrum::dictionary{ language::es, words::electrum::es },
        electrum::dictionary{ language::it, words::electrum::it },
        electrum::dictionary{ language::fr, words::electrum::fr },
        electrum::dictionary{ language::cs, words::electrum::cs },
        electrum::dictionary{ language::pt, words::electrum::pt },
        electrum::dictionary{ language::ja, words::electrum::ja },
        electrum::dictionary{ language::ko, words::electrum::ko },
        electrum::dictionary{ language::zh_Hans, words::electrum::zh_Hans },
        electrum::dictionary{ language::zh_Hant, words::electrum::zh_Hant }
    }
};

// protected static (coders)
// ----------------------------------------------------------------------------

// Electrum entropy is an entirely private (internal) format.
string_list electrum::encoder(const data_chunk& entropy, language identifier)
{
    // Bytes are the base2048 encoding, so this is byte decoding.
    return decode_base2048_list(entropy, identifier);
}

// Electrum entropy is an entirely private (internal) format.
data_chunk electrum::decoder(const string_list& words, language identifier)
{
    // Words are the base2048 decoding, so this is word encoding.
    data_chunk out;
    return encode_base2048_list(out, words, identifier) ? out : data_chunk{};
}

// Electrum also grinds randoms on a sequential nonce until the entropy is high
// enough to ensure population of words that represent the intended bit
// strength of the seed. But we do not use an internal PRNG and instead
// only accept base entropy from the caller. We want determinism from the
// entropy, and we cannot truly increase randomness. So we maintain entropy
// strength by using full provided entropy and produce exact word count.
// This is done by iterating a hash function so an arbitrary limit is required.
// The Electrum prng minimum value technique sacrifices 11 bits of entropy by
// discarding any prng value that is below 2^(strength-11).
// github.com/spesmilo/electrum/blob/master/electrum/mnemonic.py#L190-L205
electrum::grinding electrum::grinder(const data_chunk& entropy,
    seed_prefix prefix, language identifier, size_t limit)
{
    string_list words;
    data_chunk hash(entropy);
    const auto start = limit;

    // Remove unusable entropy bytes.
    const auto entropy_size = usable_size(hash);
    hash.resize(entropy_size);

    // Create a byte mask for zeroizing entropy pad bits.
    const auto padding_mask = 0xff << unused_bits(hash);

    // This just grinds away until exhausted or prefix found.
    // Previously-discovered entropy round-trips, matching on the first pass.
    do
    {
        // Normalize entropy to the wordlist by managing its pad bits.
        // Electrum pads to the left, but entropy is a private format for
        // electrum and public for bip39, so we use the bip39/mnemonic format.
        // This results in any electrum entropy/prefix value producing the same
        // words as that same entropy/checksum value in bip39/mnemonic.
        hash[entropy_size - 1u] &= padding_mask;
        words = encoder(hash, identifier);

        // Avoid collisions with Electrum v1 (en) and BIP39 mnemonics.
        // Run validator first because conflict checks can be costly.
        if (validator(words, prefix) && !is_conflict(words))
            return { hash, words, start - limit };

        // This replaces Electrum's prng with determinism.
        hash = to_chunk(sha512_hash(hash));
        hash.resize(entropy_size);
    }
    while (!is_zero(limit--));

    return { {}, {}, start };
}

// This cannot match electrum_v1 or mnemonic.
bool electrum::validator(const string_list& words, seed_prefix prefix)
{
    // Words are in normal (lower, nfkd) form, even without ICU.
    auto sentence = system::join(words);
    sentence = to_non_combining_form(sentence);
    sentence = to_compressed_form(sentence);

    const auto seed = encode_base16(hmac_sha512_hash(sentence, "Seed version"));
    return starts_with(seed, to_version(prefix));
}

// Electrum uses the same normalization function for words and passphrases.
// Passpharse entropy loss from lowering (and normalizing) should be considered.
// github.com/spesmilo/electrum/blob/master/electrum/mnemonic.py#L77
long_hash electrum::seeder(const string_list& words,
    const std::string& passphrase)
{
    constexpr size_t hmac_iterations = 2048;
    constexpr auto passphrase_prefix = "electrum";

    // Passphrase is limited to ascii (normal) if WITH_ICU undefined.
    auto phrase = passphrase;

    LCOV_EXCL_START("Always succeeds unless WITH_ICU undefined.")

    // TODO: replace the ICU lib dependency with Python's unicodedata. 
    // Conforms to the Unicode Standard for nfkd and case lowering (ICU lib).
    // ------------------------------------------------------------------------
    // seed = unicodedata.normalize('NFKD', seed)
    // seed = seed.lower()
    // ------------------------------------------------------------------------
    // These can only return false if non-ascii phrase and WITH_ICU undefined.
    if (!to_compatibility_decomposition(phrase) || !to_lower(phrase))
        return {};

    LCOV_EXCL_STOP()

    // Python's unicodedata.combining returns true if the character is of the
    // canonical combining class (which may or may not be a diacritic).
    // Libbitcoin uses Python's unicodedata implementation for consistency.
    // ------------------------------------------------------------------------
    // seed = u''.join([c for c in seed if not unicodedata.combining(c)])
    // ------------------------------------------------------------------------
    // Remove combining class characters (from the nfkd form converted above).
    phrase = to_non_combining_form(phrase);

    // Python string.whitespace represents the 6 ascii/C whitespace chars:
    // print(string.whitespace) => 09 0d 0a 0b 0c 0d 0a 20 (two duplicates).
    // Libbitcoin uses CJK intervals from Electrum sources for consistency.
    // ------------------------------------------------------------------------
    // seed = u''.join([seed[i] for i in range(len(seed))
    //    if not (seed[i] in string.whitespace and is_CJK(seed[i-1]) and
    //       is_CJK(seed[i+1]))])
    // ------------------------------------------------------------------------
    // Compress ascii whitespace and remove ascii spaces between cjk characters.
    phrase = to_compressed_form(phrase);

    // Python unicode splits on all unicode separators:
    // print(u'x\u3000y z'.split()) => [u'x', u'y', u'z']
    // Electrum joins with an ascii space (0x20) as confirmed by test results.
    // ------------------------------------------------------------------------
    // seed = u' '.join(seed.split())
    // ------------------------------------------------------------------------
    // Words are normal (lower, nfkd) form even without ICU (dictionary-match).
    auto sentence = system::join(words);
    sentence = to_non_combining_form(sentence);
    sentence = to_compressed_form(sentence);

    const auto data = to_chunk(sentence);
    const auto salt = to_chunk(passphrase_prefix + phrase);
    return pkcs5_pbkdf2_hmac_sha512(data, salt, hmac_iterations);
}

// protected static (sizers)
// ----------------------------------------------------------------------------

size_t electrum::entropy_bits(const data_slice& entropy)
{
    // The number of bits for the given number of bytes.
    return to_bits(entropy.size());
}

size_t electrum::entropy_bits(const string_list& words)
{
    // The number of bits for the given number of words.
    return words.size() * index_bits;
}

size_t electrum::entropy_size(size_t bit_strength)
{
    // The required number of bytes to achieve the given bit strength.
    return ceilinged_divide(bit_strength, byte_bits);
}

size_t electrum::entropy_size(const string_list& words)
{
    // The required number of bytes for the given number of words.
    return ceilinged_divide(entropy_bits(words), byte_bits);
}

size_t electrum::word_count(const data_slice& entropy)
{
    // The number of words that can be derived from the given entropy size.
    return entropy_bits(entropy) / index_bits;
}

size_t electrum::word_count(size_t bit_strength)
{
    // The required number of words to achieve the given bit strength.
    return ceilinged_divide(bit_strength, index_bits);
}

uint8_t electrum::unused_bits(const data_slice& entropy)
{
    // 0..10 unused bits are possible given arbitrary entropy size.
    return entropy_bits(entropy) % index_bits;
}

uint8_t electrum::unused_bytes(const data_slice& entropy)
{
    // 0..10 unused bits implies we can discard up to one byte.
    return to_bytes(unused_bits(entropy));
}

size_t electrum::usable_size(const data_slice& entropy)
{
    return entropy.size() - unused_bytes(entropy);
}

// protected static (checkers)
// ----------------------------------------------------------------------------

bool electrum::is_ambiguous(size_t count, seed_prefix prefix)
{
    constexpr size_t minimum_two_factor_authentication_words = 20;

    // HACK: github.com/spesmilo/electrum/blob/master/electrum/mnemonic.py#L258
    // In Electrum 2.7, there was a breaking change in key derivation for
    // two_factor_authentication. Unfortunately the seed prefix was reused,
    // and now we can only distinguish them based on number of words. :(
    if (prefix != seed_prefix::two_factor_authentication)
        return false;

    return
        count != word_count(strength_minimum) &&
        count < minimum_two_factor_authentication_words;
}

bool electrum::is_ambiguous(const string_list& words, language requested,
    language derived)
{
    // HACK: There are 100 same words in en/fr, all with distinct indexes.
    // If matches en and unspecified then check fr, since en is searched first.
    return
        derived == language::en &&
        requested == language::none &&
        contained_by(words, language::fr) == language::fr;
}

bool electrum::is_conflict(const string_list& words)
{
    return to_conflict(words) != seed_prefix::none;
}

electrum::seed_prefix electrum::to_conflict(const string_list& words)
{
    // Electrum only considers english for electrum_v1 conflicts.
    // Five electrum languages contain electrum_v1 english words.
    if (electrum_v1(words, language::en))
        return seed_prefix::old;

    // Generating a mnemonic checksum is unlikely but possible. But a generated
    // mnemonic (with valid checksum) can more easily match an electrum prefix.
    if (mnemonic(words))
        return seed_prefix::bip39;

    return seed_prefix::none;
}

bool electrum::normalized_is_prefix(const string_list& words, seed_prefix prefix)
{
    if (prefix == seed_prefix::old)
        return electrum_v1(words, language::en);

    if (prefix == seed_prefix::bip39)
        return mnemonic(words);

    // HACK: 2fa prefix ambiguity.
    if (is_ambiguous(words.size(), prefix))
        return false;

    if (validator(words, prefix))
        return true;

    return prefix == seed_prefix::none;
}

electrum::seed_prefix electrum::normalized_to_prefix(const string_list& words)
{
    const auto conflict = to_conflict(words);

    // Prioritize conflicts since electrum doesn't generate them.
    if (conflict != seed_prefix::none)
        return conflict;

    // Words are in normal form, even without ICU.
    if (normalized_is_prefix(words, seed_prefix::standard))
        return seed_prefix::standard;
    if (normalized_is_prefix(words, seed_prefix::witness))
        return seed_prefix::witness;
    if (normalized_is_prefix(words, seed_prefix::two_factor_authentication))
        return seed_prefix::two_factor_authentication;
    if (normalized_is_prefix(words, seed_prefix::two_factor_authentication_witness))
        return seed_prefix::two_factor_authentication_witness;

    return seed_prefix::none;
}

// public static
// ----------------------------------------------------------------------------

language electrum::contained_by(const string_list& words, language identifier)
{
    return dictionaries_.contains(words, identifier);
}

bool electrum::is_valid_dictionary(language identifier)
{
    return dictionaries_.exists(identifier);
}

bool electrum::is_valid_entropy_size(size_t size)
{
    // Electrum has no explicit minimum or maximum strengths.
    // Upper bounds here are based on use of a 512 bit hash function
    // in grind() and transportation of entropy in byte vectors.
    // A 64 byte seed (512 / 11) is 46 words (6 unused bits).
    // This limits strength to 506 bits (BIP39 is 256).
    // Lower bounds here are based on seed strength.
    // A 132 bit (exactly 12 word) seed is the Electrum default.
    // A 128 bit seed is the BIP39 minium, but this 11 Electrum words.
    // So our limits are 132 to 506 bits (12 to 46) words.

    return size >= entropy_size(strength_minimum) &&
        size <= entropy_size(strength_maximum);
}

bool electrum::is_valid_word_count(size_t count)
{
    return count >= word_count(strength_minimum) &&
        count <= word_count(strength_maximum);
}

bool electrum::is_seedable(seed_prefix prefix)
{
    // Only seed from native entropy.
    switch (prefix)
    {
        case seed_prefix::standard:
        case seed_prefix::witness:
        case seed_prefix::two_factor_authentication:
        case seed_prefix::two_factor_authentication_witness:
            return true;
        default:
            return false;
    }
}

bool electrum::is_prefix(const string_list& words, seed_prefix prefix)
{
    // Normalize to improve chance of dictionary matching.
    const auto tokens = try_normalize(words);

    // Tokens are accepted if they are contained by any single dictionary.
    if (contained_by(tokens) == language::none)
        return prefix == seed_prefix::none;

    return normalized_is_prefix(tokens, prefix);
}

bool electrum::is_prefix(const std::string& sentence, seed_prefix prefix)
{
    return is_prefix(split(sentence, language::none), prefix);
}

electrum::seed_prefix electrum::to_prefix(const string_list& words)
{
    // Normalize to improve chance of dictionary matching.
    const auto tokens = try_normalize(words);

    // Tokens are accepted if they are contained by any single dictionary.
    if (contained_by(tokens) == language::none)
        return seed_prefix::none;

    return normalized_to_prefix(words);
}

electrum::seed_prefix electrum::to_prefix(const std::string& sentence)
{
    return to_prefix(split(sentence, language::none));
}

std::string electrum::to_version(seed_prefix prefix)
{
    switch (prefix)
    {
        case seed_prefix::old:
            return version_old;
        case seed_prefix::bip39:
            return version_bip39;
        case seed_prefix::standard:
            return version_standard;
        case seed_prefix::witness:
            return version_witness;
        case seed_prefix::two_factor_authentication:
            return version_two_factor_authentication;
        case seed_prefix::two_factor_authentication_witness:
            return version_two_factor_authentication_witness;
        case seed_prefix::none:
        default:
            return version_none;
    }
}

// construction
// ----------------------------------------------------------------------------

electrum::electrum()
  : electrum_v1(), prefix_(seed_prefix::none)
{
}

electrum::electrum(const electrum& other)
  : electrum_v1(other), prefix_(other.prefix_)
{
}

electrum::electrum(const electrum_v1& old)
  : electrum_v1(old), prefix_(*this ? seed_prefix::old : seed_prefix::none)
{
}

electrum::electrum(const std::string& sentence, language identifier)
  : electrum(split(sentence, identifier), identifier)
{
}

electrum::electrum(const string_list& words, language identifier)
  : electrum(from_words(words, identifier))
{
}

electrum::electrum(const data_chunk& entropy, seed_prefix prefix,
    language identifier, size_t grind_limit)
  : electrum(from_entropy(entropy, prefix, identifier, grind_limit))
{
}

// protected
electrum::electrum(const data_chunk& entropy, const string_list& words,
    language identifier, seed_prefix prefix)
  : electrum_v1(entropy, words, identifier), prefix_(prefix)
{
}

// protected static (factories)
// ----------------------------------------------------------------------------

// To test existing entropy a caller should set grind_limit to zero (default).
electrum electrum::from_entropy(const data_chunk& entropy, seed_prefix prefix,
    language identifier, size_t grind_limit)
{
    // If allowed this would fail after grinding to limit.
    if (prefix == seed_prefix::none)
        return {};

    // Use mnemonic class instead.
    if (prefix == seed_prefix::bip39)
        return {};

    // Generates entropy from electrum_v1 entropy sizes.
    if (prefix == seed_prefix::old)
        return { entropy };

    if (!is_valid_entropy_size(entropy.size()))
        return {};

    if (!dictionaries_.exists(identifier))
        return {};

    // HACK: 2fa ambiguity.
    if (is_ambiguous(word_count(entropy), prefix))
        return {};

    // If prefix is 'none' this will return the first non-prefixed result.
    const auto grinding = grinder(entropy, prefix, identifier, grind_limit);

    // Not your lucky day.
    if (grinding.words.empty())
        return {};

    // Save derived words and ground entropy, original is discarded.
    return { grinding.entropy, grinding.words, identifier, prefix };
}

electrum electrum::from_words(const string_list& words, language identifier)
{
    if (!is_valid_word_count(words.size()))
        return {};

    // Prioritizes electrum_v1 (en) as electrum cannot generate the conflict.
    electrum_v1 old{ words, identifier };
    if (old)
        return old;

    // Normalize to improve chance of dictionary matching.
    const auto tokens = try_normalize(words);
    const auto lexicon = contained_by(tokens, identifier);

    if (lexicon == language::none)
        return {};

    if (identifier != language::none && lexicon != identifier)
        return {};

    // HACK: en-fr dictionary ambiguity.
    if (is_ambiguous(tokens, identifier, lexicon))
        return {};

    // Above prioritizes electrum matching, but if that fails then this will
    // be identified as a valid bip39 if the checksum validates. This is likely
    // in the case where a valid bip39 mnemonic is passed via the constructor.
    const auto prefix = normalized_to_prefix(tokens);

    // Save derived entropy and dictionary words, originals are discarded.
    return { decoder(tokens, lexicon), tokens, lexicon, prefix };
}

// public
// ----------------------------------------------------------------------------

electrum::seed_prefix electrum::prefix() const
{
    return prefix_;
}

long_hash electrum::to_seed(const std::string& passphrase) const
{
    if (!(*this))
        return {};

    if (!is_seedable(prefix_))
        return {};

    return seeder(words(), passphrase);
}

hd_private electrum::to_key(const std::string& passphrase,
    const context& context) const
{
    if (!(*this))
        return {};

    if (!is_seedable(prefix_))
        return {};

    // Bypass a BIP32 step, splitting directly to secret/chaincode.
    const auto halves = system::split(to_seed(passphrase));

    // The key will be invalid if the secret (part.first) does not ec verify.
    return { halves.first, halves.second, context.hd_prefixes() };
}

// static public (conversions)
// ----------------------------------------------------------------------------

hd_private electrum::to_key(const long_hash& seed, const context& context)
{
    const auto halves = system::split(seed);
    return { halves.first, halves.second, context.hd_prefixes() };
}

long_hash electrum::to_seed(const hd_private& key)
{
    return splice(key.secret(), key.chain_code());
}

} // namespace wallet
} // namespace system
} // namespace libbitcoin
