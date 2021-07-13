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
#ifndef LIBBITCOIN_SYSTEM_SETTINGS_HPP
#define LIBBITCOIN_SYSTEM_SETTINGS_HPP

#include <cstdint>
#include <cstddef>
#include <string>
#include <bitcoin/system/chain/chain.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {

/// Common database configuration settings, not thread safe.
class BC_API settings
{
public:
    settings();
    settings(chain::selection context);

    // Utilities.
    //-------------------------------------------------------------------------
    uint64_t bitcoin_to_satoshi(uint64_t value) const;

    // Computed properties.
    //--------------------------------------------------------------------------

    uint64_t max_money() const;
    uint64_t initial_subsidy() const;
    uint32_t minimum_timespan() const;
    uint32_t maximum_timespan() const;
    size_t retargeting_interval() const;
    uint256_t work_limit() const;

    // Consensus settings.
    //--------------------------------------------------------------------------

    uint32_t retargeting_factor;
    uint32_t retargeting_interval_seconds;
    uint32_t block_spacing_seconds;
    uint32_t timestamp_limit_seconds;
    uint32_t proof_of_work_limit;
    uint64_t initial_subsidy_bitcoin;
    uint64_t subsidy_interval_blocks;

    // TODO: make serialiable (see config::chain).
    chain::block genesis_block;

    // Fork settings.
    //--------------------------------------------------------------------------

    // Consensus rule change activation and enforcement parameters (opaque).
    uint32_t first_version;
    uint32_t bip34_version;
    uint32_t bip66_version;
    uint32_t bip65_version;
    uint32_t bip9_version_bit0;
    uint32_t bip9_version_bit1;
    uint32_t bip9_version_base;

    // Activation parameters (bip34-style activations).
    size_t activation_threshold;
    size_t enforcement_threshold;
    size_t activation_sample;

    // Frozen activation heights (frozen_activations).
    size_t bip65_freeze;
    size_t bip66_freeze;
    size_t bip34_freeze;

    // Block 514 is the first testnet block after date-based activation.
    // Block 173805 is the first mainnet block after date-based activation.
    // The first mainnet activation window hardwired in satoshi 0.6.0rc1 failed.
    uint32_t bip16_activation_time;

    // bip90 stops checking unspent duplicates above this bip34 activation.
    chain::check_point bip34_active_checkpoint;

    // This cannot be reactivated in a future branch due to window expiration.
    chain::check_point bip9_bit0_active_checkpoint;

    // This cannot be reactivated in a future branch due to window expiration.
    chain::check_point bip9_bit1_active_checkpoint;
};

} // namespace system
} // namespace libbitcoin

#endif
