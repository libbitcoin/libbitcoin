/**
 * Copyright (c) 2011-2021 libbitcoin developers (see AUTHORS)
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
#ifndef LIBBITCOIN_SYSTEM_SERIAL_SERIALIZE_IPP
#define LIBBITCOIN_SYSTEM_SERIAL_SERIALIZE_IPP

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/radix/radix.hpp>

namespace libbitcoin {
namespace system {

// Cannot use a data_slice override for data_array/data_chunk because it would
// pick up string and serialize as bytes vs. chars.
    
template <typename Value>
void serialize(std::ostream& output, const Value& value,
    const std::string& fallback)
{
    output << serialize(value, fallback);
}

inline std::string serialize(uint8_t value, const std::string& fallback)
{
    return serialize(static_cast<uint16_t>(value), fallback);
}

template <size_t Size>
std::string serialize(const data_array<Size>& value, const std::string&)
{
    return encode_base16(value);
}

inline std::string serialize(const data_chunk& value, const std::string&)
{
    return encode_base16(value);
}

template <typename Value, size_t Size>
std::string serialize(const std::array<Value, Size>& values,
    const std::string& fallback)
{
    string_list tokens(values.size());
    const auto serializer = [&fallback](const Value& value)
    {
        return serialize(value, fallback);
    };

    std::transform(values.begin(), values.end(), tokens.begin(), serializer);
    return join(tokens);
}

template <typename Value>
std::string serialize(const std::vector<Value>& values,
    const std::string& fallback)
{
    string_list tokens(values.size());
    const auto serializer = [&fallback](const Value& value)
    {
        return serialize(value, fallback);
    };

    std::transform(values.begin(), values.end(), tokens.begin(), serializer);
    return join(tokens);
}

template <typename Value>
std::string serialize(const Value& value, const std::string& fallback)
{
    std::ostringstream ostream;
    ostream << value;
    const auto token = ostream.str();
    return token.empty() ? fallback : token;
}

} // namespace system
} // namespace libbitcoin

#endif
