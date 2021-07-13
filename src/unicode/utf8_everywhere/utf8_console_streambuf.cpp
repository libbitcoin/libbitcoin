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
#include <bitcoin/system/unicode/utf8_everywhere/utf8_console_streambuf.hpp>

#include <cstddef>
#include <iostream>
#include <new>
#include <streambuf>
#include <bitcoin/system/exceptions.hpp>

#ifdef _MSC_VER
    #include <windows.h>
#endif

namespace libbitcoin {
namespace system {

#ifdef _MSC_VER

// Get Windows input handle.
static LPVOID get_input_handle()
{
    const auto handle = GetStdHandle(STD_INPUT_HANDLE);
    if (handle == INVALID_HANDLE_VALUE || handle == nullptr)
        throw runtime_exception("Failed to get input handle.");

    return handle;
}

// This is the factory method to privately instantiate a singleton class.
void console_streambuf::initialize(size_t size)
{
    // Set the console to operate in UTF-8 for this process.
    if (SetConsoleCP(CP_UTF8) == FALSE)
        throw runtime_exception("Failed to set console to utf8.");

    DWORD console_mode;
    if (GetConsoleMode(get_input_handle(), &console_mode) != FALSE)
    {
        // Hack for faulty std::wcin translation of non-ASCII keyboard input.
        static console_streambuf buffer(*std::wcin.rdbuf(), size);
        std::wcin.rdbuf(&buffer);
    }
}

console_streambuf::console_streambuf(
    const std::wstreambuf& stream_buffer, size_t size)
    : buffer_size_(size), buffer_(new wchar_t[buffer_size_]),
    std::wstreambuf(stream_buffer)
{
}

console_streambuf::~console_streambuf()
{
    delete[] buffer_;
}

std::streamsize console_streambuf::xsgetn(wchar_t* buffer,
    std::streamsize size)
{
    DWORD read_bytes;

    if (ReadConsoleW(get_input_handle(), buffer, static_cast<DWORD>(size),
        &read_bytes, nullptr) == FALSE)
        throw runtime_exception("Failed to read from console.");

    return static_cast<std::streamsize>(read_bytes);
}

std::wstreambuf::int_type console_streambuf::underflow()
{
    if (gptr() == nullptr || gptr() >= egptr())
    {
        const auto length = xsgetn(buffer_, buffer_size_);
        if (length > 0)
            setg(buffer_, buffer_, &buffer_[length]);
    }

    return (gptr() == nullptr || gptr() >= egptr()) ? traits_type::eof() :
        traits_type::to_int_type(*gptr());
}

#else

void console_streambuf::initialize(size_t)
{
}

console_streambuf::console_streambuf(const std::wstreambuf&, size_t)
{
}

console_streambuf::~console_streambuf()
{
}

std::streamsize console_streambuf::xsgetn(wchar_t*, std::streamsize)
{
    return 0;
}

std::wstreambuf::int_type console_streambuf::underflow()
{
    return traits_type::to_int_type(*gptr());
}

#endif

} // namespace system
} // namespace libbitcoin
