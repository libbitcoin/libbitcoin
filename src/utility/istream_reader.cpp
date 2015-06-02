/*
 * Copyright (c) 2011-2013 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <bitcoin/bitcoin/utility/istream_reader.hpp>
#include <bitcoin/bitcoin/utility/endian.hpp>

namespace libbitcoin {

istream_reader::istream_reader(std::istream& stream)
: stream_(stream)
{
}

istream_reader::operator bool() const
{
    return stream_;
}

bool istream_reader::operator!() const
{
    return !stream_;
}

bool istream_reader::is_exhausted() const
{
    return stream_ && (stream_.peek() == std::istream::traits_type::eof());
}

uint8_t istream_reader::read_byte()
{
    uint8_t result;

    stream_.read(reinterpret_cast<char*>(&result), sizeof result);

    return result;
}

uint16_t istream_reader::read_2_bytes_little_endian()
{
    return read_little_endian<uint16_t>();
}

uint32_t istream_reader::read_4_bytes_little_endian()
{
    return read_little_endian<uint32_t>();
}

uint64_t istream_reader::read_8_bytes_little_endian()
{
    return read_little_endian<uint64_t>();
}

uint64_t istream_reader::read_variable_uint_little_endian()
{
    uint8_t length = read_byte();

    if (length < 0xfd)
        return length;
    else if (length == 0xfd)
        return read_2_bytes_little_endian();
    else if (length == 0xfe)
        return read_4_bytes_little_endian();

    // length should be 0xff
    return read_8_bytes_little_endian();
}

uint16_t istream_reader::read_2_bytes_big_endian()
{
    return read_big_endian<uint16_t>();
}

uint32_t istream_reader::read_4_bytes_big_endian()
{
    return read_big_endian<uint32_t>();
}

uint64_t istream_reader::read_8_bytes_big_endian()
{
    return read_big_endian<uint64_t>();
}

uint64_t istream_reader::read_variable_uint_big_endian()
{
    uint8_t length = read_byte();

    if (length < 0xfd)
        return length;
    else if (length == 0xfd)
        return read_2_bytes_big_endian();
    else if (length == 0xfe)
        return read_4_bytes_big_endian();

    // length should be 0xff
    return read_8_bytes_big_endian();
}

data_chunk istream_reader::read_data(uint64_t n_bytes)
{
    data_chunk raw_bytes(n_bytes);

    for (uint64_t i = 0; i < n_bytes; ++i)
        raw_bytes[i] = read_byte();

    return raw_bytes;
}

void istream_reader::read_data(uint8_t* data, uint64_t n_bytes)
{
    for (uint64_t i = 0; i < n_bytes; ++i)
        data[i] = read_byte();
}

data_chunk istream_reader::read_data_to_eof()
{
    data_chunk raw_bytes;

    while (stream_ && (stream_.peek() != std::istream::traits_type::eof()))
        raw_bytes.push_back(read_byte());

    return raw_bytes;
}

hash_digest istream_reader::read_hash()
{
    return read_bytes<hash_size>();
}

short_hash istream_reader::read_short_hash()
{
    return read_bytes<short_hash_size>();
}

std::string istream_reader::read_fixed_string(uint64_t len)
{
    data_chunk string_bytes = read_data(len);
    std::string result(string_bytes.begin(), string_bytes.end());
    // Removes trailing 0s... Needed for string comparisons
    return result.c_str();
}

std::string istream_reader::read_string()
{
    uint64_t string_size = read_variable_uint_little_endian();
    return read_fixed_string(string_size);
}

} // namespace libbitcoin
