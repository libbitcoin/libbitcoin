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
#include <bitcoin/system/wallet/keys/ec_public.hpp>

#include <algorithm>
#include <iostream>
#include <utility>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/exceptions.hpp>
#include <bitcoin/system/radix/radix.hpp>
#include <bitcoin/system/wallet/addresses/payment_address.hpp>
#include <bitcoin/system/wallet/keys/ec_private.hpp>

namespace libbitcoin {
namespace system {
namespace wallet {

const uint8_t ec_public::mainnet_p2kh = 0x00;
const uint8_t ec_public::testnet_p2kh = 0x6f;

ec_public::ec_public()
  : ec_public(ec_point{})
{
}

ec_public::ec_public(const ec_point& point)
  : ec_point(point), compress_(true)
{
}

ec_public::ec_public(const ec_public& other)
  : ec_point(other), compress_(other.compress_)
{
}

ec_public::ec_public(const ec_private& secret)
  : ec_public(from_private(secret))
{
}

ec_public::ec_public(const data_chunk& decoded)
  : ec_public(from_data(decoded))
{
}

ec_public::ec_public(const std::string& base16)
  : ec_public(from_string(base16))
{
}

ec_public::ec_public(const ec_compressed& compressed, bool compress)
  : ec_point(compressed), compress_(compress)
{
}

ec_public::ec_public(const ec_uncompressed& uncompressed, bool compress)
  : ec_public(from_point(uncompressed, compress))
{
}

// Validators.
// ----------------------------------------------------------------------------

bool ec_public::is_point(const data_slice& decoded)
{
    return is_public_key(decoded);
}

// Factories.
// ----------------------------------------------------------------------------

ec_public ec_public::from_private(const ec_private& secret)
{
    if (!secret)
        return {};

    return { secret.to_public() };
}

ec_public ec_public::from_string(const std::string& base16)
{
    data_chunk decoded;
    if (!decode_base16(decoded, base16))
        return {};

    return { decoded };
}

ec_public ec_public::from_data(const data_chunk& decoded)
{
    if (!is_point(decoded))
        return {};

    if (is_compressed_key(decoded))
        return ec_public{ to_array<ec_compressed_size>(decoded), true };

    ec_compressed compressed;
    if (!system::compress(compressed, to_array<ec_uncompressed_size>(decoded)))
         return {};

    return { compressed, false };
}

ec_public ec_public::from_point(const ec_uncompressed& point, bool compress)
{
    if (!is_point(point))
        return {};

    ec_compressed compressed;
    if (!system::compress(compressed, point))
        return {};

    return { compressed, compress };
}

// Serializer.
// ----------------------------------------------------------------------------

std::string ec_public::encoded() const
{
    if (compressed())
        return encode_base16(point());

    // If the point is valid it should always decompress, but if not, is null.
    ec_uncompressed uncompressed;
    to_uncompressed(uncompressed);
    return encode_base16(uncompressed);
}

// Accessors.
// ----------------------------------------------------------------------------

bool ec_public::compressed() const
{
    return compress_;
}

// Methods.
// ----------------------------------------------------------------------------

bool ec_public::to_data(data_chunk& out) const
{
    if (!(*this))
        return false;

    if (compressed())
    {
        out.resize(ec_compressed_size);
        std::copy_n(point().begin(), ec_compressed_size, out.begin());
        return true;
    }

    ec_uncompressed uncompressed;
    if (to_uncompressed(uncompressed))
    {
        out.resize(ec_uncompressed_size);
        std::copy_n(uncompressed.begin(), ec_uncompressed_size, out.begin());
        return true;
    }

    return false;
}

bool ec_public::to_uncompressed(ec_uncompressed& out) const
{
    if (!(*this))
        return false;

    return decompress(out, to_array<ec_compressed_size>(point()));
}

payment_address ec_public::to_payment_address(uint8_t version) const
{
    return { *this, version };
}

// Operators.
// ----------------------------------------------------------------------------

ec_public& ec_public::operator=(ec_public other)
{
    swap(*this, other);
    return *this;
}

bool ec_public::operator<(const ec_public& other) const
{
    return encoded() < other.encoded();
}

bool ec_public::operator==(const ec_public& other) const
{
    return compress_ == other.compress_ && point() == other.point();
}

bool ec_public::operator!=(const ec_public& other) const
{
    return !(*this == other);
}

std::istream& operator>>(std::istream& in, ec_public& to)
{
    std::string value;
    in >> value;
    to = ec_public(value);

    if (!to)
        throw istream_exception(value);

    return in;
}

std::ostream& operator<<(std::ostream& out, const ec_public& of)
{
    out << of.encoded();
    return out;
}

// friend function, see: stackoverflow.com/a/5695855/1172329
void swap(ec_public& left, ec_public& right)
{
    using std::swap;

    // Must be unqualified (no std namespace).
    swap(static_cast<ec_point&>(left), static_cast<ec_point&>(right));
    swap(left.compress_, right.compress_);
}

} // namespace wallet
} // namespace system
} // namespace libbitcoin
