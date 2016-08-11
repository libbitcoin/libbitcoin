/**
 * Copyright (c) 2011-2015 libbitcoin developers (see AUTHORS)
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBBITCOIN_CHAIN_POINT_HPP
#define LIBBITCOIN_CHAIN_POINT_HPP

#include <cstdint>
#include <istream>
#include <string>
#include <vector>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/chain/point_byte_iterator.hpp>
#include <bitcoin/bitcoin/math/hash.hpp>
#include <bitcoin/bitcoin/utility/data.hpp>
#include <bitcoin/bitcoin/utility/reader.hpp>
#include <bitcoin/bitcoin/utility/writer.hpp>

namespace libbitcoin {
namespace chain {

class BC_API point
{
public:
    typedef std::vector<point> list;
    typedef std::vector<uint32_t> indexes;

    static point factory_from_data(const data_chunk& data);
    static point factory_from_data(std::istream& stream);
    static point factory_from_data(reader& source);
    static uint64_t satoshi_fixed_size();

    /// Preserved for compatability with server v2.
    uint64_t checksum() const;

    /// Greater entropy than checksum.
    uint64_t checksum2() const;

    bool is_null() const;
    bool from_data(const data_chunk& data);
    bool from_data(std::istream& stream);
    bool from_data(reader& source);
    data_chunk to_data() const;
    void to_data(std::ostream& stream) const;
    void to_data(writer& sink) const;
    std::string to_string() const;
    bool is_valid() const;
    void reset();
    uint64_t serialized_size() const;

    point_byte_iterator begin() const;
    point_byte_iterator end() const;

    hash_digest hash;
    uint32_t index;
};

BC_API bool operator==(const point& left, const point& right);
BC_API bool operator!=(const point& left, const point& right);

typedef point input_point;
typedef point output_point;

struct BC_API points_info
{
    output_point::list points;
    uint64_t change;
};

} // namspace chain
} // namspace libbitcoin

#endif
