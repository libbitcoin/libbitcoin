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
#include <bitcoin/system/config/point.hpp>

#include <sstream>
#include <string>
#include <utility>
#include <bitcoin/system/chain/chain.hpp>
#include <bitcoin/system/config/hash256.hpp>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/exceptions.hpp>
#include <bitcoin/system/serial/serial.hpp>

namespace libbitcoin {
namespace system {
namespace config {

using namespace boost::program_options;

const std::string point::delimiter = ":";

// Point format is currently private to bx.
static bool decode_point(chain::output_point& point, const std::string& tuple)
{
    uint32_t index;
    const auto tokens = split(tuple, point::delimiter);
    if (tokens.size() != 2 || !deserialize(index, tokens[1]))
        return false;

    // Validate and deserialize the transaction hash.
    const hash256 digest(tokens[0]);
    const hash_digest& hash = digest;
    hash_digest copy;

    // Copy the input point values.
    std::copy(hash.begin(), hash.end(), copy.begin());
    point.set_hash(std::move(copy));
    point.set_index(index);
    return true;
}

// Point format is currently private to bx.
static std::string encode_point(const chain::output_point& point)
{
    std::stringstream result;
    result << hash256(point.hash()) << point::delimiter << point.index();
    return result.str();
}

point::point()
  : value_()
{
}

point::point(const std::string& tuple)
{
    std::stringstream(tuple) >> *this;
}

point::point(const chain::output_point& value)
  : value_(value)
{
}

point::point(const point& other)
  : point(other.value_)
{
}

point::operator const chain::output_point&() const
{
    return value_;
}

std::istream& operator>>(std::istream& input, point& argument)
{
    std::string tuple;
    input >> tuple;

    if (!decode_point(argument.value_, tuple))
        throw istream_exception(tuple);

    return input;
}

std::ostream& operator<<(std::ostream& output, const point& argument)
{
    output << encode_point(argument.value_);
    return output;
}

} // namespace config
} // namespace system
} // namespace libbitcoin
