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
#ifndef LIBBITCOIN_SYSTEM_UNICODE_UTF8_EVERYWHERE_UTF8_EVERYWHERE_HPP
#define LIBBITCOIN_SYSTEM_UNICODE_UTF8_EVERYWHERE_UTF8_EVERYWHERE_HPP

#include <iostream>
#include <bitcoin/system/unicode/utf8_everywhere/utf8_console_streambuf.hpp>
#include <bitcoin/system/unicode/utf8_everywhere/utf8_environment.hpp>
#include <bitcoin/system/unicode/utf8_everywhere/utf8_ifstream.hpp>
#include <bitcoin/system/unicode/utf8_everywhere/utf8_istream.hpp>
#include <bitcoin/system/unicode/utf8_everywhere/utf8_ofstream.hpp>
#include <bitcoin/system/unicode/utf8_everywhere/utf8_ostream.hpp>
#include <bitcoin/system/unicode/utf8_everywhere/utf8_streambuf.hpp>

 // See "utf8_about" for more information on Win32 compatibility.

// BC_USE_LIBBITCOIN_MAIN should be declared prior to bc::system::main() in a
// console application. This enables Unicode argument and environment
// processing in Windows. This macro implements main() and forwards to
// bc::system::main(), which should be implemented as if it was main() with
// the expectation that argv is utf8.

namespace libbitcoin {
namespace system {

#ifdef _MSC_VER

#define BC_USE_LIBBITCOIN_MAIN \
    namespace libbitcoin \
    { \
    namespace system \
    { \
        std::istream& cin = cin_stream(); \
        std::ostream& cout = cout_stream(); \
        std::ostream& cerr = cerr_stream(); \
        int main(int argc, char* argv[]); \
    } \
    } \
    \
    int wmain(int argc, wchar_t* argv[]) \
    { \
        return libbitcoin::system::call_utf8_main(argc, argv, \
            &libbitcoin::system::main); \
    }

#else

#define BC_USE_LIBBITCOIN_MAIN \
    namespace libbitcoin \
    { \
    namespace system \
    { \
        std::istream& cin = std::cin; \
        std::ostream& cout = std::cout; \
        std::ostream& cerr = std::cerr; \
        int main(int argc, char* argv[]); \
    } \
    } \
    \
    int main(int argc, char* argv[]) \
    { \
        return libbitcoin::system::main(argc, argv); \
    }

#endif

} // namespace system
} // namespace libbitcoin

#endif
