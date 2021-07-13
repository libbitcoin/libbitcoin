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
#ifndef LIBBITCOIN_SYSTEM_TEST_ELECTRUM_V1_HPP
#define LIBBITCOIN_SYSTEM_TEST_ELECTRUM_V1_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <bitcoin/system.hpp>

 // Avoid using namespace in shared headers, but okay here.
using namespace bc::system;
using namespace bc::system::wallet;

namespace test {
namespace mnemonics_electrum_v1 {

using flags = v1_decoding::overflow;
static const auto multiple = electrum_v1::word_multiple;

struct electrum_v1_vector
{
    language lingo;
    context network;
    std::string mnemonic;
    data_chunk entropy;
    ec_uncompressed key;

    string_list words() const
    {
        return split(mnemonic);
    }

    // Overflow bug testing.

    data_chunk overflowed_seed_entropy_;
    flags overflow_flags_;
    std::string trimmed_mnemonic_;

    bool overflow() const
    {
        return !overflow_flags_.empty();
    }

    flags overflows() const
    {
        return overflow() ? overflow_flags_ : flags(words().size() / multiple);
    }

    std::string trimmed_mnemonic() const
    {
        return overflow() ? trimmed_mnemonic_ : mnemonic;
    }

    string_list trimmed_words() const
    {
        return split(trimmed_mnemonic());
    }

    data_chunk seed_entropy() const
    {
        return to_chunk(encode_base16(entropy));
    }

    data_chunk overflowed_seed_entropy() const
    {
        return overflow() ? overflowed_seed_entropy_ : seed_entropy();
    }
};

typedef std::vector<electrum_v1_vector> electrum_v1_vectors;

// Vectors verified using: electrum/old_mnemonic.py
electrum_v1_vectors vectors
{
    electrum_v1_vector
    {
        // github.com/spesmilo/electrum/issues/3149
        language::en,
        btc_mainnet_p2kh,
        "hurry idiot prefer sunset mention mist jaw inhale impossible kingdom rare squeeze",
        // [025d2f2d][00503691][003ca789][00ca155c]
        base16_chunk("025d2f2d00503691003ca78900ca155c"),
        base16_array("04""0d6d957236f4e64467cbc4d3e835955eaea1c6f20232d549d0ba4475f4adb30509f5b04df03b3827c344734185fee0b3e7a978d8f40b2a25e35248c19be79799"),
        // [025d2f2d][00503691][(1)003ca789][00ca155c]
        to_chunk("025d2f2d005036911003ca78900ca155c"),
        flags{ false, false, true, false },
        "hurry idiot prefer sunset mention mist ship baby bright kingdom rare squeeze",
    },
    electrum_v1_vector
    {
        // electrum/tests/test_wallet_vertical.py
        language::en,
        btc_mainnet_p2kh,
        "alone body father children lead goodbye phone twist exist grass kick join",
        // [14039a74][00162d9d][0cc3c31f][00168ddc]
        base16_chunk("14039a7400162d9d0cc3c31f00168ddc"),
        base16_array("04""cd805ed20aec61c7a8b409c121c6ba60a9221f46d20edbc2be83ebd91460e97937cd7d782e77c1cb08364c6bc1c98bc040fdad53f22f29f7d3a85c8e51f9c875"),
        // [14039a74][(1)00162d9d][0cc3c31f][(1)00168ddc]
        to_chunk("14039a74100162d9d0cc3c31f100168ddc"),
        flags{ false, true, false, true },
        "alone body father alas forgot forgot phone twist exist ask unable unable"
    },
    electrum_v1_vector
    {
        // electrum/tests/test_wallet_vertical.py
        language::en,
        btc_mainnet_p2kh,
        "powerful random nobody notice nothing important anyway look away hidden message over",
        base16_chunk("acb740e454c3134901d7c8f16497cc1c"),
        base16_array("04""e9d4b7866dd1e91c862aebf62a49548c7dbf7bcc6e4b7b8c9da820c7737968df9c09d5a3e271dc814a29981f81b3faaf2737b551ef5dcc6189cf0f8252c442b3")
    },
    electrum_v1_vector
    {
        // Minium value and length entropy.
        language::en,
        btc_mainnet_p2kh,
        "like like like like like like like like like like like like",
        base16_chunk("00000000000000000000000000000000"),
        base16_array("04""4e13b0f311a55b8a5db9a32e959da9f011b131019d4cebe6141b9e2c93edcbfc0954c358b062a9f94111548e50bde5847a3096b8b7872dcffadb0e9579b9017b")
    },
    electrum_v1_vector
    {
        // Minium value and maximum length entropy.
        language::en,
        btc_testnet_p2kh,
        "like like like like like like like like like like like like like like like like like like like like like like like like",
        base16_chunk("0000000000000000000000000000000000000000000000000000000000000000"),
        base16_array("04""60cd6d0d5d7f32b92c9ee7b37ff37c2327eaf22a6df66613b6f11fca22145465fd8a576a9ba858dd2426dd149714f0d0ad66cecd186e810b6148bf9f84d34a15")
    },
    electrum_v1_vector
    {
        // Maximum value for minimum length entropy.
        language::en,
        btc_mainnet_p2kh,
        "fail husband howl fail husband howl fail husband howl fail husband howl",
        base16_chunk("ffffffffffffffffffffffffffffffff"),
        base16_array("04""102fb8009e8dc9af403831e50a000511cbd83cc550355fe8fbc5130b328e1de949c357031324e08985e51a66399e5215d863129f7e17d913cdbebabd46815624")
    },
    electrum_v1_vector
    {
        // Maximum value and maximum length entropy.
        language::en,
        btc_testnet_p2kh,
        "fail husband howl fail husband howl fail husband howl fail husband howl fail husband howl fail husband howl fail husband howl fail husband howl",
        base16_chunk("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"),
        base16_array("04""d4a9b63ffc2b5c30f241aac955f9d1b1900d833c283c2a690cde7b86817e02b460ce6a8c66098692df094d89c9f4500a818135ee789f53c94050d2a552b492a4")
    },
    electrum_v1_vector
    {
        // Same words will never overflow (no distance).
        language::en,
        btc_mainnet_p2kh,
        "foe foe foe foe foe foe foe foe foe foe foe foe",
        base16_chunk("00000630000006300000063000000630"),
        base16_array("04""74b16e8c05d303df1f7b77e3308415fbc799286d11d84a4d5fab1a5a9bec04d084dd3f48e5952eedc6583fa46a389e45101ccb3bb4ad541fe7e3d29b658241b4")
    },
    electrum_v1_vector
    {
        // Double length of previous.
        language::en,
        btc_testnet_p2kh,
        "foe foe foe foe foe foe foe foe foe foe foe foe foe foe foe foe foe foe foe foe foe foe foe foe",
        base16_chunk("0000063000000630000006300000063000000630000006300000063000000630"),
        base16_array("04""0a5a88363d5ea336f02e74b9013daa0ffaa9981313aa35ee3dceeb0f0b8bee4e1a08377c4175127a33ccc30e652db27c000abd204583a431c98ceda5e38c144a")
    },
    electrum_v1_vector
    {
        // Half of previous, switch to pt.
        language::pt,
        btc_mainnet_p2kh,
        "vomito vomito vomito vomito vomito vomito vomito vomito vomito vomito vomito vomito",
        base16_chunk("00000630000006300000063000000630"),
        base16_array("04""74b16e8c05d303df1f7b77e3308415fbc799286d11d84a4d5fab1a5a9bec04d084dd3f48e5952eedc6583fa46a389e45101ccb3bb4ad541fe7e3d29b658241b4")
    },
    electrum_v1_vector
    {
        // Double length of previous.
        language::pt,
        btc_testnet_p2kh,
        "vomito vomito vomito vomito vomito vomito vomito vomito vomito vomito vomito vomito vomito vomito vomito vomito vomito vomito vomito vomito vomito vomito vomito vomito",
        base16_chunk("0000063000000630000006300000063000000630000006300000063000000630"),
        base16_array("04""0a5a88363d5ea336f02e74b9013daa0ffaa9981313aa35ee3dceeb0f0b8bee4e1a08377c4175127a33ccc30e652db27c000abd204583a431c98ceda5e38c144a")
    },
    electrum_v1_vector
    {
        language::en,
        btc_mainnet_p2kh,
        "blind faith blind faith blind faith blind faith blind faith blind faith",
        base16_chunk("c6e913bc397bef94c6e913bc397bef94"),
        base16_array("04""1ea1d1b78cdd8f7e7bd3dfd0098f19e62b308874ec2e8ebe68f9f6519a404760186168279849f8ec88a6e516e98a7ade141b2a7e08795474c77a0df0ec9f928f")
    },
    electrum_v1_vector
    {
        // Double length of previous.
        language::en,
        btc_testnet_p2kh,
        "blind faith blind faith blind faith blind faith blind faith blind faith blind faith blind faith blind faith blind faith blind faith blind faith",
        base16_chunk("c6e913bc397bef94c6e913bc397bef94c6e913bc397bef94c6e913bc397bef94"),
        base16_array("04""1b5349b64b1dcafe3d2137a5ee37e41f084a57b1b205f7ca8634d38ba5cc97a1254121136e5b9af099bfcedbd2911500571740b9354ef0aafce56cb6ec75af54")
    },
    electrum_v1_vector
    {
        language::pt,
        btc_mainnet_p2kh,
        "gear ansioso medusa bicuspide lauto ciatico jesus doping iceberg empuxo harpista feixe",
        base16_chunk("85bd8950a3534bf1367b98dbeafc4022"),
        base16_array("04""6c2a2307f4c58897f4a94afb94f3cf7bef4acce3acfee3e497f7f41c25d83afa80211364d04406fd19b18c7a554b2dd114fa598040d0657ad6fcc25b64710518")
    },
    electrum_v1_vector
    {
        // Double length of previous and switch to en.
        language::en,
        btc_testnet_p2kh,
        "bite start agree mother buy dear swallow carry knock parent gift offer bite start agree mother buy dear swallow carry knock parent gift offer",
        base16_chunk("85bd8950a3534bf1367b98dbeafc402285bd8950a3534bf1367b98dbeafc4022"),
        base16_array("04""da9b285c009ac42011ce635dafc96abb36fd221432bd7ba98660b586489e2f78e37f0cca8e107469ce516109600815abb8e65169c8b484b0dd86897aa6305eac")
    } 
};

// invalid for length
const string_list words2
{
    "blind", "faith"
};

// mixed valid languages
const string_list mixed_words12
{
    "abissinio", "faith", "blind", "faith", "blind", "faith",
    "blind", "faith", "blind", "faith", "blind", "faith"
};

// valid (non-overflow)
const string_list words12
{
    "blind", "faith", "blind", "faith", "blind", "faith",
    "blind", "faith", "blind", "faith", "blind", "faith"
};

// valid (non-overflow)
const string_list words24
{
    "blind", "faith", "blind", "faith", "blind", "faith",
    "blind", "faith", "blind", "faith", "blind", "faith",
    "blind", "faith", "blind", "faith", "blind", "faith",
    "blind", "faith", "blind", "faith", "blind", "faith"
};

// valid (two overflows)
const string_list two_overflows12
{
    "blind", "faith", "blind", "jaw", "inhale", "impossible",
    "blind", "faith", "blind", "jaw", "inhale", "impossible"
};

// invalid for length
const string_list words26
{
    "blind", "faith", "blind", "faith", "blind", "faith",
    "blind", "faith", "blind", "faith", "blind", "faith",
    "blind", "faith", "blind", "faith", "blind", "faith",
    "blind", "faith", "blind", "faith", "blind", "faith",
    "blind", "faith"
};

// Test wrapper for access to protected methods.
class accessor
  : public electrum_v1
{
public:
    accessor(const data_chunk& entropy, language identifier=language::en)
      : electrum_v1(entropy, identifier)
    {
    }

    accessor(const string_list& words, language identifier=language::none)
      : electrum_v1(words, identifier)
    {
    }

    accessor(const std::string& sentence, language identifier=language::none)
      : electrum_v1(sentence, identifier)
    {
    }

    accessor(const data_chunk& entropy, const string_list& words,
        language identifier)
      : electrum_v1(entropy, words, identifier)
    {
    }

    accessor(const v1_decoding& decoding, const string_list& words,
        language identifier)
      : electrum_v1(decoding, words, identifier)
    {
    }

    static size_t entropy_bits(const data_slice& entropy)
    {
        return electrum_v1::entropy_bits(entropy);
    }

    static size_t entropy_bits(const string_list& words)
    {
        return electrum_v1::entropy_bits(words);
    }

    static size_t entropy_size(const string_list& words)
    {
        return electrum_v1::entropy_size(words);
    }

    static size_t word_count(const data_slice& entropy)
    {
        return electrum_v1::word_count(entropy);
    }

    static v1_decoding decoder(const string_list& words, language identifier)
    {
        return electrum_v1::decoder(words, identifier);
    }

    static string_list encoder(const data_chunk& entropy, language identifier)
    {
        return electrum_v1::encoder(entropy, identifier);
    }

    static hash_digest strecher(const data_chunk& seed_entropy)
    {
        return electrum_v1::strecher(seed_entropy);
    }

    static electrum_v1 from_words(const string_list& words,
        language identifier)
    {
        return electrum_v1::from_words(words, identifier);
    }

    static electrum_v1 from_entropy(const data_chunk& entropy,
        language identifier)
    {
        return electrum_v1::from_entropy(entropy, identifier);
    }

    const v1_decoding::overflow& overflows() const
    {
        return electrum_v1::overflows();
    }

    data_chunk seed_entropy() const
    {
        return electrum_v1::seed_entropy();
    }
};

} // mnemonics_electrum_v1
} // test

#endif
