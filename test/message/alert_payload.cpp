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

BOOST_AUTO_TEST_SUITE(alert_payload_tests)

BOOST_AUTO_TEST_CASE(alert_payload__constructor_1__always__invalid)
{
    message::alert_payload instance;
    BOOST_REQUIRE(!instance.is_valid());
}

BOOST_AUTO_TEST_CASE(alert_payload__constructor_2__always__equals_params)
{
    const uint32_t version = 3452u;
    const uint64_t relay_until = 64556u;
    const uint64_t expiration = 78545u;
    const uint32_t id = 43547u;
    const uint32_t cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = { 2345u, 346754u, 234u, 4356u };
    const uint32_t min_version = 4644u;
    const uint32_t max_version = 89876u;
    const std::vector<std::string> set_sub_version = { "foo", "bar", "baz" };
    const uint32_t priority = 34323u;
    const std::string comment = "asfgsddsa";
    const std::string status_bar = "fgjdfhjg";
    const std::string reserved = "utyurtevc";

    message::alert_payload instance(version, relay_until, expiration, id,
        cancel, set_cancel, min_version, max_version, set_sub_version,
        priority, comment, status_bar, reserved);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(version, instance.version());
    BOOST_REQUIRE_EQUAL(relay_until, instance.relay_until());
    BOOST_REQUIRE_EQUAL(expiration, instance.expiration());
    BOOST_REQUIRE_EQUAL(id, instance.id());
    BOOST_REQUIRE_EQUAL(cancel, instance.cancel());
    BOOST_REQUIRE(set_cancel == instance.set_cancel());
    BOOST_REQUIRE_EQUAL(min_version, instance.min_version());
    BOOST_REQUIRE_EQUAL(max_version, instance.max_version());
    BOOST_REQUIRE(set_sub_version == instance.set_sub_version());
    BOOST_REQUIRE_EQUAL(priority, instance.priority());
    BOOST_REQUIRE_EQUAL(comment, instance.comment());
    BOOST_REQUIRE_EQUAL(status_bar, instance.status_bar());
    BOOST_REQUIRE_EQUAL(reserved, instance.reserved());
}

BOOST_AUTO_TEST_CASE(alert_payload__constructor_3__always__equals_params)
{
    const uint32_t version = 3452u;
    const uint64_t relay_until = 64556u;
    const uint64_t expiration = 78545u;
    const uint32_t id = 43547u;
    const uint32_t cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = { 2345u, 346754u, 234u, 4356u };
    const uint32_t min_version = 4644u;
    const uint32_t max_version = 89876u;
    const std::vector<std::string> set_sub_version = { "foo", "bar", "baz" };
    const uint32_t priority = 34323u;
    const std::string comment = "asfgsddsa";
    const std::string status_bar = "fgjdfhjg";
    const std::string reserved = "utyurtevc";

    auto dup_set_cancel = set_cancel;
    auto dup_set_sub_version = set_sub_version;
    auto dup_comment = comment;
    auto dup_status_bar = status_bar;
    auto dup_reserved = reserved;

    message::alert_payload instance(version, relay_until, expiration, id,
        cancel, std::move(dup_set_cancel), min_version, max_version,
        std::move(dup_set_sub_version), priority, std::move(dup_comment),
        std::move(dup_status_bar), std::move(dup_reserved));

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(version, instance.version());
    BOOST_REQUIRE_EQUAL(relay_until, instance.relay_until());
    BOOST_REQUIRE_EQUAL(expiration, instance.expiration());
    BOOST_REQUIRE_EQUAL(id, instance.id());
    BOOST_REQUIRE_EQUAL(cancel, instance.cancel());
    BOOST_REQUIRE(set_cancel == instance.set_cancel());
    BOOST_REQUIRE_EQUAL(min_version, instance.min_version());
    BOOST_REQUIRE_EQUAL(max_version, instance.max_version());
    BOOST_REQUIRE(set_sub_version == instance.set_sub_version());
    BOOST_REQUIRE_EQUAL(priority, instance.priority());
    BOOST_REQUIRE_EQUAL(comment, instance.comment());
    BOOST_REQUIRE_EQUAL(status_bar, instance.status_bar());
    BOOST_REQUIRE_EQUAL(reserved, instance.reserved());
}

BOOST_AUTO_TEST_CASE(alert_payload__constructor_4__always__equals_params)
{
    const uint32_t version = 3452u;
    const uint64_t relay_until = 64556u;
    const uint64_t expiration = 78545u;
    const uint32_t id = 43547u;
    const uint32_t cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = { 2345u, 346754u, 234u, 4356u };
    const uint32_t min_version = 4644u;
    const uint32_t max_version = 89876u;
    const std::vector<std::string> set_sub_version = { "foo", "bar", "baz" };
    const uint32_t priority = 34323u;
    const std::string comment = "asfgsddsa";
    const std::string status_bar = "fgjdfhjg";
    const std::string reserved = "utyurtevc";

    message::alert_payload value(version, relay_until, expiration, id,
        cancel, set_cancel, min_version, max_version, set_sub_version,
        priority, comment, status_bar, reserved);

    message::alert_payload instance(value);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(version, instance.version());
    BOOST_REQUIRE_EQUAL(relay_until, instance.relay_until());
    BOOST_REQUIRE_EQUAL(expiration, instance.expiration());
    BOOST_REQUIRE_EQUAL(id, instance.id());
    BOOST_REQUIRE_EQUAL(cancel, instance.cancel());
    BOOST_REQUIRE(set_cancel == instance.set_cancel());
    BOOST_REQUIRE_EQUAL(min_version, instance.min_version());
    BOOST_REQUIRE_EQUAL(max_version, instance.max_version());
    BOOST_REQUIRE(set_sub_version == instance.set_sub_version());
    BOOST_REQUIRE_EQUAL(priority, instance.priority());
    BOOST_REQUIRE_EQUAL(comment, instance.comment());
    BOOST_REQUIRE_EQUAL(status_bar, instance.status_bar());
    BOOST_REQUIRE_EQUAL(reserved, instance.reserved());
}

BOOST_AUTO_TEST_CASE(alert_payload__constructor_5__always__equals_params)
{
    const uint32_t version = 3452u;
    const uint64_t relay_until = 64556u;
    const uint64_t expiration = 78545u;
    const uint32_t id = 43547u;
    const uint32_t cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = { 2345u, 346754u, 234u, 4356u };
    const uint32_t min_version = 4644u;
    const uint32_t max_version = 89876u;
    const std::vector<std::string> set_sub_version = { "foo", "bar", "baz" };
    const uint32_t priority = 34323u;
    const std::string comment = "asfgsddsa";
    const std::string status_bar = "fgjdfhjg";
    const std::string reserved = "utyurtevc";

    message::alert_payload value(version, relay_until, expiration, id,
        cancel, set_cancel, min_version, max_version, set_sub_version,
        priority, comment, status_bar, reserved);

    message::alert_payload instance(std::move(value));

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(version, instance.version());
    BOOST_REQUIRE_EQUAL(relay_until, instance.relay_until());
    BOOST_REQUIRE_EQUAL(expiration, instance.expiration());
    BOOST_REQUIRE_EQUAL(id, instance.id());
    BOOST_REQUIRE_EQUAL(cancel, instance.cancel());
    BOOST_REQUIRE(set_cancel == instance.set_cancel());
    BOOST_REQUIRE_EQUAL(min_version, instance.min_version());
    BOOST_REQUIRE_EQUAL(max_version, instance.max_version());
    BOOST_REQUIRE(set_sub_version == instance.set_sub_version());
    BOOST_REQUIRE_EQUAL(priority, instance.priority());
    BOOST_REQUIRE_EQUAL(comment, instance.comment());
    BOOST_REQUIRE_EQUAL(status_bar, instance.status_bar());
    BOOST_REQUIRE_EQUAL(reserved, instance.reserved());
}

BOOST_AUTO_TEST_CASE(alert_payload__from_data__insufficient_bytes__failure)
{
    data_chunk raw{ 0xab, 0x11 };
    message::alert_payload instance;

    BOOST_REQUIRE(!instance.from_data(message::version::level::minimum, raw));
    BOOST_REQUIRE(!instance.is_valid());
}

BOOST_AUTO_TEST_CASE(alert_payload__factory_1__wiki_sample_test__success)
{
    const data_chunk raw
    {
        0x01, 0x00, 0x00, 0x00, 0x37, 0x66, 0x40, 0x4f, 0x00, 0x00,
        0x00, 0x00, 0xb3, 0x05, 0x43, 0x4f, 0x00, 0x00, 0x00, 0x00,
        0xf2, 0x03, 0x00, 0x00, 0xf1, 0x03, 0x00, 0x00, 0x00, 0x10,
        0x27, 0x00, 0x00, 0x48, 0xee, 0x00, 0x00, 0x00, 0x64, 0x00,
        0x00, 0x00, 0x00, 0x46, 0x53, 0x65, 0x65, 0x20, 0x62, 0x69,
        0x74, 0x63, 0x6f, 0x69, 0x6e, 0x2e, 0x6f, 0x72, 0x67, 0x2f,
        0x66, 0x65, 0x62, 0x32, 0x30, 0x20, 0x69, 0x66, 0x20, 0x79,
        0x6f, 0x75, 0x20, 0x68, 0x61, 0x76, 0x65, 0x20, 0x74, 0x72,
        0x6f, 0x75, 0x62, 0x6c, 0x65, 0x20, 0x63, 0x6f, 0x6e, 0x6e,
        0x65, 0x63, 0x74, 0x69, 0x6e, 0x67, 0x20, 0x61, 0x66, 0x74,
        0x65, 0x72, 0x20, 0x32, 0x30, 0x20, 0x46, 0x65, 0x62, 0x72,
        0x75, 0x61, 0x72, 0x79, 0x00
    };

    const message::alert_payload expected
    {
        1,
        1329620535,
        1329792435,
        1010,
        1009,
        std::vector<uint32_t>{},
        10000,
        61000,
        std::vector<std::string>{},
        100,
        "",
        "See bitcoin.org/feb20 if you have trouble connecting after 20 February",
        ""
    };

    const auto result = message::alert_payload::factory(
        message::version::level::minimum, raw);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE_EQUAL(raw.size(), result.serialized_size(message::version::level::minimum));
    BOOST_REQUIRE(result == expected);

    const auto data = expected.to_data(message::version::level::minimum);

    BOOST_REQUIRE_EQUAL(raw.size(), data.size());
    BOOST_REQUIRE_EQUAL(data.size(), expected.serialized_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_CASE(alert_payload__factory_1__roundtrip__success)
{
    message::alert_payload expected
    {
        5,
        105169,
        723544,
        1779,
        1678,
        {
            10, 25256, 37, 98485, 250
        },
        75612,
        81354,
        {
            "alpha", "beta", "gamma", "delta"
        },
        781,
        "My Comment",
        "My Status Bar",
        "RESERVED?"
    };

    const auto data = expected.to_data(message::version::level::minimum);
    const auto result = message::alert_payload::factory(
        message::version::level::minimum, data);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(message::version::level::minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::minimum), result.serialized_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_CASE(alert_payload__factory_2__roundtrip__success)
{
    message::alert_payload expected
    {
        5,
        105169,
        723544,
        1779,
        1678,
        {
            10, 25256, 37, 98485, 250
        },
        75612,
        81354,
        {
            "alpha", "beta", "gamma", "delta"
        },
        781,
        "My Comment",
        "My Status Bar",
        "RESERVED?"
    };

    const auto data = expected.to_data(message::version::level::minimum);
    stream::in::copy istream(data);
    const auto result = message::alert_payload::factory(
        message::version::level::minimum, istream);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(message::version::level::minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::minimum), result.serialized_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_CASE(alert_payload__factory_3__roundtrip__success)
{
    const message::alert_payload expected
    {
        5,
        105169,
        723544,
        1779,
        1678,
        {
            10, 25256, 37, 98485, 250
        },
        75612,
        81354,
        {
            "alpha", "beta", "gamma", "delta"
        },
        781,
        "My Comment",
        "My Status Bar",
        "RESERVED?"
    };

    const auto data = expected.to_data(message::version::level::minimum);
    read::bytes::copy source(data);
    const auto result = message::alert_payload::factory(message::version::level::minimum, source);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(message::version::level::minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::minimum), result.serialized_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_CASE(alert_payload__version__roundtrip__success)
{
    uint32_t value = 1234u;
    message::alert_payload instance;
    BOOST_REQUIRE(instance.version() != value);
    instance.set_version(value);
    BOOST_REQUIRE_EQUAL(value, instance.version());
}

BOOST_AUTO_TEST_CASE(alert_payload__relay_until__roundtrip__success)
{
    uint64_t value = 5121234u;
    message::alert_payload instance;
    BOOST_REQUIRE(instance.relay_until() != value);
    instance.set_relay_until(value);
    BOOST_REQUIRE_EQUAL(value, instance.relay_until());
}

BOOST_AUTO_TEST_CASE(alert_payload__expiration__roundtrip__success)
{
    uint64_t value = 5121234u;
    message::alert_payload instance;
    BOOST_REQUIRE(instance.expiration() != value);
    instance.set_expiration(value);
    BOOST_REQUIRE_EQUAL(value, instance.expiration());
}

BOOST_AUTO_TEST_CASE(alert_payload__id__roundtrip__success)
{
    uint32_t value = 68215u;
    message::alert_payload instance;
    BOOST_REQUIRE(instance.id() != value);
    instance.set_id(value);
    BOOST_REQUIRE_EQUAL(value, instance.id());
}

BOOST_AUTO_TEST_CASE(alert_payload__cancel__roundtrip__success)
{
    uint32_t value = 68215u;
    message::alert_payload instance;
    BOOST_REQUIRE(instance.cancel() != value);
    instance.set_cancel(value);
    BOOST_REQUIRE_EQUAL(value, instance.cancel());
}

BOOST_AUTO_TEST_CASE(alert_payload__set_cancel_accessor_1__always__returns_initialized)
{
    const uint32_t version = 3452u;
    const uint64_t relay_until = 64556u;
    const uint64_t expiration = 78545u;
    const uint32_t id = 43547u;
    const uint32_t cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = { 2345u, 346754u, 234u, 4356u };
    const uint32_t min_version = 4644u;
    const uint32_t max_version = 89876u;
    const std::vector<std::string> set_sub_version = { "foo", "bar", "baz" };
    const uint32_t priority = 34323u;
    const std::string comment = "asfgsddsa";
    const std::string status_bar = "fgjdfhjg";
    const std::string reserved = "utyurtevc";

    message::alert_payload instance(version, relay_until, expiration, id,
        cancel, set_cancel, min_version, max_version, set_sub_version,
        priority, comment, status_bar, reserved);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(set_cancel == instance.set_cancel());
}

BOOST_AUTO_TEST_CASE(alert_payload__set_cancel_accessor_2__always__returns_initialized)
{
    const uint32_t version = 3452u;
    const uint64_t relay_until = 64556u;
    const uint64_t expiration = 78545u;
    const uint32_t id = 43547u;
    const uint32_t cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = { 2345u, 346754u, 234u, 4356u };
    const uint32_t min_version = 4644u;
    const uint32_t max_version = 89876u;
    const std::vector<std::string> set_sub_version = { "foo", "bar", "baz" };
    const uint32_t priority = 34323u;
    const std::string comment = "asfgsddsa";
    const std::string status_bar = "fgjdfhjg";
    const std::string reserved = "utyurtevc";

    const message::alert_payload instance(version, relay_until, expiration, id,
        cancel, set_cancel, min_version, max_version, set_sub_version,
        priority, comment, status_bar, reserved);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(set_cancel == instance.set_cancel());
}

BOOST_AUTO_TEST_CASE(alert_payload__set_cancel_setter_1__roundtrip__success)
{
    const std::vector<uint32_t> value = { 68215u, 34542u, 4756u };
    message::alert_payload instance;
    BOOST_REQUIRE(instance.set_cancel() != value);
    instance.set_set_cancel(value);
    BOOST_REQUIRE(value == instance.set_cancel());
}

BOOST_AUTO_TEST_CASE(alert_payload__set_cancel_setter_2__roundtrip__success)
{
    const std::vector<uint32_t> value = { 68215u, 34542u, 4756u };
    auto dup_value = value;
    message::alert_payload instance;
    BOOST_REQUIRE(instance.set_cancel() != value);
    instance.set_set_cancel(std::move(dup_value));
    BOOST_REQUIRE(value == instance.set_cancel());
}

BOOST_AUTO_TEST_CASE(alert_payload__min_version__roundtrip__success)
{
    uint32_t value = 68215u;
    message::alert_payload instance;
    BOOST_REQUIRE(instance.min_version() != value);
    instance.set_min_version(value);
    BOOST_REQUIRE_EQUAL(value, instance.min_version());
}

BOOST_AUTO_TEST_CASE(alert_payload__max_version__roundtrip__success)
{
    uint32_t value = 68215u;
    message::alert_payload instance;
    BOOST_REQUIRE(instance.max_version() != value);
    instance.set_max_version(value);
    BOOST_REQUIRE_EQUAL(value, instance.max_version());
}

BOOST_AUTO_TEST_CASE(alert_payload__set_sub_version_accessor_1__always__returns_initialized)
{
    const uint32_t version = 3452u;
    const uint64_t relay_until = 64556u;
    const uint64_t expiration = 78545u;
    const uint32_t id = 43547u;
    const uint32_t cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = { 2345u, 346754u, 234u, 4356u };
    const uint32_t min_version = 4644u;
    const uint32_t max_version = 89876u;
    const std::vector<std::string> set_sub_version = { "foo", "bar", "baz" };
    const uint32_t priority = 34323u;
    const std::string comment = "asfgsddsa";
    const std::string status_bar = "fgjdfhjg";
    const std::string reserved = "utyurtevc";

    message::alert_payload instance(version, relay_until, expiration, id,
        cancel, set_cancel, min_version, max_version, set_sub_version,
        priority, comment, status_bar, reserved);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(set_sub_version == instance.set_sub_version());
}

BOOST_AUTO_TEST_CASE(alert_payload__set_sub_version_accessor_2__always__returns_initialized)
{
    const uint32_t version = 3452u;
    const uint64_t relay_until = 64556u;
    const uint64_t expiration = 78545u;
    const uint32_t id = 43547u;
    const uint32_t cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = { 2345u, 346754u, 234u, 4356u };
    const uint32_t min_version = 4644u;
    const uint32_t max_version = 89876u;
    const std::vector<std::string> set_sub_version = { "foo", "bar", "baz" };
    const uint32_t priority = 34323u;
    const std::string comment = "asfgsddsa";
    const std::string status_bar = "fgjdfhjg";
    const std::string reserved = "utyurtevc";

    const message::alert_payload instance(version, relay_until, expiration, id,
        cancel, set_cancel, min_version, max_version, set_sub_version,
        priority, comment, status_bar, reserved);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(set_sub_version == instance.set_sub_version());
}

BOOST_AUTO_TEST_CASE(alert_payload__set_sub_version_setter_1__roundtrip__success)
{
    const std::vector<std::string> value = { "asdfa", "sgfdf", "Tryertsd" };
    message::alert_payload instance;
    BOOST_REQUIRE(instance.set_sub_version() != value);
    instance.set_set_sub_version(value);
    BOOST_REQUIRE(value == instance.set_sub_version());
}

BOOST_AUTO_TEST_CASE(alert_payload__set_sub_version_setter_2__roundtrip__success)
{
    const std::vector<std::string> value = { "asdfa", "sgfdf", "Tryertsd" };
    auto dup_value = value;
    message::alert_payload instance;
    BOOST_REQUIRE(instance.set_sub_version() != value);
    instance.set_set_sub_version(std::move(dup_value));
    BOOST_REQUIRE(value == instance.set_sub_version());
}

BOOST_AUTO_TEST_CASE(alert_payload__priority__roundtrip__success)
{
    uint32_t value = 68215u;
    message::alert_payload instance;
    BOOST_REQUIRE(instance.priority() != value);
    instance.set_priority(value);
    BOOST_REQUIRE_EQUAL(value, instance.priority());
}

BOOST_AUTO_TEST_CASE(alert_payload__comment_accessor_1__always__returns_initialized)
{
    const uint32_t version = 3452u;
    const uint64_t relay_until = 64556u;
    const uint64_t expiration = 78545u;
    const uint32_t id = 43547u;
    const uint32_t cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = { 2345u, 346754u, 234u, 4356u };
    const uint32_t min_version = 4644u;
    const uint32_t max_version = 89876u;
    const std::vector<std::string> set_sub_version = { "foo", "bar", "baz" };
    const uint32_t priority = 34323u;
    const std::string comment = "asfgsddsa";
    const std::string status_bar = "fgjdfhjg";
    const std::string reserved = "utyurtevc";

    message::alert_payload instance(version, relay_until, expiration, id,
        cancel, set_cancel, min_version, max_version, set_sub_version,
        priority, comment, status_bar, reserved);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(comment, instance.comment());
}

BOOST_AUTO_TEST_CASE(alert_payload__comment_accessor_2__always__returns_initialized)
{
    const uint32_t version = 3452u;
    const uint64_t relay_until = 64556u;
    const uint64_t expiration = 78545u;
    const uint32_t id = 43547u;
    const uint32_t cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = { 2345u, 346754u, 234u, 4356u };
    const uint32_t min_version = 4644u;
    const uint32_t max_version = 89876u;
    const std::vector<std::string> set_sub_version = { "foo", "bar", "baz" };
    const uint32_t priority = 34323u;
    const std::string comment = "asfgsddsa";
    const std::string status_bar = "fgjdfhjg";
    const std::string reserved = "utyurtevc";

    const message::alert_payload instance(version, relay_until, expiration, id,
        cancel, set_cancel, min_version, max_version, set_sub_version,
        priority, comment, status_bar, reserved);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(comment, instance.comment());
}

BOOST_AUTO_TEST_CASE(alert_payload__comment_setter_1__roundtrip__success)
{
    const std::string value = "asdfa";
    message::alert_payload instance;
    BOOST_REQUIRE(instance.comment() != value);
    instance.set_comment(value);
    BOOST_REQUIRE(value == instance.comment());
}

BOOST_AUTO_TEST_CASE(alert_payload__comment_setter_2__roundtrip__success)
{
    const std::string value = "Tryertsd";
    auto dup_value = value;
    message::alert_payload instance;
    BOOST_REQUIRE(instance.comment() != value);
    instance.set_comment(std::move(dup_value));
    BOOST_REQUIRE(value == instance.comment());
}

BOOST_AUTO_TEST_CASE(alert_payload__status_bar_accessor_1__always__returns_initialized)
{
    const uint32_t version = 3452u;
    const uint64_t relay_until = 64556u;
    const uint64_t expiration = 78545u;
    const uint32_t id = 43547u;
    const uint32_t cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = { 2345u, 346754u, 234u, 4356u };
    const uint32_t min_version = 4644u;
    const uint32_t max_version = 89876u;
    const std::vector<std::string> set_sub_version = { "foo", "bar", "baz" };
    const uint32_t priority = 34323u;
    const std::string comment = "asfgsddsa";
    const std::string status_bar = "fgjdfhjg";
    const std::string reserved = "utyurtevc";

    message::alert_payload instance(version, relay_until, expiration, id,
        cancel, set_cancel, min_version, max_version, set_sub_version,
        priority, comment, status_bar, reserved);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(status_bar, instance.status_bar());
}

BOOST_AUTO_TEST_CASE(alert_payload__status_bar_accessor_2__always__returns_initialized)
{
    const uint32_t version = 3452u;
    const uint64_t relay_until = 64556u;
    const uint64_t expiration = 78545u;
    const uint32_t id = 43547u;
    const uint32_t cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = { 2345u, 346754u, 234u, 4356u };
    const uint32_t min_version = 4644u;
    const uint32_t max_version = 89876u;
    const std::vector<std::string> set_sub_version = { "foo", "bar", "baz" };
    const uint32_t priority = 34323u;
    const std::string comment = "asfgsddsa";
    const std::string status_bar = "fgjdfhjg";
    const std::string reserved = "utyurtevc";

    const message::alert_payload instance(version, relay_until, expiration, id,
        cancel, set_cancel, min_version, max_version, set_sub_version,
        priority, comment, status_bar, reserved);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(status_bar, instance.status_bar());
}

BOOST_AUTO_TEST_CASE(alert_payload__status_bar_setter_1__roundtrip__success)
{
    const std::string value = "asdfa";
    message::alert_payload instance;
    BOOST_REQUIRE(instance.status_bar() != value);
    instance.set_status_bar(value);
    BOOST_REQUIRE(value == instance.status_bar());
}

BOOST_AUTO_TEST_CASE(alert_payload__status_bar_setter_2__roundtrip__success)
{
    const std::string value = "Tryertsd";
    auto dup_value = value;
    message::alert_payload instance;
    BOOST_REQUIRE(instance.status_bar() != value);
    instance.set_status_bar(std::move(dup_value));
    BOOST_REQUIRE(value == instance.status_bar());
}

BOOST_AUTO_TEST_CASE(alert_payload__reserved_accessor_1__always__returns_initialized)
{
    const uint32_t version = 3452u;
    const uint64_t relay_until = 64556u;
    const uint64_t expiration = 78545u;
    const uint32_t id = 43547u;
    const uint32_t cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = { 2345u, 346754u, 234u, 4356u };
    const uint32_t min_version = 4644u;
    const uint32_t max_version = 89876u;
    const std::vector<std::string> set_sub_version = { "foo", "bar", "baz" };
    const uint32_t priority = 34323u;
    const std::string comment = "asfgsddsa";
    const std::string status_bar = "fgjdfhjg";
    const std::string reserved = "utyurtevc";

    message::alert_payload instance(version, relay_until, expiration, id,
        cancel, set_cancel, min_version, max_version, set_sub_version,
        priority, comment, status_bar, reserved);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(reserved, instance.reserved());
}

BOOST_AUTO_TEST_CASE(alert_payload__reserved_accessor_2__always__returns_initialized)
{
    const uint32_t version = 3452u;
    const uint64_t relay_until = 64556u;
    const uint64_t expiration = 78545u;
    const uint32_t id = 43547u;
    const uint32_t cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = { 2345u, 346754u, 234u, 4356u };
    const uint32_t min_version = 4644u;
    const uint32_t max_version = 89876u;
    const std::vector<std::string> set_sub_version = { "foo", "bar", "baz" };
    const uint32_t priority = 34323u;
    const std::string comment = "asfgsddsa";
    const std::string status_bar = "fgjdfhjg";
    const std::string reserved = "utyurtevc";

    const message::alert_payload instance(version, relay_until, expiration, id,
        cancel, set_cancel, min_version, max_version, set_sub_version,
        priority, comment, status_bar, reserved);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(reserved, instance.reserved());
}

BOOST_AUTO_TEST_CASE(alert_payload__reserved_setter_1__roundtrip__success)
{
    const std::string value = "asdfa";
    message::alert_payload instance;
    BOOST_REQUIRE(instance.reserved() != value);
    instance.set_reserved(value);
    BOOST_REQUIRE(value == instance.reserved());
}

BOOST_AUTO_TEST_CASE(alert_payload__reserved_setter_2__roundtrip__success)
{
    const std::string value = "Tryertsd";
    auto dup_value = value;
    message::alert_payload instance;
    BOOST_REQUIRE(instance.reserved() != value);
    instance.set_reserved(std::move(dup_value));
    BOOST_REQUIRE(value == instance.reserved());
}

BOOST_AUTO_TEST_CASE(alert_payload__operator_assign_equals__always__matches_equivalent)
{
    const uint32_t version = 3452u;
    const uint64_t relay_until = 64556u;
    const uint64_t expiration = 78545u;
    const uint32_t id = 43547u;
    const uint32_t cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = { 2345u, 346754u, 234u, 4356u };
    const uint32_t min_version = 4644u;
    const uint32_t max_version = 89876u;
    const std::vector<std::string> set_sub_version = { "foo", "bar", "baz" };
    const uint32_t priority = 34323u;
    const std::string comment = "asfgsddsa";
    const std::string status_bar = "fgjdfhjg";
    const std::string reserved = "utyurtevc";

    message::alert_payload value(version, relay_until, expiration, id,
        cancel, set_cancel, min_version, max_version, set_sub_version,
        priority, comment, status_bar, reserved);

    BOOST_REQUIRE(value.is_valid());

    message::alert_payload instance;
    BOOST_REQUIRE(!instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(version, instance.version());
    BOOST_REQUIRE_EQUAL(relay_until, instance.relay_until());
    BOOST_REQUIRE_EQUAL(expiration, instance.expiration());
    BOOST_REQUIRE_EQUAL(id, instance.id());
    BOOST_REQUIRE_EQUAL(cancel, instance.cancel());
    BOOST_REQUIRE(set_cancel == instance.set_cancel());
    BOOST_REQUIRE_EQUAL(min_version, instance.min_version());
    BOOST_REQUIRE_EQUAL(max_version, instance.max_version());
    BOOST_REQUIRE(set_sub_version == instance.set_sub_version());
    BOOST_REQUIRE_EQUAL(priority, instance.priority());
    BOOST_REQUIRE_EQUAL(comment, instance.comment());
    BOOST_REQUIRE_EQUAL(status_bar, instance.status_bar());
    BOOST_REQUIRE_EQUAL(reserved, instance.reserved());
}

BOOST_AUTO_TEST_CASE(alert_payload__operator_boolean_equals__duplicates__returns_true)
{
    const message::alert_payload expected(3452u, 64556u, 78545u, 43547u,
        546562345u, { 2345u, 346754u, 234u, 4356u }, 4644u, 89876u,
        { "foo", "bar", "baz" }, 34323u, "asfgsddsa", "fgjdfhjg", "utyurtevc");

    message::alert_payload instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(alert_payload__operator_boolean_equals__differs__returns_false)
{
    const message::alert_payload expected(3452u, 64556u, 78545u, 43547u,
        546562345u, { 2345u, 346754u, 234u, 4356u }, 4644u, 89876u,
        { "foo", "bar", "baz" }, 34323u, "asfgsddsa", "fgjdfhjg", "utyurtevc");

    message::alert_payload instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(alert_payload__operator_boolean_not_equals__duplicates__returns_false)
{
    const message::alert_payload expected(3452u, 64556u, 78545u, 43547u,
        546562345u, { 2345u, 346754u, 234u, 4356u }, 4644u, 89876u,
        { "foo", "bar", "baz" }, 34323u, "asfgsddsa", "fgjdfhjg", "utyurtevc");

    message::alert_payload instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(alert_payload__operator_boolean_not_equals__differs__returns_true)
{
    const message::alert_payload expected(3452u, 64556u, 78545u, 43547u,
        546562345u, { 2345u, 346754u, 234u, 4356u }, 4644u, 89876u,
        { "foo", "bar", "baz" }, 34323u, "asfgsddsa", "fgjdfhjg", "utyurtevc");

    message::alert_payload instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()
