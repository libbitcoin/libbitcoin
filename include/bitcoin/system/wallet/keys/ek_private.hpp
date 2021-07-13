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
#ifndef LIBBITCOIN_SYSTEM_WALLET_KEYS_EK_PRIVATE_HPP
#define LIBBITCOIN_SYSTEM_WALLET_KEYS_EK_PRIVATE_HPP

#include <iostream>
#include <string>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/wallet/keys/encrypted_keys.hpp>

namespace libbitcoin {
namespace system {
namespace wallet {

/// Use to pass an encrypted private key.
class BC_API ek_private
{
public:
    /// Constructors.
    ek_private();
    ek_private(const std::string& encoded);
    ek_private(const encrypted_private& key);
    ek_private(const ek_private& other);

    /// Operators.
    bool operator<(const ek_private& other) const;
    bool operator==(const ek_private& other) const;
    bool operator!=(const ek_private& other) const;
    ek_private& operator=(const ek_private& other);
    friend std::istream& operator>>(std::istream& in, ek_private& to);
    friend std::ostream& operator<<(std::ostream& out, const ek_private& of);

    /// Cast operators.
    operator bool() const;
    operator const encrypted_private&() const;

    /// Serializer.
    std::string encoded() const;

    /// Accessors.
    const encrypted_private& private_key() const;

private:
    /// Factories.
    static ek_private from_string(const std::string& encoded);

    /// Members.
    /// These should be const, apart from the need to implement assignment.
    bool valid_;
    encrypted_private private_;
};

} // namespace wallet
} // namespace system
} // namespace libbitcoin

#endif
