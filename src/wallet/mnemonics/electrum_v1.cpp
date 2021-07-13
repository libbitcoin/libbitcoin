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
#include <bitcoin/system/wallet/mnemonics/electrum_v1.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <bitcoin/system/assert.hpp>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/math/math.hpp>
#include <bitcoin/system/stream/stream.hpp>
#include <bitcoin/system/wallet/context.hpp>
#include <bitcoin/system/wallet/keys/ec_private.hpp>
#include <bitcoin/system/wallet/keys/ec_public.hpp>
#include <bitcoin/system/words/words.hpp>

namespace libbitcoin {
namespace system {
namespace wallet {

// Helper class for managing decoding overflow bug.
// ============================================================================

// github.com/spesmilo/electrum/issues/3149
// "While (32-long) hex seeds necessarily map to (12-long) seed
// words/"mnemonics", the inverse is not always true. For example, 'hurry idiot
// prefer sunset mention mist jaw inhale impossible kingdom rare squeeze'
// maps to 025d2f2d005036911003ca78900ca155c (33 chars)."
// By word triplet this is: [025d2f2d][00503691][1003ca789][00ca155c], where
// 'jaw inhale impossible' maps to 1003ca789 (33 bits, overflowing uint32).
// It is the distance between words that creates the overflow.
// Electrum v1 failed to catch this overflow, so "old seed" checks presently
// allow it, despite the invalidity. In other words, any list of 12/24 electrum
// v1 (en) words is considered valid v1 by later versions of electrum. It
// passes the decoded entropy to the strecher, creating the master private key
// despite the error. These seed words cannot be recovered from entropy as the
// encoding algorithm only parses 32 bits of entropy for each word triplet.
// But electrum itself cannot round trip these seeds, so we do not support it.
// Electrum concatenates hex characters for each triplet.
// The result can be an odd number of characters (9 total max).
// print('%08x' % x) emits >= 8 hex characters, and can be odd length.

v1_decoding::v1_decoding()
  : v1_decoding({}, {})
{
}

v1_decoding::v1_decoding(const data_chunk& entropy)
  : v1_decoding(entropy, {})
{
}

v1_decoding::v1_decoding(const data_chunk& entropy, const overflow& overflows)
  : entropy_(entropy), overflows_(overflows)
{
}

const data_chunk& v1_decoding::entropy() const
{
    return entropy_;
}

const v1_decoding::overflow& v1_decoding::overflows() const
{
    return overflows_;
}

// Electrum hashes the base16 encoded ascii *text* of the seed.
data_chunk v1_decoding::seed_entropy() const
{
    // overflows is empty for entropy constructions.
    if (overflows_.empty())
        return to_chunk(encode_base16(entropy_));

    // overflow count must always be the count of entropy uint32_t.
    if (overflows_.size() * sizeof(uint32_t) != entropy_.size())
        return {};

    read::bytes::copy in(entropy_);
    auto it = overflows_.begin();
    std::string hexadecimal;

    while (!in.is_exhausted())
    {
        // This is a big-endian read of a big-endian byte stream.
        const auto value = in.read_bytes(sizeof(uint32_t));

        // This inserts any overflow bits.
        // See comments on the decoder overflow bug.
        hexadecimal.append((*it++ ? "1" : "") + encode_base16(value));
    }

    return to_chunk(hexadecimal);
}

// ============================================================================

// Guard: decoder 'value' cannot exceed 0x1003ca7a7, safe in int64_t but can
// overflow int32_t/uint32_t. Only one bit (0x[1][003ca7a7]) can overflow.
// So we upcast indexes to int64 and later check the result for overflow.
static_assert((1625 * 1) + (1625 * 1626) + (1625 * 1626 * 1626ll) ==
    0x1003ca7a7, "upper bound on electrum_v1 decoded triplet");

// local constants
// ----------------------------------------------------------------------------

static const auto size = static_cast<int>(electrum_v1::dictionary::size());
static const auto size0 = power<int64_t>(size, 0);
static const auto size1 = power<int64_t>(size, 1);
static const auto size2 = power<int64_t>(size, 2);

// private static
// ----------------------------------------------------------------------------

const electrum_v1::dictionaries electrum_v1::dictionaries_
{
    {
        electrum_v1::dictionary{ language::en, words::electrum_v1::en },
        electrum_v1::dictionary{ language::pt, words::electrum_v1::pt }
    }
};

// protected static (coders)
// ----------------------------------------------------------------------------

// electrum/old_mnemonic.py#L1669
string_list electrum_v1::encoder(const data_chunk& entropy, language identifier)
{
    string_list words;
    words.reserve(word_count(entropy));
    read::bytes::copy reader(entropy);

    while (!reader.is_exhausted())
    {
        // Guard: entropy verified as multiples of four bytes.
        // Guard: below cannot overflow even if this reads max_uint32.
        const auto value = reader.read_4_bytes_big_endian();

        // [(value/size0 + 0)%size1] is a nop, shown for consistency.
        // Pos quotient integer div/mod is floor in c++ and python[2/3].
        const auto uno = (value / size0 + 0x0) % size1;
        const auto dos = (value / size1 + uno) % size1;
        const auto tre = (value / size2 + dos) % size1;

        // Guard: dictionary membership has been verified.
        // Guard: any positive number modulo 1626 is always [0-1625].
        words.push_back(dictionaries_.at(uno, identifier));
        words.push_back(dictionaries_.at(dos, identifier));
        words.push_back(dictionaries_.at(tre, identifier));
    }

    return words;
}

// electrum/old_mnemonic.py#L1682
v1_decoding electrum_v1::decoder(const string_list& words, language identifier)
{
    data_chunk entropy;
    entropy.reserve(entropy_size(words));
    write::bytes::data out(entropy);

    // See comments above on electrum v1 decoder overflow bug.
    v1_decoding::overflow overflows(words.size() / word_multiple);
    auto overflow = overflows.begin();

    // Word count and dictionary membership must have been validated.
    for (auto word = words.begin(); word != words.end();)
    {
        // Electrum modulos dos and tri by size1, which is a nop.
        const int64_t uno = dictionaries_.index(*word++, identifier);
        const int64_t dos = dictionaries_.index(*word++, identifier);
        const int64_t tre = dictionaries_.index(*word++, identifier);

        // [floored_modulo(uno-0, size1)*size0] is a nop, shown for consistency.
        // Neg quotient integer div/mod is ceil in c++ and floor in python[2/3].
        const auto value =
            floored_modulo(uno - 0x0, size1) * size0 +
            floored_modulo(dos - uno, size1) * size1 +
            floored_modulo(tre - dos, size1) * size2;

        // Overflow: if true then this mnemonic was not encoded from entropy.
        *overflow++ = is_greater(value, max_uint32);

        // Guard: floored modulo with positive divisor is always positive.
        // Casting away a non-zero (overflow bit) will prevent round trip.
        out.write_4_bytes_big_endian(static_cast<uint32_t>(value));
    }

    out.flush();
    return { entropy, overflows };
}

// electrum/keystore.py#L692
hash_digest electrum_v1::strecher(const data_chunk& seed_entropy)
{
    constexpr size_t hash_iterations = 100000;

    auto streched = seed_entropy;
    for (size_t count = 0; count < hash_iterations; ++count)
        streched = sha256_hash_chunk(splice(streched, seed_entropy));

    return to_array<hash_size>(streched);
}

// protected static (sizers)
// ----------------------------------------------------------------------------

size_t electrum_v1::entropy_bits(const data_slice& entropy)
{
    return to_bits(entropy.size());
}

size_t electrum_v1::entropy_bits(const string_list& words)
{
    return (words.size() * sizeof(uint32_t)) / word_multiple;
}

size_t electrum_v1::entropy_size(const string_list& words)
{
    return to_bytes(entropy_bits(words));
}

size_t electrum_v1::word_count(const data_slice& entropy)
{
    return entropy_bits(entropy) / sizeof(uint32_t);
}

// public static
// ----------------------------------------------------------------------------

language electrum_v1::contained_by(const string_list& words,
    language identifier)
{
    return dictionaries_.contains(words, identifier);
}

bool electrum_v1::is_valid_dictionary(language identifier)
{
    return dictionaries_.exists(identifier);
}

bool electrum_v1::is_valid_entropy_size(size_t size)
{
    return size == entropy_minimum || size == entropy_maximum;
}

bool electrum_v1::is_valid_word_count(size_t count)
{
    return count == word_minimum || count == word_maximum;
}

// construction
// ----------------------------------------------------------------------------

electrum_v1::electrum_v1()
  : languages(), overflows_()
{
}

electrum_v1::electrum_v1(const electrum_v1& other)
  : languages(other), overflows_(other.overflows_)
{
}

electrum_v1::electrum_v1(const std::string& sentence, language identifier)
  : electrum_v1(split(sentence, identifier), identifier)
{
}

electrum_v1::electrum_v1(const string_list& words, language identifier)
  : electrum_v1(from_words(words, identifier))
{
}

electrum_v1::electrum_v1(const data_chunk& entropy, language identifier)
  : electrum_v1(from_entropy(entropy, identifier))
{
}

electrum_v1::electrum_v1(const minimum_entropy& entropy, language identifier)
  : electrum_v1(from_entropy(to_chunk(entropy), identifier))
{
}

electrum_v1::electrum_v1(const maximum_entropy& entropy, language identifier)
  : electrum_v1(from_entropy(to_chunk(entropy), identifier))
{
}

// protected
electrum_v1::electrum_v1(const data_chunk& entropy, const string_list& words,
    language identifier)
  : electrum_v1(v1_decoding(entropy), words, identifier)
{
}

// protected
electrum_v1::electrum_v1(const v1_decoding& decoding, const string_list& words,
    language identifier)
  : languages(decoding.entropy(), words, identifier),
    overflows_(decoding.overflows())
{
}

// protected static (factories)
// ----------------------------------------------------------------------------

electrum_v1 electrum_v1::from_entropy(const data_chunk& entropy,
    language identifier)
{
    if (!is_valid_entropy_size(entropy.size()))
        return {};

    if (!dictionaries_.exists(identifier))
        return {};

    // Save entropy and derived words.
    return { entropy, encoder(entropy, identifier), identifier };
}

electrum_v1 electrum_v1::from_words(const string_list& words,
    language identifier)
{
    if (!is_valid_word_count(words.size()))
        return {};

    // Normalize to improve chance of dictionary matching.
    const auto tokens = try_normalize(words);
    const auto lexicon = contained_by(tokens, identifier);

    if (lexicon == language::none)
        return {};

    if (identifier != language::none && lexicon != identifier)
        return {};

    // Save derived entropy and dictionary words, originals are discarded.
    return { decoder(tokens, lexicon), tokens, lexicon };
}

// overflow methods
// ----------------------------------------------------------------------------
// Manage overflow bug and conversion to idiosyncratic textual seed data.

// public
bool electrum_v1::overflow() const
{
    return std::any_of(overflows_.begin(), overflows_.end(), [](bool value)
    {
        return value;
    });
}

const v1_decoding::overflow& electrum_v1::overflows() const
{
    return overflows_;
}

data_chunk electrum_v1::seed_entropy() const
{
    return v1_decoding(entropy_, overflows_).seed_entropy();
}

// public methods
// ----------------------------------------------------------------------------

ec_private electrum_v1::to_seed(const context& context) const
{
    if (!(*this))
        return {};

    // Set public key compression to false as is the electrum convention.
    // Causes derived payment addresses to use the uncompressed public key.
    static const auto compression = false;

    // Strech entropy into the seed, which is also the master private key.
    const auto seed = strecher(seed_entropy());

    // Context sets the version byte for derived payment addresses.
    // The private key will be invalid if the value does not ec verify.
    return { ec_scalar(seed), context.address_version, compression };
}

ec_public electrum_v1::to_public_key(const context& context) const
{
    if (!(*this))
        return {};

    // The public key will be invalid if the private key is invalid.
    return to_seed(context).to_public();
}

// operators
// ----------------------------------------------------------------------------

electrum_v1& electrum_v1::operator=(electrum_v1 other)
{
    swap(*this, other);
    return *this;
}

// friend function, see: stackoverflow.com/a/5695855/1172329
void swap(electrum_v1& left, electrum_v1& right)
{
    using std::swap;
    using namespace bc::system::words;

    // Must be unqualified (no std namespace).
    swap(static_cast<languages&>(left), static_cast<languages&>(right));
    swap(left.overflows_, right.overflows_);
}

} // namespace wallet
} // namespace system
} // namespace libbitcoin
