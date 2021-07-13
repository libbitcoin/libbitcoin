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
#ifndef LIBBITCOIN_SYSTEM_SERIAL_SERIALIZE_HPP
#define LIBBITCOIN_SYSTEM_SERIAL_SERIALIZE_HPP

#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <bitcoin/system/data/data.hpp>

namespace libbitcoin {
namespace system {

/// Convert specified Value to text, return fallback for empty results.
/// Value may be any non-specialized serializable type (implements <<).
/// Any type that casts to a specialization will be specialized.
/// The data_slice must either be explicitly sized to array serialize or
/// it will deserialize as a data_chunk, due to its cast operators. Both work
/// though its array serialization can truncate or zero-pad the output by Size.

/// Call serialize(Value, fallback) and send to ostream.
/// Stream configuration is not honored for value conversion to text.
template <typename Value>
void serialize(std::ostream& output, const Value& value,
    const std::string& fallback);

/// Specialize template because basic_ostream treats uint8_t as char.
inline std::string serialize(uint8_t value, const std::string& fallback="");

/// Specialize data_array to base16 (avoids join and fallback).
template <size_t Size>
std::string serialize(const data_array<Size>& value,
    const std::string& fallback="");

/// Specialize data_chunk to base16 (avoids join and fallback).
inline std::string serialize(const data_chunk& value,
    const std::string& fallback="");

/// Specialize arrays to add fallback and join with space delimiter.
template <typename Value, size_t Size>
std::string serialize(const std::array<Value, Size>& values,
    const std::string& fallback="?");

/// Specialize arrays to add fallback and join with space delimiter.
template <typename Value>
std::string serialize(const std::vector<Value>& values,
    const std::string& fallback="?");

/// General serializer.
template <typename Value>
std::string serialize(const Value& value, const std::string& fallback="");

} // namespace system
} // namespace libbitcoin

#include <bitcoin/system/impl/serial/serialize.ipp>

#endif
