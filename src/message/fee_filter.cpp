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
#include <bitcoin/system/message/fee_filter.hpp>

#include <bitcoin/system/assert.hpp>
#include <bitcoin/system/message/version.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

const std::string fee_filter::command = "feefilter";
const uint32_t fee_filter::version_minimum = version::level::bip133;
const uint32_t fee_filter::version_maximum = version::level::bip133;

fee_filter fee_filter::factory(uint32_t version,
    const data_chunk& data)
{
    fee_filter instance;
    instance.from_data(version, data);
    return instance;
}

fee_filter fee_filter::factory(uint32_t version,
    std::istream& stream)
{
    fee_filter instance;
    instance.from_data(version, stream);
    return instance;
}

fee_filter fee_filter::factory(uint32_t version, reader& source)
{
    fee_filter instance;
    instance.from_data(version, source);
    return instance;
}

size_t fee_filter::satoshi_fixed_size(uint32_t)
{
    return sizeof(minimum_fee_);
}

// This is a default instance so is invalid.
fee_filter::fee_filter()
  : minimum_fee_(0), insufficient_version_(true)
{
}

// This is not a default instance so is valid.
fee_filter::fee_filter(uint64_t minimum)
  : minimum_fee_(minimum), insufficient_version_(false)
{
}

// protected
fee_filter::fee_filter(uint64_t minimum, bool insufficient_version)
  : minimum_fee_(minimum), insufficient_version_(insufficient_version)
{
}

fee_filter::fee_filter(const fee_filter& other)
  : fee_filter(other.minimum_fee_, other.insufficient_version_)
{
}

fee_filter::fee_filter(fee_filter&& other)
  : fee_filter(other.minimum_fee_, other.insufficient_version_)
{
}

bool fee_filter::from_data(uint32_t version, const data_chunk& data)
{
    stream::in::copy istream(data);
    return from_data(version, istream);
}

bool fee_filter::from_data(uint32_t version, std::istream& stream)
{
    read::bytes::istream source(stream);
    return from_data(version, source);
}

bool fee_filter::from_data(uint32_t version, reader& source)
{
    reset();

    // Initialize as valid from deserialization.
    insufficient_version_ = false;

    minimum_fee_ = source.read_8_bytes_little_endian();

    if (version < fee_filter::version_minimum)
        source.invalidate();

    if (!source)
        reset();

    return source;
}

data_chunk fee_filter::to_data(uint32_t version) const
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

void fee_filter::to_data(uint32_t version, std::ostream& stream) const
{
    write::bytes::ostream out(stream);
    to_data(version, out);
}

void fee_filter::to_data(uint32_t, writer& sink) const
{
    sink.write_8_bytes_little_endian(minimum_fee_);
}

bool fee_filter::is_valid() const
{
    return !insufficient_version_ || (minimum_fee_ > 0);
}

// This is again a default instance so is invalid.
void fee_filter::reset()
{
    insufficient_version_ = true;
    minimum_fee_ = 0;
}

size_t fee_filter::serialized_size(uint32_t version) const
{
    return satoshi_fixed_size(version);
}

uint64_t fee_filter::minimum_fee() const
{
    return minimum_fee_;
}

void fee_filter::set_minimum_fee(uint64_t value)
{
    minimum_fee_ = value;

    // This is no longer a default instance, so is valid.
    insufficient_version_ = false;
}

fee_filter& fee_filter::operator=(fee_filter&& other)
{
    minimum_fee_ = other.minimum_fee_;
    insufficient_version_ = other.insufficient_version_;
    return *this;
}

bool fee_filter::operator==(const fee_filter& other) const
{
    return (minimum_fee_ == other.minimum_fee_);
}

bool fee_filter::operator!=(const fee_filter& other) const
{
    return !(*this == other);
}

} // namespace message
} // namespace system
} // namespace libbitcoin
