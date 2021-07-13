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
#ifndef LIBBITCOIN_SYSTEM_UNICODE_NORMALIZATION_HPP
#define LIBBITCOIN_SYSTEM_UNICODE_NORMALIZATION_HPP

#include <string>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {

/// Change case of text, complies with section 3.13 of Unicode Standard.
BC_API bool to_lower(std::string& value);
BC_API bool to_upper(std::string& value);

/// Convert text to Unicode normal form (Unicode Standard).
/// Failure indicated by empty string return for non-empty value.
BC_API bool to_canonical_composition(std::string& value);
BC_API bool to_canonical_decomposition(std::string& value);
BC_API bool to_compatibility_composition(std::string& value);
BC_API bool to_compatibility_decomposition(std::string& value);

/// Character tests.
BC_API bool is_unicode(char32_t point);
BC_API bool is_separator(char32_t point);
BC_API bool is_whitespace(char32_t point);
BC_API bool is_combining(char32_t point);
BC_API bool is_diacritic(char32_t point);
BC_API bool is_chinese_japanese_or_korean(char32_t point);

/// Remove combining class characters.
BC_API std::string to_non_combining_form(const std::string& value);

/// Remove diacritic characters.
BC_API std::string to_non_diacritic_form(const std::string& value);

/// Compress ascii whitespace and remove ascii spaces between cjk characters.
BC_API std::string to_compressed_form(const std::string& value);

} // namespace system
} // namespace libbitcoin

#endif
