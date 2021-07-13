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
#include <bitcoin/system/chain/transaction.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <numeric>
#include <type_traits>
#include <utility>
#include <vector>
#include <boost/optional.hpp>
#include <bitcoin/system/chain/chain_state.hpp>
#include <bitcoin/system/chain/enums/magic_numbers.hpp>
#include <bitcoin/system/chain/header.hpp>
#include <bitcoin/system/chain/input.hpp>
#include <bitcoin/system/chain/output.hpp>
#include <bitcoin/system/chain/script.hpp>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/error.hpp>
#include <bitcoin/system/math/math.hpp>
#include <bitcoin/system/mutex.hpp>
#include <bitcoin/system/optional.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace chain {

#define RETURN_CACHED(name, type, context) \
    hash_mutex_.lock_upgrade(); \
    if (!name##_##context##_) \
    { \
        hash_mutex_.unlock_upgrade_and_lock(); \
        name##_##context##_ = std::make_shared<hash_digest>(type##_hash( \
            script::to_##name(*this))); \
        hash_mutex_.unlock_and_lock_upgrade(); \
    } \
    const auto hash = *name##_##context##_; \
    hash_mutex_.unlock_upgrade(); \
    return hash

// HACK: unlinked must match tx slab_map::not_found.
const uint64_t transaction::validation::unlinked = max_int64;

// Read a length-prefixed collection of inputs or outputs from the source.
template<class Source, class Put>
bool read(Source& source, std::vector<Put>& puts, bool wire, bool witness)
{
    auto result = true;
    const auto count = source.read_size();

    // Guard against potential for arbitrary memory allocation.
    if (count > max_block_size)
        source.invalidate();
    else
        puts.resize(count);

    const auto deserialize = [&](Put& put)
    {
        result = result && put.from_data(source, wire, witness);
#ifndef NDEBUG
        put.script().operations();
#endif
    };

    std::for_each(puts.begin(), puts.end(), deserialize);
    return result;
}

// Write a length-prefixed collection of inputs or outputs to the sink.
template<class Sink, class Put>
void write(Sink& sink, const std::vector<Put>& puts, bool wire, bool witness)
{
    sink.write_variable(puts.size());

    const auto serialize = [&](const Put& put)
    {
        put.to_data(sink, wire, witness);
    };

    std::for_each(puts.begin(), puts.end(), serialize);
}

// Input list must be pre-populated as it determines witness count.
inline void read_witnesses(reader& source, input::list& inputs)
{
    const auto deserialize = [&](input& input)
    {
        input.set_witness(witness::factory(source, true));
    };

    std::for_each(inputs.begin(), inputs.end(), deserialize);
}

// Witness count is not written as it is inferred from input count.
inline void write_witnesses(writer& sink, const input::list& inputs)
{
    const auto serialize = [&sink](const input& input)
    {
        input.witness().to_data(sink, true);
    };

    std::for_each(inputs.begin(), inputs.end(), serialize);
}

// Constructors.
//-----------------------------------------------------------------------------

transaction::transaction()
  : metadata{},
    version_(0),
    locktime_(0),
    inputs_{},
    outputs_{}
{
}

transaction::transaction(transaction&& other)
  : metadata(std::move(other.metadata)),
    version_(other.version_),
    locktime_(other.locktime_),
    inputs_(std::move(other.inputs_)),
    outputs_(std::move(other.outputs_)),
    hash_(other.hash_cache()),
    total_input_value_(other.total_input_value_cache()),
    total_output_value_(other.total_output_value_cache())
{
}

transaction::transaction(const transaction& other)
  : metadata(other.metadata),
    version_(other.version_),
    locktime_(other.locktime_),
    inputs_(other.inputs_),
    outputs_(other.outputs_),
    hash_(other.hash_cache()),
    total_input_value_(other.total_input_value_cache()),
    total_output_value_(other.total_output_value_cache())
{
}

transaction::transaction(uint32_t version, uint32_t locktime,
    input::list&& inputs, output::list&& outputs)
  : metadata{},
    version_(version),
    locktime_(locktime),
    inputs_(std::move(inputs)),
    outputs_(std::move(outputs))
{
}

transaction::transaction(uint32_t version, uint32_t locktime,
    const input::list& inputs, const output::list& outputs)
  : metadata{},
    version_(version),
    locktime_(locktime),
    inputs_(inputs),
    outputs_(outputs)
{
}

// Private cache access for copy/move construction.
transaction::hash_ptr transaction::hash_cache() const
{
    shared_lock lock(value_mutex_);
    return hash_;
}

// Private cache access for copy/move construction.
optional64 transaction::total_input_value_cache() const
{
    shared_lock lock(value_mutex_);
    return total_input_value_;
}

// Private cache access for copy/move construction.
optional64 transaction::total_output_value_cache() const
{
    shared_lock lock(value_mutex_);
    return total_output_value_;
}

// Operators.
//-----------------------------------------------------------------------------

transaction& transaction::operator=(transaction&& other)
{
    hash_ = other.hash_cache();
    total_input_value_ = other.total_input_value_cache();
    total_output_value_ = other.total_output_value_cache();
    version_ = other.version_;
    locktime_ = other.locktime_;
    inputs_ = std::move(other.inputs_);
    outputs_ = std::move(other.outputs_);
    metadata = std::move(other.metadata);
    return *this;
}

// This can be expensive, try to avoid.
transaction& transaction::operator=(const transaction& other)
{
    hash_ = other.hash_cache();
    total_input_value_ = other.total_input_value_cache();
    total_output_value_ = other.total_output_value_cache();
    version_ = other.version_;
    locktime_ = other.locktime_;
    inputs_ = other.inputs_;
    outputs_ = other.outputs_;
    metadata = other.metadata;
    return *this;
}

bool transaction::operator==(const transaction& other) const
{
    return (version_ == other.version_)
        && (locktime_ == other.locktime_)
        && (inputs_ == other.inputs_)
        && (outputs_ == other.outputs_);
}

bool transaction::operator!=(const transaction& other) const
{
    return !(*this == other);
}

// Deserialization.
//-----------------------------------------------------------------------------

// static
transaction transaction::factory(const data_chunk& data, bool wire,
    bool witness)
{
    transaction instance;
    instance.from_data(data, wire, witness);
    return instance;
}

// static
transaction transaction::factory(std::istream& stream, bool wire, bool witness)
{

    transaction instance;
    instance.from_data(stream, wire, witness);
    return instance;
}

// static
transaction transaction::factory(reader& source, bool wire, bool witness)
{
    transaction instance;
    instance.from_data(source, wire, witness);
    return instance;
}

// static
transaction transaction::factory(reader& source, hash_digest&& hash, bool wire,
    bool witness)
{
    transaction instance;
    instance.from_data(source, std::move(hash), wire, witness);
    return instance;
}

// static
transaction transaction::factory(reader& source, const hash_digest& hash,
    bool wire, bool witness)
{
    transaction instance;
    instance.from_data(source, hash, wire, witness);
    return instance;
}

bool transaction::from_data(const data_chunk& data, bool wire, bool witness)
{
    stream::in::copy istream(data);
    return from_data(istream, wire, witness);
}

bool transaction::from_data(std::istream& stream, bool wire, bool witness)
{
    read::bytes::istream source(stream);
    return from_data(source, wire, witness);
}

// Witness is not used by outputs, just for template normalization.
bool transaction::from_data(reader& source, bool wire, bool witness)
{
    reset();

    if (wire)
    {
        // Wire (satoshi protocol) deserialization.
        version_ = source.read_4_bytes_little_endian();
        read(source, inputs_, wire, witness);

        // Detect witness as no inputs (marker) and expected flag (bip144).
        const auto marker = inputs_.size() == witness_marker &&
            source.peek_byte() == witness_flag;

        // This is always enabled so caller should validate with is_segregated.
        if (marker)
        {
            // Skip over the peeked witness flag.
            source.skip_byte();
            read(source, inputs_, wire, witness);
            read(source, outputs_, wire, witness);
            read_witnesses(source, inputs_);
        }
        else
        {
            read(source, outputs_, wire, witness);
        }

        locktime_ = source.read_4_bytes_little_endian();
    }
    else
    {
        // Database (outputs forward) serialization.
        // Witness data is managed internal to inputs.
        read(source, outputs_, wire, witness);
        read(source, inputs_, wire, witness);
        const auto locktime = source.read_variable();
        const auto version = source.read_variable();

        if (locktime > max_uint32 || version > max_uint32)
            source.invalidate();

        locktime_ = static_cast<uint32_t>(locktime);
        version_ = static_cast<uint32_t>(version);
    }

    // TODO: optimize by having reader skip witness data.
    if (!witness)
        strip_witness();

    if (!source)
        reset();

    return source;
}

bool transaction::from_data(reader& source, hash_digest&& hash, bool wire,
    bool witness)
{
    if (!from_data(source, wire, witness))
        return false;

    hash_ = std::make_shared<hash_digest>(std::move(hash));
    return true;
}

bool transaction::from_data(reader& source, const hash_digest& hash, bool wire,
    bool witness)
{
    if (!from_data(source, wire, witness))
        return false;

    hash_ = std::make_shared<hash_digest>(hash);
    return true;
}

// protected
void transaction::reset()
{
    version_ = 0;
    locktime_ = 0;
    inputs_.clear();
    inputs_.shrink_to_fit();
    outputs_.clear();
    outputs_.shrink_to_fit();
    invalidate_cache();
    outputs_hash_.reset();
    inpoints_hash_.reset();
    sequences_hash_.reset();
    segregated_ = none;
    total_input_value_ = none;
    total_output_value_ = none;
}

bool transaction::is_valid() const
{
    return (version_ != 0) || (locktime_ != 0) || !inputs_.empty() ||
        !outputs_.empty();
}

// Serialization.
//-----------------------------------------------------------------------------

// Transactions with empty witnesses always use old serialization (bip144).
// If no inputs are witness programs then witness hash is tx hash (bip141).
data_chunk transaction::to_data(bool wire, bool witness) const
{
    // Witness handling must be disabled for non-segregated txs.
    witness &= is_segregated();

    data_chunk data;
    const auto size = serialized_size(wire, witness);

    // Reserve an extra byte to prevent full reallocation in the case of
    // generate_signature_hash extension by addition of the sighash_type.
    data.reserve(size + sizeof(uint8_t));

    stream::out::data ostream(data);
    to_data(ostream, wire, witness);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void transaction::to_data(std::ostream& stream, bool wire, bool witness) const
{
    // Witness handling must be disabled for non-segregated txs.
    witness &= is_segregated();

    write::bytes::ostream out(stream);
    to_data(out, wire, witness);
}

// Witness is not used by outputs, just for template normalization.
void transaction::to_data(writer& sink, bool wire, bool witness) const
{
    if (wire)
    {
        // Witness handling must be disabled for non-segregated txs.
        witness &= is_segregated();

        // Wire (satoshi protocol) serialization.
        sink.write_4_bytes_little_endian(version_);

        if (witness)
        {
            sink.write_byte(witness_marker);
            sink.write_byte(witness_flag);
            write(sink, inputs_, wire, witness);
            write(sink, outputs_, wire, witness);
            write_witnesses(sink, inputs_);
        }
        else
        {
            write(sink, inputs_, wire, witness);
            write(sink, outputs_, wire, witness);
        }

        sink.write_4_bytes_little_endian(locktime_);
    }
    else
    {
        // Database (outputs forward) serialization.
        // Witness data is managed internal to inputs.
        write(sink, outputs_, wire, witness);
        write(sink, inputs_, wire, witness);
        sink.write_variable(locktime_);
        sink.write_variable(version_);
    }
}

// Size.
//-----------------------------------------------------------------------------

// static
size_t transaction::maximum_size(bool is_coinbase)
{
    // TODO: find smallest spendable coinbase tx with maximal input script.
    // This is not consensus critical but if too small is a disk fill vector.
    static const auto min_coinbase_tx = 1024;

    static const auto max_coinbase_tx = max_block_size - (sizeof(uint32_t) +
        header::satoshi_fixed_size());

    // A pool (non-coinbase) tx must fit into a block with at least a coinbase.
    return is_coinbase ? max_coinbase_tx : max_coinbase_tx - min_coinbase_tx;
}

size_t transaction::serialized_size(bool wire, bool witness) const
{
    // The witness parameter must be set to false for non-segregated txs.
    witness &= is_segregated();

    // Returns space for the witness although not serialized by input.
    // Returns witness space if specified even if input not segregated.
    const auto ins = [wire, witness](size_t size, const input& input)
    {
        return size + input.serialized_size(wire, witness);
    };

    const auto outs = [wire](size_t size, const output& output)
    {
        return size + output.serialized_size(wire);
    };

    // Must be both witness and wire encoding for bip144 serialization.
    return ((wire && witness) ? sizeof(witness_marker) : zero)
        + ((wire && witness) ? sizeof(witness_flag) : zero)
        + (wire ? sizeof(version_) : variable_size(version_))
        + (wire ? sizeof(locktime_) : variable_size(locktime_))
        + variable_size(inputs_.size())
        + variable_size(outputs_.size())
        + std::accumulate(inputs_.begin(), inputs_.end(), zero, ins)
        + std::accumulate(outputs_.begin(), outputs_.end(), zero, outs);
}

// Accessors.
//-----------------------------------------------------------------------------

uint32_t transaction::version() const
{
    return version_;
}

void transaction::set_version(uint32_t value)
{
    version_ = value;
    invalidate_cache();
}

uint32_t transaction::locktime() const
{
    return locktime_;
}

void transaction::set_locktime(uint32_t value)
{
    locktime_ = value;
    invalidate_cache();
}

input::list& transaction::inputs()
{
    return inputs_;
}

const input::list& transaction::inputs() const
{
    return inputs_;
}

void transaction::set_inputs(const input::list& value)
{
    inputs_ = value;
    invalidate_cache();
    inpoints_hash_.reset();
    sequences_hash_.reset();
    segregated_ = none;
    total_input_value_ = none;
}

void transaction::set_inputs(input::list&& value)
{
    inputs_ = std::move(value);
    invalidate_cache();
    segregated_ = none;
    total_input_value_ = none;
}

output::list& transaction::outputs()
{
    return outputs_;
}

const output::list& transaction::outputs() const
{
    return outputs_;
}

void transaction::set_outputs(const output::list& value)
{
    outputs_ = value;
    invalidate_cache();
    outputs_hash_.reset();
    total_output_value_ = none;
}

void transaction::set_outputs(output::list&& value)
{
    outputs_ = std::move(value);
    invalidate_cache();
    total_output_value_ = none;
}

// Cache.
//-----------------------------------------------------------------------------

// protected
void transaction::invalidate_cache() const
{
    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    hash_mutex_.lock_upgrade();

    if (hash_ || witness_hash_)
    {
        hash_mutex_.unlock_upgrade_and_lock();
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        hash_.reset();
        witness_hash_.reset();
        //---------------------------------------------------------------------
        hash_mutex_.unlock_and_lock_upgrade();
    }

    hash_mutex_.unlock_upgrade();
    ///////////////////////////////////////////////////////////////////////////
}

hash_digest transaction::hash(bool witness) const
{
    // Witness hashing must be disabled for non-segregated txs.
    witness &= is_segregated();

    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    hash_mutex_.lock_upgrade();

    if (witness)
    {
        if (!witness_hash_)
        {
            //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            hash_mutex_.unlock_upgrade_and_lock();

            // Witness coinbase tx hash is assumed to be null_hash (bip141).
            witness_hash_ = std::make_shared<hash_digest>(
                is_coinbase() ? null_hash : bitcoin_hash(to_data(true, true)));

            hash_mutex_.unlock_and_lock_upgrade();
            //-----------------------------------------------------------------
        }
    }
    else
    {
        if (!hash_)
        {
            //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            hash_mutex_.unlock_upgrade_and_lock();
            hash_ = std::make_shared<hash_digest>(bitcoin_hash(to_data(true)));
            hash_mutex_.unlock_and_lock_upgrade();
            //-----------------------------------------------------------------
        }
    }

    const auto hash = witness ? *witness_hash_ : *hash_;
    hash_mutex_.unlock_upgrade();
    ///////////////////////////////////////////////////////////////////////////

    return hash;
}

hash_digest transaction::outputs_hash() const
{
    RETURN_CACHED(outputs, bitcoin, hash);
}

hash_digest transaction::inpoints_hash() const
{
    RETURN_CACHED(inpoints, bitcoin, hash);
}

hash_digest transaction::sequences_hash() const
{
    RETURN_CACHED(sequences, bitcoin, hash);
}

// Utilities.
//-----------------------------------------------------------------------------

// Clear witness from all inputs (does not change default transaction hash).
void transaction::strip_witness()
{
    const auto strip = [](input& input)
    {
        input.strip_witness();
    };

    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    unique_lock lock(value_mutex_);

    segregated_ = false;
    std::for_each(inputs_.begin(), inputs_.end(), strip);
    ///////////////////////////////////////////////////////////////////////////
}

// Validation helpers.
//-----------------------------------------------------------------------------

bool transaction::is_coinbase() const
{
    return inputs_.size() == 1 && inputs_.front().previous_output().is_null();
}

// True if coinbase and has invalid input[0] script size.
bool transaction::is_oversized_coinbase() const
{
    if (!is_coinbase())
        return false;

    const auto script_size = inputs_.front().script().serialized_size(false);
    return script_size < min_coinbase_size || script_size > max_coinbase_size;
}

// True if not coinbase but has null previous_output(s).
bool transaction::is_null_non_coinbase() const
{
    if (is_coinbase())
        return false;

    const auto invalid = [](const input& input)
    {
        return input.previous_output().is_null();
    };

    return std::any_of(inputs_.begin(), inputs_.end(), invalid);
}

// private
bool transaction::all_inputs_final() const
{
    const auto finalized = [](const input& input)
    {
        return input.is_final();
    };

    return std::all_of(inputs_.begin(), inputs_.end(), finalized);
}

bool transaction::is_final(size_t block_height, uint32_t block_time) const
{
    const auto max_locktime = [=]()
    {
        return locktime_ < locktime_threshold ?
            safe_cast<uint32_t>(block_height) : block_time;
    };

    return is_zero(locktime_) || locktime_ < max_locktime() ||
        all_inputs_final();
}

bool transaction::is_locked(size_t block_height,
    uint32_t median_time_past) const
{
    if (version_ < relative_locktime_min_version || is_coinbase())
        return false;

    const auto locked = [block_height, median_time_past](const input& input)
    {
        return input.is_locked(block_height, median_time_past);
    };

    // If any input is relative time locked the transaction is as well.
    return std::any_of(inputs_.begin(), inputs_.end(), locked);
}

// This is not a consensus rule, just detection of an irrational use.
bool transaction::is_locktime_conflict() const
{
    return !is_zero(locktime_) && all_inputs_final();
}

// Overflow returns max_uint64.
uint64_t transaction::total_input_value() const
{
    uint64_t value;

    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    value_mutex_.lock_upgrade();

    if (!is_none(total_input_value_))
    {
        value = total_input_value_.get();
        value_mutex_.unlock_upgrade();
        //---------------------------------------------------------------------
        return value;
    }

    value_mutex_.unlock_upgrade_and_lock();
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    const auto sum = [](uint64_t total, const input& input)
    {
        const auto& prevout = input.previous_output().metadata.cache;
        const auto missing = !prevout.is_valid();

        // Treat missing previous outputs as zero-valued, no math on sentinel.
        return ceilinged_add(total, missing ? 0u : prevout.value());
    };

    value = std::accumulate(inputs_.begin(), inputs_.end(), uint64_t(0), sum);
    total_input_value_ = value;
    value_mutex_.unlock();
    ///////////////////////////////////////////////////////////////////////////

    return value;
}

// Overflow returns max_uint64.
uint64_t transaction::total_output_value() const
{
    uint64_t value;

    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    value_mutex_.lock_upgrade();

    if (!is_none(total_output_value_))
    {
        value = total_output_value_.get();
        value_mutex_.unlock_upgrade();
        //---------------------------------------------------------------------
        return value;
    }

    value_mutex_.unlock_upgrade_and_lock();
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    const auto sum = [](uint64_t total, const output& output)
    {
        return ceilinged_add(total, output.value());
    };

    value = std::accumulate(outputs_.begin(), outputs_.end(), uint64_t(0), sum);
    total_output_value_ = value;
    value_mutex_.unlock();
    ///////////////////////////////////////////////////////////////////////////

    return value;
}

uint64_t transaction::fees() const
{
    return floored_subtract(total_input_value(), total_output_value());
}

bool transaction::is_overspent() const
{
    return !is_coinbase() && total_output_value() > total_input_value();
}

// Returns max_size_t in case of overflow.
size_t transaction::signature_operations() const
{
    const auto state = metadata.state;
    const auto bip16 = state->is_enabled(rule_fork::bip16_rule);
    const auto bip141 = state->is_enabled(rule_fork::bip141_rule);
    return state ? signature_operations(bip16, bip141) : max_size_t;
}

// Overflow returns max_size_t.
size_t transaction::signature_operations(bool bip16, bool bip141) const
{
    const auto in = [bip16, bip141](size_t total, const input& input)
    {
        // This includes BIP16 p2sh additional sigops if prevout is cached.
        return ceilinged_add(total, input.signature_operations(bip16, bip141));
    };

    const auto out = [bip141](size_t total, const output& output)
    {
        return ceilinged_add(total, output.signature_operations(bip141));
    };

    return std::accumulate(inputs_.begin(), inputs_.end(), zero, in) +
        std::accumulate(outputs_.begin(), outputs_.end(), zero, out);
}

size_t transaction::weight() const
{
    // Block weight is 3 * Base size * + 1 * Total size (bip141).
    return base_size_contribution * serialized_size(true, false) +
        total_size_contribution * serialized_size(true, true);
}

bool transaction::is_missing_previous_outputs() const
{
    const auto missing = [](const input& input)
    {
        const auto& prevout = input.previous_output();
        const auto coinbase = prevout.is_null();
        const auto missing = !prevout.metadata.cache.is_valid();
        return missing && !coinbase;
    };

    // This is an optimization of !missing_inputs().empty();
    return std::any_of(inputs_.begin(), inputs_.end(), missing);
}

point::list transaction::previous_outputs() const
{
    point::list out(no_fill_allocator);
    out.resize(inputs_.size());

    std::transform(inputs_.begin(), inputs_.end(), out.begin(),
        [](const input& input)
        {
            return input.previous_output();
        });

    return out;
}

point::list transaction::missing_previous_outputs() const
{
    point::list prevouts;
    prevouts.reserve(inputs_.size());
    const auto accumulator = [&prevouts](const input& input)
    {
        const auto& prevout = input.previous_output();
        const auto missing = !prevout.metadata.cache.is_valid();

        if (missing && !prevout.is_null())
            prevouts.push_back(prevout);
    };

    std::for_each(inputs_.begin(), inputs_.end(), accumulator);
    prevouts.shrink_to_fit();
    return prevouts;
}

hash_list transaction::missing_previous_transactions() const
{
    const auto points = missing_previous_outputs();
    hash_list hashes(no_fill_allocator);
    hashes.resize(points.size());

    std::transform(points.begin(), points.end(), hashes.begin(),
        [](const output_point& point)
        {
            return point.hash();
        });

    return std::move(distinct(hashes));
}

bool transaction::is_internal_double_spend() const
{
    return !is_distinct(previous_outputs());
}

bool transaction::is_confirmed_double_spend() const
{
    const auto spent = [](const input& input)
    {
        return input.previous_output().metadata.confirmed_spent;
    };

    return std::any_of(inputs_.begin(), inputs_.end(), spent);
}

bool transaction::is_dusty(uint64_t minimum_output_value) const
{
    const auto dust = [minimum_output_value](const output& output)
    {
        return output.is_dust(minimum_output_value);
    };

    return std::any_of(outputs_.begin(), outputs_.end(), dust);
}

bool transaction::is_mature(size_t height) const
{
    const auto mature = [height](const input& input)
    {
        return input.previous_output().is_mature(height);
    };

    return std::all_of(inputs_.begin(), inputs_.end(), mature);
}

bool transaction::is_segregated() const
{
    bool value;

    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    value_mutex_.lock_upgrade();

    if (!is_none(segregated_))
    {
        value = segregated_.get();
        value_mutex_.unlock_upgrade();
        //---------------------------------------------------------------------
        return value;
    }

    value_mutex_.unlock_upgrade_and_lock();
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    const auto segregated = [](const input& input)
    {
        return input.is_segregated();
    };

    // If no block tx is has witness data the commitment is optional (bip141).
    value = std::any_of(inputs_.begin(), inputs_.end(), segregated);
    value_mutex_.unlock();
    ///////////////////////////////////////////////////////////////////////////

    return value;
}

// Coinbase transactions return success, to simplify iteration.
code transaction::connect_input(const chain_state& state,
    size_t input_index) const
{
    if (input_index >= inputs_.size())
        return error::operation_failed;

    if (is_coinbase())
        return error::success;

    const auto& prevout = inputs_[input_index].previous_output().metadata;

    // Verify that the previous output cache has been populated.
    if (!prevout.cache.is_valid())
        return error::missing_previous_output;

    const auto forks = state.enabled_forks();
    const auto index32 = static_cast<uint32_t>(input_index);

    // Verify the transaction input script against the previous output.
    return script::verify(*this, index32, forks);
}

// Validation.
//-----------------------------------------------------------------------------

// These checks are self-contained; blockchain (and so version) independent.
code transaction::check(uint64_t max_money, bool transaction_pool) const
{
    if (inputs_.empty() || outputs_.empty())
        return error::empty_transaction;

    else if (is_null_non_coinbase())
        return error::previous_output_null;

    else if (total_output_value() > max_money)
        return error::spend_overflow;

    else if (!transaction_pool && is_oversized_coinbase())
        return error::invalid_coinbase_script_size;

    else if (transaction_pool && is_coinbase())
        return error::coinbase_transaction;

    else if (transaction_pool && is_internal_double_spend())
        return error::transaction_internal_double_spend;

    // TODO: reduce by header, txcount and smallest coinbase size for height.
    else if (transaction_pool && serialized_size(true, false) >=
        maximum_size(false))
        return error::transaction_size_limit;

    // We cannot know if bip16/bip141 is enabled here so we do not check it.
    // This will not make a difference unless prevouts are populated, in which
    // case they are ignored. This means that p2sh sigops are not counted here.
    // This is a preliminary check, the final count must come from accept().
    // Reenable once sigop caching is implemented, otherwise is deoptimization.
    ////else if (transaction_pool &&
    ////    signature_operations(false, false) > max_block_sigops)
    ////    return error::transaction_legacy_sigop_limit;

    else
        return error::success;
}

code transaction::accept(bool transaction_pool) const
{
    const auto state = metadata.state;
    return state ? accept(*state, transaction_pool) : error::operation_failed;
}

// These checks assume that prevout caching is completed on all tx.inputs.
code transaction::accept(const chain_state& state, bool transaction_pool) const
{
    const auto bip16 = state.is_enabled(rule_fork::bip16_rule);
    const auto bip68 = state.is_enabled(rule_fork::bip68_rule);
    const auto bip141 = state.is_enabled(rule_fork::bip141_rule);

    // Segwit sigops are discounted by increasing limit and legacy weight.
    const auto max_sigops = bip141 ? max_fast_sigops : max_block_sigops;

    if (transaction_pool && state.is_under_checkpoint())
        return error::premature_validation;

    // A segregated tx should appear empty if bip141 is not enabled.
    if (!bip141 && is_segregated())
        return error::empty_transaction;

    if (transaction_pool && !is_final(state.height(), state.median_time_past()))
        return error::transaction_non_final;

    if (transaction_pool && version() > state.maximum_transaction_version())
        return error::transaction_version;

    else if (is_missing_previous_outputs())
        return error::missing_previous_output;

    else if (is_confirmed_double_spend())
        return error::double_spend;

    // This relates height to maturity of spent coinbase. Since reorg is the
    // only way to decrease height and reorg invalidates, this is cache safe.
    else if (!is_mature(state.height()))
        return error::coinbase_maturity;

    else if (is_overspent())
        return error::spend_exceeds_value;

    else if (bip68 && is_locked(state.height(), state.median_time_past()))
        return error::sequence_locked;

    // This recomputes sigops to include p2sh from prevouts if bip16 is true.
    else if (transaction_pool && signature_operations(bip16, bip141) > max_sigops)
        return error::transaction_embedded_sigop_limit;

    // This causes second serialized_size(true, false) computation (uncached).
    // TODO: reduce by header, txcount and smallest coinbase size for height.
    else if (transaction_pool && bip141 && weight() > max_block_weight)
        return error::transaction_weight_limit;

    else
        return error::success;
}

code transaction::connect() const
{
    const auto state = metadata.state;
    return state ? connect(*state) : error::operation_failed;
}

code transaction::connect(const chain_state& state) const
{
    code ec;

    for (size_t input = 0; input < inputs_.size(); ++input)
        if ((ec = connect_input(state, input)))
            return ec;

    return error::success;
}

#undef RETURN_CACHED

} // namespace chain
} // namespace system
} // namespace libbitcoin
