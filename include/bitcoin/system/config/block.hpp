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
#ifndef LIBBITCOIN_SYSTEM_CONFIG_BLOCK_HPP
#define LIBBITCOIN_SYSTEM_CONFIG_BLOCK_HPP

#include <iostream>
#include <string>
#include <bitcoin/system/chain/block.hpp>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {
namespace config {

/**
 * Serialization helper to convert between serialized and deserialized satoshi
 * block.
 */
class BC_API block
{
public:

    /**
     * Default constructor.
     */
    block();

    /**
     * Initialization constructor.
     * @param[in]  hexcode  The value to initialize with.
     */
    block(const std::string& hexcode);

    /**
     * Initialization constructor.
     * @param[in]  value  The value to initialize with.
     */
    block(const chain::block& value);

    /**
     * Copy constructor.
     * @param[in]  other  The object to copy into self on construct.
     */
    block(const block& other);

    /**
     * Copy assignment operator.
     * @param[in]  other  The object to copy into self on assignment.
     */
    block& operator=(const block& other);

    /**
     * Move assignment operator.
     * @param[in]  other  The object to move into self on assignment.
     */
    block& operator=(chain::block&& other);

    /**
     * Override the equality operator.
     * @param[in]  other  The other object with which to compare.
     */
    bool operator==(const block& other) const;

    /**
     * Overload cast to internal type.
     * @return  This object's value cast to internal type.
     */
    operator const chain::block&() const;

    /**
     * Get the block as a string.
     * @return hex string of block.
     */
    std::string to_string() const;

    /**
     * Overload stream in. Throws if input is invalid.
     * @param[in]   input     The input stream to read the value from.
     * @param[out]  argument  The object to receive the read value.
     * @return                The input stream reference.
     */
    friend std::istream& operator>>(std::istream& input, block& argument);

    /**
     * Overload stream out.
     * @param[in]   output    The output stream to write the value to.
     * @param[out]  argument  The object from which to obtain the value.
     * @return                The output stream reference.
     */
    friend std::ostream& operator<<(std::ostream& output,
        const block& argument);

private:

    /**
     * The state of this object's block data.
     */
    chain::block value_;
};

} // namespace config
} // namespace system
} // namespace libbitcoin

#endif
