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

// Sponsored in part by Digital Contract Design, LLC

#include <bitcoin/system/config/compact_filter.hpp>

#include <sstream>
#include <string>
#include <bitcoin/system/config/base16.hpp>
#include <bitcoin/system/exceptions.hpp>

namespace libbitcoin {
namespace system {
namespace config {

compact_filter::compact_filter()
  : value_()
{
}

compact_filter::compact_filter(const std::string& hexcode)
  : value_()
{
    std::stringstream(hexcode) >> *this;
}

compact_filter::compact_filter(const message::compact_filter& value)
  : value_(value)
{
}

compact_filter::compact_filter(const compact_filter& other)
  : compact_filter(other.value_)
{
}

compact_filter& compact_filter::operator=(const compact_filter& other)
{
    value_ = message::compact_filter(other.value_);
    return *this;
}

compact_filter& compact_filter::operator=(message::compact_filter&& other)
{
    value_ = std::move(other);
    return *this;
}

bool compact_filter::operator==(const compact_filter& other) const
{
    return value_ == other.value_;
}

compact_filter::operator const message::compact_filter&() const
{
    return value_;
}

std::string compact_filter::to_string() const
{
    std::stringstream value;
    value << *this;
    return value.str();
}

std::istream& operator>>(std::istream& input, compact_filter& argument)
{
    std::string hexcode;
    input >> hexcode;

    if (!argument.value_.from_data(base16(hexcode)))
        throw istream_exception(hexcode);

    return input;
}

std::ostream& operator<<(std::ostream& output, const compact_filter& argument)
{
    const auto bytes = argument.value_.to_data();

    output << base16(bytes);
    return output;
}

} // namespace config
} // namespace system
} // namespace libbitcoin
