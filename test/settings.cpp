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
#include "test.hpp"

BOOST_AUTO_TEST_SUITE(settings_tests)

// constructors
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(settings__construct__default_context__expected)
{
    settings configuration;
    BOOST_REQUIRE_EQUAL(configuration.block_spacing_seconds, 600);
    BOOST_REQUIRE_EQUAL(configuration.timestamp_limit_seconds, 7200);
    BOOST_REQUIRE_EQUAL(configuration.retargeting_interval_seconds, 1209600);
    BOOST_REQUIRE_EQUAL(configuration.minimum_timespan(), 302400);
    BOOST_REQUIRE_EQUAL(configuration.maximum_timespan(), 4838400);
    BOOST_REQUIRE_EQUAL(configuration.retargeting_interval(), 2016);
    BOOST_REQUIRE_EQUAL(configuration.first_version, 1);
    BOOST_REQUIRE_EQUAL(configuration.bip34_version, 2);
    BOOST_REQUIRE_EQUAL(configuration.bip66_version, 3);
    BOOST_REQUIRE_EQUAL(configuration.bip65_version, 4);
    BOOST_REQUIRE_EQUAL(configuration.bip9_version_bit0, 1);
    BOOST_REQUIRE_EQUAL(configuration.bip9_version_bit1, 2);
    BOOST_REQUIRE_EQUAL(configuration.bip9_version_base, 0x20000000);
    BOOST_REQUIRE_EQUAL(configuration.initial_subsidy_bitcoin, 50);
    BOOST_REQUIRE_EQUAL(configuration.bitcoin_to_satoshi(1), 100000000);
}

BOOST_AUTO_TEST_CASE(settings__construct__mainnet_context__expected)
{
    settings configuration(chain::selection::mainnet);
    BOOST_REQUIRE_EQUAL(configuration.block_spacing_seconds, 600);
    BOOST_REQUIRE_EQUAL(configuration.timestamp_limit_seconds, 7200);
    BOOST_REQUIRE_EQUAL(configuration.retargeting_interval_seconds, 1209600);
    BOOST_REQUIRE_EQUAL(configuration.proof_of_work_limit, 486604799);
    BOOST_REQUIRE_EQUAL(configuration.minimum_timespan(), 302400);
    BOOST_REQUIRE_EQUAL(configuration.maximum_timespan(), 4838400);
    BOOST_REQUIRE_EQUAL(configuration.retargeting_interval(), 2016);
    BOOST_REQUIRE_EQUAL(configuration.work_limit(), uint256_t(chain::compact(486604799)));
    const chain::block genesis_block = configuration.genesis_block;
    BOOST_REQUIRE_EQUAL(encode_base16(genesis_block.to_data()), "0100000000000000000000000000000000000000000000000000000000000000000000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a29ab5f49ffff001d1dac2b7c0101000000010000000000000000000000000000000000000000000000000000000000000000ffffffff4d04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73ffffffff0100f2052a01000000434104678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5fac00000000");
    BOOST_REQUIRE_EQUAL(configuration.first_version, 1);
    BOOST_REQUIRE_EQUAL(configuration.bip34_version, 2);
    BOOST_REQUIRE_EQUAL(configuration.bip66_version, 3);
    BOOST_REQUIRE_EQUAL(configuration.bip65_version, 4);
    BOOST_REQUIRE_EQUAL(configuration.bip9_version_bit0, 1);
    BOOST_REQUIRE_EQUAL(configuration.bip9_version_bit1, 2);
    BOOST_REQUIRE_EQUAL(configuration.bip9_version_base, 0x20000000);
    BOOST_REQUIRE_EQUAL(configuration.activation_threshold, 750);
    BOOST_REQUIRE_EQUAL(configuration.enforcement_threshold, 950);
    BOOST_REQUIRE_EQUAL(configuration.activation_sample, 1000);
    BOOST_REQUIRE_EQUAL(configuration.bip65_freeze, 388381);
    BOOST_REQUIRE_EQUAL(configuration.bip66_freeze, 363725);
    BOOST_REQUIRE_EQUAL(configuration.bip34_freeze, 227931);
    BOOST_REQUIRE_EQUAL(configuration.bip16_activation_time, 1333238400);
    const chain::check_point bip34_active("000000000000024b89b42a942fe0d9fea3bb44ab7bd1b19115dd6a759c0808b8", configuration.bip34_freeze);
    BOOST_REQUIRE_EQUAL(configuration.bip34_active_checkpoint, bip34_active);
    const chain::check_point bit0_active("000000000000000004a1b34462cb8aeebd5799177f7a29cf28f2d1961716b5b5", 419328);
    BOOST_REQUIRE_EQUAL(configuration.bip9_bit0_active_checkpoint, bit0_active);
    const chain::check_point bit1_active("0000000000000000001c8018d9cb3b742ef25114f27563e3fc4a1902167f9893", 481824);
    BOOST_REQUIRE_EQUAL(configuration.bip9_bit1_active_checkpoint, bit1_active);
    BOOST_REQUIRE_EQUAL(configuration.initial_subsidy_bitcoin, 50);
    BOOST_REQUIRE_EQUAL(configuration.subsidy_interval_blocks, 210000);
    BOOST_REQUIRE_EQUAL(configuration.bitcoin_to_satoshi(1), 100000000);
    BOOST_REQUIRE_EQUAL(configuration.max_money(), 2099999997690000);
}

BOOST_AUTO_TEST_CASE(settings__construct__testnet_context__expected)
{
    settings configuration(chain::selection::testnet);
    BOOST_REQUIRE_EQUAL(configuration.block_spacing_seconds, 600);
    BOOST_REQUIRE_EQUAL(configuration.timestamp_limit_seconds, 7200);
    BOOST_REQUIRE_EQUAL(configuration.retargeting_interval_seconds, 1209600);
    BOOST_REQUIRE_EQUAL(configuration.proof_of_work_limit, 486604799);
    BOOST_REQUIRE_EQUAL(configuration.minimum_timespan(), 302400);
    BOOST_REQUIRE_EQUAL(configuration.maximum_timespan(), 4838400);
    BOOST_REQUIRE_EQUAL(configuration.retargeting_interval(), 2016);
    BOOST_REQUIRE_EQUAL(configuration.work_limit(), uint256_t(chain::compact(486604799)));
    const chain::block genesis_block = configuration.genesis_block;
    BOOST_REQUIRE_EQUAL(encode_base16(genesis_block.to_data()), "0100000000000000000000000000000000000000000000000000000000000000000000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4adae5494dffff001d1aa4ae180101000000010000000000000000000000000000000000000000000000000000000000000000ffffffff4d04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73ffffffff0100f2052a01000000434104678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5fac00000000");
    BOOST_REQUIRE_EQUAL(configuration.first_version, 1);
    BOOST_REQUIRE_EQUAL(configuration.bip34_version, 2);
    BOOST_REQUIRE_EQUAL(configuration.bip66_version, 3);
    BOOST_REQUIRE_EQUAL(configuration.bip65_version, 4);
    BOOST_REQUIRE_EQUAL(configuration.bip9_version_bit0, 1);
    BOOST_REQUIRE_EQUAL(configuration.bip9_version_bit1, 2);
    BOOST_REQUIRE_EQUAL(configuration.bip9_version_base, 0x20000000);
    BOOST_REQUIRE_EQUAL(configuration.activation_threshold, 51);
    BOOST_REQUIRE_EQUAL(configuration.enforcement_threshold, 75);
    BOOST_REQUIRE_EQUAL(configuration.activation_sample, 100);
    BOOST_REQUIRE_EQUAL(configuration.bip65_freeze, 581885);
    BOOST_REQUIRE_EQUAL(configuration.bip66_freeze, 330776);
    BOOST_REQUIRE_EQUAL(configuration.bip34_freeze, 21111);
    BOOST_REQUIRE_EQUAL(configuration.bip16_activation_time, 1329264000);
    const chain::check_point bip34_active("0000000023b3a96d3484e5abb3755c413e7d41500f8e2a5c3f0dd01299cd8ef8", configuration.bip34_freeze);
    BOOST_REQUIRE_EQUAL(configuration.bip34_active_checkpoint, bip34_active);
    const chain::check_point bit0_active("00000000025e930139bac5c6c31a403776da130831ab85be56578f3fa75369bb", 770112);
    BOOST_REQUIRE_EQUAL(configuration.bip9_bit0_active_checkpoint, bit0_active);
    const chain::check_point bit1_active("00000000002b980fcd729daaa248fd9316a5200e9b367f4ff2c42453e84201ca", 834624);
    BOOST_REQUIRE_EQUAL(configuration.bip9_bit1_active_checkpoint, bit1_active);
    BOOST_REQUIRE_EQUAL(configuration.initial_subsidy_bitcoin, 50);
    BOOST_REQUIRE_EQUAL(configuration.subsidy_interval_blocks, 210000);
    BOOST_REQUIRE_EQUAL(configuration.bitcoin_to_satoshi(1), 100000000);
    BOOST_REQUIRE_EQUAL(configuration.max_money(), 2099999997690000);
}

BOOST_AUTO_TEST_CASE(settings__construct__regtest_context__expected)
{
    settings configuration(chain::selection::regtest);
    BOOST_REQUIRE_EQUAL(configuration.block_spacing_seconds, 600);
    BOOST_REQUIRE_EQUAL(configuration.timestamp_limit_seconds, 7200);
    BOOST_REQUIRE_EQUAL(configuration.retargeting_interval_seconds, 1209600);
    BOOST_REQUIRE_EQUAL(configuration.proof_of_work_limit, 545259519);
    BOOST_REQUIRE_EQUAL(configuration.minimum_timespan(), 302400);
    BOOST_REQUIRE_EQUAL(configuration.maximum_timespan(), 4838400);
    BOOST_REQUIRE_EQUAL(configuration.retargeting_interval(), 2016);
    BOOST_REQUIRE_EQUAL(configuration.work_limit(), uint256_t(chain::compact(545259519)));
    const chain::block genesis_block = configuration.genesis_block;
    BOOST_REQUIRE_EQUAL(encode_base16(genesis_block.to_data()), "0100000000000000000000000000000000000000000000000000000000000000000000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4adae5494dffff7f20020000000101000000010000000000000000000000000000000000000000000000000000000000000000ffffffff4d04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73ffffffff0100f2052a01000000434104678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5fac00000000");
    BOOST_REQUIRE_EQUAL(configuration.first_version, 1);
    BOOST_REQUIRE_EQUAL(configuration.bip34_version, 2);
    BOOST_REQUIRE_EQUAL(configuration.bip66_version, 3);
    BOOST_REQUIRE_EQUAL(configuration.bip65_version, 4);
    BOOST_REQUIRE_EQUAL(configuration.bip9_version_bit0, 1);
    BOOST_REQUIRE_EQUAL(configuration.bip9_version_bit1, 2);
    BOOST_REQUIRE_EQUAL(configuration.bip9_version_base, 0x20000000);
    BOOST_REQUIRE_EQUAL(configuration.bip65_freeze, 1351);
    BOOST_REQUIRE_EQUAL(configuration.bip66_freeze, 1251);
    BOOST_REQUIRE_EQUAL(configuration.bip34_freeze, 0);
    BOOST_REQUIRE_EQUAL(configuration.bip16_activation_time, 1329264000);
    const chain::check_point genesis(genesis_block.hash(), 0);
    BOOST_REQUIRE_EQUAL(configuration.bip34_active_checkpoint, genesis);
    BOOST_REQUIRE_EQUAL(configuration.bip9_bit0_active_checkpoint, genesis);
    BOOST_REQUIRE_EQUAL(configuration.bip9_bit1_active_checkpoint, genesis);
    BOOST_REQUIRE_EQUAL(configuration.initial_subsidy_bitcoin, 50);
    BOOST_REQUIRE_EQUAL(configuration.subsidy_interval_blocks, 150);
    BOOST_REQUIRE_EQUAL(configuration.bitcoin_to_satoshi(1), 100000000);
    BOOST_REQUIRE_EQUAL(configuration.max_money(), 1499999998350);
}

// setter methods
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(settings__initial_block_subsidy_bitcoin__set_double_value__max_money_doubled)
{
    settings configuration(chain::selection::mainnet);
    const auto double_subsidy = configuration.initial_subsidy_bitcoin * 2;
    const auto double_subsidy_satoshi = configuration.bitcoin_to_satoshi(double_subsidy);
    const auto double_max_money = configuration.max_money() + double_subsidy_satoshi * configuration.subsidy_interval_blocks;
    configuration.initial_subsidy_bitcoin = double_subsidy;
    BOOST_REQUIRE_EQUAL(configuration.max_money(), double_max_money);
}

BOOST_AUTO_TEST_CASE(settings__subsidy_interval__set_double_value__max_money_doubled)
{
    settings configuration(chain::selection::mainnet);
    const auto double_subsidy_interval = configuration.subsidy_interval_blocks * 2;
    const auto double_max_money = configuration.max_money() * 2;
    configuration.subsidy_interval_blocks = double_subsidy_interval;
    BOOST_REQUIRE_EQUAL(configuration.max_money(), double_max_money);
}

BOOST_AUTO_TEST_CASE(settings__retargeting_factor__set_double_value__timespan_limits_scaled)
{
    settings configuration(chain::selection::mainnet);
    const auto double_maximum_timespan = configuration.maximum_timespan() * 2;
    const auto half_minimum_timespan = configuration.minimum_timespan() / 2;
    configuration.retargeting_factor = 8;
    BOOST_REQUIRE_EQUAL(configuration.maximum_timespan(), double_maximum_timespan);
    BOOST_REQUIRE_EQUAL(configuration.minimum_timespan(), half_minimum_timespan);
}

BOOST_AUTO_TEST_CASE(settings__retargeting_interval_seconds__set_double_value__timespan_limits_scaled)
{
    settings configuration(chain::selection::mainnet);
    const auto double_interval_seconds = configuration.retargeting_interval_seconds * 2;
    const auto double_maximum_timespan = configuration.maximum_timespan() * 2;
    const auto double_minimum_timespan = configuration.minimum_timespan() * 2;
    const auto double_retargeting_interval = configuration.retargeting_interval() * 2;
    configuration.retargeting_interval_seconds = double_interval_seconds;
    BOOST_REQUIRE_EQUAL(configuration.maximum_timespan(), double_maximum_timespan);
    BOOST_REQUIRE_EQUAL(configuration.minimum_timespan(), double_minimum_timespan);
    BOOST_REQUIRE_EQUAL(configuration.retargeting_interval(), double_retargeting_interval);
}

BOOST_AUTO_TEST_CASE(settings__block_spacing_seconds__set_double_value__retargeting_interval_halved)
{
    settings configuration(chain::selection::mainnet);
    const auto double_block_spacing_seconds = configuration.block_spacing_seconds * 2;
    const auto half_retargeting_interval = configuration.retargeting_interval() / 2;
    configuration.block_spacing_seconds = double_block_spacing_seconds;
    BOOST_REQUIRE_EQUAL(configuration.retargeting_interval(), half_retargeting_interval);
}

BOOST_AUTO_TEST_CASE(settings__work_limit__set_double_value__work_limit_scaled)
{
    settings configuration(chain::selection::mainnet);
    configuration.proof_of_work_limit *= 2;
    const uint256_t double_work_limit(system::chain::compact(configuration.proof_of_work_limit));
    BOOST_REQUIRE_EQUAL(configuration.work_limit(), double_work_limit);
}

BOOST_AUTO_TEST_SUITE_END()
