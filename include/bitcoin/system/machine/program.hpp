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
#ifndef LIBBITCOIN_SYSTEM_MACHINE_PROGRAM_HPP
#define LIBBITCOIN_SYSTEM_MACHINE_PROGRAM_HPP

#include <cstdint>
#include <bitcoin/system/chain/chain.hpp>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/machine/number.hpp>

namespace libbitcoin {
namespace system {
namespace machine {

class BC_API program
{
public:
    typedef data_stack::value_type value_type;
    typedef chain::operation::list operations;
    typedef chain::operation::iterator op_iterator;

    // Older libstdc++ does not allow erase with const iterator.
    // This is a bug that requires we up the minimum compiler version.
    // So presently stack_iterator is a non-const iterator.
    ////typedef data_stack::const_iterator stack_iterator;
    typedef data_stack::iterator stack_iterator;

    /// Create an instance that does not expect to verify signatures.
    /// This is useful for script utilities but not with input metadata.
    /// This can only run individual operations via run(op, program).
    program();

    /// Create an instance that does not expect to verify signatures.
    /// This is useful for script utilities but not with input metadata.
    /// This can run ops via run(op, program) or the script via run(program).
    program(const chain::script& script);

    /// Create an instance with empty stacks, value unused/max (input run).
    program(const chain::script& script, const chain::transaction& transaction,
        uint32_t input_index, uint32_t forks);

    /// Create an instance with initialized stack (witness run, v0 by default).
    program(const chain::script& script, const chain::transaction& transaction,
        uint32_t input_index, uint32_t forks, data_stack&& stack,
        uint64_t value, chain::script_version version=chain::script_version::zero);

    /// Create using copied tx, input, forks, value, stack (prevout run).
    program(const chain::script& script, const program& other);

    /// Create using copied tx, input, forks, value and moved stack (p2sh run).
    program(const chain::script& script, program&& other, bool move);

    /// Utilities.
    bool is_invalid() const;

    /// Constant registers.
    uint32_t forks() const;
    uint32_t input_index() const;
    uint64_t value() const;
    chain::script_version version() const;
    const chain::transaction& transaction() const;

    /// Program registers.
    op_iterator begin() const;
    op_iterator jump() const;
    op_iterator end() const;
    size_t operation_count() const;

    /// Instructions.
    code evaluate();
    code evaluate(const chain::operation& op);
    bool increment_operation_count(const chain::operation& op);
    bool increment_operation_count(int32_t public_keys);
    bool set_jump_register(const chain::operation& op, int32_t offset);

    // Primary stack.
    //-------------------------------------------------------------------------

    /// Primary push.
    void push(bool value);
    void push_move(value_type&& item);
    void push_copy(const value_type& item);

    /// Primary pop.
    data_chunk pop();
    bool pop(int32_t& out_value);
    bool pop(number& out_number, size_t maxiumum_size=chain::max_number_size);
    bool pop_binary(number& first, number& second);
    bool pop_ternary(number& first, number& second, number& third);
    bool pop_position(stack_iterator& out_position);
    bool pop(data_stack& section, size_t count);

    /// Primary push/pop optimizations (active).
    void duplicate(size_t index);
    void swap(size_t index_left, size_t index_right);
    void erase(const stack_iterator& position);
    void erase(const stack_iterator& first, const stack_iterator& last);

    /// Primary push/pop optimizations (passive).
    bool empty() const;
    bool stack_true(bool clean) const;
    bool stack_result(bool clean) const;
    bool is_stack_overflow() const;
    bool if_(const chain::operation& op) const;
    const value_type& item(size_t index) /*const*/;
    bool top(number& out_number, size_t maxiumum_size=chain::max_number_size) /*const*/;
    stack_iterator position(size_t index) /*const*/;
    operations subscript() const;
    size_t size() const;

    // Alternate stack.
    //-------------------------------------------------------------------------

    bool empty_alternate() const;
    void push_alternate(value_type&& value);
    value_type pop_alternate();

    // Conditional stack.
    //-------------------------------------------------------------------------

    void open(bool value);
    void negate();
    void close();
    bool closed() const;
    bool succeeded() const;

private:
    // A space-efficient dynamic bitset (specialized by c++ std libr).
    typedef std::vector<bool> bool_stack;

    bool stack_to_bool(bool clean) const;

    const chain::script& script_;
    const chain::transaction& transaction_;
    const uint32_t input_index_;
    const uint32_t forks_;
    const uint64_t value_;
    const chain::script_version version_;

    size_t negative_count_;
    size_t operation_count_;
    op_iterator jump_;
    data_stack primary_;
    data_stack alternate_;
    bool_stack condition_;
};

} // namespace machine
} // namespace system
} // namespace libbitcoin

#endif
