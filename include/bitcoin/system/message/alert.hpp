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
#ifndef LIBBITCOIN_SYSTEM_MESSAGE_ALERT_HPP
#define LIBBITCOIN_SYSTEM_MESSAGE_ALERT_HPP

#include <istream>
#include <memory>
#include <string>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

class BC_API alert
{
public:
    typedef std::shared_ptr<alert> ptr;
    typedef std::shared_ptr<const alert> const_ptr;

    static alert factory(uint32_t version, const data_chunk& data);
    static alert factory(uint32_t version, std::istream& stream);
    static alert factory(uint32_t version, reader& source);

    alert();
    alert(const data_chunk& payload, const data_chunk& signature);
    alert(data_chunk&& payload, data_chunk&& signature);
    alert(const alert& other);
    alert(alert&& other);

    data_chunk& payload();
    const data_chunk& payload() const;
    void set_payload(const data_chunk& value);
    void set_payload(data_chunk&& value);

    data_chunk& signature();
    const  data_chunk& signature() const;
    void set_signature(const data_chunk& value);
    void set_signature(data_chunk&& value);

    bool from_data(uint32_t version, const data_chunk& data);
    bool from_data(uint32_t version, std::istream& stream);
    bool from_data(uint32_t version, reader& source);
    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, std::ostream& stream) const;
    void to_data(uint32_t version, writer& sink) const;
    bool is_valid() const;
    void reset();
    size_t serialized_size(uint32_t version) const;

    /// This class is move assignable but not copy assignable.
    alert& operator=(alert&& other);
    void operator=(const alert&) = delete;

    bool operator==(const alert& other) const;
    bool operator!=(const alert& other) const;

    static const std::string command;
    static const uint32_t version_minimum;
    static const uint32_t version_maximum;

private:
    data_chunk payload_;
    data_chunk signature_;
};

} // namespace message
} // namespace system
} // namespace libbitcoin

#endif
