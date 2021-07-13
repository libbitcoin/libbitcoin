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
#include <bitcoin/system/chain/enums/opcode.hpp>

#include <cstdint>
#include <string>
#include <bitcoin/system/chain/enums/rule_fork.hpp>
#include <bitcoin/system/chain/script.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/radix/radix.hpp>

namespace libbitcoin {
namespace system {
namespace chain {

#define RETURN_IF_OPCODE(text, code) \
if (norm == text) { out_code = opcode::code; return true; }

#define RETURN_IF_OPCODE_OR_ALIAS(text, alias, code) \
if (norm == text || norm == alias) { out_code = opcode::code; return true; }

std::string opcode_to_string(opcode value, uint32_t active_forks)
{
    static const auto push_zero = static_cast<uint8_t>(opcode::reserved_80);

    switch (value)
    {
        // Prefer traditional aliases.
        case opcode::push_size_0:
            return "zero";
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
            return "push_" + std::to_string(static_cast<uint8_t>(value));
        case opcode::push_one_size:
            return "pushdata1";
        case opcode::push_two_size:
            return "pushdata2";
        case opcode::push_four_size:
            return "pushdata4";
        case opcode::push_negative_1:
            return "-1";
        case opcode::reserved_80:
            return "reserved";          // deprecated, use hex
        case opcode::push_positive_1:
        case opcode::push_positive_2:
        case opcode::push_positive_3:
        case opcode::push_positive_4:
        case opcode::push_positive_5:
        case opcode::push_positive_6:
        case opcode::push_positive_7:
        case opcode::push_positive_8:
        case opcode::push_positive_9:
        case opcode::push_positive_10:
        case opcode::push_positive_11:
        case opcode::push_positive_12:
        case opcode::push_positive_13:
        case opcode::push_positive_14:
        case opcode::push_positive_15:
        case opcode::push_positive_16:
            return std::to_string(static_cast<uint8_t>(value) - push_zero);
        case opcode::nop:
            return "nop";
        case opcode::reserved_98:
            return "ver";               // deprecated, use hex
        case opcode::if_:
            return "if";
        case opcode::notif:
            return "notif";
        case opcode::disabled_verif:
            return "verif";             // deprecated, use hex
        case opcode::disabled_vernotif:
            return "vernotif";          // deprecated, use hex
        case opcode::else_:
            return "else";
        case opcode::endif:
            return "endif";
        case opcode::verify:
            return "verify";
        case opcode::return_:
            return "return";
        case opcode::toaltstack:
            return "toaltstack";
        case opcode::fromaltstack:
            return "fromaltstack";
        case opcode::drop2:
            return "2drop";
        case opcode::dup2:
            return "2dup";
        case opcode::dup3:
            return "3dup";
        case opcode::over2:
            return "2over";
        case opcode::rot2:
            return "2rot";
        case opcode::swap2:
            return "2swap";
        case opcode::ifdup:
            return "ifdup";
        case opcode::depth:
            return "depth";
        case opcode::drop:
            return "drop";
        case opcode::dup:
            return "dup";
        case opcode::nip:
            return "nip";
        case opcode::over:
            return "over";
        case opcode::pick:
            return "pick";
        case opcode::roll:
            return "roll";
        case opcode::rot:
            return "rot";
        case opcode::swap:
            return "swap";
        case opcode::tuck:
            return "tuck";
        case opcode::disabled_cat:
            return "cat";
        case opcode::disabled_substr:
            return "substr";
        case opcode::disabled_left:
            return "left";
        case opcode::disabled_right:
            return "right";
        case opcode::size:
            return "size";
        case opcode::disabled_invert:
            return "invert";
        case opcode::disabled_and:
            return "and";
        case opcode::disabled_or:
            return "or";
        case opcode::disabled_xor:
            return "xor";
        case opcode::equal:
            return "equal";
        case opcode::equalverify:
            return "equalverify";
        case opcode::reserved_137:
            return "reserved_137";      // deprecated, use hex
        case opcode::reserved_138:
            return "reserved_138";      // deprecated, use hex
        case opcode::add1:
            return "1add";
        case opcode::sub1:
            return "1sub";
        case opcode::disabled_mul2:
            return "2mul";
        case opcode::disabled_div2:
            return "2div";
        case opcode::negate:
            return "negate";
        case opcode::abs:
            return "abs";
        case opcode::not_:
            return "not";
        case opcode::nonzero:
            return "nonzero";
        case opcode::add:
            return "add";
        case opcode::sub:
            return "sub";
        case opcode::disabled_mul:
            return "mul";
        case opcode::disabled_div:
            return "div";
        case opcode::disabled_mod:
            return "mod";
        case opcode::disabled_lshift:
            return "lshift";
        case opcode::disabled_rshift:
            return "rshift";
        case opcode::booland:
            return "booland";
        case opcode::boolor:
            return "boolor";
        case opcode::numequal:
            return "numequal";
        case opcode::numequalverify:
            return "numequalverify";
        case opcode::numnotequal:
            return "numnotequal";
        case opcode::lessthan:
            return "lessthan";
        case opcode::greaterthan:
            return "greaterthan";
        case opcode::lessthanorequal:
            return "lessthanorequal";
        case opcode::greaterthanorequal:
            return "greaterthanorequal";
        case opcode::min:
            return "min";
        case opcode::max:
            return "max";
        case opcode::within:
            return "within";
        case opcode::ripemd160:
            return "ripemd160";
        case opcode::sha1:
            return "sha1";
        case opcode::sha256:
            return "sha256";
        case opcode::hash160:
            return "hash160";
        case opcode::hash256:
            return "hash256";
        case opcode::codeseparator:
            return "codeseparator";
        case opcode::checksig:
            return "checksig";
        case opcode::checksigverify:
            return "checksigverify";
        case opcode::checkmultisig:
            return "checkmultisig";
        case opcode::checkmultisigverify:
            return "checkmultisigverify";
        case opcode::nop1:
            return "nop1";
        case opcode::checklocktimeverify:
            return script::is_enabled(active_forks, rule_fork::bip65_rule) ?
                "checklocktimeverify" : "nop2";
        case opcode::checksequenceverify:
            return script::is_enabled(active_forks, rule_fork::bip112_rule) ?
                "checksequenceverify" : "nop3";
        case opcode::nop4:
            return "nop4";
        case opcode::nop5:
            return "nop5";
        case opcode::nop6:
            return "nop6";
        case opcode::nop7:
            return "nop7";
        case opcode::nop8:
            return "nop8";
        case opcode::nop9:
            return "nop9";
        case opcode::nop10:
            return "nop10";
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
            return opcode_to_hexadecimal(value);
    }
}

// This converts only names, not any data for push codes.
bool opcode_from_string(opcode& out_code, const std::string& value)
{
    // Normalize to ASCII lower case.
    const auto norm = ascii_to_lower(value);

    RETURN_IF_OPCODE("zero", push_size_0);
    RETURN_IF_OPCODE("push_0", push_size_0);
    RETURN_IF_OPCODE("push_1", push_size_1);
    RETURN_IF_OPCODE("push_2", push_size_2);
    RETURN_IF_OPCODE("push_3", push_size_3);
    RETURN_IF_OPCODE("push_4", push_size_4);
    RETURN_IF_OPCODE("push_5", push_size_5);
    RETURN_IF_OPCODE("push_6", push_size_6);
    RETURN_IF_OPCODE("push_7", push_size_7);
    RETURN_IF_OPCODE("push_8", push_size_8);
    RETURN_IF_OPCODE("push_9", push_size_9);
    RETURN_IF_OPCODE("push_10", push_size_10);
    RETURN_IF_OPCODE("push_11", push_size_11);
    RETURN_IF_OPCODE("push_12", push_size_12);
    RETURN_IF_OPCODE("push_13", push_size_13);
    RETURN_IF_OPCODE("push_14", push_size_14);
    RETURN_IF_OPCODE("push_15", push_size_15);
    RETURN_IF_OPCODE("push_16", push_size_16);
    RETURN_IF_OPCODE("push_17", push_size_17);
    RETURN_IF_OPCODE("push_18", push_size_18);
    RETURN_IF_OPCODE("push_19", push_size_19);
    RETURN_IF_OPCODE("push_20", push_size_20);
    RETURN_IF_OPCODE("push_21", push_size_21);
    RETURN_IF_OPCODE("push_22", push_size_22);
    RETURN_IF_OPCODE("push_23", push_size_23);
    RETURN_IF_OPCODE("push_24", push_size_24);
    RETURN_IF_OPCODE("push_25", push_size_25);
    RETURN_IF_OPCODE("push_26", push_size_26);
    RETURN_IF_OPCODE("push_27", push_size_27);
    RETURN_IF_OPCODE("push_28", push_size_28);
    RETURN_IF_OPCODE("push_29", push_size_29);
    RETURN_IF_OPCODE("push_30", push_size_30);
    RETURN_IF_OPCODE("push_31", push_size_31);
    RETURN_IF_OPCODE("push_32", push_size_32);
    RETURN_IF_OPCODE("push_33", push_size_33);
    RETURN_IF_OPCODE("push_34", push_size_34);
    RETURN_IF_OPCODE("push_35", push_size_35);
    RETURN_IF_OPCODE("push_36", push_size_36);
    RETURN_IF_OPCODE("push_37", push_size_37);
    RETURN_IF_OPCODE("push_38", push_size_38);
    RETURN_IF_OPCODE("push_39", push_size_39);
    RETURN_IF_OPCODE("push_40", push_size_40);
    RETURN_IF_OPCODE("push_41", push_size_41);
    RETURN_IF_OPCODE("push_42", push_size_42);
    RETURN_IF_OPCODE("push_43", push_size_43);
    RETURN_IF_OPCODE("push_44", push_size_44);
    RETURN_IF_OPCODE("push_45", push_size_45);
    RETURN_IF_OPCODE("push_46", push_size_46);
    RETURN_IF_OPCODE("push_47", push_size_47);
    RETURN_IF_OPCODE("push_48", push_size_48);
    RETURN_IF_OPCODE("push_49", push_size_49);
    RETURN_IF_OPCODE("push_50", push_size_50);
    RETURN_IF_OPCODE("push_51", push_size_51);
    RETURN_IF_OPCODE("push_52", push_size_52);
    RETURN_IF_OPCODE("push_53", push_size_53);
    RETURN_IF_OPCODE("push_54", push_size_54);
    RETURN_IF_OPCODE("push_55", push_size_55);
    RETURN_IF_OPCODE("push_56", push_size_56);
    RETURN_IF_OPCODE("push_57", push_size_57);
    RETURN_IF_OPCODE("push_58", push_size_58);
    RETURN_IF_OPCODE("push_59", push_size_59);
    RETURN_IF_OPCODE("push_60", push_size_60);
    RETURN_IF_OPCODE("push_61", push_size_61);
    RETURN_IF_OPCODE("push_62", push_size_62);
    RETURN_IF_OPCODE("push_63", push_size_63);
    RETURN_IF_OPCODE("push_64", push_size_64);
    RETURN_IF_OPCODE("push_65", push_size_65);
    RETURN_IF_OPCODE("push_66", push_size_66);
    RETURN_IF_OPCODE("push_67", push_size_67);
    RETURN_IF_OPCODE("push_68", push_size_68);
    RETURN_IF_OPCODE("push_69", push_size_69);
    RETURN_IF_OPCODE("push_70", push_size_70);
    RETURN_IF_OPCODE("push_71", push_size_71);
    RETURN_IF_OPCODE("push_72", push_size_72);
    RETURN_IF_OPCODE("push_73", push_size_73);
    RETURN_IF_OPCODE("push_74", push_size_74);
    RETURN_IF_OPCODE("push_75", push_size_75);
    RETURN_IF_OPCODE_OR_ALIAS("push_one", "pushdata1", push_one_size);
    RETURN_IF_OPCODE_OR_ALIAS("push_two", "pushdata2", push_two_size);
    RETURN_IF_OPCODE_OR_ALIAS("push_four", "pushdata4", push_four_size);
    RETURN_IF_OPCODE("-1", push_negative_1);
    RETURN_IF_OPCODE_OR_ALIAS("reserved_80", "reserved", reserved_80);
    RETURN_IF_OPCODE("0", push_size_0);
    RETURN_IF_OPCODE("1", push_positive_1);
    RETURN_IF_OPCODE("2", push_positive_2);
    RETURN_IF_OPCODE("3", push_positive_3);
    RETURN_IF_OPCODE("4", push_positive_4);
    RETURN_IF_OPCODE("5", push_positive_5);
    RETURN_IF_OPCODE("6", push_positive_6);
    RETURN_IF_OPCODE("7", push_positive_7);
    RETURN_IF_OPCODE("8", push_positive_8);
    RETURN_IF_OPCODE("9", push_positive_9);
    RETURN_IF_OPCODE("10", push_positive_10);
    RETURN_IF_OPCODE("11", push_positive_11);
    RETURN_IF_OPCODE("12", push_positive_12);
    RETURN_IF_OPCODE("13", push_positive_13);
    RETURN_IF_OPCODE("14", push_positive_14);
    RETURN_IF_OPCODE("15", push_positive_15);
    RETURN_IF_OPCODE("16", push_positive_16);
    RETURN_IF_OPCODE("nop", nop);
    RETURN_IF_OPCODE_OR_ALIAS("reserved_98", "ver", reserved_98);
    RETURN_IF_OPCODE("if", if_);
    RETURN_IF_OPCODE("notif", notif);
    RETURN_IF_OPCODE_OR_ALIAS("disabled_verif", "verif", disabled_verif);
    RETURN_IF_OPCODE_OR_ALIAS("disabled_vernotif", "vernotif", disabled_vernotif);
    RETURN_IF_OPCODE("else", else_);
    RETURN_IF_OPCODE("endif", endif);
    RETURN_IF_OPCODE("verify", verify);
    RETURN_IF_OPCODE("return", return_);
    RETURN_IF_OPCODE("toaltstack", toaltstack);
    RETURN_IF_OPCODE("fromaltstack", fromaltstack);
    RETURN_IF_OPCODE_OR_ALIAS("drop2", "2drop", drop2);
    RETURN_IF_OPCODE_OR_ALIAS("dup2", "2dup", dup2);
    RETURN_IF_OPCODE_OR_ALIAS("dup3", "3dup", dup3);
    RETURN_IF_OPCODE_OR_ALIAS("over2", "2over", over2);
    RETURN_IF_OPCODE_OR_ALIAS("rot2", "2rot", rot2);
    RETURN_IF_OPCODE_OR_ALIAS("swap2", "2swap", swap2);
    RETURN_IF_OPCODE("ifdup", ifdup);
    RETURN_IF_OPCODE("depth", depth);
    RETURN_IF_OPCODE("drop", drop);
    RETURN_IF_OPCODE("dup", dup);
    RETURN_IF_OPCODE("nip", nip);
    RETURN_IF_OPCODE("over", over);
    RETURN_IF_OPCODE("pick", pick);
    RETURN_IF_OPCODE("roll", roll);
    RETURN_IF_OPCODE("rot", rot);
    RETURN_IF_OPCODE("swap", swap);
    RETURN_IF_OPCODE("tuck", tuck);
    RETURN_IF_OPCODE("cat", disabled_cat);
    RETURN_IF_OPCODE("substr", disabled_substr);
    RETURN_IF_OPCODE("left", disabled_left);
    RETURN_IF_OPCODE("right", disabled_right);
    RETURN_IF_OPCODE("size", size);
    RETURN_IF_OPCODE("invert", disabled_invert);
    RETURN_IF_OPCODE("and", disabled_and);
    RETURN_IF_OPCODE("or", disabled_or);
    RETURN_IF_OPCODE("xor", disabled_xor);
    RETURN_IF_OPCODE("equal", equal);
    RETURN_IF_OPCODE("equalverify", equalverify);
    RETURN_IF_OPCODE_OR_ALIAS("reserved_137", "reserved1", reserved_137);
    RETURN_IF_OPCODE_OR_ALIAS("reserved_138", "reserved2", reserved_138);
    RETURN_IF_OPCODE_OR_ALIAS("add1", "1add", add1);
    RETURN_IF_OPCODE_OR_ALIAS("sub1", "1sub", sub1);
    RETURN_IF_OPCODE_OR_ALIAS("mul2", "2mul", disabled_mul2);
    RETURN_IF_OPCODE_OR_ALIAS("div2", "2div", disabled_div2);
    RETURN_IF_OPCODE("negate", negate);
    RETURN_IF_OPCODE("abs", abs);
    RETURN_IF_OPCODE("not", not_);
    RETURN_IF_OPCODE_OR_ALIAS("nonzero", "0notequal", nonzero);
    RETURN_IF_OPCODE("add", add);
    RETURN_IF_OPCODE("sub", sub);
    RETURN_IF_OPCODE("mul", disabled_mul);
    RETURN_IF_OPCODE("div", disabled_div);
    RETURN_IF_OPCODE("mod", disabled_mod);
    RETURN_IF_OPCODE("lshift", disabled_lshift);
    RETURN_IF_OPCODE("rshift", disabled_rshift);
    RETURN_IF_OPCODE("booland", booland);
    RETURN_IF_OPCODE("boolor", boolor);
    RETURN_IF_OPCODE("numequal", numequal);
    RETURN_IF_OPCODE("numequalverify", numequalverify);
    RETURN_IF_OPCODE("numnotequal", numnotequal);
    RETURN_IF_OPCODE("lessthan", lessthan);
    RETURN_IF_OPCODE("greaterthan", greaterthan);
    RETURN_IF_OPCODE("lessthanorequal", lessthanorequal);
    RETURN_IF_OPCODE("greaterthanorequal", greaterthanorequal);
    RETURN_IF_OPCODE("min", min);
    RETURN_IF_OPCODE("max", max);
    RETURN_IF_OPCODE("within", within);
    RETURN_IF_OPCODE("ripemd160", ripemd160);
    RETURN_IF_OPCODE("sha1", sha1);
    RETURN_IF_OPCODE("sha256", sha256);
    RETURN_IF_OPCODE("hash160", hash160);
    RETURN_IF_OPCODE("hash256", hash256);
    RETURN_IF_OPCODE("codeseparator", codeseparator);
    RETURN_IF_OPCODE("checksig", checksig);
    RETURN_IF_OPCODE("checksigverify", checksigverify);
    RETURN_IF_OPCODE("checkmultisig", checkmultisig);
    RETURN_IF_OPCODE("checkmultisigverify", checkmultisigverify);
    RETURN_IF_OPCODE("nop1", nop1);
    RETURN_IF_OPCODE_OR_ALIAS("checklocktimeverify", "nop2", checklocktimeverify);
    RETURN_IF_OPCODE_OR_ALIAS("checksequenceverify", "nop3", checksequenceverify);
    RETURN_IF_OPCODE("nop4", nop4);
    RETURN_IF_OPCODE("nop5", nop5);
    RETURN_IF_OPCODE("nop6", nop6);
    RETURN_IF_OPCODE("nop7", nop7);
    RETURN_IF_OPCODE("nop8", nop8);
    RETURN_IF_OPCODE("nop9", nop9);
    RETURN_IF_OPCODE("nop10", nop10);
    RETURN_IF_OPCODE("reserved_186", reserved_186);
    RETURN_IF_OPCODE("reserved_187", reserved_187);
    RETURN_IF_OPCODE("reserved_188", reserved_188);
    RETURN_IF_OPCODE("reserved_189", reserved_189);
    RETURN_IF_OPCODE("reserved_190", reserved_190);
    RETURN_IF_OPCODE("reserved_191", reserved_191);
    RETURN_IF_OPCODE("reserved_192", reserved_192);
    RETURN_IF_OPCODE("reserved_193", reserved_193);
    RETURN_IF_OPCODE("reserved_194", reserved_194);
    RETURN_IF_OPCODE("reserved_195", reserved_195);
    RETURN_IF_OPCODE("reserved_196", reserved_196);
    RETURN_IF_OPCODE("reserved_197", reserved_197);
    RETURN_IF_OPCODE("reserved_198", reserved_198);
    RETURN_IF_OPCODE("reserved_199", reserved_199);
    RETURN_IF_OPCODE("reserved_200", reserved_200);
    RETURN_IF_OPCODE("reserved_201", reserved_201);
    RETURN_IF_OPCODE("reserved_202", reserved_202);
    RETURN_IF_OPCODE("reserved_203", reserved_203);
    RETURN_IF_OPCODE("reserved_204", reserved_204);
    RETURN_IF_OPCODE("reserved_205", reserved_205);
    RETURN_IF_OPCODE("reserved_206", reserved_206);
    RETURN_IF_OPCODE("reserved_207", reserved_207);
    RETURN_IF_OPCODE("reserved_208", reserved_208);
    RETURN_IF_OPCODE("reserved_209", reserved_209);
    RETURN_IF_OPCODE("reserved_210", reserved_210);
    RETURN_IF_OPCODE("reserved_211", reserved_211);
    RETURN_IF_OPCODE("reserved_212", reserved_212);
    RETURN_IF_OPCODE("reserved_213", reserved_213);
    RETURN_IF_OPCODE("reserved_214", reserved_214);
    RETURN_IF_OPCODE("reserved_215", reserved_215);
    RETURN_IF_OPCODE("reserved_216", reserved_216);
    RETURN_IF_OPCODE("reserved_217", reserved_217);
    RETURN_IF_OPCODE("reserved_218", reserved_218);
    RETURN_IF_OPCODE("reserved_219", reserved_219);
    RETURN_IF_OPCODE("reserved_220", reserved_220);
    RETURN_IF_OPCODE("reserved_221", reserved_221);
    RETURN_IF_OPCODE("reserved_222", reserved_222);
    RETURN_IF_OPCODE("reserved_223", reserved_223);
    RETURN_IF_OPCODE("reserved_224", reserved_224);
    RETURN_IF_OPCODE("reserved_225", reserved_225);
    RETURN_IF_OPCODE("reserved_226", reserved_226);
    RETURN_IF_OPCODE("reserved_227", reserved_227);
    RETURN_IF_OPCODE("reserved_228", reserved_228);
    RETURN_IF_OPCODE("reserved_229", reserved_229);
    RETURN_IF_OPCODE("reserved_230", reserved_230);
    RETURN_IF_OPCODE("reserved_231", reserved_231);
    RETURN_IF_OPCODE("reserved_232", reserved_232);
    RETURN_IF_OPCODE("reserved_233", reserved_233);
    RETURN_IF_OPCODE("reserved_234", reserved_234);
    RETURN_IF_OPCODE("reserved_235", reserved_235);
    RETURN_IF_OPCODE("reserved_236", reserved_236);
    RETURN_IF_OPCODE("reserved_237", reserved_237);
    RETURN_IF_OPCODE("reserved_238", reserved_238);
    RETURN_IF_OPCODE("reserved_239", reserved_239);
    RETURN_IF_OPCODE("reserved_240", reserved_240);
    RETURN_IF_OPCODE("reserved_241", reserved_241);
    RETURN_IF_OPCODE("reserved_242", reserved_242);
    RETURN_IF_OPCODE("reserved_243", reserved_243);
    RETURN_IF_OPCODE("reserved_244", reserved_244);
    RETURN_IF_OPCODE("reserved_245", reserved_245);
    RETURN_IF_OPCODE("reserved_246", reserved_246);
    RETURN_IF_OPCODE("reserved_247", reserved_247);
    RETURN_IF_OPCODE("reserved_248", reserved_248);
    RETURN_IF_OPCODE("reserved_249", reserved_249);
    RETURN_IF_OPCODE("reserved_250", reserved_250);
    RETURN_IF_OPCODE("reserved_251", reserved_251);
    RETURN_IF_OPCODE("reserved_252", reserved_252);
    RETURN_IF_OPCODE("reserved_253", reserved_253);
    RETURN_IF_OPCODE("reserved_254", reserved_254);
    RETURN_IF_OPCODE("reserved_255", reserved_255);

    // Any hexadecimal byte will parse (hex prefix not lowered).
    return opcode_from_hexadecimal(out_code, value);
}

std::string opcode_to_hexadecimal(opcode code)
{
    return "0x" + encode_base16({ { static_cast<uint8_t>(code) } });
}

bool opcode_from_hexadecimal(opcode& out_code, const std::string& value)
{
    if (value.size() != 4 || value[0] != '0' || value[1] != 'x')
        return false;

    data_chunk out;
    if (!decode_base16(out, { std::next(value.begin(), 2), value.end() }))
        return false;

    out_code = static_cast<opcode>(out.front());
    return true;
}

} // namespace chain
} // namespace system
} // namespace libbitcoin
