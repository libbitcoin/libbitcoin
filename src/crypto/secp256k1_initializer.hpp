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
#ifndef LIBBITCOIN_SYSTEM_CRYPTO_SECP256K1_INITIALIZER_HPP
#define LIBBITCOIN_SYSTEM_CRYPTO_SECP256K1_INITIALIZER_HPP

#include <mutex>
#include <secp256k1.h>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {

/**
 * Virtual base class for secp256k1 context management.
 * This class holds no static state but will only initialize its state once for
 * the given mutex. This can be assigned to a static or otherwise. It lazily
 * inits the context once and destroys the context on destruct as necessary.
 */
class BC_API secp256k1_initializer
{
private:
    static void set_context(secp256k1_context** context, int flags);

protected:
    int flags_;

    /**
     * Construct a signing context initializer of the specified context.
     * @param[in]  flags  { SECP256K1_CONTEXT_SIGN, SECP256K1_CONTEXT_VERIFY }
     */
    secp256k1_initializer(int flags);

public:
    /**
     * Free the context if initialized.
     */
    ~secp256k1_initializer();

    /**
     * Call to obtain the secp256k1 context, initialized on first call.
     */
    secp256k1_context* context();

private:
    std::once_flag mutex_;
    secp256k1_context* context_;
};

/**
 * Create and hold this class to initialize signing context on first use.
 */
class BC_API secp256k1_signing
  : public secp256k1_initializer
{
public:
    /**
     * Construct a signing context initializer.
     */
    secp256k1_signing();
};

/**
 * Create and hold this class to initialize verification context on first use.
 */
class BC_API secp256k1_verification
  : public secp256k1_initializer
{
public:
    /**
     * Construct a verification context initializer.
     */
    secp256k1_verification();
};

/**
 * Use bc::system::signing.context() to obtain the secp256k1 signing context.
 */
extern secp256k1_signing signing;

/**
 * Use bc::system::verification.context() to obtain the secp256k1 verification
 * context.
 */
extern secp256k1_verification verification;

} // namespace system
} // namespace libbitcoin

#endif
