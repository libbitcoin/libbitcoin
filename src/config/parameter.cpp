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
#include <bitcoin/system/config/parameter.hpp>

#include <boost/program_options.hpp>
#include <bitcoin/system/data/data.hpp>

namespace po = boost::program_options;
using namespace libbitcoin::system::config;

const int parameter::not_positional = -1;
const char parameter::no_short_name = 0x00;
const char parameter::option_prefix_char = '-';

parameter::~parameter()
{
}

// 100% component coverage, common scenarios.
// A required argument may only be preceded by required arguments.
// Requiredness may be in error if the metadata is inconsistent.
void parameter::initialize(const po::option_description& option,
    const argument_list& arguments)
{
    set_position(position(option, arguments));
    set_args_limit(arguments_limit(position(), option, arguments));
    set_required(option.semantic()->is_required());
    set_long_name(option.long_name());
    set_short_name(short_name(option));
    set_description(option.description());
    set_format_name(option.format_name());
    set_format_parameter(option.format_parameter());
}

// 100% component coverage, all three scenarios (long, short, both)
int parameter::position(const po::option_description& option,
    const argument_list& arguments) const
{
    return static_cast<int>(find_pair_position(arguments, option.long_name()));
}

// 100% unit coverage, all three scenarios (long, short, both)
char parameter::short_name(const po::option_description& option) const
{
    // This call requires boost 1.50, don't use it.
    //auto name = option.canonical_display_name(
    //    search_options::dashed_short_prefer_short);

    // This is a substitute that allows us to use boost 1.49 for libbitcoin.
    const auto name = split(option.format_name()).front();
    const auto is_short_name = name[0] == option_prefix_char &&
        name[1] != option_prefix_char;

    return is_short_name ? name[1] : no_short_name;
}

// 100% component coverage
unsigned parameter::arguments_limit(int position,
    const po::option_description& option, const argument_list& arguments) const
{
    if (position == parameter::not_positional)
        return option.semantic()->max_tokens();

    return arguments[position].second;
}
