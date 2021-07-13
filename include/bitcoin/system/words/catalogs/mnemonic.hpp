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
#ifndef LIBBITCOIN_SYSTEM_WORDS_CATALOGS_MNEMONIC_HPP
#define LIBBITCOIN_SYSTEM_WORDS_CATALOGS_MNEMONIC_HPP

#include <bitcoin/system/words/dictionary.hpp>
#include <bitcoin/system/words/dictionaries.hpp>

namespace libbitcoin {
namespace system {
namespace words {
namespace mnemonic {

typedef words::dictionary<2048> catalog;

extern const catalog::words en;
extern const catalog::words es;
extern const catalog::words it;
extern const catalog::words fr;
extern const catalog::words cs;
extern const catalog::words pt;
extern const catalog::words ja;
extern const catalog::words ko;
extern const catalog::words zh_Hans;
extern const catalog::words zh_Hant;

typedef words::dictionaries<10, catalog::size()> catalogs;

} // namespace mnemonic
} // namespace words
} // namespace system
} // namespace libbitcoin

#endif
