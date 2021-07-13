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
#include <bitcoin/system/crypto/checksum.hpp>

#include <utility>
#include <bitcoin/system/assert.hpp>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/radix/radix.hpp>
#include <bitcoin/system/serial/serial.hpp>
#include <bitcoin/system/unicode/ascii.hpp>

namespace libbitcoin {
namespace system {

// bitcoin checksum.
// ----------------------------------------------------------------------------

static uint32_t bitcoin_checksum(const data_slice& data)
{
    return from_little_endian<uint32_t>(bitcoin_hash(data));
}

data_chunk append_checksum(const data_loaf& slices)
{
    auto out = build_chunk(slices, checksum_default_size);
    append_checksum(out);
    return out;
}

void append_checksum(data_chunk& data)
{
    const auto check = bitcoin_checksum(data);
    extend(data, to_little_endian<uint32_t>(check));
    BITCOIN_ASSERT(verify_checksum(data));
}

bool verify_checksum(const data_slice& data)
{
    if (data.size() < sizeof(uint32_t))
        return false;

    const auto position = std::prev(data.end(), sizeof(uint32_t));
    const auto check = from_little_endian<uint32_t>({ position, data.end() });
    return check == bitcoin_checksum({ data.begin(), position });
}

// bech32 checksum
// ----------------------------------------------------------------------------

static const size_t bech32_version_size = 1;
static const size_t bech32_checksum_size = 6;

static base32_chunk bech32_expand_prefix(const std::string& prefix)
{
    const auto size = prefix.size();
    const auto lower = ascii_to_lower(prefix);
    base32_chunk out(add1(2u * size), 0x00);

    for (size_t index = 0; index < size; ++index)
    {
        const char character = lower[index];
        out[index] = character >> 5;
        out[size + 1u + index] = character;
    }

    out[size] = 0x00;
    return out;
}

static base32_chunk bech32_expand_checksum(uint32_t checksum)
{
    base32_chunk out(bech32_checksum_size, 0x00);
    out[0] = (checksum >> 25);
    out[1] = (checksum >> 20);
    out[2] = (checksum >> 15);
    out[3] = (checksum >> 10);
    out[4] = (checksum >> 5);
    out[5] = (checksum >> 0);
    return out;
}

static uint32_t bech32_checksum(const base32_chunk& data)
{
    uint32_t checksum = 1;

    for (const auto& value: data)
    {
        const uint32_t coeficient = (checksum >> 25);
        checksum = ((checksum & 0x01ffffff) << 5) ^ value.convert_to<uint8_t>();

        if (coeficient & 0x01) checksum ^= 0x3b6a57b2;
        if (coeficient & 0x02) checksum ^= 0x26508e6d;
        if (coeficient & 0x04) checksum ^= 0x1ea119fa;
        if (coeficient & 0x08) checksum ^= 0x3d4233dd;
        if (coeficient & 0x10) checksum ^= 0x2a1462b3;
    }

    return checksum;
}

// BIP173: All versions use 0x00000001 (bech32).
// BIP350: Nonzero versions use 0x2bc830a3 (bech32m).
static uint32_t bech32_constant(uint8_t version)
{
    return is_zero(version) ? 0x00000001 : 0x2bc830a3;
}

static void bech32_prepend_prefix(base32_chunk& data,
    const std::string& prefix)
{
    const auto expanded = bech32_expand_prefix(prefix);
    data.insert(data.begin(), expanded.begin(), expanded.end());
}

static void bech32_append_checksum(base32_chunk& data,
    const std::string& prefix, uint8_t version)
{
    auto prefixed = data;
    bech32_prepend_prefix(prefixed, prefix);
    prefixed.resize(prefixed.size() + bech32_checksum_size, 0x00);
    const auto checksum = bech32_checksum(prefixed) ^ bech32_constant(version);
    const auto checked = bech32_expand_checksum(checksum);
    data.insert(data.end(), checked.begin(), checked.end());
}

static bool bech32_verify_checksum(const base32_chunk& checked,
    const std::string& prefix, uint8_t version)
{
    auto prefixed = checked;
    bech32_prepend_prefix(prefixed, prefix);
    return bech32_checksum(prefixed) == bech32_constant(version);
}

base32_chunk bech32_build_checked(uint8_t version, const data_chunk& program,
    const std::string& prefix)
{
    // Version expansion would truncate a value above 5 bits.
    if (version >= (1 << 5))
        return {};

    auto checked = base32_unpack(program);
    checked.insert(checked.begin(), static_cast<uint5_t>(version));
    bech32_append_checksum(checked, prefix, version);
    BITCOIN_ASSERT(bech32_verify_checksum(checked, prefix, version));

    return checked;
}

bool bech32_verify_checked(uint8_t& out_version, data_chunk& out_program,
    const std::string& prefix, const base32_chunk& checked)
{
    if (checked.size() < bech32_version_size + bech32_checksum_size)
        return false;

    out_version = checked.front().convert_to<uint8_t>();
    out_program = base32_pack(
    {
        std::next(checked.begin(), bech32_version_size),
        std::prev(checked.end(), bech32_checksum_size)
    });

    return bech32_verify_checksum(checked, prefix, out_version);
}

} // namespace system
} // namespace libbitcoin
