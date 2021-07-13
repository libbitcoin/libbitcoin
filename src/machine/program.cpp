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
#include <bitcoin/system/machine/program.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <utility>
#include <bitcoin/system/assert.hpp>
#include <bitcoin/system/chain/chain.hpp>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/machine/interpreter.hpp>
#include <bitcoin/system/machine/number.hpp>
#include <bitcoin/system/math/math.hpp>

namespace libbitcoin {
namespace system {
namespace machine {

using namespace bc::system::chain;

static const chain::transaction default_tx_;
static const chain::script default_script_;

// Constructors.
//-----------------------------------------------------------------------------

program::program()
  : script_(default_script_),
    transaction_(default_tx_),
    input_index_(0),
    forks_(0),
    value_(0),
    version_(script_version::unversioned),
    negative_count_(0),
    operation_count_(0),
    jump_(script_.begin())
{
}

program::program(const script& script)
  : script_(script),
    transaction_(default_tx_),
    input_index_(0),
    forks_(0),
    value_(0),
    version_(script_version::unversioned),
    negative_count_(0),
    operation_count_(0),
    jump_(script_.begin())
{
}

program::program(const script& script, const chain::transaction& transaction,
    uint32_t input_index, uint32_t forks)
  : script_(script),
    transaction_(transaction),
    input_index_(input_index),
    forks_(forks),
    value_(max_uint64),
    version_(script_version::unversioned),
    negative_count_(0),
    operation_count_(0),
    jump_(script_.begin())
{
}

// Condition, alternate, jump and operation_count are not copied.
program::program(const script& script, const chain::transaction& transaction,
    uint32_t input_index, uint32_t forks, data_stack&& stack, uint64_t value,
    script_version version)
  : script_(script),
    transaction_(transaction),
    input_index_(input_index),
    forks_(forks),
    value_(value),
    version_(version),
    negative_count_(0),
    operation_count_(0),
    jump_(script_.begin()),
    primary_(std::move(stack))
{
}


// Condition, alternate, jump and operation_count are not copied.
program::program(const script& script, const program& other)
  : script_(script),
    transaction_(other.transaction_),
    input_index_(other.input_index_),
    forks_(other.forks_),
    value_(other.value_),
    version_(script_version::unversioned),
    negative_count_(0),
    operation_count_(0),
    jump_(script_.begin()),
    primary_(other.primary_)
{
}

// Condition, alternate, jump and operation_count are not moved.
program::program(const script& script, program&& other, bool)
  : script_(script),
    transaction_(other.transaction_),
    input_index_(other.input_index_),
    forks_(other.forks_),
    value_(other.value_),
    version_(script_version::unversioned),
    negative_count_(0),
    operation_count_(0),
    jump_(script_.begin()),
    primary_(std::move(other.primary_))
{
}

// Instructions.
//-----------------------------------------------------------------------------

code program::evaluate()
{
    return interpreter::run(*this);
}

code program::evaluate(const operation& op)
{
    return interpreter::run(op, *this);
}

// Constant registers.
//-----------------------------------------------------------------------------

// Check initial program state for validity (i.e. can evaluation return true).
bool program::is_invalid() const
{
    // Stack elements must be within push size limit (bip141).
    // Invalid operations indicates a failure deserializing individual ops.
    return !script_.is_valid_operations()
        || script_.is_unspendable()
        || script_.is_oversized()
        || !chain::witness::is_push_size(primary_);
}

uint32_t program::forks() const
{
    return forks_;
}

uint32_t program::input_index() const
{
    return input_index_;
}

uint64_t program::value() const
{
    return value_;
}

script_version program::version() const
{
    return version_;
}

const chain::transaction& program::transaction() const
{
    return transaction_;
}

// Program registers.
//-----------------------------------------------------------------------------

program::op_iterator program::begin() const
{
    return script_.begin();
}

program::op_iterator program::jump() const
{
    return jump_;
}

program::op_iterator program::end() const
{
    return script_.end();
}

size_t program::operation_count() const
{
    return operation_count_;
}

// Instructions.
//-----------------------------------------------------------------------------

bool operation_overflow(size_t count)
{
    return count > max_counted_ops;
}

bool program::increment_operation_count(const operation& op)
{
    // Addition is safe due to script size metadata.
    if (operation::is_counted(op.code()))
        ++operation_count_;

    return !operation_overflow(operation_count_);
}

bool program::increment_operation_count(int32_t public_keys)
{
    static const auto max_keys = static_cast<int32_t>(max_script_public_keys);

    // bit.ly/2d1bsdB
    if (is_negative(public_keys)|| public_keys > max_keys)
        return false;

    // Addition is safe due to script size metadata.
    operation_count_ += public_keys;
    return !operation_overflow(operation_count_);
}

bool program::set_jump_register(const operation& op, int32_t offset)
{
    if (script_.empty())
        return false;

    const auto finder = [&op](const operation& operation)
    {
        return &operation == &op;
    };

    // This is not efficient but is simplifying and subscript is rarely used.
    // Otherwise we must track the program counter through each evaluation.
    jump_ = std::find_if(script_.begin(), script_.end(), finder);

    if (jump_ == script_.end())
        return false;

    // This does not require guard because op_codeseparator can only increment.
    // Even if the opcode is last in the sequence the increment is valid (end).
    BITCOIN_ASSERT_MSG(offset == 1, "unguarded jump offset");

    jump_ += offset;
    return true;
}

// Primary stack (push).
//-----------------------------------------------------------------------------

// push
void program::push(bool value)
{
    push_move(value ? value_type{ numbers::positive_1 } : value_type{});
}

// Be explicit about the intent to move or copy, to get compiler help.
void program::push_move(value_type&& item)
{
    primary_.push_back(std::move(item));
}

// Be explicit about the intent to move or copy, to get compiler help.
void program::push_copy(const value_type& item)
{
    primary_.push_back(item);
}

// Primary stack (pop).
//-----------------------------------------------------------------------------

// This must be guarded.
data_chunk program::pop()
{
    BITCOIN_ASSERT(!empty());
    const auto value = primary_.back();
    primary_.pop_back();
    return value;
}

bool program::pop(int32_t& out_value)
{
    number value;
    if (!pop(value))
        return false;

    out_value = value.int32();
    return true;
}

bool program::pop(number& out_number, size_t maxiumum_size)
{
    return !empty() && out_number.set_data(pop(), maxiumum_size);
}

bool program::pop_binary(number& first, number& second)
{
    // The right hand side number is at the top of the stack.
    return pop(first) && pop(second);
}

bool program::pop_ternary(number& first, number& second, number& third)
{
    // The upper bound is at stack top, lower bound next, value next.
    return pop(first) && pop(second) && pop(third);
}

// Determines if popped value is valid post-pop stack index and returns index.
bool program::pop_position(stack_iterator& out_position)
{
    int32_t signed_index;
    if (!pop(signed_index))
        return false;

    // Ensure the index is within bounds.

    if (is_negative(signed_index))
        return false;

    const auto index = static_cast<uint32_t>(signed_index);

    if (index >= size())
        return false;

    out_position = position(index);
    return true;
}

// pop1/pop2/.../pop[count]
bool program::pop(data_stack& section, size_t count)
{
    if (size() < count)
        return false;

    for (size_t index = 0; index < count; ++index)
        section.push_back(pop());

    return true;
}

// Primary push/pop optimizations (active).
//-----------------------------------------------------------------------------

// pop1/pop2/.../pop[index]/push[index]/.../push2/push1/push[index]
void program::duplicate(size_t index)
{
    push_copy(item(index));
}

// pop1/pop2/push1/push2
void program::swap(size_t index_left, size_t index_right)
{
    // TODO: refactor to allow DRY without const_cast here.
    std::swap(
        const_cast<data_stack::value_type&>(item(index_left)),
        const_cast<data_stack::value_type&>(item(index_right)));
}

// pop1/pop2/.../pop[pos-1]/pop[pos]/push[pos-1]/.../push2/push1
void program::erase(const stack_iterator& position)
{
    primary_.erase(position);
}

// pop1/pop2/.../pop[i]/pop[first]/.../pop[last]/push[i]/.../push2/push1
void program::erase(const stack_iterator& first,
    const stack_iterator& last)
{
    primary_.erase(first, last);
}

// Primary push/pop optimizations (passive).
//-----------------------------------------------------------------------------

// Reversed byte order in this example (big-endian).
// []               : false (empty)
// [00 00 00 00 00] : false (+zero)
// [80 00 00 00 00] : false (-zero)
// [42 00 00 00 00] : true
// [00 80 00 00 00] : true

// private
bool program::stack_to_bool(bool clean) const
{
    const auto& top = primary_.back();

    if (top.empty() || (clean && primary_.size() != one))
        return false;

    auto not_zero = [](uint8_t value)
    {
        return value != numbers::positive_0;
    };

    auto non_zero = [](uint8_t value) 
    {
        return (value & ~numbers::negative_sign) != numbers::positive_0;
    };

    return non_zero(top.back()) ||
        std::any_of(top.begin(), std::prev(top.end()), not_zero);
}

bool program::empty() const
{
    return primary_.empty();
}

// This must be guarded (intended for interpreter internal use).
bool program::stack_true(bool clean) const
{
    BITCOIN_ASSERT(!empty());
    return stack_to_bool(clean);
}

// This is safe to call when empty (intended for completion handlers).
bool program::stack_result(bool clean) const
{
    return !empty() && stack_true(clean);
}

bool program::is_stack_overflow() const
{
    // bit.ly/2cowHlP
    // Addition is safe due to script size metadata.
    return size() + alternate_.size() > max_stack_size;
}

bool program::if_(const operation& op) const
{
    // Skip operation if failed and the operator is unconditional.
    return op.is_conditional() || succeeded();
}

// This must be guarded.
const data_stack::value_type& program::item(size_t index) /*const*/
{
    return *position(index);
}

bool program::top(number& out_number, size_t maxiumum_size)
{
    return !empty() && out_number.set_data(item(0), maxiumum_size);
}

// This must be guarded.
program::stack_iterator program::position(size_t index) /*const*/
{
    // Decrementing 1 makes the stack index zero-based (unlike satoshi).
    BITCOIN_ASSERT(index < size());
    return std::prev(primary_.end(), ++index);
}

// Pop jump-to-end, push all back, use to construct a script.
program::operations program::subscript() const
{
    operations ops;

    for (auto op = jump(); op != end(); ++op)
        ops.push_back(*op);

    return ops;
}

size_t program::size() const
{
    return primary_.size();
}

// Alternate stack.
//-----------------------------------------------------------------------------

bool program::empty_alternate() const
{
    return alternate_.empty();
}

void program::push_alternate(value_type&& value)
{
    alternate_.push_back(std::move(value));
}

// This must be guarded.
program::value_type program::pop_alternate()
{
    BITCOIN_ASSERT(!alternate_.empty());
    const auto value = alternate_.back();
    alternate_.pop_back();
    return value;
}

// Conditional stack.
//-----------------------------------------------------------------------------

void program::open(bool value)
{
    negative_count_ += (value ? 0 : 1);
    condition_.push_back(value);
}

// This must be guarded.
void program::negate()
{
    BITCOIN_ASSERT(!closed());
    const auto value = condition_.back();
    negative_count_ += (value ? 1 : -1);
    condition_.back() = !value;

    // Optimized above to avoid succeeded loop.
    ////condition_.back() = !condition_.back();
}

// This must be guarded.
void program::close()
{
    BITCOIN_ASSERT(!closed());
    const auto value = condition_.back();
    negative_count_ += (value ? 0 : -1);
    condition_.pop_back();

    // Optimized above to avoid succeeded loop.
    ////condition_.pop_back();
}

bool program::closed() const
{
    return condition_.empty();
}

bool program::succeeded() const
{
    return is_zero(negative_count_);

    // Optimized above to avoid succeeded loop.
    ////const auto is_true = [](bool value) { return value; };
    ////return std::all_of(condition_.begin(), condition_.end(), true);
}

} // namespace machine
} // namespace system
} // namespace libbitcoin
