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
#ifndef LIBBITCOIN_SYSTEM_CONFIG_ENDPOINT_HPP
#define LIBBITCOIN_SYSTEM_CONFIG_ENDPOINT_HPP

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <bitcoin/system/config/authority.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/radix/radix.hpp>

namespace libbitcoin {
namespace system {
namespace config {

/**
 * Serialization helper for a network endpoint in URI format.
 * This is a container for a {scheme, host, port} tuple.
 */
class BC_API endpoint
{
public:
    /**
     * A list of endpoints.
     * This must provide operator<< for ostream in order to be used as a
     * boost::program_options default_value.
     */
    typedef std::vector<endpoint> list;

    /**
     * Default constructor.
     */
    endpoint();

    /**
     * Copy constructor.
     * @param[in]  other  The object to copy into self on construct.
     */
    endpoint(const endpoint& other);

    /**
     * Initialization constructor.
     * The scheme and port may be undefined, in which case the port is reported
     * as zero and the scheme is reported as an empty string.
     * @param[in]  value  The initial value of the [scheme://]host[:port] form.
     */
    endpoint(const std::string& value);

    /**
     * Initialization constructor.
     * @param[in]  authority  The value to initialize with.
     */
    endpoint(const authority& authority);

    /**
     * Initialization constructor.
     * @param[in]  host  The host name or ip address to initialize with.
     * @param[in]  port  The port to initialize with.
     */
    endpoint(const std::string& host, uint16_t port);

    /**
     * Initialization constructor.
     * @param[in]  scheme  The scheme to initialize with.
     * @param[in]  host    The host name or ip address to initialize with.
     * @param[in]  port    The port to initialize with.
     */
    endpoint(const std::string& scheme, const std::string& host,
        uint16_t port);

    /**
     * Initialization constructor.
     * @param[in]  endpoint  The endpoint addresss to initialize with.
     */
    endpoint(const boost::asio::ip::tcp::endpoint& host);

    /**
     * Initialization constructor.
     * @param[in]  ip    The boost ip addresss to initialize with.
     * @param[in]  port  The port to initialize with.
     */
    endpoint(const boost::asio::ip::address& ip, uint16_t port);

    /**
     * Getter.
     * @return True if the endpoint is initialized.
     */
    operator bool() const;

    /**
     * Getter.
     * @return The scheme of the endpoint or empty string.
     */
    const std::string& scheme() const;

    /**
     * Getter.
     * @return The host name or ip address of the endpoint.
     */
    const std::string& host() const;

    /**
     * Getter.
     * @return The tcp port of the endpoint.
     */
    uint16_t port() const;

    /**
     * Get the endpoint as a string.
     * An empty scheme and/or empty port is omitted.
     * @return The endpoint in the [scheme://]host[:port] form.
     */
    std::string to_string() const;

    /**
     * Return a new endpoint that replaces host instances of "*" with
     * "localhost".  This is intended for clients that wish to connect
     * to a service that has been configured to bind to all
     * interfaces.
     * @return The endpoint in the [scheme://]host[:port] form.
     */
    endpoint to_local() const;

    /**
     * Override the equality operator.
     * @param[in]  other  The other object with which to compare.
     */
    bool operator==(const endpoint& other) const;

    /**
     * Define stream in. Throws if input is invalid.
     * @param[in]   input     The input stream to read the value from.
     * @param[out]  argument  The object to receive the read value.
     * @return                The input stream reference.
     */
    friend std::istream& operator>>(std::istream& input,
        endpoint& argument);

    /**
     * Define stream out.
     * @param[in]   output    The output stream to write the value to.
     * @param[out]  argument  The object from which to obtain the value.
     * @return                The output stream reference.
     */
    friend std::ostream& operator<<(std::ostream& output,
        const endpoint& argument);

private:
    std::string scheme_;
    std::string host_;
    uint16_t port_;
};

} // namespace config
} // namespace system
} // namespace libbitcoin

#endif
