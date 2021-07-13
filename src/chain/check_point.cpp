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
#include <bitcoin/system/chain/check_point.hpp>

#include <cstddef>
#include <iostream>
#include <string>
#include <utility>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/radix/radix.hpp>
#include <bitcoin/system/serial/serial.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace chain {

check_point::check_point() noexcept
  : check_point(null_hash, zero, false)
{
}

check_point::check_point(check_point&& other) noexcept
  : check_point(std::move(other.hash_), other.height_, other.valid_)
{
}

check_point::check_point(const check_point& other) noexcept
  : check_point(other.hash_, other.height_, other.valid_)
{
}

check_point::check_point(hash_digest&& hash, size_t height) noexcept
  : check_point(std::move(hash), height, true)
{
}

check_point::check_point(const hash_digest& hash, size_t height) noexcept
  : check_point(hash, height, true)
{
}

check_point::check_point(const std::string& hash, size_t height) noexcept
  : check_point(from_string(hash, height))
{
}

// private
check_point::check_point(hash_digest&& hash, size_t height,
    bool valid) noexcept
  : valid_(valid), hash_(std::move(hash)), height_(height)
{
}

// private
check_point::check_point(const hash_digest& hash, size_t height,
    bool valid) noexcept
  : valid_(valid), hash_(hash), height_(height)
{
}

// private
check_point check_point::from_string(const std::string& hash,
    size_t height) noexcept
{
    hash_digest digest;
    if (!decode_hash(digest, hash))
        return {};
    
    return { std::move(digest), height, true };
}

size_t check_point::height() const noexcept
{
    return height_;
}

const hash_digest& check_point::hash() const noexcept
{
    return hash_;
}

bool check_point::is_valid() const noexcept
{
    return valid_;
}

std::string check_point::to_string() const noexcept
{
    return encode_hash(hash_) + ":" + serialize(height_);
}

check_point& check_point::operator=(check_point&& other) noexcept
{
    hash_ = std::move(other.hash_);
    height_ = other.height_;
    return *this;
}

check_point& check_point::operator=(const check_point& other) noexcept
{
    hash_ = other.hash_;
    height_ = other.height_;
    return *this;
}

bool operator<(const check_point& left, const check_point& right) noexcept
{
    // Only heights compared, for sorting.
    return left.height() < right.height();
}

bool operator==(const check_point& left, const check_point& right) noexcept
{
    // Validity not compared.
    return left.hash() == right.hash() && left.height() == right.height();
}

bool operator!=(const check_point& left, const check_point& right) noexcept
{
    return !(left == right);
}

std::ostream& operator<<(std::ostream& output, const check_point& in) noexcept
{
    output << in.to_string();
    return output;
}

std::istream& operator>>(std::istream& input, check_point& out) noexcept
{
    std::string value;
    input >> value;

    hash_digest hash{};
    size_t height(zero);
    const auto tokens = split(value, ":");

    if (tokens.size() == two &&
        decode_hash(hash, tokens.front()) &&
        deserialize(height, tokens.back()))
    {
        out = { hash, height };
        return stream_result(input, true);
    }

    out = {};
    return stream_result(input, false);
}

} // namespace chain
} // namespace system
} // namespace libbitcoin
