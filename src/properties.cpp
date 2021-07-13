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

#include <bitcoin/system/properties.hpp>

#include <cstdint>
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#undef BOOST_BIND_NO_PLACEHOLDERS
#include <boost/property_tree/json_parser.hpp>
#define BOOST_BIND_NO_PLACEHOLDERS
#include <bitcoin/system/chain/chain.hpp>
#include <bitcoin/system/config/config.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/wallet/wallet.hpp>

namespace libbitcoin {
namespace system {

using namespace pt;
using namespace bc::system::chain;
using namespace bc::system::config;
using namespace bc::system::wallet;

// property_tree is very odd in that what one might consider a node or element,
// having a "containing" name cannot be added into another node without
// creating an intervening name/container. so we create uncontained nodes and
// lists which can then be added to a parent node, creating the named container
// on the add.

// Edit with care - text property names trade DRY for readability.
// Edit with care - tests are affected by property ORDER (keep alphabetical).

// headers

ptree property_list(const config::header& header)
{
    const chain::header& block_header = header;

    ptree tree;
    tree.put("bits", block_header.bits());
    tree.put("hash", hash256(block_header.hash()));
    tree.put("merkle_root", hash256(block_header.merkle_root()));
    tree.put("nonce", block_header.nonce());
    tree.put("previous_block_hash", hash256(block_header.previous_block_hash()));
    tree.put("time_stamp", block_header.timestamp());
    tree.put("version", block_header.version());
    return tree;
}

ptree property_tree(const config::header& header)
{
    ptree tree;
    tree.add_child("header", property_list(header));
    return tree;
}

ptree property_tree(const std::vector<config::header>& headers, bool json)
{
    ptree tree;
    tree.add_child("headers", property_tree_list("header", headers, json));
    return tree;
}

// block

ptree property_list(const chain::block& block, bool json)
{
    ptree tree = property_list(block.header());
    tree.add_child("transactions", property_tree_list_of_lists("transaction",
        block.transactions(), json));
    return tree;
}

ptree property_tree(const chain::block& block, bool json)
{
    ptree tree;
    tree.add_child("block", property_list(block, json));
    return tree;
}

// inputs

ptree property_list(const chain::input& tx_input)
{
    ptree tree;

    // This does not support pay_multisig or pay_public_key (nonstandard).
    // This will have default versioning, but the address version is unused.
    const auto address = payment_address::extract_output(tx_input.script());

    if (address)
        tree.put("address_hash", hash160(address.hash()));

    tree.put("previous_output.hash", hash256(tx_input.previous_output().hash()));
    tree.put("previous_output.index", tx_input.previous_output().index());
    tree.put("script", tx_input.script().to_string(rule_fork::all_rules));
    tree.put("sequence", tx_input.sequence());

    if (tx_input.is_segregated())
        tree.put("witness", tx_input.witness().to_string());

    return tree;
}

ptree property_tree(const chain::input& tx_input)
{
    ptree tree;
    tree.add_child("input", property_list(tx_input));
    return tree;
}

ptree property_tree(const chain::input::list& tx_inputs, bool json)
{
    ptree tree;
    tree.add_child("inputs", property_tree_list("input", tx_inputs, json));
    return tree;
}

ptree property_list(const config::input& input)
{
    const chain::input& tx_input = input;
    return property_list(tx_input);
}

ptree property_tree(const config::input& input)
{
    ptree tree;
    tree.add_child("input", property_list(input));
    return tree;
}

ptree property_tree(const std::vector<config::input>& inputs, bool json)
{
    const auto tx_inputs = cast<chain::input>(inputs);

    ptree tree;
    tree.add_child("inputs", property_tree_list("input", tx_inputs, json));
    return tree;
}

// outputs

ptree property_list(const chain::output& tx_output)
{
    ptree tree;

    // This does not support pay_multisig or pay_public_key (nonstandard).
    // This will have default versioning, but the address version is unused.
    const auto address = payment_address::extract_output(tx_output.script());

    if (address)
        tree.put("address_hash", hash160(address.hash()));

    tree.put("script", tx_output.script().to_string(rule_fork::all_rules));

    // TODO: this will eventually change due to privacy problems, see:
    // lists.dyne.org/lurker/message/20140812.214120.317490ae.en.html

    if (!address)
    {
        uint32_t stealth_prefix;
        ec_compressed ephemeral_key;

        if (to_stealth_prefix(stealth_prefix, tx_output.script()) &&
            extract_ephemeral_key(ephemeral_key, tx_output.script()))
        {
            tree.put("stealth.prefix", stealth_prefix);
            tree.put("stealth.ephemeral_public_key", ec_public(ephemeral_key));
        }
    }

    tree.put("value", tx_output.value());
    return tree;
}

ptree property_tree(const chain::output& tx_output)
{
    ptree tree;
    tree.add_child("output", property_list(tx_output));
    return tree;
}

ptree property_tree(const chain::output::list& tx_outputs, bool json)
{
    ptree tree;
    tree.add_child("outputs", property_tree_list("output", tx_outputs, json));
    return tree;
}

// points

ptree property_list(const chain::point_value& point)
{
    ptree tree;
    tree.put("hash", hash256(point.hash()));
    tree.put("index", point.index());
    tree.put("value", point.value());
    return tree;
}

ptree property_tree(const chain::points_value& values, bool json)
{
    ptree tree;
    tree.add_child("points", property_tree_list("point", values.points, json));
    return tree;
}

// transactions

ptree property_list(const config::transaction& transaction, bool json)
{
    const chain::transaction& tx = transaction;

    ptree tree;
    tree.put("hash", hash256(tx.hash()));
    tree.add_child("inputs", property_tree_list("input", tx.inputs(), json));
    tree.put("lock_time", tx.locktime());
    tree.add_child("outputs", property_tree_list("output", tx.outputs(), json));
    tree.put("version", tx.version());
    return tree;
}

ptree property_tree(const config::transaction& transaction, bool json)
{
    ptree tree;
    tree.add_child("transaction", property_list(transaction, json));
    return tree;
}

ptree property_tree(const std::vector<config::transaction>& transactions,
    bool json)
{
    ptree tree;
    tree.add_child("transactions", property_tree_list_of_lists("transaction",
        transactions, json));
    return tree;
}

// payment

ptree property_list(const payment& payment)
{
    // checksum format has been changed to base16 because integer encoding is
    // ambiguous with respect to endianness. Byte order in base16 is always
    // big-endian with the exception of hashes, which are little-endian.

    ptree tree;
    tree.put("checksum", base16(payment.checksum()));
    tree.put("payload", base16(payment.payload()));
    tree.put("version", payment.prefix().front());
    return tree;
}

ptree property_tree(const payment& payment)
{
    ptree tree;
    tree.add_child("wrapper", property_list(payment));
    return tree;
}

// metadata

ptree property_list(const hash_digest& hash, size_t height, size_t index)
{
    ptree tree;
    tree.put("hash", hash256(hash));
    tree.put("height", height);
    tree.put("index", index);
    return tree;
}

ptree property_tree(const hash_digest& hash, size_t height, size_t index)
{
    ptree tree;
    tree.add_child("metadata", property_list(hash, height, index));
    return tree;
}

// settings

ptree property_tree(const settings_list& settings)
{
    ptree list;

    for (const auto& setting: settings)
        list.put(setting.first, setting.second);

    ptree tree;
    tree.add_child("settings", list);
    return tree;
}

// (non-metadata) hash / hash_list

ptree property_list(const hash_digest& hash)
{
    ptree tree;
    tree.put_value(hash256(hash));
    return tree;
}

ptree property_tree(const hash_digest& hash)
{
    ptree tree;
    tree.add_child("hash", property_list(hash));
    return tree;
}

ptree property_list(const hash_list& hashes, bool json)
{
    ptree tree;
    for (const auto& hash: hashes)
        add_child(tree, "hash", property_list(hash), json);

    return tree;
}

ptree property_tree(const hash_list& hashes, bool json)
{
    ptree tree;
    tree.add_child("hashes", property_list(hashes, json));
    return tree;
}

// sequence

ptree property_tree(uint64_t height, uint32_t sequence)
{
    ptree tree;
    tree.put("height", height);
    tree.put("sequence", sequence);
    return tree;
}

// error

ptree property_tree(const std::error_code& code, uint32_t sequence)
{
    ptree tree;
    tree.put("sequence", sequence);
    tree.put("type", "error");
    tree.put("message", code.message());
    tree.put("code", code.value());
    return tree;
}

// stealth_address

ptree property_list(const stealth_address& stealth, bool json)
{
    // We don't serialize a "reuse key" value as this is strictly an
    // optimization for the purpose of serialization and otherwise complicates
    // understanding of what is actually otherwise very simple behavior.
    // So instead we emit the reused key as one of the spend keys.
    // This means that it is typical to see the same key in scan and spend.

    const auto spends = cast<ec_public>(stealth.spend_keys());
    const auto spends_values = property_value_list("public_key", spends, json);

    ptree tree;
    tree.put("encoded", stealth);
    tree.put("filter", stealth.filter());
    tree.put("scan_public_key", ec_public(stealth.scan_key()));
    tree.put("signatures", stealth.signatures());
    tree.add_child("spends", spends_values);
    tree.put("version", stealth.version());
    return tree;
}

ptree property_tree(const stealth_address& stealth, bool json)
{
    ptree tree;
    tree.add_child("stealth_address", property_list(stealth, json));
    return tree;
}

// compact_filter

ptree property_list(const message::compact_filter& filter)
{
    ptree tree;
    tree.put("block", hash256(filter.block_hash()));
    tree.put("filter", base16(filter.filter()));
    tree.put("type", filter.filter_type());

    return tree;
}

ptree property_tree(const message::compact_filter& filter)
{
    ptree tree;
    tree.add_child("compact_filter", property_list(filter));
    return tree;
}

// compact_filter_checkpoint

ptree property_list(const message::compact_filter_checkpoint& checkpoint,
    bool json)
{
    ptree tree;
    tree.put("stop_hash", hash256(checkpoint.stop_hash()));
    tree.put("type", checkpoint.filter_type());
    tree.add_child("headers", property_list(checkpoint.filter_headers(),
        json));

    return tree;
}

ptree property_tree(const message::compact_filter_checkpoint& checkpoint,
    bool json)
{
    ptree tree;
    tree.add_child("compact_filter_checkpoint", property_list(checkpoint, json));
    return tree;
}

// compact_filter_headers

ptree property_list(const message::compact_filter_headers& headers, bool json)
{
    ptree tree;
    tree.put("stop_hash", hash256(headers.stop_hash()));
    tree.put("type", headers.filter_type());
    tree.put("previous_header", hash256(headers.previous_filter_header()));

    tree.add_child("filter_hashes", property_list(headers.filter_hashes(),
        json));

    return tree;
}

ptree property_tree(const message::compact_filter_headers& headers, bool json)
{
    ptree tree;
    tree.add_child("compact_filter_headers", property_list(headers, json));
    return tree;
}

// uri

ptree property_tree(const bitcoin_uri& uri)
{
    ptree uri_props;

    if (!uri.address().empty())
        uri_props.put("address", uri.address());

    if (uri.amount() != 0)
        uri_props.put("amount", uri.amount());

    if (!uri.label().empty())
        uri_props.put("label", uri.label());

    if (!uri.message().empty())
        uri_props.put("message", uri.message());

    if (!uri.r().empty())
        uri_props.put("r", uri.r());

    uri_props.put("scheme", "bitcoin");

    ptree tree;
    tree.add_child("uri", uri_props);
    return tree;
}

// safe json input parsing
bool property_tree(ptree& out, const std::string& json)
{
    try
    {
        stream::in::copy istream(json);
        read_json(istream, out);
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

} // namespace system
} // namespace libbitcoin
