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
#ifndef LIBBITCOIN_SYSTEM_UNICODE_CONVERSION_HPP
#define LIBBITCOIN_SYSTEM_UNICODE_CONVERSION_HPP

#include <string>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/unicode/code_points.hpp>

namespace libbitcoin {
namespace system {

/// UTF8/UTF32 are useful when iterating over Unicode characters.
/// A single char32_t generally maps to a single Unicode code point.
/// However "combining characters" (e.g. diacritics) introduce complications.
/// UTF8/UTF16 are useful when it is necessary to interface with Win32.
/// UTF16/UTF32 are useful when iterating over Unicode characters when
/// interfacing with Win32.

/// Win32 APIs support UTF16 (and formerly UCS2) but not UTF8.
/// All other platforms are presumed to provide UTF8 interfaces.
/// See "utf8_everywhere" in this library for more info on Win32 compatibility.
/// UFT16 is not a single character per wchar_t as sometimes assumed.
/// No char (byte) within a UTF8 character can be misinterpreted as an
/// ASCII character, as all UTF8 bytes are above 0x7f.

typedef std::vector<std::wstring> wstring_list;
typedef std::vector<std::u32string> u32string_list;

BC_API std::string to_utf8(char32_t point);

BC_API std::string to_utf8(const std::wstring& text);
BC_API std::string to_utf8(const std::u32string& text);
BC_API std::wstring to_utf16(const std::string& text);
BC_API std::wstring to_utf16(const std::u32string& text);
BC_API std::u32string to_utf32(const std::string& text);
BC_API std::u32string to_utf32(const std::wstring& text);

BC_API string_list to_utf8(const wstring_list& text);
BC_API string_list to_utf8(const u32string_list& text);
BC_API wstring_list to_utf16(const string_list& text);
BC_API wstring_list to_utf16(const u32string_list& text);
BC_API u32string_list to_utf32(const string_list& text);
BC_API u32string_list to_utf32(const wstring_list& text);

} // namespace system
} // namespace libbitcoin

#endif
