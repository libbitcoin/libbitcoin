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
#include <bitcoin/system/message/headers.hpp>

#include <algorithm>
#include <cstdint>
#include <initializer_list>
#include <istream>
#include <utility>
#include <bitcoin/system/message/inventory.hpp>
#include <bitcoin/system/message/inventory_vector.hpp>
#include <bitcoin/system/message/message.hpp>
#include <bitcoin/system/message/version.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

const std::string headers::command = "headers";
const uint32_t headers::version_minimum = version::level::headers;
const uint32_t headers::version_maximum = version::level::maximum;

headers headers::factory(uint32_t version, const data_chunk& data)
{
    headers instance;
    instance.from_data(version, data);
    return instance;
}

headers headers::factory(uint32_t version, std::istream& stream)
{
    headers instance;
    instance.from_data(version, stream);
    return instance;
}

headers headers::factory(uint32_t version, reader& source)
{
    headers instance;
    instance.from_data(version, source);
    return instance;
}

headers::headers()
  : elements_()
{
}

// Uses headers copy assignment.
headers::headers(const header::list& values)
  : elements_(values)
{
}

headers::headers(header::list&& values)
  : elements_(std::move(values))
{
}

headers::headers(const std::initializer_list<header>& values)
  : elements_(values)
{
}

headers::headers(const headers& other)
  : headers(other.elements_)
{
}

headers::headers(headers&& other)
  : headers(std::move(other.elements_))
{
}

bool headers::is_valid() const
{
    return !elements_.empty();
}

void headers::reset()
{
    elements_.clear();
    elements_.shrink_to_fit();
}

bool headers::from_data(uint32_t version, const data_chunk& data)
{
    stream::in::copy istream(data);
    return from_data(version, istream);
}

bool headers::from_data(uint32_t version, std::istream& stream)
{
    read::bytes::istream source(stream);
    return from_data(version, source);
}

bool headers::from_data(uint32_t version, reader& source)
{
    reset();

    const auto count = source.read_size();

    // Guard against potential for arbitrary memory allocation.
    if (count > chain::max_get_headers)
        source.invalidate();
    else
        elements_.resize(count);

    // Order is required.
    for (auto& element: elements_)
        if (!element.from_data(version, source))
            break;

    if (version < headers::version_minimum)
        source.invalidate();

    if (!source)
        reset();

    return source;
}

data_chunk headers::to_data(uint32_t version) const
{
    data_chunk data;
    const auto size = serialized_size(version);
    data.reserve(size);
    stream::out::data ostream(data);
    to_data(version, ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void headers::to_data(uint32_t version, std::ostream& stream) const
{
    write::bytes::ostream out(stream);
    to_data(version, out);
}

void headers::to_data(uint32_t version, writer& sink) const
{
    sink.write_variable(elements_.size());

    for (const auto& element: elements_)
        element.to_data(version, sink);
}

bool headers::is_sequential() const
{
    if (elements_.empty())
        return true;

    auto previous = elements_.front().hash();

    for (auto it = elements_.begin() + 1; it != elements_.end(); ++it)
    {
        if (it->previous_block_hash() != previous)
            return false;

        previous = it->hash();
    }

    return true;
}

void headers::to_hashes(hash_list& out) const
{
    out.clear();
    out.reserve(elements_.size());
    const auto map = [&out](const header& header)
    {
        out.push_back(header.hash());
    };

    std::for_each(elements_.begin(), elements_.end(), map);
}

void headers::to_inventory(inventory_vector::list& out,
    inventory::type_id type) const
{
    out.clear();
    out.reserve(elements_.size());
    const auto map = [&out, type](const header& header)
    {
        out.emplace_back(type, header.hash());
    };

    std::for_each(elements_.begin(), elements_.end(), map);
}

size_t headers::serialized_size(uint32_t version) const
{
    return variable_size(elements_.size()) +
        (elements_.size() * header::satoshi_fixed_size(version));
}

header::list& headers::elements()
{
    return elements_;
}

const header::list& headers::elements() const
{
    return elements_;
}

void headers::set_elements(const header::list& values)
{
    elements_ = values;
}

void headers::set_elements(header::list&& values)
{
    elements_ = std::move(values);
}

headers& headers::operator=(headers&& other)
{
    elements_ = std::move(other.elements_);
    return *this;
}

bool headers::operator==(const headers& other) const
{
    return (elements_ == other.elements_);
}

bool headers::operator!=(const headers& other) const
{
    return !(*this == other);
}

} // namespace message
} // namespace system
} // namespace libbitcoin
