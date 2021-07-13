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
#ifndef LIBBITCOIN_SYSTEM_WALLET_CONEXT_HPP
#define LIBBITCOIN_SYSTEM_WALLET_CONEXT_HPP

#include <cstdint>
#include <string>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/wallet/keys/ec_private.hpp>
#include <bitcoin/system/wallet/keys/hd_private.hpp>

namespace libbitcoin {
namespace system {
namespace wallet {

/// Prefix namespace tree.
namespace prefix
{
    typedef struct hds
    {
        uint32_t xprv;
        uint32_t xpub;
    } hds;

    typedef struct versions
    {
        uint8_t add;
        uint8_t wif;
    } versions;

    /// Witness address prefixes.
    /// Provide authoritative URL for any values added here.
    namespace wit
    {
        // Can be compared during execution to validate a parameter.
        constexpr auto undefined = "";

        // github.com/bitcoin/bips/blob/master/bip-0173.mediawiki
        // github.com/litecoin-project/litecoin/blob/master/src/chainparams.cpp
        namespace main
        {
            constexpr auto btc = "bc";
            constexpr auto ltc = "ltc";
        }
        namespace test
        {
            constexpr auto btc = "tb";
            constexpr auto ltc = "tltc";
        }
    }

    /// WIF key prefix bytes.
    namespace wif
    {
        // Can be compared during execution to validate a parameter.
        // en.bitcoin.it/wiki/list_of_address_prefixes
        constexpr auto undefined = 0xff;

        namespace main
        {
            constexpr auto btc = 0x80;
        }
        namespace test
        {
            constexpr auto btc = 0xef;
        }
    }

    /// Payment addresss version bytes.
    namespace add
    {
        // Can be compared during execution to validate a parameter.
        // en.bitcoin.it/wiki/list_of_address_prefixes
        constexpr auto undefined = 0xff;

        namespace p2pkh
        {
            namespace main
            {
                constexpr auto btc = 0x00;
            }
            namespace test
            {
                constexpr auto btc = 0x6f;
            }
        }
        namespace p2sh
        {
            namespace main
            {
                constexpr auto btc = 0x05;
            }
            namespace test
            {
                constexpr auto btc = 0xc4;
            }
        }
    }

    /// Extended (hd) keys, data from Satoshi Labs registry.
    /// github.com/satoshilabs/slips/blob/master/slip-0132.md
    namespace ext
    {
        // Can be compared during execution to validate a parameter.
        constexpr auto undefined = hds{ 0xffffffff, 0xffffffff };

        namespace p2pkh
        {
            namespace main
            {
                // { xpub, xprv } m/44'/0'
                constexpr auto btc = hds{ 0x0488ade4, 0x0488b21e };
                // { Ltub, Ltpv } m/44'/2'
                constexpr auto ltc = hds{ 0x019d9cfe, 0x019da462 };
                // { vtcp, vtcv } m/44'/28'
                constexpr auto vtc = hds{ 0x0488ade4, 0x0488b21e };
                // { ppub, pprv } m/44'/1997'
                constexpr auto polis = hds{ 0x03e25945, 0x03e25d7e };
            }
            namespace test
            {
                // { tpub, tprv } m/44'/1'
                constexpr auto btc = hds{ 0x04358394, 0x043587cf };
                // { ttub, ttpv } m/44'/1'
                constexpr auto ltc = hds{ 0x0436ef7d, 0x0436f6e1 };
            }
        }
        namespace p2sh
        {
            namespace main
            {
                constexpr auto btc = p2pkh::main::btc;
                constexpr auto ltc = p2pkh::main::ltc;
                constexpr auto vtc = p2pkh::main::vtc;
            }
            namespace test
            {
                constexpr auto btc = p2pkh::test::btc;
            }
            namespace p2wpkh
            {
                namespace main
                {
                    // { ypub, yprv } m/49'/0'
                    constexpr auto btc = hds{ 0x049d7878, 0x049d7cb2 };
                    // { Mtub, Mtpv } m/49'/1'
                    constexpr auto ltc = hds{ 0x01b26792, 0x01b26ef6 };
                }
                namespace test
                {
                    // { upub, uprv } m/49'/1'
                    constexpr auto btc = hds{ 0x044a4e28, 0x044a5262 };
                }
            }
            namespace p2wsh
            {
                namespace multisig
                {
                    namespace main
                    {
                        // { Ypub, Yprv }
                        constexpr auto btc = hds{ 0x0295b005, 0x0295b43f };
                    }
                    namespace test
                    {
                        // { Upub, Uprv }
                        constexpr auto btc = hds{ 0x024285b5, 0x024289ef };
                    }
                }
            }
        }
        namespace p2wpkh
        {
            namespace main
            {
                // { zpub, zprv } m/84'/0'
                constexpr auto btc = hds{ 0x04b2430c, 0x04b24746 };
                // { zpub, zprv } m/84'/57'
                constexpr auto sys = btc;
            }
            namespace test
            {
                // { vpub, vprv } m/84'/1'
                constexpr auto btc = hds{ 0x045f18bc, 0x045f1cf6 };
            }
        }
        namespace p2wsh
        {
            namespace multisig
            {
                namespace main
                {
                    // { Zpub, Zprv }
                    constexpr auto btc = hds{ 0x02aa7a99, 0x02aa7ed3 };
                    constexpr auto sys = btc;
                }
                namespace test
                {
                    // { Vpub, Vprv }
                    constexpr auto btc = hds{ 0x02575048, 0x02575483 };
                }
            }
        }
    }
}

/// Portable context for keys and addresses.
struct BC_API context
{
    /// HD prefixes incorporate script typing.
    /// Hierarchical-deterministic public/private key prefixes.
    prefix::hds hd_prefix;

    /// Witness prefixes do not incorporate script typing.
    /// Witness address prefix.
    std::string witness_prefix;

    /// Address prefixes incorporate script typing.
    /// Payment (legacy address) address version byte.
    uint8_t address_version;

    /// WIF prefixes do not incorporate script typing.
    /// WIF (legacy private key) version byte.
    uint8_t wif_prefix;

    uint64_t hd_prefixes() const
    {
        // TODO: inconsistent order: private, public
        return hd_private::to_prefixes(hd_prefix.xprv, hd_prefix.xpub);
    }

    uint16_t versions() const
    {
        // TODO: inconsistency order: public, private
        return ec_private::to_versions(address_version, wif_prefix);
    }
};

/// Predefined wallet contexts.
/// Custom contexts can also be defined by simply populating a context struct.
namespace ctx
{
    namespace btc
    {
        namespace main
        {
            const context p2pkh
            {
                prefix::ext::p2pkh::main::btc,
                prefix::wit::main::btc,
                prefix::add::p2pkh::main::btc,
                prefix::wif::main::btc
            };
            const context p2sh
            {
                prefix::ext::p2sh::main::btc,
                prefix::wit::main::btc,
                prefix::add::p2sh::main::btc,
                prefix::wif::main::btc
            };
        }
        namespace test
        {
            const context p2pkh
            {
                prefix::ext::p2pkh::test::btc,
                prefix::wit::test::btc,
                prefix::add::p2pkh::test::btc,
                prefix::wif::test::btc
            };
            const context p2sh
            {
                prefix::ext::p2sh::test::btc,
                prefix::wit::test::btc,
                prefix::add::p2sh::test::btc,
                prefix::wif::test::btc
            };
        }
    }
    namespace ltc
    {
        namespace main
        {
            const context p2pkh
            {
                prefix::ext::p2pkh::main::ltc,
                prefix::wit::main::ltc,
                prefix::add::undefined,
                prefix::wif::undefined
            };
            const context p2sh
            {
                prefix::ext::p2sh::main::ltc,
                prefix::wit::main::ltc,
                prefix::add::undefined,
                prefix::wif::undefined
            };
        }
        namespace test
        {
            const context p2pkh
            {
                prefix::ext::p2pkh::test::ltc,
                prefix::wit::test::ltc,
                prefix::add::undefined,
                prefix::wif::undefined
            };
            const context p2sh
            {
                prefix::ext::undefined,
                prefix::wit::test::ltc,
                prefix::add::undefined,
                prefix::wif::undefined
            };
        }
    }
}

/// Predefined contexts for use as default parameter values.
extern const context btc_mainnet_p2kh;
extern const context btc_mainnet_p2sh;
extern const context btc_testnet_p2kh;
extern const context btc_testnet_p2sh;

extern const context btc_mainnet;
extern const context btc_testnet;

extern const context btc;

} // namespace wallet
} // namespace system
} // namespace libbitcoin

#endif
