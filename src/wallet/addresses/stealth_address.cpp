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
#include <bitcoin/system/wallet/addresses/stealth_address.hpp>

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <utility>
#include <bitcoin/system/assert.hpp>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/exceptions.hpp>
#include <bitcoin/system/math/math.hpp>
#include <bitcoin/system/radix/radix.hpp>
#include <bitcoin/system/stream/stream.hpp>
#include <bitcoin/system/wallet/keys/stealth.hpp>

namespace libbitcoin {
namespace system {
namespace wallet {

using namespace bc::system::chain;

static constexpr uint8_t version_size = sizeof(uint8_t);
static constexpr uint8_t options_size = sizeof(uint8_t);
static constexpr uint8_t number_keys_size = sizeof(uint8_t);
static constexpr uint8_t number_sigs_size = sizeof(uint8_t);
static constexpr uint8_t filter_length_size = sizeof(uint8_t);
static constexpr uint8_t checksum_size = sizeof(uint32_t);
static constexpr uint8_t max_spend_key_count = max_uint8;

// wiki.unsystem.net/index.php/DarkWallet/Stealth#Address_format
// [version:1=0x2a][options:1][scan_pubkey:33][N:1][spend_pubkey_1:33]..
// [spend_pubkey_N:33][number_signatures:1][prefix_number_bits:1]
// [filter:prefix_number_bits / 8, round up][checksum:4]
// Estimate assumes N = 0 and prefix_length = 0:
constexpr size_t min_address_size = version_size + options_size +
    ec_compressed_size + number_keys_size + number_sigs_size +
    filter_length_size + checksum_size;

const uint8_t stealth_address::mainnet_p2kh = 0x2a;
const uint8_t stealth_address::reuse_key_flag = 1 << 0;
const size_t stealth_address::min_filter_bits = width<uint8_t>();
const size_t stealth_address::max_filter_bits = width<uint32_t>();

stealth_address::stealth_address()
  : valid_(false), version_(0), scan_key_(null_ec_compressed),
    spend_keys_(), signatures_(0), filter_()
{
}

stealth_address::stealth_address(const stealth_address& other)
  : valid_(other.valid_), version_(other.version_), scan_key_(other.scan_key_),
    spend_keys_(other.spend_keys_), signatures_(other.signatures_),
    filter_(other.filter_)
{
}

stealth_address::stealth_address(const std::string& encoded)
  : stealth_address(from_string(encoded))
{
}

stealth_address::stealth_address(const data_chunk& decoded)
  : stealth_address(from_stealth(decoded))
{
}

stealth_address::stealth_address(const binary& filter,
    const ec_compressed& scan_key, const compressed_list& spend_keys,
    uint8_t signatures, uint8_t version)
  : stealth_address(from_stealth(filter, scan_key, spend_keys, signatures,
        version))
{
}

stealth_address::stealth_address(uint8_t version, const binary& filter,
    const ec_compressed& scan_key, const compressed_list& spend_keys,
    uint8_t signatures)
  : valid_(true), filter_(filter), scan_key_(scan_key),
    spend_keys_(spend_keys), signatures_(signatures), version_(version)
{
}

// Factories.
// ----------------------------------------------------------------------------

stealth_address stealth_address::from_string(const std::string& encoded)
{
    data_chunk decoded;
    return decode_base58(decoded, encoded) ? stealth_address(decoded) :
        stealth_address();
}

// This is the stealth address parser.
stealth_address stealth_address::from_stealth(const data_chunk& decoded)
{
    // Size is guarded until we get to N.
    auto required_size = min_address_size;
    if (decoded.size() < required_size || !verify_checksum(decoded))
        return {};

    // [version:1 = 0x2a]
    auto iterator = decoded.begin();
    const auto version = *iterator;

    // [options:1]
    ++iterator;
    const auto options = *iterator;
    if (options > reuse_key_flag)
        return {};

    // [scan_pubkey:33]
    ++iterator;
    auto scan_key_begin = iterator;
    iterator += ec_compressed_size;
    ec_compressed scan_key;
    std::copy_n(scan_key_begin, ec_compressed_size, scan_key.begin());

    // [N:1]
    auto spend_keys_count = *iterator;
    ++iterator;

    // Adjust and retest required size. for pubkey list.
    required_size += spend_keys_count * ec_compressed_size;
    if (decoded.size() < required_size)
        return {};

    // We don't explicitly save 'reuse', instead we add to spend_keys_.
    compressed_list spend_keys;
    if (options == reuse_key_flag)
        spend_keys.push_back(scan_key);

    // [spend_pubkey_1:33]..[spend_pubkey_N:33]
    ec_compressed point;
    for (auto key = 0; key < spend_keys_count; ++key)
    {
        auto spend_key_begin = iterator;
        std::advance(iterator, ec_compressed_size);
        std::copy_n(spend_key_begin, ec_compressed_size, point.begin());
        spend_keys.push_back(point);
    }

    // [number_signatures:1]
    const auto signatures = *iterator;
    ++iterator;

    // [prefix_number_bits:1]
    const auto filter_bits = *iterator;
    if (filter_bits > max_filter_bits)
        return {};

    // [prefix:prefix_number_bits / 8, round up]
    ++iterator;
    const auto filter_bytes = ceilinged_divide(filter_bits, byte_bits);

    // Adjust and retest required size.
    required_size += filter_bytes;
    if (decoded.size() != required_size)
        return {};

    // Deserialize the filter bytes/blocks.
    const data_chunk raw_filter(iterator, std::next(iterator, filter_bytes));
    const binary filter(filter_bits, raw_filter);
    return { filter, scan_key, spend_keys, signatures, version };
}

// This corrects signature and spend_keys.
stealth_address stealth_address::from_stealth(const binary& filter,
    const ec_compressed& scan_key, const compressed_list& spend_keys,
    uint8_t signatures, uint8_t version)
{
    // Ensure there is at least one spend key.
    auto spenders = spend_keys;
    if (spenders.empty())
        spenders.push_back(scan_key);

    // Guard against too many keys.
    const auto spend_keys_size = spenders.size();
    if (spend_keys_size > max_spend_key_count)
        return {};

    // Guard against prefix too long.
    auto prefix_number_bits = filter.bits();
    if (prefix_number_bits > max_filter_bits)
        return {};

    // Coerce signatures to a valid range.
    const auto maximum = is_zero(signatures) || signatures > spend_keys_size;
    const auto coerced = maximum ? static_cast<uint8_t>(spend_keys_size) :
        signatures;

    // Parameter order is used to change the constructor signature.
    return { version, filter, scan_key, spenders, coerced };
}

// Cast operators.
// ----------------------------------------------------------------------------

stealth_address::operator bool() const
{
    return valid_;
}

stealth_address::operator const data_chunk() const
{
    return stealth_address::to_chunk();
}

// Serializer.
// ----------------------------------------------------------------------------

std::string stealth_address::encoded() const
{
    return encode_base58(stealth_address::to_chunk());
}

uint8_t stealth_address::version() const
{
    return version_;
}

// Accessors.
// ----------------------------------------------------------------------------

const binary& stealth_address::filter() const
{
    return filter_;
}

const ec_compressed& stealth_address::scan_key() const
{
    return scan_key_;
}

uint8_t stealth_address::signatures() const
{
    return signatures_;
}

const compressed_list& stealth_address::spend_keys() const
{
    return spend_keys_;
}

// Methods.
// ----------------------------------------------------------------------------

data_chunk stealth_address::to_chunk() const
{
    data_chunk address;
    address.push_back(version());
    address.push_back(options());
    extend(address, scan_key_);

    // Spend_pubkeys must have been guarded against a max size of 255.
    auto number_spend_pubkeys = static_cast<uint8_t>(spend_keys_.size());

    // Adjust for key reuse.
    if (reuse_key())
        --number_spend_pubkeys;

    address.push_back(number_spend_pubkeys);

    // Serialize the spend keys, excluding any that match the scan key.
    for (const auto& key: spend_keys_)
        if (key != scan_key_)
            extend(address, key);

    address.push_back(signatures_);

    // The prefix must be guarded against a size greater than 32
    // so that the bitfield can convert into uint32_t and sized by uint8_t.
    const auto prefix_number_bits = static_cast<uint8_t>(filter_.bits());

    // Serialize the prefix bytes/blocks.
    address.push_back(prefix_number_bits);
    extend(address, filter_.data());

    append_checksum(address);
    return address;
}


// Helpers.
// ----------------------------------------------------------------------------

bool stealth_address::reuse_key() const
{
    // If the spend_keys_ contains the scan_key_ then the key is reused.
    return std::find(spend_keys_.begin(), spend_keys_.end(), scan_key_) !=
        spend_keys_.end();
}

uint8_t stealth_address::options() const
{
    // There is currently only one option.
    return reuse_key() ? reuse_key_flag : 0x00;
}

// Operators.
// ----------------------------------------------------------------------------

stealth_address& stealth_address::operator=(const stealth_address& other)
{
    valid_ = other.valid_;
    version_ = other.version_;
    scan_key_ = other.scan_key_;
    spend_keys_ = other.spend_keys_;
    signatures_ = other.signatures_;
    filter_ = other.filter_;
    return *this;
}

bool stealth_address::operator<(const stealth_address& other) const
{
    return encoded() < other.encoded();
}

bool stealth_address::operator==(const stealth_address& other) const
{
    return valid_ == other.valid_ && version_ == other.version_ &&
        scan_key_ == other.scan_key_&& spend_keys_ == other.spend_keys_ &&
        signatures_ == other.signatures_ && filter_ == other.filter_;
}

bool stealth_address::operator!=(const stealth_address& other) const
{
    return !(*this == other);
}

std::istream& operator>>(std::istream& in, stealth_address& to)
{
    std::string value;
    in >> value;
    to = stealth_address(value);

    if (!to)
        throw istream_exception(value);

    return in;
}

std::ostream& operator<<(std::ostream& out, const stealth_address& of)
{
    out << of.encoded();
    return out;
}

} // namespace wallet
} // namespace system
} // namespace libbitcoin
