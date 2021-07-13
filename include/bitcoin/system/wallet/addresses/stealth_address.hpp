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
#ifndef LIBBITCOIN_SYSTEM_WALLET_ADDRESSES_STEALTH_ADDRESS_HPP
#define LIBBITCOIN_SYSTEM_WALLET_ADDRESSES_STEALTH_ADDRESS_HPP

#include <cstdint>
#include <iostream>
#include <vector>
#include <bitcoin/system/chain/chain.hpp>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {
namespace wallet {

/// A class for working with stealth payment addresses.
class BC_API stealth_address
{
public:
    /// DEPRECATED: we intend to make p2kh same as payment address versions.
    static const uint8_t mainnet_p2kh;

    /// If set and the spend_keys contains the scan_key then the key is reused.
    static const uint8_t reuse_key_flag;

    /// This is advisory in nature and likely to be enforced by a server.
    static const size_t min_filter_bits;

    /// This is the protocol limit to the size of a stealth prefix filter.
    static const size_t max_filter_bits;

    /// Constructors.
    stealth_address();
    stealth_address(const data_chunk& decoded);
    stealth_address(const std::string& encoded);
    stealth_address(const stealth_address& other);
    stealth_address(const binary& filter, const ec_compressed& scan_key,
        const compressed_list& spend_keys, uint8_t signatures=0,
        uint8_t version=mainnet_p2kh);

    /// Operators.
    bool operator<(const stealth_address& other) const;
    bool operator==(const stealth_address& other) const;
    bool operator!=(const stealth_address& other) const;
    stealth_address& operator=(const stealth_address& other);
    friend std::istream& operator>>(std::istream& in, stealth_address& to);
    friend std::ostream& operator<<(std::ostream& out,
        const stealth_address& of);

    /// Cast operators.
    operator bool() const;
    operator const data_chunk() const;

    /// Serializer.
    std::string encoded() const;

    /// Accessors.
    uint8_t version() const;
    const ec_compressed& scan_key() const;
    const compressed_list& spend_keys() const;
    uint8_t signatures() const;
    const binary& filter() const;

    /// Methods.
    data_chunk to_chunk() const;

private:
    /// Factories.
    static stealth_address from_string(const std::string& encoded);
    static stealth_address from_stealth(const data_chunk& decoded);
    static stealth_address from_stealth(const binary& filter,
        const ec_compressed& scan_key, const compressed_list& spend_keys,
        uint8_t signatures, uint8_t version);

    /// Parameter order is used to change the constructor signature.
    stealth_address(uint8_t version, const binary& filter,
        const ec_compressed& scan_key, const compressed_list& spend_keys,
        uint8_t signatures);

    /// Helpers.
    bool reuse_key() const;
    uint8_t options() const;


    /// Members.
    /// These should be const, apart from the need to implement assignment.
    bool valid_;
    uint8_t version_;
    ec_compressed scan_key_;
    compressed_list spend_keys_;
    uint8_t signatures_;
    binary filter_;
};

} // namespace wallet
} // namespace system
} // namespace libbitcoin

#endif
