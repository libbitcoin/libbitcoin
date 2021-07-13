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
#include <bitcoin/system/chain/stealth_record.hpp>

#include <cstddef>
#include <cstdint>
#include <istream>
#include <utility>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace chain {

// The sign byte of the ephemeral key is fixed (0x02) by convention.
static const auto sign = to_array(ec_even_sign);

// Constructors.
//-----------------------------------------------------------------------------

stealth_record::stealth_record()
  : height_(0), prefix_(0),
    unsigned_ephemeral_(null_hash),
    public_key_hash_(null_short_hash),
    transaction_hash_(null_hash)
{
}

stealth_record::stealth_record(chain::stealth_record&& other)
  : height_(0), prefix_(0),
    unsigned_ephemeral_(std::move(other.unsigned_ephemeral_)),
    public_key_hash_(std::move(other.public_key_hash_)),
    transaction_hash_(std::move(other.transaction_hash_))
{
}

stealth_record::stealth_record(const chain::stealth_record& other)
  : height_(other.height_), prefix_(other.prefix_),
    unsigned_ephemeral_(other.unsigned_ephemeral_),
    public_key_hash_(other.public_key_hash_),
    transaction_hash_(other.transaction_hash_)
{
}

stealth_record::stealth_record(size_t height, uint32_t prefix,
    hash_digest&& ephemeral_public_key, short_hash&& public_key_hash,
    hash_digest&& transaction_hash)
  : height_(static_cast<uint32_t>(height)),
    prefix_(prefix),
    unsigned_ephemeral_(std::move(ephemeral_public_key)),
    public_key_hash_(std::move(public_key_hash)),
    transaction_hash_(std::move(transaction_hash))
{
    // This class is used internal to the store, parameters presumed valid.
    BITCOIN_ASSERT(height <= max_uint32);
}

stealth_record::stealth_record(size_t height, uint32_t prefix,
    const hash_digest& ephemeral_public_key,
    const short_hash& public_key_hash, const hash_digest& transaction_hash)
  : height_(static_cast<uint32_t>(height)),
    prefix_(prefix),
    unsigned_ephemeral_(ephemeral_public_key),
    public_key_hash_(public_key_hash),
    transaction_hash_(transaction_hash)
{
    // This class is used internal to the store, parameters presumed valid.
    BITCOIN_ASSERT(height <= max_uint32);
}

stealth_record::stealth_record(size_t height, uint32_t prefix,
    const ec_compressed& ephemeral_public_key, short_hash&& public_key_hash,
    hash_digest&& transaction_hash)
  : height_(static_cast<uint32_t>(height)),
    prefix_(prefix),
    unsigned_ephemeral_(slice<sizeof(ec_even_sign),
        ec_compressed_size>(ephemeral_public_key)),
    public_key_hash_(std::move(public_key_hash)),
    transaction_hash_(std::move(transaction_hash))
{
    // This class is used internal to the store, parameters presumed valid.
    BITCOIN_ASSERT(height <= max_uint32);
}

stealth_record::stealth_record(size_t height, uint32_t prefix,
    const ec_compressed& ephemeral_public_key,
    const short_hash& public_key_hash, const hash_digest& transaction_hash)
  : height_(static_cast<uint32_t>(height)),
    prefix_(prefix),
    unsigned_ephemeral_(slice<sizeof(ec_even_sign),
        ec_compressed_size>(ephemeral_public_key)),
    public_key_hash_(public_key_hash),
    transaction_hash_(transaction_hash)
{
    // This class is used internal to the store, parameters presumed valid.
    BITCOIN_ASSERT(height <= max_uint32);
}

// Operators.
//-----------------------------------------------------------------------------

stealth_record& stealth_record::operator=(stealth_record&& other)
{
    height_ = other.height_;
    prefix_ = other.prefix_;
    unsigned_ephemeral_ = std::move(other.unsigned_ephemeral_);
    public_key_hash_ = std::move(other.public_key_hash_);
    transaction_hash_ = std::move(other.transaction_hash_);
    return *this;
}

stealth_record& stealth_record::operator=(const stealth_record& other)
{
    height_ = other.height_;
    prefix_ = other.prefix_;
    unsigned_ephemeral_ = other.unsigned_ephemeral_;
    public_key_hash_ = other.public_key_hash_;
    transaction_hash_ = other.transaction_hash_;
    return *this;
}

bool stealth_record::operator==(const stealth_record& other) const
{
    return (height_ == other.height_)
        && (prefix_ == other.prefix_)
        && (unsigned_ephemeral_ == other.unsigned_ephemeral_)
        && (public_key_hash_ == other.public_key_hash_)
        && (transaction_hash_ == other.transaction_hash_);
}

bool stealth_record::operator!=(const stealth_record& other) const
{
    return !(*this == other);
}

// Deserialization.
//-----------------------------------------------------------------------------

stealth_record stealth_record::factory(const data_chunk& data, bool wire)
{
    stealth_record instance;
    instance.from_data(data, wire);
    return instance;
}

stealth_record stealth_record::factory(std::istream& stream, bool wire)
{
    stealth_record instance;
    instance.from_data(stream, wire);
    return instance;
}

stealth_record stealth_record::factory(reader& source, bool wire)
{
    stealth_record instance;
    instance.from_data(source, wire);
    return instance;
}

bool stealth_record::from_data(const data_chunk& data, bool wire)
{
    stream::in::copy istream(data);
    return from_data(istream, wire);
}

bool stealth_record::from_data(std::istream& stream, bool wire)
{
    read::bytes::istream source(stream);
    return from_data(source, wire);
}

bool stealth_record::from_data(reader& source, bool wire)
{
    if (!wire)
        return from_data(source, 0, {});

    reset();
    unsigned_ephemeral_ = source.read_hash();
    public_key_hash_ = source.read_short_hash();
    transaction_hash_ = source.read_hash();

    if (!source)
        reset();

    return source;
}

// Optimize reads by short-circuiting what is unnecessary.
// Invalid returns are conflated with skipped, but are store only.
bool stealth_record::from_data(reader& source, size_t start_height,
    const binary& filter)
{
    height_ = source.read_4_bytes_little_endian();

    if (height_ < start_height)
    {
        reset();
        source.skip_bytes(serialized_size(false) - sizeof(uint32_t));
        return false;
    }

    const auto prefix = source.read_bytes(sizeof(uint32_t));
    prefix_ = from_little_endian<uint32_t>(prefix);

    if (filter != binary(filter.bits(), prefix))
    {
        reset();
        source.skip_bytes(serialized_size(false) - 2 * sizeof(uint32_t));
        return false;
    }

    unsigned_ephemeral_ = source.read_hash();
    public_key_hash_ = source.read_short_hash();
    transaction_hash_ = source.read_hash();

    if (!source)
        reset();

    return source;
}

// protected
void stealth_record::reset()
{
    height_ = 0;
    prefix_ = 0;
    unsigned_ephemeral_ = null_hash;
    public_key_hash_ = null_short_hash;
    transaction_hash_ = null_hash;
}

bool stealth_record::is_valid() const
{
    return (unsigned_ephemeral_ != null_hash)
        && (public_key_hash_ != null_short_hash)
        && (transaction_hash_ != null_hash);
}

// Serialization.
//-----------------------------------------------------------------------------

data_chunk stealth_record::to_data(bool wire) const
{
    data_chunk data;
    const auto size = serialized_size(wire);
    data.reserve(size);
    stream::out::data ostream(data);
    to_data(ostream, wire);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void stealth_record::to_data(std::ostream& stream, bool wire) const
{
    write::bytes::ostream out(stream);
    to_data(out, wire);
}

void stealth_record::to_data(writer& sink, bool wire) const
{
    if (!wire)
    {
        // Store serialization changed/optimized in v4.
        sink.write_4_bytes_little_endian(height_);
        sink.write_4_bytes_little_endian(prefix_);
    }

    sink.write_bytes(unsigned_ephemeral_);
    sink.write_bytes(public_key_hash_);
    sink.write_bytes(transaction_hash_);
}

// Properties (size, accessors).
//-----------------------------------------------------------------------------

size_t stealth_record::satoshi_fixed_size(bool wire)
{
    // Wire protocol excludes height and prefix.
    return (wire ? 0 : sizeof(uint32_t) + sizeof(uint32_t)) +
        hash_size + short_hash_size + hash_size;
}

size_t stealth_record::serialized_size(bool wire) const
{
    return satoshi_fixed_size(wire);
}

size_t stealth_record::height() const
{
    return height_;
}

uint32_t stealth_record::prefix() const
{
    return prefix_;
}

// Restore the default sign byte to the ephemeral key as a convenience.
ec_compressed stealth_record::ephemeral_public_key() const
{
    return splice(sign, unsigned_ephemeral_);
}

// Elimination of the sign by defaulting its value allows the key to be stored
// and published as a 32 byte value. This is more optimal and private than key.
const hash_digest& stealth_record::unsigned_ephemeral_public_key() const
{
    return unsigned_ephemeral_;
}

const short_hash& stealth_record::public_key_hash() const
{
    return public_key_hash_;
}

const hash_digest& stealth_record::transaction_hash() const
{
    return transaction_hash_;
}

} // namespace chain
} // namespace system
} // namespace libbitcoin
