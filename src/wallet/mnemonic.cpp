/*
 * Copyright (c) 2011-2015 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <bitcoin/bitcoin/wallet/mnemonic.hpp>

#include <algorithm>
#include <cstdint>
#include <boost/locale.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/utility/assert.hpp>
#include <bitcoin/bitcoin/utility/binary.hpp>
#include <bitcoin/bitcoin/utility/collection.hpp>
#include <bitcoin/bitcoin/utility/string.hpp>
#include "../math/external/pkcs5_pbkdf2.h"

namespace libbitcoin {

// BIP-39 private constants.
constexpr size_t bits_per_word = 11;
constexpr size_t entropy_bit_divisor = 32;
constexpr size_t hmac_iterations = 2048;

static uint8_t bip39_shift(size_t bit)
{
    return (1 << (byte_bits - (bit % byte_bits) - 1));
}

static std::string normalize_nfkd(const std::string& value)
{
    using namespace boost::locale;
    const generator locale;
    return normalize(value, norm_type::norm_nfkd, locale("UTF-8"));
}

bool validate_mnemonic(const string_list& words, const wordlist& dictionary)
{
    const auto word_count = words.size();
    if ((word_count % mnemonic_word_multiple) != 0)
        return false;

    const auto total_bits = bits_per_word * word_count;
    const auto check_bits = total_bits / (entropy_bit_divisor + 1);
    const auto entropy_bits = total_bits - check_bits;

    BITCOIN_ASSERT((entropy_bits % byte_bits) == 0);

    size_t bit = 0;
    data_chunk data((total_bits + byte_bits - 1) / byte_bits, 0);

    for (const auto& word: words)
    {
        const auto position = find_position(dictionary, word);
        if (position == -1)
            return false;

        for (size_t loop = 0; loop < bits_per_word; loop++, bit++)
        {
            if (position & (1 << (bits_per_word - loop - 1)))
            {
                const auto byte = bit / byte_bits;
                data[byte] |= bip39_shift(bit);
            }
        }
    }

    data.resize(entropy_bits / byte_bits);

    const auto mnemonic = create_mnemonic(data, dictionary);
    return std::equal(mnemonic.begin(), mnemonic.end(), words.begin());
}

string_list create_mnemonic(data_slice entropy, const wordlist &dictionary)
{
    if ((entropy.size() % mnemonic_seed_multiple) != 0)
        return string_list();

    const size_t entropy_bits = (entropy.size() * byte_bits);
    const size_t check_bits = (entropy_bits / entropy_bit_divisor);
    const size_t total_bits = (entropy_bits + check_bits);
    const size_t word_count = (total_bits / bits_per_word);

    BITCOIN_ASSERT((total_bits % bits_per_word) == 0);
    BITCOIN_ASSERT((word_count % mnemonic_word_multiple) == 0);

    const auto data = build_data({entropy, sha256_hash(entropy)});

    size_t bit = 0;
    string_list words;

    for (size_t word = 0; word < word_count; word++)
    {
        size_t position = 0;
        for (size_t loop = 0; loop < bits_per_word; loop++)
        {
            bit = (word * bits_per_word + loop);
            position <<= 1;

            const auto byte = bit / byte_bits;

            if ((data[byte] & bip39_shift(bit)) > 0)
                position++;
        }

        BITCOIN_ASSERT(position < wordlist_size);
        words.push_back(dictionary[position]);
    }

    BITCOIN_ASSERT(words.size() == ((bit + 1) / bits_per_word));
    return words;
}

bool validate_mnemonic(const string_list& mnemonic,
    const wordlist_list& wordlists)
{
    for (auto* dictionary: wordlists)
    {
        if (validate_mnemonic(mnemonic, *dictionary))
            return true;
    }
    return false;
}

long_hash decode_mnemonic(const string_list& mnemonic,
    const std::string& passphrase)
{
    const auto sentence = join(mnemonic);
    const auto salt = normalize_nfkd("mnemonic" + passphrase);

    return pkcs5_pbkdf2_hmac_sha512(
        to_data_chunk(sentence), to_data_chunk(salt), hmac_iterations);
}

} // namespace libbitcoin

