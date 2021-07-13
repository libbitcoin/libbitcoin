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
#ifndef LIBBITCOIN_SYSTEM_MESSAGE_MESSAGE_HPP
#define LIBBITCOIN_SYSTEM_MESSAGE_MESSAGE_HPP

#include <cstdint>
#include <cstddef>
#include <bitcoin/system/message/address.hpp>
#include <bitcoin/system/message/alert.hpp>
#include <bitcoin/system/message/alert_payload.hpp>
#include <bitcoin/system/message/block.hpp>
#include <bitcoin/system/message/block_transactions.hpp>
#include <bitcoin/system/message/compact_block.hpp>
#include <bitcoin/system/message/compact_filter.hpp>
#include <bitcoin/system/message/compact_filter_checkpoint.hpp>
#include <bitcoin/system/message/compact_filter_headers.hpp>
#include <bitcoin/system/message/fee_filter.hpp>
#include <bitcoin/system/message/filter_add.hpp>
#include <bitcoin/system/message/filter_clear.hpp>
#include <bitcoin/system/message/filter_load.hpp>
#include <bitcoin/system/message/get_address.hpp>
#include <bitcoin/system/message/get_block_transactions.hpp>
#include <bitcoin/system/message/get_blocks.hpp>
#include <bitcoin/system/message/get_compact_filter_checkpoint.hpp>
#include <bitcoin/system/message/get_compact_filter_headers.hpp>
#include <bitcoin/system/message/get_compact_filters.hpp>
#include <bitcoin/system/message/get_data.hpp>
#include <bitcoin/system/message/get_headers.hpp>
#include <bitcoin/system/message/headers.hpp>
#include <bitcoin/system/message/heading.hpp>
#include <bitcoin/system/message/inventory.hpp>
#include <bitcoin/system/message/inventory_vector.hpp>
#include <bitcoin/system/message/memory_pool.hpp>
#include <bitcoin/system/message/merkle_block.hpp>
#include <bitcoin/system/message/network_address.hpp>
#include <bitcoin/system/message/not_found.hpp>
#include <bitcoin/system/message/ping.hpp>
#include <bitcoin/system/message/pong.hpp>
#include <bitcoin/system/message/reject.hpp>
#include <bitcoin/system/message/send_compact.hpp>
#include <bitcoin/system/message/send_headers.hpp>
#include <bitcoin/system/message/transaction.hpp>
#include <bitcoin/system/message/verack.hpp>
#include <bitcoin/system/message/version.hpp>

// Minimum current libbitcoin protocol version:     31402
// Minimum current satoshi client protocol version: 31800

// libbitcoin-network
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// version      v2      70001           added relay field
// verack       v1
// getaddr      v1
// addr         v1
// ping         v1
// ping         v2      60001   BIP031  added nonce field
// pong         v1      60001   BIP031
// reject       v3      70002   BIP061
// ----------------------------------------------------------------------------
// alert        --                      no intent to support
// checkorder   --                      obsolete
// reply        --                      obsolete
// submitorder  --                      obsolete
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// libbitcoin-node
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// getblocks    v1
// inv          v1
// getdata      v1
// getdata      v3      70001   BIP037  allows filtered_block flag
// block        v1
// tx           v1
// notfound     v2      70001
// getheaders   v3      31800
// headers      v3      31800
// mempool      --      60002   BIP035
// mempool      v3      70002           allow multiple inv messages in reply
// sendheaders  v3      70012   BIP130
// feefilter    v3      70013   BIP133
// blocktxn     v3      70014   BIP152
// cmpctblock   v3      70014   BIP152
// getblocktxn  v3      70014   BIP152
// sendcmpct    v3      70014   BIP152
// merkleblock  v3      70001   BIP037  no bloom filters so unfiltered only
// ----------------------------------------------------------------------------
// filterload   --      70001   BIP037  no intent to support, see BIP111
// filteradd    --      70001   BIP037  no intent to support, see BIP111
// filterclear  --      70001   BIP037  no intent to support, see BIP111
// ----------------------------------------------------------------------------
// cfilter      --      70015   BIP157
// getcfilters  --      70015   BIP157
// cfcheckpt    --      70015   BIP157
// getcfcheckpt --      70015   BIP157
// cfheaders    --      70015   BIP157
// getcfheaders --      70015   BIP157
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

namespace libbitcoin {
namespace system {

#define DECLARE_MESSAGE_POINTER_TYPES(type) \
typedef message::type::ptr type##_ptr; \
typedef message::type::const_ptr type##_const_ptr

#define DECLARE_MESSAGE_POINTER_LIST_POINTER_TYPES(type) \
typedef message::type::ptr_list type##_ptr_list; \
typedef message::type::const_ptr_list type##_const_ptr_list; \
typedef message::type::const_ptr_list_ptr type##_const_ptr_list_ptr; \
typedef message::type::const_ptr_list_const_ptr type##_const_ptr_list_const_ptr

// HACK: declare these in bc namespace to reduce length.
DECLARE_MESSAGE_POINTER_TYPES(address);
DECLARE_MESSAGE_POINTER_TYPES(block);
DECLARE_MESSAGE_POINTER_TYPES(compact_block);
DECLARE_MESSAGE_POINTER_TYPES(compact_filter);
DECLARE_MESSAGE_POINTER_TYPES(compact_filter_checkpoint);
DECLARE_MESSAGE_POINTER_TYPES(compact_filter_headers);
DECLARE_MESSAGE_POINTER_TYPES(get_address);
DECLARE_MESSAGE_POINTER_TYPES(fee_filter);
DECLARE_MESSAGE_POINTER_TYPES(get_blocks);
DECLARE_MESSAGE_POINTER_TYPES(get_compact_filter_checkpoint);
DECLARE_MESSAGE_POINTER_TYPES(get_compact_filter_headers);
DECLARE_MESSAGE_POINTER_TYPES(get_compact_filters);
DECLARE_MESSAGE_POINTER_TYPES(get_data);
DECLARE_MESSAGE_POINTER_TYPES(get_headers);
DECLARE_MESSAGE_POINTER_TYPES(header);
DECLARE_MESSAGE_POINTER_TYPES(headers);
DECLARE_MESSAGE_POINTER_TYPES(inventory);
DECLARE_MESSAGE_POINTER_TYPES(memory_pool);
DECLARE_MESSAGE_POINTER_TYPES(merkle_block);
DECLARE_MESSAGE_POINTER_TYPES(not_found);
DECLARE_MESSAGE_POINTER_TYPES(ping);
DECLARE_MESSAGE_POINTER_TYPES(pong);
DECLARE_MESSAGE_POINTER_TYPES(reject);
DECLARE_MESSAGE_POINTER_TYPES(send_compact);
DECLARE_MESSAGE_POINTER_TYPES(send_headers);
DECLARE_MESSAGE_POINTER_TYPES(transaction);
DECLARE_MESSAGE_POINTER_TYPES(verack);
DECLARE_MESSAGE_POINTER_TYPES(version);
DECLARE_MESSAGE_POINTER_LIST_POINTER_TYPES(block);
DECLARE_MESSAGE_POINTER_LIST_POINTER_TYPES(header);
DECLARE_MESSAGE_POINTER_LIST_POINTER_TYPES(transaction);

#undef DECLARE_MESSAGE_POINTER_TYPES
#undef DECLARE_MESSAGE_POINTER_LIST_POINTER_TYPES

namespace message {

/// Serialize a message object to the Bitcoin wire protocol encoding.
template <typename Message>
data_chunk serialize(uint32_t version, const Message& packet,
    uint32_t magic)
{
    const auto heading_size = heading::satoshi_fixed_size();
    const auto payload_size = packet.serialized_size(version);
    const auto message_size = heading_size + payload_size;

    data_chunk message;
    message.reserve(message_size);
    auto payload = packet.to_data(version);
    message = heading(magic, Message::command, payload).to_data();
    return extend(message, std::move(payload));
}

/// Compute an internal representation of the message checksum.
BC_API uint32_t network_checksum(const data_slice& data);

} // namespace message
} // namespace system
} // namespace libbitcoin

#endif
