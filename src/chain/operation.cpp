/**
 * Copyright (c) 2011-2021 libbitcoin developers (see AUTHORS)
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
#include <bitcoin/system/chain/operation.hpp>

#include <cstdint>
#include <bitcoin/system/assert.hpp>
#include <bitcoin/system/chain/enums/numbers.hpp>
#include <bitcoin/system/chain/enums/opcode.hpp>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/machine/machine.hpp>

namespace libbitcoin {
namespace system {
namespace chain {

static constexpr auto any_invalid_code = opcode::disabled_xor;

// Constructors.
//-----------------------------------------------------------------------------

operation::operation()
  : code_(any_invalid_code), valid_(false)
{
    // The failed-state code must be disabled so it will never pass evaluation.
    BITCOIN_ASSERT(is_disabled());
}

operation::operation(operation&& other)
  : operation(other.code_, std::move(other.data_), other.valid_)
{
}

operation::operation(const operation& other)
  : operation(other.code_, other.data_, other.valid_)
{
}

operation::operation(data_chunk&& uncoded, bool minimal)
  : code_(opcode_from_data(uncoded, minimal)),
    data_(std::move(uncoded)),
    valid_(!is_oversized())
{
    if (!valid_)
        reset();

    // Revert data if opcode_from_data produced a numeric encoding.
    if (minimal && !is_payload(code_))
    {
        data_.clear();
        data_.shrink_to_fit();
    }
}

operation::operation(const data_chunk& uncoded, bool minimal)
  : code_(opcode_from_data(uncoded, minimal)),
    data_(uncoded),
    valid_(!is_oversized())
{
    if (!valid_)
        reset();

    // Revert data if opcode_from_data produced a numeric encoding.
    if (minimal && !is_payload(code_))
    {
        data_.clear();
        data_.shrink_to_fit();
    }
}

operation::operation(opcode code)
  : code_(code), valid_(true)
{
}

// protected
operation::operation(opcode code, data_chunk&& data, bool valid)
  : code_(code), data_(std::move(data)), valid_(valid)
{
}

// protected
operation::operation(opcode code, const data_chunk& data, bool valid)
  : code_(code), data_(data), valid_(valid)
{
}

// Operators.
//-----------------------------------------------------------------------------

operation& operation::operator=(operation&& other)
{
    code_ = other.code_;
    data_ = std::move(other.data_);
    valid_ = other.valid_;
    return *this;
}

operation& operation::operator=(const operation& other)
{
    code_ = other.code_;
    data_ = other.data_;
    valid_ = other.valid_;
    return *this;
}

bool operation::operator==(const operation& other) const
{
    return (code_ == other.code_) && (data_ == other.data_);
}

bool operation::operator!=(const operation& other) const
{
    return !(*this == other);
}

// Properties (size, accessors, cache).
//-----------------------------------------------------------------------------

size_t operation::serialized_size() const
{
    static constexpr auto op_size = sizeof(uint8_t);
    const auto size = data_.size();

    switch (code_)
    {
        case opcode::push_one_size:
            return op_size + sizeof(uint8_t) + size;
        case opcode::push_two_size:
            return op_size + sizeof(uint16_t) + size;
        case opcode::push_four_size:
            return op_size + sizeof(uint32_t) + size;
        default:
            return op_size + size;
    }
}

opcode operation::code() const
{
    return code_;
}

const data_chunk& operation::data() const
{
    return data_;
}

// Utilities.
//-----------------------------------------------------------------------------

// private
//*****************************************************************************
// CONSENSUS: op data size is limited to 520 bytes, which requires no more
// than two bytes to encode. However the four byte encoding can represent
// a value of any size, so remains valid despite the data size limit.
//*****************************************************************************
uint32_t operation::read_data_size(opcode code, reader& source)
{
    constexpr auto op_75 = static_cast<uint8_t>(opcode::push_size_75);

    switch (code)
    {
        case opcode::push_one_size:
            return source.read_byte();
        case opcode::push_two_size:
            return source.read_2_bytes_little_endian();
        case opcode::push_four_size:
            return source.read_4_bytes_little_endian();
        default:
            const auto byte = static_cast<uint8_t>(code);
            return byte <= op_75 ? byte : 0;
    }
}

//*****************************************************************************
// CONSENSUS: this is non-minial but consensus critical due to find_and_delete.
// Presumably this was just an oversight in the original script encoding.
//*****************************************************************************
opcode operation::opcode_from_size(size_t size)
{
    BITCOIN_ASSERT(size <= max_uint32);
    constexpr auto op_75 = static_cast<uint8_t>(opcode::push_size_75);

    if (size <= op_75)
        return static_cast<opcode>(size);
    else if (size <= max_uint8)
        return opcode::push_one_size;
    else if (size <= max_uint16)
        return opcode::push_two_size;
    else
        return opcode::push_four_size;
}

opcode operation::minimal_opcode_from_data(const data_chunk& data)
{
    const auto size = data.size();

    if (size == 1)
    {
        const auto value = data.front();

        if (value == numbers::negative_1)
            return opcode::push_negative_1;

        if (value == numbers::positive_0)
            return opcode::push_size_0;

        if (value >= numbers::positive_1 &&
            value <= numbers::positive_16)
            return opcode_from_positive(value);
    }

    // Nominal encoding is minimal for multiple bytes and non-numeric values.
    return opcode_from_size(size);
}

opcode operation::nominal_opcode_from_data(const data_chunk& data)
{
    return opcode_from_size(data.size());
}

opcode operation::opcode_from_version(uint8_t value)
{
    BITCOIN_ASSERT(value <= numbers::positive_16);
    return (value == numbers::positive_0) ? opcode::push_size_0 :
        operation::opcode_from_positive(value);
}

opcode operation::opcode_from_data(const data_chunk& data,
    bool minimal)
{
    return minimal ? minimal_opcode_from_data(data) :
        nominal_opcode_from_data(data);
}

opcode operation::opcode_from_positive(uint8_t value)
{
    BITCOIN_ASSERT(value >= number::positive_1);
    BITCOIN_ASSERT(value <= number::positive_16);
    constexpr auto op_81 = static_cast<uint8_t>(opcode::push_positive_1);
    return static_cast<opcode>(value + op_81 - 1);
}

uint8_t operation::opcode_to_positive(opcode code)
{
    BITCOIN_ASSERT(is_positive(code));
    constexpr auto op_81 = static_cast<uint8_t>(opcode::push_positive_1);
    return static_cast<uint8_t>(code) - op_81 + 1;
}

// opcode: [0-79, 81-96]
bool operation::is_push(opcode code)
{
    constexpr auto op_80 = static_cast<uint8_t>(opcode::reserved_80);
    constexpr auto op_96 = static_cast<uint8_t>(opcode::push_positive_16);
    const auto value = static_cast<uint8_t>(code);
    return value <= op_96 && value != op_80;
}

// opcode: [1-78]
bool operation::is_payload(opcode code)
{
    constexpr auto op_1 = static_cast<uint8_t>(opcode::push_size_1);
    constexpr auto op_78 = static_cast<uint8_t>(opcode::push_four_size);
    const auto value = static_cast<uint8_t>(code);
    return value >= op_1 && value <= op_78;
}

// opcode: [97-255]
bool operation::is_counted(opcode code)
{
    constexpr auto op_97 = static_cast<uint8_t>(opcode::nop);
    const auto value = static_cast<uint8_t>(code);
    return value >= op_97;
}

// stack: [[], 1-16]
bool operation::is_version(opcode code)
{
    return code == opcode::push_size_0 || is_positive(code);
}

// stack: [-1, 1-16]
bool operation::is_numeric(opcode code)
{
    return is_positive(code) || code == opcode::push_negative_1;
}

// stack: [1-16]
bool operation::is_positive(opcode code)
{
    constexpr auto op_81 = static_cast<uint8_t>(opcode::push_positive_1);
    constexpr auto op_96 = static_cast<uint8_t>(opcode::push_positive_16);
    const auto value = static_cast<uint8_t>(code);
    return value >= op_81 && value <= op_96;
}

// opcode: [80, 98, 137, 138, 186-255]
bool operation::is_reserved(opcode code)
{
    constexpr auto op_186 = static_cast<uint8_t>(opcode::reserved_186);

    switch (code)
    {
        case opcode::reserved_80:
        case opcode::reserved_98:
        case opcode::reserved_137:
        case opcode::reserved_138:
            return true;
        default:
            return static_cast<uint8_t>(code) >= op_186;
    }
}

//*****************************************************************************
// CONSENSUS: the codes VERIF and VERNOTIF are in the conditional range yet are
// not handled. As a result satoshi always processes them in the op switch.
// This causes them to always fail as unhandled. It is misleading that the
// satoshi test cases refer to these as reserved codes. These two codes behave
// exactly as the explicitly disabled codes. On the other hand VER is not within
// the satoshi conditional range test so it is in fact reserved. Presumably
// this was an unintended consequence of range testing enums.
//*****************************************************************************
bool operation::is_disabled(opcode code)
{
    switch (code)
    {
        case opcode::disabled_cat:
        case opcode::disabled_substr:
        case opcode::disabled_left:
        case opcode::disabled_right:
        case opcode::disabled_invert:
        case opcode::disabled_and:
        case opcode::disabled_or:
        case opcode::disabled_xor:
        case opcode::disabled_mul2:
        case opcode::disabled_div2:
        case opcode::disabled_mul:
        case opcode::disabled_div:
        case opcode::disabled_mod:
        case opcode::disabled_lshift:
        case opcode::disabled_rshift:
        case opcode::disabled_verif:
        case opcode::disabled_vernotif:
            return true;
        default:
            return false;
    }
}

//*****************************************************************************
// CONSENSUS: in order to properly treat VERIF and VERNOTIF as disabled (see
// is_disabled comments) those codes must not be included here.
//*****************************************************************************
bool operation::is_conditional(opcode code)
{
    switch (code)
    {
        case opcode::if_:
        case opcode::notif:
        case opcode::else_:
        case opcode::endif:
            return true;
        default:
            return false;
    }
}

//*****************************************************************************
// CONSENSUS: this test explicitly includes the satoshi 'reserved' code.
// This affects the operation count in p2sh script evaluation.
// Presumably this was an unintended consequence of range testing enums.
//*****************************************************************************
// opcode: [0-96]
bool operation::is_relaxed_push(opcode code)
{
    constexpr auto op_96 = static_cast<uint8_t>(opcode::push_positive_16);
    const auto value = static_cast<uint8_t>(code);
    return value <= op_96;
}

bool operation::is_push() const
{
    return is_push(code_);
}

bool operation::is_counted() const
{
    return is_counted(code_);
}

bool operation::is_version() const
{
    return is_version(code_);
}

bool operation::is_positive() const
{
    return is_positive(code_);
}

bool operation::is_disabled() const
{
    return is_disabled(code_);
}

bool operation::is_conditional() const
{
    return is_conditional(code_);
}

bool operation::is_relaxed_push() const
{
    return is_relaxed_push(code_);
}

bool operation::is_oversized() const
{
    // bit.ly/2eSDkOJ
    return data_.size() > max_push_data_size;
}

bool operation::is_minimal_push() const
{
    return code_ == minimal_opcode_from_data(data_);
}

bool operation::is_nominal_push() const
{
    return code_ == nominal_opcode_from_data(data_);
}

// Deserialization.
//-----------------------------------------------------------------------------

// static
operation operation::factory(const data_chunk& encoded)
{
    operation instance;
    instance.from_data(encoded);
    return instance;
}

// static
operation operation::factory(std::istream& stream)
{
    operation instance;
    instance.from_data(stream);
    return instance;
}

// static
operation operation::factory(reader& source)
{
    operation instance;
    instance.from_data(source);
    return instance;
}

bool operation::from_data(const data_chunk& encoded)
{
    stream::in::copy istream(encoded);
    return from_data(istream);
}

bool operation::from_data(std::istream& stream)
{
    read::bytes::istream source(stream);
    return from_data(source);
}

// TODO: optimize for larger data by using a shared byte array.
bool operation::from_data(reader& source)
{
    ////reset();
    valid_ = true;
    code_ = static_cast<opcode>(source.read_byte());
    const auto size = read_data_size(code_, source);

    // The max_script_size and max_push_data_size constants limit
    // evaluation, but not all scripts evaluate, so use max_block_size
    // to guard memory allocation here.
    if (size > max_block_size)
        source.invalidate();
    else
        data_ = source.read_bytes(size);

    if (!source)
        reset();

    return valid_;
}

inline bool is_push_token(const std::string& token)
{
    return token.size() > 1 && token.front() == '[' && token.back() == ']';
}

inline bool is_text_token(const std::string& token)
{
    return token.size() > 1 && token.front() == '\'' && token.back() == '\'';
}

inline bool is_valid_data_size(opcode code, size_t size)
{
    constexpr auto op_75 = static_cast<uint8_t>(opcode::push_size_75);
    const auto value = static_cast<uint8_t>(code);
    return value > op_75 || value == size;
}

inline std::string trim_token(const std::string& token)
{
    BITCOIN_ASSERT(token.size() > 1);
    return std::string(std::next(token.begin()), std::prev(token.end()));
}

inline string_list split_push_token(const std::string& token)
{
    return split(trim_token(token), ".", false, false);
}

static bool opcode_from_data_prefix(opcode& out_code,
    const std::string& prefix, const data_chunk& data)
{
    constexpr auto op_75 = static_cast<uint8_t>(opcode::push_size_75);
    const auto size = data.size();
    out_code = operation::opcode_from_size(size);

    if (prefix == "0")
    {
        return size <= op_75;
    }
    else if (prefix == "1")
    {
        out_code = opcode::push_one_size;
        return size <= max_uint8;
    }
    else if (prefix == "2")
    {
        out_code = opcode::push_two_size;
        return size <= max_uint16;
    }
    else if (prefix == "4")
    {
        out_code = opcode::push_four_size;
        return size <= max_uint32;
    }

    return false;
}

static bool data_from_number_token(data_chunk& out_data,
    const std::string& token)
{
    int64_t value;
    if (!deserialize(value, token))
        return false;

    out_data = machine::number(value).data();
    return true;
}

// The removal of spaces in v3 data is a compatibility break with our v2.
bool operation::from_string(const std::string& mnemonic)
{
    reset();

    if (is_push_token(mnemonic))
    {
        // Data encoding uses single token (with optional non-minimality).
        const auto parts = split_push_token(mnemonic);

        if (parts.size() == 1)
        {
            // Extract operation using nominal data size encoding.
            if (decode_base16(data_, parts.front()))
            {
                code_ = nominal_opcode_from_data(data_);
                valid_ = true;
            }
        }
        else if (parts.size() == 2)
        {
            // Extract operation using explicit data size encoding.
            valid_ = decode_base16(data_, parts[1]) &&
                opcode_from_data_prefix(code_, parts[0], data_);
        }
    }
    else if (is_text_token(mnemonic))
    {
        const auto text = trim_token(mnemonic);
        data_ = data_chunk{ text.begin(), text.end() };
        code_ = nominal_opcode_from_data(data_);
        valid_ = true;
    }
    else if (opcode_from_string(code_, mnemonic))
    {
        // push_one_size, push_two_size and push_four_size succeed with empty.
        // push_size_1 through push_size_75 always fail because they are empty.
        valid_ = is_valid_data_size(code_, data_.size());
    }
    else if (data_from_number_token(data_, mnemonic))
    {
        // [-1, 0, 1..16] integers captured by opcode_from_string, others here.
        // Otherwise minimal_opcode_from_data could convert integers here.
        code_ = nominal_opcode_from_data(data_);
        valid_ = true;
    }

    if (!valid_)
        reset();

    return valid_;
}

bool operation::is_valid() const
{
    return valid_;
}

// protected
void operation::reset()
{
    code_ = any_invalid_code;
    data_.clear();
    valid_ = false;
}

// Serialization.
//-----------------------------------------------------------------------------

data_chunk operation::to_data() const
{
    data_chunk data(no_fill_allocator);
    data.resize(serialized_size());
    stream::out::copy ostream(data);
    to_data(ostream);
    return data;
}

void operation::to_data(std::ostream& stream) const
{
    write::bytes::ostream out(stream);
    to_data(out);
}

void operation::to_data(writer& sink) const
{
    const auto size = data_.size();

    sink.write_byte(static_cast<uint8_t>(code_));

    switch (code_)
    {
    case opcode::push_one_size:
        sink.write_byte(static_cast<uint8_t>(size));
        break;
    case opcode::push_two_size:
        sink.write_2_bytes_little_endian(static_cast<uint16_t>(size));
        break;
    case opcode::push_four_size:
        sink.write_4_bytes_little_endian(static_cast<uint32_t>(size));
        break;
    default:
        break;
    }

    sink.write_bytes(data_);
}

static std::string opcode_to_prefix(opcode code, const data_chunk& data)
{
    // If opcode is minimal for a size-based encoding, do not set a prefix.
    if (code == operation::opcode_from_size(data.size()))
        return "";

    switch (code)
    {
    case opcode::push_one_size:
        return "1.";
    case opcode::push_two_size:
        return "2.";
    case opcode::push_four_size:
        return "4.";
    default:
        return "0.";
    }
}

// The removal of spaces in v3 data is a compatibility break with our v2.
std::string operation::to_string(uint32_t active_forks) const
{
    if (!valid_)
        return "<invalid>";

    if (data_.empty())
        return opcode_to_string(code_, active_forks);

    // Data encoding uses single token with explicit size prefix as required.
    return "[" + opcode_to_prefix(code_, data_) + encode_base16(data_) + "]";
}

} // namespace chain
} // namespace system
} // namespace libbitcoin
