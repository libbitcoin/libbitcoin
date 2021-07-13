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
#ifndef LIBBITCOIN_SYSTEM_MACHINE_INTERPRETER_HPP
#define LIBBITCOIN_SYSTEM_MACHINE_INTERPRETER_HPP

#include <cstdint>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/error.hpp>
#include <bitcoin/system/chain/chain.hpp>
#include <bitcoin/system/machine/program.hpp>

namespace libbitcoin {
namespace system {
namespace machine {

class BC_API interpreter
{
public:
    typedef error::error_code_t result;

    // Operations (shared).
    //-------------------------------------------------------------------------

    static result op_nop(chain::opcode);
    static result op_disabled(chain::opcode);
    static result op_reserved(chain::opcode);
    static result op_push_number(program& program, uint8_t value);
    static result op_push_size(program& program, const chain::operation& op);
    static result op_push_data(program& program, const data_chunk& data,
        uint32_t size_limit);

    // Operations (not shared).
    //-------------------------------------------------------------------------

    static result op_if(program& program);
    static result op_notif(program& program);
    static result op_else(program& program);
    static result op_endif(program& program);
    static result op_verify(program& program);
    static result op_return(program& program);
    static result op_to_alt_stack(program& program);
    static result op_from_alt_stack(program& program);
    static result op_drop2(program& program);
    static result op_dup2(program& program);
    static result op_dup3(program& program);
    static result op_over2(program& program);
    static result op_rot2(program& program);
    static result op_swap2(program& program);
    static result op_if_dup(program& program);
    static result op_depth(program& program);
    static result op_drop(program& program);
    static result op_dup(program& program);
    static result op_nip(program& program);
    static result op_over(program& program);
    static result op_pick(program& program);
    static result op_roll(program& program);
    static result op_rot(program& program);
    static result op_swap(program& program);
    static result op_tuck(program& program);
    static result op_size(program& program);
    static result op_equal(program& program);
    static result op_equal_verify(program& program);
    static result op_add1(program& program);
    static result op_sub1(program& program);
    static result op_negate(program& program);
    static result op_abs(program& program);
    static result op_not(program& program);
    static result op_nonzero(program& program);
    static result op_add(program& program);
    static result op_sub(program& program);
    static result op_bool_and(program& program);
    static result op_bool_or(program& program);
    static result op_num_equal(program& program);
    static result op_num_equal_verify(program& program);
    static result op_num_not_equal(program& program);
    static result op_less_than(program& program);
    static result op_greater_than(program& program);
    static result op_less_than_or_equal(program& program);
    static result op_greater_than_or_equal(program& program);
    static result op_min(program& program);
    static result op_max(program& program);
    static result op_within(program& program);
    static result op_ripemd160(program& program);
    static result op_sha1(program& program);
    static result op_sha256(program& program);
    static result op_hash160(program& program);
    static result op_hash256(program& program);
    static result op_codeseparator(program& program, const chain::operation& op);
    static result op_check_sig_verify(program& program);
    static result op_check_sig(program& program);
    static result op_check_multisig_verify(program& program);
    static result op_check_multisig(program& program);
    static result op_check_locktime_verify(program& program);
    static result op_check_sequence_verify(program& program);

    /// Run program script.
    static code run(program& program);

    /// Run individual operations (idependent of the script).
    /// For best performance use script runner for a sequence of operations.
    static code run(const chain::operation& op, program& program);

private:
    static result run_op(const chain::operation& op, program& program);
};

} // namespace machine
} // namespace system
} // namespace libbitcoin

#endif
