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
#include <bitcoin/system/message/merkle_block.hpp>

#include <bitcoin/system/assert.hpp>
#include <bitcoin/system/math/safe.hpp>
#include <bitcoin/system/message/message.hpp>
#include <bitcoin/system/message/version.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

const std::string merkle_block::command = "merkleblock";
const uint32_t merkle_block::version_minimum = version::level::bip37;
const uint32_t merkle_block::version_maximum = version::level::maximum;

merkle_block merkle_block::factory(uint32_t version, const data_chunk& data)
{
    merkle_block instance;
    instance.from_data(version, data);
    return instance;
}

merkle_block merkle_block::factory(uint32_t version, std::istream& stream)
{
    merkle_block instance;
    instance.from_data(version, stream);
    return instance;
}

merkle_block merkle_block::factory(uint32_t version, reader& source)
{
    merkle_block instance;
    instance.from_data(version, source);
    return instance;
}

merkle_block::merkle_block()
  : header_(), total_transactions_(0), hashes_(), flags_()
{
}

merkle_block::merkle_block(const chain::header& header,
    size_t total_transactions, const hash_list& hashes,
    const data_chunk& flags)
  : header_(header), total_transactions_(total_transactions), hashes_(hashes),
    flags_(flags)
{
}

merkle_block::merkle_block(chain::header&& header, size_t total_transactions,
    hash_list&& hashes, data_chunk&& flags)
  : header_(std::move(header)), total_transactions_(total_transactions),
    hashes_(std::move(hashes)), flags_(std::move(flags))
{
}

// Hack: use of safe_unsigned here isn't great. We should consider using size_t
// for the transaction count and invalidating on deserialization and construct.
merkle_block::merkle_block(const chain::block& block)
  : merkle_block(block.header(),
        safe_cast<uint32_t>(block.transactions().size()),
        block.to_hashes(), {})
{
}

merkle_block::merkle_block(const merkle_block& other)
  : merkle_block(other.header_, other.total_transactions_, other.hashes_,
      other.flags_)
{
}

merkle_block::merkle_block(merkle_block&& other)
  : merkle_block(std::move(other.header_), other.total_transactions_,
      std::move(other.hashes_), std::move(other.flags_))
{
}

bool merkle_block::is_valid() const
{
    return !hashes_.empty() || !flags_.empty() || header_.is_valid();
}

void merkle_block::reset()
{
    header_ = chain::header();
    total_transactions_ = 0;
    hashes_.clear();
    hashes_.shrink_to_fit();
    flags_.clear();
    flags_.shrink_to_fit();
}

bool merkle_block::from_data(uint32_t version, const data_chunk& data)
{
    stream::in::copy istream(data);
    return from_data(version, istream);
}

bool merkle_block::from_data(uint32_t version, std::istream& stream)
{
    read::bytes::istream source(stream);
    return from_data(version, source);
}

bool merkle_block::from_data(uint32_t version, reader& source)
{
    reset();

    if (!header_.from_data(source))
        return false;

    total_transactions_ = source.read_4_bytes_little_endian();
    const auto count = source.read_size();

    // Guard against potential for arbitrary memory allocation.
    if (count > chain::max_block_size)
        source.invalidate();
    else
        hashes_.reserve(count);

    for (size_t hash = 0; hash < hashes_.capacity() && source; ++hash)
        hashes_.push_back(source.read_hash());

    flags_ = source.read_bytes(source.read_size());

    if (version < merkle_block::version_minimum)
        source.invalidate();

    if (!source)
        reset();

    return source;
}

data_chunk merkle_block::to_data(uint32_t version) const
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

void merkle_block::to_data(uint32_t version, std::ostream& stream) const
{
    write::bytes::ostream out(stream);
    to_data(version, out);
}

void merkle_block::to_data(uint32_t, writer& sink) const
{
    header_.to_data(sink);

    const auto total32 = safe_cast<uint32_t>(total_transactions_);
    sink.write_4_bytes_little_endian(total32);
    sink.write_variable(hashes_.size());

    for (const auto& hash: hashes_)
        sink.write_bytes(hash);

    sink.write_variable(flags_.size());
    sink.write_bytes(flags_);
}

size_t merkle_block::serialized_size(uint32_t) const
{
    return header_.serialized_size() + 4u +
        variable_size(hashes_.size()) + (hash_size * hashes_.size()) +
        variable_size(flags_.size()) + flags_.size();
}

chain::header& merkle_block::header()
{
    return header_;
}

const chain::header& merkle_block::header() const
{
    return header_;
}

void merkle_block::set_header(const chain::header& value)
{
    header_ = value;
}

void merkle_block::set_header(chain::header&& value)
{
    header_ = std::move(value);
}

size_t merkle_block::total_transactions() const
{
    return total_transactions_;
}

void merkle_block::set_total_transactions(size_t value)
{
    total_transactions_ = value;
}

hash_list& merkle_block::hashes()
{
    return hashes_;
}

const hash_list& merkle_block::hashes() const
{
    return hashes_;
}

void merkle_block::set_hashes(const hash_list& value)
{
    hashes_ = value;
}

void merkle_block::set_hashes(hash_list&& value)
{
    hashes_ = std::move(value);
}

data_chunk& merkle_block::flags()
{
    return flags_;
}

const data_chunk& merkle_block::flags() const
{
    return flags_;
}

void merkle_block::set_flags(const data_chunk& value)
{
    flags_ = value;
}

void merkle_block::set_flags(data_chunk&& value)
{
    flags_ = std::move(value);
}

merkle_block& merkle_block::operator=(merkle_block&& other)
{
    header_ = std::move(other.header_);
    hashes_ = std::move(other.hashes_);
    flags_ = std::move(other.flags_);
    return *this;
}

bool merkle_block::operator==(const merkle_block& other) const
{
    auto result = (header_ == other.header_) &&
        (hashes_.size() == other.hashes_.size()) &&
        (flags_.size() == other.flags_.size());

    for (size_t i = 0; i < hashes_.size() && result; i++)
        result = (hashes_[i] == other.hashes_[i]);

    for (size_t i = 0; i < flags_.size() && result; i++)
        result = (flags_[i] == other.flags_[i]);

    return result;
}

bool merkle_block::operator!=(const merkle_block& other) const
{
    return !(*this == other);
}

} // namespace message
} // namespace system
} // namespace libbitcoin
