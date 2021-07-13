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
#include <bitcoin/system/config/base2.hpp>

#include <sstream>
#include <string>
#include <bitcoin/system/exceptions.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace config {

base2::base2()
{
}

base2::base2(const std::string& binary)
{
    std::stringstream(binary) >> *this;
}

base2::base2(const binary& value)
  : value_(value)
{
}

base2::base2(const base2& other)
  : base2(other.value_)
{
}

size_t base2::size() const
{
    return value_.bits();
}

base2::operator const binary&() const
{
    return value_;
}

std::istream& operator>>(std::istream& input, base2& argument)
{
    std::string binary;
    input >> binary;

    if (!binary::is_base2(binary))
        throw istream_exception(binary);

    std::stringstream(binary) >> argument.value_;
    return input;
}

std::ostream& operator<<(std::ostream& output, const base2& argument)
{
    output << argument.value_;
    return output;
}

} // namespace config
} // namespace system
} // namespace libbitcoin
