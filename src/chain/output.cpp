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
#include <bitcoin/system/chain/output.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <bitcoin/system/chain/enums/magic_numbers.hpp>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/stream/stream.hpp>

// If required change to bc::checked.
////#include <bitcoin/system/wallet/addresses/payment_address.hpp>

namespace libbitcoin {
namespace system {
namespace chain {

// This is a consensus critical value that must be set on reset.
const uint64_t output::not_found = sighash_null_value;

// These are non-consensus sentinel values used by the store.
const uint32_t output::validation::not_spent = max_uint32;
const uint8_t output::validation::candidate_spent_true = 1;
const uint8_t output::validation::candidate_spent_false = 0;

// Constructors.
//-----------------------------------------------------------------------------

output::output()
  : metadata{},
    value_(not_found),
    script_{}
{
}

output::output(output&& other)
  : metadata(other.metadata),
    ////addresses_(other.addresses_cache()),
    value_(other.value_),
    script_(std::move(other.script_))
{
}

output::output(const output& other)
  : metadata(other.metadata),
    ////addresses_(other.addresses_cache()),
    value_(other.value_),
    script_(other.script_)
{
}

output::output(uint64_t value, chain::script&& script)
  : metadata{},
    value_(value),
    script_(std::move(script))
{
}

output::output(uint64_t value, const chain::script& script)
  : metadata{},
    value_(value),
    script_(script)
{
}

////// Private cache access for copy/move construction.
////output::addresses_ptr output::addresses_cache() const
////{
////    shared_lock lock(mutex_);
////    return addresses_;
////}

// Operators.
//-----------------------------------------------------------------------------

output& output::operator=(output&& other)
{
    ////addresses_ = other.addresses_cache();
    value_ = other.value_;
    script_ = std::move(other.script_);
    metadata = std::move(other.metadata);
    return *this;
}

output& output::operator=(const output& other)
{
    ////addresses_ = other.addresses_cache();
    value_ = other.value_;
    script_ = other.script_;
    metadata = other.metadata;
    return *this;
}

bool output::operator==(const output& other) const
{
    return (value_ == other.value_) && (script_ == other.script_);
}

bool output::operator!=(const output& other) const
{
    return !(*this == other);
}

// Deserialization.
//-----------------------------------------------------------------------------

output output::factory(const data_chunk& data, bool wire)
{
    output instance;
    instance.from_data(data, wire);
    return instance;
}

output output::factory(std::istream& stream, bool wire)
{
    output instance;
    instance.from_data(stream, wire);
    return instance;
}

output output::factory(reader& source, bool wire)
{
    output instance;
    instance.from_data(source, wire);
    return instance;
}

bool output::from_data(const data_chunk& data, bool wire)
{
    stream::in::copy istream(data);
    return from_data(istream, wire);
}

bool output::from_data(std::istream& stream, bool wire)
{
    read::bytes::istream source(stream);
    return from_data(source, wire);
}

bool output::from_data(reader& source, bool wire, bool)
{
    reset();

    if (!wire)
    {
        const auto spent = source.read_byte() == 
            output::validation::candidate_spent_true;

        // These read updateable data in a non-atomic manner.
        // The results are unusable unless externally protected.
        metadata.candidate_spent = spent;
        metadata.confirmed_spent_height = source.read_4_bytes_little_endian();
    }

    value_ = source.read_8_bytes_little_endian();
    script_.from_data(source, true);

    if (!source)
        reset();

    return source;
}

// protected
void output::reset()
{
    value_ = output::not_found;
    script_.reset();
}

// Empty scripts are valid, validation relies on not_found only.
bool output::is_valid() const
{
    return value_ != output::not_found;
}

// Serialization.
//-----------------------------------------------------------------------------

data_chunk output::to_data(bool wire) const
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

void output::to_data(std::ostream& stream, bool wire) const
{
    write::bytes::ostream out(stream);
    to_data(out, wire);
}

void output::to_data(writer& sink, bool wire, bool) const
{
    if (!wire)
    {
        const auto spent = metadata.candidate_spent ?
            output::validation::candidate_spent_true :
            output::validation::candidate_spent_false;

        // These writes are only utilized for unreachable tx serialization.
        // Later updates and usable reads must be externally protected.
        sink.write_byte(spent);
        sink.write_4_bytes_little_endian(metadata.confirmed_spent_height);
    }

    sink.write_8_bytes_little_endian(value_);
    script_.to_data(sink, true);
}

// Size.
//-----------------------------------------------------------------------------

size_t output::serialized_size(bool wire) const
{
    const auto metadata = wire ? 0 : sizeof(uint32_t) + sizeof(uint8_t);
    return metadata + sizeof(value_) + script_.serialized_size(true);
}

// Accessors.
//-----------------------------------------------------------------------------

uint64_t output::value() const
{
    return value_;
}

void output::set_value(uint64_t value)
{
    value_ = value;
}

const chain::script& output::script() const
{
    return script_;
}

void output::set_script(const chain::script& value)
{
    script_ = value;
    ////invalidate_cache();
}

void output::set_script(chain::script&& value)
{
    script_ = std::move(value);
    ////invalidate_cache();
}

////// protected
////void output::invalidate_cache() const
////{
////    ///////////////////////////////////////////////////////////////////////////
////    // Critical Section
////    mutex_.lock_upgrade();
////
////    if (addresses_)
////    {
////        mutex_.unlock_upgrade_and_lock();
////        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////        addresses_.reset();
////        //---------------------------------------------------------------------
////        mutex_.unlock_and_lock_upgrade();
////    }
////
////    mutex_.unlock_upgrade();
////    ///////////////////////////////////////////////////////////////////////////
////}

////payment_address output::address(uint8_t p2kh_version,
////    uint8_t p2sh_version) const
////{
////    const auto value = addresses(p2kh_version, p2sh_version);
////    return value.empty() ? payment_address{} : value.front();
////}

////payment_address::list output::addresses(uint8_t p2kh_version,
////    uint8_t p2sh_version) const
////{
////    ///////////////////////////////////////////////////////////////////////////
////    // Critical Section
////    mutex_.lock_upgrade();
////
////    if (!addresses_)
////    {
////        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////        mutex_.unlock_upgrade_and_lock();
////        addresses_ = std::make_shared<payment_address::list>(
////            payment_address::extract_output(script_, p2kh_version,
////                p2sh_version));
////        mutex_.unlock_and_lock_upgrade();
////        //---------------------------------------------------------------------
////    }
////
////    const auto addresses = *addresses_;
////    mutex_.unlock_upgrade();
////    ///////////////////////////////////////////////////////////////////////////
////
////    return addresses;
////}

// Validation helpers.
//-----------------------------------------------------------------------------

size_t output::signature_operations(bool bip141) const
{
    // Penalize quadratic signature operations (bip141).
    const auto sigops_factor = bip141 ? fast_sigops_factor : 1u;

    // Count heavy sigops in the output script.
    return script_.sigops(false) * sigops_factor;
}

bool output::is_dust(uint64_t minimum_value) const
{
    // If provably unspendable it does not expand the unspent output set.
    return value_ < minimum_value && !script_.is_unspendable();
}

bool output::extract_committed_hash(hash_digest& out) const
{
    const auto& ops = script_.operations();

    if (!script::is_commitment_pattern(ops))
        return false;

    // The four byte offset for the witness commitment hash (bip141).
    const auto start = ops[1].data().begin() + sizeof(witness_head);
    std::copy_n(start, hash_size, out.begin());
    return true;
}

} // namespace chain
} // namespace system
} // namespace libbitcoin
