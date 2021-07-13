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
#include <bitcoin/system/chain/header.hpp>

#include <chrono>
#include <cstddef>
#include <utility>
#include <boost/thread.hpp>
#include <bitcoin/system/chain/chain_state.hpp>
#include <bitcoin/system/chain/compact.hpp>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/error.hpp>
#include <bitcoin/system/mutex.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace chain {

// Use system clock because we require accurate time of day.
using wall_clock = std::chrono::system_clock;

// Constructors.
//-----------------------------------------------------------------------------

header::header()
  : header(0, null_hash, null_hash, 0, 0, 0)
{
}

header::header(header&& other)
  : hash_(other.hash_cache()),
    version_(other.version_),
    previous_block_hash_(std::move(other.previous_block_hash_)),
    merkle_root_(std::move(other.merkle_root_)),
    timestamp_(other.timestamp_),
    bits_(other.bits_),
    nonce_(other.nonce_),
    metadata(std::move(other.metadata))
{
}

header::header(const header& other)
  : hash_(other.hash_cache()),
    version_(other.version_),
    previous_block_hash_(other.previous_block_hash_),
    merkle_root_(other.merkle_root_),
    timestamp_(other.timestamp_),
    bits_(other.bits_),
    nonce_(other.nonce_),
    metadata(other.metadata)
{
}

header::header(uint32_t version, hash_digest&& previous_block_hash,
    hash_digest&& merkle_root, uint32_t timestamp, uint32_t bits, uint32_t nonce)
  : version_(version),
    previous_block_hash_(std::move(previous_block_hash)),
    merkle_root_(std::move(merkle_root)),
    timestamp_(timestamp),
    bits_(bits),
    nonce_(nonce),
    metadata{}
{
}

header::header(uint32_t version, const hash_digest& previous_block_hash,
    const hash_digest& merkle_root, uint32_t timestamp, uint32_t bits,
    uint32_t nonce)
  : version_(version),
    previous_block_hash_(previous_block_hash),
    merkle_root_(merkle_root),
    timestamp_(timestamp),
    bits_(bits),
    nonce_(nonce),
    metadata{}
{
}

// Private cache access for copy/move construction.
header::hash_ptr header::hash_cache() const
{
    shared_lock lock(mutex_);
    return hash_;
}

// Operators.
//-----------------------------------------------------------------------------

header& header::operator=(header&& other)
{
    hash_ = other.hash_cache();
    version_ = other.version_;
    previous_block_hash_ = std::move(other.previous_block_hash_);
    merkle_root_ = std::move(other.merkle_root_);
    timestamp_ = other.timestamp_;
    bits_ = other.bits_;
    nonce_ = other.nonce_;
    metadata = std::move(other.metadata);
    return *this;
}

header& header::operator=(const header& other)
{
    hash_ = other.hash_cache();
    version_ = other.version_;
    previous_block_hash_ = other.previous_block_hash_;
    merkle_root_ = other.merkle_root_;
    timestamp_ = other.timestamp_;
    bits_ = other.bits_;
    nonce_ = other.nonce_;
    metadata = other.metadata;
    return *this;
}

bool header::operator==(const header& other) const
{
    return (version_ == other.version_)
        && (previous_block_hash_ == other.previous_block_hash_)
        && (merkle_root_ == other.merkle_root_)
        && (timestamp_ == other.timestamp_)
        && (bits_ == other.bits_)
        && (nonce_ == other.nonce_);
}

bool header::operator!=(const header& other) const
{
    return !(*this == other);
}

// Deserialization.
//-----------------------------------------------------------------------------

// static
header header::factory(const data_chunk& data, bool wire)
{
    header instance;
    instance.from_data(data, wire);
    return instance;
}

// static
header header::factory(std::istream& stream, bool wire)
{
    header instance;
    instance.from_data(stream, wire);
    return instance;
}

// static
header header::factory(reader& source, bool wire)
{
    header instance;
    instance.from_data(source, wire);
    return instance;
}

// static
header header::factory(reader& source, hash_digest&& hash, bool wire)
{
    header instance;
    instance.from_data(source, std::move(hash), wire);
    return instance;
}

// static
header header::factory(reader& source, const hash_digest& hash, bool wire)
{
    header instance;
    instance.from_data(source, hash, wire);
    return instance;
}

bool header::from_data(const data_chunk& data, bool wire)
{
    read::bytes::copy reader(data);
    return from_data(reader, wire);
}

bool header::from_data(std::istream& stream, bool wire)
{
    read::bytes::istream reader(stream);
    return from_data(reader, wire);
}

bool header::from_data(reader& source, bool)
{
    ////reset();

    version_ = source.read_4_bytes_little_endian();
    previous_block_hash_ = source.read_hash();
    merkle_root_ = source.read_hash();
    timestamp_ = source.read_4_bytes_little_endian();
    bits_ = source.read_4_bytes_little_endian();
    nonce_ = source.read_4_bytes_little_endian();

    if (!source)
        reset();

    return source;
}

bool header::from_data(reader& source, hash_digest&& hash, bool wire)
{
    if (!from_data(source, wire))
        return false;

    hash_ = std::make_shared<hash_digest>(std::move(hash));
    return true;
}

bool header::from_data(reader& source, const hash_digest& hash, bool wire)
{
    if (!from_data(source, wire))
        return false;

    hash_ = std::make_shared<hash_digest>(hash);
    return true;
}

// protected
void header::reset()
{
    version_ = 0;
    previous_block_hash_.fill(0);
    merkle_root_.fill(0);
    timestamp_ = 0;
    bits_ = 0;
    nonce_ = 0;
    invalidate_cache();
}

bool header::is_valid() const
{
    return (version_ != 0) ||
        (previous_block_hash_ != null_hash) ||
        (merkle_root_ != null_hash) ||
        (timestamp_ != 0) ||
        (bits_ != 0) ||
        (nonce_ != 0);
}

// Serialization.
//-----------------------------------------------------------------------------

data_chunk header::to_data(bool wire) const
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

void header::to_data(std::ostream& stream, bool wire) const
{
    write::bytes::ostream out(stream);
    to_data(out, wire);
}

void header::to_data(writer& sink, bool) const
{
    sink.write_4_bytes_little_endian(version_);
    sink.write_bytes(previous_block_hash_);
    sink.write_bytes(merkle_root_);
    sink.write_4_bytes_little_endian(timestamp_);
    sink.write_4_bytes_little_endian(bits_);
    sink.write_4_bytes_little_endian(nonce_);
}

// Size.
//-----------------------------------------------------------------------------

// static
size_t header::satoshi_fixed_size()
{
    return sizeof(version_)
        + hash_size
        + hash_size
        + sizeof(timestamp_)
        + sizeof(bits_)
        + sizeof(nonce_);
}

size_t header::serialized_size(bool) const
{
    return satoshi_fixed_size();
}

// Accessors.
//-----------------------------------------------------------------------------

uint32_t header::version() const
{
    return version_;
}

void header::set_version(uint32_t value)
{
    version_ = value;
    invalidate_cache();
}

const hash_digest& header::previous_block_hash() const
{
    return previous_block_hash_;
}

void header::set_previous_block_hash(const hash_digest& value)
{
    previous_block_hash_ = value;
    invalidate_cache();
}

void header::set_previous_block_hash(hash_digest&& value)
{
    previous_block_hash_ = std::move(value);
    invalidate_cache();
}

const hash_digest& header::merkle_root() const
{
    return merkle_root_;
}

void header::set_merkle_root(const hash_digest& value)
{
    merkle_root_ = value;
    invalidate_cache();
}

void header::set_merkle_root(hash_digest&& value)
{
    merkle_root_ = std::move(value);
    invalidate_cache();
}

uint32_t header::timestamp() const
{
    return timestamp_;
}

void header::set_timestamp(uint32_t value)
{
    timestamp_ = value;
    invalidate_cache();
}

uint32_t header::bits() const
{
    return bits_;
}

void header::set_bits(uint32_t value)
{
    bits_ = value;
    invalidate_cache();
}

uint32_t header::nonce() const
{
    return nonce_;
}

void header::set_nonce(uint32_t value)
{
    nonce_ = value;
    invalidate_cache();
}

// Cache.
//-----------------------------------------------------------------------------

// protected
void header::invalidate_cache() const
{
    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    mutex_.lock_upgrade();

    if (hash_)
    {
        mutex_.unlock_upgrade_and_lock();
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        hash_.reset();
        //---------------------------------------------------------------------
        mutex_.unlock_and_lock_upgrade();
    }

    mutex_.unlock_upgrade();
    ///////////////////////////////////////////////////////////////////////////
}

hash_digest header::hash() const
{
    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    mutex_.lock_upgrade();

    if (!hash_)
    {
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        mutex_.unlock_upgrade_and_lock();
        hash_ = std::make_shared<hash_digest>(bitcoin_hash(to_data()));
        mutex_.unlock_and_lock_upgrade();
        //---------------------------------------------------------------------
    }

    const auto hash = *hash_;
    mutex_.unlock_upgrade();
    ///////////////////////////////////////////////////////////////////////////

    return hash;
}

// Validation helpers.
//-----------------------------------------------------------------------------

/// BUGBUG: bitcoin 32bit unix time: en.wikipedia.org/wiki/Year_2038_problem
bool header::is_valid_timestamp(uint32_t timestamp_limit_seconds) const
{
    using namespace std::chrono;
    static const auto two_hours = seconds(timestamp_limit_seconds);
    const auto time = wall_clock::from_time_t(timestamp_);
    const auto future = wall_clock::now() + two_hours;
    return time <= future;
}

bool header::is_valid_proof_of_work(uint32_t proof_of_work_limit,
    bool scrypt) const
{
    const auto bits = compact(bits_);
    static const uint256_t pow_limit(compact{ proof_of_work_limit });

    if (bits.is_overflowed())
        return false;

    uint256_t target(bits);

    // Ensure claimed work is within limits.
    if (target < 1 || target > pow_limit)
        return false;

    // Conditionally use scrypt proof of work (e.g. Litecoin).
    // Ensure actual work is at least claimed amount (smaller is more work).
    return to_uint256(scrypt ? scrypt_hash(to_data()) : hash()) <= target;
}

// static
uint256_t header::proof(uint32_t bits)
{
    const auto header_bits = compact(bits);

    if (header_bits.is_overflowed())
        return 0;

    uint256_t target(header_bits);

    //*************************************************************************
    // CONSENSUS: satoshi will throw division by zero in the case where the
    // target is (2^256)-1 as the overflow will result in a zero divisor.
    // While actually achieving this work is improbable, this method operates
    // on a public method and therefore must be guarded.
    //*************************************************************************
    const auto divisor = target + 1;

    // We need to compute 2**256 / (target + 1), but we can't represent 2**256
    // as it's too large for uint256. However as 2**256 is at least as large as
    // target + 1, it is equal to ((2**256 - target - 1) / (target + 1)) + 1, or
    // (~target / (target + 1)) + 1.
    return (divisor == 0) ? 0 : (~target / divisor) + 1;
}

uint256_t header::proof() const
{
    return proof(bits_);
}


// Validation.
//-----------------------------------------------------------------------------

code header::check(uint32_t timestamp_limit_seconds,
    uint32_t proof_of_work_limit, bool scrypt) const
{

    if (!is_valid_proof_of_work(proof_of_work_limit, scrypt))
        return error::invalid_proof_of_work;

    else if (!is_valid_timestamp(timestamp_limit_seconds))
        return error::futuristic_timestamp;

    else
        return error::success;
}

code header::accept() const
{
    const auto state = metadata.state;
    return state ? accept(*state) : error::operation_failed;
}

code header::accept(const chain_state& state) const
{
    if (bits_ != state.work_required())
        return error::incorrect_proof_of_work;

    else if (state.is_checkpoint_conflict(hash()))
        return error::checkpoints_failed;

    else if (state.is_under_checkpoint())
        return error::success;

    else if (version_ < state.minimum_block_version())
        return error::invalid_block_version;

    else if (timestamp_ <= state.median_time_past())
        return error::timestamp_too_early;

    else
        return error::success;
}

} // namespace chain
} // namespace system
} // namespace libbitcoin
