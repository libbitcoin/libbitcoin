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
#include <bitcoin/system/config/directory.hpp>

#include <string>
#include <bitcoin/system/unicode/conversion.hpp>

#ifdef _MSC_VER
    #include <shlobj.h>
    #include <windows.h>
#endif

namespace libbitcoin {
namespace system {
namespace config {

// Returns empty string if unable to retrieve (including when not in Windows).
std::string windows_config_directory()
{
#ifdef _MSC_VER
    wchar_t directory[MAX_PATH];
    const auto result = SHGetFolderPathW(NULL, CSIDL_COMMON_APPDATA, NULL,
        SHGFP_TYPE_CURRENT, directory);

    if (SUCCEEDED(result))
        return to_utf8(directory);
#endif
    return "";
}

} // namespace config
} // namespace system
} // namespace libbitcoin
