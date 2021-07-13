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
#include <bitcoin/system/message/heading.hpp>

#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/message/message.hpp>
#include <bitcoin/system/message/version.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

size_t heading::maximum_size()
{
    // This assumes that the heading doesn't shrink in size.
    return satoshi_fixed_size();
}

// Pre-Witness:
// A maximal inventory is 50,000 entries, the largest valid message.
// Inventory with 50,000 entries is 3 + 36 * 50,000 bytes (1,800,003).
// The variable integer portion is maximum 3 bytes (with a count of 50,000).
// According to protocol documentation get_blocks is limited only by the
// general maximum payload size of 0x02000000 (33,554,432). But this is an
// absurd limit for a message that is properly [10 + log2(height) + 1]. Since
// protocol limits height to 2^32 this is 43. Even with expansion to 2^62
// this is limited to 75. So limit payloads to the max inventory payload size.
// Post-Witness:
// The maximum block size inclusive of witness is greater than 1,800,003, so
// with witness-enabled block size (4,000,000).
size_t heading::maximum_payload_size(uint32_t, bool witness)
{
    static constexpr size_t vector = sizeof(uint32_t) + hash_size;
    static constexpr size_t maximum = 3u + vector * chain::max_inventory;
    static_assert(maximum <= max_size_t, "maximum_payload_size overflow");
    return witness ? chain::max_block_weight : maximum;
}

size_t heading::satoshi_fixed_size()
{
    return sizeof(uint32_t) + chain::command_size + sizeof(uint32_t) +
        sizeof(uint32_t);
}

heading heading::factory(const data_chunk& data)
{
    heading instance;
    instance.from_data(data);
    return instance;
}

heading heading::factory(std::istream& stream)
{
    heading instance;
    instance.from_data(stream);
    return instance;
}

heading heading::factory(reader& source)
{
    heading instance;
    instance.from_data(source);
    return instance;
}

heading::heading()
  : magic_(0), command_(), payload_size_(0), checksum_(0)
{
}

heading::heading(uint32_t magic, const std::string& command,
    const data_chunk& payload)
  : heading(magic, command, static_cast<uint32_t>(payload.size()),
      network_checksum(payload))
{
    if (payload.size() > max_uint32)
        reset();
}

// protected
heading::heading(uint32_t magic, const std::string& command,
    uint32_t payload_size, uint32_t checksum)
  : magic_(magic), command_(command), payload_size_(payload_size),
    checksum_(checksum)
{
}

heading::heading(const heading& other)
  : heading(other.magic_, other.command_, other.payload_size_, other.checksum_)
{
}

heading::heading(heading&& other)
  : heading(other.magic_, std::move(other.command_), other.payload_size_,
      other.checksum_)
{
}

bool heading::is_valid() const
{
    return (magic_ != 0)
        || (payload_size_ != 0)
        || (checksum_ != 0)
        || !command_.empty();
}

void heading::reset()
{
    magic_ = 0;
    command_.clear();
    command_.shrink_to_fit();
    payload_size_ = 0;
    checksum_ = 0;
}

bool heading::from_data(const data_chunk& data)
{
    stream::in::copy istream(data);
    return from_data(istream);
}

bool heading::from_data(std::istream& stream)
{
    read::bytes::istream source(stream);
    return from_data(source);
}

bool heading::from_data(reader& source)
{
    reset();
    magic_ = source.read_4_bytes_little_endian();
    command_ = source.read_string(chain::command_size);
    payload_size_ = source.read_4_bytes_little_endian();
    checksum_ = source.read_4_bytes_little_endian();

    if (!source)
        reset();

    return source;
}

data_chunk heading::to_data() const
{
    data_chunk data;
    const auto size = satoshi_fixed_size();
    data.reserve(size);
    stream::out::data ostream(data);
    to_data(ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void heading::to_data(std::ostream& stream) const
{
    write::bytes::ostream out(stream);
    to_data(out);
}

void heading::to_data(writer& sink) const
{
    sink.write_4_bytes_little_endian(magic_);
    sink.write_string(command_, chain::command_size);
    sink.write_4_bytes_little_endian(payload_size_);
    sink.write_4_bytes_little_endian(checksum_);
}

message_type heading::type() const
{
    // TODO: convert to static map.
    if (command_ == address::command)
        return message_type::address;
    if (command_ == alert::command)
        return message_type::alert;
    if (command_ == block_transactions::command)
        return message_type::block_transactions;
    if (command_ == block::command)
        return message_type::block;
    if (command_ == compact_block::command)
        return message_type::compact_block;
    if (command_ == compact_filter_checkpoint::command)
        return message_type::compact_filter_checkpoint;
    if (command_ == compact_filter_headers::command)
        return message_type::compact_filter_headers;
    if (command_ == compact_filter::command)
        return message_type::compact_filter;
    if (command_ == fee_filter::command)
        return message_type::fee_filter;
    if (command_ == filter_add::command)
        return message_type::filter_add;
    if (command_ == filter_clear::command)
        return message_type::filter_clear;
    if (command_ == filter_load::command)
        return message_type::filter_load;
    if (command_ == get_address::command)
        return message_type::get_address;
    if (command_ == get_block_transactions::command)
        return message_type::get_block_transactions;
    if (command_ == get_blocks::command)
        return message_type::get_blocks;
    if (command_ == get_compact_filter_checkpoint::command)
        return message_type::get_compact_filter_checkpoint;
    if (command_ == get_compact_filter_headers::command)
        return message_type::get_compact_filter_headers;
    if (command_ == get_compact_filters::command)
        return message_type::get_compact_filters;
    if (command_ == get_data::command)
        return message_type::get_data;
    if (command_ == get_headers::command)
        return message_type::get_headers;
    if (command_ == headers::command)
        return message_type::headers;
    if (command_ == inventory::command)
        return message_type::inventory;
    if (command_ == memory_pool::command)
        return message_type::memory_pool;
    if (command_ == merkle_block::command)
        return message_type::merkle_block;
    if (command_ == not_found::command)
        return message_type::not_found;
    if (command_ == ping::command)
        return message_type::ping;
    if (command_ == pong::command)
        return message_type::pong;
    if (command_ == reject::command)
        return message_type::reject;
    if (command_ == send_compact::command)
        return message_type::send_compact;
    if (command_ == send_headers::command)
        return message_type::send_headers;
    if (command_ == transaction::command)
        return message_type::transaction;
    if (command_ == verack::command)
        return message_type::verack;
    if (command_ == version::command)
        return message_type::version;

    return message_type::unknown;
}

uint32_t heading::magic() const
{
    return magic_;
}

void heading::set_magic(uint32_t value)
{
    magic_ = value;
}

const std::string& heading::command() const
{
    return command_;
}

void heading::set_command(const std::string& value)
{
    command_ = value;
}

void heading::set_command(std::string&& value)
{
    command_ = std::move(value);
}

uint32_t heading::payload_size() const
{
    return payload_size_;
}

bool heading::verify_checksum(const data_slice& body) const
{
    return network_checksum(body) == checksum_;
}

heading& heading::operator=(heading&& other)
{
    magic_ = other.magic_;
    command_ = std::move(other.command_);
    payload_size_ = other.payload_size_;
    checksum_ = other.checksum_;
    return *this;
}

bool heading::operator==(const heading& other) const
{
    return (magic_ == other.magic_)
        && (command_ == other.command_)
        && (payload_size_ == other.payload_size_)
        && (checksum_ == other.checksum_);
}

bool heading::operator!=(const heading& other) const
{
    return !(*this == other);
}

} // namespace message
} // namespace system
} // namespace libbitcoin
