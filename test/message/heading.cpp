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

BOOST_AUTO_TEST_SUITE(heading_tests)

using namespace bc::system::message;

BOOST_AUTO_TEST_CASE(heading__constructor_default__always__invalid)
{
    const heading instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());
}

BOOST_AUTO_TEST_CASE(heading__constructor__always__expected_params)
{
    const uint32_t magic = 42;
    const auto command = "foo";
    const auto payload = to_chunk("bar");
    const heading instance(magic, command, payload);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(magic, instance.magic());
    BOOST_REQUIRE_EQUAL(command, instance.command());
    BOOST_REQUIRE_EQUAL(payload.size(), instance.payload_size());
}

BOOST_AUTO_TEST_CASE(heading__copy_constructor__always__valid_equal)
{
    const heading expected(42, "foo", to_chunk("bar"));
    const heading instance(expected);
    BOOST_REQUIRE(instance.is_valid());
}

BOOST_AUTO_TEST_CASE(heading__move_constructor__always__valid_equal)
{
    const heading expected(42, "foo", to_chunk("bar"));
    const heading instance(std::move(expected));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(heading__to_data__verify_checksum__expected)
{
    const auto payload = to_chunk("some data");
    const heading instance(42, "foo", payload);
    BOOST_REQUIRE(instance.verify_checksum(payload));
}

BOOST_AUTO_TEST_CASE(heading__from_data__insufficient_bytes__invalid)
{
    heading instance;
    BOOST_REQUIRE(!instance.from_data(data_chunk({ 42, 42 })));
}

BOOST_AUTO_TEST_CASE(heading__factory_1__sufficient_bytes__expected)
{
    const heading expected{ 42, "foo", to_chunk("barbarbarbarbarbarbar") };
    const auto data = expected.to_data();
    BOOST_REQUIRE_EQUAL(data.size(), heading::satoshi_fixed_size());

    const auto result = heading::factory(data);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(result == expected);
}

BOOST_AUTO_TEST_CASE(heading__factory_2__sufficient_bytes__expected)
{
    const heading expected(42, "foo", to_chunk("barbarbarbarbarbarbar"));
    const auto data = expected.to_data();
    BOOST_REQUIRE_EQUAL(data.size(), heading::satoshi_fixed_size());

    stream::in::copy istream(data);
    const auto result = heading::factory(istream);
    BOOST_REQUIRE(result.is_valid());
}

BOOST_AUTO_TEST_CASE(heading__factory_3__sufficient_bytes__expected)
{
    const heading expected(42, "foo", to_chunk("barbarbarbarbarbarbar"));
    const auto data = expected.to_data();
    BOOST_REQUIRE_EQUAL(data.size(), heading::satoshi_fixed_size());

    read::bytes::copy source(data);
    const auto result = heading::factory(source);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(result == expected);
}

BOOST_AUTO_TEST_CASE(heading__magic_accessor__constructor__expected)
{
    uint32_t expected = 42;
    const heading instance(expected, "foo", to_chunk("bar"));
    BOOST_REQUIRE_EQUAL(expected, instance.magic());
}

BOOST_AUTO_TEST_CASE(heading__magic_setter__accessor__round_trips)
{
    const uint32_t expected = 42;
    heading instance;
    BOOST_REQUIRE_EQUAL(instance.magic(), 0u);
    instance.set_magic(expected);
    BOOST_REQUIRE_EQUAL(instance.magic(), expected);
}

BOOST_AUTO_TEST_CASE(heading__command_accessor__constructor__expected)
{
    const auto expected = "foo";
    const heading instance(42, expected, to_chunk("bar"));
    BOOST_REQUIRE_EQUAL(instance.command(), expected);
}

BOOST_AUTO_TEST_CASE(heading__command_setter__accessor__round_trips)
{
    const auto expected = "foo";
    heading instance;
    instance.set_command(expected);
    BOOST_REQUIRE_EQUAL(instance.command(), expected);
}

BOOST_AUTO_TEST_CASE(heading__payload_size_accessor__always__expected)
{
    const auto payload = to_chunk("bar");
    const heading instance(42, "foo", payload);
    BOOST_REQUIRE_EQUAL(instance.payload_size(), payload.size());
}

BOOST_AUTO_TEST_CASE(heading__assignment__always__valid)
{
    heading value(42, "foo", to_chunk("bar"));
    BOOST_REQUIRE(value.is_valid());
    heading instance;
    BOOST_REQUIRE(!instance.is_valid());
    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
}

BOOST_AUTO_TEST_CASE(heading__equality__duplicates__true)
{
    heading expected(42, "foo", to_chunk("bar"));
    heading instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(heading__equality__different__false)
{
    heading expected(42, "foo", to_chunk("bar"));
    message::heading instance;
    BOOST_REQUIRE(!(instance == expected));
}

BOOST_AUTO_TEST_CASE(heading__inequality__duplicates__false)
{
    heading expected(42, "foo", to_chunk("bar"));
    heading instance(expected);
    BOOST_REQUIRE(!(instance != expected));
}

BOOST_AUTO_TEST_CASE(heading__inequality__different__true)
{
    heading expected(42, "foo", to_chunk("bar"));
    message::heading instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_CASE(heading__type__all_cases__match_expected)
{
    message::heading instance;
    BOOST_REQUIRE(message::message_type::unknown == instance.type());
    instance.set_command(message::address::command);
    BOOST_REQUIRE(message::message_type::address == instance.type());
    instance.set_command(message::alert::command);
    BOOST_REQUIRE(message::message_type::alert == instance.type());
    instance.set_command(message::block::command);
    BOOST_REQUIRE(message::message_type::block == instance.type());
    instance.set_command(message::block_transactions::command);
    BOOST_REQUIRE(message::message_type::block_transactions == instance.type());
    instance.set_command(message::compact_block::command);
    BOOST_REQUIRE(message::message_type::compact_block == instance.type());
    instance.set_command(message::filter_add::command);
    BOOST_REQUIRE(message::message_type::filter_add == instance.type());
    instance.set_command(message::filter_clear::command);
    BOOST_REQUIRE(message::message_type::filter_clear == instance.type());
    instance.set_command(message::filter_load::command);
    BOOST_REQUIRE(message::message_type::filter_load == instance.type());
    instance.set_command(message::get_address::command);
    BOOST_REQUIRE(message::message_type::get_address == instance.type());
    instance.set_command(message::get_block_transactions::command);
    BOOST_REQUIRE(message::message_type::get_block_transactions == instance.type());
    instance.set_command(message::get_blocks::command);
    BOOST_REQUIRE(message::message_type::get_blocks == instance.type());
    instance.set_command(message::get_data::command);
    BOOST_REQUIRE(message::message_type::get_data == instance.type());
    instance.set_command(message::get_headers::command);
    BOOST_REQUIRE(message::message_type::get_headers == instance.type());
    instance.set_command(message::headers::command);
    BOOST_REQUIRE(message::message_type::headers == instance.type());
    instance.set_command(message::inventory::command);
    BOOST_REQUIRE(message::message_type::inventory == instance.type());
    instance.set_command(message::memory_pool::command);
    BOOST_REQUIRE(message::message_type::memory_pool == instance.type());
    instance.set_command(message::merkle_block::command);
    BOOST_REQUIRE(message::message_type::merkle_block == instance.type());
    instance.set_command(message::not_found::command);
    BOOST_REQUIRE(message::message_type::not_found == instance.type());
    instance.set_command(message::ping::command);
    BOOST_REQUIRE(message::message_type::ping == instance.type());
    instance.set_command(message::pong::command);
    BOOST_REQUIRE(message::message_type::pong == instance.type());
    instance.set_command(message::reject::command);
    BOOST_REQUIRE(message::message_type::reject == instance.type());
    instance.set_command(message::send_compact::command);
    BOOST_REQUIRE(message::message_type::send_compact == instance.type());
    instance.set_command(message::send_headers::command);
    BOOST_REQUIRE(message::message_type::send_headers == instance.type());
    instance.set_command(message::transaction::command);
    BOOST_REQUIRE(message::message_type::transaction == instance.type());
    instance.set_command(message::verack::command);
    BOOST_REQUIRE(message::message_type::verack == instance.type());
    instance.set_command(message::version::command);
    BOOST_REQUIRE(message::message_type::version == instance.type());
}

BOOST_AUTO_TEST_CASE(heading__maximum_size__always__matches_satoshi_fixed_size)
{
    BOOST_REQUIRE_EQUAL(heading::satoshi_fixed_size(), heading::maximum_size());
}

BOOST_AUTO_TEST_CASE(heading__maximum_payload_size__non_witness__matches_expected)
{
    const size_t expected = 3u + (sizeof(uint32_t) + hash_size) * 50000u;
    BOOST_REQUIRE_EQUAL(expected, heading::maximum_payload_size(0u, false));
}

BOOST_AUTO_TEST_CASE(heading__maximum_payload_size__witness__matches_expected)
{
    BOOST_REQUIRE_EQUAL(chain::max_block_weight, heading::maximum_payload_size(0u, true));
}

BOOST_AUTO_TEST_SUITE_END()
