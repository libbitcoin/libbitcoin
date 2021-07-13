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
#ifndef LIBBITCOIN_SYSTEM_DATA_DATA_SLICE_HPP
#define LIBBITCOIN_SYSTEM_DATA_DATA_SLICE_HPP

#include <array>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <string>
#include <vector>
#include <bitcoin/system/constraints.hpp>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {

/// Lifetime:
/// "An rvalue may be used to initialize an rvalue reference, in which case
/// the lifetime of the object identified by the rvalue is extended until
/// the scope of the reference ends.":
/// en.cppreference.com/w/cpp/language/value_category#rvalue
/// A prvalue has the "same properties" as an rvalue:
/// en.cppreference.com/w/cpp/language/value_category#prvalue
/// A temporary is a prvalue, and includes initializer_list, but a copy is not:
/// en.cppreference.com/w/cpp/language/implicit_conversion#Temporary_materialization
/// When the data_slice is accepted as a const& it is either passed by
/// reference or materialized as a prvalue. Any reference to the slice extends
/// its lifetime to that of the reference. However, the slice does not retain
/// references to the parameters provided in its construction. Instead pointers
/// are initialized (copied) from addresses derived from these parameters. As a
/// result, any temporary used to construct the slice will be orphaned when the
/// lifetime of the temporary ends, despite retention of the slice by reference,
/// such as by a class member. The lifetime of a temporary used in 
/// materialization of a function parameter is the lifetime of the function,
/// unless extended by reference.

/// Resizable but otherwise const iterable wrapper for const memory buffer.
/// Identical to data_slab except pointer is const, and therefore accepts
/// construction from const sources (including literals and initializers).
/// Not a substitute for move overrides or containment.
/// Accepts any sizeof(T) == 1 type as a "byte" and emits uint8_t.
/// [] iteration past end is safe and returns zeros.
/// Negative size construction yields a valid empty object.
class BC_API data_slice
{
public:
    typedef size_t size_type;
    typedef uint8_t value_type;

    /// A pointer to non-mutable bytes (pointer itself is mutable).
    typedef const value_type* pointer;

    /// For stream source compatibility, until data_slice has an iterator.
    typedef pointer const_iterator;

    /// Constructors.

    /// Empty slice.
    data_slice() noexcept;

    /// Copy construction.
    data_slice(const data_slice& other) noexcept;

    /// Literal bytes constructor.
    /// Integral null terminator is not indexed.
    template <size_type Size>
    data_slice(const char(&text)[Size]) noexcept;

    /// Byte array constructor (casts Byte to uint8_t).
    template <size_type Size, typename Byte, if_byte<Byte> = true>
    data_slice(const std::array<Byte, Size>& data) noexcept;

    /// Byte vector constructor (casts Byte to uint8_t).
    template <typename Byte, if_byte<Byte> = true>
    data_slice(const std::vector<Byte>& data) noexcept;

    // TODO: restrict to iterator-to-const references.
    /// Byte iterators constructor (casts to uint8_t).
    template <typename Iterator>
    data_slice(const Iterator& begin, const Iterator& end) noexcept;

    // TODO: change to begin/size construction.
    /// Byte pointer to const constructor (casts Byte to uint8_t).
    template <typename Byte, if_byte<Byte> = true>
    data_slice(const Byte* begin, const Byte* end) noexcept;

    /// String constructor (casts char to uint8_t).
    data_slice(const std::string& text) noexcept;

    /// Byte initializer list constructor.
    data_slice(std::initializer_list<value_type> bytes) noexcept;

    /// Methods.

    /// Copy data to an array.
    /// Underfill is padded with 0x00, excess is truncated.
    template <size_type Size>
    std::array<value_type, Size> to_array() const noexcept;

    /// Copy data to a vector.
    std::vector<value_type> to_chunk() const noexcept;

    /// Convert data to a string (casts uint8_t to char).
    std::string to_string() const noexcept;

    /// Convert data to a base16 string.
    std::string encoded() const noexcept;

    /// Resize the slice by decrementing the end pointer.
    /// This is the only mutable action that can be taken on the slice.
    /// Returns true if the size was reduced (expansion is not allowed).
    bool resize(size_t size) noexcept;

    /// Properties.
    pointer data() const noexcept;
    pointer begin() const noexcept;
    pointer end() const noexcept;
    value_type front() const noexcept;
    value_type back() const noexcept;
    size_type size() const noexcept;
    bool empty() const noexcept;

    /// Unary operators.
    template<size_type Size>
    operator std::array<value_type, Size>() const noexcept;
    operator std::vector<value_type>() const noexcept;
    value_type operator[](size_type index) const noexcept;

private:
    data_slice(pointer begin, pointer end, size_type size) noexcept;

    template <size_type Size, typename Byte>
    static data_slice from_literal(const Byte(&text)[Size]) noexcept;

    template <typename Iterator>
    static data_slice from_iterators(const Iterator& begin,
        const Iterator& end) noexcept;

    template <typename Pointer>
    static data_slice from_size(Pointer begin, size_type size) noexcept;

    const pointer begin_;
    pointer end_;
    size_type size_;
};

/// Binary operators.
bool operator==(const data_slice& left, const data_slice& right) noexcept;
bool operator!=(const data_slice& left, const data_slice& right) noexcept;

typedef std::initializer_list<data_slice> data_loaf;

} // namespace system
} // namespace libbitcoin

#include <bitcoin/system/impl/data/data_slice.ipp>

#endif
