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
#include <bitcoin/system/wallet/addresses/tiff.hpp>

#include <cstdint>
#include <iostream>
#include <cmath>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/math/division.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {

enum tiff_header : uint16_t
{
    // "II" Intel byte ordering.
    little_endian = 0x4949,

    // "MM" Motorola byte ordering.
    big_endian = 0x4d4d,

    // An arbitrary but carefully chosen number, always 42.
    magic_number = 0x002a
};

enum directory_header : uint16_t
{
    // Implement only the ten required entries.
    entry_count = 0x000a
};

enum libbitcoin_token : uint32_t
{
    // Make the image data easy to visually locate in a hex dump.
    arbitrary_data_delimiter = 0xbaadf00d
};

enum field_tag : uint16_t
{
    image_width = 0x0100,
    image_length = 0x0101,
    compression = 0x0103,
    photometric_interpretation = 0x0106,
    strip_offsets = 0x0111,
    rows_per_strip = 0x0116,
    strip_byte_counts = 0x0117,
    x_resolution = 0x011a,
    y_resolution = 0x011b,
    resolution_unit = 0x0128
};

enum field_type : uint16_t
{
    half = 3,
    full = 4,
    ratio = 5
};

enum field_value_count : uint32_t
{
    single = 1
};

enum offset : uint32_t
{
    terminator = 0
};

enum compression_mode : uint16_t
{
    none = 1,
    modified_huffman = 2,
    pack_bits = 32773
};

enum zero_color : uint16_t
{
    white = 0,
    black = 1
};

enum resolution_units : uint16_t
{
    unspecified = 0,
    pixels_per_inch = 1,
    pixels_per_centimeter = 2
};

enum pad : uint16_t
{
    zero = 0
};

// Offset calculations.
constexpr auto directory0_offset = sizeof(tiff_header::big_endian) +
    sizeof(tiff_header::magic_number) + sizeof(offset);
constexpr auto entry0_offset = directory0_offset +
    sizeof(directory_header::entry_count);
constexpr auto entry_size = sizeof(field_tag) + sizeof(field_type) +
    sizeof(field_value_count) + sizeof(offset);
constexpr auto x_resolution_offset = entry0_offset +
    directory_header::entry_count * entry_size + 
    sizeof(offset::terminator) + sizeof(pad::zero);
constexpr auto y_resolution_offset = x_resolution_offset + sizeof(offset) +
    sizeof(offset);
constexpr auto strip0_offset = y_resolution_offset + sizeof(offset) +
    sizeof(offset) + sizeof(arbitrary_data_delimiter);

// Offset values must be on word boundaries.
static_assert(directory0_offset % sizeof(offset) == 0,
    "TIFF directory must begin on a word boundary.");
static_assert(x_resolution_offset % sizeof(offset) == 0,
    "TIFF offset value must begin on a word boundary (x resolutions).");
static_assert(y_resolution_offset % sizeof(offset) == 0,
    "TIFF offset value must begin on a word boundary (y resolutions).");
static_assert(strip0_offset % sizeof(offset) == 0,
    "TIFF offset value must begin on a word boundary (strip).");

// Public symbol used to parse file for image stream.
uint32_t tiff::image_offset = strip0_offset;
size_t tiff::max_image_bytes = max_uint32;

// TODO: accept and return stream.
bool tiff::to_image(std::ostream& out, const data_chunk& data, uint16_t width)
{
    // Empty image is not valid TIFF.
    if (width == 0u || data.empty())
        return false;

    // TIFF encoding limit is max_uint32.
    // This method is otherwise limited only by the stream capacity.
    if (data.size() > max_image_bytes)
        return false;

    const auto size = static_cast<uint32_t>(data.size());

    // The width > 0 and therefore row_bytes > 0.
    const auto row_bytes = ceilinged_divide(width, byte_bits);

    // Each row is the same width, so there must be no remainder.
    if (size % row_bytes != 0u)
        return false;

    // All rows will be stored in a single strip.
    const auto rows = size / row_bytes;

    write::bytes::ostream writer(out);

    // =============== Header ===============

    // These sentinals are symmetrical, so order does not matter.
    // But all writes with endianness below must conform to this setting.
    writer.write_2_bytes_big_endian(tiff_header::big_endian);
    
    // An arbitrary delimeter, always 42.
    writer.write_2_bytes_big_endian(tiff_header::magic_number);

    // Offset of IFD[0] (size of header).
    writer.write_4_bytes_big_endian(directory0_offset);

    // --------------- IFD[0] ---------------

    // Number of IFD[0] entries (fields).
    writer.write_2_bytes_big_endian(entry_count);

    // Write all required fields in required order.
    // An IFDE is always 12 bytes long (the set is an array).
    // ............... IFDE[0] ..............
    writer.write_2_bytes_big_endian(field_tag::image_width);
    writer.write_2_bytes_big_endian(field_type::full);
    writer.write_4_bytes_big_endian(field_value_count::single);
    writer.write_4_bytes_big_endian(width);

    // ............... IFDE[1] ..............
    writer.write_2_bytes_big_endian(field_tag::image_length);
    writer.write_2_bytes_big_endian(field_type::full);
    writer.write_4_bytes_big_endian(field_value_count::single);
    writer.write_4_bytes_big_endian(rows);

    // ............... IFDE[2] ..............
    writer.write_2_bytes_big_endian(field_tag::compression);
    writer.write_2_bytes_big_endian(field_type::half);
    writer.write_4_bytes_big_endian(field_value_count::single);
    writer.write_2_bytes_big_endian(compression_mode::none);
    writer.write_2_bytes_big_endian(pad::zero);

    // ............... IFDE[3] ..............
    writer.write_2_bytes_big_endian(field_tag::photometric_interpretation);
    writer.write_2_bytes_big_endian(field_type::half);
    writer.write_4_bytes_big_endian(field_value_count::single);
    writer.write_2_bytes_big_endian(zero_color::white);
    writer.write_2_bytes_big_endian(pad::zero);

    // ............... IFDE[4] ..............
    writer.write_2_bytes_big_endian(field_tag::strip_offsets);
    writer.write_2_bytes_big_endian(field_type::full);
    writer.write_4_bytes_big_endian(field_value_count::single);
    writer.write_4_bytes_big_endian(strip0_offset);

    // ............... IFDE[5] ..............
    writer.write_2_bytes_big_endian(field_tag::rows_per_strip);
    writer.write_2_bytes_big_endian(field_type::full);
    writer.write_4_bytes_big_endian(field_value_count::single);
    writer.write_4_bytes_big_endian(rows);

    // ............... IFDE[6] ..............
    writer.write_2_bytes_big_endian(field_tag::strip_byte_counts);
    writer.write_2_bytes_big_endian(field_type::full);
    writer.write_4_bytes_big_endian(field_value_count::single);
    writer.write_4_bytes_big_endian(size);

    // ............... IFDE[7] ..............
    writer.write_2_bytes_big_endian(field_tag::x_resolution);
    writer.write_2_bytes_big_endian(field_type::ratio);
    writer.write_4_bytes_big_endian(field_value_count::single);
    writer.write_4_bytes_big_endian(x_resolution_offset);

    // ............... IFDE[8] ..............
    writer.write_2_bytes_big_endian(field_tag::y_resolution);
    writer.write_2_bytes_big_endian(field_type::ratio);
    writer.write_4_bytes_big_endian(field_value_count::single);
    writer.write_4_bytes_big_endian(y_resolution_offset);

    // ............... IFDE[9] ..............
    writer.write_2_bytes_big_endian(field_tag::resolution_unit);
    writer.write_2_bytes_big_endian(field_type::half);
    writer.write_4_bytes_big_endian(field_value_count::single);
    writer.write_2_bytes_big_endian(resolution_units::pixels_per_inch);
    writer.write_2_bytes_big_endian(pad::zero);

    // --------------------------------------

    // Offset of IFD[1] or null terminator (the set is a linked list).
    writer.write_4_bytes_big_endian(offset::terminator);

    // ======================================

    // Pad to get offsets onto word boundaries.
    writer.write_2_bytes_big_endian(pad::zero);

    // x_resolution data [numerator][denominator]
    writer.write_4_bytes_big_endian(72);
    writer.write_4_bytes_big_endian(1);

    // y_resolution data [numerator][denominator]
    writer.write_4_bytes_big_endian(72);
    writer.write_4_bytes_big_endian(1);

    // ======================================

    // Make the image data easy to visually locate in a hex dump.
    writer.write_4_bytes_big_endian(arbitrary_data_delimiter);

    // Strip0 (image data).
    writer.write_bytes(data);

    writer.flush();
    return true;
}

} // namespace system
} // namespace libbitcoin
