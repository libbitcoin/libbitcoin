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

BOOST_AUTO_TEST_SUITE(data_slice_tests)

// construct, empty

BOOST_AUTO_TEST_CASE(data_slice__construct__default__empty)
{
    data_slice slice{};

    // properties (with value reads past end, zero padded)
    BOOST_REQUIRE(slice.empty());
    BOOST_REQUIRE_EQUAL(slice.size(), 0u);
    BOOST_REQUIRE_EQUAL(slice.data(), nullptr);
    BOOST_REQUIRE_EQUAL(slice.begin(), nullptr);
    BOOST_REQUIRE_EQUAL(slice.end(), nullptr);
    BOOST_REQUIRE_EQUAL(slice.front(), 0x00);
    BOOST_REQUIRE_EQUAL(slice.back(), 0x00);

    // methods
    BOOST_REQUIRE_EQUAL(slice.encoded(), "");
    BOOST_REQUIRE_EQUAL(slice.to_string(), "");
    BOOST_REQUIRE_EQUAL(slice.to_chunk(), data_chunk{});
    BOOST_REQUIRE_EQUAL(slice.to_array<0>(), data_array<0>{});

    // operator[] (value read past end, zero padded)
    BOOST_REQUIRE_EQUAL(slice[0], 0x00);

    // cast operators
    BOOST_REQUIRE(static_cast<data_chunk>(slice).empty());
    BOOST_REQUIRE_EQUAL(static_cast<data_array<32>>(slice), null_hash);

    // operator==/operator!=
    BOOST_REQUIRE(slice == slice);
    BOOST_REQUIRE(!(slice != slice));
    BOOST_REQUIRE(slice == std::string{});
    BOOST_REQUIRE(!(slice != std::string{}));
    BOOST_REQUIRE(slice == data_slice{});
    BOOST_REQUIRE(!(slice != data_slice{}));
    BOOST_REQUIRE(slice == data_array<0>{});
    BOOST_REQUIRE(!(slice != data_array<0>{}));
    BOOST_REQUIRE(slice == "");
    BOOST_REQUIRE(!(slice != ""));
}

BOOST_AUTO_TEST_CASE(data_slice__construct__text_empty__empty)
{
    BOOST_REQUIRE(data_slice("").empty());
}

BOOST_AUTO_TEST_CASE(data_slice__construct__array_empty__empty)
{
    BOOST_REQUIRE(data_slice(data_array<0>{}).empty());
}

BOOST_AUTO_TEST_CASE(data_slice__construct__string_empty__empty)
{
    BOOST_REQUIRE(data_slice(std::string{}).empty());
}

BOOST_AUTO_TEST_CASE(data_slice__construct__vector_empty__empty)
{
    BOOST_REQUIRE(data_slice(data_chunk{}).empty());
}

BOOST_AUTO_TEST_CASE(data_slice__construct__pointers_empty__empty)
{
    const std::string value{ "foobar" };
    BOOST_REQUIRE(data_slice(&value[0], &value[0]).empty());
}

BOOST_AUTO_TEST_CASE(data_slice__construct__pointers_reversed__empty)
{
    const std::string value{ "foobar" };
    BOOST_REQUIRE(data_slice(&value[5], &value[0]).empty());
}

BOOST_AUTO_TEST_CASE(data_slice__construct__iterators_empty__empty)
{
    const data_chunk value{};
    BOOST_REQUIRE(data_slice(value.begin(), value.end()).empty());
}

BOOST_AUTO_TEST_CASE(data_slice__construct__iterators_reversed__empty)
{
    const data_chunk value{ 'f', 'o', 'o', 'b', 'a', 'r' };
    BOOST_REQUIRE(data_slice(value.end(), value.begin()).empty());
}

BOOST_AUTO_TEST_CASE(data_slice__to_array__past_end__zero_padded)
{
    const auto size = 10u;
    const data_array<size> expected{ { 'f', 'o', 'o', 'b', 'a', 'r', 0, 0, 0, 0 } };
    const data_slice slice("foobar");
    BOOST_REQUIRE_EQUAL(slice.size(), 6u);
    BOOST_REQUIRE_EQUAL(slice.to_array<size>(), expected);
}

// test vector
const auto size = 6u;
const std::string string{ "foobar" };
const data_chunk data{ 'f', 'o', 'o', 'b', 'a', 'r' };
const data_array<size> byte{ { 'f', 'o', 'o', 'b', 'a', 'r' } };
const std::vector<char> char_vector{ 'f', 'o', 'o', 'b', 'a', 'r' };
const std::array<char, size> char_array{ { 'f', 'o', 'o', 'b', 'a', 'r' } };
const auto encoded = encode_base16(string);


// copy construct

BOOST_AUTO_TEST_CASE(data_slice__construct__copy__expected)
{
    const data_slice slice(string);
    const data_slice copy(slice);
    BOOST_REQUIRE_EQUAL(slice.size(), size);

    // size/data
    BOOST_REQUIRE(!slice.empty());
    BOOST_REQUIRE_EQUAL(slice.size(), size);
    BOOST_REQUIRE_EQUAL(*slice.data(), 'f');
    BOOST_REQUIRE_EQUAL(slice.back(), 'r');
    BOOST_REQUIRE_EQUAL(slice[sub1(size)], 'r');
}

// construct, not empty

BOOST_AUTO_TEST_CASE(data_slice__construct__text__expected)
{
    // negative vector
    const std::string negative_string{ "baadf00d" };
    const data_chunk negative_data{ 'b', 'a', 'a', 'd', 'f', '0', '0', 'd' };
    const data_array<8> negative_byte{ { 'b', 'a', 'a', 'd', 'f', '0', '0', 'd' } };

    // construct(literal)
    const data_slice slice("foobar");

    // size/data
    BOOST_REQUIRE(!slice.empty());
    BOOST_REQUIRE_EQUAL(slice.size(), size);
    BOOST_REQUIRE_EQUAL(*slice.data(), 'f');

    // begin
    BOOST_REQUIRE_EQUAL(slice.front(), 'f');
    BOOST_REQUIRE_EQUAL(slice[0], 'f');
    BOOST_REQUIRE_EQUAL(*slice.begin(), 'f');
    BOOST_REQUIRE_EQUAL(static_cast<data_chunk>(slice).front(), 'f');
    BOOST_REQUIRE_EQUAL(static_cast<data_array<size>>(slice).front(), 'f');
    
    // end
    BOOST_REQUIRE_EQUAL(slice.back(), 'r');
    BOOST_REQUIRE_EQUAL(slice[sub1(size)], 'r');
    BOOST_REQUIRE_EQUAL(*std::prev(slice.end()), 'r');
    BOOST_REQUIRE_EQUAL(static_cast<data_chunk>(slice).back(), 'r');
    BOOST_REQUIRE_EQUAL(static_cast<data_array<size>>(slice).back(), 'r');

    // methods
    BOOST_REQUIRE_EQUAL(slice.encoded(), encoded);
    BOOST_REQUIRE_EQUAL(slice.to_string(), string);
    BOOST_REQUIRE_EQUAL(slice.to_chunk(), data);
    BOOST_REQUIRE_EQUAL(slice.to_array<size>(), byte);

    // operator==/operator!=

    // positive left
    BOOST_REQUIRE(slice == string);
    BOOST_REQUIRE(!(slice != string));
    BOOST_REQUIRE(slice == data);
    BOOST_REQUIRE(!(slice != data));
    BOOST_REQUIRE(slice == byte);
    BOOST_REQUIRE(!(slice != byte));
    BOOST_REQUIRE(slice == "foobar");
    BOOST_REQUIRE(!(slice != "foobar"));
    BOOST_REQUIRE_EQUAL(slice, string);
    BOOST_REQUIRE_EQUAL(slice, data);
    BOOST_REQUIRE_EQUAL(slice, byte);
    BOOST_REQUIRE_EQUAL(slice, "foobar");

    // negative left
    BOOST_REQUIRE(slice != negative_string);
    BOOST_REQUIRE(!(slice == negative_string));
    BOOST_REQUIRE(slice != negative_data);
    BOOST_REQUIRE(!(slice == negative_data));
    BOOST_REQUIRE(slice != negative_byte);
    BOOST_REQUIRE(!(slice == negative_byte));
    BOOST_REQUIRE(slice != "baadf00d");
    BOOST_REQUIRE(!(slice == "baadf00d"));
    BOOST_REQUIRE_NE(slice, negative_string);
    BOOST_REQUIRE_NE(slice, negative_data);
    BOOST_REQUIRE_NE(slice, negative_byte);
    BOOST_REQUIRE_NE(slice, "baadf00d");

    // positive right
    BOOST_REQUIRE(string == slice);
    BOOST_REQUIRE(!(string != slice));
    BOOST_REQUIRE(data == slice);
    BOOST_REQUIRE(!(data != slice));
    BOOST_REQUIRE(byte == slice);
    BOOST_REQUIRE(!(byte != slice));
    BOOST_REQUIRE("foobar" == slice);
    BOOST_REQUIRE(!("foobar" != slice));
    BOOST_REQUIRE_EQUAL(string, slice);
    BOOST_REQUIRE_EQUAL(data, slice);
    BOOST_REQUIRE_EQUAL(byte, slice);
    BOOST_REQUIRE_EQUAL(data_slice("foobar"), slice);

    // negative right
    BOOST_REQUIRE(negative_string != slice);
    BOOST_REQUIRE(!(negative_string == slice));
    BOOST_REQUIRE(negative_data != slice);
    BOOST_REQUIRE(!(negative_data == slice));
    BOOST_REQUIRE(negative_byte != slice);
    BOOST_REQUIRE(!(negative_byte == slice));
    BOOST_REQUIRE("baadf00d" != slice);
    BOOST_REQUIRE(!("baadf00d" == slice));
    BOOST_REQUIRE_NE(negative_string, slice);
    BOOST_REQUIRE_NE(negative_data, slice);
    BOOST_REQUIRE_NE(negative_byte, slice);
    BOOST_REQUIRE_NE(data_slice("baadf00d"), slice);

    // construct(literal [not initializer - text])
    const data_slice slice1{ "foobar" };
    BOOST_REQUIRE(!slice1.empty());
    BOOST_REQUIRE_EQUAL(slice1.size(), size);
    BOOST_REQUIRE_EQUAL(slice1.encoded(), encoded);

    // Invalid, use:
    // data_slice slice("foobar");
    ////// construct(literal [not initializer - text])
    ////const data_slice slice2({ "foobar" });
    ////BOOST_REQUIRE(!slice2.empty());
    ////BOOST_REQUIRE_EQUAL(slice2.size(), size);
    ////BOOST_REQUIRE_EQUAL(slice2.encoded(), encoded);

    // construct(literal [not initializer - text])
    const data_slice slice3 = { "foobar" };
    BOOST_REQUIRE(!slice3.empty());
    BOOST_REQUIRE_EQUAL(slice3.size(), size);
    BOOST_REQUIRE_EQUAL(slice3.encoded(), encoded);
}

BOOST_AUTO_TEST_CASE(data_slice__construct__array__expected)
{
    // construct(data_array)
    const data_slice slice0(byte);
    BOOST_REQUIRE(!slice0.empty());
    BOOST_REQUIRE_EQUAL(slice0.size(), size);
    BOOST_REQUIRE_EQUAL(slice0.encoded(), encoded);

    // construct(data_array)
    const data_slice slice1{ byte };
    BOOST_REQUIRE(!slice1.empty());
    BOOST_REQUIRE_EQUAL(slice1.size(), size);
    BOOST_REQUIRE_EQUAL(slice1.encoded(), encoded);

    // Deleted function, use:
    // data_slice slice(byte);
    ////// construct(data_array)
    ////const data_slice slice2 = byte;
    ////BOOST_REQUIRE(!slice2.empty());
    ////BOOST_REQUIRE_EQUAL(slice2.size(), size);
    ////BOOST_REQUIRE_EQUAL(slice2.encoded(), encoded);

    // Array construction is non-ambiguous.
    // Unnecessary copy construction.
    // construct(data_array)
    const data_slice slice3({ byte });
    BOOST_REQUIRE(!slice3.empty());
    BOOST_REQUIRE_EQUAL(slice3.size(), size);
    BOOST_REQUIRE_EQUAL(slice3.encoded(), encoded);

    // construct(char_array)
    const data_slice slice4(char_array);
    BOOST_REQUIRE(!slice4.empty());
    BOOST_REQUIRE_EQUAL(slice4.size(), size);
    BOOST_REQUIRE_EQUAL(slice4.encoded(), encoded);
}

BOOST_AUTO_TEST_CASE(data_slice__construct__initializer__expected)
{
    // Invalid construction
    ////const data_slice slice0('f', 'o', 'o', 'b', 'a', 'r');
    ////BOOST_WARN(!slice0.empty());
    ////BOOST_WARN_EQUAL(slice0.size(), size);
    ////BOOST_WARN_EQUAL(slice0.encoded(), encoded);

    // construct(initializer - data)
    const data_slice slice1{ 'f', 'o', 'o', 'b', 'a', 'r' };
    BOOST_WARN(!slice1.empty());
    BOOST_WARN_EQUAL(slice1.size(), size);
    BOOST_WARN_EQUAL(slice1.encoded(), encoded);

    // construct(initializer - data)
    const data_slice slice2 = { 'f', 'o', 'o', 'b', 'a', 'r' };
    BOOST_WARN(!slice2.empty());
    BOOST_WARN_EQUAL(slice2.size(), size);
    BOOST_WARN_EQUAL(slice2.encoded(), encoded);

    // Invalid construction
    ////const data_slice slice3(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    ////BOOST_REQUIRE(!slice3.empty());
    ////BOOST_REQUIRE_EQUAL(slice3.size(), 10);
    ////BOOST_REQUIRE_EQUAL(slice3.encoded(), "00010203040506070809");

    // construct(initializer - data)
    const data_slice slice4{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    BOOST_WARN(!slice4.empty());
    BOOST_WARN_EQUAL(slice4.size(), 10);
    BOOST_WARN_EQUAL(slice4.encoded(), "00010203040506070809");

    // construct(initializer - data)
    const data_slice slice5 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    BOOST_WARN(!slice5.empty());
    BOOST_WARN_EQUAL(slice5.size(), 10);
    BOOST_WARN_EQUAL(slice5.encoded(), "00010203040506070809");

    // construct(initializer - data)
    // The non-char terminator causes the data initializer to be picked up.
    const data_slice slice6({ 'f', 'o', 'o', 'b', 'a', 'r', 0 });
    BOOST_WARN(!slice6.empty());
    BOOST_WARN_EQUAL(slice6.size(), add1(size));
    BOOST_WARN_EQUAL(slice6.encoded(), encoded + "00");

    // construct(initializer - text)
    // All characters is picked up by the char array initializer.
    // But unlike literal char arrays, it is not automatically null terminated.
    // Because data_slice sees this as a literal, it removes the last character.
    // So this initializer requires explicit null termination.
    const data_slice slice7({ 'f', 'o', 'o', 'b', 'a', 'r', '\0' });
    BOOST_WARN(!slice7.empty());
    BOOST_WARN_EQUAL(slice7.size(), size);
    BOOST_WARN_EQUAL(slice7.encoded(), encoded);
}

BOOST_AUTO_TEST_CASE(data_slice__construct__string__expected)
{
    // construct(string)
    const data_slice slice0(string);
    BOOST_REQUIRE(!slice0.empty());
    BOOST_REQUIRE_EQUAL(slice0.size(), size);
    BOOST_REQUIRE_EQUAL(slice0.encoded(), encoded);

    // construct(string)
    const data_slice slice1{ string };
    BOOST_REQUIRE(!slice1.empty());
    BOOST_REQUIRE_EQUAL(slice1.size(), size);
    BOOST_REQUIRE_EQUAL(slice1.encoded(), encoded);

    // Deleted function, use:
    // data_slice slice(string);
    ////// construct(string)
    ////const data_slice slice2 = string;
    ////BOOST_REQUIRE(!slice2.empty());
    ////BOOST_REQUIRE_EQUAL(slice2.size(), size);
    ////BOOST_REQUIRE_EQUAL(slice2.encoded(), encoded);

    // construct(string)
    const data_slice slice3 = { string };
    BOOST_REQUIRE(!slice3.empty());
    BOOST_REQUIRE_EQUAL(slice3.size(), size);
    BOOST_REQUIRE_EQUAL(slice3.encoded(), encoded);

    // Ambiguous, use:
    // data_slice slice(string);
    ////// Unnecessary copy construction.
    ////// construct(string)
    ////const data_slice slice4({ string });
    ////BOOST_REQUIRE(!slice4.empty());
    ////BOOST_REQUIRE_EQUAL(slice4.size(), size);
    ////BOOST_REQUIRE_EQUAL(slice4.encoded(), encoded);
}

BOOST_AUTO_TEST_CASE(data_slice__construct__vector__expected)
{
    // construct(data_chunk)
    const data_slice slice1(data);
    BOOST_REQUIRE(!slice1.empty());
    BOOST_REQUIRE_EQUAL(slice1.size(), size);
    BOOST_REQUIRE_EQUAL(slice1.encoded(), encoded);

    // Deleted function, use:
    // data_slice slice(data);
    ////// construct(data_chunk)
    ////const data_slice slice2 = data;
    ////BOOST_REQUIRE(!slice2.empty());
    ////BOOST_REQUIRE_EQUAL(slice2.size(), size);
    ////BOOST_REQUIRE_EQUAL(slice2.encoded(), encoded);

    // construct(data_chunk)
    const data_slice slice3 = { data };
    BOOST_REQUIRE(!slice3.empty());
    BOOST_REQUIRE_EQUAL(slice3.size(), size);
    BOOST_REQUIRE_EQUAL(slice3.encoded(), encoded);

    // Ambiguous, use:
    // data_slice slice(data);
    ////// Unnecessary copy construction.
    ////// construct(data_chunk)
    ////const data_slice slice4({ data });
    ////BOOST_REQUIRE(!slice4.empty());
    ////BOOST_REQUIRE_EQUAL(slice4.size(), size);
    ////BOOST_REQUIRE_EQUAL(slice4.encoded(), encoded);

    // construct(char_vector)
    const data_slice slice5(char_vector);
    BOOST_REQUIRE(!slice5.empty());
    BOOST_REQUIRE_EQUAL(slice5.size(), size);
    BOOST_REQUIRE_EQUAL(slice5.encoded(), encoded);
}

BOOST_AUTO_TEST_CASE(data_slice__construct__pointers__expected)
{
    // construct(pointers)
    const data_slice slice1(byte.data(), std::next(byte.data(), size));
    BOOST_REQUIRE(!slice1.empty());
    BOOST_REQUIRE_EQUAL(slice1.size(), size);
    BOOST_REQUIRE_EQUAL(slice1.encoded(), encoded);

    // construct(pointers)
    const data_slice slice2 = { byte.data(), std::next(byte.data(), size) };
    BOOST_REQUIRE(!slice2.empty());
    BOOST_REQUIRE_EQUAL(slice2.size(), size);
    BOOST_REQUIRE_EQUAL(slice2.encoded(), encoded);

    // Ambiguous, use:
    // data_slice slice(byte.data(), std::next(byte.data(), size));
    // Unnecessary copy construction.
    ////// construct(pointers)
    ////const data_slice slice3({ byte.data(), std::next(byte.data(), size) });
    ////BOOST_REQUIRE(!slice3.empty());
    ////BOOST_REQUIRE_EQUAL(slice3.size(), size);
    ////BOOST_REQUIRE_EQUAL(slice3.encoded(), encoded);
}

BOOST_AUTO_TEST_CASE(data_slice__construct__iterators__expected)
{
    // construct(iterators)
    const data_slice slice1(string.begin(), string.end());
    BOOST_REQUIRE(!slice1.empty());
    BOOST_REQUIRE_EQUAL(slice1.size(), size);
    BOOST_REQUIRE_EQUAL(slice1.encoded(), encoded);

    // construct(iterators)
    const data_slice slice2 = { string.begin(), string.end() };
    BOOST_REQUIRE(!slice2.empty());
    BOOST_REQUIRE_EQUAL(slice2.size(), size);
    BOOST_REQUIRE_EQUAL(slice2.encoded(), encoded);

    // Ambiguous, use:
    // data_slice slice(string.substr(,));
    // Unnecessary copy construction.
    ////// construct(iterators)
    ////const data_slice slice3({ string.begin(), string.end() });
    ////BOOST_REQUIRE(!slice3.empty());
    ////BOOST_REQUIRE_EQUAL(slice3.size(), size);
    ////BOOST_REQUIRE_EQUAL(slice3.encoded(), encoded);
}

// resize

BOOST_AUTO_TEST_CASE(data_slice__resize__empty__false)
{
    data_slice slice("");
    BOOST_REQUIRE(!slice.resize(0));
    BOOST_REQUIRE(!slice.resize(42));
}

BOOST_AUTO_TEST_CASE(data_slice__resize__downsize__true_expected)
{
    data_slice slice("foobar");
    BOOST_REQUIRE(slice.resize(3));
    BOOST_REQUIRE_EQUAL(slice.to_string(), "foo");
}

BOOST_AUTO_TEST_CASE(data_slice__resize__upsize__false_expected)
{
    data_slice slice("foobar");
    BOOST_REQUIRE(!slice.resize(6));
    BOOST_REQUIRE(!slice.resize(7));
    BOOST_REQUIRE(slice.resize(3));
    BOOST_REQUIRE(!slice.resize(3));
    BOOST_REQUIRE(!slice.resize(6));
    BOOST_REQUIRE_EQUAL(slice.to_string(), "foo");
}

BOOST_AUTO_TEST_SUITE_END()
