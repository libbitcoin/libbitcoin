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
#ifndef LIBBITCOIN_SYSTEM_UNICODE_ASCII_HPP
#define LIBBITCOIN_SYSTEM_UNICODE_ASCII_HPP

#include <string>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {

/// Character tests.
BC_API bool is_ascii_character(char32_t point);
BC_API bool is_ascii_separator(char32_t point);
BC_API bool is_ascii_whitespace(char32_t point);

/// Convert each ASCII letter in text to lower case.
BC_API std::string ascii_to_lower(const std::string& text);

/// Convert each ASCII letter in text to upper case.
BC_API std::string ascii_to_upper(const std::string& text);

/// True if text has upper and lower ASCII case letters.
BC_API bool has_mixed_ascii_case(const std::string& text);

/// True if all characters are in the ASCII subset of UTF8 [<128].
BC_API bool is_ascii(const std::string& text);

} // namespace system
} // namespace libbitcoin

#endif
