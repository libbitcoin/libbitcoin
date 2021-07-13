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
#ifndef LIBBITCOIN_SYSTEM_TEST_ELECTRUM_HPP
#define LIBBITCOIN_SYSTEM_TEST_ELECTRUM_HPP

#include <algorithm>
#include <cstddef>
#include <string>
#include <array>
#include <vector>
#include <bitcoin/system.hpp>

// Avoid using namespace in shared headers, but okay here.
using namespace bc::system;
using namespace bc::system::wallet;

namespace test {
namespace mnemonics_electrum {

struct electrum_vector
{
    typedef enum name
    {
        english,
        english_with_passphrase,
        japanese,
        japanese_with_passphrase,
        chinese,
        chinese_with_passphrase,
        spanish,
        spanish_with_passphrase,
        spanish2,
        spanish3
    } name;

    name index;
    language lingo;
    std::string mnemonic;
    data_chunk mnemonic_chunk;
    electrum::seed_prefix prefix;
    data_chunk entropy;
    std::string passphrase;
    data_chunk passphrase_chunk;
    long_hash seed;

    // Standardize sentence delimiters (for output comparison only).
    std::string sentence(language lingo) const
    {
        const auto is_japanese = (lingo == language::ja);
        const auto space = is_japanese ? ideographic_space : ascii_space;
        return join(split(mnemonic), space);
    }

    hd_private to_hd() const
    {
        const auto part = system::split(seed);
        return { part.first, part.second, btc_mainnet_p2kh.hd_prefixes() };
    }
};

const std::array<electrum_vector::name, 5> ascii_passphrase_vector_names
{
    {
        electrum_vector::english,
        electrum_vector::english_with_passphrase,
        electrum_vector::japanese,
        electrum_vector::chinese,
        electrum_vector::spanish
    }
};

const std::array<electrum_vector::name, 10> all_vector_names
{
    {
        electrum_vector::english,
        electrum_vector::english_with_passphrase,
        electrum_vector::japanese,
        electrum_vector::japanese_with_passphrase,
        electrum_vector::chinese,
        electrum_vector::chinese_with_passphrase,
        electrum_vector::spanish,
        electrum_vector::spanish_with_passphrase,
        electrum_vector::spanish2,
        electrum_vector::spanish3
    }
};

// Reduce verbosity using these.
typedef std::vector<electrum_vector> electrum_vectors;
using name = electrum_vector::name;
using prefix = electrum::seed_prefix;
static const data_chunk no_passphrase{};
static const std::string empty_passphrase{};

static ptrdiff_t abnormals(const electrum_vectors& vectors,
    const std::string& delimiter)
{
    return std::count_if(vectors.begin(), vectors.end(),
        [&](const electrum_vector& test)
    {
        // Must remove the ideographic_space before nfkd normalization.
        auto copy = join(split(test.mnemonic, delimiter));
        copy = ascii_to_lower(copy);
        to_compatibility_decomposition(copy);
        to_lower(copy);
        return test.mnemonic != join(split(copy), delimiter);
    });
}

// All mnemonics here are 12 words.
// Japanese mnemonics are ascii space delimited (not ideographic space).
// Electrum tests cases show 20 bytes of entropy where there is only 17 usable.
// This may be an intial seed for grinding over the prng.

// TODO: add vertical vectors
// github.com/spesmilo/electrum/blob/master/electrum/tests/test_wallet_vertical.py
// github.com/spesmilo/electrum/blob/master/electrum/tests/test_mnemonic.py

const electrum_vectors vectors
{
    electrum_vector
    {
        name::english,
        language::en,
        "wild father tree among universe such mobile favorite target dynamic credit identify",
        base16_chunk("77696c6420666174686572207472656520616d6f6e6720756e6976657273652073756368206d6f62696c65206661766f72697465207461726765742064796e616d696320637265646974206964656e74696679"),
        prefix::witness,
        base16_chunk("fb0a779f83feddb0e39aa0dde898cc3840"),
        //// base16_chunk(""),
        empty_passphrase,
        no_passphrase,
        base16_array("aac2a6302e48577ab4b46f23dbae0774e2e62c796f797d0a1b5faeb528301e3064342dafb79069e7c4c6b8c38ae11d7a973bec0d4f70626f8cc5184a8d0b0756")
    },
    electrum_vector
    {
        name::english_with_passphrase,
        language::en,
        "wild father tree among universe such mobile favorite target dynamic credit identify",
        base16_chunk("77696c6420666174686572207472656520616d6f6e6720756e6976657273652073756368206d6f62696c65206661766f72697465207461726765742064796e616d696320637265646974206964656e74696679"),
        prefix::witness,
        base16_chunk("fb0a779f83feddb0e39aa0dde898cc3840"),
        //// base16_chunk(""),
        "Did you ever hear the tragedy of Darth Plagueis the Wise?",
        base16_chunk("44696420796f7520657665722068656172207468652074726167656479206f6620446172746820506c6167756569732074686520576973653f"),
        base16_array("4aa29f2aeb0127efb55138ab9e7be83b36750358751906f86c662b21a1ea1370f949e6d1a12fa56d3d93cadda93038c76ac8118597364e46f5156fde6183c82f")
    },
    electrum_vector
    {
        name::japanese,
        language::ja,
        "なのか ひろい しなん まなぶ つぶす さがす おしゃれ かわく おいかける けさき かいとう さたん",
        base16_chunk("e381aae381aee3818b20e381b2e3828de3818420e38197e381aae3829320e381bee381aae381b5e3829920e381a4e381b5e38299e3819920e38195e3818be38299e3819920e3818ae38197e38283e3828c20e3818be3828fe3818f20e3818ae38184e3818be38191e3828b20e38191e38195e3818d20e3818be38184e381a8e3818620e38195e3819fe38293"),
        prefix::standard,
        base16_chunk("adba0594f0b9acb348d16d1e6858a3ad90"),
        //// base16_chunk("1938439226660562861250521787963972783469"),
        empty_passphrase,
        no_passphrase,
        base16_array("d3eaf0e44ddae3a5769cb08a26918e8b308258bcb057bb704c6f69713245c0b35cb92c03df9c9ece5eff826091b4e74041e010b701d44d610976ce8bfb66a8ad")
    },
    electrum_vector
    {
        name::japanese_with_passphrase,
        language::ja,
        "なのか ひろい しなん まなぶ つぶす さがす おしゃれ かわく おいかける けさき かいとう さたん",
        base16_chunk("e381aae381aee3818b20e381b2e3828de3818420e38197e381aae3829320e381bee381aae381b5e3829920e381a4e381b5e38299e3819920e38195e3818be38299e3819920e3818ae38197e38283e3828c20e3818be3828fe3818f20e3818ae38184e3818be38191e3828b20e38191e38195e3818d20e3818be38184e381a8e3818620e38195e3819fe38293"),
        prefix::standard,
        base16_chunk("adba0594f0b9acb348d16d1e6858a3ad90"),
        //// base16_chunk("1938439226660562861250521787963972783469"),
        "₿ 😀 😈     う けたま わる w͢͢͝h͡o͢͡ ̸͢k̵͟n̴͘ǫw̸̛s͘ ̀́w͘͢ḩ̵a҉̡͢t ̧̕h́o̵r͏̵rors̡ ̶͡͠lį̶e͟͟ ̶͝in͢ ͏t̕h̷̡͟e ͟͟d̛a͜r̕͡k̢̨ ͡h̴e͏a̷̢̡rt́͏ ̴̷͠ò̵̶f̸ u̧͘ní̛͜c͢͏o̷͏d̸͢e̡͝?͞",
        base16_chunk("e282bf20f09f988020f09f98882020202020e3818620e38191e3819fe381be20e3828fe3828b2077cda2cda2cd9d68cda16fcda2cda120ccb8cda26bccb5cd9f6eccb4cd98c7ab77ccb8cc9b73cd9820cc80cc8177cd98cda2e1b8a9ccb561d289cca1cda27420cca7cc9568cc816fccb572cd8fccb5726f7273cca120ccb6cda1cda06cc4afccb665cd9fcd9f20ccb6cd9d696ecda220cd8f74cc9568ccb7cca1cd9f6520cd9fcd9f64cc9b61cd9c72cc95cda16bcca2cca820cda168ccb465cd8f61ccb7cca2cca17274cc81cd8f20ccb4ccb7cda0c3b2ccb5ccb666ccb82075cca7cd986ec3adcc9bcd9c63cda2cd8f6fccb7cd8f64ccb8cda265cca1cd9d3fcd9e"),
        base16_array("251ee6b45b38ba0849e8f40794540f7e2c6d9d604c31d68d3ac50c034f8b64e4bc037c5e1e985a2fed8aad23560e690b03b120daf2e84dceb1d7857dda042457")
    },
    electrum_vector
    {
        name::chinese,
        language::zh_Hans,
        "眼 悲 叛 改 节 跃 衡 响 疆 股 遂 冬",
        base16_chunk("e79cbc20e682b220e58f9b20e694b920e88a8220e8b78320e8a1a120e5938d20e7968620e882a120e9818220e586ac"),
        prefix::witness,
        base16_chunk("39778b3f92e2bd695d31a0ced46be44870"),
        //// base16_chunk("3083737086352778425940060465574397809099"),
        empty_passphrase,
        no_passphrase,
        base16_array("0b9077db7b5a50dbb6f61821e2d35e255068a5847e221138048a20e12d80b673ce306b6fe7ac174ebc6751e11b7037be6ee9f17db8040bb44f8466d519ce2abf")
    },
    electrum_vector
    {
        name::chinese_with_passphrase,
        language::zh_Hans,
        "眼 悲 叛 改 节 跃 衡 响 疆 股 遂 冬",
        base16_chunk("e79cbc20e682b220e58f9b20e694b920e88a8220e8b78320e8a1a120e5938d20e7968620e882a120e9818220e586ac"),
        prefix::witness,
        base16_chunk("39778b3f92e2bd695d31a0ced46be44870"),
        //// base16_chunk("3083737086352778425940060465574397809099"),
        "给我一些测试向量谷歌",
        base16_chunk("e7bb99e68891e4b880e4ba9be6b58be8af95e59091e9878fe8b0b7e6ad8c"),
        base16_array("6c03dd0615cf59963620c0af6840b52e867468cc64f20a1f4c8155705738e87b8edb0fc8a6cee4085776cb3a629ff88bb1a38f37085efdbf11ce9ec5a7fa5f71")
    },
    electrum_vector
    {
        name::spanish,
        language::es,
        "almíbar tibio superar vencer hacha peatón príncipe matar consejo polen vehículo odisea",
        base16_chunk("616c6d69cc8162617220746962696f20737570657261722076656e63657220686163686120706561746fcc816e20707269cc816e63697065206d6174617220636f6e73656a6f20706f6c656e2076656869cc8163756c6f206f6469736561"),
        prefix::standard,
        base16_chunk("09fcd376fba69d5b6ebc5437f6f7d9d070"),
        //// base16_chunk("3423992296655289706780599506247192518735"),
        empty_passphrase,
        no_passphrase,
        base16_array("18bffd573a960cc775bbd80ed60b7dc00bc8796a186edebe7fc7cf1f316da0fe937852a969c5c79ded8255cdf54409537a16339fbe33fb9161af793ea47faa7a")
    },
    electrum_vector
    {
        name::spanish_with_passphrase,
        language::es,
        "almíbar tibio superar vencer hacha peatón príncipe matar consejo polen vehículo odisea",
        base16_chunk("616c6d69cc8162617220746962696f20737570657261722076656e63657220686163686120706561746fcc816e20707269cc816e63697065206d6174617220636f6e73656a6f20706f6c656e2076656869cc8163756c6f206f6469736561"),
        prefix::standard,
        base16_chunk("09fcd376fba69d5b6ebc5437f6f7d9d070"),
        //// base16_chunk("3423992296655289706780599506247192518735"),
        "araña difícil solución término cárcel",
        base16_chunk("6172616ecc83612064696669cc8163696c20736f6c7563696fcc816e207465cc81726d696e6f206361cc817263656c"),
        base16_array("363dec0e575b887cfccebee4c84fca5a3a6bed9d0e099c061fa6b85020b031f8fe3636d9af187bf432d451273c625e20f24f651ada41aae2c4ea62d87e9fa44c")
    },
    electrum_vector
    {
        name::spanish2,
        language::es,
        "equipo fiar auge langosta hacha calor trance cubrir carro pulmón oro áspero",
        base16_chunk("65717569706f20666961722061756765206c616e676f7374612068616368612063616c6f72207472616e63652063756272697220636172726f2070756c6d6fcc816e206f726f2061cc81737065726f"),
        prefix::witness,
        base16_chunk("4eab005dbd669c503b59df2bd7be988a80"),
        // one char short in original, padded left.
        //// base16_chunk("0448346710104003081119421156750490206837"),
        empty_passphrase,
        no_passphrase,
        base16_array("001ebce6bfde5851f28a0d44aae5ae0c762b600daf3b33fc8fc630aee0d207646b6f98b18e17dfe3be0a5efe2753c7cdad95860adbbb62cecad4dedb88e02a64")
    },
    electrum_vector
    {
        name::spanish3,
        language::es,
        "vidrio jabón muestra pájaro capucha eludir feliz rotar fogata pez rezar oír",
        base16_chunk("76696472696f206a61626fcc816e206d756573747261207061cc816a61726f206361707563686120656c756469722066656c697a20726f74617220666f676174612070657a2072657a6172206f69cc8172"),
        prefix::witness,
        base16_chunk("fa0ea254d472aa9655be6e5bf64f25d0f0"),
        //// base16_chunk("3444792611339130545499611089352232093648"),
        "¡Viva España! repiten veinte pueblos y al hablar dan fe del ánimo español... ¡Marquen arado martillo y clarín",
        base16_chunk("c2a1566976612045737061c3b16121207265706974656e207665696e746520707565626c6f73207920616c206861626c61722064616e2066652064656c20c3a16e696d6f2065737061c3b16f6c2e2e2e20c2a14d61727175656e20617261646f206d617274696c6c6f207920636c6172c3ad6e"),
        base16_array("c274665e5453c72f82b8444e293e048d700c59bf000cacfba597629d202dcf3aab1cf9c00ba8d3456b7943428541fed714d01d8a0a4028fc3a9bb33d981cb49f")
    }
};

// ambiguous between fr, en and v1 en
const string_list ambiguous_en_fr_v1
{
    "fragile", "fragile", "fragile", "fragile", "fragile", "fragile",
    "fragile", "fragile", "fragile", "fragile", "fragile", "fragile"
};

// "cry" not fr
const string_list ambiguous_en_v1
{
    "cry", "fragile", "fragile", "fragile", "fragile", "fragile",
    "fragile", "fragile", "fragile", "fragile", "fragile", "fragile"
};

// "fiasco" not en
const string_list ambiguous_fr_pt_v1
{
    "fiasco", "fiasco", "fiasco", "fiasco", "fiasco", "fiasco",
    "fiasco", "fiasco", "fiasco", "fiasco", "fiasco", "fiasco"
};

// "cycle" not v1
const string_list ambiguous_en_fr
{
    "cycle", "fragile", "fragile", "fragile", "fragile", "fragile",
    "fragile", "fragile", "fragile", "fragile", "fragile", "fragile"
};

// "difference" only v1
const string_list distinct_v1
{
    "difference", "fragile", "fragile", "fragile", "fragile", "fragile",
    "fragile", "fragile", "fragile", "fragile", "fragile", "fragile"
};

// "differ" only en
const string_list distinct_en
{
    "differ", "fragile", "fragile", "fragile", "fragile", "fragile",
    "fragile", "fragile", "fragile", "fragile", "fragile", "fragile"
};

// "différer" only fr
const string_list distinct_fr
{
    "différer", "fragile", "fragile", "fragile", "fragile", "fragile",
    "fragile", "fragile", "fragile", "fragile", "fragile", "fragile"
};

// valid
const string_list redundant_hans_hant
{
    "的", "一", "是", "在", "不", "的", "一", "是", "在", "不", "的", "一", "是"
};

// invalid
const string_list mixed_words
{
    "below", "가격", "あいこくしん", "abaisser", "abaco", "ábaco",
    "abdikace", "abandon", "abacate", "的", "歇", "above"
};

// invalid
const string_list similar_words
{
    "ábaco", "abaco", "ábaco", "abaco", "ábaco", "abaco",
    "ábaco", "abaco", "ábaco", "abaco", "ábaco", "abaco",
};

// The electrum test vectors do not include 2fa/2faw prefixes, so these are added.
// Menomics generated using seed prefixer, with following parameters.

// 17 null bytes, seed_prefix:standard, language::it, 340 iterations.
const auto mnemonic_standard = "amarena viola sciarpa movimento trabocco cosmico montato dogma ossa tara muffa emozione";

// 17 null bytes, seed_prefix:witness, language::it, 9544 iterations.
const auto mnemonic_witness = "mettere enzima ristoro revocato sobrio tizzone slitta croce crostata scatenare cardo tortora";

// 17 null bytes, seed_prefix:two_factor_authentication, language::it, 8813 iterations.
const auto mnemonic_two_factor_authentication = "orfano verbale vessillo sabato furbo dito gallina asino delegare chiedere alettone ulisse";

// 17 null bytes, seed_prefix:two_factor_authentication_witness, language::it, 331 iterations.
const auto mnemonic_two_factor_authentication_witness = "appetito brindare sussurro leva femmina connesso nucleo freccetta leggero tariffa virologo roccia";

// The electrum test vectors do not include 2fa negative test vectors, so these are added.
// Menomics generated using seed prefixer, with 2fa hack disabled, with following parameters.

// 16 null bytes, seed_prefix:two_factor_authentication, language::pt, 734 iterations.
const auto mnemonic_two_factor_authentication11 = "pneu jararaca olhar labareda extrato engenho descanso maroto coquetel roxo mestre";

// 17 null bytes, seed_prefix:two_factor_authentication, language::pt, 3899 iterations.
const auto mnemonic_two_factor_authentication12 = "ciente demanda absoluto aliar javali flacidez animar sambar bajular publicar educado golfe";

// 18 null bytes, seed_prefix:two_factor_authentication, language::pt, 4492 iterations.
const auto mnemonic_two_factor_authentication13 = "excesso hidratar emergir ciente falar touca vogal soletrar gaiola prezar budismo torque deitado";

// 27 null bytes, seed_prefix:two_factor_authentication, language::pt, 4658 iterations.
const auto mnemonic_two_factor_authentication19 = "trevo oficina sacudir alface tonel adorar vagaroso joia navalha carreira abduzir trevo insulto bloco costela eventual querido chutar amador";

// 28 null bytes, seed_prefix:two_factor_authentication, language::pt, 1867 iterations.
const auto mnemonic_two_factor_authentication20 = "geleia grafite raiva disfarce liberal figurado orgulho piranha mosquete infrator xarope cigana esquerda jogador queda oriundo unha melhor marquise robalo";

// 29 null bytes, seed_prefix:two_factor_authentication, language::pt, 9901 iterations.
const auto mnemonic_two_factor_authentication21 = "criminal sentado herdar joaninha iate lebre aderente arquivo confuso pedra bilhete culatra rajada enraizar complexo grudado avaria pneu fosco raridade voleibol";

// Test wrapper for access to protected methods.
class accessor
  : public electrum
{
public:
    accessor(const data_chunk& entropy, const string_list& words,
        language identifier, seed_prefix prefix)
      : electrum(entropy, words, identifier, prefix)
    {
    }

    static size_t entropy_bits(const data_slice& entropy)
    {
        return electrum::entropy_bits(entropy);
    }
    
    static size_t entropy_bits(const string_list& words)
    {
        return electrum::entropy_bits(words);
    }

    static size_t entropy_size(size_t bit_strength)
    {
        return electrum::entropy_size(bit_strength);
    }

    static size_t entropy_size(const string_list& words)
    {
        return electrum::entropy_size(words);
    }
    
    static size_t word_count(const data_slice& entropy)
    {
        return electrum::word_count(entropy);
    }

    static size_t word_count(size_t bit_strength)
    {
        return electrum::word_count(bit_strength);
    }

    static uint8_t unused_bits(const data_slice& entropy)
    {
        return electrum::unused_bits(entropy);
    }

    static uint8_t unused_bytes(const data_slice& entropy)
    {
        return electrum::unused_bytes(entropy);
    }

    static size_t usable_size(const data_slice& entropy)
    {
        return electrum::usable_size(entropy);
    }

    static bool is_conflict(const string_list& words)
    {
        return electrum::is_conflict(words);
    }

    static seed_prefix to_conflict(const string_list& words)
    {
        return electrum::to_conflict(words);
    }

    static seed_prefix normalized_to_prefix(const string_list& words)
    {
        return electrum::normalized_to_prefix(words);
    }
    static bool normalized_is_prefix(const string_list& words,
        seed_prefix prefix)
    {
        return electrum::normalized_is_prefix(words, prefix);
    }

    static bool is_ambiguous(size_t count, seed_prefix prefix)
    {
        return electrum::is_ambiguous(count, prefix);
    }

    static bool is_ambiguous(const string_list& words, language requested,
        language derived)
    {
        return electrum::is_ambiguous(words, requested, derived);
    }

    static bool is_seedable(seed_prefix prefix)
    {
        return electrum::is_seedable(prefix);
    }

    static std::string to_version(seed_prefix prefix)
    {
        return electrum::to_version(prefix);
    }

    static string_list encoder(const data_chunk& entropy, language identifier)
    {
        return electrum::encoder(entropy, identifier);
    }

    static data_chunk decoder(const string_list& words, language identifier)
    {
        return electrum::decoder(words, identifier);
    }

    static long_hash seeder(const string_list& words,
        const std::string& passphrase)
    {
        return electrum::seeder(words, passphrase);
    }

    static grinding grinder(const data_chunk& entropy, seed_prefix prefix,
        language identifier, size_t limit)
    {
        return electrum::grinder(entropy, prefix, identifier, limit);
    }

    static bool validator(const string_list& words, seed_prefix prefix)
    {
        return electrum::validator(words, prefix);
    }

    static electrum from_words(const string_list& words, language identifier)
    {
        return electrum::from_words(words, identifier);
    }

    static electrum from_entropy(const data_chunk& entropy, seed_prefix prefix,
        language identifier, uint32_t grind_limit=0)
    {
        return electrum::from_entropy(entropy, prefix, identifier, grind_limit);
    }
};

} // mnemonics_electrum
} // test

#endif