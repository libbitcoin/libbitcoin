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
#include <bitcoin/system/message/compact_block.hpp>

#include <initializer_list>
#include <bitcoin/system/message/message.hpp>
#include <bitcoin/system/message/version.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

const std::string compact_block::command = "cmpctblock";
const uint32_t compact_block::version_minimum = version::level::bip152;
const uint32_t compact_block::version_maximum = version::level::bip152;

compact_block compact_block::factory(uint32_t version, const data_chunk& data)
{
    compact_block instance;
    instance.from_data(version, data);
    return instance;
}

compact_block compact_block::factory(uint32_t version, std::istream& stream)
{
    compact_block instance;
    instance.from_data(version, stream);
    return instance;
}

compact_block compact_block::factory(uint32_t version, reader& source)
{
    compact_block instance;
    instance.from_data(version, source);
    return instance;
}

compact_block::compact_block()
  : header_(), nonce_(0), short_ids_(), transactions_()
{
}

compact_block::compact_block(const chain::header& header, uint64_t nonce,
    const short_id_list& short_ids,
    const prefilled_transaction::list& transactions)
  : header_(header),
    nonce_(nonce),
    short_ids_(short_ids),
    transactions_(transactions)
{
}

compact_block::compact_block(chain::header&& header, uint64_t nonce,
    short_id_list&& short_ids, prefilled_transaction::list&& transactions)
  : header_(std::move(header)),
    nonce_(nonce),
    short_ids_(std::move(short_ids)),
    transactions_(std::move(transactions))
{
}

compact_block::compact_block(const compact_block& other)
  : compact_block(other.header_, other.nonce_, other.short_ids_,
      other.transactions_)
{
}

compact_block::compact_block(compact_block&& other)
  : compact_block(std::move(other.header_), other.nonce_,
      std::move(other.short_ids_), std::move(other.transactions_))
{
}

bool compact_block::is_valid() const
{
    return header_.is_valid() && !short_ids_.empty() && !transactions_.empty();
}

void compact_block::reset()
{
    header_ = chain::header();
    nonce_ = 0;
    short_ids_.clear();
    short_ids_.shrink_to_fit();
    transactions_.clear();
    transactions_.shrink_to_fit();
}

bool compact_block::from_data(uint32_t version, const data_chunk& data)
{
    stream::in::copy istream(data);
    return from_data(version, istream);
}

bool compact_block::from_data(uint32_t version, std::istream& stream)
{
    read::bytes::istream source(stream);
    return from_data(version, source);
}

bool compact_block::from_data(uint32_t version, reader& source)
{
    reset();

    if (!header_.from_data(source))
        return false;

    nonce_ = source.read_8_bytes_little_endian();
    auto count = source.read_size();

    // Guard against potential for arbitrary memory allocation.
    if (count > chain::max_block_size)
        source.invalidate();
    else
        short_ids_.reserve(count);

    // Order is required.
    for (size_t id = 0; id < count && source; ++id)
        short_ids_.push_back(source.read_mini_hash());

    count = source.read_size();

    // Guard against potential for arbitrary memory allocation.
    if (count > chain::max_block_size)
        source.invalidate();
    else
        transactions_.resize(count);

    // Order is required.
    for (auto& tx: transactions_)
        if (!tx.from_data(version, source))
            break;

    if (version < compact_block::version_minimum)
        source.invalidate();

    if (!source)
        reset();

    return source;
}

data_chunk compact_block::to_data(uint32_t version) const
{
    data_chunk data;
    const auto size = serialized_size(version);
    data.reserve(size);
    stream::out::data ostream(data);
    to_data(version, ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void compact_block::to_data(uint32_t version, std::ostream& stream) const
{
    write::bytes::ostream out(stream);
    to_data(version, out);
}

void compact_block::to_data(uint32_t version, writer& sink) const
{
    header_.to_data(sink);
    sink.write_8_bytes_little_endian(nonce_);
    sink.write_variable(short_ids_.size());

    for (const auto& element: short_ids_)
        sink.write_bytes(element);

    sink.write_variable(transactions_.size());

    for (const auto& element: transactions_)
        element.to_data(version, sink);
}

size_t compact_block::serialized_size(uint32_t version) const
{
    auto size = chain::header::satoshi_fixed_size() +
        variable_size(short_ids_.size()) +
        (short_ids_.size() * 6u) +
        variable_size(transactions_.size()) + 8u;

    for (const auto& tx: transactions_)
        size += tx.serialized_size(version);

    return size;
}

chain::header& compact_block::header()
{
    return header_;
}

const chain::header& compact_block::header() const
{
    return header_;
}

void compact_block::set_header(const chain::header& value)
{
    header_ = value;
}

void compact_block::set_header(chain::header&& value)
{
    header_ = std::move(value);
}

uint64_t compact_block::nonce() const
{
    return nonce_;
}

void compact_block::set_nonce(uint64_t value)
{
    nonce_ = value;
}

compact_block::short_id_list& compact_block::short_ids()
{
    return short_ids_;
}

const compact_block::short_id_list& compact_block::short_ids() const
{
    return short_ids_;
}

void compact_block::set_short_ids(const short_id_list& value)
{
    short_ids_ = value;
}

void compact_block::set_short_ids(short_id_list&& value)
{
    short_ids_ = std::move(value);
}

prefilled_transaction::list& compact_block::transactions()
{
    return transactions_;
}

const prefilled_transaction::list& compact_block::transactions() const
{
    return transactions_;
}

void compact_block::set_transactions(const prefilled_transaction::list& value)
{
    transactions_ = value;
}

void compact_block::set_transactions(prefilled_transaction::list&& value)
{
    transactions_ = std::move(value);
}

compact_block& compact_block::operator=(compact_block&& other)
{
    header_ = std::move(other.header_);
    nonce_ = other.nonce_;
    short_ids_ = std::move(other.short_ids_);
    transactions_ = std::move(other.transactions_);
    return *this;
}

bool compact_block::operator==(const compact_block& other) const
{
    return (header_ == other.header_) && (nonce_ == other.nonce_)
        && (short_ids_ == other.short_ids_)
        && (transactions_ == other.transactions_);
}

bool compact_block::operator!=(const compact_block& other) const
{
    return !(*this == other);
}

} // namespace message
} // namespace system
} // namespace libbitcoin
