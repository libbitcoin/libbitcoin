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
#ifndef LIBBITCOIN_SYSTEM_CONFIG_HASH256_HPP
#define LIBBITCOIN_SYSTEM_CONFIG_HASH256_HPP

#include <iostream>
#include <string>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {
namespace config {

/**
 * Serialization helper for a bitcoin 256 bit hash.
 */
class BC_API hash256
{
public:
    /**
     * A list of bitcoin 256 bit hashes.
     * This must provide operator<< for ostream in order to be used as a
     * boost::program_options default_value.
     */
    typedef std::vector<hash256> list;

    /**
     * Default constructor.
     */
    hash256();

    /**
     * Copy constructor.
     * @param[in]  other  The object to copy into self on construct.
     */
    hash256(const hash256& other);

    /**
     * Initialization constructor.
     * @param[in]  hexcode  The hash value in string hexidecimal form.
     */
    hash256(const std::string& hexcode);

    /**
     * Initialization constructor.
     * @param[in]  value  The hash value to initialize with.
     */
    hash256(const hash_digest& value);

    /**
     * Get the hash as a string.
     * @return The hash in the string hexidecimal form.
     */
    std::string to_string() const;

    /**
     * Override the equality operator.
     * @param[in]  other  The other object with which to compare.
     */
    bool operator==(const hash256& other) const;

    /**
     * Cast to internal type.
     * @return  This object's value cast to internal type.
     */
    operator const hash_digest&() const;

    /**
     * Define stream in. Throws if input is invalid.
     * @param[in]   input     The input stream to read the value from.
     * @param[out]  argument  The object to receive the read value.
     * @return                The input stream reference.
     */
    friend std::istream& operator>>(std::istream& input,
        hash256& argument);

    /**
     * Define stream out.
     * @param[in]   output    The output stream to write the value to.
     * @param[out]  argument  The object from which to obtain the value.
     * @return                The output stream reference.
     */
    friend std::ostream& operator<<(std::ostream& output,
        const hash256& argument);

private:
    hash_digest value_;
};

} // namespace config
} // namespace system
} // namespace libbitcoin

#endif
