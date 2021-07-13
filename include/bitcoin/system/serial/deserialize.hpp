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
#ifndef LIBBITCOIN_SYSTEM_SERIAL_DESERIALIZE_HPP
#define LIBBITCOIN_SYSTEM_SERIAL_DESERIALIZE_HPP

#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <bitcoin/system/data/data.hpp>

namespace libbitcoin {
namespace system {

/// Trim and convert text to the Value type.
/// Value may be any non-specialized deserializable type (implements >>).
/// The data_slice type is not deserializable (cannot hold data).

/// Deserialize to untrimmed/unsplit text, call deserialize(Value, string).
/// Stream configuration is not honored for value conversion from text.
template <typename Value>
bool deserialize(Value& out, std::istream& input);

/// Specialize template to avoid trimming of string deserialization.
/// Strings are pass through unchanged, this provides type generalization.
inline bool deserialize(std::string& out, const std::string& text);

/// Specialize template because basic_ostream treats uint8_t as char.
inline bool deserialize(uint8_t& out, const std::string& text);

/// Specialize data_array to base16 (avoids split).
template <size_t Size>
bool deserialize(data_array<Size>& out, const std::string& text);

/// Specialize data_chunk to base16 (avoids split).
inline bool deserialize(data_chunk& out, const std::string& text);

/// Specialize arrays to add space delimiter.
template <typename Value, size_t Size>
bool deserialize(std::array<Value, Size>& out, const std::string& text);

/// Specialize arrays to add space delimiter.
template <typename Value>
bool deserialize(std::vector<Value>& out, const std::string& text);

/// General deserializer.
template <typename Value>
bool deserialize(Value& out, const std::string& text);

} // namespace system
} // namespace libbitcoin

#include <bitcoin/system/impl/serial/deserialize.ipp>

#endif
