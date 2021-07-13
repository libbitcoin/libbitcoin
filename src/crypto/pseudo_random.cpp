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
#include <bitcoin/system/crypto/pseudo_random.hpp>

#include <chrono>
#include <cstdint>
#include <limits>
#include <random>
#include <boost/thread.hpp>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/data/data.hpp>

namespace libbitcoin {
namespace system {

using namespace std::chrono;

// DO NOT USE srand() and rand() on MSVC as srand must be called per thread.
// Values may or may not be truly random depending on the underlying device.

void pseudo_random::fill(data_chunk& out)
{
    std::transform(out.begin(), out.end(), out.begin(), [](uint8_t)
    {
        return next();
    });
}

uint8_t pseudo_random::next()
{
    return next(std::numeric_limits<uint8_t>::min(),
        std::numeric_limits<uint8_t>::max());
}

uint8_t pseudo_random::next(uint8_t begin, uint8_t end)
{
    std::uniform_int_distribution<uint16_t> distribution(begin, end);
    return static_cast<uint8_t>(distribution(get_twister()));
}

std::mt19937& pseudo_random::get_twister()
{
    // Boost.thread will clean up the thread statics using this function.
    const auto deleter = [](std::mt19937* twister)
    {
        delete twister;
    };

    // Maintain thread static state space.
    static boost::thread_specific_ptr<std::mt19937> twister(deleter);

    // Use the clock for seeding.
    const auto get_clock_seed = []()
    {
        const auto now = high_resolution_clock::now();
        return static_cast<uint32_t>(now.time_since_epoch().count());
    };

    // This is thread safe because the instance is thread static.
    if (twister.get() == nullptr)
    {
        // Seed with high resolution clock.
        twister.reset(new std::mt19937(get_clock_seed()));
    }

    // The instance remains in scope and is deleted by thread_specific_ptr
    // when the thread terminates, so dereferencing the instance is safe.
    return *twister;
}

// Randomly select a time duration in the range:
// [(expiration - expiration / ratio) .. expiration]
// Not fully testable due to lack of random engine injection.
steady_clock::duration pseudo_random::duration(
    const steady_clock::duration& expiration, uint8_t ratio)
{
    if (is_zero(ratio))
        return expiration;

    // Uses milliseconds level resolution.
    const auto max_expire = duration_cast<milliseconds>(expiration).count();

    // [10 secs, 4] => 10000 / 4 => 2500
    const auto limit = max_expire / ratio;

    if (is_zero(limit))
        return expiration;

    // [0..2^64) % 2500 => [0..2500]
    const auto random_offset = pseudo_random::next<uint64_t>(zero, limit);

    // (10000 - [0..2500]) => [7500..10000]
    const auto expires = max_expire - random_offset;

    // [7.5..10] second duration.
    return milliseconds(expires);
}

} // namespace system
} // namespace libbitcoin
