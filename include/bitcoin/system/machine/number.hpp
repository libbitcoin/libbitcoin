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
#ifndef LIBBITCOIN_SYSTEM_MACHINE_NUMBER_HPP
#define LIBBITCOIN_SYSTEM_MACHINE_NUMBER_HPP

#include <cstddef>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {
namespace machine {

/**
 * Numeric opcodes (OP_1ADD, etc) are restricted to operating on
 * 4-byte integers. The semantics are subtle, though: operands must be
 * in the range [-2^31 +1...2^31 -1], but results may overflow (and are
 * valid as long as they are not used in a subsequent numeric operation).
 *
 * number enforces those semantics by storing results as
 * an int64 and allowing out-of-range values to be returned as a vector of
 * bytes but throwing an exception if arithmetic is done or the result is
 * interpreted as an integer.
 */
class BC_API number
{
public:
    /// Construct with zero value.
    number();

    /// Construct with specified value.
    explicit number(int64_t value);

    /// Replace the value derived from a byte vector with LSB first ordering.
    bool set_data(const data_chunk& data, size_t max_size);

    // Properties
    //-------------------------------------------------------------------------

    /// Return the value as a byte vector with LSB first ordering.
    data_chunk data() const;

    /// Return the value bounded by the limits of int32.
    int32_t int32() const;

    /// Return the unbounded value.
    int64_t int64() const;

    // Stack Helpers
    //-------------------------------------------------------------------------

    /// Return value as stack boolean (nonzero is true).
    bool is_true() const;

    /// Return value as stack boolean (zero is false).
    bool is_false() const;

    // Operators
    //-------------------------------------------------------------------------

    //*************************************************************************
    // CONSENSUS: number implements consensus critical overflow behavior for
    // all operators, specifically [-, +, +=, -=].
    //*************************************************************************

    bool operator>(int64_t value) const;
    bool operator<(int64_t value) const;
    bool operator>=(int64_t value) const;
    bool operator<=(int64_t value) const;
    bool operator==(int64_t value) const;
    bool operator!=(int64_t value) const;

    bool operator>(const number& other) const;
    bool operator<(const number& other) const;
    bool operator>=(const number& other) const;
    bool operator<=(const number& other) const;
    bool operator==(const number& other) const;
    bool operator!=(const number& other) const;

    number operator+() const;
    number operator-() const;
    number operator+(int64_t value) const;
    number operator-(int64_t value) const;
    number operator+(const number& other) const;
    number operator-(const number& other) const;

    number& operator+=(int64_t value);
    number& operator-=(int64_t value);
    number& operator+=(const number& other);
    number& operator-=(const number& other);

private:
    int64_t value_;
};

} // namespace machine
} // namespace system
} // namespace libbitcoin

#endif
