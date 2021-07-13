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
#include <bitcoin/system/message/version.hpp>

#include <algorithm>
#include <bitcoin/system/message/message.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

const std::string version::command = "version";
//const bounds message::version::version = { level::minimum, level::maximum };
const uint32_t message::version::version_minimum = level::minimum;
const uint32_t message::version::version_maximum = level::maximum;

version version::factory(uint32_t version,
    const data_chunk& data)
{
    message::version instance;
    instance.from_data(version, data);
    return instance;
}

version version::factory(uint32_t version,
    std::istream& stream)
{
    message::version instance;
    instance.from_data(version, stream);
    return instance;
}

version version::factory(uint32_t version,
    reader& source)
{
    message::version instance;
    instance.from_data(version, source);
    return instance;
}

version::version()
  : value_(0), services_(0), timestamp_(0), address_receiver_(),
    address_sender_(), nonce_(0), user_agent_(), start_height_(0),
    relay_(false)
{
}

version::version(uint32_t value, uint64_t services, uint64_t timestamp,
    const network_address& address_receiver,
    const network_address& address_sender, uint64_t nonce,
    const std::string& user_agent, uint32_t start_height, bool relay)
  : value_(value), services_(services), timestamp_(timestamp),
    address_receiver_(address_receiver), address_sender_(address_sender),
    nonce_(nonce), user_agent_(user_agent), start_height_(start_height),
    relay_(relay)
{
}

version::version(uint32_t value, uint64_t services, uint64_t timestamp,
    network_address&& address_receiver, network_address&& address_sender,
    uint64_t nonce, std::string&& user_agent, uint32_t start_height,
    bool relay)
  : value_(value), services_(services), timestamp_(timestamp),
    address_receiver_(std::move(address_receiver)),
    address_sender_(std::move(address_sender)),
    nonce_(nonce), user_agent_(std::move(user_agent)),
    start_height_(start_height), relay_(relay)
{
}

version::version(const version& other)
  : version(other.value_, other.services_, other.timestamp_,
      other.address_receiver_, other.address_sender_, other.nonce_,
      other.user_agent_, other.start_height_, other.relay_)
{
}

version::version(version&& other)
  : version(other.value_, other.services_, other.timestamp_,
      std::move(other.address_receiver_),
      std::move(other.address_sender_), other.nonce_,
      std::move(other.user_agent_), other.start_height_,
      other.relay_)
{
}

bool version::is_valid() const
{
    return (value_ != 0)
        || (services_ != 0)
        || (timestamp_ != 0)
        || address_receiver_.is_valid()
        || address_sender_.is_valid()
        || (nonce_ != 0)
        || !user_agent_.empty()
        || (start_height_ != 0)
        || (relay_ != false);
}

void version::reset()
{
    value_ = 0;
    services_ = 0;
    timestamp_ = 0;
    address_receiver_.reset();
    address_sender_.reset();
    nonce_ = 0;
    user_agent_.clear();
    user_agent_.shrink_to_fit();
    start_height_ = 0;
    relay_ = false;
}

bool version::from_data(uint32_t version, const data_chunk& data)
{
    stream::in::copy istream(data);
    return from_data(version, istream);
}

bool version::from_data(uint32_t version, std::istream& stream)
{
    read::bytes::istream source(stream);
    return from_data(version, source);
}

bool version::from_data(uint32_t version, reader& source)
{
    reset();

    value_ = source.read_4_bytes_little_endian();
    services_ = source.read_8_bytes_little_endian();
    timestamp_ = source.read_8_bytes_little_endian();
    address_receiver_.from_data(version, source, false);
    address_sender_.from_data(version, source, false);
    nonce_ = source.read_8_bytes_little_endian();
    user_agent_ = source.read_string();
    start_height_ = source.read_4_bytes_little_endian();

    // HACK: disabled check due to inconsistent node implementation.
    // The protocol expects duplication of the sender's services.
    ////if (services_ != address_sender_.services())
    ////    source.invalidate();

    const auto peer_bip37 = (value_ >= level::bip37);
    const auto self_bip37 = (version >= level::bip37);

    // The relay field is optional at or above version 70001.
    // But the peer doesn't know our version when it sends its version.
    // This is a bug in the BIP37 design as it forces older peers to adapt to
    // the expansion of the version message, which is a clear compat break.
    // So relay is eabled if either peer is below 70001, it is not set, or
    // peers are at/above 70001 and the field is set.
    relay_ = (peer_bip37 != self_bip37) || source.is_exhausted() || 
        (self_bip37 && source.read_byte() != 0);

    if (!source)
        reset();

    return source;
}

data_chunk version::to_data(uint32_t version) const
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

void version::to_data(uint32_t version, std::ostream& stream) const
{
    write::bytes::ostream out(stream);
    to_data(version, out);
}

void version::to_data(uint32_t version, writer& sink) const
{
    sink.write_4_bytes_little_endian(value_);
    const auto effective_version = std::min(version, value_);
    sink.write_8_bytes_little_endian(services_);
    sink.write_8_bytes_little_endian(timestamp_);
    address_receiver_.to_data(version, sink, false);
    address_sender_.to_data(version, sink, false);
    sink.write_8_bytes_little_endian(nonce_);
    sink.write_string(user_agent_);
    sink.write_4_bytes_little_endian(start_height_);

    if (effective_version >= level::bip37)
        sink.write_byte(relay_ ? 1 : 0);
}

size_t version::serialized_size(uint32_t version) const
{
    auto size =
        sizeof(value_) +
        sizeof(services_) +
        sizeof(timestamp_) +
        address_receiver_.serialized_size(version, false) +
        address_sender_.serialized_size(version, false) +
        sizeof(nonce_) +
        variable_size(user_agent_.size()) + user_agent_.size() +
        sizeof(start_height_);

    if (value_ >= level::bip37)
        size += sizeof(uint8_t);

    return size;
}

uint32_t version::value() const
{
    return value_;
}

void version::set_value(uint32_t value)
{
    value_ = value;
}

uint64_t version::services() const
{
    return services_;
}

void version::set_services(uint64_t services)
{
    services_ = services;
}

uint64_t version::timestamp() const
{
    return timestamp_;
}

void version::set_timestamp(uint64_t timestamp)
{
    timestamp_ = timestamp;
}

network_address& version::address_receiver()
{
    return address_receiver_;
}

const network_address& version::address_receiver() const
{
    return address_receiver_;
}

void version::set_address_receiver(network_address&& address)
{
    address_receiver_ = std::move(address);
}

network_address& version::address_sender()
{
    return address_sender_;
}

const network_address& version::address_sender() const
{
    return address_sender_;
}

void version::set_address_sender(network_address&& address)
{
    address_sender_ = std::move(address);
}

uint64_t version::nonce() const
{
    return nonce_;
}

void version::set_nonce(uint64_t nonce)
{
    nonce_ = nonce;
}

std::string& version::user_agent()
{
    return user_agent_;
}

const std::string& version::user_agent() const
{
    return user_agent_;
}

void version::set_user_agent(const std::string& agent)
{
    user_agent_ = agent;
}

void version::set_user_agent(std::string&& agent)
{
    user_agent_ = std::move(agent);
}

uint32_t version::start_height() const
{
    return start_height_;
}

void version::set_start_height(uint32_t height)
{
    start_height_ = height;
}

bool version::relay() const
{
    return relay_;
}

void version::set_relay(bool relay)
{
    relay_ = relay;
}

version& version::operator=(version&& other)
{
    value_ = other.value_;
    services_ = other.services_;
    timestamp_ = other.timestamp_;
    address_receiver_ = std::move(other.address_receiver_);
    address_sender_ = std::move(other.address_sender_);
    nonce_ = other.nonce_;
    user_agent_ = other.user_agent_;
    start_height_ = other.start_height_;
    relay_ = other.relay_;
    return *this;
}

bool version::operator==(const version& other) const
{
    return (value_ == other.value_) && (services_ == other.services_) &&
        (timestamp_ == other.timestamp_) &&
        (address_receiver_ == other.address_receiver_) &&
        (address_sender_ == other.address_sender_) &&
        (nonce_ == other.nonce_) && (user_agent_ == other.user_agent_) &&
        (start_height_ == other.start_height_) && (relay_ == other.relay_);
}

bool version::operator!=(const version& other) const
{
    return !(*this == other);
}

} // namespace message
} // namespace system
} // namespace libbitcoin
