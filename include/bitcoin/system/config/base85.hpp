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
#ifndef LIBBITCOIN_SYSTEM_CONFIG_BASE85_HPP
#define LIBBITCOIN_SYSTEM_CONFIG_BASE85_HPP

#include <iostream>
#include <string>
#include <vector>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {
namespace config {

/**
 * Serialization helper for base58 sodium keys.
 * Base85 requires four byte alignment, sodium keys are 32 bytes.
 */
class BC_API base85
{
public:

    /**
     * Default constructor.
     */
    base85();

    /**
     * Initialization constructor.
     * @param[in]  base85  The value to initialize with.
     */
    base85(const std::string& base85);

    /**
     * Initialization constructor.
     * @param[in]  value  The value to initialize with.
     */
    base85(const data_chunk& value);

    /**
     * Copy constructor.
     * @param[in]  other  The object to copy into self on construct.
     */
    base85(const base85& other);

    /**
     * Getter.
     * @return True if the data size is evenly divisible by 4.
     */
    operator bool() const;

    /**
     * Overload cast to internal type.
     * @return  This object's value cast to internal type.
     */
    operator const data_chunk&() const;

    /**
     * Get the key as a base85 encoded (z85) string.
     * @return The encoded key.
     */
    std::string to_string() const;

    /**
     * Overload stream in. Throws if input is invalid.
     * @param[in]   input     The input stream to read the value from.
     * @param[out]  argument  The object to receive the read value.
     * @return                The input stream reference.
     */
    friend std::istream& operator>>(std::istream& input,
        base85& argument);

    /**
     * Overload stream out.
     * @param[in]   output    The output stream to write the value to.
     * @param[out]  argument  The object from which to obtain the value.
     * @return                The output stream reference.
     */
    friend std::ostream& operator<<(std::ostream& output,
        const base85& argument);

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
