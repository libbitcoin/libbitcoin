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
#ifndef LIBBITCOIN_SYSTEM_MESSAGE_NOT_FOUND_HPP
#define LIBBITCOIN_SYSTEM_MESSAGE_NOT_FOUND_HPP

#include <initializer_list>
#include <istream>
#include <memory>
#include <string>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/message/inventory.hpp>
#include <bitcoin/system/message/inventory_vector.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

class BC_API not_found
  : public inventory
{
public:
    typedef std::shared_ptr<not_found> ptr;
    typedef std::shared_ptr<const not_found> const_ptr;

    static not_found factory(uint32_t version,const data_chunk& data);
    static not_found factory(uint32_t version, std::istream& stream);
    static not_found factory(uint32_t version, reader& source);

    not_found();
    not_found(const inventory_vector::list& values);
    not_found(inventory_vector::list&& values);
    not_found(const hash_list& hashes, type_id type);
    not_found(const std::initializer_list<inventory_vector>& values);
    not_found(const not_found& other);
    not_found(not_found&& other);

    bool from_data(uint32_t version, const data_chunk& data) override;
    bool from_data(uint32_t version, std::istream& stream) override;
    bool from_data(uint32_t version, reader& source) override;

    // This class is move assignable but not copy assignable.
    not_found& operator=(not_found&& other);
    void operator=(const not_found&) = delete;

    bool operator==(const not_found& other) const;
    bool operator!=(const not_found& other) const;

    static const std::string command;
    static const uint32_t version_minimum;
    static const uint32_t version_maximum;
};

} // namespace message
} // namespace system
} // namespace libbitcoin

#endif
