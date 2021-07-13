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
#include <bitcoin/system/config/output.hpp>

#include <cstdint>
#include <sstream>
#include <string>
#include <bitcoin/system/config/point.hpp>
#include <bitcoin/system/config/script.hpp>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/exceptions.hpp>
#include <bitcoin/system/wallet/wallet.hpp>
#include <bitcoin/system/serial/serial.hpp>

namespace libbitcoin {
namespace system {
namespace config {

using namespace boost::program_options;

output::output()
  : is_stealth_(false), amount_(0), version_(0), script_(),
    pay_to_hash_(null_short_hash)
{
}

output::output(const std::string& tuple)
  : output()
{
    std::stringstream(tuple) >> *this;
}

bool output::is_stealth() const
{
    return is_stealth_;
}

uint64_t output::amount() const
{
    return amount_;
}

uint8_t output::version() const
{
    return version_;
}

const chain::script& output::script() const
{
    return script_;
}

const short_hash& output::pay_to_hash() const
{
    return pay_to_hash_;
}

std::istream& operator>>(std::istream& input, output& argument)
{
    std::string tuple;
    input >> tuple;

    const auto tokens = split(tuple, point::delimiter);
    if (tokens.size() < 2 || tokens.size() > 3)
        throw istream_exception(tuple);

    uint64_t amount;
    deserialize(amount, tokens[1]);

    argument.amount_ = amount;
    const auto& target = tokens.front();

    // Is the target a payment address?
    const wallet::payment_address payment(target);
    if (payment)
    {
        argument.version_ = payment.prefix();
        argument.pay_to_hash_ = payment.hash();
        return input;
    }

    // TODO: remove stealth outputs.
    // Is the target a stealth address?
    const wallet::stealth_address stealth(target);
    if (stealth)
    {
        if (stealth.spend_keys().size() != 1 || tokens.size() != 3)
            throw istream_exception(tuple);

        data_chunk seed;
        if (!decode_base16(seed, tokens[2]) ||
            seed.size() < chain::minimum_seed_size)
            throw istream_exception(tuple);

        ec_secret ephemeral_secret;
        if (!create_stealth_data(argument.script_, ephemeral_secret,
            stealth.filter(), seed))
            throw istream_exception(tuple);

        ec_compressed stealth_key;
        if (!uncover_stealth(stealth_key, stealth.scan_key(), ephemeral_secret,
            stealth.spend_keys().front()))
            throw istream_exception(tuple);

        argument.is_stealth_ = true;
        argument.pay_to_hash_ = bitcoin_short_hash(stealth_key);
        argument.version_ = stealth.version();
        return input;
    }

    // The target must be a serialized script.
    // Note that it is possible for a base16 encoded script to be interpreted
    // as an address above. That is unlikely but considered intended behavior.
    data_chunk decoded;
    if (!decode_base16(decoded, target))
        throw istream_exception(target);

    argument.script_ = script(decoded);
    return input;
}

} // namespace config
} // namespace system
} // namespace libbitcoin
