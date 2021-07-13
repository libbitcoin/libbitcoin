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
#include <bitcoin/system/config/input.hpp>

#include <sstream>
#include <string>
#include <bitcoin/system/chain/input.hpp>
#include <bitcoin/system/chain/input_point.hpp>
#include <bitcoin/system/config/point.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/exceptions.hpp>
#include <bitcoin/system/serial/deserialize.hpp>

namespace libbitcoin {
namespace system {
namespace config {

using namespace boost::program_options;

// input is a private encoding in bx.
static bool decode_input(chain::input& input, const std::string& tuple)
{
    const auto tokens = split(tuple, point::delimiter);
    if (tokens.size() != 2 && tokens.size() != 3)
        return false;

    input.set_sequence(chain::max_input_sequence);
    input.set_previous_output(point(tokens[0] + ":" + tokens[1]));

    // TODO: remove stealth inputs.
    if (tokens.size() == 3)
    {
        uint32_t value;
        if (!deserialize(value, tokens[2]))
            return false;

        input.set_sequence(value);
    }

    return true;
}

// input is currently a private encoding in bx.
static std::string encode_input(const chain::input& input)
{
    std::stringstream result;
    result << point(input.previous_output()) << point::delimiter
        << input.sequence();

    return result.str();
}

input::input()
  : value_()
{
}

input::input(const std::string& tuple)
{
    std::stringstream(tuple) >> *this;
}

input::input(const chain::input& value)
  : value_(value)
{
}

input::input(const input& other)
  : input(other.value_)
{
}

input::input(const chain::input_point& value)
  : value_({value, {}, chain::max_input_sequence})
{
}

input::operator const chain::input&() const
{
    return value_;
}

std::istream& operator>>(std::istream& stream, input& argument)
{
    std::string tuple;
    stream >> tuple;

    if (!decode_input(argument.value_, tuple))
        throw istream_exception(tuple);

    return stream;
}

std::ostream& operator<<(std::ostream& output, const input& argument)
{
    output << encode_input(argument.value_);
    return output;
}

} // namespace config
} // namespace system
} // namespace libbitcoin
