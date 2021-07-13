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
#include <bitcoin/system/wallet/addresses/stealth_sender.hpp>

#include <cstdint>
#include <bitcoin/system/chain/chain.hpp>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/wallet/addresses/payment_address.hpp>
#include <bitcoin/system/wallet/keys/stealth.hpp>

namespace libbitcoin {
namespace system {
namespace wallet {

stealth_sender::stealth_sender(const stealth_address& address,
    const data_chunk& seed, const binary& filter, uint8_t version)
  : version_(version)
{
    ec_secret ephemeral_private;
    if (create_ephemeral_key(ephemeral_private, seed))
        initialize(ephemeral_private, address, seed, filter);
}

stealth_sender::stealth_sender(const ec_secret& ephemeral_private,
    const stealth_address& address, const data_chunk& seed,
    const binary& filter, uint8_t version)
  : version_(version)
{
    initialize(ephemeral_private, address, seed, filter);
}

stealth_sender::operator bool() const
{
    return address_;
}

// private
// TODO: convert to factory and make script_ and address_ const.
void stealth_sender::initialize(const ec_secret& ephemeral_private,
    const stealth_address& address, const data_chunk& seed,
    const binary& filter)
{
    ec_compressed ephemeral_public;
    if (!secret_to_public(ephemeral_public, ephemeral_private))
        return;

    const auto& spend_keys = address.spend_keys();
    if (spend_keys.size() != 1)
        return;

    ec_compressed sender_public;
    if (!uncover_stealth(sender_public, address.scan_key(), ephemeral_private,
        spend_keys.front()))
        return;

    if (create_stealth_script(script_, ephemeral_private, filter, seed))
        address_ = { sender_public, version_ };
}

// Will be invalid if construct fails.
const chain::script& stealth_sender::stealth_script() const
{
    return script_;
}

// Will be invalid if construct fails.
const wallet::payment_address& stealth_sender::payment_address() const
{
    return address_;
}

} // namespace wallet
} // namespace system
} // namespace libbitcoin
