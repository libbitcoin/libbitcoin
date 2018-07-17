/**
 * Copyright (c) 2011-2018 libbitcoin developers (see AUTHORS)
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
#include <cstdint>
#include <bitcoin/bitcoin/settings.hpp>
#include <bitcoin/bitcoin/chain/block.hpp>

namespace libbitcoin {

// Common default values (no settings context).
settings::settings()
  : retargeting_factor(4),
    target_spacing_seconds(10 * 60),
    easy_spacing_seconds(20 * 60),
    timestamp_future_seconds(2 * 60 * 60),
    target_timespan_seconds(2 * 7 * 24 * 60 * 60),
    retarget_proof_of_work_limit(0x1d00ffff),
    no_retarget_proof_of_work_limit(0x207fffff),
    min_timespan(target_timespan_seconds / retargeting_factor),
    max_timespan(target_timespan_seconds * retargeting_factor),
    retargeting_interval(target_timespan_seconds / target_spacing_seconds),
    genesis_block(*this)
{
}

uint32_t settings::work_limit(bool retarget) const
{
    return retarget ? retarget_proof_of_work_limit :
        no_retarget_proof_of_work_limit;
}

settings::settings(config::settings context)
  : settings()
{
    switch (context)
    {
        case config::settings::mainnet:
        {
            genesis_block.from_data({
                0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x3b, 0xa3, 0xed, 0xfd,
                0x7a, 0x7b, 0x12, 0xb2, 0x7a, 0xc7, 0x2c, 0x3e,
                0x67, 0x76, 0x8f, 0x61, 0x7f, 0xc8, 0x1b, 0xc3,
                0x88, 0x8a, 0x51, 0x32, 0x3a, 0x9f, 0xb8, 0xaa,
                0x4b, 0x1e, 0x5e, 0x4a, 0x29, 0xab, 0x5f, 0x49,
                0xff, 0xff, 0x00, 0x1d, 0x1d, 0xac, 0x2b, 0x7c,
                0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
                0xff, 0xff, 0x4d, 0x04, 0xff, 0xff, 0x00, 0x1d,
                0x01, 0x04, 0x45, 0x54, 0x68, 0x65, 0x20, 0x54,
                0x69, 0x6d, 0x65, 0x73, 0x20, 0x30, 0x33, 0x2f,
                0x4a, 0x61, 0x6e, 0x2f, 0x32, 0x30, 0x30, 0x39,
                0x20, 0x43, 0x68, 0x61, 0x6e, 0x63, 0x65, 0x6c,
                0x6c, 0x6f, 0x72, 0x20, 0x6f, 0x6e, 0x20, 0x62,
                0x72, 0x69, 0x6e, 0x6b, 0x20, 0x6f, 0x66, 0x20,
                0x73, 0x65, 0x63, 0x6f, 0x6e, 0x64, 0x20, 0x62,
                0x61, 0x69, 0x6c, 0x6f, 0x75, 0x74, 0x20, 0x66,
                0x6f, 0x72, 0x20, 0x62, 0x61, 0x6e, 0x6b, 0x73,
                0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0xf2, 0x05,
                0x2a, 0x01, 0x00, 0x00, 0x00, 0x43, 0x41, 0x04,
                0x67, 0x8a, 0xfd, 0xb0, 0xfe, 0x55, 0x48, 0x27,
                0x19, 0x67, 0xf1, 0xa6, 0x71, 0x30, 0xb7, 0x10,
                0x5c, 0xd6, 0xa8, 0x28, 0xe0, 0x39, 0x09, 0xa6,
                0x79, 0x62, 0xe0, 0xea, 0x1f, 0x61, 0xde, 0xb6,
                0x49, 0xf6, 0xbc, 0x3f, 0x4c, 0xef, 0x38, 0xc4,
                0xf3, 0x55, 0x04, 0xe5, 0x1e, 0xc1, 0x12, 0xde,
                0x5c, 0x38, 0x4d, 0xf7, 0xba, 0x0b, 0x8d, 0x57,
                0x8a, 0x4c, 0x70, 0x2b, 0x6b, 0xf1, 0x1d, 0x5f,
                0xac, 0x00, 0x00, 0x00, 0x00});

            break;
        }

        case config::settings::testnet:
        {
            genesis_block.from_data({
                0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x3b, 0xa3, 0xed, 0xfd,
                0x7a, 0x7b, 0x12, 0xb2, 0x7a, 0xc7, 0x2c, 0x3e,
                0x67, 0x76, 0x8f, 0x61, 0x7f, 0xc8, 0x1b, 0xc3,
                0x88, 0x8a, 0x51, 0x32, 0x3a, 0x9f, 0xb8, 0xaa,
                0x4b, 0x1e, 0x5e, 0x4a, 0xda, 0xe5, 0x49, 0x4d,
                0xff, 0xff, 0x00, 0x1d, 0x1a, 0xa4, 0xae, 0x18,
                0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
                0xff, 0xff, 0x4d, 0x04, 0xff, 0xff, 0x00, 0x1d,
                0x01, 0x04, 0x45, 0x54, 0x68, 0x65, 0x20, 0x54,
                0x69, 0x6d, 0x65, 0x73, 0x20, 0x30, 0x33, 0x2f,
                0x4a, 0x61, 0x6e, 0x2f, 0x32, 0x30, 0x30, 0x39,
                0x20, 0x43, 0x68, 0x61, 0x6e, 0x63, 0x65, 0x6c,
                0x6c, 0x6f, 0x72, 0x20, 0x6f, 0x6e, 0x20, 0x62,
                0x72, 0x69, 0x6e, 0x6b, 0x20, 0x6f, 0x66, 0x20,
                0x73, 0x65, 0x63, 0x6f, 0x6e, 0x64, 0x20, 0x62,
                0x61, 0x69, 0x6c, 0x6f, 0x75, 0x74, 0x20, 0x66,
                0x6f, 0x72, 0x20, 0x62, 0x61, 0x6e, 0x6b, 0x73,
                0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0xf2, 0x05,
                0x2a, 0x01, 0x00, 0x00, 0x00, 0x43, 0x41, 0x04,
                0x67, 0x8a, 0xfd, 0xb0, 0xfe, 0x55, 0x48, 0x27,
                0x19, 0x67, 0xf1, 0xa6, 0x71, 0x30, 0xb7, 0x10,
                0x5c, 0xd6, 0xa8, 0x28, 0xe0, 0x39, 0x09, 0xa6,
                0x79, 0x62, 0xe0, 0xea, 0x1f, 0x61, 0xde, 0xb6,
                0x49, 0xf6, 0xbc, 0x3f, 0x4c, 0xef, 0x38, 0xc4,
                0xf3, 0x55, 0x04, 0xe5, 0x1e, 0xc1, 0x12, 0xde,
                0x5c, 0x38, 0x4d, 0xf7, 0xba, 0x0b, 0x8d, 0x57,
                0x8a, 0x4c, 0x70, 0x2b, 0x6b, 0xf1, 0x1d, 0x5f,
                0xac, 0x00, 0x00, 0x00, 0x00});

            break;
        }

        case config::settings::regtest:
        {
            genesis_block.from_data({
                0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x3b, 0xa3, 0xed, 0xfd,
                0x7a, 0x7b, 0x12, 0xb2, 0x7a, 0xc7, 0x2c, 0x3e,
                0x67, 0x76, 0x8f, 0x61, 0x7f, 0xc8, 0x1b, 0xc3,
                0x88, 0x8a, 0x51, 0x32, 0x3a, 0x9f, 0xb8, 0xaa,
                0x4b, 0x1e, 0x5e, 0x4a, 0xda, 0xe5, 0x49, 0x4d,
                0xff, 0xff, 0x7f, 0x20, 0x02, 0x00, 0x00, 0x00,
                0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
                0xff, 0xff, 0x4d, 0x04, 0xff, 0xff, 0x00, 0x1d,
                0x01, 0x04, 0x45, 0x54, 0x68, 0x65, 0x20, 0x54,
                0x69, 0x6d, 0x65, 0x73, 0x20, 0x30, 0x33, 0x2f,
                0x4a, 0x61, 0x6e, 0x2f, 0x32, 0x30, 0x30, 0x39,
                0x20, 0x43, 0x68, 0x61, 0x6e, 0x63, 0x65, 0x6c,
                0x6c, 0x6f, 0x72, 0x20, 0x6f, 0x6e, 0x20, 0x62,
                0x72, 0x69, 0x6e, 0x6b, 0x20, 0x6f, 0x66, 0x20,
                0x73, 0x65, 0x63, 0x6f, 0x6e, 0x64, 0x20, 0x62,
                0x61, 0x69, 0x6c, 0x6f, 0x75, 0x74, 0x20, 0x66,
                0x6f, 0x72, 0x20, 0x62, 0x61, 0x6e, 0x6b, 0x73,
                0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0xf2, 0x05,
                0x2a, 0x01, 0x00, 0x00, 0x00, 0x43, 0x41, 0x04,
                0x67, 0x8a, 0xfd, 0xb0, 0xfe, 0x55, 0x48, 0x27,
                0x19, 0x67, 0xf1, 0xa6, 0x71, 0x30, 0xb7, 0x10,
                0x5c, 0xd6, 0xa8, 0x28, 0xe0, 0x39, 0x09, 0xa6,
                0x79, 0x62, 0xe0, 0xea, 0x1f, 0x61, 0xde, 0xb6,
                0x49, 0xf6, 0xbc, 0x3f, 0x4c, 0xef, 0x38, 0xc4,
                0xf3, 0x55, 0x04, 0xe5, 0x1e, 0xc1, 0x12, 0xde,
                0x5c, 0x38, 0x4d, 0xf7, 0xba, 0x0b, 0x8d, 0x57,
                0x8a, 0x4c, 0x70, 0x2b, 0x6b, 0xf1, 0x1d, 0x5f,
                0xac, 0x00, 0x00, 0x00, 0x00});

            break;
        }

        default:
        case config::settings::none:
        {
        }
    }
}

} // namespace libbitcoin
