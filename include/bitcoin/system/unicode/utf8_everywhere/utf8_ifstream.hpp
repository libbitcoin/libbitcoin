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
#ifndef LIBBITCOIN_SYSTEM_UNICODE_UTF8_EVERYWHERE_UTF8_IFSTREAM_HPP
#define LIBBITCOIN_SYSTEM_UNICODE_UTF8_EVERYWHERE_UTF8_IFSTREAM_HPP

#include <fstream>
#include <boost/filesystem.hpp>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {

/// Use bc::system::ifstream in place of std::ifstream.
/// This provides unicode and long path translation for Win32.
class BC_API ifstream
  : public std::ifstream
{
public:
    /// This also opens the file.
    ifstream(const boost::filesystem::path& path,
        std::ifstream::openmode mode=std::ifstream::in);
};

} // namespace system
} // namespace libbitcoin

#endif
