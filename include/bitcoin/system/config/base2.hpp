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
#ifndef LIBBITCOIN_SYSTEM_CONFIG_BASE2_HPP
#define LIBBITCOIN_SYSTEM_CONFIG_BASE2_HPP

#include <cstddef>
#include <iostream>
#include <string>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace config {

/**
 * Serialization helper for base2 encoded data.
 */
class BC_API base2
{
public:

    /**
     * Default constructor.
     */
    base2();

    /**
     * Initialization constructor.
     * @param[in]  bin  The value to initialize with.
     */
    base2(const std::string& binary);

    /**
     * @param[in]  value  The value to initialize with.
     */
    base2(const binary& value);

    /**
     * Copy constructor.
     * @param[in]  other  The object to copy into self on construct.
     */
    base2(const base2& other);

    /**
     * Get number of bits in value.
     */
    size_t size() const;

    /**
     * Overload cast to internal type.
     * @return  This object's value cast to internal type reference.
     */
    operator const binary&() const;

    /**
     * Overload stream in. If input is invalid sets no bytes in argument.
     * @param[in]   input     The input stream to read the value from.
     * @param[out]  argument  The object to receive the read value.
     * @return                The input stream reference.
     */
    friend std::istream& operator>>(std::istream& input,
        base2& argument);

    /**
     * Overload stream out.
     * @param[in]   output    The output stream to write the value to.
     * @param[out]  argument  The object from which to obtain the value.
     * @return                The output stream reference.
     */
    friend std::ostream& operator<<(std::ostream& output,
        const base2& argument);

private:

    /**
     * The state of this object.
     */
    binary value_;
};

} // namespace config
} // namespace system
} // namespace libbitcoin

#endif
