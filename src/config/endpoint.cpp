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
#include <bitcoin/system/config/endpoint.hpp>

#include <cstdint>
#include <string>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <bitcoin/system/config/endpoint.hpp>
#include <bitcoin/system/exceptions.hpp>

namespace libbitcoin {
namespace system {
namespace config {

endpoint::endpoint()
  : endpoint("localhost")
{
}

endpoint::endpoint(const endpoint& other)
  : scheme_(other.scheme()), host_(other.host()), port_(other.port())
{
}

endpoint::endpoint(const std::string& value)
{
    std::stringstream(value) >> *this;
}

endpoint::endpoint(const authority& authority)
  : endpoint(authority.to_string())
{
}

endpoint::endpoint(const std::string& host, uint16_t port)
  : host_(host), port_(port)
{
}

endpoint::endpoint(const std::string& scheme, const std::string& host,
    uint16_t port)
  : scheme_(scheme), host_(host), port_(port)
{
}

endpoint::endpoint(const boost::asio::ip::tcp::endpoint& host)
  : endpoint(host.address(), host.port())
{
}

endpoint::endpoint(const boost::asio::ip::address& ip, uint16_t port)
  : host_(ip.to_string()), port_(port)
{
}

const std::string& endpoint::scheme() const
{
    return scheme_;
}

const std::string& endpoint::host() const
{
    return host_;
}

uint16_t endpoint::port() const
{
    return port_;
}

std::string endpoint::to_string() const
{
    std::stringstream value;
    value << *this;
    return value.str();
}

endpoint endpoint::to_local() const
{
    const auto host = host_ == "*" ? "localhost" : host_;
    return endpoint(scheme_, host, port_);
}

endpoint::operator bool() const
{
    // Return true if initialized.
    // TODO: this is a quick hack, along with http/https.
    return !scheme_.empty();
}

bool endpoint::operator==(const endpoint& other) const
{
    return host_ == other.host_ && port_ == other.port_ &&
        scheme_ == other.scheme_;
}

std::istream& operator>>(std::istream& input, endpoint& argument)
{
    std::string value;
    input >> value;

    // std::regex requires gcc 4.9, so we are using boost::regex for now.
    static const boost::regex regular("^((tcp|udp|http|https|inproc):\\/\\/)?"
        "(\\[([0-9a-f:\\.]+)]|([^:]+))(:([0-9]{1,5}))?$");

    boost::sregex_iterator it(value.begin(), value.end(), regular), end;
    if (it == end)
        throw istream_exception(value);

    const auto& match = *it;
    argument.scheme_ = match[2];
    argument.host_ = match[3];
    std::string port(match[7]);

    try
    {
        argument.port_ = port.empty() ? 0u : boost::lexical_cast<uint16_t>(port);
    }
    catch (const boost::exception&)
    {
        throw istream_exception(value);
    }

    return input;
}

std::ostream& operator<<(std::ostream& output, const endpoint& argument)
{
    if (!argument.scheme().empty())
        output << argument.scheme() << "://";

    output << argument.host();

    if (argument.port() != 0u)
        output << ":" << argument.port();

    return output;
}

} // namespace config
} // namespace system
} // namespace libbitcoin
