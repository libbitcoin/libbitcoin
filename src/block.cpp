/*
 * Copyright (c) 2011-2013 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <bitcoin/bitcoin/block.hpp>

#include <bitcoin/bitcoin/constants.hpp>
#include <bitcoin/bitcoin/formats/base16.hpp>
#include <bitcoin/bitcoin/transaction.hpp>
#include <bitcoin/bitcoin/math/hash.hpp>
#include <bitcoin/bitcoin/utility/assert.hpp>
#include <bitcoin/bitcoin/utility/serializer.hpp>

namespace libbitcoin {

uint64_t block_value(size_t height)
{
    uint64_t subsidy = coin_price(initial_block_reward);
    subsidy >>= (height / reward_interval);
    return subsidy;
}

hash_number block_work(uint32_t bits)
{
    hash_number target;
    if (!target.set_compact(bits))
        return 0;

    if (target == 0)
        return 0;

    // We need to compute 2**256 / (bnTarget+1), but we can't represent 2**256
    // as it's too large for a uint256. However as 2**256 is at least as large
    // as bnTarget+1 it's equal to ((2**256 - bnTarget - 1) / (bnTarget+1)) + 1
    // or ~bnTarget / (nTarget+1) + 1.
    return (~target / (target + 1)) + 1;
}

hash_digest hash_block_header(const block_header_type& header)
{
    data_chunk raw_block_header = header;
    return bitcoin_hash(raw_block_header);
}

index_list block_locator_indexes(size_t top_height)
{
    index_list indexes;
    BITCOIN_ASSERT(top_height <= bc::max_int32);
    const auto height32 = static_cast<int32_t>(top_height);

    // Start at max_height, push last 10 indexes first.
    // TODO: modify loop so conversion to signed int is not required.
    int32_t start = 0, step = 1;
    for (auto index = height32; index > 0; index -= step, ++start)
    {
        if (start >= 10)
            step *= 2;

        indexes.push_back(index);
    }

    indexes.push_back(0);
    return indexes;
}

block_type genesis_block()
{
    block_header_type header;
    header.version = 1;
    header.previous_block_hash = null_hash;
    header.merkle = hash_literal(
        "4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    header.bits = 0x1d00ffff;

#ifdef ENABLE_TESTNET
    header.timestamp = 1296688602;
    header.nonce = 414098458;
#else
    header.timestamp = 1231006505;
    header.nonce = 2083236893;
#endif

    transaction_type coinbase_tx;
    coinbase_tx.version = 1;
    coinbase_tx.locktime = 0;

    transaction_input_type coinbase_input;
    coinbase_input.previous_output.hash = null_hash;
<<<<<<< HEAD
    coinbase_input.previous_output.index = bc::max_uint32;
    coinbase_input.script = raw_data_script(genesis_input_script);
    coinbase_input.sequence = max_input_sequence;
=======
    coinbase_input.previous_output.index =
        std::numeric_limits<uint32_t>::max();
    // The Times 03/Jan/2009 Chancellor on brink of second bailout for banks
    coinbase_input.script.operations.push_back(
        operation{ opcode::raw_data,
        data_chunk{ 0x04, 0xff, 0xff, 0x00, 0x1d, 0x01, 0x04, 0x45,
                   0x54, 0x68, 0x65, 0x20, 0x54, 0x69, 0x6d, 0x65,
                   0x73, 0x20, 0x30, 0x33, 0x2f, 0x4a, 0x61, 0x6e,
                   0x2f, 0x32, 0x30, 0x30, 0x39, 0x20, 0x43, 0x68,
                   0x61, 0x6e, 0x63, 0x65, 0x6c, 0x6c, 0x6f, 0x72,
                   0x20, 0x6f, 0x6e, 0x20, 0x62, 0x72, 0x69, 0x6e,
                   0x6b, 0x20, 0x6f, 0x66, 0x20, 0x73, 0x65, 0x63,
                   0x6f, 0x6e, 0x64, 0x20, 0x62, 0x61, 0x69, 0x6c,
                   0x6f, 0x75, 0x74, 0x20, 0x66, 0x6f, 0x72, 0x20,
                   0x62, 0x61, 0x6e, 0x6b, 0x73 } });

    coinbase_input.sequence = std::numeric_limits<uint32_t>::max();
>>>>>>> Refactor primitive types, type parsing code.
    coinbase_tx.inputs.push_back(coinbase_input);

    transaction_output_type coinbase_output;
    coinbase_output.value = coin_price(50);
<<<<<<< HEAD
    coinbase_output.script.push_operation(genesis_output_script);
    coinbase_output.script.push_operation(
        operation{ opcode::checksig, data_chunk() });
=======
    coinbase_output.script.operations.push_back(
        operation{opcode::special,
            data_chunk{0x04, 0x67, 0x8a, 0xfd, 0xb0, 0xfe, 0x55, 0x48,
                       0x27, 0x19, 0x67, 0xf1, 0xa6, 0x71, 0x30, 0xb7,
                       0x10, 0x5c, 0xd6, 0xa8, 0x28, 0xe0, 0x39, 0x09,
                       0xa6, 0x79, 0x62, 0xe0, 0xea, 0x1f, 0x61, 0xde,
                       0xb6, 0x49, 0xf6, 0xbc, 0x3f, 0x4c, 0xef, 0x38,
                       0xc4, 0xf3, 0x55, 0x04, 0xe5, 0x1e, 0xc1, 0x12,
                       0xde, 0x5c, 0x38, 0x4d, 0xf7, 0xba, 0x0b, 0x8d,
                       0x57, 0x8a, 0x4c, 0x70, 0x2b, 0x6b, 0xf1, 0x1d,
                       0x5f}});
    coinbase_output.script.operations.push_back(
        operation{opcode::checksig, data_chunk()});
>>>>>>> Refactor primitive types, type parsing code.
    coinbase_tx.outputs.push_back(coinbase_output);

    block_type genesis;
    genesis.header = header;
    genesis.transactions.push_back(coinbase_tx);
    BITCOIN_ASSERT(genesis.transactions.size() == 1);
    BITCOIN_ASSERT(
        generate_merkle_root(genesis.transactions) == genesis.header.merkle);
    return genesis;
}

} // namespace libbitcoin

