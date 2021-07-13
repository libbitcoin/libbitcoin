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
#ifndef LIBBITCOIN_SYSTEM_PARSE_ENCRYPTED_TOKEN_HPP
#define LIBBITCOIN_SYSTEM_PARSE_ENCRYPTED_TOKEN_HPP

#include <cstdint>
#include <cstddef>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/wallet/keys/encrypted_keys.hpp>
#include "parse_encrypted_key.hpp"

namespace libbitcoin {
namespace system {
namespace wallet {

// Swap not defined.
class parse_encrypted_token
  : public parse_encrypted_prefix<8u>
{
public:
    static data_array<prefix_size> prefix_factory(bool lot_sequence);

    explicit parse_encrypted_token(const encrypted_token& value);

    bool lot_sequence() const;
    hash_digest data() const;
    ek_entropy entropy() const;
    one_byte sign() const;

private:
    bool verify_context() const;
    bool verify_magic() const;

    static constexpr uint8_t lot_context_ = 0x51;
    static constexpr uint8_t default_context_ = 0x53;
    static const data_array<magic_size> magic_;

    const ek_entropy entropy_;
    const one_byte sign_;
    const hash_digest data_;
};

} // namespace wallet
} // namespace system
} // namespace libbitcoin

#endif
