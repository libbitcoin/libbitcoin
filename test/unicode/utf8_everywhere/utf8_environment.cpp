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
#include "../../test.hpp"
#include <cstring>
#include <vector>

BOOST_AUTO_TEST_SUITE(utf8_environment_tests)

// utf8_max_character_size

BOOST_AUTO_TEST_CASE(utf8_environment__utf8_max_character_size__always__4)
{
    BOOST_REQUIRE_EQUAL(utf8_max_character_size, 4u);
}

BOOST_AUTO_TEST_CASE(utf8_environment__utf8_remainder_size__empty__zero)
{
    BOOST_REQUIRE_EQUAL(utf8_remainder_size(nullptr, 0), 0u);
}

// utf8_remainder_size

BOOST_AUTO_TEST_CASE(utf8_environment__utf8_remainder_size__ascii_bytes__zero)
{
    BOOST_REQUIRE_EQUAL(utf8_remainder_size("a", 1), 0u);
    BOOST_REQUIRE_EQUAL(utf8_remainder_size("ab", 2), 0u);
    BOOST_REQUIRE_EQUAL(utf8_remainder_size("abc", 3), 0u);
    BOOST_REQUIRE_EQUAL(utf8_remainder_size("abcd", 4), 0u);
    BOOST_REQUIRE_EQUAL(utf8_remainder_size("abcde", 5), 0u);
    BOOST_REQUIRE_EQUAL(utf8_remainder_size("abcdef", 6), 0u);
}

BOOST_AUTO_TEST_CASE(utf8_environment__utf8_remainder_size__whole_characters__zero)
{
    BOOST_REQUIRE_EQUAL(utf8_remainder_size("\xDF\xbf", 2), 0u);
    BOOST_REQUIRE_EQUAL(utf8_remainder_size("\xEF\xbf\xbf", 3), 0u);
    BOOST_REQUIRE_EQUAL(utf8_remainder_size("\xF7\xbf\xbf\xbf", 4), 0u);
}

BOOST_AUTO_TEST_CASE(utf8_environment__utf8_remainder_size__truncated_trailing_bytes__expected)
{
    BOOST_REQUIRE_EQUAL(utf8_remainder_size("\xDF", 1), 1u);
    BOOST_REQUIRE_EQUAL(utf8_remainder_size("\xEF", 1), 1u);
    BOOST_REQUIRE_EQUAL(utf8_remainder_size("\xEF\xbf", 2), 2u);
    BOOST_REQUIRE_EQUAL(utf8_remainder_size("\xF7", 1), 1u);
    BOOST_REQUIRE_EQUAL(utf8_remainder_size("\xF7\xbf", 2), 2u);
    BOOST_REQUIRE_EQUAL(utf8_remainder_size("\xF7\xbf\xbf", 3), 3u);
}

BOOST_AUTO_TEST_CASE(utf8_environment__utf8_remainder_size__invalid_leading_bytes__zero)
{
    BOOST_REQUIRE_EQUAL(utf8_remainder_size("\xbf", 1), 0u);
    BOOST_REQUIRE_EQUAL(utf8_remainder_size("\xbf\xbf", 2), 0u);
    BOOST_REQUIRE_EQUAL(utf8_remainder_size("\xbf\xbf\xbf", 3), 0u);
    BOOST_REQUIRE_EQUAL(utf8_remainder_size("\xbf\xbf\xbf\xbf", 4), 0u);
}

// to_utf8 (wchar_t[])

BOOST_AUTO_TEST_CASE(utf8_environment__to_utf8_array__null_out__zero)
{
    const wchar_t* in = L"";
    BOOST_REQUIRE_EQUAL(to_utf8(nullptr, 42, in, 42), 0u);
}

BOOST_AUTO_TEST_CASE(utf8_environment__to_utf8_array__null_in__zero)
{
    char out[42];
    BOOST_REQUIRE_EQUAL(to_utf8(out, 42, nullptr, 42), 0u);
}

BOOST_AUTO_TEST_CASE(utf8_environment__to_utf8_array__zero_in__zero)
{
    char out[42];
    const wchar_t* in = L"";
    BOOST_REQUIRE_EQUAL(to_utf8(out, 42, in, 0), 0u);
}

BOOST_AUTO_TEST_CASE(utf8_environment__to_utf8_array__zero_out__zero)
{
    char out[42];
    const wchar_t* in = L"";
    BOOST_REQUIRE_EQUAL(to_utf8(out, 0, in, 42), 0u);
}

BOOST_AUTO_TEST_CASE(utf8_environment__to_utf8_array__ascii__test)
{
    char utf8[20];

    // Text buffer provides null termination for test comparison.
    memset(utf8, 0, sizeof(utf8) / sizeof(char));

    // Use of L is not recommended as it will only work for ascii.
    const std::wstring utf16(L"ascii");
    const std::string expected_utf8("ascii");

    const auto size = to_utf8(utf8, sizeof(utf8), utf16.c_str(), (int)utf16.size());
    BOOST_REQUIRE_EQUAL(utf8, expected_utf8);
    BOOST_REQUIRE_EQUAL(size, expected_utf8.size());
}

BOOST_AUTO_TEST_CASE(utf8_environment__to_utf8_array__non_ascii__test)
{
    char utf8[36];

    // Text buffer provides null termination for test comparison.
    memset(utf8, 0, sizeof(utf8) / sizeof(char));

    const std::string expected_utf8("テスト");
    const auto utf16 = to_utf16(expected_utf8);

    const auto size = to_utf8(utf8, sizeof(utf8), utf16.c_str(), (int)utf16.size());

    BOOST_REQUIRE_EQUAL(utf8, expected_utf8);
    BOOST_REQUIRE_EQUAL(size, expected_utf8.size());
}

// to_utf16 (char[])

BOOST_AUTO_TEST_CASE(utf8_environment__to_utf16_array__null_truncated__zero)
{
    size_t truncated;
    const auto in = "";
    BOOST_REQUIRE_EQUAL(to_utf16(truncated, nullptr, 42, in, 42), 0u);
}

BOOST_AUTO_TEST_CASE(utf8_environment__to_utf16_array__null_out__zero)
{
    size_t truncated;
    const auto in = "";
    BOOST_REQUIRE_EQUAL(to_utf16(truncated, nullptr, 42, in, 42), 0u);
}

BOOST_AUTO_TEST_CASE(utf8_environment__to_utf16_array__null_in__zero)
{
    size_t truncated;
    wchar_t out[42];
    BOOST_REQUIRE_EQUAL(to_utf16(truncated, out, 42, nullptr, 42), 0u);
}

BOOST_AUTO_TEST_CASE(utf8_environment__to_utf16_array__zero_in__zero)
{
    size_t truncated;
    wchar_t out[42];
    const auto in = "";
    BOOST_REQUIRE_EQUAL(to_utf16(truncated, out, 42, in, 0), 0u);
}

BOOST_AUTO_TEST_CASE(utf8_environment__to_utf16_array__zero_out__zero)
{
    size_t truncated;
    wchar_t* out = nullptr;
    const auto in = "";
    BOOST_REQUIRE_EQUAL(to_utf16(truncated, out, 0, in, 42), 0u);
}

BOOST_AUTO_TEST_CASE(utf8_environment__to_utf16_array__ascii__expected)
{
    const auto length = 20u;
    wchar_t utf16[length];

    // Text buffer provides null termination for test comparison.
    wmemset(utf16, 0, length);

    // Use of L is not recommended as it will only work for ascii.
    const std::wstring expected_utf16(L"ascii");
    const std::string utf8("ascii");

    size_t truncated;
    const auto size = to_utf16(truncated, utf16, length, utf8.c_str(), utf8.length());

    BOOST_REQUIRE_EQUAL(utf16, expected_utf16.c_str());
    BOOST_REQUIRE_EQUAL(size, expected_utf16.size());
    BOOST_REQUIRE_EQUAL(truncated, 0u);
}

BOOST_AUTO_TEST_CASE(utf8_environment__to_utf16_array__non_ascii__expected)
{
    const auto length = 36u;
    wchar_t utf16[length];

    // Text buffer provides null termination for test comparison.
    wmemset(utf16, 0, length);

    const std::string utf8("テスト");
    const auto expected_utf16 = to_utf16(utf8);

    size_t truncated;
    const auto size = to_utf16(truncated, utf16, length, utf8.c_str(), utf8.length());

    BOOST_REQUIRE_EQUAL(utf16, expected_utf16.c_str());
    BOOST_REQUIRE_EQUAL(size, expected_utf16.size());
    BOOST_REQUIRE_EQUAL(truncated, 0u);
}

BOOST_AUTO_TEST_CASE(utf8_environment__to_utf16_array__non_ascii_truncation1__expected)
{
    const auto length = 36u;
    wchar_t utf16[length];

    // Text buffer provides null termination for test comparison.
    wmemset(utf16, 0, length);

    std::string utf8("テスト");
    auto expected_utf16 = to_utf16(utf8);

    // Lop off last byte, which will split the last character.
    const auto drop_bytes = 1u;
    utf8.resize(utf8.size() - drop_bytes);

    // Expect the loss of the last wide character.
    expected_utf16.resize(expected_utf16.size() - 1u);

    // Expect the truncation of the remaining bytes of the last character.
    const auto expected_truncated = strlen("ト") - 1u;

    size_t truncated;
    const auto size = to_utf16(truncated, utf16, length, utf8.c_str(), utf8.length());

    BOOST_REQUIRE_EQUAL(truncated, expected_truncated);
    BOOST_REQUIRE_EQUAL(utf16, expected_utf16.c_str());
    BOOST_REQUIRE_EQUAL(size, expected_utf16.size());
}

BOOST_AUTO_TEST_CASE(utf8_environment__to_utf16_array__non_ascii_truncation2__expected)
{
    const auto length = 36u;
    wchar_t utf16[length];

    // Text buffer provides null termination for test comparison.
    wmemset(utf16, 0, length);

    std::string utf8("テスト");
    auto expected_utf16 = to_utf16(utf8);

    // Lop off last two bytes, which will split the last character.
    const auto drop_bytes = 2u;
    utf8.resize(utf8.size() - drop_bytes);

    // Expect the loss of the last wide character.
    expected_utf16.resize(expected_utf16.size() - 1u);

    // Expect the truncation of the remaining bytes of the last character.
    const auto expected_truncated = strlen("ト") - drop_bytes;

    size_t truncated;
    const auto size = to_utf16(truncated, utf16, length, utf8.c_str(), utf8.length());

    BOOST_REQUIRE_EQUAL(truncated, expected_truncated);
    BOOST_REQUIRE_EQUAL(utf16, expected_utf16.c_str());
    BOOST_REQUIRE_EQUAL(size, expected_utf16.size());
}

// BC_USE_LIBBITCOIN_MAIN

#ifdef _MSC_VER

// allocate_environment (vars) / free_environment

BOOST_AUTO_TEST_CASE(utf8_environment__allocate_environment_vars__ascii__expected)
{
    std::vector<const wchar_t*> wide_environment = { L"ascii", nullptr };
    auto variables = const_cast<wchar_t**>(&wide_environment[0]);
    auto narrow_environment = allocate_environment(variables);
    BOOST_REQUIRE_EQUAL(narrow_environment[0], "ascii");
    free_environment(narrow_environment);
}

BOOST_AUTO_TEST_CASE(utf8_environment__allocate_environment_vars__utf16__expected)
{
    // We cannot use L for literal encoding of non-ascii text on Windows.
    auto utf16 = to_utf16("テスト");
    auto non_literal_utf16 = utf16.c_str();
    std::vector<const wchar_t*> wide_environment = { L"ascii", non_literal_utf16, nullptr };
    auto variables = const_cast<wchar_t**>(&wide_environment[0]);
    auto narrow_environment = allocate_environment(variables);
    BOOST_REQUIRE_EQUAL(narrow_environment[0], "ascii");
    BOOST_REQUIRE_EQUAL(narrow_environment[1], "テスト");
    free_environment(narrow_environment);
}

BOOST_AUTO_TEST_CASE(utf8_environment__allocate_environment_vars__null_termination__expected)
{
    std::vector<const wchar_t*> wide_environment = { L"ascii", nullptr };
    auto variables = const_cast<wchar_t**>(&wide_environment[0]);
    auto expected_count = static_cast<int>(wide_environment.size()) - 1u;
    auto narrow_environment = allocate_environment(variables);

    // Each argument is a null terminated string.
    const auto length = strlen(narrow_environment[0]);
    auto variable_terminator = narrow_environment[0][length];
    BOOST_REQUIRE_EQUAL(variable_terminator, '\0');

    // The argument vector is a null terminated array.
    auto environment_terminator = narrow_environment[expected_count];
    BOOST_REQUIRE_EQUAL(environment_terminator, (char*)nullptr);
    free_environment(narrow_environment);
}

// allocate_environment (args) / free_environment

BOOST_AUTO_TEST_CASE(utf8_environment__allocate_environment_args__ascii__expected)
{
    std::vector<const wchar_t*> wide_args = { L"ascii", nullptr };
    auto argv = const_cast<wchar_t**>(&wide_args[0]);
    auto argc = static_cast<int>(wide_args.size()) - 1u;
    auto narrow_args = allocate_environment(argc, argv);
    BOOST_REQUIRE_EQUAL(narrow_args[0], "ascii");
    free_environment(narrow_args);
}

BOOST_AUTO_TEST_CASE(utf8_environment__allocate_environment_args__utf16__expected)
{
    // We cannot use L for literal encoding of non-ascii text on Windows.
    auto utf16 = to_utf16("テスト");
    auto non_literal_utf16 = utf16.c_str();
    std::vector<const wchar_t*> wide_args = { L"ascii", non_literal_utf16, nullptr };
    auto argv = const_cast<wchar_t**>(&wide_args[0]);
    auto argc = static_cast<int>(wide_args.size()) - 1u;
    auto narrow_args = allocate_environment(argc, argv);
    BOOST_REQUIRE_EQUAL(narrow_args[0], "ascii");
    BOOST_REQUIRE_EQUAL(narrow_args[1], "テスト");
    free_environment(narrow_args);
}

BOOST_AUTO_TEST_CASE(utf8_environment__allocate_environment_args__null_termination__expected)
{
    std::vector<const wchar_t*> wide_args = { L"ascii", nullptr };
    auto argv = const_cast<wchar_t**>(&wide_args[0]);
    auto argc = static_cast<int>(wide_args.size()) - 1u;
    auto narrow_args = allocate_environment(argc, argv);

    // Each argument is a null terminated string.
    const auto length = strlen(narrow_args[0]);
    auto arg_terminator = narrow_args[0][length];
    BOOST_REQUIRE_EQUAL(arg_terminator, '\0');

    // The argument vector is a null terminated array.
    auto argv_terminator = narrow_args[argc];
    BOOST_REQUIRE_EQUAL(argv_terminator, (char*)nullptr);
    free_environment(narrow_args);
}

// to_extended_path

BOOST_AUTO_TEST_CASE(utf8_environment__to_extended_path__always__expected)
{
    ////std::string to_extended_path(const path& path);
}

BOOST_AUTO_TEST_CASE(utf8_environment__to_extended_path16__always__expected)
{
    ////std::wstring to_extended_path16(const std::string& path);
}

#endif // _MSC_VER

BOOST_AUTO_TEST_SUITE_END()
