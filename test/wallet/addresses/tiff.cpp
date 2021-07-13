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
#include "../../test.hpp"

BOOST_AUTO_TEST_SUITE(tiff_tests)

BOOST_AUTO_TEST_CASE(tiff__to_image__rectangular_image__true)
{
    // A rectangle of 5 pixel width has 1 byte rows, so any number of bytes is ok.
    static const auto size = 42u;
    static const auto width = 5u;

    data_chunk bitmap(size, 'x');
    data_chunk tiff;
    stream::out::data stream(tiff);
    BOOST_REQUIRE(tiff::to_image(stream, bitmap, width));
}

BOOST_AUTO_TEST_CASE(tiff__to_image__mismatched_image_size__false)
{
    // A rectangle of 9 pixel width has 2 byte rows, so byte count must be even.
    static const auto size = 41u;
    static const auto width = 9u;

    data_chunk bitmap(size, 'x');
    data_chunk tiff;
    stream::out::data stream(tiff);
    BOOST_REQUIRE(!tiff::to_image(stream, bitmap, width));
}

BOOST_AUTO_TEST_CASE(tiff__to_image__empty__false)
{
    static const auto width = 1u;
    static const data_chunk bitmap;
    data_chunk tiff;
    stream::out::data stream(tiff);
    BOOST_REQUIRE(!tiff::to_image(stream, bitmap, width));
}

BOOST_AUTO_TEST_CASE(tiff__to_image__width_0__false)
{
    static const auto width = 0u;
    static const data_chunk bitmap{ 'x' };
    data_chunk tiff;
    stream::out::data stream(tiff);
    BOOST_REQUIRE(!tiff::to_image(stream, bitmap, width));
}

BOOST_AUTO_TEST_CASE(tiff__to_image__perfect_square__expected_true)
{
    // A square with sides of 4 pixels is 4 rows of 1 byte.
    static const auto width = 4u;
    static const data_chunk bitmap{ 0x20, 0xa0, 0x20, 0xb0 };

    // Manually-contructed expectation of tiff encoding.
    static const data_chunk expected
    {
        // Header.
        0x4d, 0x4d,             // "MM" Motorola encoding (big endian)
        0x00, 0x2a,             // magic number (42)
        0x00, 0x00, 0x00, 0x08, // IFD0 offset

        // [Each directory may be anywhere in the file]

        // IFD0, word boundary required.
        0x00, 0x0a,             // count of entries

        // IFDE0
        0x01, 0x00,             // image_width
        0x00, 0x04,             // long value
        0x00, 0x00, 0x00, 0x01, // count
        0x00, 0x00, 0x00, 0x04, // side

        // IFDE1
        0x01, 0x01,             // image_length
        0x00, 0x04,             // long value
        0x00, 0x00, 0x00, 0x01, // count
        0x00, 0x00, 0x00, 0x04, // side

        // IFDE2
        0x01, 0x03,             // compression
        0x00, 0x03,             // short value
        0x00, 0x00, 0x00, 0x01, // count
        0x00, 0x01,             // none
        0x00, 0x00,             // pad

        // IFDE3
        0x01, 0x06,             // photometric_interpretation
        0x00, 0x03,             // short value
        0x00, 0x00, 0x00, 0x01, // count
        0x00, 0x00,             // zero is white
        0x00, 0x00,             // pad

        // IFDE4
        0x01, 0x11,             // strip_offsets
        0x00, 0x04,             // long value
        0x00, 0x00, 0x00, 0x01, // count
        0x00, 0x00, 0x00, 0x9c, // value (not offset)

        // IFDE5
        0x01, 0x16,             // rows_per_strip
        0x00, 0x04,             // long value
        0x00, 0x00, 0x00, 0x01, // count
        0x00, 0x00, 0x00, 0x04, // side

        // IFDE6
        0x01, 0x17,             // strip_byte_counts
        0x00, 0x04,             // long value
        0x00, 0x00, 0x00, 0x01, // count
        0x00, 0x00, 0x00, 0x04, // value (not offset)

        // IFDE7
        0x01, 0x1a,             // x_resolution
        0x00, 0x05,             // ratio value
        0x00, 0x00, 0x00, 0x01, // count
        0x00, 0x00, 0x00, 0x88, // value (offset)

        // IFDE8
        0x01, 0x1b,             // y_resolution
        0x00, 0x05,             // ratio value
        0x00, 0x00, 0x00, 0x01, // count
        0x00, 0x00, 0x00, 0x90, // value (offset)

        // IFDE9
        0x01, 0x28,             // resolution_unit
        0x00, 0x03,             // short value
        0x00, 0x00, 0x00, 0x01, // count
        0x00, 0x01,             // inch
        0x00, 0x00,             // pad

        // IFD0 Terminator
        0x00, 0x00, 0x00, 0x00, // null teminated linked list

        // pad
        0x00, 0x00,             // pad

        // [Each field value may be anywhere in the file.]

        // Offset field values, word boundaries required.
        0x00, 0x00, 0x00, 0x48, // horizontal resolution numerator
        0x00, 0x00, 0x00, 0x01, // horizontal resolution denominator
        0x00, 0x00, 0x00, 0x48, // vertical resolution numerator
        0x00, 0x00, 0x00, 0x01, // vertical resolution denominator

        // Libbitcoin proprietary data delimiter.
        0xba, 0xad, 0xf0, 0x0d,

        // [Image strips may be anywhere in the file.]

        // Image (strip) - big endian, each row padded with 4 bits.
        0x20, // 0010 0000
        0xa0, // 1010 0000
        0x20, // 0010 0000
        0xb0  // 1011 0000
    };

    data_chunk tiff;
    stream::out::data stream(tiff);
    BOOST_REQUIRE(tiff::to_image(stream, bitmap, width));

    // Encode as base16 so that failure message is intelligible.
    BOOST_REQUIRE_EQUAL(encode_base16(tiff), encode_base16(expected));
}

BOOST_AUTO_TEST_SUITE_END()
