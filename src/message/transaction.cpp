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
#include <bitcoin/system/message/transaction.hpp>

#include <istream>
#include <utility>
#include <bitcoin/system/chain/chain.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/message/version.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

const std::string transaction::command = "tx";
const uint32_t transaction::version_minimum = version::level::minimum;
const uint32_t transaction::version_maximum = version::level::maximum;

transaction transaction::factory(uint32_t version,
    const data_chunk& data)
{
    transaction instance;
    instance.from_data(version, data);
    return instance;
}

transaction transaction::factory(uint32_t version,
    std::istream& stream)
{
    transaction instance;
    instance.from_data(version, stream);
    return instance;
}

transaction transaction::factory(uint32_t version,
    reader& source)
{
    transaction instance;
    instance.from_data(version, source);
    return instance;
}

transaction::transaction()
  : chain::transaction()
{
}

transaction::transaction(transaction&& other)
  : chain::transaction(std::move(other))
{
}

transaction::transaction(const transaction& other)
  : chain::transaction(other)
{
}

transaction::transaction(chain::transaction&& other)
  : chain::transaction(std::move(other))
{
}

transaction::transaction(const chain::transaction& other)
  : chain::transaction(other)
{
}

transaction::transaction(uint32_t version, uint32_t locktime,
    chain::input::list&& inputs, chain::output::list&& outputs)
  : chain::transaction(version, locktime, std::move(inputs),
        std::move(outputs))
{
}

transaction::transaction(uint32_t version, uint32_t locktime,
    const chain::input::list& inputs, const chain::output::list& outputs)
  : chain::transaction(version, locktime, inputs, outputs)
{
}

// Witness is always deserialized if present.

bool transaction::from_data(uint32_t, const data_chunk& data)
{
    return chain::transaction::from_data(data, true, true);
}

bool transaction::from_data(uint32_t, std::istream& stream)
{
    return chain::transaction::from_data(stream, true, true);
}

bool transaction::from_data(uint32_t, reader& source)
{
    return chain::transaction::from_data(source, true, true);
}

// Witness is always serialized if present.

data_chunk transaction::to_data(uint32_t) const
{
    return chain::transaction::to_data(true, true);
}

void transaction::to_data(uint32_t, std::ostream& stream) const
{
    chain::transaction::to_data(stream, true, true);
}

void transaction::to_data(uint32_t, writer& sink) const
{
    chain::transaction::to_data(sink, true, true);
}

// Witness size is always counted if present.

size_t transaction::serialized_size(uint32_t) const
{
    return chain::transaction::serialized_size(true, true);
}

transaction& transaction::operator=(chain::transaction&& other)
{
    reset();
    chain::transaction::operator=(std::move(other));
    return *this;
}

transaction& transaction::operator=(transaction&& other)
{
    chain::transaction::operator=(std::move(other));
    return *this;
}

bool transaction::operator==(const chain::transaction& other) const
{
    return chain::transaction::operator==(other);
}

bool transaction::operator!=(const chain::transaction& other) const
{
    return chain::transaction::operator!=(other);
}

bool transaction::operator==(const transaction& other) const
{
    return chain::transaction::operator==(other);
}

bool transaction::operator!=(const transaction& other) const
{
    return !(*this == other);
}

} // namespace message
} // namespace system
} // namespace libbitcoin
