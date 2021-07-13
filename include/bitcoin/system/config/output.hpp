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
#ifndef LIBBITCOIN_SYSTEM_CONFIG_OUTPUT_HPP
#define LIBBITCOIN_SYSTEM_CONFIG_OUTPUT_HPP

#include <cstdint>
#include <iostream>
#include <string>
#include <bitcoin/system/chain/chain.hpp>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {
namespace config {

/**
 * Serialization helper to convert between a base58-string:number and
 * a vector of chain::output.
 */
class BC_API output
{
public:

    /**
     * Default constructor.
     */
    output();

    /**
     * Initialization constructor.
     * @param[in]  tuple  The value to initialize with.
     */
    output(const std::string& tuple);

    /// Parsed properties
    bool is_stealth() const;
    uint64_t amount() const;
    uint8_t version() const;
    const chain::script& script() const;
    const short_hash& pay_to_hash() const;

    /**
     * Overload stream in. Throws if input is invalid.
     * @param[in]   input     The input stream to read the value from.
     * @param[out]  argument  The object to receive the read value.
     * @return                The input stream reference.
     */
    friend std::istream& operator>>(std::istream& input, output& argument);

private:

    /**
     * The transaction output state of this object.
     * This data is translated to an output given expected version information.
     */
    bool is_stealth_;
    uint64_t amount_;
    uint8_t version_;
    chain::script script_;
    short_hash pay_to_hash_;
};

} // namespace config
} // namespace system
} // namespace libbitcoin

#endif
