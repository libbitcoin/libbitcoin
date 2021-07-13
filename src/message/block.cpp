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
#include <bitcoin/system/message/block.hpp>

#include <cstdint>
#include <cstddef>
#include <istream>
#include <utility>
#include <bitcoin/system/chain/chain.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/message/version.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

const std::string block::command = "block";
const uint32_t block::version_minimum = version::level::minimum;
const uint32_t block::version_maximum = version::level::maximum;

block block::factory(uint32_t version, const data_chunk& data)
{
    block instance;
    instance.from_data(version, data);
    return instance;
}

block block::factory(uint32_t version, std::istream& stream)
{
    block instance;
    instance.from_data(version, stream);
    return instance;
}

block block::factory(uint32_t version, reader& source)
{
    block instance;
    instance.from_data(version, source);
    return instance;
}

block::block()
  : chain::block()
{
}

block::block(block&& other)
  : chain::block(std::move(other))
{
}

block::block(const block& other)
  : chain::block(other)
{
}

block::block(chain::block&& other)
  : chain::block(std::move(other))
{
}

block::block(const chain::block& other)
  : chain::block(other)
{
}

block::block(chain::header&& header, chain::transaction::list&& transactions)
  : chain::block(std::move(header), std::move(transactions))
{
}

block::block(const chain::header& header,
    const chain::transaction::list& transactions)
  : chain::block(header, transactions)
{
}

// Witness is always deserialized if present.

bool block::from_data(uint32_t, const data_chunk& data)
{
    return chain::block::from_data(data, true);
}

bool block::from_data(uint32_t, std::istream& stream)
{
    return chain::block::from_data(stream, true);
}

bool block::from_data(uint32_t, reader& source)
{
    return chain::block::from_data(source, true);
}

// Witness is always serialized if present.

data_chunk block::to_data(uint32_t) const
{
    return chain::block::to_data(true);
}

void block::to_data(uint32_t, std::ostream& stream) const
{
    chain::block::to_data(stream, true);
}

void block::to_data(uint32_t, writer& sink) const
{
    chain::block::to_data(sink, true);
}

// Witness size is always counted if present.

size_t block::serialized_size(uint32_t) const
{
    return chain::block::serialized_size(true);
}

block& block::operator=(chain::block&& other)
{
    reset();
    chain::block::operator=(std::move(other));
    return *this;
}

block& block::operator=(block&& other)
{
    chain::block::operator=(std::move(other));
    return *this;
}

bool block::operator==(const chain::block& other) const
{
    return chain::block::operator==(other);
}

bool block::operator!=(const chain::block& other) const
{
    return chain::block::operator!=(other);
}

bool block::operator==(const block& other) const
{
    return chain::block::operator==(other);
}

bool block::operator!=(const block& other) const
{
    return !(*this == other);
}

} // namespace message
} // namespace system
} // namespace libbitcoin
