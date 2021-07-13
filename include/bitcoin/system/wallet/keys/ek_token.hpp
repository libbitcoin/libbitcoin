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
#ifndef LIBBITCOIN_SYSTEM_WALLET_KEYS_EK_TOKEN_HPP
#define LIBBITCOIN_SYSTEM_WALLET_KEYS_EK_TOKEN_HPP

#include <iostream>
#include <string>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/wallet/keys/encrypted_keys.hpp>

namespace libbitcoin {
namespace system {
namespace wallet {

/**
 * Serialization helper to convert between base58 string and bip38 token.
 */
class BC_API ek_token
{
public:
    /// Constructors.
    ek_token();
    ek_token(const std::string& encoded);
    ek_token(const encrypted_token& key);
    ek_token(const ek_token& other);

    /// Operators.
    bool operator<(const ek_token& other) const;
    bool operator==(const ek_token& other) const;
    bool operator!=(const ek_token& other) const;
    ek_token& operator=(const ek_token& other);
    friend std::istream& operator>>(std::istream& in, ek_token& to);
    friend std::ostream& operator<<(std::ostream& out, const ek_token& of);

    /// Cast operators.
    operator bool() const;
    operator const encrypted_token&() const;

    /// Serializer.
    std::string encoded() const;

    /// Accessors.
    const encrypted_token& token() const;

private:
    /// Factories.
    static ek_token from_string(const std::string& encoded);

    /// Members.
    /// These should be const, apart from the need to implement assignment.
    bool valid_;
    encrypted_token token_;
};

} // namespace wallet
} // namespace system
} // namespace libbitcoin

#endif
