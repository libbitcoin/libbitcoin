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
#ifndef LIBBITCOIN_SYSTEM_MUTEX_HPP
#define LIBBITCOIN_SYSTEM_MUTEX_HPP

#include <boost/thread.hpp>

namespace libbitcoin {
namespace system {

/// The mutex class is a synchronization primitive that can be used to protect
/// shared data from being simultaneously accessed by multiple threads.

/// C++17: std::shared_mutex.
/// A mutex for creating a shared or exclusive critical section.
typedef boost::shared_mutex shared_mutex;

/// C++??: there is no upgradeable lock concept.
/// An upgradeable (to exclusive) and downgradeable (to shared) mutex.
typedef boost::upgrade_mutex upgrade_mutex;

/// Locks are objects that create a shared or exclusive critical section upon
/// construct and release it upon destruct. This is a simplification over
/// explicit critical section creation using mutex methods. Shared critical
/// sections are generally for readers and exclusive critical sections are
/// generally for writiers. A shared mutex type is presumed for unique locks,
/// as it is generally the case that the written state must be read elsewhere
/// under a shared lock.

/// C++11: std::unique_lock
/// Acquire an exclusive lock for the lifetime of the instance.
typedef boost::unique_lock<shared_mutex> unique_lock;

/// C++14: std::shared_lock
/// Acquire a shared lock for the lifetime of the instance.
typedef boost::shared_lock<shared_mutex> shared_lock;

} // namespace system
} // namespace libbitcoin

#endif
