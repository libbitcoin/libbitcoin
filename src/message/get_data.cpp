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
#include <bitcoin/system/message/get_data.hpp>

#include <algorithm>
#include <initializer_list>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/message/inventory.hpp>
#include <bitcoin/system/message/inventory_vector.hpp>
#include <bitcoin/system/message/version.hpp>

namespace libbitcoin {
namespace system {
namespace message {

const std::string get_data::command = "getdata";
const uint32_t get_data::version_minimum = version::level::minimum;
const uint32_t get_data::version_maximum = version::level::maximum;

get_data get_data::factory(uint32_t version,
    const data_chunk& data)
{
    get_data instance;
    instance.from_data(version,data);
    return instance;
}

get_data get_data::factory(uint32_t version,
    std::istream& stream)
{
    get_data instance;
    instance.from_data(version, stream);
    return instance;
}

get_data get_data::factory(uint32_t version,
    reader& source)
{
    get_data instance;
    instance.from_data(version, source);
    return instance;
}

get_data::get_data()
  : inventory()
{
}

get_data::get_data(const inventory_vector::list& values)
  : inventory(values)
{
}

get_data::get_data(inventory_vector::list&& values)
  : inventory(values)
{
}

get_data::get_data(const hash_list& hashes, inventory::type_id type)
  : inventory(hashes, type)
{
}

get_data::get_data(const std::initializer_list<inventory_vector>& values)
  : inventory(values)
{
}

get_data::get_data(const get_data& other)
  : inventory(other)
{
}

get_data::get_data(get_data&& other)
  : inventory(other)
{
}

bool get_data::from_data(uint32_t version, const data_chunk& data)
{
    return inventory::from_data(version, data);
}

bool get_data::from_data(uint32_t version, std::istream& stream)
{
    return inventory::from_data(version, stream);
}

bool get_data::from_data(uint32_t version, reader& source)
{
    if (!inventory::from_data(version, source))
        return false;

    if (version < get_data::version_minimum)
        source.invalidate();

    if (!source)
        reset();

    return source;
}

void get_data::to_witness()
{
    const auto convert = [](inventory_vector& element)
    {
        element.to_witness();
    };

    std::for_each(inventories().begin(), inventories().end(), convert);
}

get_data& get_data::operator=(get_data&& other)
{
    set_inventories(other.inventories());
    return *this;
}

bool get_data::operator==(const get_data& other) const
{
    return (static_cast<inventory>(*this) == static_cast<inventory>(other));
}

bool get_data::operator!=(const get_data& other) const
{
    return (static_cast<inventory>(*this) != static_cast<inventory>(other));
}

} // namespace message
} // namespace system
} // namespace libbitcoin
