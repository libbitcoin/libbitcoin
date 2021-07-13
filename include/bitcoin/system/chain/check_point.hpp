/**
 * Copyright (c) 2011-2021 libbitcoin developers (see AUTHORS)
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
#ifndef LIBBITCOIN_SYSTEM_CHAIN_CHECK_POINT_HPP
#define LIBBITCOIN_SYSTEM_CHAIN_CHECK_POINT_HPP

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/constraints.hpp>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {
namespace chain {

class BC_API check_point
{
public:
    typedef std::vector<check_point> list;

    check_point() noexcept;
    check_point(check_point&& other) noexcept;
    check_point(const check_point& other) noexcept;

    check_point(hash_digest&& hash, size_t height) noexcept;
    check_point(const hash_digest& hash, size_t height) noexcept;
    explicit check_point(const std::string& hash, size_t height) noexcept;

    template <size_t Size, if_size<Size, add1(two * hash_size)> = true>
    check_point::check_point(const char(&string)[Size], size_t height) noexcept
      : check_point(std::string(string), height)
    {
    }

    size_t height() const noexcept;
    const hash_digest& hash() const noexcept;

    bool is_valid() const noexcept;
    std::string to_string() const noexcept;

    check_point& operator=(check_point&& other) noexcept;
    check_point& operator=(const check_point& other) noexcept;

private:
    check_point(hash_digest&& hash, size_t height, bool valid) noexcept;
    check_point(const hash_digest& hash, size_t height, bool valid) noexcept;
    static check_point from_string(const std::string& hash,
        size_t height) noexcept;

    bool valid_;
    hash_digest hash_;
    size_t height_;
};

bool operator<(const check_point& left, const check_point& right) noexcept;
bool operator==(const check_point& left, const check_point& right) noexcept;
bool operator!=(const check_point& left, const check_point& right) noexcept;
std::ostream& operator<<(std::ostream& output, const check_point& in) noexcept;
std::istream& operator>>(std::istream& input, check_point& out) noexcept;

} // namespace chain
} // namespace system
} // namespace libbitcoin

#endif
