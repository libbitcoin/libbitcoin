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

BOOST_AUTO_TEST_SUITE(output_tests)

const auto valid_raw_output = base16_chunk("20300500000000001976a914905f933de850988603aafeeb2fd7fce61e66fe5d88ac");

BOOST_AUTO_TEST_CASE(output__constructor_1__always__returns_default_initialized)
{
    chain::output instance;
    BOOST_REQUIRE(!instance.is_valid());
}

BOOST_AUTO_TEST_CASE(output__constructor_2__valid_input__returns_input_initialized)
{
    uint64_t value = 643u;
    chain::script script;
    const auto data = base16_chunk("ece424a6bb6ddf4db592c0faed60685047a361b1");
    BOOST_REQUIRE(script.from_data(data, false));

    chain::output instance(value, script);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(script == instance.script());
    BOOST_REQUIRE_EQUAL(value, instance.value());
}

BOOST_AUTO_TEST_CASE(output__constructor_3__valid_input__returns_input_initialized)
{
    uint64_t value = 643u;
    chain::script script;
    const auto data = base16_chunk("ece424a6bb6ddf4db592c0faed60685047a361b1");
    BOOST_REQUIRE(script.from_data(data, false));

    // This must be non-const.
    auto dup_script = script;

    chain::output instance(value, std::move(dup_script));

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(script == instance.script());
    BOOST_REQUIRE_EQUAL(value, instance.value());
}

BOOST_AUTO_TEST_CASE(output__constructor_4__valid_input__returns_input_initialized)
{
    chain::output expected;
    BOOST_REQUIRE(expected.from_data(valid_raw_output));

    chain::output instance(expected);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(expected == instance);
}

BOOST_AUTO_TEST_CASE(output__constructor_5__valid_input__returns_input_initialized)
{
    // This must be non-const.
    chain::output expected;
    BOOST_REQUIRE(expected.from_data(valid_raw_output));

    chain::output instance(std::move(expected));
    BOOST_REQUIRE(instance.is_valid());
}

BOOST_AUTO_TEST_CASE(output__from_data__insufficient_bytes__failure)
{
    data_chunk data(2);

    chain::output instance;

    BOOST_REQUIRE(!instance.from_data(data));
    BOOST_REQUIRE(!instance.is_valid());
}

BOOST_AUTO_TEST_CASE(output__factory_1__valid_input_success)
{
    auto instance = chain::output::factory(valid_raw_output);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(instance.serialized_size(), valid_raw_output.size());

    // Re-save and compare against original.
    data_chunk resave = instance.to_data();
    BOOST_REQUIRE_EQUAL(resave.size(), valid_raw_output.size());
    BOOST_REQUIRE(resave == valid_raw_output);
}

BOOST_AUTO_TEST_CASE(output__factory_2__valid_input_success)
{
    stream::in::copy stream(valid_raw_output);
    auto instance = chain::output::factory(stream);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(instance.serialized_size(), valid_raw_output.size());

    // Re-save and compare against original.
    data_chunk resave = instance.to_data();
    BOOST_REQUIRE_EQUAL(resave.size(), valid_raw_output.size());
    BOOST_REQUIRE(resave == valid_raw_output);
}

BOOST_AUTO_TEST_CASE(output__factory_3__valid_input_success)
{
    read::bytes::copy source(valid_raw_output);
    auto instance = chain::output::factory(source);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(instance.serialized_size(), valid_raw_output.size());

    // Re-save and compare against original.
    data_chunk resave = instance.to_data();
    BOOST_REQUIRE_EQUAL(resave.size(), valid_raw_output.size());
    BOOST_REQUIRE(resave == valid_raw_output);
}

BOOST_AUTO_TEST_CASE(output__signature_operations__always__returns_script_sigops_false)
{
    chain::output instance;
    BOOST_REQUIRE_EQUAL(instance.script().sigops(false), instance.signature_operations(false));
}

BOOST_AUTO_TEST_CASE(output__value__roundtrip__success)
{
    uint64_t expected = 523542u;
    chain::output instance;
    BOOST_REQUIRE(expected != instance.value());
    instance.set_value(expected);
    BOOST_REQUIRE_EQUAL(expected, instance.value());
}

BOOST_AUTO_TEST_CASE(output__script_setter_1__roundtrip__success)
{
    chain::script value;
    const auto data = base16_chunk("ece424a6bb6ddf4db592c0faed60685047a361b1");
     BOOST_REQUIRE(value.from_data(data, false));

    chain::output instance;
    BOOST_REQUIRE(value != instance.script());
    instance.set_script(value);
    BOOST_REQUIRE(value == instance.script());
    const auto& restricted = instance;
    BOOST_REQUIRE(value == instance.script());
    BOOST_REQUIRE(value == restricted.script());
}

BOOST_AUTO_TEST_CASE(output__script_setter_2__roundtrip__success)
{
    chain::script value;
    const auto data = base16_chunk("ece424a6bb6ddf4db592c0faed60685047a361b1");
    BOOST_REQUIRE(value.from_data(data, false));

    // This must be non-const.
    auto dup_value = value;

    chain::output instance;
    BOOST_REQUIRE(value != instance.script());
    instance.set_script(std::move(dup_value));
    BOOST_REQUIRE(value == instance.script());
    const auto& restricted = instance;
    BOOST_REQUIRE(value == instance.script());
    BOOST_REQUIRE(value == restricted.script());
}

BOOST_AUTO_TEST_CASE(output__operator_assign_equals_1__always__matches_equivalent)
{
    chain::output expected;
    BOOST_REQUIRE(expected.from_data(valid_raw_output));
    chain::output instance;
    instance = chain::output::factory(valid_raw_output);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(output__operator_assign_equals_2__always__matches_equivalent)
{
    chain::output expected;
    BOOST_REQUIRE(expected.from_data(valid_raw_output));
    chain::output instance;
    instance = expected;
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(output__operator_boolean_equals__duplicates__returns_true)
{
    chain::output alpha;
    chain::output beta;
    BOOST_REQUIRE(alpha.from_data(valid_raw_output));
    BOOST_REQUIRE(beta.from_data(valid_raw_output));
    BOOST_REQUIRE(alpha == beta);
}

BOOST_AUTO_TEST_CASE(output__operator_boolean_equals__differs__returns_false)
{
    chain::output alpha;
    chain::output beta;
    BOOST_REQUIRE(alpha.from_data(valid_raw_output));
    BOOST_REQUIRE_EQUAL(false, alpha == beta);
}

BOOST_AUTO_TEST_CASE(output__operator_boolean_not_equals__duplicates__returns_false)
{
    chain::output alpha;
    chain::output beta;
    BOOST_REQUIRE(alpha.from_data(valid_raw_output));
    BOOST_REQUIRE(beta.from_data(valid_raw_output));
    BOOST_REQUIRE_EQUAL(false, alpha != beta);
}

BOOST_AUTO_TEST_CASE(output__operator_boolean_not_equals__differs__returns_true)
{
    chain::output alpha;
    chain::output beta;
    BOOST_REQUIRE(alpha.from_data(valid_raw_output));
    BOOST_REQUIRE(alpha != beta);
}

BOOST_AUTO_TEST_SUITE_END()
