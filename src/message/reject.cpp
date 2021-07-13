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
#include <bitcoin/system/message/reject.hpp>

#include <bitcoin/system/message/block.hpp>
#include <bitcoin/system/message/message.hpp>
#include <bitcoin/system/message/transaction.hpp>
#include <bitcoin/system/message/version.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

const std::string reject::command = "reject";
const uint32_t reject::version_minimum = version::level::bip61;
const uint32_t reject::version_maximum = version::level::maximum;

reject reject::factory(uint32_t version,
    const data_chunk& data)
{
    reject instance;
    instance.from_data(version, data);
    return instance;
}

reject reject::factory(uint32_t version,
    std::istream& stream)
{
    reject instance;
    instance.from_data(version, stream);
    return instance;
}

reject reject::factory(uint32_t version,
    reader& source)
{
    reject instance;
    instance.from_data(version, source);
    return instance;
}

reject::reject()
  : code_(reason_code::undefined), message_(), reason_(), data_(null_hash)
{
}

reject::reject(reason_code code, const std::string& message,
    const std::string& reason)
  : reject(code, message, reason, null_hash)
{
}

reject::reject(reason_code code, std::string&& message, std::string&& reason)
  : code_(code),
    message_(std::move(message)),
    reason_(std::move(reason)),
    data_(null_hash)
{
}

reject::reject(reason_code code, const std::string& message,
    const std::string& reason, const hash_digest& data)
  : code_(code),
    message_(message),
    reason_(reason),
    data_(data)
{
}

reject::reject(reason_code code, std::string&& message, std::string&& reason,
    hash_digest&& data)
  : code_(code),
    message_(std::move(message)),
    reason_(std::move(reason)),
    data_(std::move(data))
{
}

reject::reject(const reject& other)
  : reject(other.code_, other.message_, other.reason_, other.data_)
{
}

reject::reject(reject&& other)
  : reject(other.code_, std::move(other.message_), std::move(other.reason_),
      std::move(other.data_))
{
}

bool reject::is_valid() const
{
    return !message_.empty()
        || (code_!= reason_code::undefined)
        || !reason_.empty()
        || (data_!= null_hash);
}

void reject::reset()
{
    message_.clear();
    message_.shrink_to_fit();
    code_ = reason_code::undefined;
    reason_.clear();
    reason_.shrink_to_fit();
    data_.fill(0);
}

bool reject::from_data(uint32_t version, const data_chunk& data)
{
    stream::in::copy istream(data);
    return from_data(version, istream);
}

bool reject::from_data(uint32_t version, std::istream& stream)
{
    read::bytes::istream source(stream);
    return from_data(version, source);
}

bool reject::from_data(uint32_t version, reader& source)
{
    reset();

    message_ = source.read_string();
    code_ = reason_from_byte(source.read_byte());
    reason_ = source.read_string();

    if ((message_ == block::command) ||
        (message_ == transaction::command))
    {
        // Some nodes do not follow the documented convention of supplying hash
        // for tx and block rejects. Use this to prevent error on empty stream.
        const auto bytes = source.read_bytes();

        if (bytes.size() == hash_size)
            data_ = to_array<hash_size>(bytes);
    }

    if (version < reject::version_minimum)
        source.invalidate();

    if (!source)
        reset();

    return source;
}

data_chunk reject::to_data(uint32_t version) const
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

void reject::to_data(uint32_t version, std::ostream& stream) const
{
    write::bytes::ostream out(stream);
    to_data(version, out);
}

void reject::to_data(uint32_t, writer& sink) const
{
    sink.write_string(message_);
    sink.write_byte(reason_to_byte(code_));
    sink.write_string(reason_);

    if ((message_ == block::command) ||
        (message_ == transaction::command))
    {
        sink.write_bytes(data_);
    }
}

size_t reject::serialized_size(uint32_t) const
{
    size_t size = 1u + variable_size(message_.size()) +
        message_.size() + variable_size(reason_.size()) + reason_.size();

    if ((message_ == block::command) ||
        (message_ == transaction::command))
    {
        size += hash_size;
    }

    return size;
}

reject::reason_code reject::code() const
{
    return code_;
}

void reject::set_code(reason_code value)
{
    code_ = value;
}

std::string& reject::message()
{
    return message_;
}

const std::string& reject::message() const
{
    return message_;
}

void reject::set_message(const std::string& value)
{
    message_ = value;
}

void reject::set_message(std::string&& value)
{
    message_ = std::move(value);
}

std::string& reject::reason()
{
    return reason_;
}

const std::string& reject::reason() const
{
    return reason_;
}

void reject::set_reason(const std::string& value)
{
    reason_ = value;
}

void reject::set_reason(std::string&& value)
{
    reason_ = std::move(value);
}

hash_digest& reject::data()
{
    return data_;
}

const hash_digest& reject::data() const
{
    return data_;
}

void reject::set_data(const hash_digest& value)
{
    data_ = value;
}

void reject::set_data(hash_digest&& value)
{
    data_ = std::move(value);
}

reject& reject::operator=(reject&& other)
{
    code_ = other.code_;
    reason_ = std::move(other.reason_);
    message_ = std::move(other.message_);
    data_ = std::move(other.data_);
    return *this;
}

bool reject::operator==(const reject& other) const
{
    return (code_ == other.code_) && (reason_ == other.reason_)
        && (message_ == other.message_) && (data_ == other.data_);
}

bool reject::operator!=(const reject& other) const
{
    return !(*this == other);
}

reject::reason_code reject::reason_from_byte(uint8_t byte)
{
    switch (byte)
    {
        case 0x01:
            return reason_code::malformed;
        case 0x10:
            return reason_code::invalid;
        case 0x11:
            return reason_code::obsolete;
        case 0x12:
            return reason_code::duplicate;
        case 0x40:
            return reason_code::nonstandard;
        case 0x41:
            return reason_code::dust;
        case 0x42:
            return reason_code::insufficient_fee;
        case 0x43:
            return reason_code::checkpoint;
        default:
            return reason_code::undefined;
    }
}

uint8_t reject::reason_to_byte(reason_code value)
{
    switch (value)
    {
        case reason_code::malformed:
            return 0x01;
        case reason_code::invalid:
            return 0x10;
        case reason_code::obsolete:
            return 0x11;
        case reason_code::duplicate:
            return 0x12;
        case reason_code::nonstandard:
            return 0x40;
        case reason_code::dust:
            return 0x41;
        case reason_code::insufficient_fee:
            return 0x42;
        case reason_code::checkpoint:
            return 0x43;
        default:
            return 0x00;
    }
}

} // namespace message
} // namespace system
} // namespace libbitcoin
