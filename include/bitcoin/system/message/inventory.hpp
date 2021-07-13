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
#ifndef LIBBITCOIN_SYSTEM_MESSAGE_INVENTORY_HPP
#define LIBBITCOIN_SYSTEM_MESSAGE_INVENTORY_HPP

#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <istream>
#include <memory>
#include <string>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/message/inventory_vector.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

class BC_API inventory
{
public:
    typedef std::shared_ptr<inventory> ptr;
    typedef std::shared_ptr<const inventory> const_ptr;
    typedef inventory_vector::type_id type_id;

    static inventory factory(uint32_t version, const data_chunk& data);
    static inventory factory(uint32_t version, std::istream& stream);
    static inventory factory(uint32_t version, reader& source);

    inventory();
    inventory(const inventory_vector::list& values);
    inventory(inventory_vector::list&& values);
    inventory(const hash_list& hashes, type_id type);
    inventory(const std::initializer_list<inventory_vector>& values);
    inventory(const inventory& other);
    inventory(inventory&& other);

    inventory_vector::list& inventories();
    const inventory_vector::list& inventories() const;
    void set_inventories(const inventory_vector::list& value);
    void set_inventories(inventory_vector::list&& value);

    virtual bool from_data(uint32_t version, const data_chunk& data);
    virtual bool from_data(uint32_t version, std::istream& stream);
    virtual bool from_data(uint32_t version, reader& source);
    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, std::ostream& stream) const;
    void to_data(uint32_t version, writer& sink) const;
    void to_hashes(hash_list& out, type_id type) const;
    void reduce(inventory_vector::list& out, type_id type) const;
    bool is_valid() const;
    void reset();
    size_t serialized_size(uint32_t version) const;
    size_t count(type_id type) const;

    // This class is move assignable but not copy assignable.
    inventory& operator=(inventory&& other);
    void operator=(const inventory&) = delete;

    bool operator==(const inventory& other) const;
    bool operator!=(const inventory& other) const;

    static const std::string command;
    static const uint32_t version_minimum;
    static const uint32_t version_maximum;

private:
    inventory_vector::list inventories_;
};

} // namespace message
} // namespace system
} // namespace libbitcoin

#endif
