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
#ifndef LIBBITCOIN_SYSTEM_CONFIG_POINT_HPP
#define LIBBITCOIN_SYSTEM_CONFIG_POINT_HPP

#include <iostream>
#include <string>
#include <bitcoin/system/chain/chain.hpp>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {
namespace config {

/**
 * Serialization helper to convert between text and an output_point.
 */
class BC_API point
{
public:
    static const std::string delimiter;

    /**
     * Default constructor.
     */
    point();

    /**
     * Initialization constructor.
     * @param[in]  tuple  The value to initialize with.
     */
    point(const std::string& tuple);

    /**
     * Initialization constructor.
     * @param[in]  value  The value to initialize with.
     */
    point(const chain::output_point& value);

    /**
     * Copy constructor.
     * @param[in]  other  The object to copy into self on construct.
     */
    point(const point& other);

    /**
     * Overload cast to internal type.
     * @return  This object's value cast to internal type.
     */
    operator const chain::output_point&() const;

    /**
     * Overload stream in. Throws if input is invalid.
     * @param[in]   input     The input stream to read the value from.
     * @param[out]  argument  The object to receive the read value.
     * @return                The input stream reference.
     */
    friend std::istream& operator>>(std::istream& input, point& argument);

    /**
     * Overload stream out.
     * @param[in]   output    The output stream to write the value to.
     * @param[out]  argument  The object from which to obtain the value.
     * @return                The output stream reference.
     */
    friend std::ostream& operator<<(std::ostream& output,
        const point& argument);

private:

    /**
     * The state of this object.
     */
    chain::output_point value_;
};

} // namespace config
} // namespace system
} // namespace libbitcoin

#endif
