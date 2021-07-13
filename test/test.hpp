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
#ifndef LIBBITCOIN_SYSTEM_TEST_HPP
#define LIBBITCOIN_SYSTEM_TEST_HPP

#include <boost/test/unit_test.hpp>

#include <array>
#include <iostream>
#include <vector>
#include <bitcoin/system.hpp>

using namespace bc;
using namespace bc::system;

namespace std {

// data_slice -> base16(data)
std::ostream& operator<<(std::ostream& stream,
    const data_slice& slice) noexcept;

// vector<Type> -> join(<<Type)
template <typename Type>
std::ostream& operator<<(std::ostream& stream,
    const std::vector<Type>& values) noexcept
{
    // Ok when testing serialize because only used for error message out.
    stream << serialize(values);
    return stream;
}

// array<Type, Size> -> join(<<Type)
template <typename Type, size_t Size>
std::ostream& operator<<(std::ostream& stream,
    const std::array<Type, Size>& values) noexcept
{
    // Ok when testing serialize because only used for error message out.
    stream << serialize(values);
    return stream;
}

} // namespace std

#define TEST_NAME \
    boost::unit_test::framework::current_test_case().p_name.get()
#define SUITE_NAME \
    boost::unit_test::framework::current_auto_test_suite().p_name.get()
#define TEST_PATH \
    test::directory + "/" + TEST_NAME

namespace test {

// Common directory for all test file creations.
// Subdirectories and/or files must be differentiated (i.e. by TEST_NAME).
// Total path length cannot exceed MAX_PATH in _MSC_VER builds.
extern const std::string directory;

bool clear(const boost::filesystem::path& directory) noexcept;
bool create(const boost::filesystem::path& file_path) noexcept;
bool exists(const boost::filesystem::path& file_path) noexcept;
bool remove(const boost::filesystem::path& file_path) noexcept;

} // namespace test

#endif
