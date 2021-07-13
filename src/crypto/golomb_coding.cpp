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

// Sponsored in part by Digital Contract Design, LLC

#include <bitcoin/system/crypto/golomb_coding.hpp>

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <vector>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/data/data.hpp>

// Avoid in header, circular dependency with stream to crypto.
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace golomb {

static void construct(bitwriter& sink, const data_stack& items, uint8_t bits,
    const half_hash& entropy, uint64_t target_false_positive_rate);
static void construct(bitwriter& sink, const data_stack& items, uint8_t bits,
    const siphash_key& entropy, uint64_t target_false_positive_rate);
static bool match(const data_chunk& target, bitreader& compressed_set,
    uint64_t set_size, const half_hash& entropy, uint8_t bits,
    uint64_t target_false_positive_rate);
static bool match(const data_chunk& target, bitreader& compressed_set,
    uint64_t set_size, const siphash_key& entropy, uint8_t bits,
    uint64_t target_false_positive_rate);
static bool match(const data_stack& targets, bitreader& compressed_set,
    uint64_t set_size, const half_hash& entropy, uint8_t bits,
    uint64_t target_false_positive_rate);
static bool match(const data_stack& targets, bitreader& compressed_set,
    uint64_t set_size, const siphash_key& entropy, uint8_t bits,
    uint64_t target_false_positive_rate);

static void encode(bitwriter& sink, uint64_t value, uint8_t modulo_exponent)
{
    const uint64_t quotient = value >> modulo_exponent;
    for (uint64_t index = 0; index < quotient; index++)
        sink.write_bit(true);

    sink.write_bit(false);
    sink.write_bits(value, modulo_exponent);
}

static uint64_t decode(bitreader& source, uint8_t modulo_exponent)
{
    uint64_t quotient = 0;
    while (source.read_bit())
        quotient++;

    const auto remainder = source.read_bits(modulo_exponent);
    return ((quotient << modulo_exponent) + remainder);
}

static uint64_t hash_to_range(const data_slice& item, uint64_t bound,
    const siphash_key& key)
{
    const auto product = uint128_t(siphash(key, item)) * uint128_t(bound);
    return (product >> width<uint64_t>()).convert_to<uint64_t>();
}

static std::vector<uint64_t> hashed_set_construct(const data_stack& items,
    uint64_t set_size, uint64_t target_false_positive_rate,
    const siphash_key& key)
{
    const auto bound = safe_multiply(target_false_positive_rate, set_size);
    std::vector<uint64_t> hashes(items.size(), no_fill_allocator);

    std::transform(items.begin(), items.end(), hashes.begin(),
        [&](const data_chunk& item)
        {
            return hash_to_range(item, bound, key);
        });

    return std::move(sort(hashes));
}

// Golomb-coded set construction
// ----------------------------------------------------------------------------

data_chunk construct(const data_stack& items, uint8_t bits,
    const half_hash& entropy, uint64_t target_false_positive_rate)
{
    return construct(items, bits, to_siphash_key(entropy),
        target_false_positive_rate);
}

data_chunk construct(const data_stack& items, uint8_t bits,
    const siphash_key& entropy, uint64_t target_false_positive_rate)
{
    data_chunk result;
    stream::out::data stream(result);
    construct(stream, items, bits, entropy, target_false_positive_rate);
    stream.flush();
    return result;
}

void construct(std::ostream& stream, const data_stack& items, uint8_t bits,
    const half_hash& entropy, uint64_t target_false_positive_rate)
{
    construct(stream, items, bits, to_siphash_key(entropy),
        target_false_positive_rate);
}

void construct(std::ostream& stream, const data_stack& items, uint8_t bits,
    const siphash_key& entropy, uint64_t target_false_positive_rate)
{
    write::bits::ostream sink(stream);
    construct(sink, items, bits, entropy, target_false_positive_rate);
    sink.flush();
}

static void construct(bitwriter& sink, const data_stack& items, uint8_t bits,
    const half_hash& entropy, uint64_t target_false_positive_rate)
{
    construct(sink, items, bits, to_siphash_key(entropy),
        target_false_positive_rate);
}

static void construct(bitwriter& sink, const data_stack& items, uint8_t bits,
    const siphash_key& entropy, uint64_t target_false_positive_rate)
{
    const auto set = hashed_set_construct(items, items.size(),
        target_false_positive_rate, entropy);

    uint64_t previous = 0;
    for (auto value: set)
    {
        encode(sink, value - previous, bits);
        previous = value;
    };
}

// Single element match
// ----------------------------------------------------------------------------

bool match(const data_chunk& target, const data_chunk& compressed_set,
    uint64_t set_size, const half_hash& entropy, uint8_t bits,
    uint64_t target_false_positive_rate)
{
    return match(target, compressed_set, set_size, to_siphash_key(entropy),
        bits, target_false_positive_rate);
}

bool match(const data_chunk& target, const data_chunk& compressed_set,
    uint64_t set_size, const siphash_key& entropy, uint8_t bits,
    uint64_t target_false_positive_rate)
{
    stream::in::copy source(compressed_set);
    return match(target, source, set_size, entropy, bits,
        target_false_positive_rate);
}

bool match(const data_chunk& target, std::istream& compressed_set,
    uint64_t set_size, const half_hash& entropy, uint8_t bits,
    uint64_t target_false_positive_rate)
{
    return match(target, compressed_set, set_size, to_siphash_key(entropy),
        bits, target_false_positive_rate);
}

bool match(const data_chunk& target, std::istream& compressed_set,
    uint64_t set_size, const siphash_key& entropy, uint8_t bits,
    uint64_t target_false_positive_rate)
{
    read::bits::istream reader(compressed_set);
    return match(target, reader, set_size, entropy, bits,
        target_false_positive_rate);
}

static bool match(const data_chunk& target, bitreader& compressed_set,
    uint64_t set_size, const half_hash& entropy, uint8_t bits,
    uint64_t target_false_positive_rate)
{
    return match(target, compressed_set, set_size, to_siphash_key(entropy),
        bits, target_false_positive_rate);
}

static bool match(const data_chunk& target, bitreader& compressed_set,
    uint64_t set_size, const siphash_key& entropy, uint8_t bits,
    uint64_t target_false_positive_rate)
{
    const auto bound = target_false_positive_rate * set_size;
    const auto range = hash_to_range(target, bound, entropy);

    uint64_t previous = 0;
    for (uint64_t index = 0; index < set_size; index++)
    {
        const auto value = previous + decode(compressed_set, bits);

        if (value == range)
            return true;

        if (value > range)
            break;

        previous = value;
    }

    return false;
}

// Intersection match
// ----------------------------------------------------------------------------

bool match(const data_stack& targets, const data_chunk& compressed_set,
    uint64_t set_size, const half_hash& entropy, uint8_t bits,
    uint64_t target_false_positive_rate)
{
    return match(targets, compressed_set, set_size, to_siphash_key(entropy),
        bits, target_false_positive_rate);
}

bool match(const data_stack& targets, const data_chunk& compressed_set,
    uint64_t set_size, const siphash_key& entropy, uint8_t bits,
    uint64_t target_false_positive_rate)
{
    stream::in::copy source(compressed_set);
    return match(targets, source, set_size, entropy, bits,
        target_false_positive_rate);
}

bool match(const data_stack& targets, std::istream& compressed_set,
    uint64_t set_size, const half_hash& entropy, uint8_t bits,
    uint64_t target_false_positive_rate)
{
    return match(targets, compressed_set, set_size, to_siphash_key(entropy),
        bits, target_false_positive_rate);
}

bool match(const data_stack& targets, std::istream& compressed_set,
    uint64_t set_size, const siphash_key& entropy, uint8_t bits,
    uint64_t target_false_positive_rate)
{
    read::bits::istream reader(compressed_set);
    return match(targets, reader, set_size, entropy, bits,
        target_false_positive_rate);
}

static bool match(const data_stack& targets, bitreader& compressed_set,
    uint64_t set_size, const half_hash& entropy, uint8_t bits,
    uint64_t target_false_positive_rate)
{
    return match(targets, compressed_set, set_size, to_siphash_key(entropy),
        bits, target_false_positive_rate);
}

static bool match(const data_stack& targets, bitreader& compressed_set,
    uint64_t set_size, const siphash_key& entropy, uint8_t bits,
    uint64_t target_false_positive_rate)
{
    if (targets.empty())
        return false;

    const auto set = hashed_set_construct(targets, set_size,
        target_false_positive_rate, entropy);

    uint64_t range = 0;
    auto it = set.begin();

    for (uint64_t index = 0; index < set_size && it != set.end(); index++)
    {
        range += decode(compressed_set, bits);

        for (auto value = *it; it != set.end(); value = *(it++))
        {
            if (value == range)
                return true;

            if (value > range)
                break;
        }
    }

    return false;
}

} // namespace golomb
} // namespace system
} // namespace libbitcoin
