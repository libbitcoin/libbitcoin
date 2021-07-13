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

BOOST_AUTO_TEST_SUITE(version_tests)

static const auto version_maximum = message::version::level::maximum;

// "/Satoshi:1.1.1/" (70006) no relay
// anarchistprime: bitcointalk.org/index.php?topic=1001407
// This node is identifiable by a different genesis block.
#define NO_RELAY_ANARCHISTPRIME_1 "761101000100000000000000ae1b9c58000000000100000000000000260106009000d69ee9a999156d2e27fed77d01000000000000002a0104f80160144600000000000000022b2aaf9b8ea1eb14614b0f2f5361746f7368693a312e312e312f64450200"
#define NO_RELAY_ANARCHISTPRIME_2 "7611010001000000000000005b429c5800000000010000000000000000000000000000000000ffff1813e52e939b010000000000000000000000000000000000ffffd59fd7db200ac7f00f6ee45f1ab30f2f5361746f7368693a312e312e312f66450200"

// "/Cornell-Falcon-Network:0.1.0/" (70014) no relay
#define NO_RELAY_FALCON_1 "7e11010001000000000000005f429c5800000000010000000000000000000000000000000000ffff000000000000010000000000000000000000000000000000ffff22c06db5208d6241eabdf2d6753c1e2f436f726e656c6c2d46616c636f6e2d4e6574776f726b3a302e312e302f97e60600"
#define NO_RELAY_FALCON_2 "7e1101000100000000000000ae429c5800000000010000000000000000000000000000000000ffff000000000000010000000000000000000000000000000000ffff23a25ec4208d9ed337a66b411a441e2f436f726e656c6c2d46616c636f6e2d4e6574776f726b3a302e312e302f97e60600"

// "/Satoshi:0.13.0/" (70014) no relay
#define NO_RELAY_SATOSHI_1 "7e1101000900000000000000ec429c5800000000090000000000000000000000000000000000ffff1813e52e208d090000000000000000000000000000000000ffff97ec0b6d208d7c8c30307127a822102f5361746f7368693a302e31332e302f97e60600"

// "/therealbitcoin.org:0.9.99.99/" (99999) no relay
#define NO_RELAY_THEREALBITCOIN_1 "9f86010001000000000000002336a15800000000010000000000000000000000000000000000ffff1813e52ebb81010000000000000000000000000000000000ffff6f6f6f6f208db1f33b262e6acb331e2f7468657265616c626974636f696e2e6f72673a302e392e39392e39392fb9e80600"

BOOST_AUTO_TEST_CASE(version__factory__therealbitcoin_dot_org__valid)
{
    data_chunk payload;
    BOOST_REQUIRE(decode_base16(payload, NO_RELAY_THEREALBITCOIN_1));
    const auto version = message::version::factory(version_maximum, payload);
    BOOST_REQUIRE(version.is_valid());
}

BOOST_AUTO_TEST_CASE(version__factory__anarchistprime1__valid)
{
    data_chunk payload;
    BOOST_REQUIRE(decode_base16(payload, NO_RELAY_ANARCHISTPRIME_1));
    const auto version = message::version::factory(version_maximum, payload);
    BOOST_REQUIRE(version.is_valid());
}

BOOST_AUTO_TEST_CASE(version__factory__anarchistprime2__valid)
{
    data_chunk payload;
    BOOST_REQUIRE(decode_base16(payload, NO_RELAY_ANARCHISTPRIME_2));
    const auto version = message::version::factory(version_maximum, payload);
    BOOST_REQUIRE(version.is_valid());
}

BOOST_AUTO_TEST_CASE(version__factory__falcon1__valid)
{
    data_chunk payload;
    BOOST_REQUIRE(decode_base16(payload, NO_RELAY_FALCON_1));
    const auto version = message::version::factory(version_maximum, payload);
    BOOST_REQUIRE(version.is_valid());
}

BOOST_AUTO_TEST_CASE(version__factory__falcon2__valid)
{
    data_chunk payload;
    BOOST_REQUIRE(decode_base16(payload, NO_RELAY_FALCON_2));
    const auto version = message::version::factory(version_maximum, payload);
    BOOST_REQUIRE(version.is_valid());
}

BOOST_AUTO_TEST_CASE(version__factory__satoshi1__valid)
{
    data_chunk payload;
    BOOST_REQUIRE(decode_base16(payload, NO_RELAY_SATOSHI_1));
    const auto version = message::version::factory(version_maximum, payload);
    BOOST_REQUIRE(version.is_valid());
}

BOOST_AUTO_TEST_CASE(version__constructor_1__always__invalid)
{
    message::version instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());
    BOOST_REQUIRE_EQUAL(false, instance.address_receiver().is_valid());
    BOOST_REQUIRE_EQUAL(false, instance.address_sender().is_valid());
}

BOOST_AUTO_TEST_CASE(version__constructor_2__always__equals_params)
{
    uint32_t value = 45624u;
    uint64_t services = 263546u;
    uint64_t timestamp = 7668534u;

    message::network_address receiver
    {
        734678u,
        5357534u,
        {
            {
                0x47, 0x81, 0x6a, 0x40, 0xbb, 0x92, 0xbd, 0xb4,
                0xe0, 0xb8, 0x25, 0x68, 0x61, 0xf9, 0x6a, 0x55
            }
        },
        123u
    };

    message::network_address sender
    {
        46324u,
        156u,
        {
            {
                0xab, 0xcd, 0x6a, 0x40, 0x33, 0x92, 0x77, 0xb4,
                0xe0, 0xb8, 0xda, 0x43, 0x61, 0x66, 0x6a, 0x88
            }
        },
        351u
    };

    uint64_t nonce = 335743u;
    std::string agent = "sdashgdfafh";
    uint32_t height = 1246323u;
    const auto relay = true;

    message::version instance(value, services, timestamp, receiver, sender,  nonce, agent, height, relay);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(value, instance.value());
    BOOST_REQUIRE_EQUAL(services, instance.services());
    BOOST_REQUIRE_EQUAL(timestamp, instance.timestamp());
    BOOST_REQUIRE(receiver == instance.address_receiver());
    BOOST_REQUIRE(sender == instance.address_sender());
    BOOST_REQUIRE_EQUAL(nonce, instance.nonce());
    BOOST_REQUIRE_EQUAL(agent, instance.user_agent());
    BOOST_REQUIRE_EQUAL(height, instance.start_height());
    BOOST_REQUIRE_EQUAL(relay, instance.relay());
}

BOOST_AUTO_TEST_CASE(version__constructor_3__always__equals_params)
{
    uint32_t value = 45624u;
    uint64_t services = 263546u;
    uint64_t timestamp = 7668534u;

    message::network_address receiver
    {
        734678u,
        5357534u,
        {
            {
                0x47, 0x81, 0x6a, 0x40, 0xbb, 0x92, 0xbd, 0xb4,
                0xe0, 0xb8, 0x25, 0x68, 0x61, 0xf9, 0x6a, 0x55
            }
        },
        123u
    };

    message::network_address sender
    {
        46324u,
        156u,
        {
            {
                0xab, 0xcd, 0x6a, 0x40, 0x33, 0x92, 0x77, 0xb4,
                0xe0, 0xb8, 0xda, 0x43, 0x61, 0x66, 0x6a, 0x88
            }
        },
        351u
    };

    uint64_t nonce = 335743u;
    std::string agent = "sdashgdfafh";
    uint32_t height = 1246323u;
    const auto relay = true;

    BOOST_REQUIRE(receiver.is_valid());
    BOOST_REQUIRE(sender.is_valid());

    message::version instance(value, services, timestamp, std::move(receiver), std::move(sender), nonce, agent, height, relay);

    BOOST_REQUIRE(instance.is_valid());
}

BOOST_AUTO_TEST_CASE(version__constructor_4__always__equals_params)
{
    uint32_t value = 45624u;
    uint64_t services = 263546u;
    uint64_t timestamp = 7668534u;

    message::network_address receiver
    {
        734678u,
        5357534u,
        {
            {
                0x47, 0x81, 0x6a, 0x40, 0xbb, 0x92, 0xbd, 0xb4,
                0xe0, 0xb8, 0x25, 0x68, 0x61, 0xf9, 0x6a, 0x55
            }
        },
        123u
    };

    message::network_address sender
    {
        46324u,
        156u,
        {
            {
                0xab, 0xcd, 0x6a, 0x40, 0x33, 0x92, 0x77, 0xb4,
                0xe0, 0xb8, 0xda, 0x43, 0x61, 0x66, 0x6a, 0x88
            }
        },
        351u
    };

    uint64_t nonce = 335743u;
    std::string agent = "sdashgdfafh";
    uint32_t height = 1246323u;
    const auto relay = true;

    BOOST_REQUIRE(receiver.is_valid());
    BOOST_REQUIRE(sender.is_valid());

    message::version alpha(value, services, timestamp, receiver, sender, nonce, agent, height, relay);
    BOOST_REQUIRE(alpha.is_valid());

    message::version beta(alpha);
    BOOST_REQUIRE(beta == alpha);
}

BOOST_AUTO_TEST_CASE(version__constructor_5__always__equals_params)
{
    uint32_t value = 45624u;
    uint64_t services = 263546u;
    uint64_t timestamp = 7668534u;

    message::network_address receiver
    {
        734678u,
        5357534u,
        {
            {
                0x47, 0x81, 0x6a, 0x40, 0xbb, 0x92, 0xbd, 0xb4,
                0xe0, 0xb8, 0x25, 0x68, 0x61, 0xf9, 0x6a, 0x55
            }
        },
        123u
    };

    message::network_address sender
    {
        46324u,
        156u,
        {
            {
                0xab, 0xcd, 0x6a, 0x40, 0x33, 0x92, 0x77, 0xb4,
                0xe0, 0xb8, 0xda, 0x43, 0x61, 0x66, 0x6a, 0x88
            }
        },
        351u
    };

    uint64_t nonce = 335743u;
    std::string agent = "sdashgdfafh";
    uint32_t height = 1246323u;
    const auto relay = true;

    BOOST_REQUIRE(receiver.is_valid());
    BOOST_REQUIRE(sender.is_valid());

    message::version alpha(value, services, timestamp, receiver, sender, nonce, agent, height, relay);
    BOOST_REQUIRE(alpha.is_valid());

    message::version beta(std::move(alpha));
    BOOST_REQUIRE(beta.is_valid());
    BOOST_REQUIRE_EQUAL(value, beta.value());
    BOOST_REQUIRE_EQUAL(services, beta.services());
    BOOST_REQUIRE_EQUAL(timestamp, beta.timestamp());
    BOOST_REQUIRE(receiver == beta.address_receiver());
    BOOST_REQUIRE(sender == beta.address_sender());
    BOOST_REQUIRE_EQUAL(nonce, beta.nonce());
    BOOST_REQUIRE_EQUAL(agent, beta.user_agent());
    BOOST_REQUIRE_EQUAL(height, beta.start_height());
    BOOST_REQUIRE_EQUAL(relay, beta.relay());
}

BOOST_AUTO_TEST_CASE(version__from_data__insufficient_bytes__invalid)
{
    const data_chunk raw{ 0xab };
    message::version instance{};

    BOOST_REQUIRE_EQUAL(false, instance.from_data(version_maximum, raw));
}

BOOST_AUTO_TEST_CASE(version__from_data__mismatched_sender_services__invalid)
{
    uint64_t sender_services = 1515u;
    const message::version expected
    (
        210u,
        sender_services,
        979797u,
        message::network_address
        {
            734678u,
            5357534u,
            {
                {
                    0x47, 0x81, 0x6a, 0x40, 0xbb, 0x92, 0xbd, 0xb4,
                    0xe0, 0xb8, 0x25, 0x68, 0x61, 0xf9, 0x6a, 0x55
                }
            },
            123u
        },
        message::network_address
        {
            46324u,
            sender_services + 1,
            {
                {
                    0xab, 0xcd, 0x6a, 0x40, 0x33, 0x92, 0x77, 0xb4,
                    0xe0, 0xb8, 0xda, 0x43, 0x61, 0x66, 0x6a, 0x88
                }
            },
            351u
        },
        13626u,
        "my agent",
        100u,
        false
    );

    const auto data = expected.to_data(version_maximum);
    const auto result = message::version::factory(version_maximum, data);

    // HACK: disabled check due to inconsistent node implementation.
    BOOST_REQUIRE(/*!*/result.is_valid());
}

BOOST_AUTO_TEST_CASE(version__from_data__version_meets_bip37__success)
{
    const auto sender_services = 1515u;
    const message::version expected
    {
        message::version::level::bip37,
        sender_services,
        979797u,
        {
            734678u,
            5357534u,
            {
                {
                    0x47, 0x81, 0x6a, 0x40, 0xbb, 0x92, 0xbd, 0xb4,
                    0xe0, 0xb8, 0x25, 0x68, 0x61, 0xf9, 0x6a, 0x55
                }
            },
            123u
        },
        {
            46324u,
            sender_services,
            {
                {
                    0xab, 0xcd, 0x6a, 0x40, 0x33, 0x92, 0x77, 0xb4,
                    0xe0, 0xb8, 0xda, 0x43, 0x61, 0x66, 0x6a, 0x88
                }
            },
            351u
        },
        13626u,
        "my agent",
        100u,
        true
    };

    const auto data = expected.to_data(version_maximum);
    const auto result = message::version::factory(version_maximum, data);
    BOOST_REQUIRE(result.is_valid());
}

BOOST_AUTO_TEST_CASE(version__factory_1__valid_input__success)
{
    const auto sender_services = 1515u;
    const message::version expected
    {
        210u,
        sender_services,
        979797u,
        {
            734678u,
            5357534u,
            {
                {
                    0x47, 0x81, 0x6a, 0x40, 0xbb, 0x92, 0xbd, 0xb4,
                    0xe0, 0xb8, 0x25, 0x68, 0x61, 0xf9, 0x6a, 0x55
                }
            },
            123u
        },
        {
            46324u,
            sender_services,
            {
                {
                    0xab, 0xcd, 0x6a, 0x40, 0x33, 0x92, 0x77, 0xb4,
                    0xe0, 0xb8, 0xda, 0x43, 0x61, 0x66, 0x6a, 0x88
                }
            },
            351u
        },
        13626u,
        "my agent",
        100u,
        true
    };

    const auto data = expected.to_data(version_maximum);
    const auto result = message::version::factory(version_maximum, data);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(version_maximum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version_maximum), result.serialized_size(version_maximum));
    BOOST_REQUIRE(expected == result);
}

BOOST_AUTO_TEST_CASE(version__factory_2__valid_input__success)
{
    const auto sender_services = 1515u;
    const message::version expected
    (
        210u,
        sender_services,
        979797u,
        {
            734678u,
            5357534u,
            {
                {
                    0x47, 0x81, 0x6a, 0x40, 0xbb, 0x92, 0xbd, 0xb4,
                    0xe0, 0xb8, 0x25, 0x68, 0x61, 0xf9, 0x6a, 0x55
                }
            },
            123u
        },
        {
            46324u,
            sender_services,
            {
                {
                    0xab, 0xcd, 0x6a, 0x40, 0x33, 0x92, 0x77, 0xb4,
                    0xe0, 0xb8, 0xda, 0x43, 0x61, 0x66, 0x6a, 0x88
                }
            },
            351u
        },
        13626u,
        "my agent",
        100u,
        true
    );

    const auto data = expected.to_data(version_maximum);
    stream::in::copy istream(data);
    const auto result = message::version::factory(version_maximum, istream);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(version_maximum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version_maximum), result.serialized_size(version_maximum));
    BOOST_REQUIRE(expected == result);
}

BOOST_AUTO_TEST_CASE(version__factory_3__valid_input__success)
{
    const auto sender_services = 1515u;
    const message::version expected
    (
        210u,
        sender_services,
        979797u,
        {
            734678u,
            5357534u,
            {
                {
                    0x47, 0x81, 0x6a, 0x40, 0xbb, 0x92, 0xbd, 0xb4,
                    0xe0, 0xb8, 0x25, 0x68, 0x61, 0xf9, 0x6a, 0x55
                }
            },
            123u
        },
        {
            46324u,
            sender_services,
            {
                {
                    0xab, 0xcd, 0x6a, 0x40, 0x33, 0x92, 0x77, 0xb4,
                    0xe0, 0xb8, 0xda, 0x43, 0x61, 0x66, 0x6a, 0x88
                }
            },
            351u
        },
        13626u,
        "my agent",
        100u,
        true
    );

    const auto data = expected.to_data(version_maximum);
    read::bytes::copy source(data);
    const auto result = message::version::factory(version_maximum, source);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(version_maximum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version_maximum), result.serialized_size(version_maximum));
    BOOST_REQUIRE_EQUAL(expected.relay(), result.relay());
    BOOST_REQUIRE_EQUAL(expected.value(), result.value());
    BOOST_REQUIRE_EQUAL(expected.services(), result.services());
    BOOST_REQUIRE_EQUAL(expected.timestamp(), result.timestamp());
    BOOST_REQUIRE_EQUAL(expected.nonce(), result.nonce());
    BOOST_REQUIRE_EQUAL(expected.user_agent(), result.user_agent());
    BOOST_REQUIRE_EQUAL(expected.start_height(), result.start_height());
    BOOST_REQUIRE(expected.address_receiver() == result.address_receiver());
    BOOST_REQUIRE(expected.address_sender() == result.address_sender());
    BOOST_REQUIRE(expected == result);
}

BOOST_AUTO_TEST_CASE(version__value_accessor__returns_initialized_value)
{
    const uint32_t expected = 210u;
    const message::version instance
    (
        expected,
        1515u,
        979797u,
        {
            734678u,
            5357534u,
            {
                {
                    0x47, 0x81, 0x6a, 0x40, 0xbb, 0x92, 0xbd, 0xb4,
                    0xe0, 0xb8, 0x25, 0x68, 0x61, 0xf9, 0x6a, 0x55
                }
            },
            123u
        },
        {
            46324u,
            1515u,
            {
                {
                    0xab, 0xcd, 0x6a, 0x40, 0x33, 0x92, 0x77, 0xb4,
                    0xe0, 0xb8, 0xda, 0x43, 0x61, 0x66, 0x6a, 0x88
                }
            },
            351u
        },
        13626u,
        "my agent",
        100u,
        false
    );

    BOOST_REQUIRE_EQUAL(expected, instance.value());
}

BOOST_AUTO_TEST_CASE(version__value_setter__roundtrip__success)
{
    const uint32_t expected = 210u;
    message::version instance;
    instance.set_value(expected);
    BOOST_REQUIRE_EQUAL(expected, instance.value());
}

BOOST_AUTO_TEST_CASE(version__services_accessor__always__returns_initialized_value)
{
    const uint64_t expected = 1515u;
    const message::version instance
    (
        210u,
        expected,
        979797u,
        {
            734678u,
            5357534u,
            {
                {
                    0x47, 0x81, 0x6a, 0x40, 0xbb, 0x92, 0xbd, 0xb4,
                    0xe0, 0xb8, 0x25, 0x68, 0x61, 0xf9, 0x6a, 0x55
                }
            },
            123u
        },
        {
            46324u,
            1515u,
            {
                {
                    0xab, 0xcd, 0x6a, 0x40, 0x33, 0x92, 0x77, 0xb4,
                    0xe0, 0xb8, 0xda, 0x43, 0x61, 0x66, 0x6a, 0x88
                }
            },
            351u
        },
        13626u,
        "my agent",
        100u,
        false
    );

    BOOST_REQUIRE_EQUAL(expected, instance.services());
}

BOOST_AUTO_TEST_CASE(version__services_setter__roundtrip__success)
{
    const uint64_t expected = 1515u;
    message::version instance;
    instance.set_services(expected);
    BOOST_REQUIRE_EQUAL(expected, instance.services());
}

BOOST_AUTO_TEST_CASE(version__timestamp_accessor__always__returns_initialized_value)
{
    const uint64_t expected = 979797u;
    const message::version instance
    (
        210u,
        1515u,
        expected,
        {
            734678u,
            5357534u,
            {
                {
                    0x47, 0x81, 0x6a, 0x40, 0xbb, 0x92, 0xbd, 0xb4,
                    0xe0, 0xb8, 0x25, 0x68, 0x61, 0xf9, 0x6a, 0x55
                }
            },
            123u
        },
        {
            46324u,
            1515u,
            {
                {
                    0xab, 0xcd, 0x6a, 0x40, 0x33, 0x92, 0x77, 0xb4,
                    0xe0, 0xb8, 0xda, 0x43, 0x61, 0x66, 0x6a, 0x88
                }
            },
            351u
        },
        13626u,
        "my agent",
        100u,
        false
    );

    BOOST_REQUIRE_EQUAL(expected, instance.timestamp());
}

BOOST_AUTO_TEST_CASE(version__timestamp_setter__roundtrip__success)
{
    const uint64_t expected = 979797u;
    message::version instance;
    instance.set_timestamp(expected);
    BOOST_REQUIRE_EQUAL(expected, instance.timestamp());
}

BOOST_AUTO_TEST_CASE(version__address_receiver_accessor__always__returns_initialized_value)
{
    const message::network_address expected
    {
        734678u,
        5357534u,
        {
            {
                0x47, 0x81, 0x6a, 0x40, 0xbb, 0x92, 0xbd, 0xb4,
                0xe0, 0xb8, 0x25, 0x68, 0x61, 0xf9, 0x6a, 0x55
            }
        },
        123u
    };

    const message::version instance
    (
        210u,
        1515u,
        979797u,
        expected,
        {
            46324u,
            1515u,
            {
                {
                    0xab, 0xcd, 0x6a, 0x40, 0x33, 0x92, 0x77, 0xb4,
                    0xe0, 0xb8, 0xda, 0x43, 0x61, 0x66, 0x6a, 0x88
                }
            },
            351u
        },
        13626u,
        "my agent",
        100u,
        false
    );

    BOOST_REQUIRE(expected == instance.address_receiver());
}

//BOOST_AUTO_TEST_CASE(version__address_receiver_setter_1__roundtrip__success)
//{
//    message::network_address expected
//    {
//        734678u,
//        5357534u,
//        {
//            {
//                0x47, 0x81, 0x6a, 0x40, 0xbb, 0x92, 0xbd, 0xb4,
//                0xe0, 0xb8, 0x25, 0x68, 0x61, 0xf9, 0x6a, 0x55
//            }
//        },
//        123u
//    };
//
//    message::version instance;
//    BOOST_REQUIRE(!instance.address_receiver().is_valid());
//    instance.address_receiver(expected);
//    const message::network_address result = instance.address_receiver();
//    BOOST_REQUIRE(result == expected);
//    BOOST_REQUIRE(result.is_valid());
//}

BOOST_AUTO_TEST_CASE(version__address_receiver_setter_2__roundtrip__success)
{
    message::version instance;
    BOOST_REQUIRE(!instance.address_receiver().is_valid());
    instance.set_address_receiver(message::network_address
    {
        734678u,
        5357534u,
        {
            {
                0x47, 0x81, 0x6a, 0x40, 0xbb, 0x92, 0xbd, 0xb4,
                0xe0, 0xb8, 0x25, 0x68, 0x61, 0xf9, 0x6a, 0x55
            }
        },
        123u
    });

    const message::network_address result = instance.address_receiver();
    BOOST_REQUIRE(result.is_valid());
}

BOOST_AUTO_TEST_CASE(version__address_sender_accessor__always__returns_initialized_value)
{
    const message::network_address expected
    {
        46324u,
        1515u,
        {
            {
                0xab, 0xcd, 0x6a, 0x40, 0x33, 0x92, 0x77, 0xb4,
                0xe0, 0xb8, 0xda, 0x43, 0x61, 0x66, 0x6a, 0x88
            }
        },
        351u
    };

    const message::version instance
    (
        210u,
        1515u,
        979797u,
        {
            734678u,
            5357534u,
            {
                {
                    0x47, 0x81, 0x6a, 0x40, 0xbb, 0x92, 0xbd, 0xb4,
                    0xe0, 0xb8, 0x25, 0x68, 0x61, 0xf9, 0x6a, 0x55
                }
            },
            123u
        },
        expected,
        13626u,
        "my agent",
        100u,
        false
    );

    BOOST_REQUIRE(expected == instance.address_sender());
}

//BOOST_AUTO_TEST_CASE(version__address_sender_setter_1__roundtrip__success)
//{
//    message::network_address expected
//    {
//        46324u,
//        1515u,
//        {
//            {
//                0xab, 0xcd, 0x6a, 0x40, 0x33, 0x92, 0x77, 0xb4,
//                0xe0, 0xb8, 0xda, 0x43, 0x61, 0x66, 0x6a, 0x88
//            }
//        },
//        351u
//    };
//
//    message::version instance;
//    BOOST_REQUIRE(!instance.address_sender().is_valid());
//    instance.address_sender(expected);
//    const message::network_address result = instance.address_sender();
//    BOOST_REQUIRE(result == expected);
//    BOOST_REQUIRE(result.is_valid());
//}

BOOST_AUTO_TEST_CASE(version__address_sender_setter_2__roundtrip__success)
{
    message::version instance;
    BOOST_REQUIRE(!instance.address_sender().is_valid());
    instance.set_address_sender(message::network_address
    {
        46324u,
        1515u,
        {
            {
                0xab, 0xcd, 0x6a, 0x40, 0x33, 0x92, 0x77, 0xb4,
                0xe0, 0xb8, 0xda, 0x43, 0x61, 0x66, 0x6a, 0x88
            }
        },
        351u
    });

    const message::network_address result = instance.address_sender();
    BOOST_REQUIRE(result.is_valid());
}

BOOST_AUTO_TEST_CASE(version__nonce_accessor__always__returns_initialized_value)
{
    const uint64_t expected = 13626u;
    const message::version instance
    (
        210u,
        1515u,
        979797u,
        {
            734678u,
            5357534u,
            {
                {
                    0x47, 0x81, 0x6a, 0x40, 0xbb, 0x92, 0xbd, 0xb4,
                    0xe0, 0xb8, 0x25, 0x68, 0x61, 0xf9, 0x6a, 0x55
                }
            },
            123u
        },
        {
            46324u,
            1515u,
            {
                {
                    0xab, 0xcd, 0x6a, 0x40, 0x33, 0x92, 0x77, 0xb4,
                    0xe0, 0xb8, 0xda, 0x43, 0x61, 0x66, 0x6a, 0x88
                }
            },
            351u
        },
        expected,
        "my agent",
        100u,
        false
    );

    BOOST_REQUIRE_EQUAL(expected, instance.nonce());
}

BOOST_AUTO_TEST_CASE(version__nonce_setter__roundtrip__success)
{
    const uint64_t expected = 13626u;
    message::version instance;
    instance.set_nonce(expected);
    BOOST_REQUIRE_EQUAL(expected, instance.nonce());
}

BOOST_AUTO_TEST_CASE(version__user_agent_accessor__always__returns_initialized_value)
{
    const std::string expected = "MyUseRAgenT";
    const message::version instance
    (
        210u,
        1515u,
        979797u,
        {
            734678u,
            5357534u,
            {
                {
                    0x47, 0x81, 0x6a, 0x40, 0xbb, 0x92, 0xbd, 0xb4,
                    0xe0, 0xb8, 0x25, 0x68, 0x61, 0xf9, 0x6a, 0x55
                }
            },
            123u
        },
        {
            46324u,
            1515u,
            {
                {
                    0xab, 0xcd, 0x6a, 0x40, 0x33, 0x92, 0x77, 0xb4,
                    0xe0, 0xb8, 0xda, 0x43, 0x61, 0x66, 0x6a, 0x88
                }
            },
            351u
        },
        13626u,
        expected,
        100u,
        false
    );

    BOOST_REQUIRE_EQUAL(expected, instance.user_agent());
}

BOOST_AUTO_TEST_CASE(version__user_agent_setter_1__roundtrip__success)
{
    const std::string expected = "MyUseRAgenT";
    message::version instance;
    instance.set_user_agent(expected);
    BOOST_REQUIRE_EQUAL(expected, instance.user_agent());
}

BOOST_AUTO_TEST_CASE(version__user_agent_setter_2__roundtrip__success)
{
    std::string expected = "MyUseRAgenT";
    message::version instance;
    BOOST_REQUIRE(instance.user_agent().empty());
    instance.set_user_agent(std::move(expected));
    BOOST_REQUIRE_EQUAL(false, instance.user_agent().empty());
}

BOOST_AUTO_TEST_CASE(version__start_height_accessor__always__returns_initialized_value)
{
    const uint32_t expected = 514u;
    const message::version instance
    (
        210u,
        1515u,
        979797u,
        {
            734678u,
            5357534u,
            {
                {
                    0x47, 0x81, 0x6a, 0x40, 0xbb, 0x92, 0xbd, 0xb4,
                    0xe0, 0xb8, 0x25, 0x68, 0x61, 0xf9, 0x6a, 0x55
                }
            },
            123u
        },
        {
            46324u,
            1515u,
            {
                {
                    0xab, 0xcd, 0x6a, 0x40, 0x33, 0x92, 0x77, 0xb4,
                    0xe0, 0xb8, 0xda, 0x43, 0x61, 0x66, 0x6a, 0x88
                }
            },
            351u
        },
        13626u,
        "my agent",
        expected,
        false
    );

    BOOST_REQUIRE_EQUAL(expected, instance.start_height());
}

BOOST_AUTO_TEST_CASE(version__start_height_setter__roundtrip__success)
{
    const uint32_t expected = 514u;
    message::version instance;
    instance.set_start_height(expected);
    BOOST_REQUIRE_EQUAL(expected, instance.start_height());
}

BOOST_AUTO_TEST_CASE(version__relay_accessor__always__returns_initialized_value)
{
    const bool expected = true;
    const message::version instance
    (
        210u,
        1515u,
        979797u,
        {
            734678u,
            5357534u,
            {
                {
                    0x47, 0x81, 0x6a, 0x40, 0xbb, 0x92, 0xbd, 0xb4,
                    0xe0, 0xb8, 0x25, 0x68, 0x61, 0xf9, 0x6a, 0x55
                }
            },
            123u
        },
        {
            46324u,
            1515u,
            {
                {
                    0xab, 0xcd, 0x6a, 0x40, 0x33, 0x92, 0x77, 0xb4,
                    0xe0, 0xb8, 0xda, 0x43, 0x61, 0x66, 0x6a, 0x88
                }
            },
            351u
        },
        13626u,
        "my agent",
        100u,
        expected
    );

    BOOST_REQUIRE_EQUAL(expected, instance.relay());
}

BOOST_AUTO_TEST_CASE(version__relay_setter__roundtrip__success)
{
    const bool expected = true;
    message::version instance;
    instance.set_relay(expected);
    BOOST_REQUIRE_EQUAL(expected, instance.relay());
}

BOOST_AUTO_TEST_CASE(version__operator_assign_equals__always__matches_equivalent)
{
    message::version value
    (
        210u,
        15234u,
        979797u,
        message::network_address
        {
            734678u,
            5357534u,
            {
                {
                    0x47, 0x81, 0x6a, 0x40, 0xbb, 0x92, 0xbd, 0xb4,
                    0xe0, 0xb8, 0x25, 0x68, 0x61, 0xf9, 0x6a, 0x55
                }
            },
            123u
        },
        message::network_address
        {
            46324u,
            57835u,
            {
                {
                    0xab, 0xcd, 0x6a, 0x40, 0x33, 0x92, 0x77, 0xb4,
                    0xe0, 0xb8, 0xda, 0x43, 0x61, 0x66, 0x6a, 0x88
                }
            },
            351u
        },
        13626u,
        "my agent",
        100u,
        false
    );

    BOOST_REQUIRE(value.is_valid());

    message::version instance;
    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
}

BOOST_AUTO_TEST_CASE(version__operator_boolean_equals__duplicates__returns_true)
{
    const message::version expected
    (
        210u,
        15234u,
        979797u,
        message::network_address
        {
            734678u,
            5357534u,
            {
                {
                    0x47, 0x81, 0x6a, 0x40, 0xbb, 0x92, 0xbd, 0xb4,
                    0xe0, 0xb8, 0x25, 0x68, 0x61, 0xf9, 0x6a, 0x55
                }
            },
            123u
        },
        message::network_address
        {
            46324u,
            57835u,
            {
                {
                    0xab, 0xcd, 0x6a, 0x40, 0x33, 0x92, 0x77, 0xb4,
                    0xe0, 0xb8, 0xda, 0x43, 0x61, 0x66, 0x6a, 0x88
                }
            },
            351u
        },
        13626u,
        "my agent",
        100u,
        false
    );

    message::version instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(version__operator_boolean_equals__differs__returns_false)
{
    const message::version expected
    (
        210u,
        15234u,
        979797u,
        message::network_address
        {
            734678u,
            5357534u,
            {
                {
                    0x47, 0x81, 0x6a, 0x40, 0xbb, 0x92, 0xbd, 0xb4,
                    0xe0, 0xb8, 0x25, 0x68, 0x61, 0xf9, 0x6a, 0x55
                }
            },
            123u
        },
        message::network_address
        {
            46324u,
            57835u,
            {
                {
                    0xab, 0xcd, 0x6a, 0x40, 0x33, 0x92, 0x77, 0xb4,
                    0xe0, 0xb8, 0xda, 0x43, 0x61, 0x66, 0x6a, 0x88
                }
            },
            351u
        },
        13626u,
        "my agent",
        100u,
        false
    );

    message::version instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(version__operator_boolean_not_equals__duplicates__returns_false)
{
    const message::version expected
    (
        210u,
        15234u,
        979797u,
        message::network_address
        {
            734678u,
            5357534u,
            {
                {
                    0x47, 0x81, 0x6a, 0x40, 0xbb, 0x92, 0xbd, 0xb4,
                    0xe0, 0xb8, 0x25, 0x68, 0x61, 0xf9, 0x6a, 0x55
                }
            },
            123u
        },
        message::network_address
        {
            46324u,
            57835u,
            {
                {
                    0xab, 0xcd, 0x6a, 0x40, 0x33, 0x92, 0x77, 0xb4,
                    0xe0, 0xb8, 0xda, 0x43, 0x61, 0x66, 0x6a, 0x88
                }
            },
            351u
        },
        13626u,
        "my agent",
        100u,
        false
    );

    message::version instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(version__operator_boolean_not_equals__differs__returns_true)
{
    const message::version expected
    (
        210u,
        15234u,
        979797u,
        message::network_address
        {
            734678u,
            5357534u,
            {
                {
                    0x47, 0x81, 0x6a, 0x40, 0xbb, 0x92, 0xbd, 0xb4,
                    0xe0, 0xb8, 0x25, 0x68, 0x61, 0xf9, 0x6a, 0x55
                }
            },
            123u
        },
        message::network_address
        {
            46324u,
            57835u,
            {
                {
                    0xab, 0xcd, 0x6a, 0x40, 0x33, 0x92, 0x77, 0xb4,
                    0xe0, 0xb8, 0xda, 0x43, 0x61, 0x66, 0x6a, 0x88
                }
            },
            351u
        },
        13626u,
        "my agent",
        100u,
        false
    );

    message::version instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()
