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
#include <bitcoin/system/message/not_found.hpp>

#include <initializer_list>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/message/inventory.hpp>
#include <bitcoin/system/message/version.hpp>

namespace libbitcoin {
namespace system {
namespace message {

const std::string not_found::command = "notfound";
const uint32_t not_found::version_minimum = version::level::bip37;
const uint32_t not_found::version_maximum = version::level::maximum;

not_found not_found::factory(uint32_t version,
    const data_chunk& data)
{
    not_found instance;
    instance.from_data(version, data);
    return instance;
}

not_found not_found::factory(uint32_t version,
    std::istream& stream)
{
    not_found instance;
    instance.from_data(version, stream);
    return instance;
}

not_found not_found::factory(uint32_t version,
    reader& source)
{
    not_found instance;
    instance.from_data(version, source);
    return instance;
}

not_found::not_found()
  : inventory()
{
}

not_found::not_found(const inventory_vector::list& values)
  : inventory(values)
{
}

not_found::not_found(inventory_vector::list&& values)
  : inventory(values)
{
}

not_found::not_found(const hash_list& hashes, inventory::type_id type)
  : inventory(hashes, type)
{
}

not_found::not_found(const std::initializer_list<inventory_vector>& values)
  : inventory(values)
{
}

not_found::not_found(const not_found& other)
  : inventory(other)
{
}

not_found::not_found(not_found&& other)
  : inventory(other)
{
}

bool not_found::from_data(uint32_t version, const data_chunk& data)
{
    return inventory::from_data(version, data);
}

bool not_found::from_data(uint32_t version, std::istream& stream)
{
    return inventory::from_data(version, stream);
}

bool not_found::from_data(uint32_t version, reader& source)
{
    if (!inventory::from_data(version, source))
        return false;

    if (version < not_found::version_minimum)
        source.invalidate();

    if (!source)
        reset();

    return source;
}

not_found& not_found::operator=(not_found&& other)
{
    set_inventories(other.inventories());
    return *this;
}

bool not_found::operator==(const not_found& other) const
{
    return (static_cast<inventory>(*this) == static_cast<inventory>(other));
}

bool not_found::operator!=(const not_found& other) const
{
    return (static_cast<inventory>(*this) != static_cast<inventory>(other));
}

} // namespace message
} // namespace system
} // namespace libbitcoin
