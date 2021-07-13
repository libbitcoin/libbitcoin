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
#ifndef LIBBITCOIN_SYSTEM_EXCEPTIONS_HPP
#define LIBBITCOIN_SYSTEM_EXCEPTIONS_HPP

#include <iostream>
#include <exception>
#include <stdexcept>
#include <boost/program_options.hpp>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {

// We do not use exceptions for general purpose error handling.

// Dependencies, including boost and qrencode sources may throw.
// We try to avoid the use of external API calls that throw.
// If an external API throws for non-exceptional results, we catch and squash.

// We throw for iostream read/write failures, given the nature of streams.

// We throw for division by zero and other basic math parameterization errors
// because the functions are public and are otherwise unguarded. When calling
// them internally we first verify parameters to prevent hitting the exception.
// This behavior is no different than when using c++ math operators directly.

// We do not generally catch or throw for out of memory conditions as they are
// pervasive and it would be impractical to attempt to inject an allocator into
// all possible external API memory allocations.

// UTF8 Everywhere initialization failure.
using runtime_exception = std::runtime_error;

// ICU initialization failure.
using dependency_exception = std::runtime_error;

// Maths.
using overflow_exception = std::overflow_error;
using underflow_exception = std::underflow_error;

// This is used as a guard for math operations that should not fail under
// expected conditions (such as block height exceeding max_uint32). This should
// eventually be replaced with common parameter guards.
using range_exception = std::range_error;

// Streams.
using ostream_exception = std::iostream::failure;
using istream_exception = boost::program_options::invalid_option_value;
using ifstream_exception = boost::program_options::reading_file;

class BC_API end_of_stream
  : std::exception
{
};

} // namespace system
} // namespace libbitcoin

#endif
