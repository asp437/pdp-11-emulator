//
// Created by Aleksandr Parfenov on 01.10.16.
//

#include "CPU.h"

#include <iomanip>

const uint16 SINGLE_OPERAND_INSTRUCTION_MASK = (const uint16) 0177700;
const uint16 DOUBLE_OPERAND_INSTRUCTION_MASK = (const uint16) 0170000;
const uint16 REGISTER_ONLY_INSTRUCTION_MASK = (const uint16) 0177770;
const uint16 REGISTER_OPERAND_INSTRUCTION_MASK = (const uint16) 0177000;
const uint16 NO_OPERANDS_INSTRUCTION_MASK = (const uint16) 0177777;
const uint16 CONDITION_CODE_INSTRUCTION_MASK = (const uint16) 0177740;
const uint16 BRANCHING_OFFSET_INSTRUCTION_MASK = (const uint16) 0177400;
const uint16 BRANCHING_OFFSET_MASK = (const uint16) 0000377;

CPU::CPU(Unibus *unibus) : _unibus(unibus) {
  // TODO: Initialize PC and PSW
  // Single Operand Instructions
  register_instruction("CLR", SINGLE_OPERAND_INSTRUCTION_MASK, 0005000, &CPU::opcode_clr);
  register_instruction("CLRB", SINGLE_OPERAND_INSTRUCTION_MASK, 0105000, &CPU::opcode_clrb);
  register_instruction("COM", SINGLE_OPERAND_INSTRUCTION_MASK, 0005100, &CPU::opcode_com);
  register_instruction("COMB", SINGLE_OPERAND_INSTRUCTION_MASK, 0105100, &CPU::opcode_comb);
  register_instruction("INC", SINGLE_OPERAND_INSTRUCTION_MASK, 0005200, &CPU::opcode_inc);
  register_instruction("INCB", SINGLE_OPERAND_INSTRUCTION_MASK, 0105200, &CPU::opcode_incb);
  register_instruction("DEC", SINGLE_OPERAND_INSTRUCTION_MASK, 0005300, &CPU::opcode_dec);
  register_instruction("DECB", SINGLE_OPERAND_INSTRUCTION_MASK, 0105300, &CPU::opcode_decb);
  register_instruction("NEG", SINGLE_OPERAND_INSTRUCTION_MASK, 0005400, &CPU::opcode_neg);
  register_instruction("NEGB", SINGLE_OPERAND_INSTRUCTION_MASK, 0105400, &CPU::opcode_negb);
  register_instruction("TST", SINGLE_OPERAND_INSTRUCTION_MASK, 0005700, &CPU::opcode_tst);
  register_instruction("TSTB", SINGLE_OPERAND_INSTRUCTION_MASK, 0105700, &CPU::opcode_tstb);
  register_instruction("ASR", SINGLE_OPERAND_INSTRUCTION_MASK, 0006200, &CPU::opcode_asr);
  register_instruction("ASRB", SINGLE_OPERAND_INSTRUCTION_MASK, 0106200, &CPU::opcode_asrb);
  register_instruction("ASL", SINGLE_OPERAND_INSTRUCTION_MASK, 0006300, &CPU::opcode_asl);
  register_instruction("ASLB", SINGLE_OPERAND_INSTRUCTION_MASK, 0106300, &CPU::opcode_aslb);
  register_instruction("ROR", SINGLE_OPERAND_INSTRUCTION_MASK, 0006000, &CPU::opcode_ror);
  register_instruction("RORB", SINGLE_OPERAND_INSTRUCTION_MASK, 0106000, &CPU::opcode_rorb);
  register_instruction("ROL", SINGLE_OPERAND_INSTRUCTION_MASK, 0006100, &CPU::opcode_rol);
  register_instruction("ROLB", SINGLE_OPERAND_INSTRUCTION_MASK, 0106100, &CPU::opcode_rolb);
  register_instruction("SWAB", SINGLE_OPERAND_INSTRUCTION_MASK, 0000300, &CPU::opcode_swab);
  register_instruction("ADC", SINGLE_OPERAND_INSTRUCTION_MASK, 0005500, &CPU::opcode_adc);
  register_instruction("ADCB", SINGLE_OPERAND_INSTRUCTION_MASK, 0105500, &CPU::opcode_adcb);
  register_instruction("SBC", SINGLE_OPERAND_INSTRUCTION_MASK, 0005600, &CPU::opcode_sbc);
  register_instruction("SBCB", SINGLE_OPERAND_INSTRUCTION_MASK, 0105600, &CPU::opcode_sbcb);
  register_instruction("SXT", SINGLE_OPERAND_INSTRUCTION_MASK, 0006700, &CPU::opcode_sxt);

  // Double Operand Instructions
  register_instruction("MOV", DOUBLE_OPERAND_INSTRUCTION_MASK, 0010000, &CPU::opcode_mov);
  register_instruction("MOVB", DOUBLE_OPERAND_INSTRUCTION_MASK, 0110000, &CPU::opcode_movb);
  register_instruction("CMP", DOUBLE_OPERAND_INSTRUCTION_MASK, 0020000, &CPU::opcode_cmp);
  register_instruction("CMPB", DOUBLE_OPERAND_INSTRUCTION_MASK, 0120000, &CPU::opcode_cmpb);
  register_instruction("ADD", DOUBLE_OPERAND_INSTRUCTION_MASK, 0060000, &CPU::opcode_add);
  register_instruction("SUB", DOUBLE_OPERAND_INSTRUCTION_MASK, 0160000, &CPU::opcode_sub);
  register_instruction("BIT", DOUBLE_OPERAND_INSTRUCTION_MASK, 0030000, &CPU::opcode_bit);
  register_instruction("BITB", DOUBLE_OPERAND_INSTRUCTION_MASK, 0130000, &CPU::opcode_bitb);
  register_instruction("BIC", DOUBLE_OPERAND_INSTRUCTION_MASK, 0040000, &CPU::opcode_bic);
  register_instruction("BICB", DOUBLE_OPERAND_INSTRUCTION_MASK, 0140000, &CPU::opcode_bicb);
  register_instruction("BIS", DOUBLE_OPERAND_INSTRUCTION_MASK, 0050000, &CPU::opcode_bis);
  register_instruction("BISB", DOUBLE_OPERAND_INSTRUCTION_MASK, 0150000, &CPU::opcode_bisb);
  register_instruction("XOR", REGISTER_OPERAND_INSTRUCTION_MASK, 0074000, &CPU::opcode_xor);

  // Program Control Instructions
  register_instruction("BR", BRANCHING_OFFSET_INSTRUCTION_MASK, 0000400, &CPU::opcode_br);
  register_instruction("BNE", BRANCHING_OFFSET_INSTRUCTION_MASK, 0001000, &CPU::opcode_bne);
  register_instruction("BEQ", BRANCHING_OFFSET_INSTRUCTION_MASK, 0001400, &CPU::opcode_beq);
  register_instruction("BPL", BRANCHING_OFFSET_INSTRUCTION_MASK, 0100000, &CPU::opcode_bpl);
  register_instruction("BMI", BRANCHING_OFFSET_INSTRUCTION_MASK, 0100400, &CPU::opcode_bmi);
  register_instruction("BVC", BRANCHING_OFFSET_INSTRUCTION_MASK, 0102000, &CPU::opcode_bvc);
  register_instruction("BVS", BRANCHING_OFFSET_INSTRUCTION_MASK, 0102400, &CPU::opcode_bvs);
  register_instruction("BCC", BRANCHING_OFFSET_INSTRUCTION_MASK, 0103000, &CPU::opcode_bcc);
  register_instruction("BCS", BRANCHING_OFFSET_INSTRUCTION_MASK, 0103400, &CPU::opcode_bcs);
  register_instruction("BGE", BRANCHING_OFFSET_INSTRUCTION_MASK, 0002000, &CPU::opcode_bge);
  register_instruction("BLT", BRANCHING_OFFSET_INSTRUCTION_MASK, 0002400, &CPU::opcode_blt);
  register_instruction("BGT", BRANCHING_OFFSET_INSTRUCTION_MASK, 0003000, &CPU::opcode_bgt);
  register_instruction("BLE", BRANCHING_OFFSET_INSTRUCTION_MASK, 0003400, &CPU::opcode_ble);
  register_instruction("BHI", BRANCHING_OFFSET_INSTRUCTION_MASK, 0101000, &CPU::opcode_bhi);
  register_instruction("BLOS", BRANCHING_OFFSET_INSTRUCTION_MASK, 0101400, &CPU::opcode_blos);
  register_instruction("BHIS", BRANCHING_OFFSET_INSTRUCTION_MASK, 0103000, &CPU::opcode_bhis);
  register_instruction("BLO", BRANCHING_OFFSET_INSTRUCTION_MASK, 0103400, &CPU::opcode_blo);
  register_instruction("JMP", SINGLE_OPERAND_INSTRUCTION_MASK, 0000100, &CPU::opcode_jmp);

  // Subroutine Instructions
  register_instruction("JSR", REGISTER_OPERAND_INSTRUCTION_MASK, 0004000, &CPU::opcode_jsr);
  register_instruction("RTS", REGISTER_ONLY_INSTRUCTION_MASK, 0000200, &CPU::opcode_rts);
  register_instruction("MARK", SINGLE_OPERAND_INSTRUCTION_MASK, 0006400, &CPU::opcode_mark);
  register_instruction("SOB", REGISTER_OPERAND_INSTRUCTION_MASK, 0077000, &CPU::opcode_sob);
  register_instruction("EMT", BRANCHING_OFFSET_INSTRUCTION_MASK, 0104000, &CPU::opcode_emt);
  register_instruction("TRAP", BRANCHING_OFFSET_INSTRUCTION_MASK, 0104400, &CPU::opcode_trap);
  register_instruction("BPT", NO_OPERANDS_INSTRUCTION_MASK, 0000003, &CPU::opcode_bpt);
  register_instruction("IOT", NO_OPERANDS_INSTRUCTION_MASK, 0000004, &CPU::opcode_iot);
  register_instruction("RTI", NO_OPERANDS_INSTRUCTION_MASK, 0000002, &CPU::opcode_rti);
  register_instruction("RTT", NO_OPERANDS_INSTRUCTION_MASK, 0000006, &CPU::opcode_rtt);

  // Miscellaneous Instructions
  register_instruction("HALT", NO_OPERANDS_INSTRUCTION_MASK, 0000000, &CPU::opcode_halt);
  register_instruction("WAIT", NO_OPERANDS_INSTRUCTION_MASK, 0000001, &CPU::opcode_wait);
  register_instruction("RESET", NO_OPERANDS_INSTRUCTION_MASK, 0000005, &CPU::opcode_reset);
  register_instruction("MFPI", SINGLE_OPERAND_INSTRUCTION_MASK, 0006500, &CPU::opcode_mfpi);
  register_instruction("MTPI", SINGLE_OPERAND_INSTRUCTION_MASK, 0006600, &CPU::opcode_mtpi);
  register_instruction("NOP", NO_OPERANDS_INSTRUCTION_MASK, 0000240, &CPU::opcode_cco); // Used for disasm mnemonic
  register_instruction("CLN", NO_OPERANDS_INSTRUCTION_MASK, 0000250, &CPU::opcode_cco); // Used for disasm mnemonic
  register_instruction("CLZ", NO_OPERANDS_INSTRUCTION_MASK, 0000244, &CPU::opcode_cco); // Used for disasm mnemonic
  register_instruction("CLV", NO_OPERANDS_INSTRUCTION_MASK, 0000242, &CPU::opcode_cco); // Used for disasm mnemonic
  register_instruction("CLC", NO_OPERANDS_INSTRUCTION_MASK, 0000241, &CPU::opcode_cco); // Used for disasm mnemonic
  register_instruction("CCC", NO_OPERANDS_INSTRUCTION_MASK, 0000257, &CPU::opcode_cco); // Used for disasm mnemonic
  register_instruction("SEN", NO_OPERANDS_INSTRUCTION_MASK, 0000270, &CPU::opcode_cco); // Used for disasm mnemonic
  register_instruction("SEZ", NO_OPERANDS_INSTRUCTION_MASK, 0000264, &CPU::opcode_cco); // Used for disasm mnemonic
  register_instruction("SEV", NO_OPERANDS_INSTRUCTION_MASK, 0000262, &CPU::opcode_cco); // Used for disasm mnemonic
  register_instruction("SEC", NO_OPERANDS_INSTRUCTION_MASK, 0000261, &CPU::opcode_cco); // Used for disasm mnemonic
  register_instruction("SCC", NO_OPERANDS_INSTRUCTION_MASK, 0000277, &CPU::opcode_cco); // Used for disasm mnemonic
  register_instruction("CCO", CONDITION_CODE_INSTRUCTION_MASK, 0000240, &CPU::opcode_cco);

  cout << "Totally registered " << _instruction_set.size() << " instructions." << endl;
}

void CPU::register_instruction(string mnemonic,
                               uint16 opcode_mask,
                               uint16 opcode_signature,
                               void (CPU::*opcode_f)(uint16)) {
  CPUInstruction instruction;
  instruction.mnemonic = mnemonic;
  instruction.opcode_mask = opcode_mask;
  instruction.opcode_signature = opcode_signature;
  instruction.opcode_func = opcode_f;
  _instruction_set.push_back(instruction);

  ios_base::fmtflags cout_flags = cout.flags();
  cout << "CPU Instruction " << setw(4) << mnemonic << " (OPCODE SIGNATURE 0" << oct << setfill('0') << setw(6)
       << opcode_signature << ") registered." << endl;
  cout << setfill(' ') << setw(0);
  cout.flags(cout_flags);
}

void CPU::set_value(uint8 mode, uint8 address, uint16 value, bool byte_wide, bool update_pointers) {
  uint16 index;
  uint16 pointer;

  switch (mode) {
  case 0: // Register
    this->_r[address].r = value;
    break;
  case 1: // Register Deferred
    if (byte_wide)
      _unibus->write_byte(this->_r[address].r, (uint8) value);
    else
      _unibus->write_word(this->_r[address].r, value);
    break;
  case 2: // Autoincrement
    if (byte_wide) {
      _unibus->write_byte(this->_r[address].r, (uint8) value);
      this->_r[address].r += update_pointers ? 1 : 0;
    } else {
      _unibus->write_word(this->_r[address].r, value);
      this->_r[address].r += update_pointers ? 2 : 0;
    }
    break;
  case 3: // Autoincrement Deferred
    pointer = _unibus->read_word(this->_r[address].r);
    if (byte_wide) {
      _unibus->write_byte(pointer, (uint8) value);
    } else {
      _unibus->write_word(pointer, value);
    }
    this->_r[address].r += update_pointers ? 2 : 0;
    break;
  case 4: // Autodecrement
    if (byte_wide) {
      this->_r[address].r -= update_pointers ? 1 : 0;
      _unibus->write_byte(this->_r[address].r, (uint8) value);
    } else {
      this->_r[address].r -= update_pointers ? 2 : 0;
      _unibus->write_word(this->_r[address].r, value);
    }
    break;
  case 5: // Autodecrement Deferred
    pointer = _unibus->read_word(this->_r[address].r);
    if (byte_wide) {
      _unibus->write_byte(pointer, (uint8) value);
    } else {
      _unibus->write_word(pointer, value);
    }
    this->_r[address].r -= update_pointers ? 2 : 0;
    break;
  case 6: // Index
    index = _unibus->read_word(_pc.r);
    if (update_pointers)
      _pc_step += 2;
    if (byte_wide) {
      _unibus->write_byte(this->_r[address].r + index, (uint8) value);
    } else {
      _unibus->write_word(this->_r[address].r + index, value);
    }
    break;
  case 7: // Index Deferred
    index = _unibus->read_word(_pc.r);
    pointer = _unibus->read_word(this->_r[address].r + index);
    if (update_pointers)
      _pc_step += 2;
    if (byte_wide) {
      _unibus->write_byte(pointer, (uint8) value);
    } else {
      _unibus->write_word(pointer, value);
    }
    break;
  default:
    throw new runtime_error("Wrong addressing mode");
  }
}

uint16 CPU::get_value(uint8 mode, uint8 address, bool byte_wide, bool update_pointers) {
  uint16 index;
  uint16 pointer;
  uint16 value;

  switch (mode) {
  case 0: // Register
    return this->_r[address].r;
  case 1: // Register Deferred
    if (byte_wide)
      return _unibus->read_byte(this->_r[address].r);
    else
      return _unibus->read_word(this->_r[address].r);
  case 2: // Autoincrement
    if (byte_wide) {
      value = _unibus->read_byte(this->_r[address].r);
      this->_r[address].r += update_pointers ? 1 : 0;
    } else {
      value = _unibus->read_word(this->_r[address].r);
      this->_r[address].r += update_pointers ? 2 : 0;
    }
    return value;
  case 3: // Autoincrement Deferred
    pointer = _unibus->read_word(this->_r[address].r);
    if (byte_wide) {
      value = _unibus->read_byte(pointer);
    } else {
      value = _unibus->read_word(pointer);
    }
    this->_r[address].r += update_pointers ? 2 : 0;
    return value;
  case 4: // Autodecrement
    if (byte_wide) {
      this->_r[address].r -= update_pointers ? 1 : 0;
      return _unibus->read_byte(this->_r[address].r);
    } else {
      this->_r[address].r -= update_pointers ? 2 : 0;
      return _unibus->read_word(this->_r[address].r);
    }
  case 5: // Autodecrement Deferred
    this->_r[address].r -= update_pointers ? 2 : 0;
    pointer = _unibus->read_word(this->_r[address].r);
    if (byte_wide) {
      return _unibus->read_byte(pointer);
    } else {
      return _unibus->read_word(pointer);
    }
  case 6: // Index
    index = _unibus->read_word(_pc.r);
    if (update_pointers)
      _pc_step += 2;
    if (byte_wide) {
      return _unibus->read_byte(this->_r[address].r + index);
    } else {
      return _unibus->read_word(this->_r[address].r + index);
    }
  case 7: // Index Deferred
    index = _unibus->read_word(_pc.r);
    pointer = _unibus->read_word(this->_r[address].r + index);
    if (update_pointers)
      _pc_step += 2;
    if (byte_wide) {
      return _unibus->read_byte(pointer);
    } else {
      return _unibus->read_word(pointer);
    }
  default:
    throw new runtime_error("Wrong addressing mode");
  }
}

void CPU::set_destination_value(uint16 opcode, uint16 value, bool byte_wide, bool update_pointers) {
  uint8 mode = ((uint8) (opcode & 0000070)) >> 3;
  uint8 address = (uint8) (opcode & 0000007);
  set_value(mode, address, value, byte_wide, update_pointers);
}

uint16 CPU::get_destination_value(uint16 opcode, bool byte_wide, bool update_pointers) {
  uint8 mode = ((uint8) (opcode & 0000070)) >> 3;
  uint8 address = (uint8) (opcode & 0000007);
  return get_value(mode, address, byte_wide, update_pointers);
}

void CPU::set_source_value(uint16 opcode, uint16 value, bool byte_wide, bool update_pointers) {
  uint8 mode = (uint8) ((opcode & 0007000) >> 9);
  uint8 address = (uint8) (opcode & 0000700);
  set_value(mode, address, value, byte_wide, update_pointers);
}

uint16 CPU::get_source_value(uint16 opcode, bool byte_wide, bool update_pointers) {
  uint8 mode = (uint8) ((opcode & 0007000) >> 9);
  uint8 address = (uint8) (opcode & 0000700);
  return get_value(mode, address, byte_wide, update_pointers);
}

void CPU::stack_push(uint16 value) {
  _unibus->write_word(_sp.r, value);
  _sp.r -= 2;
}

uint16 CPU::stack_pop() {
  _sp.r += 2;
  return _unibus->read_word(_sp.r);
}

void CPU::execute_command() {
  uint16 opcode = _unibus->read_word((uint18) this->_pc.r);
  _pc.r += 2; // Already move forward from opcode
  _pc_step = 0;
  for (auto instruction_it = _instruction_set.begin(); instruction_it != _instruction_set.end();
       ++instruction_it) {
    // TODO: Optimize execution, combine opcodes wrt. masks
    if ((opcode & instruction_it->opcode_mask) == instruction_it->opcode_signature) {
      (this->*(instruction_it->opcode_func))(opcode);
      break;
    }
  }

  this->_pc.r += _pc_step;
}

void CPU::opcode_clr(uint16 opcode) {
  _psw.N = _psw.V = _psw.C = 0;
  _psw.Z = 1;
  set_destination_value(opcode, 0);
}

void CPU::opcode_clrb(uint16 opcode) {
  _psw.N = _psw.V = _psw.C = 0;
  _psw.Z = 1;
  set_destination_value(opcode, 0, true);
}

void CPU::opcode_com(uint16 opcode) {
  _psw.V = 0;
  _psw.C = 1;
  uint16 val16 = ~get_destination_value(opcode);
  _psw.Z = is_zero16(val16);
  _psw.N = is_negative16(val16);
  set_destination_value(opcode, val16, false, false);
}

void CPU::opcode_comb(uint16 opcode) {
  _psw.V = 0;
  _psw.C = 1;
  uint8 val8 = ~((uint8) get_destination_value(opcode, true));
  _psw.Z = is_zero8(val8);
  _psw.N = is_negative8(val8);
  set_destination_value(opcode, val8, true, false);
}

void CPU::opcode_inc(uint16 opcode) {
  uint16 val16 = get_destination_value(opcode);
  _psw.V = (uint8) (val16 == 077777 ? 1 : 0);
  val16++;
  _psw.Z = is_zero16(val16);
  _psw.N = is_negative16(val16);
  set_destination_value(opcode, val16, false, false);
}

void CPU::opcode_incb(uint16 opcode) {
  uint8 val8 = (uint8) get_destination_value(opcode, true);
  _psw.V = (uint8) (val8 == 0177 ? 1 : 0);
  val8++;
  _psw.Z = is_zero8(val8);
  _psw.N = is_negative8(val8);
  set_destination_value(opcode, val8, true, false);
}

void CPU::opcode_dec(uint16 opcode) {
  uint16 val16 = get_destination_value(opcode);
  _psw.V = (uint8) (val16 == 0100000 ? 1 : 0);
  val16--;
  _psw.Z = is_zero16(val16);
  _psw.N = is_negative16(val16);
  set_destination_value(opcode, val16, false, false);
}

void CPU::opcode_decb(uint16 opcode) {
  uint8 val8 = (uint8) get_destination_value(opcode, true);
  _psw.V = (uint8) (val8 == 0200 ? 1 : 0);
  val8--;
  _psw.Z = is_zero8(val8);
  _psw.N = is_negative8(val8);
  set_destination_value(opcode, val8, true, false);
}

void CPU::opcode_neg(uint16 opcode) {
  uint16 val16 = get_destination_value(opcode);
  val16 = (uint16) ((val16 == 0100000) ? 0100000 : -val16);
  _psw.V = (uint8) (val16 == 0100000 ? 1 : 0);
  _psw.Z = is_zero16(val16);
  _psw.N = is_negative16(val16);
  _psw.C = (uint8) (val16 == 0 ? 0 : 1);
  set_destination_value(opcode, val16, false, false);
}

void CPU::opcode_negb(uint16 opcode) {
  uint8 val8 = (uint8) get_destination_value(opcode, true);
  val8 = (uint8) ((val8 == 0100000) ? 0100000 : -val8);
  _psw.V = (uint8) (val8 == 0100000 ? 1 : 0);
  _psw.Z = is_zero8(val8);
  _psw.N = is_negative8(val8);
  _psw.C = (uint8) (val8 == 0 ? 0 : 1);
  set_destination_value(opcode, val8, true, false);
}

void CPU::opcode_tst(uint16 opcode) {
  uint16 val16 = get_destination_value(opcode);
  _psw.V = _psw.C = 0;
  _psw.Z = is_zero16(val16);
  _psw.N = is_negative16(val16);
  set_destination_value(opcode, val16, false, false);
}

void CPU::opcode_tstb(uint16 opcode) {
  uint8 val8 = (uint8) get_destination_value(opcode, true);
  _psw.V = _psw.C = 0;
  _psw.Z = is_zero8(val8);
  _psw.N = is_negative8(val8);
  set_destination_value(opcode, val8, true, false);
}

void CPU::opcode_asr(uint16 opcode) {
  uint16 val16 = get_destination_value(opcode);
  _psw.C = (uint8) (val16 & 0x1);
  uint8 sign_bit = (uint8) (val16 >> 15);
  val16 = (((uint16) sign_bit) << 15) | (val16 >> 1);
  _psw.Z = is_zero16(val16);
  _psw.N = sign_bit;
  _psw.V = _psw.N ^ _psw.C;
  set_destination_value(opcode, val16, false, false);
}

void CPU::opcode_asrb(uint16 opcode) {
  uint8 val8 = (uint8) get_destination_value(opcode, true);
  _psw.C = (uint8) (val8 & 0001);
  uint8 sign_bit = (uint8) (val8 >> 7);
  val8 = (sign_bit << 7) | (val8 >> 1);
  _psw.Z = is_zero8(val8);
  _psw.N = sign_bit;
  _psw.V = _psw.N ^ _psw.C;
  set_destination_value(opcode, val8, true, false);
}

void CPU::opcode_asl(uint16 opcode) {
  uint16 val16 = get_destination_value(opcode);
  _psw.C = (uint8) (val16 >> 15);
  val16 = val16 << 1;
  _psw.Z = is_zero16(val16);
  _psw.N = is_negative16(val16);
  _psw.V = _psw.N ^ _psw.C;
  set_destination_value(opcode, val16, false, false);
}

void CPU::opcode_aslb(uint16 opcode) {
  uint8 val8 = (uint8) get_destination_value(opcode, true);
  _psw.C = (uint8) (val8 >> 7);
  val8 = val8 << 1;
  _psw.Z = is_zero8(val8);
  _psw.N = is_negative8(val8);
  _psw.V = _psw.N ^ _psw.C;
  set_destination_value(opcode, val8, true, false);
}

void CPU::opcode_ror(uint16 opcode) {
  uint16 val16 = get_destination_value(opcode);
  uint8 sign_bit = _psw.C;
  _psw.C = (uint8) (val16 & 0001);
  val16 = (((uint16) sign_bit) << 15) | (val16 >> 1);
  _psw.Z = is_zero16(val16);
  _psw.N = is_negative16(val16);
  _psw.V = _psw.N ^ _psw.C;
  set_destination_value(opcode, val16, false, false);
}

void CPU::opcode_rorb(uint16 opcode) {
  uint8 val8 = (uint8) get_destination_value(opcode, true);
  uint8 sign_bit = _psw.C;
  _psw.C = (uint8) (val8 & 0001);
  val8 = (sign_bit << 7) | (val8 >> 1);
  _psw.Z = is_zero8(val8);
  _psw.N = is_negative8(val8);
  _psw.V = _psw.N ^ _psw.C;
  set_destination_value(opcode, val8, true, false);
}

void CPU::opcode_rol(uint16 opcode) {
  uint16 val16 = get_destination_value(opcode);
  uint8 sign_bit = (uint8) (val16 >> 15);
  val16 = _psw.C | (val16 << 1);
  _psw.C = sign_bit;
  _psw.Z = is_zero16(val16);
  _psw.N = is_negative16(val16);
  _psw.V = _psw.N ^ _psw.C;
  set_destination_value(opcode, val16, false, false);
}

void CPU::opcode_rolb(uint16 opcode) {
  uint8 val8 = (uint8) get_destination_value(opcode, true);
  uint8 sign_bit = val8 >> 7;
  val8 = _psw.C | (val8 << 1);
  _psw.C = sign_bit;
  _psw.Z = is_zero8(val8);
  _psw.N = is_negative8(val8);
  _psw.V = _psw.N ^ _psw.C;
  set_destination_value(opcode, val8, true, false);
}

void CPU::opcode_swab(uint16 opcode) {
  uint16 val16 = get_destination_value(opcode);
  uint8 val8 = (uint8) (val16 & 0377);
  uint8 tmp8 = (uint8) ((val16 >> 8) & 0377);
  val16 = (((uint16) val8) << 8) | tmp8;
  _psw.V = _psw.C = 0;
  _psw.N = is_negative8(tmp8);
  _psw.Z = is_zero8(tmp8);
  set_destination_value(opcode, val16, false, false);
}

void CPU::opcode_adc(uint16 opcode) {
  uint16 val16 = get_destination_value(opcode);
  uint8 tmp8 = _psw.C;
  _psw.C = (uint8) ((val16 == 0177777 && tmp8 == 1) ? 1 : 0);
  _psw.V = (uint8) ((val16 == 0077777 && tmp8 == 1) ? 1 : 0);
  val16 = val16 + tmp8;
  _psw.N = is_negative16(val16);
  _psw.Z = is_zero16(val16);
  set_destination_value(opcode, val16, false, false);
}

void CPU::opcode_adcb(uint16 opcode) {
  uint8 val8 = (uint8) get_destination_value(opcode, true);
  uint8 tmp8 = _psw.C;
  _psw.C = (uint8) ((val8 == 0377 && tmp8 == 1) ? 1 : 0);
  _psw.V = (uint8) ((val8 == 0177 && tmp8 == 1) ? 1 : 0);
  val8 = val8 + tmp8;
  _psw.N = is_negative8(val8);
  _psw.Z = is_zero8(val8);
  set_destination_value(opcode, val8, true, false);
}

void CPU::opcode_sbc(uint16 opcode) {
  uint16 val16 = get_destination_value(opcode);
  uint8 tmp8 = _psw.C;
  _psw.C = (uint8) ((val16 == 0000000 && tmp8 == 1) ? 1 : 0);
  _psw.V = (uint8) ((val16 == 0100000) ? 1 : 0);
  val16 = val16 - tmp8;
  _psw.N = is_negative16(val16);
  _psw.Z = is_zero16(val16);
  set_destination_value(opcode, val16, false, false);
}

void CPU::opcode_sbcb(uint16 opcode) {
  uint8 val8 = (uint8) get_destination_value(opcode, true);
  uint8 tmp8 = _psw.C;
  _psw.C = (uint8) ((val8 == 0377 && tmp8 == 1) ? 1 : 0);
  _psw.V = (uint8) ((val8 == 0177) ? 1 : 0);
  val8 = val8 - tmp8;
  _psw.N = is_negative8(val8);
  _psw.Z = is_zero8(val8);
  set_destination_value(opcode, val8, true, false);
}

void CPU::opcode_sxt(uint16 opcode) {
  uint16 val16 = (uint16) (_psw.N == 0 ? 0 : -1);
  _psw.Z = is_zero8(_psw.N);
  set_destination_value(opcode, val16);
}

void CPU::opcode_mov(uint16 opcode) {
  uint16 val16 = get_source_value(opcode);
  set_destination_value(opcode, val16);
  _psw.N = is_negative16(val16);
  _psw.Z = is_zero16(val16);
  _psw.V = 0;
}

void CPU::opcode_movb(uint16 opcode) {
  uint8 val8 = (uint8) get_source_value(opcode, true);
  set_destination_value(opcode, val8, true);
  _psw.N = is_negative8(val8);
  _psw.Z = is_zero8(val8);
  _psw.V = 0;
}

void CPU::opcode_cmp(uint16 opcode) {
  uint16 src_val16 = get_source_value(opcode);
  uint16 dst_val16 = get_destination_value(opcode);
  uint16 val16 = src_val16 - dst_val16;
  _psw.N = is_negative16(val16);
  _psw.Z = is_zero16(val16);
  _psw.V = (uint8) (is_negative16(src_val16) != is_negative16(dst_val16) && is_negative16(val16) == is_negative16(dst_val16) ? 1 : 0);
  _psw.C = (uint8) ((((uint32) src_val16) - dst_val16) != val16 ? 1 : 0);
}

void CPU::opcode_cmpb(uint16 opcode) {
  uint8 src_val8 = (uint8) get_source_value(opcode, true);
  uint8 dst_val8 = (uint8) get_destination_value(opcode, true);
  uint8 val8 = src_val8 - dst_val8;
  _psw.N = is_negative8(val8);
  _psw.Z = is_zero8(val8);
  _psw.V = (uint8) (is_negative8(src_val8) != is_negative8(dst_val8) && is_negative8(val8) == is_negative8(dst_val8) ? 1 : 0);
  _psw.C = (uint8) ((((uint32) src_val8) - dst_val8) != val8 ? 1 : 0);
}

void CPU::opcode_add(uint16 opcode) {
  uint16 src_val16 = get_source_value(opcode);
  uint16 dst_val16 = get_destination_value(opcode);
  uint16 val16 = src_val16 + dst_val16;
  set_destination_value(opcode, val16, false, false);
  _psw.N = is_negative16(val16);
  _psw.Z = is_zero16(val16);
  _psw.V = (uint8) (is_negative16(src_val16) == is_negative16(dst_val16) && is_negative16(val16) != is_negative16(dst_val16) ? 1 : 0);
  _psw.C = (uint8) ((((uint32) src_val16) + dst_val16) != val16 ? 1 : 0);
}

void CPU::opcode_sub(uint16 opcode) {
  uint16 src_val16 = get_source_value(opcode);
  uint16 dst_val16 = get_destination_value(opcode);
  uint16 val16 = src_val16 - dst_val16;
  set_destination_value(opcode, val16, false, false);
  _psw.N = is_negative16(val16);
  _psw.Z = is_zero16(val16);
  _psw.V = (uint8) (is_negative16(src_val16) != is_negative16(dst_val16) && is_negative16(val16) == is_negative16(dst_val16) ? 1 : 0);
  _psw.C = (uint8) ((((uint32) src_val16) - dst_val16) != val16 ? 1 : 0);
}

void CPU::opcode_bit(uint16 opcode) {
  uint16 src_val16 = get_source_value(opcode);
  uint16 dst_val16 = get_destination_value(opcode);
  uint16 val16 = src_val16 & dst_val16;
  _psw.N = is_negative16(val16);
  _psw.Z = is_zero16(val16);
  _psw.V = (uint8) 0;
}

void CPU::opcode_bitb(uint16 opcode) {
  uint8 src_val8 = (uint8) get_source_value(opcode, true);
  uint8 dst_val8 = (uint8) get_destination_value(opcode, true);
  uint8 val8 = src_val8 & dst_val8;
  _psw.N = is_negative8(val8);
  _psw.Z = is_zero8(val8);
  _psw.V = (uint8) 0;
}

void CPU::opcode_bic(uint16 opcode) {
  uint16 src_val16 = get_source_value(opcode);
  uint16 dst_val16 = get_destination_value(opcode);
  uint16 val16 = ~src_val16 & dst_val16;
  set_destination_value(opcode, val16, false, false);
  _psw.N = is_negative16(val16);
  _psw.Z = is_zero16(val16);
  _psw.V = (uint8) 0;
}

void CPU::opcode_bicb(uint16 opcode) {
  uint8 src_val8 = (uint8) get_source_value(opcode, true);
  uint8 dst_val8 = (uint8) get_destination_value(opcode, true);
  uint8 val8 = ~src_val8 & dst_val8;
  set_destination_value(opcode, val8, true, false);
  _psw.N = is_negative8(val8);
  _psw.Z = is_zero8(val8);
  _psw.V = (uint8) 0;
}

void CPU::opcode_bis(uint16 opcode) {
  uint16 src_val16 = get_source_value(opcode);
  uint16 dst_val16 = get_destination_value(opcode);
  uint16 val16 = src_val16 | dst_val16;
  set_destination_value(opcode, val16, false, false);
  _psw.N = is_negative16(val16);
  _psw.Z = is_zero16(val16);
  _psw.V = (uint8) 0;
}

void CPU::opcode_bisb(uint16 opcode) {
  uint8 src_val8 = (uint8) get_source_value(opcode, true);
  uint8 dst_val8 = (uint8) get_destination_value(opcode, true);
  uint8 val8 = src_val8 | dst_val8;
  set_destination_value(opcode, val8, true, false);
  _psw.N = is_negative8(val8);
  _psw.Z = is_zero8(val8);
  _psw.V = (uint8) 0;
}

void CPU::opcode_xor(uint16 opcode) {
  uint16 src_val16 = (uint16) ((opcode & 0000700) >> 6);
  uint16 dst_val16 = get_destination_value(opcode);
  uint16 val16 = src_val16 ^dst_val16;
  set_destination_value(opcode, val16, false, false);
  _psw.N = is_negative16(val16);
  _psw.Z = is_zero16(val16);
  _psw.V = (uint8) 0;
}

void CPU::opcode_br(uint16 opcode) {
  uint16 offset = opcode & BRANCHING_OFFSET_MASK;
  _pc.r = _pc.r + (offset << 1);
}

void CPU::opcode_bne(uint16 opcode) {
  uint16 offset = opcode & BRANCHING_OFFSET_MASK;
  if (_psw.Z == 0)
    _pc.r = _pc.r + (offset << 1);
}

void CPU::opcode_beq(uint16 opcode) {
  uint16 offset = opcode & BRANCHING_OFFSET_MASK;
  if (_psw.Z == 1)
    _pc.r = _pc.r + (offset << 1);
}

void CPU::opcode_bpl(uint16 opcode) {
  uint16 offset = opcode & BRANCHING_OFFSET_MASK;
  if (_psw.N == 0)
    _pc.r = _pc.r + (offset << 1);
}

void CPU::opcode_bmi(uint16 opcode) {
  uint16 offset = opcode & BRANCHING_OFFSET_MASK;
  if (_psw.N == 1)
    _pc.r = _pc.r + (offset << 1);
}

void CPU::opcode_bvc(uint16 opcode) {
  uint16 offset = opcode & BRANCHING_OFFSET_MASK;
  if (_psw.V == 0)
    _pc.r = _pc.r + (offset << 1);
}

void CPU::opcode_bvs(uint16 opcode) {
  uint16 offset = opcode & BRANCHING_OFFSET_MASK;
  if (_psw.V == 1)
    _pc.r = _pc.r + (offset << 1);
}

void CPU::opcode_bcc(uint16 opcode) {
  uint16 offset = opcode & BRANCHING_OFFSET_MASK;
  if (_psw.C == 0)
    _pc.r = _pc.r + (offset << 1);
}

void CPU::opcode_bcs(uint16 opcode) {
  uint16 offset = opcode & BRANCHING_OFFSET_MASK;
  if (_psw.C == 1)
    _pc.r = _pc.r + (offset << 1);
}

void CPU::opcode_bge(uint16 opcode) {
  uint16 offset = opcode & BRANCHING_OFFSET_MASK;
  if (_psw.V == _psw.N)
    _pc.r = _pc.r + (offset << 1);
}

void CPU::opcode_blt(uint16 opcode) {
  uint16 offset = opcode & BRANCHING_OFFSET_MASK;
  if ((_psw.V ^ _psw.N) == 1)
    _pc.r = _pc.r + (offset << 1);
}

void CPU::opcode_bgt(uint16 opcode) {
  uint16 offset = opcode & BRANCHING_OFFSET_MASK;
  if (_psw.V == _psw.N && _psw.Z == 0)
    _pc.r = _pc.r + (offset << 1);
}

void CPU::opcode_ble(uint16 opcode) {
  uint16 offset = opcode & BRANCHING_OFFSET_MASK;
  if ((_psw.V ^ _psw.N) == 1 && _psw.Z == 0)
    _pc.r = _pc.r + (offset << 1);
}

void CPU::opcode_bhi(uint16 opcode) {
  uint16 offset = opcode & BRANCHING_OFFSET_MASK;
  if (_psw.C == 0 && _psw.Z == 0)
    _pc.r = _pc.r + (offset << 1);
}

void CPU::opcode_blos(uint16 opcode) {
  uint16 offset = opcode & BRANCHING_OFFSET_MASK;
  if (_psw.C == 1 || _psw.Z == 1)
    _pc.r = _pc.r + (offset << 1);
}

void CPU::opcode_bhis(uint16 opcode) {
  uint16 offset = opcode & BRANCHING_OFFSET_MASK;
  if (_psw.C == 0)
    _pc.r = _pc.r + (offset << 1);
}

void CPU::opcode_blo(uint16 opcode) {
  uint16 offset = opcode & BRANCHING_OFFSET_MASK;
  if (_psw.C == 1)
    _pc.r = _pc.r + (offset << 1);
}

void CPU::opcode_jmp(uint16 opcode) {
  if ((opcode & 0000070) >> 3) {
    throw new logic_error("Illegal JMP instruction");
  }
  uint16 val16 = get_destination_value(opcode);
  _pc.r = val16;
}

void CPU::opcode_jsr(uint16 opcode) {
  uint16 tmp16 = get_destination_value(opcode);
  uint16 reg_n = (uint16) ((opcode >> 6) & 07);
  stack_push(_r[reg_n].r);
  _r[reg_n].r = _pc.r;
  _pc.r = tmp16;
}

void CPU::opcode_rts(uint16 opcode) {
  uint16 reg_n = (uint16) (opcode & 07);
  _pc.r = _r[reg_n].r;
  _r[reg_n].r = stack_pop();
}

void CPU::opcode_mark(uint16 opcode) {
  uint16 nn = (uint16) (opcode & 077);
  _sp.r = _sp.r + (nn << 1);
  _pc.r = _r[5].r;
  _r[5].r = _unibus->read_word(_sp.r);
}

void CPU::opcode_sob(uint16 opcode) {
  uint16 offset = (uint16) (opcode & 077);
  uint16 reg_n = (uint16) ((opcode >> 6) & 07);
  _r[reg_n].r -= 1;
  if (_r[reg_n].r != 0)
    _pc.r = _pc.r - (offset << 1);
}

void CPU::opcode_emt(uint16 opcode) { // TODO: Check this instruction
  stack_push(_psw.ps);
  stack_push(_pc.r);
  _pc.r = _unibus->read_word(030);
  _psw.ps = (uint8) _unibus->read_word(032);
}

void CPU::opcode_trap(uint16 opcode) { // TODO: Check this instruction
  stack_push(_psw.ps);
  stack_push(_pc.r);
  _pc.r = _unibus->read_word(034);
  _psw.ps = (uint8) _unibus->read_word(036);
}

void CPU::opcode_bpt(uint16 opcode) { // TODO: Check this instruction
  stack_push(_psw.ps);
  stack_push(_pc.r);
  _pc.r = _unibus->read_word(014);
  _psw.ps = (uint8) _unibus->read_word(016);
}

void CPU::opcode_iot(uint16 opcode) { // TODO: Check this instruction
  stack_push(_psw.ps);
  stack_push(_pc.r);
  _pc.r = _unibus->read_word(020);
  _psw.ps = (uint8) _unibus->read_word(022);
}

void CPU::opcode_rti(uint16 opcode) { // TODO: Check this instruction
  _pc.r = stack_pop();
  _psw.ps = (uint8) stack_pop();
}

void CPU::opcode_rtt(uint16 opcode) { // TODO: Check this instruction
  _pc.r = stack_pop();
  _psw.ps = (uint8) stack_pop();
}

void CPU::opcode_halt(uint16 opcode) {
  // TODO: Implement HALT instruction
}

void CPU::opcode_wait(uint16 opcode) {
  // TODO: Implement WAIT instruction
}

void CPU::opcode_reset(uint16 opcode) {
  _unibus->set_init_line(10);
}

void CPU::opcode_mfpi(uint16 opcode) { // TODO: Check this opode
  uint16 tmp = get_destination_value(opcode);
  stack_push(tmp);
  _psw.N = is_negative16(tmp);
  _psw.Z = is_zero16(tmp);
  _psw.V = 0;
}

void CPU::opcode_mtpi(uint16 opcode) { // TODO: Check this opode
  uint16 tmp = stack_pop();
  set_destination_value(opcode, tmp);
  _psw.N = is_negative16(tmp);
  _psw.Z = is_zero16(tmp);
  _psw.V = 0;
}

void CPU::opcode_cco(uint16 opcode) {
  uint8 val = (uint8) ((opcode >> 4) & 1);
  if ((opcode & 0000001) == 0000001)
    _psw.C = val;
  if ((opcode & 0000002) == 0000002)
    _psw.V = val;
  if ((opcode & 0000004) == 0000004)
    _psw.Z = val;
  if ((opcode & 0000010) == 0000010)
    _psw.N = val;
}
