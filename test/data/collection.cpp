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
#include "../test.hpp"
#include <array>
#include <cstdint>
#include <vector>

BOOST_AUTO_TEST_SUITE(collection_tests)

// TODO:
// contains
// find_pair_position
// find_position
// insert_sorted

// binary_search:
// Collection must implement [] and .size().
// Either list members or the search key must implement lexical compare (<, >).

// binary_search (native byte comparison operators)

BOOST_AUTO_TEST_CASE(limits__binary_search_native__empty__not_found)
{
    const std::string empty = "";
    const auto value = 'a';
    BOOST_REQUIRE_EQUAL(binary_search(empty, value), negative_one);
}

BOOST_AUTO_TEST_CASE(limits__binary_search_native__one_element_unmatched__not_found)
{
    const std::string single = "a";
    const auto value = 'b';
    BOOST_REQUIRE_EQUAL(binary_search(single, value), negative_one);
}

BOOST_AUTO_TEST_CASE(limits__binary_search_native__one_element_match__0)
{
    const std::string single = "a";
    const auto value = 'a';
    BOOST_REQUIRE_EQUAL(binary_search(single, value), 0);
}

BOOST_AUTO_TEST_CASE(limits__binary_search_native__two_elements_match_first__0)
{
    const std::string sorted = "ab";
    const auto value = 'a';
    BOOST_REQUIRE_EQUAL(binary_search(sorted, value), 0);
}

BOOST_AUTO_TEST_CASE(limits__binary_search_native__two_elements_match_second__1)
{
    const std::string sorted = "ab";
    const auto value = 'b';
    auto result = binary_search(sorted, value);
    BOOST_REQUIRE_EQUAL(result, 1);
}

BOOST_AUTO_TEST_CASE(limits__binary_search_native__three_elements_match_second__1)
{
    const std::string sorted = "abc";
    const auto value = 'b';
    BOOST_REQUIRE_EQUAL(binary_search(sorted, value), 1);
}

BOOST_AUTO_TEST_CASE(limits__binary_search_native__sorted_contained__expected)
{
    const std::string sorted = "abcdefjkxyz";
    const auto value = 'x';
    BOOST_REQUIRE_EQUAL(binary_search(sorted, value), 8);
}

BOOST_AUTO_TEST_CASE(limits__binary_search_native__reverse_sorted_contained__unlucky)
{
    const std::string unsorted = "zyxwvutscba";
    const auto value = 'x';
    BOOST_REQUIRE_EQUAL(binary_search(unsorted, value), -1);
}

// binary_search (list element comparison operators)

BOOST_AUTO_TEST_CASE(limits__binary_search_element__three_various_elements_unmatched__not_found)
{
    const string_list sorted{ "afgdjdfj", "btffghfg", "cfdd" };
    const auto value = "bcd";
    BOOST_REQUIRE_EQUAL(binary_search(sorted, value), negative_one);
}

BOOST_AUTO_TEST_CASE(limits__binary_search_element__three_various_elements_matched__found)
{
    const string_list sorted{ "afgdjdfj", "btffghfg", "cfdd" };
    const auto value = "cfdd";
    BOOST_REQUIRE_EQUAL(binary_search(sorted, value), 2);
}

BOOST_AUTO_TEST_CASE(limits__binary_search_element__unsorted_contained__unlucky)
{
    const string_list unsorted{ { "z", "y", "x" } };
    const auto value = "z";
    BOOST_REQUIRE_EQUAL(binary_search(unsorted, value), negative_one);
}

// binary_search (key comparison operators)

BOOST_AUTO_TEST_CASE(limits__binary_search_key__three_various_elements_unmatched__not_found)
{
    const std::vector<const char*> sorted{ "afgdjdfj", "btffghfg", "cfdd" };
    const std::string value = "bcd";
    BOOST_REQUIRE_EQUAL(binary_search(sorted, value), negative_one);
}

BOOST_AUTO_TEST_CASE(limits__binary_search_key__three_various_elements_matched__found)
{
    const std::vector<const char*> sorted{ "afgdjdfj", "btffghfg", "cfdd" };
    const std::string value = "cfdd";
    BOOST_REQUIRE_EQUAL(binary_search(sorted, value), 2);
}

BOOST_AUTO_TEST_CASE(limits__binary_search_key__unsorted_contained__unlucky)
{
    const std::vector<const char*> unsorted{ "z", "y", "x" };
    const std::string value = "z";
    BOOST_REQUIRE_EQUAL(binary_search(unsorted, value), negative_one);
}

// cast (vector)

BOOST_AUTO_TEST_CASE(collection__cast_vector__empty__empty)
{
    BOOST_REQUIRE(cast<char>(data_chunk{}).empty());
}

BOOST_AUTO_TEST_CASE(collection__cast_vector__one_element__same)
{
    const auto result = cast<char>(data_chunk{ 42 });
    BOOST_REQUIRE_EQUAL(result.size(), 1u);
    BOOST_REQUIRE_EQUAL(result.front(), 42);
}

BOOST_AUTO_TEST_CASE(collection__cast_vector__distinct_types__same)
{
    const data_chunk value{ 42, 24 };
    const auto result = cast<uint32_t>(value);
    BOOST_REQUIRE_EQUAL(value.size(), result.size());
    BOOST_REQUIRE_EQUAL(result[0], value[0]);
    BOOST_REQUIRE_EQUAL(result[1], value[1]);
}

BOOST_AUTO_TEST_CASE(collection__cast_vector__narrowing__same)
{
    // Implicit narrowing cast doesn't warn.
    const std::vector<uint32_t> value{ 42, 24 };
    const auto result = cast<uint8_t>(value);
    BOOST_REQUIRE_EQUAL(value.size(), result.size());
    BOOST_REQUIRE_EQUAL(result[0], value[0]);
    BOOST_REQUIRE_EQUAL(result[1], value[1]);
}

// cast (array)

BOOST_AUTO_TEST_CASE(collection__cast_array__empty__empty)
{
    BOOST_REQUIRE(cast<char>(data_array<0>{}).empty());
}

BOOST_AUTO_TEST_CASE(collection__cast_array__one_element__same)
{
    const auto result = cast<char>(data_array<1>{ 42 });
    BOOST_REQUIRE_EQUAL(result.size(), 1u);
    BOOST_REQUIRE_EQUAL(result.front(), 42);
}

BOOST_AUTO_TEST_CASE(collection__cast_array__distinct_types__same)
{
    const data_array<2> value{ 42, 24 };
    const auto result = cast<uint32_t>(value);
    BOOST_REQUIRE_EQUAL(value.size(), result.size());
    BOOST_REQUIRE_EQUAL(result[0], value[0]);
    BOOST_REQUIRE_EQUAL(result[1], value[1]);
}

BOOST_AUTO_TEST_CASE(collection__cast_array__narrowing__same)
{
    // Implicit narrowing cast doesn't warn.
    const std::array<uint32_t, 2> value{ 42, 24 };
    const auto result = cast<uint8_t>(value);
    BOOST_REQUIRE_EQUAL(value.size(), result.size());
    BOOST_REQUIRE_EQUAL(result[0], value[0]);
    BOOST_REQUIRE_EQUAL(result[1], value[1]);
}

// move_append

BOOST_AUTO_TEST_CASE(collection__move_append__both_empty__empty)
{
    data_chunk source;
    data_chunk target;
    move_append(target, source);
    BOOST_REQUIRE(source.empty());
    BOOST_REQUIRE(target.empty());
}

BOOST_AUTO_TEST_CASE(collection__move_append__source_empty__expected)
{
    data_chunk source;
    data_chunk target{ 0, 2, 4, 6, 8 };
    const data_chunk expected = target;
    move_append(target, source);
    BOOST_REQUIRE_EQUAL(target, expected);
}

BOOST_AUTO_TEST_CASE(collection__move_append__target_empty__expected)
{
    data_chunk source{ 0, 2, 4, 6, 8 };
    data_chunk target;
    const data_chunk expected = source;
    move_append(target, source);
    BOOST_REQUIRE_EQUAL(target, expected);
}

BOOST_AUTO_TEST_CASE(collection__move_append__neither_empty__expected)
{
    data_chunk source{ 10, 12, 14, 16, 18 };
    data_chunk target{ 0, 2, 4, 6, 8 };
    const data_chunk expected{ 0, 2, 4, 6, 8, 10, 12, 14, 16, 18 };
    move_append(target, source);
    BOOST_REQUIRE_EQUAL(target, expected);
}

// pop

BOOST_AUTO_TEST_CASE(collection__pop__empty__empty_default)
{
    data_chunk stack;
    const auto value = pop(stack);
    BOOST_REQUIRE(stack.empty());
    BOOST_REQUIRE_EQUAL(value, 0u);
}

BOOST_AUTO_TEST_CASE(collection__pop__single__empty_expected)
{
    const uint8_t expected = 42u;
    data_chunk stack{ expected };
    const auto value = pop(stack);
    BOOST_REQUIRE(stack.empty());
    BOOST_REQUIRE_EQUAL(value, expected);
}

BOOST_AUTO_TEST_CASE(collection__pop__multiple__popped_expected)
{
    const uint8_t expected_value = 42u;
    data_chunk stack{ 0, 1, 2, 3, expected_value };
    const data_chunk expected_stack{ 0, 1, 2, 3 };
    const auto value = pop(stack);
    BOOST_REQUIRE_EQUAL(value, expected_value);
    BOOST_REQUIRE_EQUAL(stack, expected_stack);
}

// is_distinct

BOOST_AUTO_TEST_CASE(collection__is_distinct__empty__true)
{
    BOOST_REQUIRE(is_distinct(data_chunk{}));
}

BOOST_AUTO_TEST_CASE(collection__is_distinct__single__true)
{
    BOOST_REQUIRE(is_distinct(data_chunk{ 42 }));
}

BOOST_AUTO_TEST_CASE(collection__is_distinct__distinct__true)
{
    BOOST_REQUIRE(is_distinct(data_chunk{ 0, 2, 4, 6, 8 }));
}

BOOST_AUTO_TEST_CASE(collection__is_distinct__const_nondistinct__false)
{
    const data_chunk set{ 0, 2, 4, 2, 8 };
    BOOST_REQUIRE(!is_distinct(set));
}

// is_sorted

BOOST_AUTO_TEST_CASE(collection__is_sorted__empty__true)
{
    BOOST_REQUIRE(is_sorted(data_chunk{}));
}

BOOST_AUTO_TEST_CASE(collection__is_sorted__single__true)
{
    BOOST_REQUIRE(is_sorted(data_chunk{ 42 }));
}

BOOST_AUTO_TEST_CASE(collection__is_sorted__sorted_distinct__true)
{
    BOOST_REQUIRE(is_sorted(data_chunk{ 0, 2, 4, 6, 8 }));
}

BOOST_AUTO_TEST_CASE(collection__is_sorted__sorted_non_distinct__true)
{
    BOOST_REQUIRE(is_sorted(data_chunk{ 0, 2, 2, 6, 8 }));
}


BOOST_AUTO_TEST_CASE(collection__is_sorted__unsorted__false)
{
    BOOST_REQUIRE(!is_sorted(data_chunk{ 0, 2, 4, 2, 8 }));
}

// distinct

BOOST_AUTO_TEST_CASE(collection__distinct__empty__empty)
{
    BOOST_REQUIRE(distinct(data_chunk{}).empty());
}

BOOST_AUTO_TEST_CASE(collection__distinct__single__unchanged)
{
    const uint8_t expected = 42;
    BOOST_REQUIRE_EQUAL(distinct(data_chunk{ expected }).front(), expected);
}

BOOST_AUTO_TEST_CASE(collection__distinct__distinct_sorted__unchanged)
{
    const data_chunk expected{ 0, 2, 4, 6, 8 };
    BOOST_REQUIRE_EQUAL(distinct(data_chunk{ 0, 2, 4, 6, 8 }), expected);
}

BOOST_AUTO_TEST_CASE(collection__distinct__distinct_unsorted__sorted)
{
    const data_chunk expected{ 0, 2, 4, 6, 8 };
    BOOST_REQUIRE_EQUAL(distinct(data_chunk{ 2, 0, 8, 6, 4 }), expected);
}

BOOST_AUTO_TEST_CASE(collection__distinct__const_distinct_unsorted_duplicates__sorted_distinct)
{
    const data_chunk set{ 2, 0, 0, 8, 6, 4 };
    const data_chunk expected{ 0, 2, 4, 6, 8 };
    BOOST_REQUIRE_EQUAL(distinct(set), expected);
}

// reverse

BOOST_AUTO_TEST_CASE(collection__reverse__empty__empty)
{
    BOOST_REQUIRE(reverse(data_chunk{}).empty());
}

BOOST_AUTO_TEST_CASE(collection__reverse__single__unchanged)
{
    const uint8_t expected = 42;
    BOOST_REQUIRE_EQUAL(reverse(data_chunk{ expected }).front(), expected);
}

BOOST_AUTO_TEST_CASE(collection__reverse__const__reversed)
{
    const data_chunk set{ 2, 0, 0, 8, 6, 4 };
    const data_chunk expected{ 4, 6, 8, 0, 0, 2 };
    BOOST_REQUIRE_EQUAL(reverse(set), expected);
}

// sort

BOOST_AUTO_TEST_CASE(collection__sort__empty__empty)
{
    BOOST_REQUIRE(sort(data_chunk{}).empty());
}

BOOST_AUTO_TEST_CASE(collection__sort__single__unchanged)
{
    const uint8_t expected = 42;
    BOOST_REQUIRE_EQUAL(sort(data_chunk{ expected }).front(), expected);
}

BOOST_AUTO_TEST_CASE(collection__sort__const_unsorted__sorted)
{
    const data_chunk set{ 2, 0, 0, 8, 6, 4 };
    const data_chunk expected{ 0, 0, 2, 4, 6, 8 };
    BOOST_REQUIRE_EQUAL(sort(set), expected);
}

// starts_with

BOOST_AUTO_TEST_CASE(collection__starts_with__empty_empty__true)
{
    static const data_chunk buffer;
    static const data_chunk sequence;
    BOOST_REQUIRE(starts_with(buffer.begin(), buffer.end(), sequence));
}

BOOST_AUTO_TEST_CASE(collection__starts_with__not_empty_empty__false)
{
    static const data_chunk buffer;
    static const data_chunk sequence{ 42 };
    BOOST_REQUIRE(!starts_with(buffer.begin(), buffer.end(), sequence));
}

BOOST_AUTO_TEST_CASE(collection__starts_with__same_vectors__true)
{
    static const data_chunk buffer{ 42 };
    static const data_chunk sequence{ 42 };
    BOOST_REQUIRE(starts_with(buffer.begin(), buffer.end(), sequence));
}

BOOST_AUTO_TEST_CASE(collection__starts_with__matched_vectors__true)
{
    static const data_chunk buffer{ 42, 24, 15 };
    static const data_chunk sequence{ 42 };
    BOOST_REQUIRE(starts_with(buffer.begin(), buffer.end(), sequence));
}

BOOST_AUTO_TEST_CASE(collection__starts_with__mismatched_arrays__false)
{
    // Must be the same length when using arrays.
    static const data_array<2> buffer{ { 24, 0 } };
    static const data_array<2> sequence{ { 24, 42 } };
    const auto result = starts_with(buffer.begin(), buffer.end(), sequence);
    BOOST_REQUIRE(!result);
}

BOOST_AUTO_TEST_CASE(collection__starts_with__matched_arrays__true)
{
    // Must be the same length when using arrays.
    static const data_array<2> buffer{ { 24, 42 } };
    static const data_array<2> sequence{ { 24, 42 } };
    const auto result = starts_with(buffer.begin(), buffer.end(), sequence);
    BOOST_REQUIRE(result);
}

BOOST_AUTO_TEST_SUITE_END()
