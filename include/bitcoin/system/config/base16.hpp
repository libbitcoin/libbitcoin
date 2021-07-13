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
#ifndef LIBBITCOIN_SYSTEM_CONFIG_BASE16_HPP
#define LIBBITCOIN_SYSTEM_CONFIG_BASE16_HPP

#include <array>
#include <iostream>
#include <string>
#include <cstdint>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {
namespace config {

/**
 * Serialization helper for base16 encoded data.
 */
class BC_API base16
{
public:

    /**
     * Default constructor.
     */
    base16();

    /**
     * Initialization constructor.
     * @param[in]  hexcode  The value to initialize with.
     */
    base16(const std::string& hexcode);

    /**
     * Initialization constructor.
     * @param[in]  value  The value to initialize with.
     */
    base16(const data_chunk& value);

    /**
     * Initialization constructor.
     * @param[in]  value  The value to initialize with.
     */
    template<size_t Size>
    base16(const data_array<Size>& value)
      : value_(value.begin(), value.end())
    {
    }

    /**
     * Copy constructor.
     * @param[in]  other  The object to copy into self on construct.
     */
    base16(const base16& other);

    /**
     * Overload cast to internal type.
     * @return  This object's value cast to internal type reference.
     */
    operator const data_chunk&() const;

    /**
     * Overload stream in. If input is invalid sets no bytes in argument.
     * @param[in]   input     The input stream to read the value from.
     * @param[out]  argument  The object to receive the read value.
     * @return                The input stream reference.
     */
    friend std::istream& operator>>(std::istream& input,
        base16& argument);

    /**
     * Overload stream out.
     * @param[in]   output    The output stream to write the value to.
     * @param[out]  argument  The object from which to obtain the value.
     * @return                The output stream reference.
     */
    friend std::ostream& operator<<(std::ostream& output,
        const base16& argument);

private:

    /**
     * The state of this object.
     */
    data_chunk value_;
};

} // namespace config
} // namespace system
} // namespace libbitcoin

#endif
