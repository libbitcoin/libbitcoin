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
#ifndef LIBBITCOIN_SYSTEM_WALLET_KEYS_EC_SCALAR_HPP
#define LIBBITCOIN_SYSTEM_WALLET_KEYS_EC_SCALAR_HPP

#include <cstdint>
#include <memory>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {
    
/// ec_scalar is initialized to zero.
/// Failed operations return zero.
/// The bool operator reflects the zero state (validity).
/// Does not implement string serialization.
class BC_API ec_scalar
{
public:
    /// Constructors.
    ec_scalar();
    ec_scalar(ec_scalar&& scalar);
    ec_scalar(const ec_scalar& scalar);
    ec_scalar(ec_secret&& secret);
    ec_scalar(const ec_secret& secret);
    ec_scalar(int64_t value);

    /// Operators.
    ec_scalar& operator=(const ec_scalar& scalar);
    ec_scalar& operator=(ec_scalar&& scalar);
    ec_scalar& operator=(const ec_secret& secret);
    ec_scalar& operator=(ec_secret&& secret);
    ec_scalar& operator+=(const ec_scalar& scalar);
    ec_scalar& operator-=(const ec_scalar& scalar);
    ec_scalar& operator*=(const ec_scalar& scalar);
    ec_scalar operator-() const;

    /// Cast operators.
    operator bool() const;
    operator const ec_secret&() const;

    /// Accessors.
    const ec_secret& secret() const;

private:
    static ec_scalar from_int64(int64_t value);

    // This should be const, apart from the need to implement assignment.
    ec_secret secret_;
};

bool operator==(int64_t left, const ec_scalar& right);
bool operator!=(int64_t left, const ec_scalar& right);
bool operator==(const ec_scalar& left, int64_t right);
bool operator!=(const ec_scalar& left, int64_t right);
bool operator==(const ec_scalar& left, const ec_scalar& right);
bool operator!=(const ec_scalar& left, const ec_scalar& right);
ec_scalar operator+(const ec_scalar& left, const ec_scalar& right);
ec_scalar operator-(const ec_scalar& left, const ec_scalar& right);
ec_scalar operator*(const ec_scalar& left, const ec_scalar& right);

} // namespace system
} // namespace libbitcoin

#endif

