/**
 * Copyright (c) 2011-2021 libbitcoin developers (see AUTHORS)
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
#ifndef LIBBITCOIN_SYSTEM_WORDS_CATALOGS_ELECTRUM_V1_HPP
#define LIBBITCOIN_SYSTEM_WORDS_CATALOGS_ELECTRUM_V1_HPP

#include <bitcoin/system/words/dictionary.hpp>
#include <bitcoin/system/words/dictionaries.hpp>

namespace libbitcoin {
namespace system {
namespace words {
namespace electrum_v1 {

typedef words::dictionary<1626> catalog;

extern const catalog::words en;
extern const catalog::words pt;

typedef words::dictionaries<2, catalog::size()> catalogs;

} // namespace electrum_v1
} // namespace words
} // namespace system
} // namespace libbitcoin

#endif
