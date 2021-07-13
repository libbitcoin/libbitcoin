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
#ifndef LIBBITCOIN_SYSTEM_STREAM_STREAMERS_BYTE_READER_IPP
#define LIBBITCOIN_SYSTEM_STREAM_STREAMERS_BYTE_READER_IPP

#include <cstddef>
#include <cstdint>
#include <istream>
#include <string>
#include <utility>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/constraints.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/error.hpp>
#include <bitcoin/system/math/math.hpp>
#include <bitcoin/system/serial/serial.hpp>
#include <bitcoin/system/stream/streamers/byte_writer.hpp>

namespace libbitcoin {
namespace system {

// All public methods must rely on protected for stream state except validity.

template <typename IStream>
const uint8_t byte_reader<IStream>::pad = 0x00;

// constructors
//-----------------------------------------------------------------------------

template <typename IStream>
byte_reader<IStream>::byte_reader(IStream& source) noexcept
  : stream_(source)
{
}

template <typename IStream>
byte_reader<IStream>::~byte_reader() noexcept
{
}

// big endian
//-----------------------------------------------------------------------------

template <typename IStream>
template <typename Integer, if_integer<Integer>>
Integer byte_reader<IStream>::read_big_endian() noexcept
{
    // Call into virtual reader (vs. stream) so derived class can reuse.
    return from_big_endian<Integer>(read_bytes(sizeof(Integer)));
}

template <typename IStream>
uint16_t byte_reader<IStream>::read_2_bytes_big_endian() noexcept
{
    return read_big_endian<uint16_t>();
}

template <typename IStream>
uint32_t byte_reader<IStream>::read_4_bytes_big_endian() noexcept
{
    return read_big_endian<uint32_t>();
}

template <typename IStream>
uint64_t byte_reader<IStream>::read_8_bytes_big_endian() noexcept
{
    return read_big_endian<uint64_t>();
}

// little endian
//-----------------------------------------------------------------------------

template <typename IStream>
template <typename Integer, if_integer<Integer>>
Integer byte_reader<IStream>::read_little_endian() noexcept
{
    // Call into virtual reader (vs. stream) so derived class can reuse.
    return from_little_endian<Integer>(read_bytes(sizeof(Integer)));
}

template <typename IStream>
uint16_t byte_reader<IStream>::read_2_bytes_little_endian() noexcept
{
    return read_little_endian<uint16_t>();
}

template <typename IStream>
uint32_t byte_reader<IStream>::read_4_bytes_little_endian() noexcept
{
    return read_little_endian<uint32_t>();
}

template <typename IStream>
uint64_t byte_reader<IStream>::read_8_bytes_little_endian() noexcept
{
    return read_little_endian<uint64_t>();
}

template <typename IStream>
uint64_t byte_reader<IStream>::read_variable() noexcept
{
    const auto value = read_byte();

    switch (value)
    {
        case varint_eight_bytes:
            return read_8_bytes_little_endian();
        case varint_four_bytes:
            return read_4_bytes_little_endian();
        case varint_two_bytes:
            return read_2_bytes_little_endian();
        default:
            return value;
    }
}

template <typename IStream>
size_t byte_reader<IStream>::read_size() noexcept
{
    const auto size = read_variable();

    // This facilitates safely passing the size into a follow-on reader.
    // Return zero allows follow-on use before testing reader state.
    if (size > max_size_t)
    {
        invalid();
        return zero;
    }

    return static_cast<size_t>(size);
}

template <typename IStream>
code byte_reader<IStream>::read_error_code() noexcept
{
    const auto value = read_little_endian<uint32_t>();
    return code(static_cast<error::error_code_t>(value));
}

// bytes
//-----------------------------------------------------------------------------

template <typename IStream>
template <size_t Size>
data_array<Size> byte_reader<IStream>::read_forward() noexcept
{
    // Truncated bytes are populated with 0x00.
    // Reader supports directly populating an array, this avoids a copy.
    data_array<Size> out{};
    do_read_bytes(out.data(), Size);
    return std::move(out);
}

template <typename IStream>
template <size_t Size>
data_array<Size> byte_reader<IStream>::read_reverse() noexcept
{
    return std::move(system::reverse(read_forward<Size>()));
}

template <typename IStream>
std::ostream& byte_reader<IStream>::read(std::ostream& out) noexcept
{
    // This creates an intermediate buffer the size of the stream.
    // This is presumed to be more optimal than looping individual bytes.
    write::bytes::ostream(out).write_bytes(read_bytes());
    return out;
}

template <typename IStream>
mini_hash byte_reader<IStream>::read_mini_hash() noexcept
{
    return read_forward<mini_hash_size>();
}

template <typename IStream>
short_hash byte_reader<IStream>::read_short_hash() noexcept
{
    return read_forward<short_hash_size>();
}

template <typename IStream>
hash_digest byte_reader<IStream>::read_hash() noexcept
{
    return read_forward<hash_size>();
}

template <typename IStream>
long_hash byte_reader<IStream>::read_long_hash() noexcept
{
    return read_forward<long_hash_size>();
}

template <typename IStream>
uint8_t byte_reader<IStream>::peek_byte() noexcept
{
    return do_peek_byte();
}

template <typename IStream>
uint8_t byte_reader<IStream>::read_byte() noexcept
{
    uint8_t value = pad;
    do_read_bytes(&value, one);
    return value;
}

template <typename IStream>
data_chunk byte_reader<IStream>::read_bytes() noexcept
{
    // Isolating get_exhausted to first call is an optimization (must clear).
    if (get_exhausted())
        return {};

    // This will always produce at least one (zero) terminating byte.
    data_chunk out;
    while (valid())
        out.push_back(read_byte());

    clear();
    out.resize(sub1(out.size()));
    out.shrink_to_fit();
    return std::move(out);
}

template <typename IStream>
data_chunk byte_reader<IStream>::read_bytes(size_t size) noexcept
{
    data_chunk out(no_fill_allocator);
    out.resize(size);
    do_read_bytes(out.data(), size);
    return std::move(out);
}

template <typename IStream>
void byte_reader<IStream>::read_bytes(uint8_t* buffer, size_t size) noexcept
{
    do_read_bytes(buffer, size);
}

// strings
//-----------------------------------------------------------------------------

template <typename IStream>
std::string byte_reader<IStream>::read_string() noexcept
{
    return read_string(read_size());
}

template <typename IStream>
std::string byte_reader<IStream>::read_string(size_t size) noexcept
{
    // Isolating get_exhausted to first call is an optimization (must clear).
    if (get_exhausted())
        return {};

    // This will produce one (zero) terminating byte if size exceeds available.
    std::string out;
    out.reserve(add1(size));
    while (!is_zero(size--) && valid())
        out.push_back(read_byte());

    // Removes zero and all after, required for bitcoin string deserialization.
    const auto position = out.find(pad);
    out.resize(position == std::string::npos ? out.size() : position);
    out.shrink_to_fit();

    clear();
    return out;
}

// context
//-----------------------------------------------------------------------------

template <typename IStream>
void byte_reader<IStream>::skip_byte() noexcept
{
    do_skip_bytes(one);
}


template <typename IStream>
void byte_reader<IStream>::skip_bytes(size_t size) noexcept
{
    do_skip_bytes(size);
}

template <typename IStream>
void byte_reader<IStream>::rewind_byte() noexcept
{
    do_rewind_bytes(one);
}

template <typename IStream>
void byte_reader<IStream>::rewind_bytes(size_t size) noexcept
{
    do_rewind_bytes(size);
}

template <typename IStream>
bool byte_reader<IStream>::is_exhausted() const noexcept
{
    // True if invalid or if no bytes remain in the stream.
    return get_exhausted();
}

template <typename IStream>
void byte_reader<IStream>::invalidate() noexcept
{
    // Permanently invalidate the stream/reader.
    invalid();
}

template <typename IStream>
byte_reader<IStream>::operator bool() const noexcept
{
    // True if any call created an error state, even if there have been
    // subsequent calls, or if any error state preexists on the stream.
    return valid();
}

// This should not be necessary with bool() defined, but it is.
template <typename IStream>
bool byte_reader<IStream>::operator!() const noexcept
{
    return !valid();
}

// protected virtual
//-----------------------------------------------------------------------------
// These may only call non-virtual (private) methods (due to overriding).

template <typename IStream>
uint8_t byte_reader<IStream>::do_peek_byte() noexcept
{
    // This sets eofbit (or badbit) on empty and eofbit if otherwise at end.
    // eofbit does not cause !!eofbit == true, but badbit does, so we validate
    // the call the achieve consistent behavior. The reader will be invalid if
    // the stream is peeked past end, including when empty.
    const auto value = stream_.peek();
    validate();
    return valid() ? value : pad;
}

template <typename IStream>
void byte_reader<IStream>::do_read_bytes(uint8_t* buffer, size_t size) noexcept
{
    // It is not generally more efficient to call stream_.get() for one byte.
    // partially-failed reads here will be populated by the stream, not padded.
    // However, both copy_source and stringstream will zero-fill partial reads.
    // Read on empty is inconsistent, so validate the result. The reader will be
    // invalid if the stream is get past end, including when empty.
    stream_.read(reinterpret_cast<char*>(buffer), size);
    validate();
}

template <typename IStream>
void byte_reader<IStream>::do_skip_bytes(size_t size) noexcept
{
    // pos_type is not an integer so cannot use limit cast here,
    // but sizeof(std::istream/istringstream::pos_type) is 24 bytes.
    seeker(static_cast<IStream::pos_type>(size));
}

template <typename IStream>
void byte_reader<IStream>::do_rewind_bytes(size_t size) noexcept
{
    // pos_type is not an integer so cannot use limit cast here,
    // but sizeof(std::istream/istringstream::pos_type) is 24 bytes.
    seeker(-static_cast<IStream::pos_type>(size));
}

template <typename IStream>
bool byte_reader<IStream>::get_exhausted() const noexcept
{
    // Empty behavior is broadly inconsistent across implementations.
    // It is also necessary to start many reads, including initial reads, with
    // an exhaustion check, which must be consistent and not state-changing.
    // A state change would preclude testing for errors after testing for end.
    // This method is const because it reliably creates no net state change.
    // Streams are valid unless read or peeked. Peek does not change stream
    // position, so it is used to force the stream into a failure state when
    // empty. peek past end always sets eofbit but peek state on empty is
    // inconsistent across streams, though a flag is always set. eofbit is set
    // by istringstream and badbit is set by boost.

    if (!valid())
        return true;

    // Peek to force error on eof, save condition, restore valid stream state.
    stream_.peek();
    const auto eof = !valid();
    stream_.clear();

    return eof;
}

// private
// ----------------------------------------------------------------------------
// These may only call other private methods (due to overriding).

template <typename IStream>
bool byte_reader<IStream>::valid() const noexcept
{
    // zero is the istream documented flag for no error.
    return stream_.rdstate() == IStream::goodbit;
}

template <typename IStream>
void byte_reader<IStream>::invalid() noexcept
{
    // If eofbit is set, failbit is generally set on all operations.
    // badbit is unrecoverable, set the others to ensure consistency.
    stream_.setstate(IStream::eofbit | IStream::failbit | IStream::badbit);
}

template <typename IStream>
void byte_reader<IStream>::validate() noexcept
{
    // Ensure that any failure in the call fully invalidates the stream/reader.
    // Some errors are recoverable, so a sequence of operations without testing
    // for validity could miss an error on intervening operations. For example,
    // seekg can reset eofbit and read past doesn't set badbit (recoverable).
    if (!valid())
        invalid();
}

template <typename IStream>
void byte_reader<IStream>::clear() noexcept
{
    stream_.clear();
}

template <typename IStream>
void byte_reader<IStream>::seeker(typename IStream::pos_type offset) noexcept
{
    // Force these to be consistent by treating zero seek as a no-op.
    // boost/istringstream both succeed on non-empty zero seek.
    // istringstream succeeds on empty zero seek, boost sets failbit.
    if (is_zero(offset))
        return;

    // Force these to be consistent, and avoid propagating exceptions.
    // istringstream sets failbit on non-empty over/underflow, boost throws.
    // boost/istringstream both set failbit on empty over/underflow.
    try
    {
        stream_.seekg(offset, IStream::cur);
        validate();
    }
    catch (const IStream::failure&)
    {
        invalid();
    }
}

} // namespace system
} // namespace libbitcoin

#endif
