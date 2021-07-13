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
#ifndef LIBBITCOIN_SYSTEM_STREAM_BINARY_HPP
#define LIBBITCOIN_SYSTEM_STREAM_BINARY_HPP

#include <cstddef>
#include <functional>
#include <iostream>
#include <string>
#include <bitcoin/system/constraints.hpp>
#include <bitcoin/system/data/data_chunk.hpp>
#include <bitcoin/system/data/data_slice.hpp>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {

/// Constant class for bit vector serialization and comparison.
class BC_API binary
{
public:
    /// True if all characters are '0' or '1'.
    static bool is_base2(const std::string& text) noexcept;

    /// Constructors.
    binary() noexcept;
    binary(binary&& other) noexcept;
    binary(const binary& other) noexcept;
    binary(const std::string& bits) noexcept;
    binary(size_t bits, const data_slice& data) noexcept;

    /// Methods.
    std::string encoded() const noexcept;
    const data_chunk& data() const noexcept;
    size_t bytes() const noexcept;
    size_t bits() const noexcept;

    /// Operators.
    operator const data_chunk&() const noexcept;
    bool operator[](size_t index) const noexcept;
    bool operator<(const binary& other) const noexcept;
    binary& operator=(binary&& other) noexcept;
    binary& operator=(const binary& other) noexcept;

private:
    binary(data_chunk&& bytes, size_t bits) noexcept;
    static binary from_string(const std::string bits) noexcept;
    static binary from_data(size_t size, data_chunk&& bytes) noexcept;

    size_t bits_;
    data_chunk bytes_;
};

bool operator==(const binary& left, const binary& right) noexcept;
bool operator!=(const binary& left, const binary& right) noexcept;
std::istream& operator>>(std::istream& in, binary& to) noexcept;
std::ostream& operator<<(std::ostream& out, const binary& of) noexcept;

} // namespace system
} // namespace libbitcoin

namespace std
{

template<>
struct hash<bc::system::binary>
{
    size_t operator()(const bc::system::binary& value) const
    {
        return std::hash<std::string>()(value.encoded());
    }
};

} // namespace std

#endif
