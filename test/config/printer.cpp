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
#include "../test.hpp"
#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

BOOST_AUTO_TEST_SUITE(printer_tests)

namespace po = boost::program_options;
using namespace bc::system::config;

#define CONFIG_APPLICATION "APP"
#define CONFIG_DESCRIPTION "DESCRIPTION"
#define CONFIG_COMMAND "COMMAND"
//#define CONFIG_CATEGORY "CATEGORY"

#define CONFIG_PRINTER_SETUP_ARGUMENTS(initializer) \
    po::options_description options; \
    po::positional_options_description arguments; \
    initializer; \
    printer help(options, arguments, CONFIG_APPLICATION, CONFIG_DESCRIPTION, CONFIG_COMMAND)

#define CONFIG_PRINTER_SETUP() \
    CONFIG_PRINTER_SETUP_ARGUMENTS(options.add_options())

#define CONFIG_PRINTER_INITIALIZE(number_of_parameters, number_of_names) \
    help.initialize(); \
    BOOST_REQUIRE_EQUAL(help.get_parameters().size(), number_of_parameters); \
    BOOST_REQUIRE_EQUAL(help.get_argument_names().size(), number_of_names)

// ------------------------------------------------------------------------- //
BOOST_AUTO_TEST_SUITE(printer__columnize)

BOOST_AUTO_TEST_CASE(printer__columnize__paragraph_empty_width_0__empty)
{
    CONFIG_PRINTER_SETUP();
    auto rows = help.columnize("", 0);
    BOOST_REQUIRE_EQUAL(rows.size(), 0u);
}

BOOST_AUTO_TEST_CASE(printer__columnize__paragraph_empty_width_1__empty)
{
    CONFIG_PRINTER_SETUP();
    auto rows = help.columnize("", 1);
    BOOST_REQUIRE_EQUAL(rows.size(), 0u);
}

BOOST_AUTO_TEST_CASE(printer__columnize__short_word_width_10__one_word_row)
{
    CONFIG_PRINTER_SETUP();
    auto rows = help.columnize("foo", 10);
    BOOST_REQUIRE_EQUAL(rows.size(), 1u);
    BOOST_REQUIRE_EQUAL(rows.front(), "foo");
}

BOOST_AUTO_TEST_CASE(printer__columnize__two_short_words_width_10__two_word_row)
{
    CONFIG_PRINTER_SETUP();
    auto rows = help.columnize("foo bar", 10);
    BOOST_REQUIRE_EQUAL(rows.size(), 1u);
    BOOST_REQUIRE_EQUAL(rows.front(), "foo bar");
}

BOOST_AUTO_TEST_CASE(printer__columnize__overflow_width_10__two_rows)
{
    CONFIG_PRINTER_SETUP();
    auto rows = help.columnize("foo bar overflow", 10);
    BOOST_REQUIRE_EQUAL(rows.size(), 2u);
    BOOST_REQUIRE_EQUAL(rows[0], "foo bar");
    BOOST_REQUIRE_EQUAL(rows[1], "overflow");
}

BOOST_AUTO_TEST_CASE(printer__columnize__first_word_overflow_width_10__two_rows)
{
    CONFIG_PRINTER_SETUP();
    auto rows = help.columnize("morethantenchars foo bar", 10);
    BOOST_REQUIRE_EQUAL(rows.size(), 2u);
    BOOST_REQUIRE_EQUAL(rows[0], "morethantenchars");
    BOOST_REQUIRE_EQUAL(rows[1], "foo bar");
}

BOOST_AUTO_TEST_CASE(printer__columnize__middle_word_overflow_width_10__three_rows)
{
    CONFIG_PRINTER_SETUP();
    auto rows = help.columnize("foo bar morethantenchars test", 10);
    BOOST_REQUIRE_EQUAL(rows.size(), 3u);
    BOOST_REQUIRE_EQUAL(rows[0], "foo bar");
    BOOST_REQUIRE_EQUAL(rows[1], "morethantenchars");
    BOOST_REQUIRE_EQUAL(rows[2], "test");
}

BOOST_AUTO_TEST_CASE(printer__columnize__last_word_overflow_width_10__two_rows)
{
    CONFIG_PRINTER_SETUP();
    auto rows = help.columnize("foo bar morethantenchars", 10);
    BOOST_REQUIRE_EQUAL(rows.size(), 2u);
    BOOST_REQUIRE_EQUAL(rows[0], "foo bar");
    BOOST_REQUIRE_EQUAL(rows[1], "morethantenchars");
}

BOOST_AUTO_TEST_CASE(printer__columnize__excess_whitespace_width_10__space_removed)
{
    CONFIG_PRINTER_SETUP();
    auto rows = help.columnize("  \tfoo   bar \n\n  morethantenchars\r\n  ", 10);
    BOOST_REQUIRE_EQUAL(rows.size(), 3u);
    BOOST_REQUIRE_EQUAL(rows[0], "\tfoo bar");
    BOOST_REQUIRE_EQUAL(rows[1], "\n\n");
    BOOST_REQUIRE_EQUAL(rows[2], "morethantenchars\r\n");
}

BOOST_AUTO_TEST_SUITE_END()

// ------------------------------------------------------------------------- //
BOOST_AUTO_TEST_SUITE(printer__format_parameters_table)

BOOST_AUTO_TEST_CASE(printer__format_parameters_table__positional_empty__empty)
{
    CONFIG_PRINTER_SETUP();
    BOOST_REQUIRE_EQUAL(help.format_parameters_table(true), "");
}

BOOST_AUTO_TEST_CASE(printer__format_parameters_table__named_empty__empty)
{
    CONFIG_PRINTER_SETUP();
    BOOST_REQUIRE_EQUAL(help.format_parameters_table(false), "");
}

BOOST_AUTO_TEST_CASE(printer__format_parameters_table__named_three_options__three_options)
{
    CONFIG_PRINTER_SETUP_ARGUMENTS(options.add_options()
        ("long", "Long name only.")
        (",m", "Short name only.")
        ("short_long,s", "Long and short name."));
    CONFIG_PRINTER_INITIALIZE(3u, 0u);
    BOOST_REQUIRE_EQUAL(help.format_parameters_table(false),
        "--long               Long name only.                                     \n"
        "-m                   Short name only.                                    \n"
        "-s [--short_long]    Long and short name.                                \n"
    );
}

BOOST_AUTO_TEST_CASE(printer__format_parameters_table__reversed_named_three_options__three_sorted_options)
{
    CONFIG_PRINTER_SETUP_ARGUMENTS(options.add_options()
        ("short_long,s", "Long and short name.")
        (",m", "Short name only.")
        ("long", "Long name only."));
    CONFIG_PRINTER_INITIALIZE(3u, 0u);
    BOOST_REQUIRE_EQUAL(help.format_parameters_table(false),
        "--long               Long name only.                                     \n"
        "-m                   Short name only.                                    \n"
        "-s [--short_long]    Long and short name.                                \n"
    );
}

BOOST_AUTO_TEST_CASE(printer__format_parameters_table__unsorted_named_three_options_no_matching_arguments__three_sorted_options)
{
    CONFIG_PRINTER_SETUP_ARGUMENTS(options.add_options()
        ("first,f", "First option description.")
        ("second,x", "Second option description.")
        ("third", "Third option description.");
        arguments.add("forty-two", 42);
        arguments.add("THIRD", -1));
    CONFIG_PRINTER_INITIALIZE(3u, 2u);
    BOOST_REQUIRE_EQUAL(help.format_parameters_table(false),
        "--third              Third option description.                           \n"
        "-f [--first]         First option description.                           \n"
        "-x [--second]        Second option description.                          \n"
    );
}

BOOST_AUTO_TEST_CASE(printer__format_parameters_table__named_three_options_two_matching_arguments__one_option)
{
    CONFIG_PRINTER_SETUP_ARGUMENTS(options.add_options()
        ("first,f", "First option description.")
        ("second,x", "Second option description.")
        ("THIRD", "Third option description.");
        arguments.add("FIRST", 1);
        arguments.add("second", 42);
        arguments.add("THIRD", -1));
    CONFIG_PRINTER_INITIALIZE(3u, 3u);
    BOOST_REQUIRE_EQUAL(help.format_parameters_table(false),
        "-f [--first]         First option description.                           \n"
    );
}

BOOST_AUTO_TEST_CASE(printer__format_parameters_table__positional_three_options__empty)
{
    CONFIG_PRINTER_SETUP_ARGUMENTS(options.add_options()
        ("long", "Long name only.")
        ("short_long,s", "Long and short name.")
        (",m", "Short name only."));
    CONFIG_PRINTER_INITIALIZE(3u, 0u);
    BOOST_REQUIRE_EQUAL(help.format_parameters_table(true), "");
}

BOOST_AUTO_TEST_CASE(printer__format_parameters_table__positional_three_options_one_matching_argument__one_argument)
{
    CONFIG_PRINTER_SETUP_ARGUMENTS(options.add_options()
        ("first,f", "First option description.")
        ("second,x", "Second option description.")
        ("THIRD", "Third option description.");
        arguments.add("FIRST", 1);
        arguments.add("SECOND", 42);
        arguments.add("THIRD", -1));
    CONFIG_PRINTER_INITIALIZE(3u, 3u);
    BOOST_REQUIRE_EQUAL(help.format_parameters_table(true),
        "THIRD                Third option description.                           \n"
    );
}

BOOST_AUTO_TEST_CASE(printer__format_parameters_table__reverse_positional_three_options_three_matching_arguments__three_unsorted_arguments)
{
    CONFIG_PRINTER_SETUP_ARGUMENTS(options.add_options()
        ("third", "Third option description.")
        ("SECOND", "Second option description.")
        ("FIRST", "First option description.");
        arguments.add("FIRST", 1);
        arguments.add("SECOND", 42);
        arguments.add("third", -1));
    CONFIG_PRINTER_INITIALIZE(3u, 3u);
    BOOST_REQUIRE_EQUAL(help.format_parameters_table(true),
        "THIRD                Third option description.                           \n"
        "SECOND               Second option description.                          \n"
        "FIRST                First option description.                           \n"
    );
}

BOOST_AUTO_TEST_CASE(printer__format_parameters_table__positional_three_options_two_matching_arguments_overflow__two_arguments_overflow)
{
    CONFIG_PRINTER_SETUP_ARGUMENTS(options.add_options()
        ("first,f", "First option description.")
        ("SECOND,x",
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do "
            "eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut "
            "enim ad minim veniam, quis nostrud exercitation ullamco laboris "
            "nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor "
            "in reprehenderit in voluptate velit esse cillum dolore eu fugiat "
            "nulla pariatur. Excepteur sint occaecat cupidatat non proident, "
            "sunt in culpa qui officia deserunt mollit anim id est laborum.")
        ("THIRD", "Third option description.");
    arguments.add("FIRST", 1);
    arguments.add("SECOND", 42);
    arguments.add("THIRD", -1));
    CONFIG_PRINTER_INITIALIZE(3u, 3u);
    auto table = help.format_parameters_table(true);
    BOOST_REQUIRE_EQUAL(table,
        "SECOND               Lorem ipsum dolor sit amet, consectetur adipiscing  \n"
        "                     elit, sed do eiusmod tempor incididunt ut labore et \n"
        "                     dolore magna aliqua. Ut enim ad minim veniam, quis  \n"
        "                     nostrud exercitation ullamco laboris nisi ut aliquip\n"
        "                     ex ea commodo consequat. Duis aute irure dolor in   \n"
        "                     reprehenderit in voluptate velit esse cillum dolore \n"
        "                     eu fugiat nulla pariatur. Excepteur sint occaecat   \n"
        "                     cupidatat non proident, sunt in culpa qui officia   \n"
        "                     deserunt mollit anim id est laborum.                \n"
        "THIRD                Third option description.                           \n"
    );
}

BOOST_AUTO_TEST_SUITE_END()

// ------------------------------------------------------------------------- //
BOOST_AUTO_TEST_SUITE(printer__format_usage_parameters)

BOOST_AUTO_TEST_CASE(printer__format_usage_parameters__unsorted_two_options_one_arg__sorted)
{
    CONFIG_PRINTER_SETUP_ARGUMENTS(options.add_options()
        ("second,x", "Second option description.")
        ("first,f", "First option description.")
        ("THIRD", "Third option description.");
        arguments.add("FIRST", 1);
        arguments.add("SECOND", 42);
        arguments.add("THIRD", -1));
    CONFIG_PRINTER_INITIALIZE(3u, 3u);
    BOOST_REQUIRE_EQUAL(help.format_usage_parameters(), "[-fx] [THIRD]...");
}

BOOST_AUTO_TEST_CASE(printer__format_usage_parameters__unsorted_multiple_parameters__sorted_parameters)
{
    using namespace boost::filesystem;
    using namespace boost::program_options;
    CONFIG_PRINTER_SETUP_ARGUMENTS(options.add_options()
        ("short_long,s", "Long and short name.")
        (",m", "Short name only.")
        ("longy", value<int>()->required(), "Long name only.")
        ("SIMPLE", value<std::string>(), "Simple string.")
        ("defaulty", value<bool>()->default_value(true), "Defaulted bool.")
        ("required", value<path>()->required(), "Required path.")
        ("untoggled", value<bool>()->zero_tokens(), "Zero token but not short.")
        ("toggled,t", value<bool>()->zero_tokens(), "Toggled, zero token and short.")
        ("ARRAY", value<std::vector<std::string>>(), "String vector.")
        ("multy", value<int>()->multitoken(), "Multi-token int.");
        arguments.add("required", 1);
        arguments.add("SIMPLE", 1);
        arguments.add("ARRAY", -1));
    CONFIG_PRINTER_INITIALIZE(10u, 3u);
    BOOST_REQUIRE_EQUAL(help.format_usage_parameters(), "[-mst] --longy value [--untoggled] [--defaulty value] [--multy value]... REQUIRED [SIMPLE] [ARRAY]...");
}
BOOST_AUTO_TEST_SUITE_END()

// ------------------------------------------------------------------------- //
BOOST_AUTO_TEST_SUITE(printer__generate_argument_names)

#define BX_PRINTER_GENERATE_ARGUMENT_NAMES(number_of_names) \
    help.generate_argument_names(); \
    BOOST_REQUIRE_EQUAL(help.get_argument_names().size(), number_of_names)

BOOST_AUTO_TEST_CASE(printer__generate_argument_names__empty_arguments_empty_options__empty)
{
    CONFIG_PRINTER_SETUP();
    BX_PRINTER_GENERATE_ARGUMENT_NAMES(0u);
}

BOOST_AUTO_TEST_CASE(printer__generate_argument_names__empty_arguments_multiple_options__empty)
{
    CONFIG_PRINTER_SETUP_ARGUMENTS(options.add_options()
        ("long", "Long name only.")
        ("short_long,s", "Long and short name.")
        (",m", "Short name only."));
    BX_PRINTER_GENERATE_ARGUMENT_NAMES(0u);
}

BOOST_AUTO_TEST_CASE(printer__generate_argument_names__one_argument_1__one_name_1)
{
    CONFIG_PRINTER_SETUP_ARGUMENTS(arguments.add("one", 1));
    BX_PRINTER_GENERATE_ARGUMENT_NAMES(1u);
    const auto& names = help.get_argument_names();
    BOOST_REQUIRE_EQUAL(names[0].first, "one");
    BOOST_REQUIRE_EQUAL(names[0].second, 1);
}

BOOST_AUTO_TEST_CASE(printer__generate_argument_names__one_argument_42__one_name_42)
{
    CONFIG_PRINTER_SETUP_ARGUMENTS(arguments.add("forty-two", 42));
    BX_PRINTER_GENERATE_ARGUMENT_NAMES(1u);
    const auto& names = help.get_argument_names();
    BOOST_REQUIRE_EQUAL(names[0].first, "forty-two");
    BOOST_REQUIRE_EQUAL(names[0].second, 42);
}

BOOST_AUTO_TEST_CASE(printer__generate_argument_names__one_argument_max_arguments__one_name_max_arguments)
{
    CONFIG_PRINTER_SETUP_ARGUMENTS(arguments.add("max_arguments", printer::max_arguments));
    BX_PRINTER_GENERATE_ARGUMENT_NAMES(1u);
    const auto& names = help.get_argument_names();
    BOOST_REQUIRE_EQUAL(names[0].first, "max_arguments");
    BOOST_REQUIRE_EQUAL(names[0].second, printer::max_arguments);
}

BOOST_AUTO_TEST_CASE(printer__generate_argument_names__one_argument_max_arguments_plus_1__one_name_negative_1)
{
    CONFIG_PRINTER_SETUP_ARGUMENTS(arguments.add("max_arguments+1", printer::max_arguments + 1));
    BX_PRINTER_GENERATE_ARGUMENT_NAMES(1u);
    const auto& names = help.get_argument_names();
    BOOST_REQUIRE_EQUAL(names[0].first, "max_arguments+1");
    BOOST_REQUIRE_EQUAL(names[0].second, -1);
}

BOOST_AUTO_TEST_CASE(printer__generate_argument_names__one_argument_negative_1__one_name_negative_1)
{
    CONFIG_PRINTER_SETUP_ARGUMENTS(arguments.add("negative-one", -1));
    BX_PRINTER_GENERATE_ARGUMENT_NAMES(1u);
    const auto& names = help.get_argument_names();
    BOOST_REQUIRE_EQUAL(names[0].first, "negative-one");
    BOOST_REQUIRE_EQUAL(names[0].second, -1);
}

BOOST_AUTO_TEST_CASE(printer__generate_argument_names__multiple_arguments__expected_names)
{
    CONFIG_PRINTER_SETUP_ARGUMENTS(
        arguments.add("forty-two", 42);
        arguments.add("max_arguments", printer::max_arguments));
    BX_PRINTER_GENERATE_ARGUMENT_NAMES(2u);
    const auto& names = help.get_argument_names();
    BOOST_REQUIRE_EQUAL(names[0].first, "forty-two");
    BOOST_REQUIRE_EQUAL(names[0].second, 42);
    BOOST_REQUIRE_EQUAL(names[1].first, "max_arguments");
    BOOST_REQUIRE_EQUAL(names[1].second, printer::max_arguments);
}

BOOST_AUTO_TEST_CASE(printer__generate_argument_names__multiple_arguments_negative_1__expected_names)
{
    CONFIG_PRINTER_SETUP_ARGUMENTS(
        arguments.add("forty-two", 42);
        arguments.add("max_arguments", printer::max_arguments);
        arguments.add("negative-one", -1));
    BX_PRINTER_GENERATE_ARGUMENT_NAMES(3u);
    const auto& names = help.get_argument_names();
    BOOST_REQUIRE_EQUAL(names[0].first, "forty-two");
    BOOST_REQUIRE_EQUAL(names[0].second, 42);
    BOOST_REQUIRE_EQUAL(names[1].first, "max_arguments");
    BOOST_REQUIRE_EQUAL(names[1].second, printer::max_arguments);
    BOOST_REQUIRE_EQUAL(names[2].first, "negative-one");
    BOOST_REQUIRE_EQUAL(names[2].second, -1);
}

BOOST_AUTO_TEST_SUITE_END()

// ------------------------------------------------------------------------- //
BOOST_AUTO_TEST_SUITE(printer__generate_parameters)

#define BX_PRINTER_GENERATE_PARAMETERS(number_of_parameters) \
    help.generate_parameters(); \
    BOOST_REQUIRE_EQUAL(help.get_parameters().size(), number_of_parameters)

BOOST_AUTO_TEST_CASE(printer__generate_parameters__empty__empty_parameters)
{
    CONFIG_PRINTER_SETUP();
    BX_PRINTER_GENERATE_PARAMETERS(0u);
}

BOOST_AUTO_TEST_CASE(printer__generate_parameters__one_option__expected_parameter)
{
    CONFIG_PRINTER_SETUP_ARGUMENTS(options.add_options()
        ("foo_bar,f", "Foobar option name."));
    BX_PRINTER_GENERATE_PARAMETERS(1u);
    const auto& parameters = help.get_parameters();
    BOOST_REQUIRE_EQUAL(parameters[0].short_name(), 'f');
}

BOOST_AUTO_TEST_CASE(printer__generate_parameters__unsorted_three_options__expected_sorted_parameters)
{
    CONFIG_PRINTER_SETUP_ARGUMENTS(options.add_options()
        ("first,f", "First option description.")
        ("second,x", "Second option description.")
        ("third", "Third option description."));
    BX_PRINTER_GENERATE_PARAMETERS(3u);
    const auto& parameters = help.get_parameters();
    BOOST_REQUIRE_EQUAL(parameters[0].long_name(), "third");
    BOOST_REQUIRE_EQUAL(parameters[1].short_name(), 'f');
    BOOST_REQUIRE_EQUAL(parameters[2].description(), "Second option description.");
}

BOOST_AUTO_TEST_SUITE_END()

// ------------------------------------------------------------------------- //
BOOST_AUTO_TEST_SUITE(printer__initialize)

BOOST_AUTO_TEST_CASE(printer__initialize__unsorted_multitple_options__expected_sorted_parameters)
{
    CONFIG_PRINTER_SETUP_ARGUMENTS(options.add_options()
        ("first,f", "First option description.")
        ("second,x", "Second option description.")
        ("third", "Third option description.");
        arguments.add("forty-two", 42);
        arguments.add("negative-one", -1));
    CONFIG_PRINTER_INITIALIZE(3u, 2u);
    const auto& names = help.get_argument_names();
    const auto& parameters = help.get_parameters();
    BOOST_REQUIRE_EQUAL(names[0].first, "forty-two");
    BOOST_REQUIRE_EQUAL(names[0].second, 42);
    BOOST_REQUIRE_EQUAL(names[1].first, "negative-one");
    BOOST_REQUIRE_EQUAL(names[1].second, -1);
    BOOST_REQUIRE_EQUAL(parameters[0].long_name(), "third");
    BOOST_REQUIRE_EQUAL(parameters[1].short_name(), 'f');
    BOOST_REQUIRE_EQUAL(parameters[2].description(), "Second option description.");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
