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
#ifndef LIBBITCOIN_SYSTEM_MESSAGE_GET_HEADERS_HPP
#define LIBBITCOIN_SYSTEM_MESSAGE_GET_HEADERS_HPP

#include <istream>
#include <memory>
#include <string>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/message/get_blocks.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

class BC_API get_headers
  : public get_blocks
{
public:
    typedef std::shared_ptr<get_headers> ptr;
    typedef std::shared_ptr<const get_headers> const_ptr;

    static get_headers factory(uint32_t version, const data_chunk& data);
    static get_headers factory(uint32_t version, std::istream& stream);
    static get_headers factory(uint32_t version, reader& source);

    get_headers();
    get_headers(const hash_list& start, const hash_digest& stop);
    get_headers(hash_list&& start, hash_digest&& stop);
    get_headers(const get_headers& other);
    get_headers(get_headers&& other);

    bool from_data(uint32_t version, const data_chunk& data) override;
    bool from_data(uint32_t version, std::istream& stream) override;
    bool from_data(uint32_t version, reader& source) override;

    // This class is move assignable but not copy assignable.
    get_headers& operator=(get_headers&& other);
    void operator=(const get_headers&) = delete;

    bool operator==(const get_headers& other) const;
    bool operator!=(const get_headers& other) const;

    static const std::string command;
    static const uint32_t version_minimum;
    static const uint32_t version_maximum;
};

} // namespace message
} // namespace system
} // namespace libbitcoin

#endif
