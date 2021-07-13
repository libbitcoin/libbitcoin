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
#include <array>
#include <iterator>
#include <string>
#include <vector>

BOOST_AUTO_TEST_SUITE(data_reference_tests)

// test vector
const auto size = 6u;
const std::string string{ "foobar" };
const data_chunk data{ 'f', 'o', 'o', 'b', 'a', 'r' };
const data_array<size> byte{ { 'f', 'o', 'o', 'b', 'a', 'r' } };
const std::vector<char> char_vector{ 'f', 'o', 'o', 'b', 'a', 'r' };
const std::array<char, size> char_array{ { 'f', 'o', 'o', 'b', 'a', 'r' } };
const auto encoded = encode_base16(string);

// construct, empty

BOOST_AUTO_TEST_CASE(data_reference__construct__default__empty)
{
    data_reference reference{};
    data_reference reference2(string);

    // properties (with value reads past end, zero padded)
    BOOST_REQUIRE(reference.empty());
    BOOST_REQUIRE_EQUAL(reference.size(), 0u);
    BOOST_REQUIRE_EQUAL(reference.data(), nullptr);
    BOOST_REQUIRE_EQUAL(reference.begin(), nullptr);
    BOOST_REQUIRE_EQUAL(reference.end(), nullptr);
    BOOST_REQUIRE_EQUAL(reference.front(), 0x00);
    BOOST_REQUIRE_EQUAL(reference.back(), 0x00);

    // methods
    BOOST_REQUIRE_EQUAL(reference.encoded(), "");
    BOOST_REQUIRE_EQUAL(reference.to_string(), "");
    BOOST_REQUIRE_EQUAL(reference.to_chunk(), data_chunk{});
    BOOST_REQUIRE_EQUAL(reference.to_array<0>(), data_array<0>{});

    // operator[] (value read past end, zero padded)
    BOOST_REQUIRE_EQUAL(reference[0], 0x00);

    // cast operators
    BOOST_REQUIRE(static_cast<data_chunk>(reference).empty());
    BOOST_REQUIRE_EQUAL(static_cast<data_array<32>>(reference), null_hash);

    // operator==/operator!=
    BOOST_REQUIRE(reference == reference);
    BOOST_REQUIRE(!(reference != reference));
    BOOST_REQUIRE(!(reference == reference2));
    BOOST_REQUIRE(reference != reference2);
}

BOOST_AUTO_TEST_CASE(data_reference__construct__pointers_empty__empty)
{
    const std::string value{ "foobar" };
    BOOST_REQUIRE(data_reference(&value[0], &value[0]).empty());
}

BOOST_AUTO_TEST_CASE(data_reference__construct__pointers_reversed__empty)
{
    const std::string value{ "foobar" };
    BOOST_REQUIRE(data_reference(&value[5], &value[0]).empty());
}

BOOST_AUTO_TEST_CASE(data_reference__construct__iterators_empty__empty)
{
    const data_chunk value{};
    BOOST_REQUIRE(data_reference(value.begin(), value.end()).empty());
}

BOOST_AUTO_TEST_CASE(data_reference__construct__iterators_reversed__empty)
{
    const data_chunk value{ 'f', 'o', 'o', 'b', 'a', 'r' };
    BOOST_REQUIRE(data_reference(value.end(), value.begin()).empty());
}

// copy construct

BOOST_AUTO_TEST_CASE(data_reference__construct__copy__expected)
{
    const data_reference reference(string);
    const data_reference copy(reference);
    BOOST_REQUIRE_EQUAL(reference.size(), size);

    // size/data
    BOOST_REQUIRE(!reference.empty());
    BOOST_REQUIRE_EQUAL(reference.size(), size);
    BOOST_REQUIRE_EQUAL(*reference.data(), 'f');
    BOOST_REQUIRE_EQUAL(reference.back(), 'r');
    BOOST_REQUIRE_EQUAL(reference[sub1(size)], 'r');
}

// construct, not empty

BOOST_AUTO_TEST_CASE(data_reference__construct__string__expected)
{
    // negative vector
    const std::string negative_string{ "baadf00d" };
    const data_chunk negative_data{ 'b', 'a', 'a', 'd', 'f', '0', '0', 'd' };
    const data_array<8> negative_byte{ { 'b', 'a', 'a', 'd', 'f', '0', '0', 'd' } };

    // construct(literal)
    const data_reference reference(string);

    // size/data
    BOOST_REQUIRE(!reference.empty());
    BOOST_REQUIRE_EQUAL(reference.size(), size);
    BOOST_REQUIRE_EQUAL(*reference.data(), 'f');

    // begin
    BOOST_REQUIRE_EQUAL(reference.front(), 'f');
    BOOST_REQUIRE_EQUAL(reference[0], 'f');
    BOOST_REQUIRE_EQUAL(*reference.begin(), 'f');
    BOOST_REQUIRE_EQUAL(static_cast<data_chunk>(reference).front(), 'f');
    BOOST_REQUIRE_EQUAL(static_cast<data_array<size>>(reference).front(), 'f');
    
    // end
    BOOST_REQUIRE_EQUAL(reference.back(), 'r');
    BOOST_REQUIRE_EQUAL(reference[sub1(size)], 'r');
    BOOST_REQUIRE_EQUAL(*std::prev(reference.end()), 'r');
    BOOST_REQUIRE_EQUAL(static_cast<data_chunk>(reference).back(), 'r');
    BOOST_REQUIRE_EQUAL(static_cast<data_array<size>>(reference).back(), 'r');

    // methods
    BOOST_REQUIRE_EQUAL(reference.encoded(), encoded);
    BOOST_REQUIRE_EQUAL(reference.to_string(), string);
    BOOST_REQUIRE_EQUAL(reference.to_chunk(), data);
    BOOST_REQUIRE_EQUAL(reference.to_array<size>(), byte);

    // operator==/operator!=

    // positive left
    BOOST_REQUIRE(reference == string);
    BOOST_REQUIRE(!(reference != string));
    BOOST_REQUIRE(reference == data);
    BOOST_REQUIRE(!(reference != data));
    BOOST_REQUIRE(reference == byte);
    BOOST_REQUIRE(!(reference != byte));
    BOOST_REQUIRE_EQUAL(reference, string);
    BOOST_REQUIRE_EQUAL(reference, data);
    BOOST_REQUIRE_EQUAL(reference, byte);

    // negative left
    BOOST_REQUIRE(reference != negative_string);
    BOOST_REQUIRE(!(reference == negative_string));
    BOOST_REQUIRE(reference != negative_data);
    BOOST_REQUIRE(!(reference == negative_data));
    BOOST_REQUIRE(reference != negative_byte);
    BOOST_REQUIRE(!(reference == negative_byte));
    BOOST_REQUIRE_NE(reference, negative_string);
    BOOST_REQUIRE_NE(reference, negative_data);
    BOOST_REQUIRE_NE(reference, negative_byte);

    // positive right
    BOOST_REQUIRE(string == reference);
    BOOST_REQUIRE(!(string != reference));
    BOOST_REQUIRE(data == reference);
    BOOST_REQUIRE(!(data != reference));
    BOOST_REQUIRE(byte == reference);
    BOOST_REQUIRE(!(byte != reference));
    BOOST_REQUIRE_EQUAL(string, reference);
    BOOST_REQUIRE_EQUAL(data, reference);
    BOOST_REQUIRE_EQUAL(byte, reference);

    // negative right
    BOOST_REQUIRE(negative_string != reference);
    BOOST_REQUIRE(!(negative_string == reference));
    BOOST_REQUIRE(negative_data != reference);
    BOOST_REQUIRE(!(negative_data == reference));
    BOOST_REQUIRE(negative_byte != reference);
    BOOST_REQUIRE(!(negative_byte == reference));
    BOOST_REQUIRE_NE(negative_string, reference);
    BOOST_REQUIRE_NE(negative_data, reference);
    BOOST_REQUIRE_NE(negative_byte, reference);
}

BOOST_AUTO_TEST_CASE(data_reference__construct__array__expected)
{
    // construct(data_array)
    const data_reference reference0(byte);
    BOOST_REQUIRE(!reference0.empty());
    BOOST_REQUIRE_EQUAL(reference0.size(), size);
    BOOST_REQUIRE_EQUAL(reference0.encoded(), encoded);

    // construct(data_array)
    const data_reference reference1{ byte };
    BOOST_REQUIRE(!reference1.empty());
    BOOST_REQUIRE_EQUAL(reference1.size(), size);
    BOOST_REQUIRE_EQUAL(reference1.encoded(), encoded);

    // construct(char_array)
    const data_reference reference4(char_array);
    BOOST_REQUIRE(!reference4.empty());
    BOOST_REQUIRE_EQUAL(reference4.size(), size);
    BOOST_REQUIRE_EQUAL(reference4.encoded(), encoded);
}

BOOST_AUTO_TEST_CASE(data_reference__construct__vector__expected)
{
    // construct(data_chunk)
    const data_reference reference1(data);
    BOOST_REQUIRE(!reference1.empty());
    BOOST_REQUIRE_EQUAL(reference1.size(), size);
    BOOST_REQUIRE_EQUAL(reference1.encoded(), encoded);

    // construct(data_chunk)
    const data_reference reference3 = { data };
    BOOST_REQUIRE(!reference3.empty());
    BOOST_REQUIRE_EQUAL(reference3.size(), size);
    BOOST_REQUIRE_EQUAL(reference3.encoded(), encoded);

    // construct(char_vector)
    const data_reference reference5(char_vector);
    BOOST_REQUIRE(!reference5.empty());
    BOOST_REQUIRE_EQUAL(reference5.size(), size);
    BOOST_REQUIRE_EQUAL(reference5.encoded(), encoded);
}

BOOST_AUTO_TEST_CASE(data_reference__construct__pointers__expected)
{
    // construct(pointers)
    const data_reference reference1(byte.data(), std::next(byte.data(), size));
    BOOST_REQUIRE(!reference1.empty());
    BOOST_REQUIRE_EQUAL(reference1.size(), size);
    BOOST_REQUIRE_EQUAL(reference1.encoded(), encoded);

    // construct(pointers)
    const data_reference reference2 = { byte.data(), std::next(byte.data(), size) };
    BOOST_REQUIRE(!reference2.empty());
    BOOST_REQUIRE_EQUAL(reference2.size(), size);
    BOOST_REQUIRE_EQUAL(reference2.encoded(), encoded);
}

BOOST_AUTO_TEST_CASE(data_reference__construct__iterators__expected)
{
    // construct(iterators)
    const data_reference reference1(string.begin(), string.end());
    BOOST_REQUIRE(!reference1.empty());
    BOOST_REQUIRE_EQUAL(reference1.size(), size);
    BOOST_REQUIRE_EQUAL(reference1.encoded(), encoded);

    // construct(iterators)
    const data_reference reference2 = { string.begin(), string.end() };
    BOOST_REQUIRE(!reference2.empty());
    BOOST_REQUIRE_EQUAL(reference2.size(), size);
    BOOST_REQUIRE_EQUAL(reference2.encoded(), encoded);
}

// resize

BOOST_AUTO_TEST_CASE(data_reference__resize__empty__false)
{
    const std::string empty;
    data_reference reference(empty);
    BOOST_REQUIRE(!reference.resize(0));
    BOOST_REQUIRE(!reference.resize(42));
}

BOOST_AUTO_TEST_CASE(data_reference__resize__downsize__true_expected)
{
    data_reference reference(string);
    BOOST_REQUIRE(reference.resize(3));
    BOOST_REQUIRE_EQUAL(reference.to_string(), "foo");
}

BOOST_AUTO_TEST_CASE(data_reference__resize__upsize__false_expected)
{
    data_reference reference(string);
    BOOST_REQUIRE(!reference.resize(6));
    BOOST_REQUIRE(!reference.resize(7));
    BOOST_REQUIRE(reference.resize(3));
    BOOST_REQUIRE(!reference.resize(3));
    BOOST_REQUIRE(!reference.resize(6));
    BOOST_REQUIRE_EQUAL(reference.to_string(), "foo");
}

BOOST_AUTO_TEST_SUITE_END()
