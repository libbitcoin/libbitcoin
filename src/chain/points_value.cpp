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
#include <numeric>
#include <cstdint>
#include <bitcoin/system/chain/point_value.hpp>
#include <bitcoin/system/chain/points_value.hpp>
#include <bitcoin/system/math/safe.hpp>

namespace libbitcoin {
namespace system {
namespace chain {

// static
// ----------------------------------------------------------------------------

void points_value::greedy(points_value& out, const points_value& unspent,
    uint64_t minimum_value)
{
    out.points.clear();

    // The minimum required value does not exist.
    if (unspent.value() < minimum_value)
        return;

    // Optimization for simple case not requiring search.
    if (unspent.points.size() == 1)
    {
        out.points.push_back(unspent.points.front());
        return;
    }

    // Copy the points list for safe manipulation.
    auto points = unspent.points;

    const auto below = [minimum_value](const point_value& point)
    {
        return point.value() < minimum_value;
    };

    const auto lesser = [](const point_value& left, const point_value& right)
    {
        return left.value() < right.value();
    };

    const auto greater = [](const point_value& left, const point_value& right)
    {
        return left.value() > right.value();
    };

    // Reorder list between values that exceed minimum and those that do not.
    const auto sufficient = std::partition(points.begin(), points.end(), below);

    // If there are values large enough, return the smallest (of the largest).
    const auto minimum = std::min_element(sufficient, points.end(), lesser);

    if (minimum != points.end())
    {
        out.points.push_back(*minimum);
        return;
    }

    // Sort all by descending value in order to use the fewest inputs possible.
    std::sort(points.begin(), points.end(), greater);

    // This is naive, will not necessarily find the smallest combination.
    for (const auto& point: points)
    {
        out.points.push_back(point);

        if (out.value() >= minimum_value)
            return;
    }
}

void points_value::individual(points_value& out, const points_value& unspent,
    uint64_t minimum_value)
{
    out.points.clear();
    out.points.reserve(unspent.points.size());

    // Select all individual points that satisfy the minimum.
    for (const auto& point: unspent.points)
        if (point.value() >= minimum_value)
            out.points.push_back(point);

    out.points.shrink_to_fit();

    const auto lesser = [](const point_value& left, const point_value& right)
    {
        return left.value() < right.value();
    };

    // Return in ascending order by value.
    std::sort(out.points.begin(), out.points.end(), lesser);
}

void points_value::select(points_value& out, const points_value& unspent,
    uint64_t minimum_value, selection option)
{
    switch (option)
    {
        case selection::individual:
            individual(out, unspent, minimum_value);
            break;
        case selection::greedy:
        default:
            greedy(out, unspent, minimum_value);
            break;
    }
}

// public
// ----------------------------------------------------------------------------

uint64_t points_value::value() const
{
    const auto sum = [](uint64_t total, const point_value& point)
    {
        return safe_add(total, point.value());
    };

    return std::accumulate(points.begin(), points.end(), uint64_t(0), sum);
}

} // namespace chain
} // namespace system
} // namespace libbitcoin
