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
#ifndef LIBBITCOIN_SYSTEM_WALLET_ADDRESSES_QRENCODE_HPP
#define LIBBITCOIN_SYSTEM_WALLET_ADDRESSES_QRENCODE_HPP

#include <cstdint>
#include <iostream>
#include <string>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {
namespace wallet {

class BC_API qr_code
{
public:
    enum class encode_mode
    {
        eight_bit = 2,
        kanji = 3
    };

    enum class recovery_level
    {
        low = 0,
        medium,
        high,
        highest
    };

    static uint8_t maximum_version;

    /// False if version > maximum_version or size > tiff::max_image_bytes.
    /// Create a TIFF formatter QR code representing the given string value.
    static bool encode(std::ostream& out, const std::string& value,
        uint8_t version=0, uint16_t scale=8, uint16_t margin=2,
        recovery_level level=recovery_level::low,
        encode_mode mode=encode_mode::eight_bit, bool case_sensitive=true);

protected:
    /// Convert QR encoded data stream to bit stream with margin and scaling.
    static data_chunk to_pixels(const data_chunk& coded, uint32_t width_coded,
        uint16_t scale=8, uint16_t margin=2);
};

} // namespace wallet
} // namespace system
} // namespace libbitcoin

#endif
