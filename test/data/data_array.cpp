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
#include "../test.hpp"

BOOST_AUTO_TEST_SUITE(data_array_tests)

// to_array (byte)

BOOST_AUTO_TEST_CASE(data_array__to_array1__value__expected_size_and_value)
{
    const uint8_t expected = 42;
    const auto result = to_array(expected);
    BOOST_REQUIRE_EQUAL(result.size(), 1u);
    BOOST_REQUIRE_EQUAL(result[0], expected);
}

// to_array (data_slice)

BOOST_AUTO_TEST_CASE(data_array__to_array2__to_string__inverse)
{
    data_array<3> result{ { 24, 0, 15 } };
    BOOST_REQUIRE_EQUAL(to_array<3>(to_string(result)), result);
}

BOOST_AUTO_TEST_CASE(data_array__to_array2__double_long_hash__expected)
{
    const char l = 42;
    const uint8_t u = 24;

    // Uses data_slice data initializer construction.
    const auto result = to_array<long_hash_size>(
    {
        42, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    });
    BOOST_REQUIRE_EQUAL(result[0], l);
    BOOST_REQUIRE_EQUAL(result[long_hash_size / 2], u);
}

// build_array

BOOST_AUTO_TEST_CASE(data_array__build_array__capacity_empty__zeroed)
{
    const auto result = build_array<3>({});
    BOOST_REQUIRE_EQUAL(result.size(), 3u);
    BOOST_REQUIRE_EQUAL(result[0], 0u);
    BOOST_REQUIRE_EQUAL(result[1], 0u);
    BOOST_REQUIRE_EQUAL(result[2], 0u);
}

BOOST_AUTO_TEST_CASE(data_array__build_array__capacity__expected_values)
{
    const auto result = build_array<2 + 1 + 3>(
    {
        data_array<2>{ { 42, 14 } },
        data_array<1>{ { 12 } },
        data_array<3>{ { 24, 0, 15 } }
    });
    BOOST_REQUIRE_EQUAL(result.size(), 6u);
    BOOST_REQUIRE_EQUAL(result[0], 42u);
    BOOST_REQUIRE_EQUAL(result[1], 14u);
    BOOST_REQUIRE_EQUAL(result[2], 12u);
    BOOST_REQUIRE_EQUAL(result[3], 24u);
    BOOST_REQUIRE_EQUAL(result[4], 0u);
    BOOST_REQUIRE_EQUAL(result[5], 15u);
}

BOOST_AUTO_TEST_CASE(data_array__build_array__under_capacity__expected_zeroed)
{
    const auto result = build_array<3>(
    {
        data_array<2>{ { 24, 42 } }
    });
    BOOST_REQUIRE_EQUAL(result.size(), 3u);
    BOOST_REQUIRE_EQUAL(result[0], 24);
    BOOST_REQUIRE_EQUAL(result[1], 42);
    BOOST_REQUIRE_EQUAL(result[2], 0);
}

BOOST_AUTO_TEST_CASE(data_array__build_array__overflow__truncated_values)
{
    const auto result = build_array<3>(
    {
        data_array<2>{ { 1, 2 } },
        data_array<2>{ { 3, 4 } },
        data_array<3>{ { 5, 6, 7 } }
    });
    BOOST_REQUIRE_EQUAL(result.size(), 3u);
    BOOST_REQUIRE_EQUAL(result[0], 1u);
    BOOST_REQUIRE_EQUAL(result[1], 2u);
    BOOST_REQUIRE_EQUAL(result[2], 3u);
}

// extend

BOOST_AUTO_TEST_CASE(data_array__extend1__copy_twice__expected)
{
    const uint8_t expected = 24;
    data_chunk buffer1{ 0 };
    extend(buffer1, null_hash);
    data_chunk buffer2{ expected };
    extend(buffer1, buffer2);
    extend(buffer1, null_hash);
    BOOST_REQUIRE_EQUAL(buffer1.size(), 2u * hash_size + 2u);
    BOOST_REQUIRE_EQUAL(buffer1[hash_size + 1], expected);
}

BOOST_AUTO_TEST_CASE(data_array__extend2__move_twice__expected)
{
    const uint8_t expected = 24;
    auto hash1 = null_hash;
    auto hash2 = null_hash;
    data_chunk buffer1{ 0 };
    extend(buffer1, std::move(hash1));
    data_chunk buffer2{ expected };
    extend(buffer1, std::move(buffer2));
    extend(buffer1, std::move(hash2));
    BOOST_REQUIRE_EQUAL(buffer1.size(), 2u * hash_size + 2u);
    BOOST_REQUIRE_EQUAL(buffer1[hash_size + 1], expected);
}

// slice

BOOST_AUTO_TEST_CASE(data_array__slice__empty_selection__empty)
{
    const data_array<3> source
    {
        { 0, 0, 0 }
    };
    const auto result = slice<2, 2>(source);
    BOOST_REQUIRE(result.empty());
}

BOOST_AUTO_TEST_CASE(data_array__slice__three_bytes_front__expected)
{
    const data_array<3> source
    {
        { 24, 42, 55 }
    };
    const auto result = slice<0, 3>(source);
    BOOST_REQUIRE_EQUAL(result.size(), 3u);
    BOOST_REQUIRE_EQUAL(result[0], 24u);
    BOOST_REQUIRE_EQUAL(result[1], 42u);
    BOOST_REQUIRE_EQUAL(result[2], 55u);
}

BOOST_AUTO_TEST_CASE(data_array__slice__three_bytes_middle__expected)
{
    const data_array<3> source
    {
        { 7, 42, 5 }
    };
    const auto result = slice<1, 2>(source);
    BOOST_REQUIRE_EQUAL(result.size(), 1u);
    BOOST_REQUIRE_EQUAL(result[0], 42u);
}

BOOST_AUTO_TEST_CASE(data_array__slice__three_bytes_end__expected)
{
    const data_array<3> source
    {
        { 2, 7, 11 }
    };
    const auto result = slice<1, 3>(source);
    BOOST_REQUIRE_EQUAL(result.size(), 2u);
    BOOST_REQUIRE_EQUAL(result[0], 7u);
    BOOST_REQUIRE_EQUAL(result[1], 11u);
}

// splice(,)

BOOST_AUTO_TEST_CASE(data_array__splice2__two_bytes_each__expected)
{
    const data_array<2> left
    {
        { 42, 0 }
    };
    const data_array<2> right
    {
        { 0, 24 }
    };
    const auto result = splice(left, right);
    BOOST_REQUIRE_EQUAL(result.size(), 4u);
    BOOST_REQUIRE_EQUAL(result[0], 42u);
    BOOST_REQUIRE_EQUAL(result[1], 0u);
    BOOST_REQUIRE_EQUAL(result[2], 0u);
    BOOST_REQUIRE_EQUAL(result[3], 24u);
}

// splice(,,)

BOOST_AUTO_TEST_CASE(data_array__splice3__one_two_three_bytes__expected)
{
    const data_array<1> left
    {
        { 42 }
    };
    const data_array<2> middle
    {
        { 7, 11 }
    };
    const data_array<3> right
    {
        { 5, 0, 24 }
    };
    const auto result = splice(left, middle, right);
    BOOST_REQUIRE_EQUAL(result.size(), 6u);
    BOOST_REQUIRE_EQUAL(result[0], 42u);
    BOOST_REQUIRE_EQUAL(result[1], 7u);
    BOOST_REQUIRE_EQUAL(result[2], 11u);
    BOOST_REQUIRE_EQUAL(result[3], 5u);
    BOOST_REQUIRE_EQUAL(result[4], 0u);
    BOOST_REQUIRE_EQUAL(result[5], 24u);
}

// split

BOOST_AUTO_TEST_CASE(data_array__split__two_bytes__expected)
{
    const data_array<2> source
    {
        { 42, 24 }
    };

    const auto result = split(source);
    BOOST_REQUIRE_EQUAL(result.first.size(), 1u);
    BOOST_REQUIRE_EQUAL(result.first[0], 42u);
    BOOST_REQUIRE_EQUAL(result.second.size(), 1u);
    BOOST_REQUIRE_EQUAL(result.second[0], 24u);
}

BOOST_AUTO_TEST_CASE(data_array__split__long_hash__expected)
{
    const long_hash source
    {
        {
            5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        }
    };
    const auto result = split(source);
    BOOST_REQUIRE_EQUAL(result.first.size(), 32u);
    BOOST_REQUIRE_EQUAL(result.first[0], 5u);
    BOOST_REQUIRE_EQUAL(result.second.size(), 32u);
    BOOST_REQUIRE_EQUAL(result.second[0], 9u);
}

// xor_data

BOOST_AUTO_TEST_CASE(data_array__xor_data1__empty__empty)
{
    static const data_array<1> source{ { 0x0 } };
    const auto result = xor_data<0>(source, source);
    BOOST_REQUIRE_EQUAL(result.size(), 0u);
}

BOOST_AUTO_TEST_CASE(data_array__xor_data1__same__zeros)
{
    static const data_array<2> source{ { 7, 11 } };
    const auto result = xor_data<2>(source, source);
    BOOST_REQUIRE_EQUAL(result.size(), 2u);
    BOOST_REQUIRE_EQUAL(result[0], 0u);
    BOOST_REQUIRE_EQUAL(result[1], 0u);
}

BOOST_AUTO_TEST_CASE(data_array__xor_data1__distinct__ones)
{
    static const data_array<2> source1{ { 0x00, 0xff } };
    static const data_array<2> source2{ { 0xff, 0x00 } };
    const auto result = xor_data<2>(source1, source2);
    BOOST_REQUIRE_EQUAL(result.size(), 2u);
    BOOST_REQUIRE_EQUAL(result[0], 0xff);
    BOOST_REQUIRE_EQUAL(result[1], 0xff);
}

// xor_offset

BOOST_AUTO_TEST_CASE(data_array__xor_offset__empty__empty)
{
    static const data_array<1> source{ { 0x0 } };
    const auto result = xor_offset<0, 0, 0>(source, source);
    BOOST_REQUIRE_EQUAL(result.size(), 0u);
}

BOOST_AUTO_TEST_CASE(data_array__xor_offset__empty_out__empty)
{
    static const data_array<2> source1{ { 0x00, 0xff } };
    static const data_array<3> source2{ { 0xff, 0x00, 0xff } };
    const auto result = xor_offset<0, 0, 0>(source1, source2);
    BOOST_REQUIRE_EQUAL(result.size(), 0u);
}

BOOST_AUTO_TEST_CASE(data_array__xor_offset__same__zeros)
{
    static const data_array<2> source1{ { 0x00, 0xff } };
    const auto result = xor_offset<2, 0, 0>(source1, source1);
    BOOST_REQUIRE_EQUAL(result.size(), 2u);
    BOOST_REQUIRE_EQUAL(result[0], 0);
    BOOST_REQUIRE_EQUAL(result[1], 0);
}

BOOST_AUTO_TEST_CASE(data_array__xor_offset__distinct_trimmed__ones)
{
    static const data_array<3> source1{ { 0x00, 0xff, 0x00 } };
    static const data_array<4> source2{ { 0xff, 0x00, 0xff, 0x00 } };
    const auto result = xor_offset<3, 0, 0>(source1, source2);
    BOOST_REQUIRE_EQUAL(result.size(), 3u);
    BOOST_REQUIRE_EQUAL(result[0], 0xff);
    BOOST_REQUIRE_EQUAL(result[1], 0xff);
    BOOST_REQUIRE_EQUAL(result[2], 0xff);
}

BOOST_AUTO_TEST_CASE(data_array__xor_offset__distinct_same_offsets_expected)
{
    const uint8_t expected0 = 42u ^ 24u;
    const uint8_t expected1 = 13u ^ 13u;
    static const data_array<4> source1{ { 7, 42, 13, 0x00 } };
    static const data_array<5> source2{ { 7, 24, 13, 0xff, 0x00 } };
    const auto result = xor_offset<2, 1, 1>(source1, source2);
    BOOST_REQUIRE_EQUAL(result.size(), 2u);
    BOOST_REQUIRE_EQUAL(result[0], expected0);
    BOOST_REQUIRE_EQUAL(result[1], expected1);
}

BOOST_AUTO_TEST_CASE(data_array__xor_offset__distinct_different_offsets_expected)
{
    const uint8_t expected0 = 42u ^ 24u;
    const uint8_t expected1 = 13u ^ 13u;
    static const data_array<4> source1{ { 7, 42, 13, 0x00 } };
    static const data_array<5> source2{ { 0, 7, 24, 13, 0xff } };
    const auto result = xor_offset<2, 1, 2>(source1, source2);
    BOOST_REQUIRE_EQUAL(result.size(), 2u);
    BOOST_REQUIRE_EQUAL(result[0], expected0);
    BOOST_REQUIRE_EQUAL(result[1], expected1);
}

BOOST_AUTO_TEST_SUITE_END()
