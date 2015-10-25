/**
 * Copyright (c) 2011-2015 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBBITCOIN_NETWORK_PROTOCOL_BASE_HPP
#define LIBBITCOIN_NETWORK_PROTOCOL_BASE_HPP

#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <bitcoin/bitcoin/network/asio.hpp>
#include <bitcoin/bitcoin/network/channel.hpp>
#include <bitcoin/bitcoin/network/protocol.hpp>
#include <bitcoin/bitcoin/utility/deadline.hpp>
#include <bitcoin/bitcoin/utility/dispatcher.hpp>
#include <bitcoin/bitcoin/utility/threadpool.hpp>

namespace libbitcoin {
namespace network {

/**
 * Templated intermediate base class for protocol implementations.
 * This simplifies calling bind, send and subscribe.
 */
template <class Protocol>
class protocol_base
  : public protocol
{
protected:
    /**
     * Construct a base protocol instance.
     * @param[in]  pool     The thread pool used by the dispacher.
     * @param[in]  channel  The channel on which to start the protocol.
     * @param[in]  name     The instance name for logging purposes.
     * @param[in]  handler  Callback invoked upon stop if not null.
     */
    protocol_base(threadpool& pool, channel::ptr channel,
        const std::string& name, completion_handler handler=nullptr)
      : protocol(pool, channel, name, handler),
        weak_channel_(channel)
    {
    }

    /**
     * Construct a base protocol instance.
     * @param[in]  pool     The thread pool used by the dispacher.
     * @param[in]  channel  The channel on which to start the protocol.
     * @param[in]  timeout  The timer period.
     * @param[in]  name     The instance name for logging purposes.
     * @param[in]  handler  Callback invoked upon stop if not null.
     */
    protocol_base(threadpool& pool, channel::ptr channel,
        const asio::duration& timeout, const std::string& name,
        completion_handler handler=nullptr)
      : protocol_base(pool, channel, name, handler)
    {
    }

    template <typename Handler, typename... Args>
    auto bind(Handler&& handler, Args&&... args) ->
        decltype(std::bind(std::forward<Handler>(handler),
            std::shared_ptr<Protocol>(), std::forward<Args>(args)...))
    {
        return std::bind(std::forward<Handler>(handler),
            shared_from_base<Protocol>(), std::forward<Args>(args)...);
    }

    template <class Message, typename Handler, typename... Args>
    void send(Message&& packet, Handler&& handler, Args&&... args)
    {
        auto channel = weak_channel_.lock();
        if (channel)
            channel->send(std::forward<Message>(packet),
                dispatch_.ordered_delegate(std::forward<Handler>(handler),
                    shared_from_base<Protocol>(), 
                        std::forward<Args>(args)...));
    }

    template <class Message, typename Handler, typename... Args>
    void subscribe(Handler&& handler, Args&&... args)
    {
        auto channel = weak_channel_.lock();
        if (channel)
            channel->template subscribe<Message>(
                dispatch_.ordered_delegate(std::forward<Handler>(handler),
                    shared_from_base<Protocol>(),
                        std::forward<Args>(args)...));
    }

private:

    // A weak reference is used to allow disposal of the instance.
    // A copy use used in order to prohibit access by derived protocols.
    std::weak_ptr<channel> weak_channel_;
};

} // namespace network
} // namespace libbitcoin

#endif
