###############################################################################
#  Copyright (c) 2014-2020 libbitcoin-system developers (see COPYING).
#
#         GENERATED SOURCE CODE, DO NOT EDIT EXCEPT EXPERIMENTALLY
#
###############################################################################

# libbitcoin-system project configuration.
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(libbitcoin-system LANGUAGES C CXX)

enable_testing()

list( APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules" )
include(CheckIncludeFiles)
include(CheckSymbolExists)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (MSVC)
  set( CANONICAL_LIB_NAME libbitcoin-system )
else ()
  set( CANONICAL_LIB_NAME bitcoin-system )
  find_package( PkgConfig REQUIRED )

  set( prefix "${CMAKE_PREFIX_PATH}" )
  set( exec_prefix "\${prefix}" )
  set( libdir "\${exec_prefix}/lib" )
  set( includedir "\${exec_prefix}/include" )

  set( PACKAGE_VERSION "4.0.0" )
  set( VERSION "${PACKAGE_VERSION}" )
endif ()

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

# Add compiler options
#------------------------------------------------------------------------------
# Warn on all stuff.
add_compile_options( "-Wall" )

# Warn on extra stuff.
add_compile_options( "-Wextra" )

# Be really annoying.
add_compile_options( "-Wpedantic" )

# Disallow warning on style order of declarations.
add_compile_options( "-Wno-reorder" )

# Suppress warning for incomplete field initialization.
add_compile_options( "-Wno-missing-field-initializers" )

# Conform to style.
add_compile_options( "-Wno-missing-braces" )

# Ignore comments within comments or commenting of backslash extended lines.
add_compile_options( "-Wno-comment" )

# Allow use of C99 'long long' type.
add_compile_options( "-Wno-long-long" )

# Conflict in stdlib under clang.
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options( "-Wno-mismatched-tags" )
endif()

# Limit delays and warnings.
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options( "-fno-var-tracking-assignments" )
endif()

# Implement -Dpkgconfigdir and output ${pkgconfigdir}.
#------------------------------------------------------------------------------
set( pkgconfigdir "${libdir}/pkgconfig" CACHE PATH "Path to pkgconfig directory." )

# Implement -Dwith-tests and declare with-tests.
#------------------------------------------------------------------------------
set( with-tests "yes" CACHE BOOL "Compile with unit tests." )

# Implement -Dwith-examples and declare with-examples.
#------------------------------------------------------------------------------
set( with-examples "yes" CACHE BOOL "Compile with examples." )

# Implement -Dwith-icu and define BOOST_HAS_ICU and output ${icu}.
#------------------------------------------------------------------------------
set( with-icu "no" CACHE BOOL "Compile with International Components for Unicode." )

if (with-icu)
    add_definitions( -DBOOST_HAS_ICU )
endif()

if (with-icu)
    set( icu "-DWITH_ICU" )
endif()

# Implement -Denable-ndebug and define NDEBUG.
#------------------------------------------------------------------------------
set( enable-ndebug "yes" CACHE BOOL "Compile without debug assertions." )

if (enable-ndebug)
    add_definitions( -DNDEBUG )
endif()

# Inherit -Denable-shared and define BOOST_ALL_DYN_LINK.
#------------------------------------------------------------------------------
if (BUILD_SHARED_LIBS)
    add_definitions( -DBOOST_ALL_DYN_LINK )
endif()

# Find boost
#------------------------------------------------------------------------------
find_package( Boost 1.72.0 REQUIRED COMPONENTS
    atomic
    chrono
    date_time
    filesystem
    iostreams
    locale
    log_setup
    log
    program_options
    regex
    system
    thread
    unit_test_framework )

set( boost_atomic_LIBS "-lboost_atomic" )
set( boost_chrono_LIBS "-lboost_chrono" )
set( boost_date_time_LIBS "-lboost_date_time" )
set( boost_filesystem_LIBS "-lboost_filesystem" )
set( boost_iostreams_LIBS "-lboost_iostreams" )
set( boost_locale_LIBS "-lboost_locale" )
set( boost_log_setup_LIBS "-lboost_log_setup" )
set( boost_log_LIBS "-lboost_log" )
set( boost_program_options_LIBS "-lboost_program_options" )
set( boost_regex_LIBS "-lboost_regex" )
set( boost_system_LIBS "-lboost_system" )
set( boost_thread_LIBS "-lboost_thread" )
set( boost_unit_test_framework_LIBS "-lboost_unit_test_framework" )

if (enable-ndebug)
    set( Boost_LIBRARY_DIR "${Boost_LIBRARY_DIR_DEBUG}" )
else ()
    set( Boost_LIBRARY_DIR "${Boost_LIBRARY_DIR_RELEASE}" )
endif()

set( boost_CPPFLAGS "-I${Boost_INCLUDE_DIR}" )
set( boost_LDFLAGS "-L${Boost_LIBRARY_DIR}" )

set( CMAKE_THREAD_PREFER_PTHREAD ON )
set( THREADS_PREFER_PTHREAD_FLAG ON )

# Find threads
#------------------------------------------------------------------------------
if (NOT ANDROID)
    find_package( Threads REQUIRED )
endif()

if ( Threads_FOUND AND NOT ANDROID )
    # TODO: find proper detection - documentation on FindThreads appears incorrect.
    set( pthread_CPPFLAGS "" )
    set( pthread_LIBRARIES "-lpthread" )
    set( pthread_LIBS "-lpthread" )
endif()

# Find rt
#------------------------------------------------------------------------------
if ((${CMAKE_SYSTEM_NAME} MATCHES "Linux") AND NOT ANDROID)
    find_package( Rt REQUIRED )
endif()

# Find icu-i18n
#------------------------------------------------------------------------------
if (with-icu)
    find_package( Icu-I18N 55.2 REQUIRED )
endif()

# Find dl
#------------------------------------------------------------------------------
if ((${CMAKE_SYSTEM_NAME} MATCHES "Linux"))
    find_package( Dl REQUIRED )
endif()

# Find secp256k1
#------------------------------------------------------------------------------
find_package( Secp256K1 0.1.0.19 REQUIRED )

# Define project common includes directories
#------------------------------------------------------------------------------
if (BUILD_SHARED_LIBS)
    include_directories( SYSTEM
        ${Boost_INCLUDE_DIR}
        ${pthread_INCLUDE_DIRS}
        ${rt_INCLUDE_DIRS}
        ${icu_i18n_INCLUDE_DIRS}
        ${dl_INCLUDE_DIRS}
        ${secp256k1_INCLUDE_DIRS} )
else()
    include_directories( SYSTEM
        ${Boost_INCLUDE_DIR}
        ${pthread_INCLUDE_DIRS}
        ${rt_INCLUDE_DIRS}
        ${icu_i18n_STATIC_INCLUDE_DIRS}
        ${dl_INCLUDE_DIRS}
        ${secp256k1_STATIC_INCLUDE_DIRS} )
endif()

# Define project common library directories
#------------------------------------------------------------------------------
if (BUILD_SHARED_LIBS)
    link_directories(
        ${Boost_LIBRARY_DIRS}
        ${pthread_LIBRARY_DIRS}
        ${rt_LIBRARY_DIRS}
        ${icu_i18n_LIBRARY_DIRS}
        ${dl_LIBRARY_DIRS}
        ${secp256k1_LIBRARY_DIRS} )
else()
    link_directories(
        ${Boost_LIBRARY_DIRS}
        ${pthread_LIBRARY_DIRS}
        ${rt_LIBRARY_DIRS}
        ${icu_i18n_STATIC_LIBRARY_DIRS}
        ${dl_LIBRARY_DIRS}
        ${secp256k1_STATIC_LIBRARY_DIRS} )
endif()

# Define project common libraries/linker flags.
#------------------------------------------------------------------------------
if (BUILD_SHARED_LIBS)
    link_libraries(
        "-fstack-protector"
        "-fstack-protector-all"
        ${Boost_CHRONO_LIBRARY}
        ${Boost_DATE_TIME_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_IOSTREAMS_LIBRARY}
        ${Boost_LOCALE_LIBRARY}
        ${Boost_LOG_LIBRARY}
        ${Boost_PROGRAM_OPTIONS_LIBRARY}
        ${Boost_REGEX_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        ${pthread_LIBRARIES}
        ${rt_LIBRARIES}
        ${icu_i18n_LIBRARIES}
        ${dl_LIBRARIES}
        ${secp256k1_LIBRARIES} )
else()
    link_libraries(
        "-fstack-protector"
        "-fstack-protector-all"
        ${Boost_CHRONO_LIBRARY}
        ${Boost_DATE_TIME_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_IOSTREAMS_LIBRARY}
        ${Boost_LOCALE_LIBRARY}
        ${Boost_LOG_LIBRARY}
        ${Boost_PROGRAM_OPTIONS_LIBRARY}
        ${Boost_REGEX_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        ${pthread_LIBRARIES}
        ${rt_LIBRARIES}
        ${icu_i18n_STATIC_LIBRARIES}
        ${dl_LIBRARIES}
        ${secp256k1_STATIC_LIBRARIES} )
endif()

add_definitions(
    ${icu} )

# Define ${CANONICAL_LIB_NAME} project.
#------------------------------------------------------------------------------
add_library( ${CANONICAL_LIB_NAME}
    "../../src/error.cpp"
    "../../src/properties.cpp"
    "../../src/settings.cpp"
    "../../src/chain/block.cpp"
    "../../src/chain/block_filter.cpp"
    "../../src/chain/chain_state.cpp"
    "../../src/chain/check_point.cpp"
    "../../src/chain/compact.cpp"
    "../../src/chain/header.cpp"
    "../../src/chain/input.cpp"
    "../../src/chain/operation.cpp"
    "../../src/chain/output.cpp"
    "../../src/chain/output_point.cpp"
    "../../src/chain/payment_record.cpp"
    "../../src/chain/point.cpp"
    "../../src/chain/point_value.cpp"
    "../../src/chain/points_value.cpp"
    "../../src/chain/script.cpp"
    "../../src/chain/stealth_record.cpp"
    "../../src/chain/transaction.cpp"
    "../../src/chain/witness.cpp"
    "../../src/chain/enums/opcode.cpp"
    "../../src/config/authority.cpp"
    "../../src/config/base16.cpp"
    "../../src/config/base2.cpp"
    "../../src/config/base32.cpp"
    "../../src/config/base58.cpp"
    "../../src/config/base64.cpp"
    "../../src/config/base85.cpp"
    "../../src/config/block.cpp"
    "../../src/config/compact_filter.cpp"
    "../../src/config/directory.cpp"
    "../../src/config/endpoint.cpp"
    "../../src/config/hash160.cpp"
    "../../src/config/hash256.cpp"
    "../../src/config/header.cpp"
    "../../src/config/input.cpp"
    "../../src/config/output.cpp"
    "../../src/config/parameter.cpp"
    "../../src/config/parser.cpp"
    "../../src/config/point.cpp"
    "../../src/config/printer.cpp"
    "../../src/config/script.cpp"
    "../../src/config/transaction.cpp"
    "../../src/crypto/checksum.cpp"
    "../../src/crypto/elliptic_curve.cpp"
    "../../src/crypto/encryption.cpp"
    "../../src/crypto/golomb_coding.cpp"
    "../../src/crypto/hash.cpp"
    "../../src/crypto/pseudo_random.cpp"
    "../../src/crypto/ring_signature.cpp"
    "../../src/crypto/secp256k1_initializer.cpp"
    "../../src/crypto/secp256k1_initializer.hpp"
    "../../src/crypto/siphash.cpp"
    "../../src/crypto/external/aes256.c"
    "../../src/crypto/external/aes256.h"
    "../../src/crypto/external/crypto_scrypt.c"
    "../../src/crypto/external/crypto_scrypt.h"
    "../../src/crypto/external/hmac_sha256.c"
    "../../src/crypto/external/hmac_sha256.h"
    "../../src/crypto/external/hmac_sha512.c"
    "../../src/crypto/external/hmac_sha512.h"
    "../../src/crypto/external/lax_der_parsing.c"
    "../../src/crypto/external/lax_der_parsing.h"
    "../../src/crypto/external/pbkdf2_sha256.c"
    "../../src/crypto/external/pbkdf2_sha256.h"
    "../../src/crypto/external/pkcs5_pbkdf2.c"
    "../../src/crypto/external/pkcs5_pbkdf2.h"
    "../../src/crypto/external/ripemd160.c"
    "../../src/crypto/external/ripemd160.h"
    "../../src/crypto/external/sha1.c"
    "../../src/crypto/external/sha1.h"
    "../../src/crypto/external/sha256.c"
    "../../src/crypto/external/sha256.h"
    "../../src/crypto/external/sha512.c"
    "../../src/crypto/external/sha512.h"
    "../../src/crypto/external/zeroize.c"
    "../../src/crypto/external/zeroize.h"
    "../../src/data/data_chunk.cpp"
    "../../src/data/data_reference.cpp"
    "../../src/data/data_slab.cpp"
    "../../src/data/data_slice.cpp"
    "../../src/data/string.cpp"
    "../../src/machine/interpreter.cpp"
    "../../src/machine/number.cpp"
    "../../src/machine/program.cpp"
    "../../src/message/address.cpp"
    "../../src/message/alert.cpp"
    "../../src/message/alert_payload.cpp"
    "../../src/message/block.cpp"
    "../../src/message/block_transactions.cpp"
    "../../src/message/compact_block.cpp"
    "../../src/message/compact_filter.cpp"
    "../../src/message/compact_filter_checkpoint.cpp"
    "../../src/message/compact_filter_headers.cpp"
    "../../src/message/fee_filter.cpp"
    "../../src/message/filter_add.cpp"
    "../../src/message/filter_clear.cpp"
    "../../src/message/filter_load.cpp"
    "../../src/message/get_address.cpp"
    "../../src/message/get_block_transactions.cpp"
    "../../src/message/get_blocks.cpp"
    "../../src/message/get_compact_filter_checkpoint.cpp"
    "../../src/message/get_compact_filter_headers.cpp"
    "../../src/message/get_compact_filters.cpp"
    "../../src/message/get_data.cpp"
    "../../src/message/get_headers.cpp"
    "../../src/message/header.cpp"
    "../../src/message/headers.cpp"
    "../../src/message/heading.cpp"
    "../../src/message/inventory.cpp"
    "../../src/message/inventory_vector.cpp"
    "../../src/message/memory_pool.cpp"
    "../../src/message/merkle_block.cpp"
    "../../src/message/message.cpp"
    "../../src/message/network_address.cpp"
    "../../src/message/not_found.cpp"
    "../../src/message/ping.cpp"
    "../../src/message/pong.cpp"
    "../../src/message/prefilled_transaction.cpp"
    "../../src/message/reject.cpp"
    "../../src/message/send_compact.cpp"
    "../../src/message/send_headers.cpp"
    "../../src/message/transaction.cpp"
    "../../src/message/verack.cpp"
    "../../src/message/version.cpp"
    "../../src/radix/base_10.cpp"
    "../../src/radix/base_16.cpp"
    "../../src/radix/base_2048.cpp"
    "../../src/radix/base_32.cpp"
    "../../src/radix/base_58.cpp"
    "../../src/radix/base_64.cpp"
    "../../src/radix/base_85.cpp"
    "../../src/stream/binary.cpp"
    "../../src/unicode/ascii.cpp"
    "../../src/unicode/code_points.cpp"
    "../../src/unicode/conversion.cpp"
    "../../src/unicode/normalization.cpp"
    "../../src/unicode/utf8_everywhere/utf8_console_streambuf.cpp"
    "../../src/unicode/utf8_everywhere/utf8_environment.cpp"
    "../../src/unicode/utf8_everywhere/utf8_ifstream.cpp"
    "../../src/unicode/utf8_everywhere/utf8_istream.cpp"
    "../../src/unicode/utf8_everywhere/utf8_ofstream.cpp"
    "../../src/unicode/utf8_everywhere/utf8_ostream.cpp"
    "../../src/unicode/utf8_everywhere/utf8_streambuf.cpp"
    "../../src/wallet/context.cpp"
    "../../src/wallet/message.cpp"
    "../../src/wallet/neutrino_filter.cpp"
    "../../src/wallet/addresses/bitcoin_uri.cpp"
    "../../src/wallet/addresses/payment_address.cpp"
    "../../src/wallet/addresses/qr_code.cpp"
    "../../src/wallet/addresses/stealth_address.cpp"
    "../../src/wallet/addresses/stealth_receiver.cpp"
    "../../src/wallet/addresses/stealth_sender.cpp"
    "../../src/wallet/addresses/tiff.cpp"
    "../../src/wallet/addresses/uri.cpp"
    "../../src/wallet/addresses/witness_address.cpp"
    "../../src/wallet/addresses/qrencode/bitstream.c"
    "../../src/wallet/addresses/qrencode/bitstream.h"
    "../../src/wallet/addresses/qrencode/mask.c"
    "../../src/wallet/addresses/qrencode/mask.h"
    "../../src/wallet/addresses/qrencode/mmask.c"
    "../../src/wallet/addresses/qrencode/mmask.h"
    "../../src/wallet/addresses/qrencode/mqrspec.c"
    "../../src/wallet/addresses/qrencode/mqrspec.h"
    "../../src/wallet/addresses/qrencode/qrencode.c"
    "../../src/wallet/addresses/qrencode/qrencode.h"
    "../../src/wallet/addresses/qrencode/qrencode_inner.h"
    "../../src/wallet/addresses/qrencode/qrinput.c"
    "../../src/wallet/addresses/qrencode/qrinput.h"
    "../../src/wallet/addresses/qrencode/qrspec.c"
    "../../src/wallet/addresses/qrencode/qrspec.h"
    "../../src/wallet/addresses/qrencode/rsecc.c"
    "../../src/wallet/addresses/qrencode/rsecc.h"
    "../../src/wallet/addresses/qrencode/split.c"
    "../../src/wallet/addresses/qrencode/split.h"
    "../../src/wallet/keys/ec_point.cpp"
    "../../src/wallet/keys/ec_private.cpp"
    "../../src/wallet/keys/ec_public.cpp"
    "../../src/wallet/keys/ec_scalar.cpp"
    "../../src/wallet/keys/ek_private.cpp"
    "../../src/wallet/keys/ek_public.cpp"
    "../../src/wallet/keys/ek_token.cpp"
    "../../src/wallet/keys/encrypted_keys.cpp"
    "../../src/wallet/keys/hd_private.cpp"
    "../../src/wallet/keys/hd_public.cpp"
    "../../src/wallet/keys/mini_keys.cpp"
    "../../src/wallet/keys/stealth.cpp"
    "../../src/wallet/keys/parse_encrypted_keys/parse_encrypted_key.hpp"
    "../../src/wallet/keys/parse_encrypted_keys/parse_encrypted_key.ipp"
    "../../src/wallet/keys/parse_encrypted_keys/parse_encrypted_prefix.hpp"
    "../../src/wallet/keys/parse_encrypted_keys/parse_encrypted_prefix.ipp"
    "../../src/wallet/keys/parse_encrypted_keys/parse_encrypted_private.cpp"
    "../../src/wallet/keys/parse_encrypted_keys/parse_encrypted_private.hpp"
    "../../src/wallet/keys/parse_encrypted_keys/parse_encrypted_public.cpp"
    "../../src/wallet/keys/parse_encrypted_keys/parse_encrypted_public.hpp"
    "../../src/wallet/keys/parse_encrypted_keys/parse_encrypted_token.cpp"
    "../../src/wallet/keys/parse_encrypted_keys/parse_encrypted_token.hpp"
    "../../src/wallet/mnemonics/electrum.cpp"
    "../../src/wallet/mnemonics/electrum_v1.cpp"
    "../../src/wallet/mnemonics/mnemonic.cpp"
    "../../src/words/languages.cpp"
    "../../src/words/catalogs/electrum.cpp"
    "../../src/words/catalogs/electrum_v1.cpp"
    "../../src/words/catalogs/mnemonic.cpp" )

# ${CANONICAL_LIB_NAME} project specific include directories.
#------------------------------------------------------------------------------
if (BUILD_SHARED_LIBS)
    target_include_directories( ${CANONICAL_LIB_NAME} PRIVATE
        "../../include"
        ${Boost_INCLUDE_DIR}
        ${pthread_INCLUDE_DIRS}
        ${rt_INCLUDE_DIRS}
        ${icu_i18n_INCLUDE_DIRS}
        ${dl_INCLUDE_DIRS}
        ${secp256k1_INCLUDE_DIRS} )
else()
    target_include_directories( ${CANONICAL_LIB_NAME} PRIVATE
        "../../include"
        ${Boost_INCLUDE_DIR}
        ${pthread_INCLUDE_DIRS}
        ${rt_INCLUDE_DIRS}
        ${icu_i18n_STATIC_INCLUDE_DIRS}
        ${dl_INCLUDE_DIRS}
        ${secp256k1_STATIC_INCLUDE_DIRS} )
endif()

target_include_directories( ${CANONICAL_LIB_NAME} PUBLIC
"../../include" )

# ${CANONICAL_LIB_NAME} project specific libraries/linker flags.
#------------------------------------------------------------------------------
if (BUILD_SHARED_LIBS)
    target_link_libraries( ${CANONICAL_LIB_NAME}
        ${Boost_CHRONO_LIBRARY}
        ${Boost_DATE_TIME_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_IOSTREAMS_LIBRARY}
        ${Boost_LOCALE_LIBRARY}
        ${Boost_LOG_LIBRARY}
        ${Boost_PROGRAM_OPTIONS_LIBRARY}
        ${Boost_REGEX_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        ${pthread_LIBRARIES}
        ${rt_LIBRARIES}
        ${icu_i18n_LIBRARIES}
        ${dl_LIBRARIES}
        ${secp256k1_LIBRARIES} )
else()
    target_link_libraries( ${CANONICAL_LIB_NAME}
        ${Boost_CHRONO_LIBRARY}
        ${Boost_DATE_TIME_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_IOSTREAMS_LIBRARY}
        ${Boost_LOCALE_LIBRARY}
        ${Boost_LOG_LIBRARY}
        ${Boost_PROGRAM_OPTIONS_LIBRARY}
        ${Boost_REGEX_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        ${pthread_LIBRARIES}
        ${rt_LIBRARIES}
        ${icu_i18n_STATIC_LIBRARIES}
        ${dl_LIBRARIES}
        ${secp256k1_STATIC_LIBRARIES} )
endif()

# Define libbitcoin-system-examples project.
#------------------------------------------------------------------------------
if (with-examples)
    add_executable( libbitcoin-system-examples
        "../../examples/main.cpp" )

#     libbitcoin-system-examples project specific include directories.
#------------------------------------------------------------------------------
    target_include_directories( libbitcoin-system-examples PRIVATE
        "../../include" )

#     libbitcoin-system-examples project specific libraries/linker flags.
#------------------------------------------------------------------------------
    target_link_libraries( libbitcoin-system-examples
        ${CANONICAL_LIB_NAME} )

endif()

# Define libbitcoin-system-test project.
#------------------------------------------------------------------------------
if (with-tests)
    add_executable( libbitcoin-system-test
        "../../test/constants.cpp"
        "../../test/constraints.cpp"
        "../../test/main.cpp"
        "../../test/properties.cpp"
        "../../test/settings.cpp"
        "../../test/test.cpp"
        "../../test/test.hpp"
        "../../test/chain/block.cpp"
        "../../test/chain/block_filter.cpp"
        "../../test/chain/chain_state.cpp"
        "../../test/chain/check_point.cpp"
        "../../test/chain/compact.cpp"
        "../../test/chain/header.cpp"
        "../../test/chain/input.cpp"
        "../../test/chain/output.cpp"
        "../../test/chain/output_point.cpp"
        "../../test/chain/payment_record.cpp"
        "../../test/chain/point.cpp"
        "../../test/chain/point_value.cpp"
        "../../test/chain/points_value.cpp"
        "../../test/chain/satoshi_words.cpp"
        "../../test/chain/script.cpp"
        "../../test/chain/script.hpp"
        "../../test/chain/stealth_record.cpp"
        "../../test/chain/transaction.cpp"
        "../../test/chain/enums/opcode.cpp"
        "../../test/config/authority.cpp"
        "../../test/config/base16.cpp"
        "../../test/config/base2.cpp"
        "../../test/config/base32.cpp"
        "../../test/config/base58.cpp"
        "../../test/config/base64.cpp"
        "../../test/config/base85.cpp"
        "../../test/config/block.cpp"
        "../../test/config/compact_filter.cpp"
        "../../test/config/endpoint.cpp"
        "../../test/config/hash256.cpp"
        "../../test/config/parameter.cpp"
        "../../test/config/printer.cpp"
        "../../test/crypto/checksum.cpp"
        "../../test/crypto/elliptic_curve.cpp"
        "../../test/crypto/hash.cpp"
        "../../test/crypto/hash.hpp"
        "../../test/crypto/pseudo_random.cpp"
        "../../test/crypto/ring_signature.cpp"
        "../../test/crypto/siphash.cpp"
        "../../test/crypto/siphash.hpp"
        "../../test/data/collection.cpp"
        "../../test/data/data_array.cpp"
        "../../test/data/data_chunk.cpp"
        "../../test/data/data_reference.cpp"
        "../../test/data/data_slab.cpp"
        "../../test/data/data_slice.cpp"
        "../../test/data/integer.cpp"
        "../../test/data/string.cpp"
        "../../test/machine/number.cpp"
        "../../test/machine/number.hpp"
        "../../test/machine/operation.cpp"
        "../../test/math/addition.cpp"
        "../../test/math/bits.cpp"
        "../../test/math/division.cpp"
        "../../test/math/limits.cpp"
        "../../test/math/power.cpp"
        "../../test/math/safe.cpp"
        "../../test/math/sign.cpp"
        "../../test/message/address.cpp"
        "../../test/message/alert.cpp"
        "../../test/message/alert_payload.cpp"
        "../../test/message/block.cpp"
        "../../test/message/block_transactions.cpp"
        "../../test/message/compact_block.cpp"
        "../../test/message/compact_filter.cpp"
        "../../test/message/compact_filter_checkpoint.cpp"
        "../../test/message/compact_filter_headers.cpp"
        "../../test/message/fee_filter.cpp"
        "../../test/message/filter_add.cpp"
        "../../test/message/filter_clear.cpp"
        "../../test/message/filter_load.cpp"
        "../../test/message/get_address.cpp"
        "../../test/message/get_block_transactions.cpp"
        "../../test/message/get_blocks.cpp"
        "../../test/message/get_compact_filter_checkpoint.cpp"
        "../../test/message/get_compact_filter_headers.cpp"
        "../../test/message/get_compact_filters.cpp"
        "../../test/message/get_data.cpp"
        "../../test/message/get_headers.cpp"
        "../../test/message/header.cpp"
        "../../test/message/headers.cpp"
        "../../test/message/heading.cpp"
        "../../test/message/inventory.cpp"
        "../../test/message/inventory_vector.cpp"
        "../../test/message/memory_pool.cpp"
        "../../test/message/merkle_block.cpp"
        "../../test/message/network_address.cpp"
        "../../test/message/not_found.cpp"
        "../../test/message/ping.cpp"
        "../../test/message/pong.cpp"
        "../../test/message/prefilled_transaction.cpp"
        "../../test/message/reject.cpp"
        "../../test/message/send_compact.cpp"
        "../../test/message/send_headers.cpp"
        "../../test/message/transaction.cpp"
        "../../test/message/verack.cpp"
        "../../test/message/version.cpp"
        "../../test/radix/base_10.cpp"
        "../../test/radix/base_16.cpp"
        "../../test/radix/base_2048.cpp"
        "../../test/radix/base_32.cpp"
        "../../test/radix/base_58.cpp"
        "../../test/radix/base_64.cpp"
        "../../test/radix/base_85.cpp"
        "../../test/serial/deserialize.cpp"
        "../../test/serial/endian.cpp"
        "../../test/serial/serialize.cpp"
        "../../test/stream/binary.cpp"
        "../../test/stream/device.cpp"
        "../../test/stream/stream.cpp"
        "../../test/stream/streamers.cpp"
        "../../test/stream/devices/copy_sink.cpp"
        "../../test/stream/devices/copy_source.cpp"
        "../../test/stream/devices/flip_sink.cpp"
        "../../test/stream/devices/push_sink.cpp"
        "../../test/stream/streamers/bit_flipper.cpp"
        "../../test/stream/streamers/bit_reader.cpp"
        "../../test/stream/streamers/bit_writer.cpp"
        "../../test/stream/streamers/byte_flipper.cpp"
        "../../test/stream/streamers/byte_reader.cpp"
        "../../test/stream/streamers/byte_writer.cpp"
        "../../test/unicode/ascii.cpp"
        "../../test/unicode/code_points.cpp"
        "../../test/unicode/conversion.cpp"
        "../../test/unicode/normalization.cpp"
        "../../test/unicode/utf8_everywhere/utf8_environment.cpp"
        "../../test/unicode/utf8_everywhere/utf8_ifstream.cpp"
        "../../test/unicode/utf8_everywhere/utf8_istream.cpp"
        "../../test/unicode/utf8_everywhere/utf8_ofstream.cpp"
        "../../test/unicode/utf8_everywhere/utf8_ostream.cpp"
        "../../test/wallet/context.cpp"
        "../../test/wallet/message.cpp"
        "../../test/wallet/neutrino_filter.cpp"
        "../../test/wallet/addresses/bitcoin_uri.cpp"
        "../../test/wallet/addresses/checked.cpp"
        "../../test/wallet/addresses/payment_address.cpp"
        "../../test/wallet/addresses/qr_code.cpp"
        "../../test/wallet/addresses/stealth_address.cpp"
        "../../test/wallet/addresses/stealth_receiver.cpp"
        "../../test/wallet/addresses/stealth_sender.cpp"
        "../../test/wallet/addresses/tiff.cpp"
        "../../test/wallet/addresses/uri.cpp"
        "../../test/wallet/addresses/uri_reader.cpp"
        "../../test/wallet/addresses/witness_address.cpp"
        "../../test/wallet/keys/ec_point.cpp"
        "../../test/wallet/keys/ec_private.cpp"
        "../../test/wallet/keys/ec_public.cpp"
        "../../test/wallet/keys/ec_scalar.cpp"
        "../../test/wallet/keys/encrypted_keys.cpp"
        "../../test/wallet/keys/hd_private.cpp"
        "../../test/wallet/keys/hd_public.cpp"
        "../../test/wallet/keys/stealth.cpp"
        "../../test/wallet/mnemonics/electrum.cpp"
        "../../test/wallet/mnemonics/electrum.hpp"
        "../../test/wallet/mnemonics/electrum_v1.cpp"
        "../../test/wallet/mnemonics/electrum_v1.hpp"
        "../../test/wallet/mnemonics/mnemonic.cpp"
        "../../test/wallet/mnemonics/mnemonic.hpp"
        "../../test/words/dictionaries.cpp"
        "../../test/words/dictionaries.hpp"
        "../../test/words/dictionary.cpp"
        "../../test/words/dictionary.hpp"
        "../../test/words/languages.cpp"
        "../../test/words/languages.hpp"
        "../../test/words/catalogs/electrum.cpp"
        "../../test/words/catalogs/electrum.hpp"
        "../../test/words/catalogs/electrum_v1.cpp"
        "../../test/words/catalogs/electrum_v1.hpp"
        "../../test/words/catalogs/mnemonic.cpp"
        "../../test/words/catalogs/mnemonic.hpp" )

    add_test( NAME libbitcoin-system-test COMMAND libbitcoin-system-test
            --run_test=*
            --show_progress=no
            --detect_memory_leak=0
            --report_level=no
            --build_info=yes )

#     libbitcoin-system-test project specific include directories.
#------------------------------------------------------------------------------
    target_include_directories( libbitcoin-system-test PRIVATE
        "../../include" )

#     libbitcoin-system-test project specific libraries/linker flags.
#------------------------------------------------------------------------------
    target_link_libraries( libbitcoin-system-test
        ${CANONICAL_LIB_NAME}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} )

endif()

# Manage pkgconfig installation.
#------------------------------------------------------------------------------
configure_file(
  "../../libbitcoin-system.pc.in"
  "libbitcoin-system.pc" @ONLY )

install( FILES
    "${CMAKE_CURRENT_BINARY_DIR}/libbitcoin-system.pc"
    DESTINATION "${pkgconfigdir}" )

# Manage installation of docs.
#------------------------------------------------------------------------------
install( FILES
    "../../AUTHORS"
    "../../COPYING"
    "../../ChangeLog"
    "../../INSTALL"
    "../../NEWS"
    "../../README"
    DESTINATION share/doc/libbitcoin-system )

# Manage ${CANONICAL_LIB_NAME} installation.
#------------------------------------------------------------------------------
install( TARGETS ${CANONICAL_LIB_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include )

# Manage include installation.
#------------------------------------------------------------------------------
install( DIRECTORY "../../include/bitcoin"
    DESTINATION include )

