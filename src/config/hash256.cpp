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
#include <bitcoin/system/config/hash256.hpp>

#include <sstream>
#include <string>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/exceptions.hpp>
#include <bitcoin/system/radix/radix.hpp>

namespace libbitcoin {
namespace system {
namespace config {

hash256::hash256()
  : value_(null_hash)
{
}

hash256::hash256(const std::string& hexcode)
  : hash256()
{
    std::stringstream(hexcode) >> *this;
}

hash256::hash256(const hash_digest& value)
  : value_(value)
{
}

hash256::hash256(const hash256& other)
  : hash256(other.value_)
{
}

std::string hash256::to_string() const
{
    std::stringstream value;
    value << *this;
    return value.str();
}

hash256::operator const hash_digest&() const
{
    return value_;
}

std::istream& operator>>(std::istream& input, hash256& argument)
{
    std::string hexcode;
    input >> hexcode;

    if (!decode_hash(argument.value_, hexcode))
        throw istream_exception(hexcode);

    return input;
}

std::ostream& operator<<(std::ostream& output, const hash256& argument)
{
    output << encode_hash(argument.value_);
    return output;
}

} // namespace config
} // namespace system
} // namespace libbitcoin
