/*
 * Copyright (c) 2011-2015 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <bitcoin/bitcoin/message/verack.hpp>
#include <bitcoin/bitcoin/utility/serializer.hpp>

namespace libbitcoin {
namespace message {

verack::verack()
{
}

verack::verack(std::istream& stream)
{
    if (stream.fail() || !(stream.peek() == std::istream::traits_type::eof()))
        throw std::ios_base::failure("verack");
}

//verack::verack(const data_chunk& value)
//: verack(value.begin(), value.end())
//{
//}

verack::operator const data_chunk() const
{
    data_chunk result(0);
    return result;
}

size_t verack::satoshi_size() const
{
    return verack::satoshi_fixed_size();
}


size_t verack::satoshi_fixed_size()
{
    return 0;
}

} // end message
} // end libbitcoin
