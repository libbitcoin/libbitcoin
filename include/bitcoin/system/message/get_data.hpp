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
#ifndef LIBBITCOIN_SYSTEM_MESSAGE_GET_DATA_HPP
#define LIBBITCOIN_SYSTEM_MESSAGE_GET_DATA_HPP

#include <initializer_list>
#include <memory>
#include <istream>
#include <string>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/message/inventory.hpp>
#include <bitcoin/system/message/inventory_vector.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

class BC_API get_data
  : public inventory
{
public:
    typedef std::shared_ptr<get_data> ptr;
    typedef std::shared_ptr<const get_data> const_ptr;

    static get_data factory(uint32_t version, const data_chunk& data);
    static get_data factory(uint32_t version, std::istream& stream);
    static get_data factory(uint32_t version, reader& source);

    get_data();
    get_data(const inventory_vector::list& list);
    get_data(inventory_vector::list&& list);
    get_data(const hash_list& hashes, type_id type);
    get_data(const std::initializer_list<inventory_vector>& elements);
    get_data(const get_data& other);
    get_data(get_data&& other);

    bool from_data(uint32_t version, const data_chunk& data) override;
    bool from_data(uint32_t version, std::istream& stream) override;
    bool from_data(uint32_t version, reader& source) override;

    /// Convert message types to witness types.
    void to_witness();

    // This class is move assignable but not copy assignable.
    get_data& operator=(get_data&& other);
    void operator=(const get_data&) = delete;

    bool operator==(const get_data& other) const;
    bool operator!=(const get_data& other) const;

    static const std::string command;
    static const uint32_t version_minimum;
    static const uint32_t version_maximum;
};

} // namespace message
} // namespace system
} // namespace libbitcoin

#endif
