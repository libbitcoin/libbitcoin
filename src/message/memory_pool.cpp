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
#include <bitcoin/system/message/memory_pool.hpp>

#include <bitcoin/system/assert.hpp>
#include <bitcoin/system/message/version.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

const std::string memory_pool::command = "mempool";
const uint32_t memory_pool::version_minimum = version::level::bip35;
const uint32_t memory_pool::version_maximum = version::level::maximum;

memory_pool memory_pool::factory(uint32_t version,
    const data_chunk& data)
{
    memory_pool instance;
    instance.from_data(version, data);
    return instance;
}

memory_pool memory_pool::factory(uint32_t version,
    std::istream& stream)
{
    memory_pool instance;
    instance.from_data(version, stream);
    return instance;
}

memory_pool memory_pool::factory(uint32_t version,
    reader& source)
{
    memory_pool instance;
    instance.from_data(version, source);
    return instance;
}

// This is a default instance so is invalid.
// The only way to make this valid is to deserialize it :/.
memory_pool::memory_pool()
  : insufficient_version_(true)
{
}

// protected
memory_pool::memory_pool(bool insufficient_version)
  : insufficient_version_(insufficient_version)
{
}

memory_pool::memory_pool(const memory_pool& other)
  : memory_pool(other.insufficient_version_)
{
}

memory_pool::memory_pool(memory_pool&& other)
  : memory_pool(other.insufficient_version_)
{
}

bool memory_pool::is_valid() const
{
    return !insufficient_version_;
}

// This is again a default instance so is invalid.
void memory_pool::reset()
{
    insufficient_version_ = true;
}

bool memory_pool::from_data(uint32_t version, const data_chunk& data)
{
    stream::in::copy istream(data);
    return from_data(version, istream);
}

bool memory_pool::from_data(uint32_t version, std::istream& stream)
{
    read::bytes::istream source(stream);
    return from_data(version, source);
}

bool memory_pool::from_data(uint32_t version, reader& source)
{
    reset();

    // Initialize as valid from deserialization.
    insufficient_version_ = false;

    if (version < memory_pool::version_minimum)
        source.invalidate();

    if (!source)
        reset();

    return source;
}

data_chunk memory_pool::to_data(uint32_t version) const
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

void memory_pool::to_data(uint32_t version, std::ostream& stream) const
{
    write::bytes::ostream out(stream);
    to_data(version, out);
}

void memory_pool::to_data(uint32_t, writer&) const
{
}

size_t memory_pool::serialized_size(uint32_t version) const
{
    return memory_pool::satoshi_fixed_size(version);
}

size_t memory_pool::satoshi_fixed_size(uint32_t)
{
    return 0;
}

} // namespace message
} // namespace system
} // namespace libbitcoin
