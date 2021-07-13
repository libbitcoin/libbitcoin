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
#ifndef LIBBITCOIN_SYSTEM_RADIX_BASE_64_HPP
#define LIBBITCOIN_SYSTEM_RADIX_BASE_64_HPP

#include <string>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {

/**
 * Encode data as base64.
 * @return the base64 encoded string.
 */
BC_API std::string encode_base64(const data_slice& unencoded);

/**
 * Attempt to decode base64 data.
 * @return false if the input contains non-base64 characters.
 */
BC_API bool decode_base64(data_chunk& out, const std::string& in);

// TODO: en.cppreference.com/w/cpp/language/user_literal

} // namespace system
} // namespace libbitcoin

#endif

