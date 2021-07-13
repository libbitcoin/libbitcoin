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
#include <bitcoin/system/chain/script.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <numeric>
#include <sstream>
#include <utility>
#include <boost/range/adaptor/reversed.hpp>
#include <bitcoin/system/assert.hpp>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/chain/enums/magic_numbers.hpp>
#include <bitcoin/system/chain/enums/opcode.hpp>
#include <bitcoin/system/chain/operation.hpp>
#include <bitcoin/system/chain/transaction.hpp>
#include <bitcoin/system/chain/witness.hpp>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/error.hpp>
#include <bitcoin/system/machine/machine.hpp>
#include <bitcoin/system/radix/radix.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace chain {

using namespace bc::system::machine;

const hash_digest script::one = hash_literal(
    "0000000000000000000000000000000000000000000000000000000000000001");

// Constructors.
//-----------------------------------------------------------------------------

// A default instance is invalid (until modified).
script::script()
  : cached_(false),
    valid_(false)
{
}

script::script(script&& other)
  : operations_(std::move(other.operations_move())),
    cached_(!operations_.empty()),
    bytes_(std::move(other.bytes_)),
    valid_(other.valid_)
{
}

script::script(const script& other)
  : operations_(other.operations_copy()),
    cached_(!operations_.empty()),
    bytes_(other.bytes_),
    valid_(other.valid_)
{
}

script::script(const operation::list& ops)
{
    from_operations(ops);
}

script::script(operation::list&& ops)
{
    from_operations(std::move(ops));
}

script::script(data_chunk&& encoded, bool prefix)
{
    if (prefix)
    {
        valid_ = from_data(encoded, prefix);
        return;
    }

    // This is an optimization that avoids streaming the encoded bytes.
    bytes_ = std::move(encoded);
    cached_ = false;
    valid_ = true;
}

script::script(const data_chunk& encoded, bool prefix)
{
    valid_ = from_data(encoded, prefix);
}

// Private cache access for move construction.
operation::list& script::operations_move()
{
    shared_lock lock(mutex_);
    return operations_;
}

// Private cache access for copy construction.
const operation::list& script::operations_copy() const
{
    shared_lock lock(mutex_);
    return operations_;
}

// Operators.
//-----------------------------------------------------------------------------

// Concurrent read/write is not supported, so no critical section.
script& script::operator=(script&& other)
{
    operations_ = other.operations_move();
    cached_ = !operations_.empty();
    bytes_ = std::move(other.bytes_);
    valid_ = other.valid_;
    return *this;
}

// Concurrent read/write is not supported, so no critical section.
script& script::operator=(const script& other)
{
    operations_ = other.operations_copy();
    cached_ = !operations_.empty();
    bytes_ = other.bytes_;
    valid_ = other.valid_;
    return *this;
}

bool script::operator==(const script& other) const
{
    return bytes_ == other.bytes_;
}

bool script::operator!=(const script& other) const
{
    return !(*this == other);
}

// Deserialization.
//-----------------------------------------------------------------------------

// static
script script::factory(const data_chunk& encoded, bool prefix)
{
    script instance;
    instance.from_data(encoded, prefix);
    return instance;
}

// static
script script::factory(std::istream& stream, bool prefix)
{
    script instance;
    instance.from_data(stream, prefix);
    return instance;
}

// static
script script::factory(reader& source, bool prefix)
{
    script instance;
    instance.from_data(source, prefix);
    return instance;
}

bool script::from_data(const data_chunk& encoded, bool prefix)
{
    stream::in::copy istream(encoded);
    return from_data(istream, prefix);
}

bool script::from_data(std::istream& stream, bool prefix)
{
    read::bytes::istream source(stream);
    return from_data(source, prefix);
}

// Concurrent read/write is not supported, so no critical section.
bool script::from_data(reader& source, bool prefix)
{
    reset();
    valid_ = true;

    if (prefix)
    {
        const auto size = source.read_size();

        // The max_script_size constant limits evaluation, but not all scripts
        // evaluate, so use max_block_size to guard memory allocation here.
        if (size > max_block_size)
            source.invalidate();
        else
            bytes_ = source.read_bytes(size);
    }
    else
    {
        bytes_ = source.read_bytes();
    }

    if (!source)
        reset();

    return source;
}

// Concurrent read/write is not supported, so no critical section.
bool script::from_string(const std::string& mnemonic)
{
    reset();

    // There is strictly one operation per string token.
    const auto tokens = split(mnemonic);
    operation::list ops;
    ops.resize(tokens.empty() || tokens.front().empty() ? 0 : tokens.size());

    // Create an op list from the split tokens, one operation per token.
    for (size_t index = 0; index < ops.size(); ++index)
        if (!ops[index].from_string(tokens[index]))
            return false;

    from_operations(ops);
    return true;
}

// Concurrent read/write is not supported, so no critical section.
void script::from_operations(operation::list&& ops)
{
    ////reset();
    bytes_ = operations_to_data(ops);
    operations_ = std::move(ops);
    cached_ = true;
    valid_ = true;
}

// Concurrent read/write is not supported, so no critical section.
void script::from_operations(const operation::list& ops)
{
    ////reset();
    bytes_ = operations_to_data(ops);
    operations_ = ops;
    cached_ = true;
    valid_ = true;
}

// private/static
data_chunk script::operations_to_data(const operation::list& ops)
{
    data_chunk out;
    const auto size = serialized_size(ops);
    out.reserve(size);
    const auto concatenate = [&out](const operation& op)
    {
        auto bytes = op.to_data();
        std::move(bytes.begin(), bytes.end(), std::back_inserter(out));
    };

    std::for_each(ops.begin(), ops.end(), concatenate);
    BITCOIN_ASSERT(out.size() == size);
    return out;
}

// private/static
size_t script::serialized_size(const operation::list& ops)
{
    const auto op_size = [](size_t total, const operation& op)
    {
        return total + op.serialized_size();
    };

    return std::accumulate(ops.begin(), ops.end(), zero, op_size);
}

// protected
// Concurrent read/write is not supported, so no critical section.
void script::reset()
{
    bytes_.clear();
    bytes_.shrink_to_fit();
    valid_ = false;
    cached_ = false;
    operations_.clear();
    operations_.shrink_to_fit();
}

bool script::is_valid() const
{
    // All script bytes are valid under some circumstance (e.g. coinbase).
    // This returns false if a prefix and byte count does not match.
    return valid_;
}

bool script::is_valid_operations() const
{
    // Script validity is independent of individual operation validity.
    // There is a trailing invalid/default op if a push op had a size mismatch.
    return operations().empty() || operations_.back().is_valid();
}

// Serialization.
//-----------------------------------------------------------------------------

data_chunk script::to_data(bool prefix) const
{
    data_chunk data;
    const auto size = serialized_size(prefix);
    data.reserve(size);
    stream::out::data ostream(data);
    to_data(ostream, prefix);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void script::to_data(std::ostream& stream, bool prefix) const
{
    write::bytes::ostream out(stream);
    to_data(out, prefix);
}

void script::to_data(writer& sink, bool prefix) const
{
    // TODO: optimize by always storing the prefixed serialization.
    if (prefix)
        sink.write_variable(serialized_size(false));

    sink.write_bytes(bytes_);
}

std::string script::to_string(uint32_t active_forks) const
{
    auto first = true;
    std::ostringstream text;

    for (const auto& op: operations())
    {
        text << (first ? "" : " ") << op.to_string(active_forks);
        first = false;
    }

    // An invalid operation has a specialized serialization.
    return text.str();
}

hash_digest script::to_payments_key() const
{
    return sha256_hash(to_data(false));
}


// Iteration.
//-----------------------------------------------------------------------------
// These are syntactic sugar that allow the caller to iterate ops directly.
// The first operations access must be method-based to guarantee the cache.

void script::clear()
{
    reset();
}

bool script::empty() const
{
    return operations().empty();
}

size_t script::size() const
{
    return operations().size();
}

const operation& script::front() const
{
    BITCOIN_ASSERT(!operations().empty());
    return operations().front();
}

const operation& script::back() const
{
    BITCOIN_ASSERT(!operations().empty());
    return operations().back();
}

const operation& script::operator[](size_t index) const
{
    BITCOIN_ASSERT(index < operations().size());
    return operations()[index];
}

operation::iterator script::begin() const
{
    return operations().begin();
}

operation::iterator script::end() const
{
    return operations().end();
}

// Properties (size, accessors, cache).
//-----------------------------------------------------------------------------

size_t script::serialized_size(bool prefix) const
{
    auto size = bytes_.size();

    if (prefix)
        size += variable_size(size);

    return size;
}

// protected
const operation::list& script::operations() const
{
    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    mutex_.lock_upgrade();

    if (cached_)
    {
        mutex_.unlock_upgrade();
        //---------------------------------------------------------------------
        return operations_;
    }

    operation op;
    read::bytes::copy source(bytes_);
    const auto size = bytes_.size();

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    mutex_.unlock_upgrade_and_lock();

    // One operation per byte is the upper limit of operations.
    operations_.reserve(size);

    // ************************************************************************
    // CONSENSUS: In the case of a coinbase script we must parse the entire
    // script, beyond just the BIP34 requirements, so that sigops can be
    // calculated from the script. These are counted despite being irrelevant.
    // In this case an invalid script is parsed to the extent possible.
    // ************************************************************************

    // If an op fails it is pushed to operations and the loop terminates.
    // To validate the ops the caller must test the last op.is_valid(), or may
    // text script.is_valid_operations(), which is done in script metadata.
    while (!source.is_exhausted())
    {
        op.from_data(source);
        operations_.push_back(std::move(op));
    }

    operations_.shrink_to_fit();
    cached_ = true;

    mutex_.unlock();
    ///////////////////////////////////////////////////////////////////////////

    return operations_;
}

// Signing (unversioned).
//-----------------------------------------------------------------------------

inline hash_digest signature_hash(const transaction& tx, uint32_t sighash_type)
{
    // There is no rational interpretation of a signature hash for a coinbase.
    BITCOIN_ASSERT(!tx.is_coinbase());

    // TODO: pass overallocated stream buffer to serializer (optimization).
    auto serialized = tx.to_data(true, false);
    extend(serialized, to_little_endian(sighash_type));
    return bitcoin_hash(serialized);
}

//*****************************************************************************
// CONSENSUS: Due to masking of bits 6/7 (8 is the anyone_can_pay flag),
// there are 4 possible 7 bit values that can set "single" and 4 others that
// can set none, and yet all other values set "all".
//*****************************************************************************
inline sighash_algorithm to_sighash_enum(uint8_t sighash_type)
{
    switch (sighash_type & sighash_algorithm::mask)
    {
        case sighash_algorithm::hash_single:
            return sighash_algorithm::hash_single;
        case sighash_algorithm::hash_none:
            return sighash_algorithm::hash_none;
        default:
            return sighash_algorithm::hash_all;
    }
}

static hash_digest sign_none(const transaction& tx, uint32_t input_index,
    const script& script_code, uint8_t sighash_type)
{
    input::list ins;
    const auto& inputs = tx.inputs();
    const auto any = !is_zero(sighash_type & sighash_algorithm::anyone_can_pay);
    ins.reserve(any ? one : inputs.size());

    BITCOIN_ASSERT(input_index < inputs.size());
    const auto& self = inputs[input_index];

    if (any)
    {
        // Retain only self.
        ins.emplace_back(self.previous_output(), script_code, self.sequence());
    }
    else
    {
        // Erase all input scripts and sequences.
        for (const auto& input: inputs)
            ins.emplace_back(input.previous_output(), script{}, 0);

        // Replace self that is lost in the loop.
        ins[input_index].set_script(script_code);
        ins[input_index].set_sequence(self.sequence());
    }

    // Move new inputs to new transaction and drop outputs.
    return signature_hash({ tx.version(), tx.locktime(), std::move(ins), {} },
        sighash_type);
}

static hash_digest sign_single(const transaction& tx, uint32_t input_index,
    const script& script_code, uint8_t sighash_type)
{
    input::list ins;
    const auto& inputs = tx.inputs();
    const auto any = !is_zero(sighash_type & sighash_algorithm::anyone_can_pay);
    ins.reserve(any ? one : inputs.size());

    BITCOIN_ASSERT(input_index < inputs.size());
    const auto& self = inputs[input_index];

    if (any)
    {
        // Retain only self.
        ins.emplace_back(self.previous_output(), script_code, self.sequence());
    }
    else
    {
        // Erase all input scripts and sequences.
        for (const auto& input: inputs)
            ins.emplace_back(input.previous_output(), script{}, 0);

        // Replace self that is lost in the loop.
        ins[input_index].set_script(script_code);
        ins[input_index].set_sequence(self.sequence());
    }

    // Trim and clear outputs except that of specified input index.
    const auto& outputs = tx.outputs();
    output::list outs(input_index + 1);

    BITCOIN_ASSERT(input_index < outputs.size());
    outs.back() = outputs[input_index];

    // Move new inputs and new outputs to new transaction.
    return signature_hash({ tx.version(), tx.locktime(), std::move(ins),
        std::move(outs) }, sighash_type);
}

static hash_digest sign_all(const transaction& tx, uint32_t input_index,
    const script& script_code, uint8_t sighash_type)
{
    input::list ins;
    const auto& inputs = tx.inputs();
    const auto any = !is_zero(sighash_type & sighash_algorithm::anyone_can_pay);
    ins.reserve(any ? one : inputs.size());

    BITCOIN_ASSERT(input_index < inputs.size());
    const auto& self = inputs[input_index];

    if (any)
    {
        // Retain only self.
        ins.emplace_back(self.previous_output(), script_code, self.sequence());
    }
    else
    {
        // Erase all input scripts.
        for (const auto& input: inputs)
            ins.emplace_back(input.previous_output(), script{},
                input.sequence());

        // Replace self that is lost in the loop.
        ins[input_index].set_script(script_code);
        ////ins[input_index].set_sequence(self.sequence());
    }

    // Move new inputs and copy outputs to new transaction.
    transaction out(tx.version(), tx.locktime(), input::list{}, tx.outputs());
    out.set_inputs(std::move(ins));
    return signature_hash(out, sighash_type);
}

static bool is_index_overflow(const transaction& tx, uint32_t input_index,
    sighash_algorithm sighash)
{
    return input_index >= tx.inputs().size() ||
        (input_index >= tx.outputs().size() && 
            sighash == sighash_algorithm::hash_single);
}

// TODO: optimize to prevent script reconstruction.
static script strip_code_seperators(const script& script_code)
{
    operation::list ops;

    for (auto op = script_code.begin(); op != script_code.end(); ++op)
        if (op->code() != opcode::codeseparator)
            ops.push_back(*op);

    return { std::move(ops) };
}

// private/static
hash_digest script::generate_unversioned_signature_hash(const transaction& tx,
    uint32_t input_index, const script& script_code, uint8_t sighash_type)
{
    const auto sighash = to_sighash_enum(sighash_type);

    //*************************************************************************
    // CONSENSUS: wacky satoshi behavior (continuing with one hash).
    //*************************************************************************
    if (is_index_overflow(tx, input_index, sighash))
        return script::one;

    //*************************************************************************
    // CONSENSUS: more wacky satoshi behavior.
    //*************************************************************************
    const auto stripped = strip_code_seperators(script_code);

    // The sighash serializations are isolated for clarity and optimization.
    switch (sighash)
    {
        case sighash_algorithm::hash_none:
            return sign_none(tx, input_index, stripped, sighash_type);
        case sighash_algorithm::hash_single:
            return sign_single(tx, input_index, stripped, sighash_type);
        default:
        case sighash_algorithm::hash_all:
            return sign_all(tx, input_index, stripped, sighash_type);
    }
}

// Signing (version 0).
//-----------------------------------------------------------------------------

data_chunk script::to_outputs(const transaction& tx)
{
    const auto sum = [&](size_t total, const output& output)
    {
        return total + output.serialized_size();
    };

    const auto& outs = tx.outputs();
    auto size = std::accumulate(outs.begin(), outs.end(), zero, sum);
    data_chunk data;
    data.reserve(size);
    write::bytes::data out(data);

    const auto write = [&](const output& output)
    {
        output.to_data(out, true);
    };

    std::for_each(outs.begin(), outs.end(), write);
    out.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

data_chunk script::to_inpoints(const transaction& tx)
{
    const auto sum = [&](size_t total, const input& input)
    {
        return total + input.previous_output().serialized_size();
    };

    const auto& ins = tx.inputs();
    auto size = std::accumulate(ins.begin(), ins.end(), zero, sum);
    data_chunk data;
    data.reserve(size);
    write::bytes::data out(data);

    const auto write = [&](const input& input)
    {
        input.previous_output().to_data(out);
    };

    std::for_each(ins.begin(), ins.end(), write);
    out.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

data_chunk script::to_sequences(const transaction& tx)
{
    const auto sum = [&](size_t total, const input& /* input */)
    {
        return total + sizeof(uint32_t);
    };

    const auto& ins = tx.inputs();
    auto size = std::accumulate(ins.begin(), ins.end(), zero, sum);
    data_chunk data;
    data.reserve(size);
    write::bytes::data out(data);

    const auto write = [&](const input& input)
    {
        out.write_4_bytes_little_endian(input.sequence());
    };

    std::for_each(ins.begin(), ins.end(), write);
    out.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

static size_t version_0_preimage_size(size_t script_size)
{
    return sizeof(uint32_t)
        + hash_size
        + hash_size
        + point::satoshi_fixed_size()
        + script_size
        + sizeof(uint64_t)
        + sizeof(uint32_t)
        + hash_size
        + sizeof(uint32_t)
        + sizeof(uint32_t);
}

// private/static
hash_digest script::generate_version_0_signature_hash(const transaction& tx,
    uint32_t input_index, const script& script_code, uint64_t value,
    uint8_t sighash_type)
{
    const auto sighash = to_sighash_enum(sighash_type);

    const auto any = !is_zero(sighash_type & sighash_algorithm::anyone_can_pay);
    const auto single = (sighash == sighash_algorithm::hash_single);
    //// const auto none = (sighash == sighash_algorithm::hash_none);
    const auto all = (sighash == sighash_algorithm::hash_all);

    // Unlike unversioned algorithm this does not allow an invalid input index.
    BITCOIN_ASSERT(input_index < tx.inputs().size());
    const auto& input = tx.inputs()[input_index];
    const auto script_size = script_code.serialized_size(true);
    const auto size = version_0_preimage_size(script_size);

    data_chunk data;
    data.reserve(size);
    write::bytes::data out(data);

    // 1. transaction version (4).
    out.write_little_endian(tx.version());

    // 2. inpoints double sha256 hash (32).
    out.write_bytes(!any ? tx.inpoints_hash() : null_hash);

    // 3. sequences double sha256 hash (32).
    out.write_bytes(!any && all ? tx.sequences_hash() : null_hash);

    // 4. outpoint (32-byte hash + 4-byte little endian).
    input.previous_output().to_data(out);

    // 5. script of the input (with prefix).
    script_code.to_data(out, true);

    // 6. value of the output spent by this input (8).
    out.write_little_endian(value);

    // 7. sequence of the input (4).
    out.write_little_endian(input.sequence());

    // 8. outputs (or output) double hash, or null hash (32).
    out.write_bytes(all ? tx.outputs_hash() :
        (single && input_index < tx.outputs().size() ?
            bitcoin_hash(tx.outputs()[input_index].to_data()) : null_hash));

    // 9. transaction locktime (4).
    out.write_little_endian(tx.locktime());

    // 10. hash type of the signature (4 [not 1]).
    out.write_4_bytes_little_endian(sighash_type);

    out.flush();
    BITCOIN_ASSERT(data.size() == size);
    return bitcoin_hash(data);
}

// Signing (unversioned and version 0).
//-----------------------------------------------------------------------------

// static
hash_digest script::generate_signature_hash(const transaction& tx,
    uint32_t input_index, const script& script_code, uint8_t sighash_type,
    script_version version, uint64_t value)
{
    // The way of serialization is changed (bip143).
    switch (version)
    {
        case script_version::unversioned:
            return generate_unversioned_signature_hash(tx, input_index,
                script_code, sighash_type);
        case script_version::zero:
            return generate_version_0_signature_hash(tx, input_index,
                script_code, value, sighash_type);
        case script_version::reserved:
        default:
            BITCOIN_ASSERT_MSG(false, "invalid script version");
            return {};
    }
}

// static
bool script::check_signature(const ec_signature& signature,
    uint8_t sighash_type, const data_chunk& public_key,
    const script& script_code, const transaction& tx, uint32_t input_index,
    script_version version, uint64_t value)
{
    if (signature.empty() || public_key.empty())
        return false;

    // This always produces a valid signature hash, including one_hash.
    const auto sighash = chain::script::generate_signature_hash(tx,
        input_index, script_code, sighash_type, version, value);

    // Validate the EC signature.
    return verify_signature(public_key, sighash, signature);
}

// static
bool script::create_endorsement(endorsement& out, const ec_secret& secret,
    const script& prevout_script, const transaction& tx, uint32_t input_index,
    uint8_t sighash_type, script_version version, uint64_t value)
{
    out.reserve(max_endorsement_size);

    // This always produces a valid signature hash, including one_hash.
    const auto sighash = chain::script::generate_signature_hash(tx,
        input_index, prevout_script, sighash_type, version, value);

    // Create the EC signature and encode as DER.
    ec_signature signature;
    if (!sign(signature, secret, sighash) || !encode_signature(out, signature))
        return false;

    // Add the sighash type to the end of the DER signature -> endorsement.
    out.push_back(sighash_type);
    out.shrink_to_fit();
    return true;
}

// Utilities (static).
//-----------------------------------------------------------------------------

bool script::is_push_only(const operation::list& ops)
{
    const auto push = [](const operation& op)
    {
        return op.is_push();
    };

    return std::all_of(ops.begin(), ops.end(), push);
}

//*****************************************************************************
// CONSENSUS: this pattern is used to activate bip16 validation rules.
//*****************************************************************************
bool script::is_relaxed_push(const operation::list& ops)
{
    const auto push = [&](const operation& op)
    {
        return op.is_relaxed_push();
    };

    return std::all_of(ops.begin(), ops.end(), push);
}

//*****************************************************************************
// CONSENSUS: BIP34 requires coinbase input script to begin with one byte that
// indicates the height size. This is inconsistent with an extreme future where
// the size byte overflows. However satoshi actually requires nominal encoding.
//*****************************************************************************
bool script::is_coinbase_pattern(const operation::list& ops, size_t height)
{
    return !ops.empty()
        && ops[0].is_nominal_push()
        && ops[0].data() == number(height).data();
}

//*****************************************************************************
// CONSENSUS: this pattern is used to commit to bip141 witness data.
//*****************************************************************************
bool script::is_commitment_pattern(const operation::list& ops)
{
    static const auto header = to_big_endian(witness_head);

    // Bytes after commitment are optional with no consensus meaning (bip141).
    // Commitment is not executable so invalid trailing operations are allowed.
    return ops.size() > 1
        && ops[0].code() == opcode::return_
        && ops[1].code() == opcode::push_size_36
        && std::equal(header.begin(), header.end(), ops[1].data().begin());
}

//*****************************************************************************
// CONSENSUS: this pattern is used in bip141 validation rules.
//*****************************************************************************
bool script::is_witness_program_pattern(const operation::list& ops)
{
    return ops.size() == 2
        && ops[0].is_version()
        && ops[1].data().size() >= min_witness_program
        && ops[1].data().size() <= max_witness_program;
}

// The satoshi client tests for 83 bytes total. This allows for the waste of
// one byte to represent up to 75 bytes using the push_one_size opcode.
// It also allows any number of push ops and limits it to 0 value and 1 per tx.
////bool script::is_pay_null_data_pattern(const operation::list& ops)
////{
////    static constexpr auto op_76 = static_cast<uint8_t>(opcode::push_one_size);
////
////    return ops.size() >= 2
////        && ops[0].code() == opcode::return_
////        && static_cast<uint8_t>(ops[1].code()) <= op_76
////        && ops[1].data().size() <= max_null_data_size;
////}

// The satoshi client enables configurable data size for policy.
bool script::is_pay_null_data_pattern(const operation::list& ops)
{
    return ops.size() == 2
        && ops[0].code() == opcode::return_
        && ops[1].is_minimal_push()
        && ops[1].data().size() <= max_null_data_size;
}

// TODO: expand this to the 20 signature op_check_multisig limit.
// The current 16 (or 20) limit does not affect server indexing because bare
// multisig is not indexable and p2sh multisig is byte-limited to 15 sigs.
// The satoshi client policy limit is 3 signatures for bare multisig.
bool script::is_pay_multisig_pattern(const operation::list& ops)
{
    static constexpr auto op_1 = static_cast<uint8_t>(opcode::push_positive_1);
    static constexpr auto op_16 = static_cast<uint8_t>(opcode::push_positive_16);

    const auto op_count = ops.size();

    if (op_count < 4 || ops[op_count - 1].code() != opcode::checkmultisig)
        return false;

    const auto op_m = static_cast<uint8_t>(ops[0].code());
    const auto op_n = static_cast<uint8_t>(ops[op_count - 2].code());

    if (op_m < op_1 || op_m > op_n || op_n < op_1 || op_n > op_16)
        return false;

    const auto number = op_n - op_1 + 1u;
    const auto points = op_count - 3u;

    if (number != points)
        return false;

    for (auto op = std::next(ops.begin()); op != std::prev(ops.end(), 2); ++op)
        if (!is_public_key(op->data()))
            return false;

    return true;
}

// The satoshi client considers this non-standard for policy.
bool script::is_pay_public_key_pattern(const operation::list& ops)
{
    return ops.size() == 2
        && is_public_key(ops[0].data())
        && ops[1].code() == opcode::checksig;
}

bool script::is_pay_key_hash_pattern(const operation::list& ops)
{
    return ops.size() == 5
        && ops[0].code() == opcode::dup
        && ops[1].code() == opcode::hash160
        && ops[2].data().size() == short_hash_size
        && ops[3].code() == opcode::equalverify
        && ops[4].code() == opcode::checksig;
}

//*****************************************************************************
// CONSENSUS: this pattern is used to activate bip16 validation rules.
//*****************************************************************************
bool script::is_pay_script_hash_pattern(const operation::list& ops)
{
    return ops.size() == 3
        && ops[0].code() == opcode::hash160
        && ops[1].code() == opcode::push_size_20
        && ops[2].code() == opcode::equal;
}

bool script::is_pay_witness_pattern(const operation::list& ops)
{
    return ops.size() == 2
        && ops[0].is_version()
        && ops[1].is_push();
}

bool script::is_pay_witness_key_hash_pattern(const operation::list& ops)
{
    return ops.size() == 2
        && ops[0].code() == opcode::push_size_0
        && ops[1].code() == opcode::push_size_20;
}

//*****************************************************************************
// CONSENSUS: this pattern is used to activate bip141 validation rules.
//*****************************************************************************
bool script::is_pay_witness_script_hash_pattern(const operation::list& ops)
{
    return ops.size() == 2
        && ops[0].code() == opcode::push_size_0
        && ops[1].code() == opcode::push_size_32;
}

// The first push is based on wacky satoshi op_check_multisig behavior that
// we must perpetuate, though it's appearance here is policy not consensus.
// Limiting to push_size_0 eliminates pattern ambiguity with little downside.
bool script::is_sign_multisig_pattern(const operation::list& ops)
{
    return ops.size() >= 2
        && ops[0].code() == opcode::push_size_0
        && std::all_of(std::next(ops.begin()), ops.end(),
            [](const operation& op)
            {
                return is_endorsement(op.data());
            });
}

bool script::is_sign_public_key_pattern(const operation::list& ops)
{
    return ops.size() == 1
        && is_endorsement(ops[0].data());
}

//*****************************************************************************
// CONSENSUS: this pattern is used to activate bip141 validation rules.
//*****************************************************************************
bool script::is_sign_key_hash_pattern(const operation::list& ops)
{
    return ops.size() == 2
        && is_endorsement(ops[0].data())
        && is_public_key(ops[1].data());
}

// Ambiguous with is_sign_key_hash when second/last op is a public key.
// Ambiguous with is_sign_public_key_pattern when only op is an endorsement.
bool script::is_sign_script_hash_pattern(const operation::list& ops)
{
    return !ops.empty()
        && is_push_only(ops)
        && !ops.back().data().empty();
}

operation::list script::to_pay_null_data_pattern(const data_slice& data)
{
    if (data.size() > max_null_data_size)
        return {};

    return operation::list
    {
        { opcode::return_ },
        { to_chunk(data) }
    };
}

operation::list script::to_pay_public_key_pattern(const data_slice& point)
{
    if (!is_public_key(point))
        return {};

    return operation::list
    {
        { to_chunk(point) },
        { opcode::checksig }
    };
}

operation::list script::to_pay_key_hash_pattern(const short_hash& hash)
{
    return operation::list
    {
        { opcode::dup },
        { opcode::hash160 },
        { to_chunk(hash) },
        { opcode::equalverify },
        { opcode::checksig }
    };
}

operation::list script::to_pay_script_hash_pattern(const short_hash& hash)
{
    return operation::list
    {
        { opcode::hash160 },
        { to_chunk(hash) },
        { opcode::equal }
    };
}

// TODO: limit to 20 for consistency with op_check_multisig.
operation::list script::to_pay_multisig_pattern(uint8_t signatures,
    const compressed_list& points)
{
    return to_pay_multisig_pattern(signatures,
        to_stack<ec_compressed_size>(points));
}

// TODO: expand this to a 20 signature limit.
// This supports up to 16 signatures, however check_multisig is limited to 20.
// The embedded script is limited to 520 bytes, an effective limit of 15 for
// p2sh multisig, which can be as low as 7 when using all uncompressed keys.
operation::list script::to_pay_multisig_pattern(uint8_t signatures,
    const data_stack& points)
{
    static constexpr auto op_81 = static_cast<uint8_t>(opcode::push_positive_1);
    static constexpr auto op_96 = static_cast<uint8_t>(opcode::push_positive_16);
    static constexpr auto zero = op_81 - 1;
    static constexpr auto max = op_96 - zero;

    const auto m = signatures;
    const auto n = points.size();

    if (m < 1 || m > n || n < 1 || n > max)
        return operation::list();

    const auto op_m = static_cast<opcode>(m + zero);
    const auto op_n = static_cast<opcode>(points.size() + zero);

    operation::list ops;
    ops.reserve(points.size() + 3);
    ops.emplace_back(op_m);

    for (const auto& point: points)
    {
        if (!is_public_key(point))
            return {};

        ops.emplace_back(point);
    }

    ops.emplace_back(op_n);
    ops.emplace_back(opcode::checkmultisig);
    return ops;
}

operation::list script::to_pay_witness_pattern(uint8_t version, const data_slice& data)
{
    return
    {
        { operation::opcode_from_version(version) },
        { to_chunk(data) },
    };
}

operation::list script::to_pay_witness_key_hash_pattern(const short_hash& hash)
{
    return
    {
        { opcode::push_size_0 },
        { to_chunk(hash) },
    };
}

operation::list script::to_pay_witness_script_hash_pattern(const hash_digest& hash)
{
    return
    {
        { opcode::push_size_0 },
        { to_chunk(hash) }
    };
}

// Utilities (non-static).
//-----------------------------------------------------------------------------

const data_chunk& script::witness_program() const
{
    static const data_chunk empty;

    // The first operations access must be method-based to guarantee the cache.
    const auto& ops = operations();
    return is_witness_program_pattern(ops) ? ops[1].data() : empty;
}

script_version script::version() const
{
    // The first operations access must be method-based to guarantee the cache.
    const auto& ops = operations();

    if (!is_witness_program_pattern(ops))
        return script_version::unversioned;

    switch (ops[0].code())
    {
        case opcode::push_size_0:
            return script_version::zero;
        default:
            return script_version::reserved;
    }
}

// Caller should test for is_sign_script_hash_pattern when sign_key_hash result
// as it is possible for an input script to match both patterns.
script_pattern script::pattern() const
{
    const auto input = output_pattern();
    return input == script_pattern::non_standard ? input_pattern() : input;
}

// Output patterns are mutually and input unambiguous.
// The bip141 coinbase pattern is not tested here, must test independently.
script_pattern script::output_pattern() const
{
    // The first operations access must be method-based to guarantee the cache.
    if (is_pay_key_hash_pattern(operations()))
        return script_pattern::pay_key_hash;

    if (is_pay_script_hash_pattern(operations_))
        return script_pattern::pay_script_hash;

    if (is_pay_null_data_pattern(operations_))
        return script_pattern::pay_null_data;

    if (is_pay_public_key_pattern(operations_))
        return script_pattern::pay_public_key;

    // Limited to 16 signatures though op_check_multisig allows 20.
    if (is_pay_multisig_pattern(operations_))
        return script_pattern::pay_multisig;

    return script_pattern::non_standard;
}

// A sign_key_hash result always implies sign_script_hash as well.
// The bip34 coinbase pattern is not tested here, must test independently.
script_pattern script::input_pattern() const
{
    // The first operations access must be method-based to guarantee the cache.
    if (is_sign_key_hash_pattern(operations()))
        return script_pattern::sign_key_hash;

    // This must follow is_sign_key_hash_pattern for ambiguity comment to hold.
    if (is_sign_script_hash_pattern(operations_))
        return script_pattern::sign_script_hash;

    if (is_sign_public_key_pattern(operations_))
        return script_pattern::sign_public_key;

    if (is_sign_multisig_pattern(operations_))
        return script_pattern::sign_multisig;

    return script_pattern::non_standard;
}

bool script::is_pay_to_witness(uint32_t forks) const
{
    // This is used internally as an optimization over using script::pattern.
    // The first operations access must be method-based to guarantee the cache.
    return is_enabled(forks, rule_fork::bip141_rule) &&
        is_witness_program_pattern(operations());
}

bool script::is_pay_to_script_hash(uint32_t forks) const
{
    // This is used internally as an optimization over using script::pattern.
    // The first operations access must be method-based to guarantee the cache.
    return is_enabled(forks, rule_fork::bip16_rule) &&
        is_pay_script_hash_pattern(operations());
}

// Count 1..16 multisig accurately for embedded (bip16) and witness (bip141).
inline size_t multisig_sigops(bool accurate, opcode code)
{
    return accurate && operation::is_positive(code) ?
        operation::opcode_to_positive(code) : multisig_default_sigops;
}

size_t script::sigops(bool accurate) const
{
    size_t total = 0;
    auto preceding = opcode::push_negative_1;

    // The first operations access must be method-based to guarantee the cache.
    for (const auto& op: operations())
    {
        const auto code = op.code();

        if (code == opcode::checksig ||
            code == opcode::checksigverify)
        {
            ++total;
        }
        else if (
            code == opcode::checkmultisig ||
            code == opcode::checkmultisigverify)
        {
            total += multisig_sigops(accurate, preceding);
        }

        preceding = code;
    }

    return total;
}

//*****************************************************************************
// CONSENSUS: this is a pointless, broken, premature optimization attempt.
// The comparison and erase are not limited to a single operation and so can
// erase arbitrary upstream data from the script.
//*****************************************************************************
void script::find_and_delete_(const data_chunk& endorsement)
{
    // If this is empty it would produce an empty script but not operation.
    // So we test it for empty prior to operation reserialization.
    if (endorsement.empty())
        return;

    // The value must be serialized to script using non-minimal encoding.
    // Non-minimally-encoded target values will therefore not match.
    const auto value = operation(endorsement, false).to_data();

    operation op;
    read::bytes::copy source(bytes_);
    std::vector<data_chunk::iterator> found;

    // The exhaustion test handles stream end and op deserialization failure.
    for (auto it = bytes_.begin(); !source.is_exhausted();
        it += (source ? op.serialized_size() : zero))
    {
        // Track all found values for later deletion.
        for (; starts_with(it, bytes_.end(), value); it += value.size())
        {
            source.skip_bytes(value.size());
            found.push_back(it);
        }

        // Read the next op code following last found value.
        op.from_data(source);
    }

    // Delete any found values, reversed to prevent iterator invalidation.
    for (const auto& it: boost::adaptors::reverse(found))
        bytes_.erase(it, it + value.size());
}

// Concurrent read/write is not supported, so no critical section.
void script::find_and_delete(const data_stack& endorsements)
{
    for (const auto& endorsement: endorsements)
        find_and_delete_(endorsement);

    // Invalidate the cache so that the operations may be regenerated.
    operations_.clear();
    cached_ = false;
    bytes_.shrink_to_fit();
}

////// This is slightly more efficient because the script does not get parsed,
////// but the static template implementation is more self-explanatory.
////bool script::is_coinbase_pattern(size_t height) const
////{
////    const auto actual = to_data(false);
////
////    // Create the expected script as a non-minimal byte vector.
////    script compare(operation::list{ { number(height).data(), false } });
////    const auto expected = compare.to_data(false);
////
////    // Require the actual script start with the expected coinbase script.
////    return std::equal(expected.begin(), expected.end(), actual.begin());
////}

bool script::is_oversized() const
{
    return serialized_size(false) > max_script_size;
}

// An unspendable script is any that can provably not be spent under any
// circumstance. This allows for exclusion of the output as unspendable.
// The criteria below are not be comprehensive but are fast to evaluate.
bool script::is_unspendable() const
{
    // The first operations access must be method-based to guarantee the cache.
    return !operations().empty() && operations_[0].code() == opcode::return_;
}

// Validation.
//-----------------------------------------------------------------------------

code script::verify(const transaction& tx, uint32_t input_index,
    uint32_t forks, const script& prevout_script, uint64_t value)
{
    if (input_index >= tx.inputs().size())
        return error::operation_failed;

    code ec;
    bool witnessed;
    const auto& in = tx.inputs()[input_index];

    // Evaluate input script.
    program input(in.script(), tx, input_index, forks);
    if ((ec = input.evaluate()))
        return ec;

    // Evaluate output script using stack result from input script.
    program prevout(prevout_script, input);
    if ((ec = prevout.evaluate()))
        return ec;

    // This precludes bare witness programs of -0 (undocumented).
    if (!prevout.stack_result(false))
        return error::stack_false;

    // Triggered by output script push of version and witness program (bip141).
    if ((witnessed = prevout_script.is_pay_to_witness(forks)))
    {
        // The input script must be empty (bip141).
        if (!in.script().empty())
            return error::dirty_witness;

        // Validate the native script.
        if ((ec = in.witness().verify(tx, input_index, forks, prevout_script,
            value)))
            return ec;
    }

    // p2sh and p2w are mutually exclusive.
    else if (prevout_script.is_pay_to_script_hash(forks))
    {
        if (!is_relaxed_push(in.script().operations()))
            return error::invalid_script_embed;

        // Embedded script must be at the top of the stack (bip16).
        script embedded_script(input.pop(), false);

        program embedded(embedded_script, std::move(input), true);
        if ((ec = embedded.evaluate()))
            return ec;

        // This precludes embedded witness programs of -0 (undocumented).
        if (!embedded.stack_result(false))
            return error::stack_false;

        // Triggered by embedded push of version and witness program (bip141).
        if ((witnessed = embedded_script.is_pay_to_witness(forks)))
        {
            // The input script must be a push of the embedded_script (bip141).
            if (in.script().size() != 1)
                return error::dirty_witness;

            // Validate the non-native script.
            if ((ec = in.witness().verify(tx, input_index, forks,
                embedded_script, value)))
                return ec;
        }
    }

    // Witness must be empty if no bip141 or invalid witness program (bip141).
    if (!witnessed && !in.witness().empty())
        return error::unexpected_witness;

    return error::success;
}

code script::verify(const transaction& tx, uint32_t input_index,
    uint32_t forks)
{
    if (input_index >= tx.inputs().size())
        return error::operation_failed;

    const auto& in = tx.inputs()[input_index];
    const auto& prevout = in.previous_output().metadata.cache;
    return verify(tx, input_index, forks, prevout.script(), prevout.value());
}

} // namespace chain
} // namespace system
} // namespace libbitcoin
