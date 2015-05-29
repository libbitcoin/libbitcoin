/*
 * Copyright (c) 2011-2013 libbitcoin developers (see AUTHORS)
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
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <system_error>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <bitcoin/bitcoin/error.hpp>
#include <bitcoin/bitcoin/formats/base16.hpp>
#include <bitcoin/bitcoin/network/hosts.hpp>
#include <bitcoin/bitcoin/unicode/ifstream.hpp>
#include <bitcoin/bitcoin/unicode/ofstream.hpp>
#include <bitcoin/bitcoin/unicode/unicode.hpp>
#include <bitcoin/bitcoin/utility/async_strand.hpp>
#include <bitcoin/bitcoin/utility/string.hpp>
#include <bitcoin/bitcoin/utility/threadpool.hpp>

namespace libbitcoin {
namespace network {

using boost::filesystem::path;

hosts::hosts(threadpool& pool, const std::string& path, size_t capacity)
  : strand_(pool), hosts_path_(path), buffer_(capacity)
{
    srand(static_cast<uint32_t>(time(nullptr)));
}

/// Deprecated, set path on construct.
hosts::hosts(threadpool& pool, size_t capacity)
  : hosts(pool, "hosts.p2p", capacity)
{
    //BITCOIN_ASSERT_MSG(false, "hosts::hosts deprecated");
}
hosts::~hosts()
{
}

void hosts::load(load_handler handle_load)
{
    strand_.randomly_queue(
        &hosts::do_load, this, hosts_path_.string(), handle_load);
}

/// Deprecated, set path on construct.
void hosts::load(const std::string& path, load_handler handle_load)
{
    //BITCOIN_ASSERT_MSG(false,
    //    "hosts::load deprecated, set path on construct");
    strand_.randomly_queue(&hosts::do_load, this, path, handle_load);
}
void hosts::do_load(const path& path, load_handler handle_load)
{
    bc::ifstream file(path.string());
    if (file.bad())
    {
        const auto error = std::make_error_code(std::errc::io_error);
        handle_load(error);
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        auto parts = split(line);
        if (parts.size() != 2)
            continue;
        data_chunk raw_ip;
        if (!decode_base16(raw_ip, parts[0]))
            continue;
        hosts_field field;
        if (raw_ip.size() != field.ip.size())
            continue;
        std::copy(raw_ip.begin(), raw_ip.end(), field.ip.begin());

        boost::trim(parts[1]);
        field.port = boost::lexical_cast<uint16_t>(parts[1]);
        strand_.randomly_queue(
            [this, field]()
            {
                buffer_.push_back(field);
            });
    }

    handle_load(std::error_code());
}

void hosts::save(save_handler handle_save)
{
    strand_.randomly_queue(
        std::bind(&hosts::do_save, this, hosts_path_.string(), handle_save));
}

/// Deprecated, set path on construct.
void hosts::save(const std::string& path, save_handler handle_save)
{
    //BITCOIN_ASSERT_MSG(false,
    //    "hosts::save deprecated, set path on construct");
    strand_.randomly_queue(
        std::bind(&hosts::do_save, this, path, handle_save));
}
void hosts::do_save(const path& path, save_handler handle_save)
{
    bc::ofstream file(path.string());
    if (file.bad())
    {
        const auto error = std::make_error_code(std::errc::io_error);
        handle_save(error);
        return;
    }

    for (const hosts_field& field: buffer_)
        file << encode_base16(field.ip) << ' ' << field.port << std::endl;
    handle_save(std::error_code());
}

void hosts::store(const message::network_address& address,
    store_handler handle_store)
{
    strand_.randomly_queue(
        [this, address, handle_store]()
        {
            buffer_.push_back(hosts_field{address.ip, address.port});
            handle_store(std::error_code());
        });
}

void hosts::remove(const message::network_address& address,
    remove_handler handle_remove)
{
    strand_.randomly_queue(
        std::bind(&hosts::do_remove,
            this, address, handle_remove));
}
bool hosts::hosts_field::operator==(const hosts_field& other)
{
    return ip == other.ip && port == other.port;
}
void hosts::do_remove(const message::network_address& address,
    remove_handler handle_remove)
{
    auto it = std::find(buffer_.begin(), buffer_.end(),
        hosts_field{address.ip, address.port});
    if (it == buffer_.end())
    {
        handle_remove(error::not_found);
        return;
    }

    buffer_.erase(it);
    handle_remove(std::error_code());
}

void hosts::fetch_address(fetch_address_handler handle_fetch)
{
    strand_.randomly_queue(
        std::bind(&hosts::do_fetch_address,
            this, handle_fetch));
}
void hosts::do_fetch_address(fetch_address_handler handle_fetch)
{
    if (buffer_.empty())
    {
        handle_fetch(error::not_found, message::network_address());
        return;
    }

    size_t index = rand() % buffer_.size();
    message::network_address address;
    address.timestamp = 0;
    address.services = 0;
    address.ip = buffer_[index].ip;
    address.port = buffer_[index].port;
    handle_fetch(std::error_code(), address);
}

void hosts::fetch_count(fetch_count_handler handle_fetch)
{
    strand_.randomly_queue(
        std::bind(&hosts::do_fetch_count,
            this, handle_fetch));
}
void hosts::do_fetch_count(fetch_count_handler handle_fetch)
{
    handle_fetch(std::error_code(), buffer_.size());
}

} // namespace network
} // namespace libbitcoin

