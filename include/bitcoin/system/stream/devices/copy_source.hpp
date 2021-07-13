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
#ifndef LIBBITCOIN_SYSTEM_STREAM_DEVICES_COPY_SOURCE_HPP
#define LIBBITCOIN_SYSTEM_STREAM_DEVICES_COPY_SOURCE_HPP

#include <utility>
#include <boost/iostreams/stream.hpp>
#include <bitcoin/system/constraints.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/math/limits.hpp>
#include <bitcoin/system/stream/device.hpp>

namespace libbitcoin {
namespace system {

/// Source for ios::stream, copies bytes from Container.
template <typename Container, if_base_of<data_reference, Container> = true>
class copy_source
  : public device<Container>
{
public:
    typedef const Container& container;
    struct category
      : ios::input_seekable, ios::direct_tag
    {
    };

    copy_source(const Container& data) noexcept
      : device(limit<size_type>(data.size())),
        container_(data),
        next_(data.begin())
    {
    }

protected:
    sequence do_sequence() const noexcept override
    {
        // input_sequence/output_sequence both require non-const buffer ptrs,
        // but the data member is const, so we must cast it for direct devices.
        const auto begin = const_cast<value_type*>(container_.begin());
        const auto end = const_cast<value_type*>(container_.end());

        return std::make_pair(
            reinterpret_cast<char_type*>(begin),
            reinterpret_cast<char_type*>(end));
    }

private:
    const Container& container_;
    typename Container::const_iterator next_;
};

} // namespace system
} // namespace libbitcoin

#endif
