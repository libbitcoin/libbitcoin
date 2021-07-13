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
#include "secp256k1_initializer.hpp"

#include <mutex>
#include <secp256k1.h>
#include <bitcoin/system/constants.hpp>

namespace libbitcoin {
namespace system {

// We do not share contexts because they may or may not both be required.
secp256k1_signing signing;
secp256k1_verification verification;

// Static helper for use with std::call_once.
void secp256k1_initializer::set_context(secp256k1_context** context,
    int flags)
{
    *context = secp256k1_context_create(flags);
}

// Protected base class constructor (must be derived).
secp256k1_initializer::secp256k1_initializer(int flags)
  : flags_(flags), context_(nullptr)
{
}

// Clean up the context on destruct.
secp256k1_initializer::~secp256k1_initializer()
{
    if (!is_null(context_))
        secp256k1_context_destroy(context_);
}

// Get the curve context and initialize on first use.
secp256k1_context* secp256k1_initializer::context()
{
    std::call_once(mutex_, set_context, &context_, flags_);
    return context_;
}

// Concrete type for signing init.
secp256k1_signing::secp256k1_signing()
  : secp256k1_initializer(SECP256K1_CONTEXT_SIGN)
{
}

// Concrete type for verification init.
secp256k1_verification::secp256k1_verification()
  : secp256k1_initializer(SECP256K1_CONTEXT_VERIFY)
{
}

} // namespace system
} // namespace libbitcoin
