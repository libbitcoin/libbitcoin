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
#ifndef LIBBITCOIN_SYSTEM_OPTIONAL_HPP
#define LIBBITCOIN_SYSTEM_OPTIONAL_HPP

#include <cstddef>
#include <cstdint>
#include <boost/optional.hpp>

namespace libbitcoin {
namespace system {

// The default (unset) value for boost::optional types.
const auto none = boost::none;

template <typename Type>
using optional = boost::optional<Type>;

typedef optional<bool> optional_flag;
typedef optional<size_t> optional_size;
typedef optional<uint64_t> optional64;
typedef optional<uint32_t> optional32;
typedef optional<uint16_t> optional16;
typedef optional<uint8_t> optional8;

// TODO: test.
template <typename Type>
inline bool is_none(optional<Type> value)
{
    return value == none;
}

} // namespace system
} // namespace libbitcoin

#endif
