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
#ifndef LIBBITCOIN_SYSTEM_DATA_DATA_SLAB_HPP
#define LIBBITCOIN_SYSTEM_DATA_DATA_SLAB_HPP

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <bitcoin/system/constraints.hpp>
#include <bitcoin/system/data/data_slice.hpp>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {

/// Lifetime:
/// No prvalues are accepted, as they are not meaningfully-writeable.
/// The slab does not extend the lifetime of any rvalue passed to it.
/// The caller must ensure that the references or pointers passed to the slab
/// are not orphaned during its lifetime, which is consistent with any passage
/// by reference. See data_slice for a detailed explanation of lifetime.
/// en.cppreference.com/w/cpp/language/reference#Dangling_references

/// Resizable but otherwise const iterable wrapper for non-const memory buffer.
/// Identical to data_slice except pointer is non-const, and therefore does not
/// accept construction from const or movable (non-writeable) sources.
/// Not a substitute for move overrides or containment.
/// Accepts any sizeof(T) == 1 type as a "byte" and emits uint8_t.
/// [] iteration past end is safe and returns zeros.
/// Negative size construction yields a valid empty object.
class BC_API data_slab
{
public:
    typedef size_t size_type;
    typedef uint8_t value_type;

    /// A pointer to mutable bytes (pointer itself is also mutable).
    typedef value_type* pointer;

    /// For stream source compatibility, until data_slab has an iterator.
    typedef pointer iterator;

    /// Constructors.

    /// Empty slab.
    data_slab() noexcept;

    /// Copy construction.
    data_slab(const data_slab& other) noexcept;

    /// Byte array constructor (casts Byte to uint8_t).
    template <size_type Size, typename Byte, if_byte<Byte> = true>
    data_slab(std::array<Byte, Size>& data) noexcept;

    // TODO: test.
    /// Byte vector constructor (casts Byte to uint8_t).
    template <typename Byte, if_byte<Byte> = true>
    data_slab(std::vector<Byte>& data) noexcept;

    // TODO: restrict to iterator-to-non-const references.
    /// Byte iterators constructor (casts to uint8_t).
    template <typename Iterator>
    data_slab(Iterator& begin, Iterator& end) noexcept;

    // TODO: change to begin/size construction.
    /// Byte pointer to non-const constructor (casts Byte to uint8_t).
    template <typename Byte, if_byte<Byte> = true>
    data_slab(Byte* begin, Byte* end) noexcept;

    // TODO: test.
    /// String constructor (casts char to uint8_t).
    data_slab(std::string& text) noexcept;

    /// Methods.

    /// Copy data to an array.
    /// Underfill is padded with 0x00, excess is truncated.
    template <size_type Size>
    std::array<value_type, Size> to_array() const noexcept;

    /// Copy data to a vector.
    std::vector<value_type> to_chunk() const noexcept;

    /// Cast buffer to a data_slice.
    data_slice to_slice() const noexcept;

    /// Convert data to a string (casts uint8_t to char).
    std::string to_string() const noexcept;

    /// Convert data to a base16 string.
    std::string encoded() const noexcept;

    /// Resize the slab by decrementing the end pointer.
    /// This is the only mutable action that can be taken on the slab.
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
    operator data_slice() const noexcept;
    value_type operator[](size_type index) const noexcept;

private:
    data_slab(pointer begin, pointer end, size_type size) noexcept;

    template <typename Iterator>
    static data_slab from_iterators(Iterator& begin, Iterator& end) noexcept;

    template <typename Pointer>
    static data_slab from_size(Pointer begin, size_type size) noexcept;

    const pointer begin_;
    pointer end_;
    size_type size_;
};

/// Binary operators.
bool operator==(const data_slab& left, const data_slab& right) noexcept;
bool operator!=(const data_slab& left, const data_slab& right) noexcept;

} // namespace system
} // namespace libbitcoin

#include <bitcoin/system/impl/data/data_slab.ipp>

#endif
