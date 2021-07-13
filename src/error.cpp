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
#include <bitcoin/system/error.hpp>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

using namespace bc::system;

class error_category_impl
  : public std::error_category
{
public:
    virtual const char* name() const noexcept;
    virtual std::string message(int ev) const noexcept;
    virtual std::error_condition default_error_condition(int ev)
        const noexcept;
};

static const error_category_impl& get_error_category_instance()
{
    static error_category_impl instance;
    return instance;
}

const char* error_category_impl::name() const noexcept
{
    return "bitcoin";
}

std::string error_category_impl::message(int ev) const noexcept
{
    static const std::unordered_map<int, std::string> messages =
    {
        // general codes
        { error::success, "success" },
        { error::deprecated1, "deprecated1" },
        { error::unknown, "unknown error" },
        { error::not_found, "object does not exist" },
        { error::file_system, "file system error" },
        { error::not_implemented, "feature not implemented" },
        { error::oversubscribed, "service oversubscribed" },

        // database
        { error::store_block_invalid_height, "block out of order" },
        { error::store_block_missing_parent, "block missing parent" },
        { error::store_block_duplicate, "block duplicate" },
        { error::store_lock_failure, "lock failure" },
        { error::store_incorrect_state, "incorrect state" },

        // network
        { error::service_stopped, "service stopped" },
        { error::operation_failed, "operation failed" },
        { error::resolve_failed, "resolving hostname failed" },
        { error::network_unreachable, "unable to reach remote host" },
        { error::address_in_use, "address already in use" },
        { error::listen_failed, "incoming connection failed" },
        { error::accept_failed, "connection acceptance failed" },
        { error::bad_stream, "bad data stream" },
        { error::channel_timeout, "connection timed out" },
        { error::address_blocked, "address blocked by policy" },
        { error::channel_stopped, "channel stopped" },
        { error::peer_throttling, "unresponsive peer may be throttling" },

        // blockchain
        { error::duplicate_block, "duplicate block" },
        { error::orphan_block, "missing block parent" },
        { error::invalid_previous_block, "previous block failed to validate" },
        { error::insufficient_work, "insufficient work to reorganize" },
        { error::duplicate_transaction, "duplicate transaction" },
        { error::orphan_transaction, "missing transaction parent" },
        { error::transaction_version, "transaction version not standard" },
        { error::insufficient_fee, "insufficient transaction fee" },
        { error::stale_chain, "blockchain too far behind" },
        { error::dusty_transaction, "output value too low" },

        // check header
        { error::invalid_proof_of_work, "proof of work invalid" },
        { error::futuristic_timestamp, "timestamp too far in the future" },

        // accept header
        { error::checkpoints_failed, "block hash rejected by checkpoint" },
        { error::invalid_block_version, "block version rejected at current height" },
        { error::incorrect_proof_of_work, "proof of work does not match bits field" },
        { error::timestamp_too_early, "block timestamp is too early" },

        // check block
        { error::block_size_limit, "block size limit exceeded" },
        { error::empty_block, "block has no transactions" },
        { error::first_not_coinbase, "first transaction not a coinbase" },
        { error::extra_coinbases, "more than one coinbase" },
        { error::internal_duplicate, "matching transaction hashes in block" },
        { error::block_internal_double_spend, "double spend internal to block" },
        { error::forward_reference, "transactions out of order" },
        { error::merkle_mismatch, "merkle root mismatch" },
        { error::block_legacy_sigop_limit, "too many block legacy signature operations" },

        // accept block
        { error::block_non_final, "block contains a non-final transaction" },
        { error::coinbase_height_mismatch, "block height mismatch in coinbase" },
        { error::coinbase_value_limit, "coinbase value too high" },
        { error::block_embedded_sigop_limit, "too many block embedded signature operations" },
        { error::invalid_witness_commitment, "invalid witness commitment" },
        { error::block_weight_limit, "block weight limit exceeded" },

        // check transaction
        { error::empty_transaction, "transaction inputs or outputs empty" },
        { error::previous_output_null, "non-coinbase transaction has input with null previous output" },
        { error::spend_overflow, "spend outside valid range" },
        { error::invalid_coinbase_script_size, "coinbase script too small or large" },
        { error::coinbase_transaction, "coinbase transaction disallowed in memory pool" },
        { error::transaction_internal_double_spend, "double spend internal to transaction" },
        { error::transaction_size_limit, "transaction size limit exceeded" },
        { error::transaction_legacy_sigop_limit, "too many transaction legacy signature operations" },

        // accept transaction
        { error::transaction_non_final, "transaction currently non-final for next block" },
        { error::premature_validation, "transaction validation under checkpoint" },
        { error::unspent_duplicate, "matching transaction with unspent outputs" },
        { error::missing_previous_output, "previous output not found" },
        { error::double_spend, "double spend of input" },
        { error::coinbase_maturity, "immature coinbase spent" },
        { error::spend_exceeds_value, "spend exceeds value of inputs" },
        { error::transaction_embedded_sigop_limit, "too many transaction embedded signature operations" },
        { error::sequence_locked, "transaction currently locked" },
        { error::transaction_weight_limit, "transaction weight limit exceeded" },

        // connect input
        { error::invalid_script, "invalid script" },
        { error::invalid_script_size, "invalid script size" },
        { error::invalid_push_data_size, "invalid push data size" },
        { error::invalid_operation_count, "invalid operation count" },
        { error::invalid_stack_size, "invalid stack size" },
        { error::invalid_stack_scope, "invalid stack scope" },
        { error::invalid_script_embed, "invalid script embed" },
        { error::invalid_signature_encoding, "invalid signature encoding" },
        { error::deprecated2, "deprecated2" },
        { error::incorrect_signature, "incorrect signature" },
        { error::unexpected_witness, "unexpected witness" },
        { error::invalid_witness, "invalid witness" },
        { error::dirty_witness, "dirty witness" },
        { error::stack_false, "stack false" },

        // http
        { error::http_invalid_request, "invalid request" },
        { error::http_method_not_found, "method not found" },
        { error::http_internal_error, "internal error" },

        // op eval
        { error::op_disabled, "op_disabled" },
        { error::op_reserved, "op_reserved" },
        { error::op_push_size, "op_push_size" },
        { error::op_push_data, "op_push_data" },
        { error::op_if, "op_if" },
        { error::op_notif, "op_notif" },
        { error::op_else, "op_else" },
        { error::op_endif, "op_endif" },
        { error::op_verify1, "op_verify1" },
        { error::op_verify2, "op_verify2" },
        { error::op_return, "op_return" },
        { error::op_to_alt_stack, "op_to_alt_stack" },
        { error::op_from_alt_stack, "op_from_alt_stack" },
        { error::op_drop2, "op_drop2" },
        { error::op_dup2, "op_dup2" },
        { error::op_dup3, "op_dup3" },
        { error::op_over2, "op_over2" },
        { error::op_rot2, "op_rot2" },
        { error::op_swap2, "op_swap2" },
        { error::op_if_dup, "op_if_dup" },
        { error::op_drop, "op_drop" },
        { error::op_dup, "op_dup" },
        { error::op_nip, "op_nip" },
        { error::op_over, "op_over" },
        { error::op_pick, "op_pick" },
        { error::op_roll, "op_roll" },
        { error::op_rot, "op_rot" },
        { error::op_swap, "op_swap" },
        { error::op_tuck, "op_tuck" },
        { error::op_size, "op_size" },
        { error::op_equal, "op_equal" },
        { error::op_equal_verify1, "op_equal_verify1" },
        { error::op_equal_verify2, "op_equal_verify2" },
        { error::op_add1, "op_add1" },
        { error::op_sub1, "op_sub1" },
        { error::op_negate, "op_negate" },
        { error::op_abs, "op_abs" },
        { error::op_not, "op_not" },
        { error::op_nonzero, "op_nonzero" },
        { error::op_add, "op_add" },
        { error::op_sub, "op_sub" },
        { error::op_bool_and, "op_bool_and" },
        { error::op_bool_or, "op_bool_or" },
        { error::op_num_equal, "op_num_equal" },
        { error::op_num_equal_verify1, "op_num_equal_verify1" },
        { error::op_num_equal_verify2, "op_num_equal_verify2" },
        { error::op_num_not_equal, "op_num_not_equal" },
        { error::op_less_than, "op_less_than" },
        { error::op_greater_than, "op_greater_than" },
        { error::op_less_than_or_equal, "op_less_than_or_equal" },
        { error::op_greater_than_or_equal, "op_greater_than_or_equal" },
        { error::op_min, "op_min" },
        { error::op_max, "op_max" },
        { error::op_within, "op_within" },
        { error::op_ripemd160, "op_ripemd160" },
        { error::op_sha1, "op_sha1" },
        { error::op_sha256, "op_sha256" },
        { error::op_hash160, "op_hash160" },
        { error::op_hash256, "op_hash256" },
        { error::op_code_seperator, "op_code_seperator" },
        { error::op_check_sig_verify1, "op_check_sig_verify1" },
        { error::op_check_sig, "op_check_sig" },
        { error::op_check_multisig_verify1, "op_check_multisig_verify1" },
        { error::op_check_multisig_verify2, "op_check_multisig_verify2" },
        { error::op_check_multisig_verify3, "op_check_multisig_verify3" },
        { error::op_check_multisig_verify4, "op_check_multisig_verify4" },
        { error::op_check_multisig_verify5, "op_check_multisig_verify5" },
        { error::op_check_multisig_verify6, "op_check_multisig_verify6" },
        { error::op_check_multisig_verify7, "op_check_multisig_verify7" },
        { error::op_check_multisig, "op_check_multisig" },
        { error::op_check_locktime_verify1, "op_check_locktime_verify1" },
        { error::op_check_locktime_verify2, "op_check_locktime_verify2" },
        { error::op_check_locktime_verify3, "op_check_locktime_verify3" },
        { error::op_check_locktime_verify4, "op_check_locktime_verify4" },
        { error::op_check_locktime_verify5, "op_check_locktime_verify5" },
        { error::op_check_locktime_verify6, "op_check_locktime_verify6" },
        { error::op_check_sequence_verify1, "op_check_sequence_verify1" },
        { error::op_check_sequence_verify2, "op_check_sequence_verify2" },
        { error::op_check_sequence_verify3, "op_check_sequence_verify3" },
        { error::op_check_sequence_verify4, "op_check_sequence_verify4" },
        { error::op_check_sequence_verify5, "op_check_sequence_verify5" },
        { error::op_check_sequence_verify6, "op_check_sequence_verify6" },
        { error::op_check_sequence_verify7, "op_check_sequence_verify7" },

        // Added out of order (bip147).
        { error::op_check_multisig_verify8, "op_check_multisig_verify8" },

        // Added bip157 blockchain related
        { error::unrecognized_filter_type, "Unrecognized filter type" },
        { error::invalid_response_range, "The range of responses is invalid" },
        { error::configuration_disabled, "Disabled by configuration" },
        { error::metadata_prevout_missing, "Missing expected cached prevout" }
    };

    const auto message = messages.find(ev);
    return message != messages.end() ? message->second : "invalid code";
}

// We are not currently using this.
std::error_condition error_category_impl::default_error_condition(int ev)
    const noexcept
{
    return std::error_condition(ev, *this);
}

namespace libbitcoin {
namespace system {
namespace error {

    code make_error_code(error_code_t e)
    {
        return code(e, get_error_category_instance());
    }

    std::error_condition make_error_condition(error_condition_t e)
    {
        return std::error_condition(e, get_error_category_instance());
    }

    error_code_t boost_to_error_code(const boost_code& ec)
    {
        namespace boost_error = boost::system::errc;

#ifdef _MSC_VER
        // TODO: is there a means to map ASIO errors to boost errors?
        // ASIO codes are unique on Windows but not on Linux.
        namespace asio_error = boost::asio::error;
#endif
        // TODO: use a static map (hash table)
        switch (ec.value())
        {
            // There should be no boost mapping to the shutdown sentinel.
            //    return error::service_stopped;

            case boost_error::success:
                return error::success;

            // network errors
#ifdef _MSC_VER
            case asio_error::connection_aborted:
            case asio_error::connection_reset:
            case asio_error::operation_aborted:
            case asio_error::operation_not_supported:
#endif
            case boost_error::connection_aborted:
            case boost_error::connection_refused:
            case boost_error::connection_reset:
            case boost_error::not_connected:
            case boost_error::operation_canceled:
            case boost_error::operation_not_permitted:
            case boost_error::operation_not_supported:
            case boost_error::owner_dead:
            case boost_error::permission_denied:
                return error::operation_failed;

#ifdef _MSC_VER
            case asio_error::address_family_not_supported:
#endif
            case boost_error::address_family_not_supported:
            case boost_error::address_not_available:
            case boost_error::bad_address:
            case boost_error::destination_address_required:
                return error::resolve_failed;

            case boost_error::broken_pipe:
            case boost_error::host_unreachable:
            case boost_error::network_down:
            case boost_error::network_reset:
            case boost_error::network_unreachable:
            case boost_error::no_link:
            case boost_error::no_protocol_option:
            case boost_error::no_such_file_or_directory:
            case boost_error::not_a_socket:
            case boost_error::protocol_not_supported:
            case boost_error::wrong_protocol_type:
                return error::network_unreachable;

            case boost_error::address_in_use:
            case boost_error::already_connected:
            case boost_error::connection_already_in_progress:
            case boost_error::operation_in_progress:
                return error::address_in_use;

            case boost_error::bad_message:
            case boost_error::illegal_byte_sequence:
            case boost_error::io_error:
            case boost_error::message_size:
            case boost_error::no_message_available:
            case boost_error::no_message:
            case boost_error::no_stream_resources:
            case boost_error::not_a_stream:
            case boost_error::protocol_error:
                return error::bad_stream;

#ifdef _MSC_VER
            case asio_error::timed_out:
#endif
            case boost_error::stream_timeout:
            case boost_error::timed_out:
                return error::channel_timeout;

            // file system errors
            case boost_error::cross_device_link:
            case boost_error::bad_file_descriptor:
            case boost_error::device_or_resource_busy:
            case boost_error::directory_not_empty:
            case boost_error::executable_format_error:
            case boost_error::file_exists:
            case boost_error::file_too_large:
            case boost_error::filename_too_long:
            case boost_error::invalid_seek:
            case boost_error::is_a_directory:
            case boost_error::no_space_on_device:
            case boost_error::no_such_device:
            case boost_error::no_such_device_or_address:
            case boost_error::read_only_file_system:
            // same as operation_would_block on non-windows
            //case boost_error::resource_unavailable_try_again:
            case boost_error::text_file_busy:
            case boost_error::too_many_files_open:
            case boost_error::too_many_files_open_in_system:
            case boost_error::too_many_links:
            case boost_error::too_many_symbolic_link_levels:
                return error::file_system;

            // unknown errors
            case boost_error::argument_list_too_long:
            case boost_error::argument_out_of_domain:
            case boost_error::function_not_supported:
            case boost_error::identifier_removed:
            case boost_error::inappropriate_io_control_operation:
            case boost_error::interrupted:
            case boost_error::invalid_argument:
            case boost_error::no_buffer_space:
            case boost_error::no_child_process:
            case boost_error::no_lock_available:
            case boost_error::no_such_process:
            case boost_error::not_a_directory:
            case boost_error::not_enough_memory:
            case boost_error::operation_would_block:
            case boost_error::resource_deadlock_would_occur:
            case boost_error::result_out_of_range:
            case boost_error::state_not_recoverable:
            case boost_error::value_too_large:
            default:
                return error::unknown;
        }
    }

    error_code_t posix_to_error_code(int ec)
    {
        // TODO: expand mapping for database scenario.
        switch (ec)
        {
            // protocol codes (from zeromq)
            case ENOBUFS:
            case ENOTSUP:
            case EPROTONOSUPPORT:
                return error::operation_failed;
            case ENETDOWN:
                return error::network_unreachable;
            case EADDRINUSE:
                return error::address_in_use;
            case EADDRNOTAVAIL:
                return error::resolve_failed;
            case ECONNREFUSED:
                return error::accept_failed;
            case EINPROGRESS:
                return error::channel_timeout;
                return error::bad_stream;
            case EAGAIN:
                return error::channel_timeout;
            case EFAULT:
                return error::bad_stream;
            case EINTR:
            case ENOTSOCK:
                return error::service_stopped;
            default:
                return error::unknown;
        }
    }

} // namespace error
} // namespace system
} // namespace libbitcoin
