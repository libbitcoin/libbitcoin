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
#ifndef LIBBITCOIN_SYSTEM_STREAM_DEVICES_PUSH_SINK_HPP
#define LIBBITCOIN_SYSTEM_STREAM_DEVICES_PUSH_SINK_HPP

#include <iterator>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/constraints.hpp>
#include <bitcoin/system/math/addition.hpp>
#include <bitcoin/system/math/limits.hpp>
#include <bitcoin/system/stream/device.hpp>

namespace libbitcoin {
namespace system {

/// Sink for ios::stream, appends bytes to Container.
/// Container may be any insertable object with contiguous byte data.
/// This is limited to std::string and std::vector of uint8_t.
/// Push streams are buffered, indirect (inefficient) and require flush.
template <typename Container, if_byte_insertable<Container> = true>
class push_sink
  : public device<Container>
{
public:
    typedef Container& container;
    struct category
      : ios::sink_tag, ios::optimally_buffered_tag
    {
    };

    push_sink(Container& data) noexcept
      : device(limit<size_type>(data.max_size() - data.size())),
        container_(data),
        next_(data.end())
    {
    }

protected:
    const size_type default_buffer_size = 1024;

    void do_write(const value_type* from, size_type size) noexcept override
    {
        auto start = container_.insert(next_, from, std::next(from, size));
        next_ = std::next(start, size);
    }

    size_type do_optimal_buffer_size() const noexcept override
    {
        // This is only called at stream construct.
        // The compiler determines capacity, so this may be unreliable to test.
        // Create a buffer equal to the reserved but unused space, or default.
        auto space = floored_subtract(container_.capacity(), container_.size());
        return is_zero(space) ? default_buffer_size : space;
    }

private:
    Container& container_;
    typename Container::iterator next_;
};

} // namespace system
} // namespace libbitcoin

#endif
