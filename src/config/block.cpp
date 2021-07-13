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
#include <bitcoin/system/config/block.hpp>

#include <sstream>
#include <string>
#include <bitcoin/system/chain/block.hpp>
#include <bitcoin/system/config/base16.hpp>
#include <bitcoin/system/exceptions.hpp>

namespace libbitcoin {
namespace system {
namespace config {

block::block()
  : value_()
{
}

block::block(const std::string& hexcode)
  : value_()
{
    std::stringstream(hexcode) >> *this;
}

block::block(const chain::block& value)
  : value_(value)
{
}

block::block(const block& other)
  : block(other.value_)
{
}

block& block::operator=(const block& other)
{
    value_ = chain::block(other.value_);
    return *this;
}

block& block::operator=(chain::block&& other)
{
    value_ = std::move(other);
    return *this;
}

bool block::operator==(const block& other) const
{
    return value_ == other.value_;
}

block::operator const chain::block&() const
{
    return value_;
}

std::string block::to_string() const
{
    std::stringstream value;
    value << *this;
    return value.str();
}

std::istream& operator>>(std::istream& input, block& argument)
{
    std::string hexcode;
    input >> hexcode;

    if (!argument.value_.from_data(base16(hexcode)))
        throw istream_exception(hexcode);
    return input;
}

std::ostream& operator<<(std::ostream& output, const block& argument)
{
    const auto bytes = argument.value_.to_data();

    output << base16(bytes);
    return output;
}

} // namespace config
} // namespace system
} // namespace libbitcoin
