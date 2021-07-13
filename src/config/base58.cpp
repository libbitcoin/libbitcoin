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
#include <bitcoin/system/config/base58.hpp>

#include <sstream>
#include <string>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/exceptions.hpp>
#include <bitcoin/system/radix/base_58.hpp>

namespace libbitcoin {
namespace system {
namespace config {

base58::base58()
{
}

base58::base58(const std::string& base58)
{
    std::stringstream(base58) >> *this;
}

base58::base58(const data_chunk& value)
  : value_(value)
{
}

base58::base58(const base58& other)
  : base58(other.value_)
{
}

base58::operator const data_chunk&() const
{
    return value_;
}

std::istream& operator>>(std::istream& input, base58& argument)
{
    std::string base58;
    input >> base58;

    if (!decode_base58(argument.value_, base58))
        throw istream_exception(base58);

    return input;
}

std::ostream& operator<<(std::ostream& output, const base58& argument)
{
    output << encode_base58(argument.value_);
    return output;
}

} // namespace config
} // namespace system
} // namespace libbitcoin
