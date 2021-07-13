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
#include <bitcoin/system/config/base64.hpp>

#include <sstream>
#include <string>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/exceptions.hpp>
#include <bitcoin/system/radix/base_64.hpp>

namespace libbitcoin {
namespace system {
namespace config {

base64::base64()
{
}

base64::base64(const std::string& base64)
{
    std::stringstream(base64) >> *this;
}

base64::base64(const data_chunk& value)
  : value_(value)
{
}

base64::base64(const base64& other)
  : base64(other.value_)
{
}

base64::operator const data_chunk&() const
{
    return value_;
}

std::istream& operator>>(std::istream& input, base64& argument)
{
    std::string base64;
    input >> base64;

    if (!decode_base64(argument.value_, base64))
        throw istream_exception(base64);

    return input;
}

std::ostream& operator<<(std::ostream& output, const base64& argument)
{
    output << encode_base64(argument.value_);
    return output;
}

} // namespace config
} // namespace system
} // namespace libbitcoin
