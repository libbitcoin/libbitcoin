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
#ifndef LIBBITCOIN_SYSTEM_MESSAGE_ALERT_PAYLOAD_HPP
#define LIBBITCOIN_SYSTEM_MESSAGE_ALERT_PAYLOAD_HPP

#include <istream>
#include <string>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/stream/stream.hpp>

namespace libbitcoin {
namespace system {
namespace message {

class BC_API alert_payload
{
public:
    static alert_payload factory(uint32_t version, const data_chunk& data);
    static alert_payload factory(uint32_t version, std::istream& stream);
    static alert_payload factory(uint32_t version, reader& source);

    alert_payload();
    alert_payload(uint32_t version, uint64_t relay_until, uint64_t expiration,
        uint32_t id, uint32_t cancel, const std::vector<uint32_t>& set_cancel,
        uint32_t min_version, uint32_t max_version,
        const std::vector<std::string>& set_sub_version, uint32_t priority,
        const std::string& comment, const std::string& status_bar,
        const std::string& reserved);
    alert_payload(uint32_t version, uint64_t relay_until, uint64_t expiration,
        uint32_t id, uint32_t cancel, std::vector<uint32_t>&& set_cancel,
        uint32_t min_version, uint32_t max_version,
        std::vector<std::string>&& set_sub_version, uint32_t priority,
        std::string&& comment, std::string&& status_bar,
        std::string&& reserved);
    alert_payload(const alert_payload& other);
    alert_payload(alert_payload&& other);

    uint32_t version() const;
    void set_version(uint32_t value);

    uint64_t relay_until() const;
    void set_relay_until(uint64_t value);

    uint64_t expiration() const;
    void set_expiration(uint64_t value);

    uint32_t id() const;
    void set_id(uint32_t value);

    uint32_t cancel() const;
    void set_cancel(uint32_t value);

    std::vector<uint32_t>& set_cancel();
    const std::vector<uint32_t>& set_cancel() const;
    void set_set_cancel(const std::vector<uint32_t>& value);
    void set_set_cancel(std::vector<uint32_t>&& value);

    uint32_t min_version() const;
    void set_min_version(uint32_t value);

    uint32_t max_version() const;
    void set_max_version(uint32_t value);

    std::vector<std::string>& set_sub_version();
    const std::vector<std::string>& set_sub_version() const;
    void set_set_sub_version(const std::vector<std::string>& value);
    void set_set_sub_version(std::vector<std::string>&& value);

    uint32_t priority() const;
    void set_priority(uint32_t value);

    std::string& comment();
    const std::string& comment() const;
    void set_comment(const std::string& value);
    void set_comment(std::string&& value);

    std::string& status_bar();
    const std::string& status_bar() const;
    void set_status_bar(const std::string& value);
    void set_status_bar(std::string&& value);

    std::string& reserved();
    const std::string& reserved() const;
    void set_reserved(const std::string& value);
    void set_reserved(std::string&& value);

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
    alert_payload& operator=(alert_payload&& other);
    void operator=(const alert_payload&) = delete;

    bool operator==(const alert_payload& other) const;
    bool operator!=(const alert_payload& other) const;

    static const ec_uncompressed satoshi_public_key;

private:
    uint32_t version_;
    uint64_t relay_until_;
    uint64_t expiration_;
    uint32_t id_;
    uint32_t cancel_;
    std::vector<uint32_t> set_cancel_;
    uint32_t min_version_;
    uint32_t max_version_;
    std::vector<std::string> set_sub_version_;
    uint32_t priority_;
    std::string comment_;
    std::string status_bar_;
    std::string reserved_;
};

} // namespace message
} // namespace system
} // namespace libbitcoin

#endif
