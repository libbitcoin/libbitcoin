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
#ifndef LIBBITCOIN_SYSTEM_WALLET_KEYS_HD_PRIVATE_HPP
#define LIBBITCOIN_SYSTEM_WALLET_KEYS_HD_PRIVATE_HPP

#include <cstdint>
#include <iostream>
#include <string>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/wallet/keys/ec_private.hpp>
#include <bitcoin/system/wallet/keys/ec_public.hpp>
#include <bitcoin/system/wallet/keys/hd_public.hpp>

namespace libbitcoin {
namespace system {
namespace wallet {

/// An extended private key, as defined by BIP32.
/// Additional prefix codes are documented in SLIP132.
/// github.com/satoshilabs/slips/blob/master/slip-0132.md
class BC_API hd_private
  : public hd_public
{
public:
    static const uint64_t mainnet;
    static const uint64_t testnet;

    static uint32_t to_prefix(uint64_t prefixes)
    {
        return prefixes >> 32;
    }

    static uint64_t to_prefixes(uint32_t private_prefix,
        uint32_t public_prefix)
    {
        return uint64_t(private_prefix) << 32 | public_prefix;
    }

    /// Constructors.
    hd_private();
    hd_private(const hd_private& other);
    hd_private(const data_chunk& entropy, uint64_t prefixes=mainnet);
    hd_private(const hd_key& private_key);
    hd_private(const hd_key& private_key, uint64_t prefixes);
    hd_private(const hd_key& private_key, uint32_t public_prefix);
    hd_private(const std::string& encoded);
    hd_private(const std::string& encoded, uint64_t prefixes);
    hd_private(const std::string& encoded, uint32_t public_prefix);
    hd_private(const ec_secret& secret, const hd_chain_code& chain_code,
        uint64_t prefixes=mainnet);

    /// Operators.
    bool operator<(const hd_private& other) const;
    bool operator==(const hd_private& other) const;
    bool operator!=(const hd_private& other) const;
    hd_private& operator=(hd_private other);
    friend std::istream& operator>>(std::istream& in, hd_private& to);
    friend std::ostream& operator<<(std::ostream& out,
        const hd_private& of);

    /// Swap implementation required to properly handle derived class assign.
    friend void swap(hd_private& left, hd_private& right);

    /// Cast operators.
    operator const ec_secret&() const;

    /// Serializer.
    std::string encoded() const;

    /// Accessors.
    const ec_secret& secret() const;

    /// Methods.
    hd_key to_hd_key() const;
    hd_public to_public() const;
    hd_private derive_private(uint32_t index) const;
    hd_public derive_public(uint32_t index) const;

private:
    /// Factories.
    static hd_private from_entropy(const data_slice& seed, uint64_t prefixes);
    static hd_private from_key(const hd_key& decoded, uint32_t public_prefix);
    static hd_private from_key(const hd_key& decoded, uint64_t prefixes);
    static hd_private from_private(const ec_secret& secret,
        const hd_chain_code& chain_code, uint64_t prefixes);
    static hd_private from_string(const std::string& encoded, uint32_t public_prefix);
    static hd_private from_string(const std::string& encoded, uint64_t prefixes);

    hd_private(const ec_secret& secret, const hd_chain_code& chain_code,
        const hd_lineage& lineage);

    /// Members.
    /// This should be const, apart from the need to implement assignment.
    ec_secret secret_;
};

} // namespace wallet
} // namespace system
} // namespace libbitcoin

#endif
