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
#include <string>

BOOST_AUTO_TEST_SUITE(qr_code_tests)

using namespace bc::system::wallet;

// encode

static const uint16_t no_scale = 0u;
static const uint16_t no_margin = 0u;
static const uint8_t default_version = 0u;
static const auto low = qr_code::recovery_level::low;

BOOST_AUTO_TEST_CASE(qr_code__encode__no_value__false)
{
    data_chunk out;
    stream::out::data sink(out);
    BOOST_REQUIRE(!qr_code::encode(sink, ""));
}

BOOST_AUTO_TEST_CASE(qr_code__encode__excessive_character_value__false)
{
    data_chunk out;
    stream::out::data sink(out);

    // ERANGE returned by qrencode.
    std::string value(bc::max_uint16, 0x42);
    BOOST_REQUIRE(!qr_code::encode(sink, value));
}

BOOST_AUTO_TEST_CASE(qr_code__encode__one_character_version_0_scale_1_margin_0__expected)
{
    static const data_chunk expected
    {
        // TIFF metadata.
        0x4d, 0x4d,             // "MM" - Motorola encoding (big endian).
        0x00, 0x2a,             // Arbitrary delimeter (42).
        0x00, 0x00, 0x00, 0x08, // Offset of IFD[0].

        // [Each directory may be anywhere in the file]

        // Number of IFD[0] entries, word boundary required.
        0x00, 0x0a,

        // IFD[0] entries.
        0x01, 0x00,             // image_width
        0x00, 0x04,             // 32 bit value
        0x00, 0x00, 0x00, 0x01, // 1 value
        0x00, 0x00, 0x00, 0x15, // 21 pixels wide (column count)
        
        0x01, 0x01,             // image_length
        0x00, 0x04,             // 32 bit value
        0x00, 0x00, 0x00, 0x01, // 1 value
        0x00, 0x00, 0x00, 0x15, // 21 pixels high (row count)
        
        0x01, 0x03,             // compression
        0x00, 0x03,             // 16 bit value
        0x00, 0x00, 0x00, 0x01, // 1 value
        0x00, 0x01,             // compression_mode::none
        0x00, 0x00,             // padding
        
        0x01, 0x06,             // photometric_interpretation
        0x00, 0x03,             // 16 bit value
        0x00, 0x00, 0x00, 0x01, // 1 value
        0x00, 0x00,             // zero_color::white
        0x00, 0x00,             // padding
        
        0x01, 0x11,             // strip_offsets
        0x00, 0x04,             // 32 bit value
        0x00, 0x00, 0x00, 0x01, // 1 value
        0x00, 0x00, 0x00, 0x9c, // offset to value
        
        0x01, 0x16,             // rows_per_strip
        0x00, 0x04,             // 32 bit value
        0x00, 0x00, 0x00, 0x01, // 1 value
        0x00, 0x00, 0x00, 0x15, // 21 rows per (1) strip
        
        0x01, 0x17,             // strip_byte_counts
        0x00, 0x04,             // 32 bit value
        0x00, 0x00, 0x00, 0x01, // 1 value
        0x00, 0x00, 0x00, 0x3f, // 63 bytes per (1) strip
        
        0x01, 0x1a,             // x_resolution
        0x00, 0x05,             // 64 bit value
        0x00, 0x00, 0x00, 0x01, // 1 value
        0x00, 0x00, 0x00, 0x88, // offset to value
        
        0x01, 0x1b,             // y_resolution
        0x00, 0x05,             // 64 bit value
        0x00, 0x00, 0x00, 0x01, // 1 value
        0x00, 0x00, 0x00, 0x90, // offset to value
        
        0x01, 0x28,             // resolution_unit
        0x00, 0x03,             // 16 bit value
        0x00, 0x00, 0x00, 0x01, // 1 value
        0x00, 0x01,             // resolution_units::pixels_per_inch
        0x00, 0x00,             // padding

        // Linked IFD list terminator.
        0x00, 0x00, 0x00, 0x00,

        // Pad to get offsets onto word boundaries.
        0x00, 0x00,
        
        // x_resolution data [numerator][denominator]
        0x00, 0x00, 0x00, 0x48,
        0x00, 0x00, 0x00, 0x01,

        // y_resolution data [numerator][denominator]
        0x00, 0x00, 0x00, 0x48,
        0x00, 0x00, 0x00, 0x01,

        // Libbitcoin data delimiter.
        0xba, 0xad, 0xf0, 0x0d,

        // [Image strips may be anywhere in the file.]

        // Image data.
        // 21 pixels per row = 3 bytes per padded row.
        // 21 rows * 3 bytes per row = 63 bytes = 15 * 4 + 3.
        0xfe, 0x5b, 0xf8, 0x82,
        0x72, 0x08, 0xba, 0xda,
        0xe8, 0xba, 0x52, 0xe8,
        0xba, 0x2a, 0xe8, 0x82,
        0x0a, 0x08, 0xfe, 0xab,
        0xf8, 0x00, 0xd8, 0x00,
        0xef, 0xf6, 0x20, 0x31,
        0x02, 0x30, 0xbf, 0xa8,
        0x88, 0x20, 0x02, 0x38,
        0x0e, 0xea, 0xa8, 0x00,
        0x95, 0x50, 0xfe, 0x97,
        0x70, 0x82, 0xbd, 0xd0,
        0xba, 0xb7, 0x68, 0xba,
        0x02, 0x30, 0xba, 0x88,
        0x88, 0x82, 0xe2, 0x38,
        0xfe, 0xca, 0xa8
    };

    data_chunk out;
    stream::out::data sink(out);
    BOOST_REQUIRE(qr_code::encode(sink, "X", default_version, 1, no_margin));

    // Encode as base16 so that failure message is intelligible.
    BOOST_REQUIRE_EQUAL(encode_base16(out), encode_base16(expected));
}

BOOST_AUTO_TEST_CASE(qr_code__encode__invalid_version__false)
{
    data_chunk out;
    stream::out::data sink(out);

    // EINVAL returned by qrencode.
    BOOST_REQUIRE(!qr_code::encode(sink, "X", qr_code::maximum_version + 1u));
}

BOOST_AUTO_TEST_CASE(qr_code__encode__maximum_version__expected_size)
{
    data_chunk out;
    stream::out::data sink(out);
    BOOST_REQUIRE(qr_code::encode(sink, "X", qr_code::maximum_version));
    BOOST_REQUIRE_EQUAL(out.size(), 252916u);
}

BOOST_AUTO_TEST_CASE(qr_code__encode__excessive_margin__false)
{
    data_chunk out;
    stream::out::data sink(out);

    // At scale 0, (2 * (2^16 - 1))^2 > (2^32 - 1), the TIFF pixel limit. 
    BOOST_REQUIRE(!qr_code::encode(sink, "X", default_version, no_scale, bc::max_uint16));
}

BOOST_AUTO_TEST_CASE(qr_code__encode__excessive_scale__false)
{
    data_chunk out;
    stream::out::data sink(out);

    // At margin 0, scaling even a single pixel to 2^16 overflows the TIFF pixel limit.
    BOOST_REQUIRE(!qr_code::encode(sink, "X", default_version, bc::max_uint16, no_margin));
}

// recovery_level

BOOST_AUTO_TEST_CASE(qr_code__encode__recovery_level_low__expected_size)
{
    data_chunk out;
    stream::out::data sink(out);
    BOOST_REQUIRE(qr_code::encode(sink, "X", default_version, 1, no_margin, qr_code::recovery_level::low));
    BOOST_REQUIRE_EQUAL(out.size(), 219u);
}

BOOST_AUTO_TEST_CASE(qr_code__encode__recovery_level_medium__expected_size)
{
    data_chunk out;
    stream::out::data sink(out);
    BOOST_REQUIRE(qr_code::encode(sink, "X", default_version, 1, no_margin, qr_code::recovery_level::medium));
    BOOST_REQUIRE_EQUAL(out.size(), 219u);
}

BOOST_AUTO_TEST_CASE(qr_code__encode__recovery_level_high__expected_size)
{
    data_chunk out;
    stream::out::data sink(out);
    BOOST_REQUIRE(qr_code::encode(sink, "X", default_version, 1, no_margin, qr_code::recovery_level::high));
    BOOST_REQUIRE_EQUAL(out.size(), 219u);
}

BOOST_AUTO_TEST_CASE(qr_code__encode__recovery_level_highest__expected_size)
{
    data_chunk out;
    stream::out::data sink(out);
    BOOST_REQUIRE(qr_code::encode(sink, "X", default_version, 1, no_margin, qr_code::recovery_level::highest));
    BOOST_REQUIRE_EQUAL(out.size(), 219u);
}

// encode_mode

BOOST_AUTO_TEST_CASE(qr_code__encode__encode_mode_eight_bit__expected_size)
{
    data_chunk out;
    stream::out::data sink(out);
    BOOST_REQUIRE(qr_code::encode(sink, "X", default_version, 1, no_margin, low, qr_code::encode_mode::eight_bit));
    BOOST_REQUIRE_EQUAL(out.size(), 219u);
}

BOOST_AUTO_TEST_CASE(qr_code__encode__encode_mode_kanji__expected_size)
{
    data_chunk out;
    stream::out::data sink(out);
    BOOST_REQUIRE(qr_code::encode(sink, "X", default_version, 1, no_margin, low, qr_code::encode_mode::kanji));
    BOOST_REQUIRE_EQUAL(out.size(), 219u);
}

// to_pixels

class qr_code_accessor
  : qr_code
{
public:
    static data_chunk to_pixels(const data_chunk& coded, uint32_t coded_width,
        uint16_t scale=8, uint16_t margin=2)
    {
        return qr_code::to_pixels(coded, coded_width, scale, margin);
    }
};

BOOST_AUTO_TEST_CASE(qr_code__to_pixels__no_image_scale_0_margin_0__empty)
{
    // No image with no margin creates empty result (success).
    static const data_chunk expected{};

    static const data_chunk data;
    const auto pixels = qr_code_accessor::to_pixels(data, 0, 0, 0);

    // Encode as base16 so that failure message is intelligible.
    BOOST_REQUIRE_EQUAL(encode_base16(pixels), encode_base16(expected));
}

BOOST_AUTO_TEST_CASE(qr_code__to_pixels__no_image_scale_8_margin_2__expected)
{
    // A 2 pixel margin on no image creates 16 zeroed pixels.
    static const data_chunk expected{ 0x00, 0x00, 0x00, 0x00 };

    static const data_chunk data;
    const auto pixels = qr_code_accessor::to_pixels(data, 0, 8, 2);

    // Encode as base16 so that failure message is intelligible.
    BOOST_REQUIRE_EQUAL(encode_base16(pixels), encode_base16(expected));
}

BOOST_AUTO_TEST_CASE(qr_code__to_pixels__one_pixel_scale_0_margin_0__empty)
{
    // Scale zero creates a no image if there is no margin specified.
    static const data_chunk expected{};

    // This pixel state is true.
    static const data_chunk data{ 0xff };
    const auto pixels = qr_code_accessor::to_pixels(data, 1, 0, 0);

    // Encode as base16 so that failure message is intelligible.
    BOOST_REQUIRE_EQUAL(encode_base16(pixels), encode_base16(expected));
}

BOOST_AUTO_TEST_CASE(qr_code__to_pixels__one_pixel_scale_1_margin_0__expected)
{
    // Scale 1 does not expand. A single pixel image with no margin creates a
    // one byte image with only the most significant bit set to 1.
    static const data_chunk expected{ 0x80 };

    // This pixel state is true.
    static const data_chunk data{ 0x01 };
    const auto pixels = qr_code_accessor::to_pixels(data, 1, 1, 0);

    // Encode as base16 so that failure message is intelligible.
    BOOST_REQUIRE_EQUAL(encode_base16(pixels), encode_base16(expected));
}

BOOST_AUTO_TEST_CASE(qr_code__to_pixels__one_pixel_scale_1_margin_1__expected)
{
    // Scale 1 does not expand. A single pixel image with single pixel margin
    // creates a 9 pixel image with the 5th most significant bit set to 1,
    // plus row padding.
    static const data_chunk expected{ 0x00, 0x40, 0x00 };

    // This pixel state is true.
    static const data_chunk data{ 0x01 };
    const auto pixels = qr_code_accessor::to_pixels(data, 1, 1, 1);

    // Encode as base16 so that failure message is intelligible.
    BOOST_REQUIRE_EQUAL(encode_base16(pixels), encode_base16(expected));
}

BOOST_AUTO_TEST_CASE(qr_code__to_pixels__endianness_independent__expected)
{
    // Area is 2x2px so image is scaled to 4x4px and 2px margin makes it 8x8px.
    static const data_chunk expected
    {
        // byte  pixel
        0x00, // 00 0000 00 
        0x00, // 00 0000 00

        0x3c, // 00 1111 00
        0x3c, // 00 1111 00

        0x00, // 00 0000 00
        0x00, // 00 0000 00

        0x00, // 00 0000 00
        0x00  // 00 0000 00
    };

    // The pixel states are true, true, false, false.
    static const data_chunk data
    {
        0x01, 0xff,
        0xfe, 0xfe
    };

    // This test is mirror image with respect to bitwise endianness so it
    // succeeds whether bits are written big endian or little endian. 
    const auto pixels = qr_code_accessor::to_pixels(data, 2, 2, 2);

    // Encode as base16 so that failure message is intelligible.
    BOOST_REQUIRE_EQUAL(encode_base16(pixels), encode_base16(expected));
}

BOOST_AUTO_TEST_CASE(qr_code__to_pixels__little_endian_dependent__expected)
{
    // Area is 2x2px so image is scaled to 4x4px and 2px margin makes it 8x8px.
    static const data_chunk expected
    {
        // byte  pixel
        0x00, // 00 0000 00 
        0x00, // 00 0000 00

        0x30, // 00 1100 00
        0x30, // 00 1100 00

        0x0c, // 00 0011 00
        0x0c, // 00 0011 00

        0x00, // 00 0000 00
        0x00  // 00 0000 00
    };

    // The pixel states are true, false, false, true.
    static const data_chunk data
    {
        0x01, 0xfe,
        0xfe, 0xff
    };

    // This test is not mirror image with respect to bitwise endianness.
    const auto pixels = qr_code_accessor::to_pixels(data, 2, 2, 2);

    // Encode as base16 so that failure message is intelligible.
    BOOST_REQUIRE_EQUAL(encode_base16(pixels), encode_base16(expected));
}

BOOST_AUTO_TEST_SUITE_END()
