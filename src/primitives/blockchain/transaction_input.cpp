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
#include <bitcoin/bitcoin/primitives/blockchain/transaction_input.hpp>
#include <sstream>

namespace libbitcoin {

transaction_input_type::transaction_input_type()
{
}

transaction_input_type::transaction_input_type(const data_chunk& value)
: transaction_input_type(value.begin(), value.end())
{
}

transaction_input_type::operator const data_chunk() const
{
    data_chunk result(satoshi_size());
    auto serial = make_serializer(result.begin());

    serial.write_hash(previous_output.hash);
    serial.write_4_bytes(previous_output.index);
    data_chunk raw_script = script;
    serial.write_variable_uint(raw_script.size());
    serial.write_data(raw_script);
    serial.write_4_bytes(sequence);

    BITCOIN_ASSERT(std::distance(result.begin(), serial.iterator()) == satoshi_size());

    return result;
}

size_t transaction_input_type::satoshi_size() const
{
    size_t script_size = script.satoshi_size();

    return 40 + variable_uint_size(script_size) + script_size;
}

std::string transaction_input_type::to_string() const
{
    std::ostringstream ss;

    ss << previous_output.to_string() << "\n"
        << "\t" << script.to_string() << "\n"
        << "\tsequence = " << sequence << "\n";

    return ss.str();
}

}
