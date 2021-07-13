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
#include <bitcoin/system/wallet/addresses/stealth_receiver.hpp>

#include <cstdint>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/wallet/addresses/payment_address.hpp>
#include <bitcoin/system/wallet/addresses/stealth_address.hpp>
#include <bitcoin/system/wallet/keys/stealth.hpp>

namespace libbitcoin {
namespace system {
namespace wallet {

// TODO: use to factory and make address_ and spend_public_ const.
stealth_receiver::stealth_receiver(const ec_secret& scan_private,
    const ec_secret& spend_private, const binary& filter, uint8_t version)
  : version_(version), scan_private_(scan_private),
    spend_private_(spend_private)
{
    ec_compressed scan_public;
    if (secret_to_public(scan_public, scan_private_) &&
        secret_to_public(spend_public_, spend_private_))
    {
        address_ = { filter, scan_public, { spend_public_ } };
    }
}

stealth_receiver::operator bool() const
{
    return address_;
}

// Will be invalid if construct fails.
const wallet::stealth_address&  stealth_receiver::stealth_address() const
{
    return address_;
}

bool stealth_receiver::derive_address(payment_address& out_address,
    const ec_compressed& ephemeral_public) const
{
    ec_compressed receiver_public;
    if (!uncover_stealth(receiver_public, ephemeral_public, scan_private_,
        spend_public_))
        return false;

    out_address = { receiver_public, version_ };
    return true;
}

bool stealth_receiver::derive_private(ec_secret& out_private,
    const ec_compressed& ephemeral_public) const
{
    return uncover_stealth(out_private, ephemeral_public, scan_private_,
        spend_private_);
}

} // namespace wallet
} // namespace system
} // namespace libbitcoin
