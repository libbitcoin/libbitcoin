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
#include "../../test.hpp"
#include <algorithm>

BOOST_AUTO_TEST_SUITE(stealth_receiver_tests)

using namespace bc::system::wallet;

#define MAIN_KEY "tprv8ctN3HAF9dCgX9ggdCwiZHa7c3UHuG2Ev4jgYWDhTHDUVWKKsg7znbr3vYtmCzVqcMQsjd9cSKsyKGaDvTAUMkw1UphETe1j8LcT21eWPkH"
#define STEALTH_ADDRESS "vJmudwspxzmEoz1AP5tTrRMcuop6XjNWa1SnjHFmLeSc9DAkro6J6oYnD7MubLHx9wT3rm7D6xgA8U9Lr9zjzijhVSuUbYdMNYUN27"
#define EPHEMERAL_PRIVATE "f91e673103863bbeb0ef1852cd8eade6b73ea55afc9b1873be62bf628eac072a"
#define RECEIVER_PRIVATE "fc696c9f7143916f24977210c806101866c7fa13cc06982978d80518c91af2fb"
#define DERIVED_ADDRESS "mtKffkQLTw2D6f6mTkrWfi8qxLv4jL1LrK"

// TODO: test individual methods in isolation.
BOOST_AUTO_TEST_CASE(stealth_receiver__exchange_between_sender_and_receiver__always__round_trips)
{
    static const auto version = payment_address::testnet_p2kh;
    const hd_private main_key(MAIN_KEY, hd_private::testnet);
    const auto scan_key = main_key.derive_private(0 + hd_first_hardened_key);
    const auto spend_key = main_key.derive_private(1 + hd_first_hardened_key);
    const auto& scan_private = scan_key.secret();
    const auto& spend_private = spend_key.secret();

    const stealth_receiver receiver(scan_private, spend_private, binary{}, version);
    BOOST_REQUIRE(receiver);

    const auto& address = receiver.stealth_address();
    BOOST_REQUIRE_EQUAL(address.encoded(), STEALTH_ADDRESS);

    // Instead of generating a random ephemeral_private, use this one.
    ec_secret ephemeral_private;
    BOOST_REQUIRE(decode_base16(ephemeral_private, EPHEMERAL_PRIVATE));

    // Sender sends BTC to send_address and the preceding output is
    // ephemeral_public right-padded up to 80 bytes total (max standard op_return).
    const stealth_sender sender(ephemeral_private, address, data_chunk{}, binary{}, version);
    BOOST_REQUIRE(sender);

    const auto& payment = sender.payment_address();
    BOOST_REQUIRE_EQUAL(payment.encoded(), DERIVED_ADDRESS);

    // Receiver scans blockchain to get a list of potentially-matching values.
    // client.fetch_stealth() will yield rows of:
    // [ephemkey:32] [address:20] [tx_hash:32]
    // Normally this is obtained by the server via client.fetch_stealth.
    ec_compressed ephemeral_public;
    BOOST_REQUIRE(extract_ephemeral_key(ephemeral_public, sender.stealth_script()));

    // The receiver can regenerate send_address using just ephemeral_public.
    payment_address derived_address;
    BOOST_REQUIRE(receiver.derive_address(derived_address, ephemeral_public));
    BOOST_REQUIRE_EQUAL(derived_address, sender.payment_address());

    // Only reciever can derive stealth private, as it requires both scan and
    // spend private keys.
    ec_secret receiver_private;
    BOOST_REQUIRE(receiver.derive_private(receiver_private, ephemeral_public));

    ec_compressed receiver_public;
    BOOST_REQUIRE(secret_to_public(receiver_public, receiver_private));

    // The receiver now has the stealth private key and the send address.
    BOOST_REQUIRE_EQUAL(encode_base16(receiver_private), RECEIVER_PRIVATE);
    BOOST_REQUIRE_EQUAL(payment_address(receiver_public, version), derived_address);
}

BOOST_AUTO_TEST_SUITE_END()
