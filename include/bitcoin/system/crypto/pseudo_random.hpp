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
#ifndef LIBBITCOIN_SYSTEM_CRYPTO_PSEUDO_RANDOM_HPP
#define LIBBITCOIN_SYSTEM_CRYPTO_PSEUDO_RANDOM_HPP

#include <algorithm>
#include <chrono>
#include <random>
#include <cstdint>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/constraints.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {

class BC_API pseudo_random
{
  public:
    /**
     * Fill a byte array with randomness using the default random engine.
     */
    template<size_t Size>
    static void fill(data_array<Size>& out)
    {
        std::transform(out.begin(), out.end(), out.begin(), [](uint8_t)
        {
            return next();
        });
    }

    /**
     * Fill a byte vector with randomness using the default random engine.
     */
    static void fill(data_chunk& out);

    /**
     * Generate a pseudo random number within the uint8_t domain.
     * Specialized: uniform_int_distribution is undefined for sizes < 16 bits.
     * @return  The number.
     */
    static uint8_t next();

    /**
     * Generate a pseudo random number within [begin, end].
     * Specialized: uniform_int_distribution is undefined for sizes < 16 bits.
     * @return  The number.
     */
    static uint8_t next(uint8_t begin, uint8_t end);

    /**
     * Generate a pseudo random number within the Type domain.
     * @return  The number.
     */
    template<typename Type, if_integer<Type> = true>
    static Type next()
    {
        return next(std::numeric_limits<Type>::min(),
            std::numeric_limits<Type>::max());
    }

    /**
     * Generate a pseudo random number within [begin, end].
     * @return  The number.
     */
    template<typename Type, if_integer<Type> = true>
    static Type next(Type begin, Type end)
    {
        std::uniform_int_distribution<Type> distribution(begin, end);
        return distribution(get_twister());
    }

    /**
     * Shuffle a container elements using the random engine.
     */
    template<class Container>
    static void shuffle(Container& out)
    {
        std::shuffle(out.begin(), out.end(), get_twister());
    }

    /**
     * Convert a time duration to a value in the range [max/ratio, max].
     * @param[in]  maximum  The maximum value to return.
     * @param[in]  ratio    The determinant of the minimum duration as the inverse
     *                      portion of the maximum duration.
     * @return              The randomized duration.
     */
    static std::chrono::steady_clock::duration duration(
        const std::chrono::steady_clock::duration& maximum, uint8_t ratio=2);

private:
    static std::mt19937& get_twister();
};

} // namespace system
} // namespace libbitcoin

#endif
