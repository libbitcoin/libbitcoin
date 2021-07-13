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

// TODO: move this into the tests by source file organization.

BOOST_AUTO_TEST_SUITE(satoshi_words)

BOOST_AUTO_TEST_CASE(satoshi_words_mainnet)
{
    // Create mainnet genesis block (contains a single coinbase transaction).
    const chain::block block = settings(chain::selection::mainnet).genesis_block;
    const auto& transactions = block.transactions();
    BOOST_REQUIRE_EQUAL(transactions.size(), 1u);

    // Coinbase tx (first in block) has a single input.
    const auto& coinbase_tx = transactions[0];
    const auto& coinbase_inputs = coinbase_tx.inputs();
    BOOST_REQUIRE_EQUAL(coinbase_inputs.size(), 1u);

    // Convert the input script to its raw format.
    const auto& coinbase_input = coinbase_inputs[0];
    const auto raw_message = coinbase_input.script().to_data(false);
    BOOST_REQUIRE_GT(raw_message.size(), 8u);

    // Convert to a string after removing the 8 byte checksum.
    std::string message;
    message.resize(raw_message.size() - 8);
    std::copy(raw_message.begin() + 8, raw_message.end(), message.begin());

    BOOST_REQUIRE_EQUAL(message, "The Times 03/Jan/2009 Chancellor on brink of second bailout for banks");
}

BOOST_AUTO_TEST_SUITE_END()
