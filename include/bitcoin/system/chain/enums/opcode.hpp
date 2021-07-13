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
#ifndef LIBBITCOIN_SYSTEM_CHAIN_ENUMS_OPCODE_HPP
#define LIBBITCOIN_SYSTEM_CHAIN_ENUMS_OPCODE_HPP

#include <cstdint>
#include <string>
#include <bitcoin/system/define.hpp>

namespace libbitcoin {
namespace system {
namespace chain {

enum class opcode : uint8_t
{
    //-------------------------------------------------------------------------
    // is_relaxed_push, is_push (excluding reserved_80)

    push_size_0 = 0,        // is_version (pushes [] to the stack, not 0)
    push_size_1 = 1,
    push_size_2 = 2,
    push_size_3 = 3,
    push_size_4 = 4,
    push_size_5 = 5,
    push_size_6 = 6,
    push_size_7 = 7,
    push_size_8 = 8,
    push_size_9 = 9,
    push_size_10 = 10,
    push_size_11 = 11,
    push_size_12 = 12,
    push_size_13 = 13,
    push_size_14 = 14,
    push_size_15 = 15,
    push_size_16 = 16,
    push_size_17 = 17,
    push_size_18 = 18,
    push_size_19 = 19,
    push_size_20 = 20,
    push_size_21 = 21,
    push_size_22 = 22,
    push_size_23 = 23,
    push_size_24 = 24,
    push_size_25 = 25,
    push_size_26 = 26,
    push_size_27 = 27,
    push_size_28 = 28,
    push_size_29 = 29,
    push_size_30 = 30,
    push_size_31 = 31,
    push_size_32 = 32,
    push_size_33 = 33,
    push_size_34 = 34,
    push_size_35 = 35,
    push_size_36 = 36,
    push_size_37 = 37,
    push_size_38 = 38,
    push_size_39 = 39,
    push_size_40 = 40,
    push_size_41 = 41,
    push_size_42 = 42,
    push_size_43 = 43,
    push_size_44 = 44,
    push_size_45 = 45,
    push_size_46 = 46,
    push_size_47 = 47,
    push_size_48 = 48,
    push_size_49 = 49,
    push_size_50 = 50,
    push_size_51 = 51,
    push_size_52 = 52,
    push_size_53 = 53,
    push_size_54 = 54,
    push_size_55 = 55,
    push_size_56 = 56,
    push_size_57 = 57,
    push_size_58 = 58,
    push_size_59 = 59,
    push_size_60 = 60,
    push_size_61 = 61,
    push_size_62 = 62,
    push_size_63 = 63,
    push_size_64 = 64,
    push_size_65 = 65,
    push_size_66 = 66,
    push_size_67 = 67,
    push_size_68 = 68,
    push_size_69 = 69,
    push_size_70 = 70,
    push_size_71 = 71,
    push_size_72 = 72,
    push_size_73 = 73,
    push_size_74 = 74,
    push_size_75 = 75,
    push_one_size = 76,
    push_two_size = 77,
    push_four_size = 78,
    push_negative_1 = 79,   // is_numeric
    reserved_80 = 80,       // [reserved]
    push_positive_1 = 81,   // is_numeric, is_positive, is_version
    push_positive_2 = 82,   // is_numeric, is_positive, is_version
    push_positive_3 = 83,   // is_numeric, is_positive, is_version
    push_positive_4 = 84,   // is_numeric, is_positive, is_version
    push_positive_5 = 85,   // is_numeric, is_positive, is_version
    push_positive_6 = 86,   // is_numeric, is_positive, is_version
    push_positive_7 = 87,   // is_numeric, is_positive, is_version
    push_positive_8 = 88,   // is_numeric, is_positive, is_version
    push_positive_9 = 89,   // is_numeric, is_positive, is_version
    push_positive_10 = 90,  // is_numeric, is_positive, is_version
    push_positive_11 = 91,  // is_numeric, is_positive, is_version
    push_positive_12 = 92,  // is_numeric, is_positive, is_version
    push_positive_13 = 93,  // is_numeric, is_positive, is_version
    push_positive_14 = 94,  // is_numeric, is_positive, is_version
    push_positive_15 = 95,  // is_numeric, is_positive, is_version
    push_positive_16 = 96,  // is_numeric, is_positive, is_version

    //-------------------------------------------------------------------------
    // is_counted

    nop = 97,
    reserved_98 = 98,       // [ver]
    if_ = 99,               // is_conditional
    notif = 100,            // is_conditional
    disabled_verif = 101,   // is_disabled
    disabled_vernotif = 102,// is_disabled
    else_ = 103,            // is_conditional
    endif = 104,            // is_conditional
    verify = 105,
    return_ = 106,
    toaltstack = 107,
    fromaltstack = 108,
    drop2 = 109,
    dup2 = 110,
    dup3 = 111,
    over2 = 112,
    rot2 = 113,
    swap2 = 114,
    ifdup = 115,
    depth = 116,
    drop = 117,
    dup = 118,
    nip = 119,
    over = 120,
    pick = 121,
    roll = 122,
    rot = 123,
    swap = 124,
    tuck = 125,
    disabled_cat = 126,     // is_disabled
    disabled_substr = 127,  // is_disabled
    disabled_left = 128,    // is_disabled
    disabled_right = 129,   // is_disabled
    size = 130,
    disabled_invert = 131,  // is_disabled
    disabled_and = 132,     // is_disabled
    disabled_or = 133,      // is_disabled
    disabled_xor = 134,     // is_disabled
    equal = 135,
    equalverify = 136,
    reserved_137 = 137,     // [reserved1]
    reserved_138 = 138,     // [reserved2]
    add1 = 139,
    sub1 = 140,
    disabled_mul2 = 141,    // is_disabled
    disabled_div2 = 142,    // is_disabled
    negate = 143,
    abs = 144,
    not_ = 145,
    nonzero = 146,
    add = 147,
    sub = 148,
    disabled_mul = 149,     // is_disabled
    disabled_div = 150,     // is_disabled
    disabled_mod = 151,     // is_disabled
    disabled_lshift = 152,  // is_disabled
    disabled_rshift = 153,  // is_disabled
    booland = 154,
    boolor = 155,
    numequal = 156,
    numequalverify = 157,
    numnotequal = 158,
    lessthan = 159,
    greaterthan = 160,
    lessthanorequal = 161,
    greaterthanorequal = 162,
    min = 163,
    max = 164,
    within = 165,
    ripemd160 = 166,
    sha1 = 167,
    sha256 = 168,
    hash160 = 169,
    hash256 = 170,
    codeseparator = 171,
    checksig = 172,
    checksigverify = 173,
    checkmultisig = 174,
    checkmultisigverify = 175,
    nop1 = 176,
    nop2 = 177,
    checklocktimeverify = nop2,
    nop3 = 178,
    checksequenceverify = nop3,
    nop4 = 179,
    nop5 = 180,
    nop6 = 181,
    nop7 = 182,
    nop8 = 183,
    nop9 = 184,
    nop10 = 185,
    reserved_186 = 186,
    reserved_187 = 187,
    reserved_188 = 188,
    reserved_189 = 189,
    reserved_190 = 190,
    reserved_191 = 191,
    reserved_192 = 192,
    reserved_193 = 193,
    reserved_194 = 194,
    reserved_195 = 195,
    reserved_196 = 196,
    reserved_197 = 197,
    reserved_198 = 198,
    reserved_199 = 199,
    reserved_200 = 200,
    reserved_201 = 201,
    reserved_202 = 202,
    reserved_203 = 203,
    reserved_204 = 204,
    reserved_205 = 205,
    reserved_206 = 206,
    reserved_207 = 207,
    reserved_208 = 208,
    reserved_209 = 209,
    reserved_210 = 210,
    reserved_211 = 211,
    reserved_212 = 212,
    reserved_213 = 213,
    reserved_214 = 214,
    reserved_215 = 215,
    reserved_216 = 216,
    reserved_217 = 217,
    reserved_218 = 218,
    reserved_219 = 219,
    reserved_220 = 220,
    reserved_221 = 221,
    reserved_222 = 222,
    reserved_223 = 223,
    reserved_224 = 224,
    reserved_225 = 225,
    reserved_226 = 226,
    reserved_227 = 227,
    reserved_228 = 228,
    reserved_229 = 229,
    reserved_230 = 230,
    reserved_231 = 231,
    reserved_232 = 232,
    reserved_233 = 233,
    reserved_234 = 234,
    reserved_235 = 235,
    reserved_236 = 236,
    reserved_237 = 237,
    reserved_238 = 238,
    reserved_239 = 239,
    reserved_240 = 240,
    reserved_241 = 241,
    reserved_242 = 242,
    reserved_243 = 243,
    reserved_244 = 244,
    reserved_245 = 245,
    reserved_246 = 246,
    reserved_247 = 247,
    reserved_248 = 248,
    reserved_249 = 249,
    reserved_250 = 250,
    reserved_251 = 251,
    reserved_252 = 252,
    reserved_253 = 253,
    reserved_254 = 254,
    reserved_255 = 255
};

/// Convert the opcode to a mnemonic string.
BC_API std::string opcode_to_string(opcode value, uint32_t active_forks);

/// Convert a string to an opcode.
BC_API bool opcode_from_string(opcode& out_code, const std::string& value);

/// Convert any opcode to a string hexadecimal representation.
BC_API std::string opcode_to_hexadecimal(opcode code);

/// Convert any hexadecimal byte to an opcode.
BC_API bool opcode_from_hexadecimal(opcode& out_code,
    const std::string& value);

} // namespace chain
} // namespace system
} // namespace libbitcoin

#endif
