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
#ifndef LIBBITCOIN_SYSTEM_WALLET_ADDRESSES_STEALTH_SENDER_HPP
#define LIBBITCOIN_SYSTEM_WALLET_ADDRESSES_STEALTH_SENDER_HPP

#include <cstdint>
#include <bitcoin/system/chain/chain.hpp>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/wallet/addresses/payment_address.hpp>
#include <bitcoin/system/wallet/addresses/stealth_address.hpp>

namespace libbitcoin {
namespace system {
namespace wallet {

/// This class does not support multisignature stealth addresses.
class BC_API stealth_sender
{
public:
    /// Constructors.
    /// Generate a send address from the stealth address.
    stealth_sender(const stealth_address& address, const data_chunk& seed,
        const binary& filter, uint8_t version=payment_address::mainnet_p2kh);

    /// Generate a send address from the stealth address.
    stealth_sender(const ec_secret& ephemeral_private,
        const stealth_address& address, const data_chunk& seed,
        const binary& filter, uint8_t version=payment_address::mainnet_p2kh);

    /// Caller must test after construct.
    operator bool() const;

    /// Attach this script to the output before the send output.
    const chain::script& stealth_script() const;

    /// The bitcoin payment address to which the payment will be made.
    const wallet::payment_address& payment_address() const;

private:
    void initialize(const ec_secret& ephemeral_private,
        const stealth_address& address, const data_chunk& seed,
        const binary& filter);

    const uint8_t version_;
    chain::script script_;
    wallet::payment_address address_;
};

} // namespace wallet
} // namespace system
} // namespace libbitcoin

#endif

