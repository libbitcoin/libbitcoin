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
#include <bitcoin/system/wallet/keys/hd_private.hpp>

#include <cstdint>
#include <iostream>
#include <string>
#include <utility>
#include <bitcoin/system/assert.hpp>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/exceptions.hpp>
#include <bitcoin/system/radix/radix.hpp>
#include <bitcoin/system/serial/serial.hpp>
#include <bitcoin/system/stream/stream.hpp>
#include <bitcoin/system/wallet/keys/ec_private.hpp>
#include <bitcoin/system/wallet/keys/ec_public.hpp>

namespace libbitcoin {
namespace system {
namespace wallet {

const uint64_t hd_private::mainnet = to_prefixes(0x0488ade4, hd_public::mainnet);
const uint64_t hd_private::testnet = to_prefixes(0x04358394, hd_public::testnet);

hd_private::hd_private()
  : hd_public(), secret_(null_hash)
{
}

hd_private::hd_private(const hd_private& other)
  : hd_public(other), secret_(other.secret_)
{
}

hd_private::hd_private(const data_chunk& entropy, uint64_t prefixes)
  : hd_private(from_entropy(entropy, prefixes))
{
}

// This reads the private version and sets the public to mainnet.
hd_private::hd_private(const hd_key& private_key)
  : hd_private(from_key(private_key, hd_public::mainnet))
{
}
// This reads the private version and sets the public.
hd_private::hd_private(const hd_key& private_key, uint32_t public_prefix)
  : hd_private(from_key(private_key, public_prefix))
{
}

// This validates the private version and sets the public.
hd_private::hd_private(const hd_key& private_key, uint64_t prefixes)
  : hd_private(from_key(private_key, prefixes))
{
}

// This reads the private version and sets the public to mainnet.
hd_private::hd_private(const std::string& encoded)
  : hd_private(from_string(encoded, hd_public::mainnet))
{
}

// This reads the private version and sets the public.
hd_private::hd_private(const std::string& encoded, uint32_t public_prefix)
  : hd_private(from_string(encoded, public_prefix))
{
}

// This validates the private version and sets the public.
hd_private::hd_private(const std::string& encoded, uint64_t prefixes)
  : hd_private(from_string(encoded, prefixes))
{
}

hd_private::hd_private(const ec_secret& secret,
    const hd_chain_code& chain_code, uint64_t prefixes)
  : hd_public(from_private(secret, chain_code, prefixes)),
    secret_(secret)
{
}

// private
hd_private::hd_private(const ec_secret& secret,
    const hd_chain_code& chain_code, const hd_lineage& lineage)
  : hd_public(from_secret(secret, chain_code, lineage)),
    secret_(secret)
{
}

// Factories.
// ----------------------------------------------------------------------------

hd_private hd_private::from_private(const ec_secret& secret,
    const hd_chain_code& chain_code, uint64_t prefixes)
{
    // The key is invalid if parse256(IL) >= n or 0:
    if (!verify(secret))
        return {};

    const hd_lineage master
    {
        prefixes,
        0x00,
        0x00000000,
        0x00000000
    };

    return hd_private(secret, chain_code, master);
}

hd_private hd_private::from_entropy(const data_slice& entropy,
    uint64_t prefixes)
{
    // This is a magic constant from BIP32.
    static const auto magic = to_chunk("Bitcoin seed");
    const auto intermediate = split(hmac_sha512_hash(entropy, magic));

    return hd_private(intermediate.first, intermediate.second, prefixes);
}

hd_private hd_private::from_key(const hd_key& key, uint32_t public_prefix)
{
    const auto prefix = from_big_endian<uint32_t>(key);
    return from_key(key, to_prefixes(prefix, public_prefix));
}

hd_private hd_private::from_key(const hd_key& key, uint64_t prefixes)
{
    read::bytes::copy source(key);

    const auto prefix = source.read_4_bytes_big_endian();
    const auto depth = source.read_byte();
    const auto parent = source.read_4_bytes_big_endian();
    const auto child = source.read_4_bytes_big_endian();
    const auto chain = source.read_hash();
    source.skip_byte();
    const auto secret = source.read_hash();

    // Validate the prefix against the provided value.
    if (prefix != to_prefix(prefixes))
        return {};

    const hd_lineage lineage
    {
        prefixes,
        depth,
        parent,
        child
    };

    return hd_private(secret, chain, lineage);
}

hd_private hd_private::from_string(const std::string& encoded,
    uint32_t public_prefix)
{
    hd_key key;
    if (!decode_base58(key, encoded))
        return {};

    return hd_private(from_key(key, public_prefix));
}

hd_private hd_private::from_string(const std::string& encoded,
    uint64_t prefixes)
{
    hd_key key;
    return decode_base58(key, encoded) ? hd_private(key, prefixes) :
        hd_private{};
}

// Cast operators.
// ----------------------------------------------------------------------------

hd_private::operator const ec_secret&() const
{
    return secret_;
}

// Serializer.
// ----------------------------------------------------------------------------

std::string hd_private::encoded() const
{
    return encode_base58(to_hd_key());
}

/// Accessors.
// ----------------------------------------------------------------------------

const ec_secret& hd_private::secret() const
{
    return secret_;
}

// Methods.
// ----------------------------------------------------------------------------

// HD keys do not carry a payment address prefix (just like WIF).
// So we are currently not converting to ec_public or ec_private.

hd_key hd_private::to_hd_key() const
{
    static constexpr uint8_t private_key_padding = 0x00;

    return insert_checksum<hd_key_size>(
    {
        to_big_endian(to_prefix(lineage_.prefixes)),
        to_array(lineage_.depth),
        to_big_endian(lineage_.parent_fingerprint),
        to_big_endian(lineage_.child_number),
        chain_,
        to_array(private_key_padding),
        secret_
    });
}

hd_public hd_private::to_public() const
{
    return hd_public(((hd_public)*this).to_hd_key(),
        hd_public::to_prefix(lineage_.prefixes));
}

hd_private hd_private::derive_private(uint32_t index) const
{
    constexpr uint8_t depth = 0;

    const auto data = (index >= hd_first_hardened_key) ?
        splice(to_array(depth), secret_, to_big_endian(index)) :
        splice(point_, to_big_endian(index));

    const auto intermediate = split(hmac_sha512_hash(data, chain_));

    // The child key ki is (parse256(IL) + kpar) mod n:
    auto child = secret_;
    if (!ec_add(child, intermediate.first))
        return {};

    if (lineage_.depth == max_uint8)
        return {};

    const hd_lineage lineage
    {
        lineage_.prefixes,
        static_cast<uint8_t>(add1(lineage_.depth)),
        fingerprint(),
        index
    };

    return hd_private(child, intermediate.second, lineage);
}

hd_public hd_private::derive_public(uint32_t index) const
{
    return derive_private(index).to_public();
}

// Operators.
// ----------------------------------------------------------------------------

hd_private& hd_private::operator=(hd_private other)
{
    swap(*this, other);
    return *this;
}

bool hd_private::operator<(const hd_private& other) const
{
    return encoded() < other.encoded();
}

bool hd_private::operator==(const hd_private& other) const
{
    return secret_ == other.secret_ && valid_ == other.valid_ &&
        chain_ == other.chain_ && lineage_ == other.lineage_ &&
        point_ == other.point_;
}

bool hd_private::operator!=(const hd_private& other) const
{
    return !(*this == other);
}

// We must assume mainnet for public version here.
// When converting this to public a clone of this key should be used, with the
// public version specified - after validating the private version.
std::istream& operator>>(std::istream& in, hd_private& to)
{
    std::string value;
    in >> value;
    to = hd_private(value, hd_public::mainnet);

    if (!to)
        throw istream_exception(value);

    return in;
}

std::ostream& operator<<(std::ostream& out, const hd_private& of)
{
    out << of.encoded();
    return out;
}

// friend function, see: stackoverflow.com/a/5695855/1172329
void swap(hd_private& left, hd_private& right)
{
    using std::swap;

    // Must be unqualified (no std namespace).
    swap(static_cast<hd_public&>(left), static_cast<hd_public&>(right));
    swap(left.secret_, right.secret_);
}

} // namespace wallet
} // namespace system
} // namespace libbitcoin
