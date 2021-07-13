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

BOOST_AUTO_TEST_SUITE(data_slab_tests)

// construct, empty

BOOST_AUTO_TEST_CASE(data_slab__construct__default__empty)
{
    data_slab slab{};

    // properties (with value reads past end, zero padded)
    BOOST_REQUIRE(slab.empty());
    BOOST_REQUIRE_EQUAL(slab.size(), 0u);
    BOOST_REQUIRE_EQUAL(slab.data(), nullptr);
    BOOST_REQUIRE_EQUAL(slab.begin(), nullptr);
    BOOST_REQUIRE_EQUAL(slab.end(), nullptr);
    BOOST_REQUIRE_EQUAL(slab.front(), 0x00);
    BOOST_REQUIRE_EQUAL(slab.back(), 0x00);

    // methods
    BOOST_REQUIRE_EQUAL(slab.encoded(), "");
    BOOST_REQUIRE_EQUAL(slab.to_string(), "");
    BOOST_REQUIRE_EQUAL(slab.to_chunk(), data_chunk{});
    BOOST_REQUIRE_EQUAL(slab.to_array<0>(), data_array<0>{});

    // operator[] (value read past end, zero padded)
    BOOST_REQUIRE_EQUAL(slab[0], 0x00);

    // cast operators
    BOOST_REQUIRE(static_cast<data_chunk>(slab).empty());
    BOOST_REQUIRE_EQUAL(static_cast<data_array<32>>(slab), null_hash);

    // operator==/operator!=
    BOOST_REQUIRE(slab == slab);
    BOOST_REQUIRE(!(slab != slab));
    BOOST_REQUIRE(slab == data_slab{});
    BOOST_REQUIRE(!(slab != data_slab{}));
    BOOST_REQUIRE(slab == data_array<0>{});
    BOOST_REQUIRE(!(slab != data_array<0>{}));
}

BOOST_AUTO_TEST_CASE(data_slab__construct__array_empty__empty)
{
    BOOST_REQUIRE(data_slab(data_array<0>{}).empty());
}

BOOST_AUTO_TEST_CASE(data_slab__construct__pointers_empty__empty)
{
    std::string value{ "foobar" };
    BOOST_REQUIRE(data_slab(&value[0], &value[0]).empty());
}

BOOST_AUTO_TEST_CASE(data_slab__construct__pointers_reversed__empty)
{
    std::string value{ "foobar" };
    BOOST_REQUIRE(data_slab(&value[5], &value[0]).empty());
}

BOOST_AUTO_TEST_CASE(data_slab__construct__iterators_empty__empty)
{
    data_chunk value{};
    BOOST_REQUIRE(data_slab(value.begin(), value.end()).empty());
}

BOOST_AUTO_TEST_CASE(data_slab__construct__iterators_reversed__empty)
{
    data_chunk value{ 'f', 'o', 'o', 'b', 'a', 'r' };
    BOOST_REQUIRE(data_slab(value.end(), value.begin()).empty());
}

// test vector (careful to not mutate in tests).
const auto size = 6u;
std::string string{ "foobar" };
data_chunk data{ 'f', 'o', 'o', 'b', 'a', 'r' };
data_array<size> byte{ { 'f', 'o', 'o', 'b', 'a', 'r' } };
std::array<char, size> char_array{ { 'f', 'o', 'o', 'b', 'a', 'r' } };
data_array<8> negative_byte{ { 'b', 'a', 'a', 'd', 'f', '0', '0', 'd' } };
auto encoded = encode_base16(string);


// copy construct

BOOST_AUTO_TEST_CASE(data_slab__construct__copy__expected)
{
    const data_slab slab(byte);
    const data_slab copy(slab);
    BOOST_REQUIRE_EQUAL(slab.size(), size);

    // size/data
    BOOST_REQUIRE(!slab.empty());
    BOOST_REQUIRE_EQUAL(slab.size(), size);
    BOOST_REQUIRE_EQUAL(*slab.data(), 'f');
    BOOST_REQUIRE_EQUAL(slab.back(), 'r');
    BOOST_REQUIRE_EQUAL(slab[sub1(size)], 'r');
}

// construct, not empty

BOOST_AUTO_TEST_CASE(data_slab__construct__text__expected)
{
    // negative vector
    std::string negative_string{ "baadf00d" };
    data_chunk negative_data{ 'b', 'a', 'a', 'd', 'f', '0', '0', 'd' };
    data_array<8> negative_byte{ { 'b', 'a', 'a', 'd', 'f', '0', '0', 'd' } };

    // construct(string reference)
    const data_slab slab(string);

    // size/data
    BOOST_REQUIRE(!slab.empty());
    BOOST_REQUIRE_EQUAL(slab.size(), size);
    BOOST_REQUIRE_EQUAL(*slab.data(), 'f');

    // begin
    BOOST_REQUIRE_EQUAL(slab.front(), 'f');
    BOOST_REQUIRE_EQUAL(slab[0], 'f');
    BOOST_REQUIRE_EQUAL(*slab.begin(), 'f');
    BOOST_REQUIRE_EQUAL(static_cast<data_chunk>(slab).front(), 'f');
    BOOST_REQUIRE_EQUAL(static_cast<data_array<size>>(slab).front(), 'f');
    
    // end
    BOOST_REQUIRE_EQUAL(slab.back(), 'r');
    BOOST_REQUIRE_EQUAL(slab[sub1(size)], 'r');
    BOOST_REQUIRE_EQUAL(*std::prev(slab.end()), 'r');
    BOOST_REQUIRE_EQUAL(static_cast<data_chunk>(slab).back(), 'r');
    BOOST_REQUIRE_EQUAL(static_cast<data_slice>(slab).back(), 'r');
    BOOST_REQUIRE_EQUAL(static_cast<data_array<size>>(slab).back(), 'r');

    // methods
    BOOST_REQUIRE_EQUAL(slab.encoded(), encoded);
    BOOST_REQUIRE_EQUAL(slab.to_string(), string);
    BOOST_REQUIRE_EQUAL(slab.to_chunk(), data);
    BOOST_REQUIRE_EQUAL(slab.to_slice(), data);
    BOOST_REQUIRE_EQUAL(slab.to_array<size>(), byte);

    // operator==/operator!=

    // positive left
    BOOST_REQUIRE(slab == string);
    BOOST_REQUIRE(!(slab != string));
    BOOST_REQUIRE(slab == data);
    BOOST_REQUIRE(!(slab != data));
    BOOST_REQUIRE(slab == byte);
    BOOST_REQUIRE(!(slab != byte));
    BOOST_REQUIRE_EQUAL(slab, string);
    BOOST_REQUIRE_EQUAL(slab, data);
    BOOST_REQUIRE_EQUAL(slab, byte);

    // negative left
    BOOST_REQUIRE(slab != negative_string);
    BOOST_REQUIRE(!(slab == negative_string));
    BOOST_REQUIRE(slab != negative_data);
    BOOST_REQUIRE(!(slab == negative_data));
    BOOST_REQUIRE(slab != negative_byte);
    BOOST_REQUIRE(!(slab == negative_byte));
    BOOST_REQUIRE_NE(slab, negative_string);
    BOOST_REQUIRE_NE(slab, negative_data);
    BOOST_REQUIRE_NE(slab, negative_byte);

    // positive right
    BOOST_REQUIRE(string == slab);
    BOOST_REQUIRE(!(string != slab));
    BOOST_REQUIRE(data == slab);
    BOOST_REQUIRE(!(data != slab));
    BOOST_REQUIRE(byte == slab);
    BOOST_REQUIRE(!(byte != slab));
    BOOST_REQUIRE_EQUAL(string, slab);
    BOOST_REQUIRE_EQUAL(data, slab);
    BOOST_REQUIRE_EQUAL(byte, slab);

    // negative right
    BOOST_REQUIRE(negative_string != slab);
    BOOST_REQUIRE(!(negative_string == slab));
    BOOST_REQUIRE(negative_data != slab);
    BOOST_REQUIRE(!(negative_data == slab));
    BOOST_REQUIRE(negative_byte != slab);
    BOOST_REQUIRE(!(negative_byte == slab));
    BOOST_REQUIRE_NE(negative_string, slab);
    BOOST_REQUIRE_NE(negative_data, slab);
    BOOST_REQUIRE_NE(negative_byte, slab);
}

BOOST_AUTO_TEST_CASE(data_slab__construct__array__expected)
{
    // construct(data_array)
    const data_slab slab0(byte);
    BOOST_REQUIRE(!slab0.empty());
    BOOST_REQUIRE_EQUAL(slab0.size(), size);
    BOOST_REQUIRE_EQUAL(slab0.encoded(), encoded);

    // construct(data_array)
    const data_slab slab1{ byte };
    BOOST_REQUIRE(!slab1.empty());
    BOOST_REQUIRE_EQUAL(slab1.size(), size);
    BOOST_REQUIRE_EQUAL(slab1.encoded(), encoded);

    // Array construction is non-ambiguous.
    // Unnecessary copy construction.
    // construct(data_array)
    const data_slab slab3({ byte });
    BOOST_REQUIRE(!slab3.empty());
    BOOST_REQUIRE_EQUAL(slab3.size(), size);
    BOOST_REQUIRE_EQUAL(slab3.encoded(), encoded);

    // construct(char_array)
    const data_slab slab4(char_array);
    BOOST_REQUIRE(!slab4.empty());
    BOOST_REQUIRE_EQUAL(slab4.size(), size);
    BOOST_REQUIRE_EQUAL(slab4.encoded(), encoded);
}

BOOST_AUTO_TEST_CASE(data_slab__construct__pointers__expected)
{
    // construct(pointers)
    const data_slab slab1(byte.data(), std::next(byte.data(), size));
    BOOST_REQUIRE(!slab1.empty());
    BOOST_REQUIRE_EQUAL(slab1.size(), size);
    BOOST_REQUIRE_EQUAL(slab1.encoded(), encoded);

    // construct(pointers)
    const data_slab slab2 = { byte.data(), std::next(byte.data(), size) };
    BOOST_REQUIRE(!slab2.empty());
    BOOST_REQUIRE_EQUAL(slab2.size(), size);
    BOOST_REQUIRE_EQUAL(slab2.encoded(), encoded);
}

BOOST_AUTO_TEST_CASE(data_slab__construct__iterators__expected)
{
    // construct(iterators)
    const data_slab slab1(string.begin(), string.end());
    BOOST_REQUIRE(!slab1.empty());
    BOOST_REQUIRE_EQUAL(slab1.size(), size);
    BOOST_REQUIRE_EQUAL(slab1.encoded(), encoded);

    // construct(iterators)
    const data_slab slab2 = { string.begin(), string.end() };
    BOOST_REQUIRE(!slab2.empty());
    BOOST_REQUIRE_EQUAL(slab2.size(), size);
    BOOST_REQUIRE_EQUAL(slab2.encoded(), encoded);
}

// resize

BOOST_AUTO_TEST_CASE(data_slab__resize__empty__false)
{
    data_array<0> empty{ {} };
    data_slab slab(empty);
    BOOST_REQUIRE(!slab.resize(0));
    BOOST_REQUIRE(!slab.resize(42));
}

BOOST_AUTO_TEST_CASE(data_slab__resize__downsize__true_expected)
{
    data_array<6> data{ { 'f', 'o', 'o', 'b', 'a', 'r' } };
    data_slab slab(data);
    BOOST_REQUIRE(slab.resize(3));
    BOOST_REQUIRE_EQUAL(slab.to_string(), "foo");
}

BOOST_AUTO_TEST_CASE(data_slab__resize__upsize__false_expected)
{
    data_array<6> data{ { 'f', 'o', 'o', 'b', 'a', 'r' } };
    data_slab slab(data);
    BOOST_REQUIRE(!slab.resize(6));
    BOOST_REQUIRE(!slab.resize(7));
    BOOST_REQUIRE(slab.resize(3));
    BOOST_REQUIRE(!slab.resize(3));
    BOOST_REQUIRE(!slab.resize(6));
    BOOST_REQUIRE_EQUAL(slab.to_string(), "foo");
}

BOOST_AUTO_TEST_SUITE_END()
