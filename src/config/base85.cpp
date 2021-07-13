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
#include <bitcoin/system/config/base85.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <bitcoin/system/exceptions.hpp>
#include <bitcoin/system/radix/base_85.hpp>

namespace libbitcoin {
namespace system {
namespace config {

base85::base85()
  : value_()
{
}

base85::base85(const std::string& base85)
{
    std::stringstream(base85) >> *this;
}

base85::base85(const data_chunk& value)
  : value_(value)
{
}

base85::base85(const base85& other)
  : base85(other.value_)
{
}

base85::operator bool() const
{
    return (value_.size() % 4) == 0u;
}

base85::operator const data_chunk&() const
{
    return value_;
}

std::string base85::to_string() const
{
    std::stringstream value;
    value << *this;
    return value.str();
}

std::istream& operator>>(std::istream& input, base85& argument)
{
    std::string base85;
    input >> base85;

    data_chunk out_value;
    if (!decode_base85(out_value, base85) || (out_value.size() % 4) != 0u)
        throw istream_exception(base85);

    argument.value_ = out_value;
    return input;
}

std::ostream& operator<<(std::ostream& output, const base85& argument)
{
    std::string decoded;

    // Base85 requires four byte alignment.
    if (!encode_base85(decoded, argument.value_))
    {
        using namespace boost::program_options;
        BOOST_THROW_EXCEPTION(std::iostream::failure(decoded));
    }

    output << decoded;
    return output;
}

} // namespace config
} // namespace system
} // namespace libbitcoin
