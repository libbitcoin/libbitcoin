/**
 * Copyright (c) 2011-2017 libbitcoin developers (see AUTHORS)
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
#include <bitcoin/bitcoin/machine/operation.hpp>

#include <string>
#include <bitcoin/bitcoin/formats/base_16.hpp>
#include <bitcoin/bitcoin/machine/opcode.hpp>
#include <bitcoin/bitcoin/utility/assert.hpp>
#include <bitcoin/bitcoin/utility/container_sink.hpp>
#include <bitcoin/bitcoin/utility/data.hpp>
#include <bitcoin/bitcoin/utility/container_source.hpp>
#include <bitcoin/bitcoin/utility/istream_reader.hpp>
#include <bitcoin/bitcoin/utility/ostream_writer.hpp>
#include <bitcoin/bitcoin/utility/string.hpp>

namespace libbitcoin {
namespace machine {

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
    data_source istream(encoded);
    return from_data(istream);
}

bool operation::from_data(std::istream& stream)
{
    istream_reader source(stream);
    return from_data(source);
}

// TODO: optimize for larger data by using a shared byte array.
bool operation::from_data(reader& source)
{
    reset();

    valid_ = true;
    code_ = static_cast<opcode>(source.read_byte());
    const auto size = read_data_size(code_, source);

    // Guard against potential for arbitary memory allocation.
    if (size > max_push_data_size)
        source.invalidate();
    else if (size != 0)
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


inline std::string trim_token(const std::string& token)
{
    BITCOIN_ASSERT(token.size() > 1);
    return std::string(token.begin() + 1, token.end() - 1);
}

inline string_list split_push_token(const std::string& token)
{
    return split(trim_token(token), ".", false);
}

static bool is_valid_data_size(opcode code, size_t size)
{
    BC_CONSTEXPR auto op_75 = static_cast<uint8_t>(opcode::push_size_75);
    const auto value = static_cast<uint8_t>(code);
    return value > op_75 || value == size;
}

static bool opcode_from_data_prefix(opcode& out_code,
    const std::string& prefix, const data_chunk& data)
{
    BC_CONSTEXPR auto op_75 = static_cast<uint8_t>(opcode::push_size_75);
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

// The removal of spaces in v3 data is a compatability break with our v2.
bool operation::from_string(const std::string& mnemonic)
{
    reset();

    if (is_push_token(mnemonic))
    {
        // Data encoding uses single token (with optional non-minimality).
        const auto parts = split_push_token(mnemonic);

        if (parts.size() == 1)
        {
            // Extract operation using nominal data encoding.
            if (decode_base16(data_, parts[0]) && !is_oversized())
            {
                code_ = nominal_opcode_from_data(data_);
                valid_ = true;
            }
        }
        else if (parts.size() == 2)
        {
            // Extract operation using explicit data size encoding.
            valid_ = decode_base16(data_, parts[1]) && !is_oversized() &&
                opcode_from_data_prefix(code_, parts[0], data_);
        }
    }
    else if (is_text_token(mnemonic))
    {
        const auto text = trim_token(mnemonic);
        data_ = data_chunk{ text.begin(), text.end() };
        code_ = nominal_opcode_from_data(data_);
        valid_ = !is_oversized();
    }
    else if (opcode_from_string(code_, mnemonic))
    {
        // push_one_size, push_two_size and push_four_size succeed with empty.
        // push_size_1 through push_size_75 always fail because they are empty.
        valid_ = is_valid_data_size(code_, data_.size());
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
    code_ = invalid_code;
    data_.clear();
    valid_ = false;
}

// Serialization.
//-----------------------------------------------------------------------------

data_chunk operation::to_data() const
{
    data_chunk data;
    data.reserve(serialized_size());
    data_sink ostream(data);
    to_data(ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == serialized_size());
    return data;
}

void operation::to_data(std::ostream& stream) const
{
    ostream_writer sink(stream);
    to_data(sink);
}

void operation::to_data(writer& sink) const
{
    static constexpr auto op_75 = static_cast<uint8_t>(opcode::push_size_75);
    const auto size = data_.size();
    const auto code = static_cast<uint8_t>(code_);
    sink.write_byte(code);

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

// The removal of spaces in v3 data is a compatability break with our v2.
std::string operation::to_string(uint32_t active_forks) const
{
    if (!valid_)
        return "<invalid>";

    if (data_.empty())
        return opcode_to_string(code_, active_forks);

    // Data encoding uses single token (with optional non-minimality).
    return "[" + opcode_to_prefix(code_, data_) + encode_base16(data_) + "]";
}

} // namespace machine
} // namespace libbitcoin
