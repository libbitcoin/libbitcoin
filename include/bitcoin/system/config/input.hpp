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
#ifndef LIBBITCOIN_SYSTEM_CONFIG_INPUT_HPP
#define LIBBITCOIN_SYSTEM_CONFIG_INPUT_HPP

#include <iostream>
#include <string>
#include <bitcoin/system/chain/chain.hpp>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {
namespace config {

/**
 * Serialization helper stub for chain::input.
 */
class BC_API input
{
public:

    /**
     * Default constructor.
     */
    input();

    /**
     * Initialization constructor.
     * @param[in]  tuple  The value to initialize with.
     */
    input(const std::string& tuple);

    /**
     * Initialization constructor. Only the point is retained.
     * @param[in]  value  The value to initialize with.
     */
    input(const chain::input& value);

    /**
     * Copy constructor.
     * @param[in]  other  The object to copy into self on construct.
     */
    input(const input& other);

    /**
     * Initialization constructor. Aspects of the input other than the point
     * are defaulted.
     * @param[in]  value  The value to initialize with.
     */
    input(const chain::input_point& value);

    /**
     * Overload cast to internal type.
     * @return  This object's value cast to internal type.
     */
    operator const chain::input&() const;

    /**
     * Overload stream in. Throws if input is invalid.
     * @param[in]   input     The input stream to read the value from.
     * @param[out]  argument  The object to receive the read value.
     * @return                The input stream reference.
     */
    friend std::istream& operator>>(std::istream& stream, input& argument);

    /**
     * Overload stream out.
     * @param[in]   output    The output stream to write the value to.
     * @param[out]  argument  The object from which to obtain the value.
     * @return                The output stream reference.
     */
    friend std::ostream& operator<<(std::ostream& output,
        const input& argument);

private:

    /**
     * The state of this object.
     */
    chain::input value_;
};

} // namespace explorer
} // namespace system
} // namespace libbitcoin

#endif
