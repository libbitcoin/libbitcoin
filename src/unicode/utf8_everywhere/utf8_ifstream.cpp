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
#include <bitcoin/system/unicode/utf8_everywhere/utf8_ifstream.hpp>

#include <fstream>
#include <boost/filesystem.hpp>
#include <bitcoin/system/unicode/utf8_everywhere/utf8_environment.hpp>

namespace libbitcoin {
namespace system {
    
// C++17: use std::filesystem.
// VC++ EXTENSION: "construct with wide-named file".
ifstream::ifstream(const boost::filesystem::path& path,
    std::ifstream::openmode mode)
  : std::ifstream(to_extended_path(path), mode)
{
    // This opens the file.
}

} // namespace system
} // namespace libbitcoin
