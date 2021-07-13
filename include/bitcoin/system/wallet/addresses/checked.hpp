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
#ifndef LIBBITCOIN_SYSTEM_WALLET_ADDRESSES_CHECKED_HPP
#define LIBBITCOIN_SYSTEM_WALLET_ADDRESSES_CHECKED_HPP

#include <cstddef>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>

namespace libbitcoin {
namespace system {
namespace wallet {

// TODO: use this template to define all checked wallet primitives.
// For example: typedef checked<1, short_hash_size, 4> payment;

/// Checked types are used for bitcoin payment addresses and other standards
/// that leverage the same technique. The behavior is unique to bitcoin.
/// This class is optimized to transport value_type (only) at no material
/// cost over a simple array, while simplifying parsing and checksumming.
template <size_t Prefix, size_t Payload,
    size_t Checksum = checksum_default_size>
class checked
  : public data_slice
{
public:
    static constexpr size_t value_size = (Prefix + Payload + Checksum);

    typedef data_array<Prefix> prefix_type;
    typedef data_array<Payload> payload_type;
    typedef data_array<Checksum> checksum_type;
    typedef data_array<value_size> value_type;

    /// Constructors.

    checked();
    checked(checked&& other);
    checked(const checked& other);
    checked(value_type&& value);
    checked(const value_type& value);

    /// Validity is guaranteed from this construction.
    checked(const prefix_type& prefix, const payload_type& payload);

    /// Operators.

    checked& operator=(checked&& other);
    checked& operator=(const checked& other);

    operator bool() const;
    operator data_chunk() const;
    operator const value_type&() const;

    /// Properties.

    prefix_type prefix() const;
    payload_type payload() const;
    checksum_type checksum() const;
    const value_type& value() const;

private:
    static checked from_payload(const prefix_type& version,
        const payload_type& payload);

    value_type value_;
};

/// Comparison operators.

template <size_t Prefix, size_t Payload,
    size_t Checksum = checksum_default_size>
bool operator==(
    const checked<Prefix, Payload, Checksum>& left,
    const checked<Prefix, Payload, Checksum>& right)
{
    return left.value() == right.value();
}

template <size_t Prefix, size_t Payload,
    size_t Checksum = checksum_default_size>
bool operator!=(
    const checked<Prefix, Payload, Checksum>& left,
    const checked<Prefix, Payload, Checksum>& right)
{
    return !(left == right);
}

} // namespace wallet
} // namespace system
} // namespace libbitcoin

#include <bitcoin/system/impl/wallet/addresses/checked.ipp>

#endif
