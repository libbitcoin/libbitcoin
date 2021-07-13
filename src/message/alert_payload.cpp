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
#include <bitcoin/system/message/alert_payload.hpp>

#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/message/message.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

// Libbitcoin doesn't use this.
const ec_uncompressed alert_payload::satoshi_public_key
{
    {
        0x04, 0xfc, 0x97, 0x02, 0x84, 0x78, 0x40, 0xaa, 0xf1, 0x95, 0xde,
        0x84, 0x42, 0xeb, 0xec, 0xed, 0xf5, 0xb0, 0x95, 0xcd, 0xbb, 0x9b,
        0xc7, 0x16, 0xbd, 0xa9, 0x11, 0x09, 0x71, 0xb2, 0x8a, 0x49, 0xe0,
        0xea, 0xd8, 0x56, 0x4f, 0xf0, 0xdb, 0x22, 0x20, 0x9e, 0x03, 0x74,
        0x78, 0x2c, 0x09, 0x3b, 0xb8, 0x99, 0x69, 0x2d, 0x52, 0x4e, 0x9d,
        0x6a, 0x69, 0x56, 0xe7, 0xc5, 0xec, 0xbc, 0xd6, 0x82, 0x84
    }
};

alert_payload alert_payload::factory(uint32_t version,
    const data_chunk& data)
{
    alert_payload instance;
    instance.from_data(version, data);
    return instance;
}

alert_payload alert_payload::factory(uint32_t version,
    std::istream& stream)
{
    alert_payload instance;
    instance.from_data(version, stream);
    return instance;
}

alert_payload alert_payload::factory(uint32_t version,
    reader& source)
{
    alert_payload instance;
    instance.from_data(version, source);
    return instance;
}

alert_payload::alert_payload()
  : version_(0),
    relay_until_(0),
    expiration_(0),
    id_(0),
    cancel_(0),
    min_version_(0),
    max_version_(0),
    priority_(0)
{
}

alert_payload::alert_payload(
    uint32_t version,
    uint64_t relay_until,
    uint64_t expiration,
    uint32_t id,
    uint32_t cancel,
    const std::vector<uint32_t>& set_cancel,
    uint32_t min_version,
    uint32_t max_version,
    const std::vector<std::string>& set_sub_version,
    uint32_t priority,
    const std::string& comment,
    const std::string& status_bar,
    const std::string& reserved)
  : version_(version),
    relay_until_(relay_until),
    expiration_(expiration),
    id_(id),
    cancel_(cancel),
    set_cancel_(set_cancel),
    min_version_(min_version),
    max_version_(max_version),
    set_sub_version_(set_sub_version),
    priority_(priority),
    comment_(comment),
    status_bar_(status_bar),
    reserved_(reserved)
{
}

alert_payload::alert_payload(
    uint32_t version,
    uint64_t relay_until,
    uint64_t expiration,
    uint32_t id,
    uint32_t cancel,
    std::vector<uint32_t>&& set_cancel,
    uint32_t min_version,
    uint32_t max_version,
    std::vector<std::string>&& set_sub_version,
    uint32_t priority,
    std::string&& comment,
    std::string&& status_bar,
    std::string&& reserved)
  : version_(version),
    relay_until_(relay_until),
    expiration_(expiration),
    id_(id),
    cancel_(cancel),
    set_cancel_(std::move(set_cancel)),
    min_version_(min_version),
    max_version_(max_version),
    set_sub_version_(std::move(set_sub_version)),
    priority_(priority),
    comment_(std::move(comment)),
    status_bar_(std::move(status_bar)),
    reserved_(std::move(reserved))
{
}

alert_payload::alert_payload(const alert_payload& other)
  : alert_payload(
        other.version_,
        other.relay_until_,
        other.expiration_,
        other.id_,
        other.cancel_,
        other.set_cancel_,
        other.min_version_,
        other.max_version_,
        other.set_sub_version_,
        other.priority_,
        other.comment_,
        other.status_bar_,
        other.reserved_)
{
}

alert_payload::alert_payload(alert_payload&& other)
  : alert_payload(
        other.version_,
        other.relay_until_,
        other.expiration_,
        other.id_,
        other.cancel_,
        std::move(other.set_cancel_),
        other.min_version_,
        other.max_version_,
        std::move(other.set_sub_version_),
        other.priority_,
        std::move(other.comment_),
        std::move(other.status_bar_),
        std::move(other.reserved_))
{
}

bool alert_payload::is_valid() const
{
    return (version_ != 0)
        || (relay_until_ != 0)
        || (expiration_ != 0)
        || (id_ != 0)
        || (cancel_ != 0)
        || !set_cancel_.empty()
        || (min_version_ != 0)
        || (max_version_ != 0)
        || !set_sub_version_.empty()
        || (priority_ != 0)
        || !comment_.empty()
        || !status_bar_.empty()
        || !reserved_.empty();
}

void alert_payload::reset()
{
    version_ = 0;
    relay_until_ = 0;
    expiration_ = 0;
    id_ = 0;
    cancel_ = 0;
    set_cancel_.clear();
    set_cancel_.shrink_to_fit();
    min_version_ = 0;
    max_version_ = 0;
    set_sub_version_.clear();
    set_sub_version_.shrink_to_fit();
    priority_ = 0;
    comment_.clear();
    comment_.shrink_to_fit();
    status_bar_.clear();
    status_bar_.shrink_to_fit();
    reserved_.clear();
    reserved_.shrink_to_fit();
}

bool alert_payload::from_data(uint32_t version, const data_chunk& data)
{
    stream::in::copy istream(data);
    return from_data(version, istream);
}

bool alert_payload::from_data(uint32_t version, std::istream& stream)
{
    read::bytes::istream source(stream);
    return from_data(version, source);
}

bool alert_payload::from_data(uint32_t, reader& source)
{
    reset();

    this->version_ = source.read_4_bytes_little_endian();
    relay_until_ = source.read_8_bytes_little_endian();
    expiration_ = source.read_8_bytes_little_endian();
    id_ = source.read_4_bytes_little_endian();
    cancel_ = source.read_4_bytes_little_endian();
    set_cancel_.reserve(source.read_size());

    for (size_t i = 0; i < set_cancel_.capacity() && source; i++)
        set_cancel_.push_back(source.read_4_bytes_little_endian());

    min_version_ = source.read_4_bytes_little_endian();
    max_version_ = source.read_4_bytes_little_endian();
    set_sub_version_.reserve(source.read_size());

    for (size_t i = 0; i < set_sub_version_.capacity() && source; i++)
        set_sub_version_.push_back(source.read_string());

    priority_ = source.read_4_bytes_little_endian();
    comment_ = source.read_string();
    status_bar_ = source.read_string();
    reserved_ = source.read_string();

    if (!source)
        reset();

    return source;
}

data_chunk alert_payload::to_data(uint32_t version) const
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

void alert_payload::to_data(uint32_t version, std::ostream& stream) const
{
    write::bytes::ostream out(stream);
    to_data(version, out);
}

void alert_payload::to_data(uint32_t, writer& sink) const
{
    sink.write_4_bytes_little_endian(this->version_);
    sink.write_8_bytes_little_endian(relay_until_);
    sink.write_8_bytes_little_endian(expiration_);
    sink.write_4_bytes_little_endian(id_);
    sink.write_4_bytes_little_endian(cancel_);
    sink.write_variable(set_cancel_.size());

    for (const auto& entry: set_cancel_)
        sink.write_4_bytes_little_endian(entry);

    sink.write_4_bytes_little_endian(min_version_);
    sink.write_4_bytes_little_endian(max_version_);
    sink.write_variable(set_sub_version_.size());

    for (const auto& entry: set_sub_version_)
        sink.write_string(entry);

    sink.write_4_bytes_little_endian(priority_);
    sink.write_string(comment_);
    sink.write_string(status_bar_);
    sink.write_string(reserved_);
}

size_t alert_payload::serialized_size(uint32_t) const
{
    size_t size = 40u +
        variable_size(comment_.size()) + comment_.size() +
        variable_size(status_bar_.size()) + status_bar_.size() +
        variable_size(reserved_.size()) + reserved_.size() +
        variable_size(set_cancel_.size()) + (4 * set_cancel_.size()) +
        variable_size(set_sub_version_.size());

    for (const auto& sub_version : set_sub_version_)
        size += variable_size(sub_version.size()) + sub_version.size();

    return size;
}

uint32_t alert_payload::version() const
{
    return version_;
}

void alert_payload::set_version(uint32_t value)
{
    version_ = value;
}

uint64_t alert_payload::relay_until() const
{
    return relay_until_;
}

void alert_payload::set_relay_until(uint64_t value)
{
    relay_until_ = value;
}

uint64_t alert_payload::expiration() const
{
    return expiration_;
}

void alert_payload::set_expiration(uint64_t value)
{
    expiration_ = value;
}

uint32_t alert_payload::id() const
{
    return id_;
}

void alert_payload::set_id(uint32_t value)
{
    id_ = value;
}

uint32_t alert_payload::cancel() const
{
    return cancel_;
}

void alert_payload::set_cancel(uint32_t value)
{
    cancel_ = value;
}

std::vector<uint32_t>& alert_payload::set_cancel()
{
    return set_cancel_;
}

const std::vector<uint32_t>& alert_payload::set_cancel() const
{
    return set_cancel_;
}

void alert_payload::set_set_cancel(const std::vector<uint32_t>& value)
{
    set_cancel_ = value;
}

void alert_payload::set_set_cancel(std::vector<uint32_t>&& value)
{
    set_cancel_ = std::move(value);
}

uint32_t alert_payload::min_version() const
{
    return min_version_;
}

void alert_payload::set_min_version(uint32_t value)
{
    min_version_ = value;
}

uint32_t alert_payload::max_version() const
{
    return max_version_;
}

void alert_payload::set_max_version(uint32_t value)
{
    max_version_ = value;
}

std::vector<std::string>& alert_payload::set_sub_version()
{
    return set_sub_version_;
}

const std::vector<std::string>& alert_payload::set_sub_version() const
{
    return set_sub_version_;
}

void alert_payload::set_set_sub_version(const std::vector<std::string>& value)
{
    set_sub_version_ = value;
}

void alert_payload::set_set_sub_version(std::vector<std::string>&& value)
{
    set_sub_version_ = std::move(value);
}

uint32_t alert_payload::priority() const
{
    return priority_;
}

void alert_payload::set_priority(uint32_t value)
{
    priority_ = value;
}

std::string& alert_payload::comment()
{
    return comment_;
}

const std::string& alert_payload::comment() const
{
    return comment_;
}

void alert_payload::set_comment(const std::string& value)
{
    comment_ = value;
}

void alert_payload::set_comment(std::string&& value)
{
    comment_ = std::move(value);
}

std::string& alert_payload::status_bar()
{
    return status_bar_;
}

const std::string& alert_payload::status_bar() const
{
    return status_bar_;
}

void alert_payload::set_status_bar(const std::string& value)
{
    status_bar_ = value;
}

void alert_payload::set_status_bar(std::string&& value)
{
    status_bar_ = std::move(value);
}

std::string& alert_payload::reserved()
{
    return reserved_;
}

const std::string& alert_payload::reserved() const
{
    return reserved_;
}

void alert_payload::set_reserved(const std::string& value)
{
    reserved_ = value;
}

void alert_payload::set_reserved(std::string&& value)
{
    reserved_ = std::move(value);
}

alert_payload& alert_payload::operator=(alert_payload&& other)
{
    version_ = other.version_;
    relay_until_ = other.relay_until_;
    expiration_ = other.expiration_;
    id_ = other.id_;
    cancel_ = other.cancel_;
    set_cancel_ = std::move(other.set_cancel_);
    min_version_ = other.min_version_;
    max_version_ = other.max_version_;
    set_sub_version_ = std::move(other.set_sub_version_);
    priority_ = other.priority_;
    comment_ = std::move(other.comment_);
    status_bar_ = std::move(other.status_bar_);
    reserved_ = std::move(other.reserved_);
    return *this;
}

bool alert_payload::operator==(const alert_payload& other) const
{
    return (version_ == other.version_)
        && (relay_until_ == other.relay_until_)
        && (expiration_ == other.expiration_)
        && (id_ == other.id_)
        && (cancel_ == other.cancel_)
        && (set_cancel_ == other.set_cancel_)
        && (min_version_ == other.min_version_)
        && (max_version_ == other.max_version_)
        && (set_sub_version_ == other.set_sub_version_)
        && (priority_ == other.priority_)
        && (comment_ == other.comment_)
        && (status_bar_ == other.status_bar_)
        && (reserved_ == other.reserved_);
}

bool alert_payload::operator!=(const alert_payload& other) const
{
    return !(*this == other);
}

} // namespace message
} // namespace system
} // namespace libbitcoin
