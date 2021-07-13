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
#ifndef LIBBITCOIN_SYSTEM_CHAIN_WITNESS_HPP
#define LIBBITCOIN_SYSTEM_CHAIN_WITNESS_HPP

#include <cstddef>
#include <istream>
#include <string>
#include <bitcoin/system/chain/operation.hpp>
#include <bitcoin/system/chain/script.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace chain {
    
class transaction;

class BC_API witness
{
public:
    typedef data_stack::const_iterator iterator;

    // Constructors.
    //-------------------------------------------------------------------------

    witness();

    witness(witness&& other);
    witness(const witness& other);

    witness(data_stack&& stack);
    witness(const data_stack& stack);

    witness(data_chunk&& encoded, bool prefix);
    witness(const data_chunk& encoded, bool prefix);

    // Operators.
    //-------------------------------------------------------------------------

    /// This class is move assignable and copy assignable.
    witness& operator=(witness&& other);
    witness& operator=(const witness& other);

    bool operator==(const witness& other) const;
    bool operator!=(const witness& other) const;

    // Deserialization (from witness stack).
    //-------------------------------------------------------------------------
    // Prefixed data assumed valid here though caller may confirm with is_valid.

    static witness factory(const data_chunk& encoded, bool prefix);
    static witness factory(std::istream& stream, bool prefix);
    static witness factory(reader& source, bool prefix);

    /// Deserialization invalidates the iterator.
    bool from_data(const data_chunk& encoded, bool prefix);
    bool from_data(std::istream& stream, bool prefix);
    bool from_data(reader& source, bool prefix);

    /// The witness deserialized according to count and size prefixing.
    bool is_valid() const;

    // Serialization.
    //-------------------------------------------------------------------------

    data_chunk to_data(bool prefix) const;
    void to_data(std::ostream& stream, bool prefix) const;
    void to_data(writer& sink, bool prefix) const;

    std::string to_string() const;

    // Iteration.
    //-------------------------------------------------------------------------

    void clear();
    bool empty() const;
    size_t size() const;
    const data_chunk& front() const;
    const data_chunk& back() const;
    iterator begin() const;
    iterator end() const;
    const data_chunk& operator[](size_t index) const;

    // Properties (size, accessors, cache).
    //-------------------------------------------------------------------------

    size_t serialized_size(bool prefix) const;
    const data_stack& stack() const;

    // Utilities.
    //-------------------------------------------------------------------------

    static bool is_push_size(const data_stack& stack);
    static bool is_reserved_pattern(const data_stack& stack);

    bool extract_sigop_script(script& out_script,
        const script& program_script) const;
    bool extract_script(script& out_script, data_stack& out_stack,
        const script& program_script) const;

    // Validation.
    //-------------------------------------------------------------------------

    code verify(const transaction& tx, uint32_t input_index, uint32_t forks,
        const script& program_script, uint64_t value) const;

protected:
    // So that input may call reset from its own.
    friend class input;

    void reset();

private:
    static size_t serialized_size(const data_stack& stack);

    bool valid_;
    data_stack stack_;
};

} // namespace chain
} // namespace system
} // namespace libbitcoin

#endif
