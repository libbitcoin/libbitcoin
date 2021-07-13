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
#include <bitcoin/system/words/catalogs/electrum.hpp>

#include <bitcoin/system/words/catalogs/mnemonic.hpp>

namespace libbitcoin {
namespace system {
namespace words {
namespace electrum {

// github.com/spesmilo/electrum/tree/master/electrum/wordlist
// The portuguese wordlist in the Electrum v1 repo is for Electrum v2.
// The Electrum v1 repo includes en, es, ja, and zh_Hant only.
// This list has been extended to include all BIP39 word lists.

// Electrum v2 word lists are currently identical to BIP39 word lists.
const catalog::words& en = mnemonic::en;
const catalog::words& es = mnemonic::es;
const catalog::words& it = mnemonic::it;
const catalog::words& fr = mnemonic::fr;
const catalog::words& cs = mnemonic::cs;
const catalog::words& pt = mnemonic::pt;
const catalog::words& ja = mnemonic::ja;
const catalog::words& ko = mnemonic::ko;
const catalog::words& zh_Hans = mnemonic::zh_Hans;
const catalog::words& zh_Hant = mnemonic::zh_Hant;

} // namespace electrum
} // namespace words
} // namespace system
} // namespace libbitcoin
