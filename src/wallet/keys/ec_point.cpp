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
#include <bitcoin/system/wallet/keys/ec_point.hpp>

#include <cstdint>
#include <utility>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/radix/radix.hpp>

namespace libbitcoin {
namespace system {

// TODO: move to elliptic_curve
const uint8_t ec_point::invalid = 0x00;
const uint8_t ec_point::compressed_even = 0x02;
const uint8_t ec_point::compressed_odd = 0x03;
const uint8_t ec_point::uncompressed = 0x04;
const ec_point ec_point::generator{ ec_compressed_generator };

// private
bool ec_point::is_valid() const
{
    return point_.front() != invalid;
}

// construction
// ----------------------------------------------------------------------------

ec_point::ec_point()
  : point_(null_ec_compressed)
{
}

ec_point::ec_point(ec_point&& point)
  : point_(std::move(point.point_))
{
}

ec_point::ec_point(const ec_point& point)
  : point_(point.point_)
{
}

ec_point::ec_point(ec_compressed&& compressed)
  : point_(std::move(compressed))
{
}

ec_point::ec_point(const ec_compressed& compressed)
  : point_(compressed)
{
}

// assignment operators
// ----------------------------------------------------------------------------

ec_point& ec_point::operator=(ec_point&& point)
{
    if (&point == this)
        return *this;

    point_ = std::move(point.point_);
    return *this;
}

ec_point& ec_point::operator=(const ec_point& point)
{
    if (&point == this)
        return *this;

    point_ = point.point_;
    return *this;
}

ec_point& ec_point::operator=(ec_compressed&& compressed)
{
    point_ = std::move(compressed);
    return *this;
}

ec_point& ec_point::operator=(const ec_compressed& compressed)
{
    point_ = compressed;
    return *this;
}

// arithmetic assignment operators
// ----------------------------------------------------------------------------

ec_point& ec_point::operator+=(const ec_point& point)
{
    if (!is_valid())
        return *this;

    *this = (*this + point);
    return *this;
}

ec_point& ec_point::operator-=(const ec_point& point)
{
    if (!is_valid())
        return *this;

    *this = (*this - point);
    return *this;
}

ec_point& ec_point::operator*=(const ec_scalar& point)
{
    if (!is_valid())
        return *this;

    *this = (*this * point);
    return *this;
}

// unary operators (const)
// ----------------------------------------------------------------------------

ec_point ec_point::operator-() const
{
    if (!is_valid())
        return {};

    auto out = point_;
    if (!ec_negate(out))
        return {};
    
    return ec_point{ out };
}

// binary math operators (const)
// ----------------------------------------------------------------------------

ec_point operator+(const ec_point& left, const ec_point& right)
{
    if (!left || !right)
        return {};

    ec_compressed out = left.point();
    if (!ec_add(out, right.point()))
        return {};
    
    return ec_point{ out };
}

ec_point operator-(const ec_point& left, const ec_point& right)
{
    if (!left || !right)
        return {};

    return left + -right;
}

ec_point operator*(const ec_point& left, const ec_scalar& right)
{
    if (!left || !right)
        return {};

    auto out = left.point();
    if (!ec_multiply(out, right.secret()))
        return {};
    
    return ec_point{ out };
}

ec_point operator*(const ec_scalar& left, const ec_point& right)
{
    return right * left;
}

// comparison operators (const)
// ----------------------------------------------------------------------------

bool operator==(const ec_point& left, const ec_point& right)
{
    return left.point() == right.point();
}

bool operator!=(const ec_point& left, const ec_point& right)
{
    return !(left == right);
}

// cast operators
// ----------------------------------------------------------------------------

ec_point::operator bool() const
{
    return is_valid();
}

ec_point::operator const ec_compressed&() const
{
    return point_;
}

// properties
// ----------------------------------------------------------------------------

const ec_compressed& ec_point::point() const
{
    return point_;
}

} // namespace system
} // namespace libbitcoin
