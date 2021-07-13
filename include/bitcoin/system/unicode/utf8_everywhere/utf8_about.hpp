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
#ifndef LIBBITCOIN_SYSTEM_UNICODE_UTF8_EVERYWHERE_UTF8_ABOUT_HPP
#define LIBBITCOIN_SYSTEM_UNICODE_UTF8_EVERYWHERE_UTF8_ABOUT_HPP

// Utf8 Everywhere
//
// Windows and other environments, such as Java, that supported Unicode prior
// to the advent of utf8 utilize 16 bit characters. These are typically encoded
// as wchar_t in C++. Unicode no longer fits in 16 bits and as such these
// implementations now require variable length character encoding just as utf8.
// Windows support for UCS2 was added in Windows NT 3.1. Support for UTF16
// was added in Windows 2000. So its wide characters are also variable length.
// boost.org/doc/libs/1_75_0/libs/locale/doc/html/recommendations_and_myths.html
//
// Libbitcoin embraces the "utf8 everywhere" design: utf8everywhere.org
// The objective is to use utf8 as the canonical string encoding, pushing
// wchar_t translation to the edge (stdio, argv, O/S and external API calls).
// The macro BC_USE_LIBBITCOIN_MAIN does most of the heavy lifting to ensure
// that stdio and argv is configured for utf8. The 'to_utf' functions are
// provided for API translations.

// Regarding source files:
//
// Don't use #pragma execution_character_set("utf-8") in VC++.
// This instruction causes sources to be explicitly interpreted as utf8.
// However this technique improperly encodes literals ("フ" for example).
// Instead use non-BOM UTF8 encoded files. To do this use "save as..."
// utf8 without a "signature" (BOM) in the Visual Studio save dialog.
// This is not required for files that contain only ascii characters.
// This is the typical file format for C++ sources in other environments.
// Do not use the L qualifier when source is UTF-8 w/out BOM (mangles output):
// const auto utf16 = L"acción.кошка.日本国";
// std::wcout << utf16;

// Regarding Boost:
//
// When working with boost and utf8 narrow characters on Win32 the thread must
// be configured for utf8. Also when working with boost::filesystem::path the
// static path object must be imbued with the utf8 locale or paths will be
// incorrectly translated. These changes are automatically configured by
// BC_USE_LIBBITCOIN_MAIN.

// Regarding console applications:
//
// BC_USE_LIBBITCOIN_MAIN should be declared prior to bc::system::main() in a
// console application. This translates arguments and the environment to UTF8
// in Win32 builds. This macro implements main() and forwards to
// bc::system::main(), which should be implemented as if it was main() with
// the expectation that argv is utf8.

// Regarding console I/O:
// 
// BC_USE_LIBBITCOIN_MAIN sets up three UTF8 streams in Win32 builds and on
// other builds these are aliased to the standard streams.
// Never use std::cout|std::cerr|std::cin (aborts on assertion):
// std::cout << "text";
// std::cerr << "text";
// std::cin >> string;
// Instead always use the following (defined by BC_USE_LIBBITCOIN_MAIN):
// bc::system::cin
// bc::system::cout
// bc::system::cerr

#endif
