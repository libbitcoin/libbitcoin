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
#ifndef LIBBITCOIN_SYSTEM_CRYPTO_CHECKSUM_HPP
#define LIBBITCOIN_SYSTEM_CRYPTO_CHECKSUM_HPP

#include <cstddef>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/radix/radix.hpp>

namespace libbitcoin {
namespace system {

// checksum, used by payment_address, wif, hd and ek.
// p2p message checksum is implemented independently.
// ----------------------------------------------------------------------------

/// These utilities are used for bitcoin payment addresses and other standards
/// that leverage the same technique. A bitcoin checksum is the leading bytes
/// of a double sha256 hash of any data. Bitcoin standards typically use a four
/// byte checksum, though any length up to min(Size, 256) is allowed here.
/// Accessing Checksum (length) parameter requires explicitly specifying both.

static const size_t checksum_default_size = sizeof(uint32_t);

/// Append the bitcoin checksum of slices to end of new Size array.
/// Underfill is padded with 0x00, excess is truncated.
template <size_t Size, size_t Checksum = checksum_default_size>
data_array<Size> insert_checksum(const data_loaf& slices);

/// Append bitcoin checksum of preceding data to end of existing Size array.
template <size_t Size, size_t Checksum = checksum_default_size>
void insert_checksum(data_array<Size>& data);

/// Verify the last bytes are a bitcoin checksum of the preceding bytes.
template <size_t Size, size_t Checksum = checksum_default_size>
bool verify_checksum(const data_array<Size>& data);

/// Append slices and a four byte bitcoin checksum.
BC_API data_chunk append_checksum(const data_loaf& slices);

/// Append a four byte bitcoin checksum of data to itself.
BC_API void append_checksum(data_chunk& data);

/// Verify the last four bytes are a bitcoin checksum of the preceding bytes.
BC_API bool verify_checksum(const data_slice& data);

// bech32 checksum, used by witness_address.
// ----------------------------------------------------------------------------

/// These utilities are used for witness payment addresses and other standards
/// that leverage the same technique. These hide the complexities of data
/// expansion and contraction inherent to BIP173. base_32 class is a typical
/// byte encoder/decoder, yet fully complaint with BIP173. The output from
/// decode_base32 can be fed directly to bech32_verify_checked(data) and
/// the output of bech32_build_checked can be fed directly to encode_base32.
/// The implementation does not support unversioned bech32 payloads as there
/// is no use case and support requires exposure of bech32 internals.

/// Combine witness version, program and checksum.
/// The result may be passed to encode_base32 when creating a witness address.
/// For implementation details see wallet::witness_address. Version is limited
/// to 5 bits (less than 32) by bech32 design and is otherwise truncated. Non-
/// zero versions select bech32m (vs. bech32), resulting in distinct checksum.
BC_API base32_chunk bech32_build_checked(uint8_t version,
    const data_chunk& program, const std::string& prefix);

/// Verify the bech32 checksum and extract witness version and program.
/// The data parameter may obtained from a witness address using decode_base32.
/// For implementation details see wallet::witness_address.
BC_API bool bech32_verify_checked(uint8_t& out_version,
    data_chunk& out_program, const std::string& prefix,
    const base32_chunk& checked);

} // namespace system
} // namespace libbitcoin

#include <bitcoin/system/impl/crypto/checksum.ipp>

#endif
