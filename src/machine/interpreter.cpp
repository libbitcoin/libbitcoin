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
#include <bitcoin/system/machine/interpreter.hpp>

#include <cstdint>
#include <utility>
#include <bitcoin/system/chain/chain.hpp>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/crypto/crypto.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/error.hpp>
#include <bitcoin/system/machine/number.hpp>
#include <bitcoin/system/machine/program.hpp>
#include <bitcoin/system/math/math.hpp>

namespace libbitcoin {
namespace system {
namespace machine {

using namespace bc::system::chain;

static constexpr auto op_75 = static_cast<uint8_t>(opcode::push_size_75);

// Operations (shared).
//-----------------------------------------------------------------------------

interpreter::result interpreter::op_nop(opcode)
{
    return error::success;
}

interpreter::result interpreter::op_disabled(opcode)
{
    return error::op_disabled;
}

interpreter::result interpreter::op_reserved(opcode)
{
    return error::op_reserved;
}

interpreter::result interpreter::op_push_number(program& program,
    uint8_t value)
{
    program.push_move({ value });
    return error::success;
}

interpreter::result interpreter::op_push_size(program& program,
    const operation& op)
{
    if (op.data().size() > op_75)
        return error::op_push_size;

    program.push_copy(op.data());
    return error::success;
}

interpreter::result interpreter::op_push_data(program& program,
    const data_chunk& data, uint32_t size_limit)
{
    if (data.size() > size_limit)
        return error::op_push_data;

    program.push_copy(data);
    return error::success;
}

// Operations (not shared).
//-----------------------------------------------------------------------------
// All index parameters are zero-based and relative to stack top.

interpreter::result interpreter::op_if(program& program)
{
    auto value = false;

    if (program.succeeded())
    {
        if (program.empty())
            return error::op_if;

        value = program.stack_true(false);
        program.pop();
    }

    program.open(value);
    return error::success;
}

interpreter::result interpreter::op_notif(program& program)
{
    auto value = false;

    if (program.succeeded())
    {
        if (program.empty())
            return error::op_notif;

        value = !program.stack_true(false);
        program.pop();
    }

    program.open(value);
    return error::success;
}

interpreter::result interpreter::op_else(program& program)
{
    if (program.closed())
        return error::op_else;

    program.negate();
    return error::success;
}

interpreter::result interpreter::op_endif(program& program)
{
    if (program.closed())
        return error::op_endif;

    program.close();
    return error::success;
}

interpreter::result interpreter::op_verify(program& program)
{
    if (program.empty())
        return error::op_verify1;

    if (!program.stack_true(false))
        return error::op_verify2;

    program.pop();
    return error::success;
}

interpreter::result interpreter::op_return(program&)
{
    return error::op_return;
}

interpreter::result interpreter::op_to_alt_stack(program& program)
{
    if (program.empty())
        return error::op_to_alt_stack;

    program.push_alternate(program.pop());
    return error::success;
}

interpreter::result interpreter::op_from_alt_stack(program& program)
{
    if (program.empty_alternate())
        return error::op_from_alt_stack;

    program.push_move(program.pop_alternate());
    return error::success;
}

interpreter::result interpreter::op_drop2(program& program)
{
    if (program.size() < 2)
        return error::op_drop2;

    program.pop();
    program.pop();
    return error::success;
}

interpreter::result interpreter::op_dup2(program& program)
{
    if (program.size() < 2)
        return error::op_dup2;

    auto item1 = program.item(1);
    auto item0 = program.item(0);

    program.push_move(std::move(item1));
    program.push_move(std::move(item0));
    return error::success;
}

interpreter::result interpreter::op_dup3(program& program)
{
    if (program.size() < 3)
        return error::op_dup3;

    auto item2 = program.item(2);
    auto item1 = program.item(1);
    auto item0 = program.item(0);

    program.push_move(std::move(item2));
    program.push_move(std::move(item1));
    program.push_move(std::move(item0));
    return error::success;
}

interpreter::result interpreter::op_over2(program& program)
{
    if (program.size() < 4)
        return error::op_over2;

    auto item3 = program.item(3);
    auto item2 = program.item(2);

    program.push_move(std::move(item3));
    program.push_move(std::move(item2));
    return error::success;
}

interpreter::result interpreter::op_rot2(program& program)
{
    if (program.size() < 6)
        return error::op_rot2;

    const auto position_5 = program.position(5);
    const auto position_4 = program.position(4);

    auto copy_5 = *position_5;
    auto copy_4 = *position_4;

    program.erase(position_5, position_4 + 1);
    program.push_move(std::move(copy_5));
    program.push_move(std::move(copy_4));
    return error::success;
}

interpreter::result interpreter::op_swap2(program& program)
{
    if (program.size() < 4)
        return error::op_swap2;

    program.swap(3, 1);
    program.swap(2, 0);
    return error::success;
}

interpreter::result interpreter::op_if_dup(program& program)
{
    if (program.empty())
        return error::op_if_dup;

    if (program.stack_true(false))
        program.duplicate(0);

    return error::success;
}

interpreter::result interpreter::op_depth(program& program)
{
    program.push_move(number(program.size()).data());
    return error::success;
}

interpreter::result interpreter::op_drop(program& program)
{
    if (program.empty())
        return error::op_drop;

    program.pop();
    return error::success;
}

interpreter::result interpreter::op_dup(program& program)
{
    if (program.empty())
        return error::op_dup;

    program.duplicate(0);
    return error::success;
}

interpreter::result interpreter::op_nip(program& program)
{
    if (program.size() < 2)
        return error::op_nip;

    program.erase(program.position(1));
    return error::success;
}

interpreter::result interpreter::op_over(program& program)
{
    if (program.size() < 2)
        return error::op_over;

    program.duplicate(1);
    return error::success;
}

interpreter::result interpreter::op_pick(program& program)
{
    program::stack_iterator position;
    if (!program.pop_position(position))
        return error::op_pick;

    program.push_copy(*position);
    return error::success;
}

interpreter::result interpreter::op_roll(program& program)
{
    program::stack_iterator position;
    if (!program.pop_position(position))
        return error::op_roll;

    auto copy = *position;
    program.erase(position);
    program.push_move(std::move(copy));
    return error::success;
}

interpreter::result interpreter::op_rot(program& program)
{
    if (program.size() < 3)
        return error::op_rot;

    program.swap(2, 1);
    program.swap(1, 0);
    return error::success;
}

interpreter::result interpreter::op_swap(program& program)
{
    if (program.size() < 2)
        return error::op_swap;

    program.swap(1, 0);
    return error::success;
}

interpreter::result interpreter::op_tuck(program& program)
{
    if (program.size() < 2)
        return error::op_tuck;

    auto first = program.pop();
    auto second = program.pop();
    program.push_copy(first);
    program.push_move(std::move(second));
    program.push_move(std::move(first));
    return error::success;
}

interpreter::result interpreter::op_size(program& program)
{
    if (program.empty())
        return error::op_size;

    auto top = program.pop();
    const auto size = top.size();
    program.push_move(std::move(top));
    program.push_move(number(size).data());
    return error::success;
}

interpreter::result interpreter::op_equal(program& program)
{
    if (program.size() < 2)
        return error::op_equal;

    program.push(program.pop() == program.pop());
    return error::success;
}

interpreter::result interpreter::op_equal_verify(program& program)
{
    if (program.size() < 2)
        return error::op_equal_verify1;

    return (program.pop() == program.pop()) ? error::success :
        error::op_equal_verify2;
}

interpreter::result interpreter::op_add1(program& program)
{
    number number;
    if (!program.pop(number))
        return error::op_add1;

    number += 1;
    program.push_move(number.data());
    return error::success;
}

interpreter::result interpreter::op_sub1(program& program)
{
    number number;
    if (!program.pop(number))
        return error::op_sub1;

    number -= 1;
    program.push_move(number.data());
    return error::success;
}

interpreter::result interpreter::op_negate(program& program)
{
    number number;
    if (!program.pop(number))
        return error::op_negate;

    number = -number;
    program.push_move(number.data());
    return error::success;
}

interpreter::result interpreter::op_abs(program& program)
{
    number number;
    if (!program.pop(number))
        return error::op_abs;

    if (number < 0)
        number = -number;

    program.push_move(number.data());
    return error::success;
}

interpreter::result interpreter::op_not(program& program)
{
    number number;
    if (!program.pop(number))
        return error::op_not;

    program.push(number.is_false());
    return error::success;
}

interpreter::result interpreter::op_nonzero(program& program)
{
    number number;
    if (!program.pop(number))
        return error::op_nonzero;

    program.push(number.is_true());
    return error::success;
}

interpreter::result interpreter::op_add(program& program)
{
    number first, second;
    if (!program.pop_binary(first, second))
        return error::op_add;

    const auto result = first + second;
    program.push_move(result.data());
    return error::success;
}

interpreter::result interpreter::op_sub(program& program)
{
    number first, second;
    if (!program.pop_binary(first, second))
        return error::op_sub;

    const auto result = second - first;
    program.push_move(result.data());
    return error::success;
}

interpreter::result interpreter::op_bool_and(program& program)
{
    number first, second;
    if (!program.pop_binary(first, second))
        return error::op_bool_and;

    program.push(first.is_true() && second.is_true());
    return error::success;
}

interpreter::result interpreter::op_bool_or(program& program)
{
    number first, second;
    if (!program.pop_binary(first, second))
        return error::op_bool_or;

    program.push(first.is_true() || second.is_true());
    return error::success;
}

interpreter::result interpreter::op_num_equal(program& program)
{
    number first, second;
    if (!program.pop_binary(first, second))
        return error::op_num_equal;

    program.push(first == second);
    return error::success;
}

interpreter::result interpreter::op_num_equal_verify(program& program)
{
    number first, second;
    if (!program.pop_binary(first, second))
        return error::op_num_equal_verify1;

    return (first == second) ? error::success :
        error::op_num_equal_verify2;
}

interpreter::result interpreter::op_num_not_equal(program& program)
{
    number first, second;
    if (!program.pop_binary(first, second))
        return error::op_num_not_equal;

    program.push(first != second);
    return error::success;
}

interpreter::result interpreter::op_less_than(program& program)
{
    number first, second;
    if (!program.pop_binary(first, second))
        return error::op_less_than;

    program.push(second < first);
    return error::success;
}

interpreter::result interpreter::op_greater_than(program& program)
{
    number first, second;
    if (!program.pop_binary(first, second))
        return error::op_greater_than;

    program.push(second > first);
    return error::success;
}

interpreter::result interpreter::op_less_than_or_equal(program& program)
{
    number first, second;
    if (!program.pop_binary(first, second))
        return error::op_less_than_or_equal;

    program.push(second <= first);
    return error::success;
}

interpreter::result interpreter::op_greater_than_or_equal(
    program& program)
{
    number first, second;
    if (!program.pop_binary(first, second))
        return error::op_greater_than_or_equal;

    program.push(second >= first);
    return error::success;
}

interpreter::result interpreter::op_min(program& program)
{
    number first, second;
    if (!program.pop_binary(first, second))
        return error::op_min;

    program.push_move(second < first ? second.data() : first.data());
    return error::success;
}

interpreter::result interpreter::op_max(program& program)
{
    number first, second;
    if (!program.pop_binary(first, second))
        return error::op_max;

    program.push_move(second > first ? second.data() : first.data());
    return error::success;
}

interpreter::result interpreter::op_within(program& program)
{
    number first, second, third;
    if (!program.pop_ternary(first, second, third))
        return error::op_within;

    program.push(second <= third && third < first);
    return error::success;
}

interpreter::result interpreter::op_ripemd160(program& program)
{
    if (program.empty())
        return error::op_ripemd160;

    program.push_move(ripemd160_hash_chunk(program.pop()));
    return error::success;
}

interpreter::result interpreter::op_sha1(program& program)
{
    if (program.empty())
        return error::op_sha1;

    program.push_move(sha1_hash_chunk(program.pop()));
    return error::success;
}

interpreter::result interpreter::op_sha256(program& program)
{
    if (program.empty())
        return error::op_sha256;

    program.push_move(sha256_hash_chunk(program.pop()));
    return error::success;
}

interpreter::result interpreter::op_hash160(program& program)
{
    if (program.empty())
        return error::op_hash160;

    program.push_move(ripemd160_hash_chunk(sha256_hash(program.pop())));
    return error::success;
}

interpreter::result interpreter::op_hash256(program& program)
{
    if (program.empty())
        return error::op_hash256;

    program.push_move(sha256_hash_chunk(sha256_hash(program.pop())));
    return error::success;
}

interpreter::result interpreter::op_codeseparator(program& program,
    const operation& op)
{
    return program.set_jump_register(op, +1) ? error::success :
        error::op_code_seperator;
}

interpreter::result interpreter::op_check_sig_verify(program& program)
{
    if (program.size() < 2)
        return error::op_check_sig_verify1;

    uint8_t sighash;
    ec_signature signature;
    der_signature distinguished;
    auto bip66 = script::is_enabled(program.forks(), rule_fork::bip66_rule);
    auto bip143 = script::is_enabled(program.forks(), rule_fork::bip143_rule);
    auto version = bip143 ? program.version() : script_version::unversioned;

    const auto public_key = program.pop();
    auto endorsement = program.pop();

    // Create a subscript with endorsements stripped (sort of).
    script script_code(program.subscript());

    // BIP143: find and delete of the signature is not applied for v0.
    if (program.version() != script_version::zero)
        script_code.find_and_delete({ endorsement });

    // BIP66: Continue to allow empty signature to push false vs. fail.
    if (endorsement.empty())
        return error::incorrect_signature;

    // Parse endorsement into DER signature into an EC signature.
    if (!parse_endorsement(sighash, distinguished, std::move(endorsement)) ||
        !parse_signature(signature, distinguished, bip66))
        return error::invalid_signature_encoding;

    // Version condition preserves independence of bip141 and bip143.
    return script::check_signature(signature, sighash, public_key,
        script_code, program.transaction(), program.input_index(),
            version, program.value()) ? error::success :
                error::incorrect_signature;
}

interpreter::result interpreter::op_check_sig(program& program)
{
    const auto verified = op_check_sig_verify(program);
    const auto bip66 = script::is_enabled(program.forks(),
        rule_fork::bip66_rule);

    // BIP66: invalid signature encoding fails the operation.
    if (bip66 && verified == error::invalid_signature_encoding)
        return error::op_check_sig;

    program.push(verified == error::success);
    return error::success;
}

interpreter::result interpreter::op_check_multisig_verify(
    program& program)
{
    int32_t key_count = 0;
    if (!program.pop(key_count))
        return error::op_check_multisig_verify1;

    // Multisig script public keys are counted as op codes.
    if (!program.increment_operation_count(key_count))
        return error::op_check_multisig_verify2;

    data_stack public_keys;
    if (!program.pop(public_keys, key_count))
        return error::op_check_multisig_verify3;

    int32_t signature_count;
    if (!program.pop(signature_count))
        return error::op_check_multisig_verify4;

    if (is_negative(signature_count) || signature_count > key_count)
        return error::op_check_multisig_verify5;

    data_stack endorsements;
    if (!program.pop(endorsements, signature_count))
        return error::op_check_multisig_verify6;

    if (program.empty())
        return error::op_check_multisig_verify7;

    //*************************************************************************
    // CONSENSUS: Satoshi bug, discard stack element, malleable until bip147.
    //*************************************************************************
    if (!program.pop().empty() && script::is_enabled(program.forks(),
        rule_fork::bip147_rule))
        return error::op_check_multisig_verify8;

    uint8_t sighash;
    ec_signature signature;
    der_signature distinguished;
    auto endorsement = endorsements.begin();
    auto bip66 = script::is_enabled(program.forks(), rule_fork::bip66_rule);
    auto bip143 = script::is_enabled(program.forks(), rule_fork::bip143_rule);
    auto version = bip143 ? program.version() : script_version::unversioned;

    // Before looping create subscript with endorsements stripped (sort of).
    script script_code(program.subscript());

    // BIP143: find and delete of the signature is not applied for v0.
    if (version != script_version::zero)
        script_code.find_and_delete(endorsements);

    for (const auto& public_key: public_keys)
    {
        // The exact number of signatures are required and must be in order.
        if (endorsement == endorsements.end())
            break;

        // BIP66: Continue to allow empty signature to push false vs. fail.
        if (endorsement->empty())
            continue;

        // Parse endorsement into DER signature into an EC signature.
        if (!parse_endorsement(sighash, distinguished, *endorsement) ||
            !parse_signature(signature, distinguished, bip66))
            return error::invalid_signature_encoding;

        // Version condition preserves independence of bip141 and bip143.
        if (script::check_signature(signature, sighash, public_key,
            script_code, program.transaction(), program.input_index(),
            version, program.value()))
            ++endorsement;
    }

    return endorsement == endorsements.end() ? error::success :
        error::incorrect_signature;
}

interpreter::result interpreter::op_check_multisig(program& program)
{
    const auto verified = op_check_multisig_verify(program);
    const auto bip66 = script::is_enabled(program.forks(),
        rule_fork::bip66_rule);

    // BIP66: invalid signature encoding fails the operation.
    if (bip66 && verified == error::invalid_signature_encoding)
        return error::op_check_multisig;

    program.push(verified == error::success);
    return error::success;
}

interpreter::result interpreter::op_check_locktime_verify(
    program& program)
{
    // BIP65: nop2 subsumed by checklocktimeverify when bip65 fork is active.
    if (!script::is_enabled(program.forks(), rule_fork::bip65_rule))
        return op_nop(opcode::nop2);

    const auto& tx = program.transaction();
    const auto input_index = program.input_index();

    if (input_index >= tx.inputs().size())
        return error::op_check_locktime_verify1;

    // BIP65: the tx sequence is 0xffffffff.
    if (tx.inputs()[input_index].is_final())
        return error::op_check_locktime_verify2;

    // BIP65: the stack is empty.
    // BIP65: extend the (signed) script number range to 5 bytes.
    number stack;
    if (!program.top(stack, max_check_locktime_verify_number_size))
        return error::op_check_locktime_verify3;

    // BIP65: the top stack item is negative.
    if (stack < 0)
        return error::op_check_locktime_verify4;

    // The top stack item is positive, so cast is safe.
    const auto locktime = static_cast<uint64_t>(stack.int64());

    // BIP65: the stack locktime type differs from that of tx.
    if ((locktime < locktime_threshold) !=
        (tx.locktime() < locktime_threshold))
        return error::op_check_locktime_verify5;

    // BIP65: the stack locktime is greater than the tx locktime.
    return (locktime > tx.locktime()) ? error::op_check_locktime_verify6 :
        error::success;
}

interpreter::result interpreter::op_check_sequence_verify(
    program& program)
{
    // BIP112: nop3 subsumed by checksequenceverify when bip112 fork is active.
    if (!script::is_enabled(program.forks(), rule_fork::bip112_rule))
        return op_nop(opcode::nop3);

    const auto& tx = program.transaction();
    const auto input_index = program.input_index();

    if (input_index >= tx.inputs().size())
        return error::op_check_sequence_verify1;

    // BIP112: the stack is empty.
    // BIP112: extend the (signed) script number range to 5 bytes.
    number stack;
    if (!program.top(stack, max_check_sequence_verify_number_size))
        return error::op_check_sequence_verify2;

    // BIP112: the top stack item is negative.
    if (stack < 0)
        return error::op_check_sequence_verify3;

    // The top stack item is positive, so conversion is safe.
    const auto sequence = absolute(stack.int64());

    // BIP112: the stack sequence is disabled, treat as nop3.
    if (!is_zero(sequence & relative_locktime_disabled))
        return op_nop(opcode::nop3);

    // BIP112: the stack sequence is enabled and tx version less than 2.
    if (tx.version() < relative_locktime_min_version)
        return error::op_check_sequence_verify4;

    const auto tx_sequence = tx.inputs()[input_index].sequence();

    // BIP112: the transaction sequence is disabled.
    if (!is_zero(tx_sequence & relative_locktime_disabled))
        return error::op_check_sequence_verify5;

    // BIP112: the stack sequence type differs from that of tx input.
    if ((sequence & relative_locktime_time_locked) !=
        (tx_sequence & relative_locktime_time_locked))
        return error::op_check_sequence_verify6;

    // BIP112: the masked stack sequence is greater than the tx sequence.
    return (sequence & relative_locktime_mask) >
        (tx_sequence & relative_locktime_mask) ?
        error::op_check_sequence_verify7 : error::success;
}

// private:
// It is expected that the compiler will produce a very efficient jump table.
interpreter::result interpreter::run_op(const operation& op,
    program& program)
{
    const auto code = op.code();

    switch (op.code())
    {
        case opcode::push_size_0:
        case opcode::push_size_1:
        case opcode::push_size_2:
        case opcode::push_size_3:
        case opcode::push_size_4:
        case opcode::push_size_5:
        case opcode::push_size_6:
        case opcode::push_size_7:
        case opcode::push_size_8:
        case opcode::push_size_9:
        case opcode::push_size_10:
        case opcode::push_size_11:
        case opcode::push_size_12:
        case opcode::push_size_13:
        case opcode::push_size_14:
        case opcode::push_size_15:
        case opcode::push_size_16:
        case opcode::push_size_17:
        case opcode::push_size_18:
        case opcode::push_size_19:
        case opcode::push_size_20:
        case opcode::push_size_21:
        case opcode::push_size_22:
        case opcode::push_size_23:
        case opcode::push_size_24:
        case opcode::push_size_25:
        case opcode::push_size_26:
        case opcode::push_size_27:
        case opcode::push_size_28:
        case opcode::push_size_29:
        case opcode::push_size_30:
        case opcode::push_size_31:
        case opcode::push_size_32:
        case opcode::push_size_33:
        case opcode::push_size_34:
        case opcode::push_size_35:
        case opcode::push_size_36:
        case opcode::push_size_37:
        case opcode::push_size_38:
        case opcode::push_size_39:
        case opcode::push_size_40:
        case opcode::push_size_41:
        case opcode::push_size_42:
        case opcode::push_size_43:
        case opcode::push_size_44:
        case opcode::push_size_45:
        case opcode::push_size_46:
        case opcode::push_size_47:
        case opcode::push_size_48:
        case opcode::push_size_49:
        case opcode::push_size_50:
        case opcode::push_size_51:
        case opcode::push_size_52:
        case opcode::push_size_53:
        case opcode::push_size_54:
        case opcode::push_size_55:
        case opcode::push_size_56:
        case opcode::push_size_57:
        case opcode::push_size_58:
        case opcode::push_size_59:
        case opcode::push_size_60:
        case opcode::push_size_61:
        case opcode::push_size_62:
        case opcode::push_size_63:
        case opcode::push_size_64:
        case opcode::push_size_65:
        case opcode::push_size_66:
        case opcode::push_size_67:
        case opcode::push_size_68:
        case opcode::push_size_69:
        case opcode::push_size_70:
        case opcode::push_size_71:
        case opcode::push_size_72:
        case opcode::push_size_73:
        case opcode::push_size_74:
        case opcode::push_size_75:
            return op_push_size(program, op);
        case opcode::push_one_size:
            return op_push_data(program, op.data(), max_uint8);
        case opcode::push_two_size:
            return op_push_data(program, op.data(), max_uint16);
        case opcode::push_four_size:
            return op_push_data(program, op.data(), max_uint32);
        case opcode::push_negative_1:
            return op_push_number(program, numbers::negative_1);
        case opcode::reserved_80:
            return op_reserved(code);
        case opcode::push_positive_1:
            return op_push_number(program, numbers::positive_1);
        case opcode::push_positive_2:
            return op_push_number(program, numbers::positive_2);
        case opcode::push_positive_3:
            return op_push_number(program, numbers::positive_3);
        case opcode::push_positive_4:
            return op_push_number(program, numbers::positive_4);
        case opcode::push_positive_5:
            return op_push_number(program, numbers::positive_5);
        case opcode::push_positive_6:
            return op_push_number(program, numbers::positive_6);
        case opcode::push_positive_7:
            return op_push_number(program, numbers::positive_7);
        case opcode::push_positive_8:
            return op_push_number(program, numbers::positive_8);
        case opcode::push_positive_9:
            return op_push_number(program, numbers::positive_9);
        case opcode::push_positive_10:
            return op_push_number(program, numbers::positive_10);
        case opcode::push_positive_11:
            return op_push_number(program, numbers::positive_11);
        case opcode::push_positive_12:
            return op_push_number(program, numbers::positive_12);
        case opcode::push_positive_13:
            return op_push_number(program, numbers::positive_13);
        case opcode::push_positive_14:
            return op_push_number(program, numbers::positive_14);
        case opcode::push_positive_15:
            return op_push_number(program, numbers::positive_15);
        case opcode::push_positive_16:
            return op_push_number(program, numbers::positive_16);
        case opcode::nop:
            return op_nop(code);
        case opcode::reserved_98:
            return op_reserved(code);
        case opcode::if_:
            return op_if(program);
        case opcode::notif:
            return op_notif(program);
        case opcode::disabled_verif:
            return op_disabled(code);
        case opcode::disabled_vernotif:
            return op_disabled(code);
        case opcode::else_:
            return op_else(program);
        case opcode::endif:
            return op_endif(program);
        case opcode::verify:
            return op_verify(program);
        case opcode::return_:
            return op_return(program);
        case opcode::toaltstack:
            return op_to_alt_stack(program);
        case opcode::fromaltstack:
            return op_from_alt_stack(program);
        case opcode::drop2:
            return op_drop2(program);
        case opcode::dup2:
            return op_dup2(program);
        case opcode::dup3:
            return op_dup3(program);
        case opcode::over2:
            return op_over2(program);
        case opcode::rot2:
            return op_rot2(program);
        case opcode::swap2:
            return op_swap2(program);
        case opcode::ifdup:
            return op_if_dup(program);
        case opcode::depth:
            return op_depth(program);
        case opcode::drop:
            return op_drop(program);
        case opcode::dup:
            return op_dup(program);
        case opcode::nip:
            return op_nip(program);
        case opcode::over:
            return op_over(program);
        case opcode::pick:
            return op_pick(program);
        case opcode::roll:
            return op_roll(program);
        case opcode::rot:
            return op_rot(program);
        case opcode::swap:
            return op_swap(program);
        case opcode::tuck:
            return op_tuck(program);
        case opcode::disabled_cat:
            return op_disabled(code);
        case opcode::disabled_substr:
            return op_disabled(code);
        case opcode::disabled_left:
            return op_disabled(code);
        case opcode::disabled_right:
            return op_disabled(code);
        case opcode::size:
            return op_size(program);
        case opcode::disabled_invert:
            return op_disabled(code);
        case opcode::disabled_and:
            return op_disabled(code);
        case opcode::disabled_or:
            return op_disabled(code);
        case opcode::disabled_xor:
            return op_disabled(code);
        case opcode::equal:
            return op_equal(program);
        case opcode::equalverify:
            return op_equal_verify(program);
        case opcode::reserved_137:
            return op_reserved(code);
        case opcode::reserved_138:
            return op_reserved(code);
        case opcode::add1:
            return op_add1(program);
        case opcode::sub1:
            return op_sub1(program);
        case opcode::disabled_mul2:
            return op_disabled(code);
        case opcode::disabled_div2:
            return op_disabled(code);
        case opcode::negate:
            return op_negate(program);
        case opcode::abs:
            return op_abs(program);
        case opcode::not_:
            return op_not(program);
        case opcode::nonzero:
            return op_nonzero(program);
        case opcode::add:
            return op_add(program);
        case opcode::sub:
            return op_sub(program);
        case opcode::disabled_mul:
            return op_disabled(code);
        case opcode::disabled_div:
            return op_disabled(code);
        case opcode::disabled_mod:
            return op_disabled(code);
        case opcode::disabled_lshift:
            return op_disabled(code);
        case opcode::disabled_rshift:
            return op_disabled(code);
        case opcode::booland:
            return op_bool_and(program);
        case opcode::boolor:
            return op_bool_or(program);
        case opcode::numequal:
            return op_num_equal(program);
        case opcode::numequalverify:
            return op_num_equal_verify(program);
        case opcode::numnotequal:
            return op_num_not_equal(program);
        case opcode::lessthan:
            return op_less_than(program);
        case opcode::greaterthan:
            return op_greater_than(program);
        case opcode::lessthanorequal:
            return op_less_than_or_equal(program);
        case opcode::greaterthanorequal:
            return op_greater_than_or_equal(program);
        case opcode::min:
            return op_min(program);
        case opcode::max:
            return op_max(program);
        case opcode::within:
            return op_within(program);
        case opcode::ripemd160:
            return op_ripemd160(program);
        case opcode::sha1:
            return op_sha1(program);
        case opcode::sha256:
            return op_sha256(program);
        case opcode::hash160:
            return op_hash160(program);
        case opcode::hash256:
            return op_hash256(program);
        case opcode::codeseparator:
            return op_codeseparator(program, op);
        case opcode::checksig:
            return op_check_sig(program);
        case opcode::checksigverify:
            return op_check_sig_verify(program);
        case opcode::checkmultisig:
            return op_check_multisig(program);
        case opcode::checkmultisigverify:
            return op_check_multisig_verify(program);
        case opcode::nop1:
            return op_nop(code);
        case opcode::checklocktimeverify:
            return op_check_locktime_verify(program);
        case opcode::checksequenceverify:
            return op_check_sequence_verify(program);
        case opcode::nop4:
        case opcode::nop5:
        case opcode::nop6:
        case opcode::nop7:
        case opcode::nop8:
        case opcode::nop9:
        case opcode::nop10:
            return op_nop(code);
        case opcode::reserved_186:
        case opcode::reserved_187:
        case opcode::reserved_188:
        case opcode::reserved_189:
        case opcode::reserved_190:
        case opcode::reserved_191:
        case opcode::reserved_192:
        case opcode::reserved_193:
        case opcode::reserved_194:
        case opcode::reserved_195:
        case opcode::reserved_196:
        case opcode::reserved_197:
        case opcode::reserved_198:
        case opcode::reserved_199:
        case opcode::reserved_200:
        case opcode::reserved_201:
        case opcode::reserved_202:
        case opcode::reserved_203:
        case opcode::reserved_204:
        case opcode::reserved_205:
        case opcode::reserved_206:
        case opcode::reserved_207:
        case opcode::reserved_208:
        case opcode::reserved_209:
        case opcode::reserved_210:
        case opcode::reserved_211:
        case opcode::reserved_212:
        case opcode::reserved_213:
        case opcode::reserved_214:
        case opcode::reserved_215:
        case opcode::reserved_216:
        case opcode::reserved_217:
        case opcode::reserved_218:
        case opcode::reserved_219:
        case opcode::reserved_220:
        case opcode::reserved_221:
        case opcode::reserved_222:
        case opcode::reserved_223:
        case opcode::reserved_224:
        case opcode::reserved_225:
        case opcode::reserved_226:
        case opcode::reserved_227:
        case opcode::reserved_228:
        case opcode::reserved_229:
        case opcode::reserved_230:
        case opcode::reserved_231:
        case opcode::reserved_232:
        case opcode::reserved_233:
        case opcode::reserved_234:
        case opcode::reserved_235:
        case opcode::reserved_236:
        case opcode::reserved_237:
        case opcode::reserved_238:
        case opcode::reserved_239:
        case opcode::reserved_240:
        case opcode::reserved_241:
        case opcode::reserved_242:
        case opcode::reserved_243:
        case opcode::reserved_244:
        case opcode::reserved_245:
        case opcode::reserved_246:
        case opcode::reserved_247:
        case opcode::reserved_248:
        case opcode::reserved_249:
        case opcode::reserved_250:
        case opcode::reserved_251:
        case opcode::reserved_252:
        case opcode::reserved_253:
        case opcode::reserved_254:
        case opcode::reserved_255:
        default:
            return op_reserved(code);
    }
}

code interpreter::run(program& program)
{
    code ec;

    if (program.is_invalid())
        return error::invalid_script;

    for (const auto& op: program)
    {
        if (op.is_oversized())
            return error::invalid_push_data_size;

        if (op.is_disabled())
            return error::op_disabled;

        if (!program.increment_operation_count(op))
            return error::invalid_operation_count;

        if (program.if_(op))
        {
            if ((ec = run_op(op, program)))
                return ec;

            if (program.is_stack_overflow())
                return error::invalid_stack_size;
        }
    }

    return program.closed() ? error::success : error::invalid_stack_scope;
}

code interpreter::run(const operation& op, program& program)
{
    return run_op(op, program);
}

} // namespace machine
} // namespace system
} // namespace libbitcoin
