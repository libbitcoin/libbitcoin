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
#ifndef LIBBITCOIN_SYSTEM_WALLET_ADDRESSES_CHECKED_IPP
#define LIBBITCOIN_SYSTEM_WALLET_ADDRESSES_CHECKED_IPP

#include <cstddef>
#include <tuple>
#include <utility>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {
namespace wallet {

// construction
// ----------------------------------------------------------------------------

template <size_t Prefix, size_t Payload, size_t Checksum>
checked<Prefix, Payload, Checksum>::checked()
  : data_slice(value_), value_()
{
    // This is an invalid instance (unchecked).
}

template <size_t Prefix, size_t Payload, size_t Checksum>
checked<Prefix, Payload, Checksum>::checked(checked&& other)
  : data_slice(value_), value_(std::move(other.value_))
{
    // This may be an invalid instance.
}

template <size_t Prefix, size_t Payload, size_t Checksum>
checked<Prefix, Payload, Checksum>::checked(const checked& other)
  : data_slice(value_), value_(other.value_)
{
    // This may be an invalid instance.
}

template <size_t Prefix, size_t Payload, size_t Checksum>
checked<Prefix, Payload, Checksum>::checked(const prefix_type& prefix,
    const payload_type& payload)
  : checked(from_payload(prefix, payload))
{
    // This is a valid instance.
}

template <size_t Prefix, size_t Payload, size_t Checksum>
checked<Prefix, Payload, Checksum>::checked(value_type&& value)
  : data_slice(value_), value_(std::move(value))
{
    // This may be an invalid instance (if value is unchecked).
}

template <size_t Prefix, size_t Payload, size_t Checksum>
checked<Prefix, Payload, Checksum>::checked(const value_type& value)
  : data_slice(value_), value_(value)
{
    // This may be an invalid instance (if value is unchecked).
}

// private
template <size_t Prefix, size_t Payload, size_t Checksum>
checked<Prefix, Payload, Checksum>
checked<Prefix, Payload, Checksum>::from_payload(const prefix_type& version,
    const payload_type& payload)
{
    // Accessing the Checksum parameter requires explicitly specifying both.
    return insert_checksum<value_size, Checksum>({ version, payload });
}

// operators
// ----------------------------------------------------------------------------

template <size_t Prefix, size_t Payload, size_t Checksum>
checked<Prefix, Payload, Checksum>&
checked<Prefix, Payload, Checksum>::operator=(checked&& other)
{
    if (&other == this)
        return *this;

    // This may be an invalid instance (if value is unchecked).
    value_ = std::move(other.value_);
    return *this;
}

template <size_t Prefix, size_t Payload, size_t Checksum>
checked<Prefix, Payload, Checksum>&
checked<Prefix, Payload, Checksum>::operator=(const checked& other)
{
    if (&other == this)
        return *this;

    // This may be an invalid instance (if value is unchecked).
    value_ = other.value_;
    return *this;
}

template <size_t Prefix, size_t Payload, size_t Checksum>
checked<Prefix, Payload, Checksum>::operator bool() const
{
    // Accessing the Checksum parameter requires explicitly specifying both.
    return verify_checksum<value_size, Checksum>(value_);
}

template <size_t Prefix, size_t Payload, size_t Checksum>
checked<Prefix, Payload, Checksum>::operator data_chunk() const
{
    return to_chunk(value_);
}

template <size_t Prefix, size_t Payload, size_t Checksum>
checked<Prefix, Payload, Checksum>::operator
const typename checked<Prefix, Payload, Checksum>::value_type&() const
{
    return value_;
}

// properties
// ----------------------------------------------------------------------------

template <size_t Prefix, size_t Payload, size_t Checksum>
typename checked<Prefix, Payload, Checksum>::prefix_type
checked<Prefix, Payload, Checksum>::prefix() const
{
    return slice<0u, Prefix>(value_);
}

template <size_t Prefix, size_t Payload, size_t Checksum>
typename checked<Prefix, Payload, Checksum>::payload_type
checked<Prefix, Payload, Checksum>::payload() const
{
    return slice<Prefix, Prefix + Payload>(value_);
}

template <size_t Prefix, size_t Payload, size_t Checksum>
typename checked<Prefix, Payload, Checksum>::checksum_type
checked<Prefix, Payload, Checksum>::checksum() const
{
    return slice<Prefix + Payload, value_size>(value_);
}

template <size_t Prefix, size_t Payload, size_t Checksum>
const typename checked<Prefix, Payload, Checksum>::value_type&
checked<Prefix, Payload, Checksum>::value() const
{
    return value_;
}

} // namespace wallet
} // namespace system
} // namespace libbitcoin

#endif
