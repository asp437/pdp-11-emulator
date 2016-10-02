//
// Created by Aleksandr Parfenov on 01.10.16.
//

#include "CPU.h"

#include <iomanip>
#include <cmath>

// TODO: Rewrite negative check
// TODO: Write common flags check macorss/function
const uint16 SINGLE_OPERAND_INSTRUCTION_MASK = (const uint16) 07777700;
const uint16 DOUBLE_OPERAND_INSTRUCTION_MASK = (const uint16) 07770000;
const uint16 REGISTER_OPERAND_INSTRUCTION_MASK = (const uint16) 07777000;

CPU::CPU(Unibus *unibus) : _unibus(unibus) {
  // TODO: Initialize PC and PSW, register all opcodes
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

  cout << "Totally registered " << cpu_instruction_set.size() << " instructions." << endl;
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
  cpu_instruction_set.push_back(instruction);

  ios_base::fmtflags cout_flags = cout.flags();
  cout << "CPU Instruction " << setw(4) << mnemonic << " (OPCODE SIGNATURE 0" << oct << setfill('0') << setw(6)
       << opcode_signature << ") registered." << endl;
  cout << setfill(' ') << setw(0);
  cout.flags(cout_flags);
}

void CPU::set_value(uint16 mode, uint16 address, uint16 value, bool byte_wide, bool update_pointers) {
  uint16 index;
  uint16 pointer;

  switch (mode) {
  case 0: // Register
    this->r[address].r = value;
    break;
  case 1: // Register Deferred
    if (byte_wide)
      _unibus->write_byte(this->r[address].r, (uint8) value);
    else
      _unibus->write_word(this->r[address].r, value);
    break;
  case 2: // Autoincrement
    if (byte_wide) {
      _unibus->write_byte(this->r[address].r, (uint8) value);
      this->r[address].r += update_pointers ? 1 : 0;
    } else {
      _unibus->write_word(this->r[address].r, value);
      this->r[address].r += update_pointers ? 2 : 0;
    }
    break;
  case 3: // Autoincrement Deferred
    pointer = _unibus->read_word(this->r[address].r);
    if (byte_wide) {
      _unibus->write_byte(pointer, (uint8) value);
    } else {
      _unibus->write_word(pointer, value);
    }
    this->r[address].r += update_pointers ? 2 : 0;
    break;
  case 4: // Autodecrement
    if (byte_wide) {
      this->r[address].r -= update_pointers ? 1 : 0;
      _unibus->write_byte(this->r[address].r, (uint8) value);
    } else {
      this->r[address].r -= update_pointers ? 2 : 0;
      _unibus->write_word(this->r[address].r, value);
    }
    break;
  case 5: // Autodecrement Deferred
    pointer = _unibus->read_word(this->r[address].r);
    if (byte_wide) {
      _unibus->write_byte(pointer, (uint8) value);
    } else {
      _unibus->write_word(pointer, value);
    }
    this->r[address].r -= update_pointers ? 2 : 0;
    break;
  case 6: // Index
    index = _unibus->read_word(pc.r);
    if (byte_wide) {
      _unibus->write_byte(this->r[address].r + index, (uint8) value);
    } else {
      _unibus->write_word(this->r[address].r + index, value);
    }
    break;
  case 7: // Index Deferred
    index = _unibus->read_word(pc.r);
    pointer = _unibus->read_word(this->r[address].r + index);
    if (byte_wide) {
      _unibus->write_byte(pointer, (uint8) value);
    } else {
      _unibus->write_word(pointer, value);
    }
    break;
  }
}

uint16 CPU::get_value(uint16 mode, uint16 address, bool byte_wide, bool update_pointers) {
  uint16 index;
  uint16 pointer;
  uint16 value;

  switch (mode) {
  case 0: // Register
    return this->r[address].r;
  case 1: // Register Deferred
    if (byte_wide)
      return _unibus->read_byte(this->r[address].r);
    else
      return _unibus->read_word(this->r[address].r);
  case 2: // Autoincrement
    if (byte_wide) {
      value = _unibus->read_byte(this->r[address].r);
      this->r[address].r += update_pointers ? 1 : 0;
    } else {
      value = _unibus->read_word(this->r[address].r);
      this->r[address].r += update_pointers ? 2 : 0;
    }
    return value;
  case 3: // Autoincrement Deferred
    pointer = _unibus->read_word(this->r[address].r);
    if (byte_wide) {
      value = _unibus->read_byte(pointer);
    } else {
      value = _unibus->read_word(pointer);
    }
    this->r[address].r += update_pointers ? 2 : 0;
    return value;
  case 4: // Autodecrement
    if (byte_wide) {
      this->r[address].r -= update_pointers ? 1 : 0;
      return _unibus->read_byte(this->r[address].r);
    } else {
      this->r[address].r -= update_pointers ? 2 : 0;
      return _unibus->read_word(this->r[address].r);
    }
  case 5: // Autodecrement Deferred
    this->r[address].r -= update_pointers ? 2 : 0;
    pointer = _unibus->read_word(this->r[address].r);
    if (byte_wide) {
      return _unibus->read_byte(pointer);
    } else {
      return _unibus->read_word(pointer);
    }
  case 6: // Index
    index = _unibus->read_word(pc.r);
    if (byte_wide) {
      return _unibus->read_byte(this->r[address].r + index);
    } else {
      return _unibus->read_word(this->r[address].r + index);
    }
  case 7: // Index Deferred
    index = _unibus->read_word(pc.r);
    pointer = _unibus->read_word(this->r[address].r + index);
    if (byte_wide) {
      return _unibus->read_byte(pointer);
    } else {
      return _unibus->read_word(pointer);
    }
  }
  return 0; // TODO: Throw exception
}

void CPU::set_destination_value(uint16 opcode, uint16 value, bool byte_wide, bool update_pointers) {
  uint8 mode = (uint8) (opcode & 0000070) >> 3;
  uint8 address = (uint8) (opcode & 0000007);
  set_value(mode, address, value, byte_wide, update_pointers);
}

uint16 CPU::get_destination_value(uint16 opcode, bool byte_wide, bool update_pointers) {
  uint8 mode = (uint8) (opcode & 0000070) >> 3;
  uint8 address = (uint8) (opcode & 0000007);
  return get_value(mode, address, byte_wide, update_pointers);
}

void CPU::set_source_value(uint16 opcode, uint16 value, bool byte_wide, bool update_pointers) {
  uint8 mode = (uint8) (opcode & 0007000) >> 9;
  uint8 address = (uint8) (opcode & 0000700);
  set_value(mode, address, value, byte_wide, update_pointers);
}

uint16 CPU::get_source_value(uint16 opcode, bool byte_wide, bool update_pointers) {
  uint8 mode = (uint8) (opcode & 0007000) >> 9;
  uint8 address = (uint8) (opcode & 0000700);
  return get_value(mode, address, byte_wide, update_pointers);
}

void CPU::execute_command() {
  uint16 opcode = _unibus->read_word((uint18) this->pc.r);

  for (auto instruction_it = cpu_instruction_set.begin(); instruction_it != cpu_instruction_set.end();
       ++instruction_it) {
    // TODO: Optimize execution, combine opcodes wrt. masks
    if ((opcode & instruction_it->opcode_mask) == instruction_it->opcode_signature) {
      (this->*(instruction_it->opcode_func))(opcode);
    }
  }

  // TODO: Increment wrt. index operand addressing
  this->pc.r += 2;
}

void CPU::opcode_clr(uint16 opcode) {
  psw.N = psw.V = psw.C = 0;
  psw.Z = 1;
  set_destination_value(opcode, 0);
}

void CPU::opcode_clrb(uint16 opcode) {
  psw.N = psw.V = psw.C = 0;
  psw.Z = 1;
  set_destination_value(opcode, 0, true);
}

void CPU::opcode_com(uint16 opcode) {
  psw.V = 0;
  psw.C = 1;
  uint16 val16 = ~get_destination_value(opcode);
  psw.Z = (uint8) (val16 == 0 ? 1 : 0);
  psw.N = (uint8) (val16 >> 15);
  set_destination_value(opcode, val16, false, false);
}

void CPU::opcode_comb(uint16 opcode) {
  psw.V = 0;
  psw.C = 1;
  uint8 val8 = ~((uint8) get_destination_value(opcode, true));
  psw.Z = (uint8) (val8 == 0 ? 1 : 0);
  psw.N = (uint8) (val8 >> 7);
  set_destination_value(opcode, val8, true, false);
}

void CPU::opcode_inc(uint16 opcode) {
  uint16 val16 = get_destination_value(opcode);
  psw.V = (uint8) (val16 == 077777 ? 1 : 0);
  val16++;
  psw.Z = (uint8) (val16 == 0 ? 1 : 0);
  psw.N = (uint8) (val16 < 0 ? 1 : 0);
  set_destination_value(opcode, val16, false, false);
}

void CPU::opcode_incb(uint16 opcode) {
  uint8 val8 = (uint8) get_destination_value(opcode, true);
  psw.V = (uint8) (val8 == 077777 ? 1 : 0); // TODO: Maybe compare with 0177?
  val8++;
  psw.Z = (uint8) (val8 == 0 ? 1 : 0);
  psw.N = (uint8) (val8 < 0 ? 1 : 0);
  set_destination_value(opcode, val8, true, false);
}

void CPU::opcode_dec(uint16 opcode) {
  uint16 val16 = get_destination_value(opcode);
  psw.V = (uint8) (val16 == 0100000 ? 1 : 0);
  val16--;
  psw.Z = (uint8) (val16 == 0 ? 1 : 0);
  psw.N = (uint8) (val16 < 0 ? 1 : 0);
  set_destination_value(opcode, val16, false, false);
}

void CPU::opcode_decb(uint16 opcode) {
  uint8 val8 = (uint8) get_destination_value(opcode, true);
  psw.V = (uint8) (val8 == 0100000 ? 1 : 0); // TODO: Maybe compare with 0200?
  val8--;
  psw.Z = (uint8) (val8 == 0 ? 1 : 0);
  psw.N = (uint8) (val8 < 0 ? 1 : 0);
  set_destination_value(opcode, val8, true, false);
}

void CPU::opcode_neg(uint16 opcode) {
  uint16 val16 = get_destination_value(opcode);
  val16 = (uint16) ((val16 == 0100000) ? 0100000 : -val16);
  psw.V = (uint8) (val16 == 0100000 ? 1 : 0);
  psw.Z = (uint8) (val16 == 0 ? 1 : 0);
  psw.N = (uint8) (val16 < 0 ? 1 : 0);
  psw.C = (uint8) (val16 == 0 ? 0 : 1);
  set_destination_value(opcode, val16, false, false);
}

void CPU::opcode_negb(uint16 opcode) {
  uint8 val8 = (uint8) get_destination_value(opcode, true);
  val8 = (uint8) ((val8 == 0100000) ? 0100000 : -val8);
  psw.V = (uint8) (val8 == 0100000 ? 1 : 0);
  psw.Z = (uint8) (val8 == 0 ? 1 : 0);
  psw.N = (uint8) (val8 < 0 ? 1 : 0);
  psw.C = (uint8) (val8 == 0 ? 0 : 1);
  set_destination_value(opcode, val8, true, false);
}

void CPU::opcode_tst(uint16 opcode) {
  uint16 val16 = get_destination_value(opcode);
  psw.V = psw.C = 0;
  psw.Z = (uint8) (val16 == 0 ? 1 : 0);
  psw.N = (uint8) (val16 < 0 ? 1 : 0);
  set_destination_value(opcode, val16, false, false);
}

void CPU::opcode_tstb(uint16 opcode) {
  uint8 val8 = (uint8) get_destination_value(opcode, true);
  psw.V = psw.C = 0;
  psw.Z = (uint8) (val8 == 0 ? 1 : 0);
  psw.N = (uint8) (val8 < 0 ? 1 : 0);
  set_destination_value(opcode, val8, true, false);
}

void CPU::opcode_asr(uint16 opcode) {
  uint16 val16 = get_destination_value(opcode);
  psw.C = (uint8) (val16 & 0x1);
  uint8 sign_bit = (uint8) (val16 >> 15);
  val16 = (((uint16) sign_bit) << 15) | (val16 >> 1);
  psw.Z = (uint8) (val16 == 0 ? 1 : 0);
  psw.N = sign_bit;
  psw.V = psw.N ^ psw.C;
  set_destination_value(opcode, val16, false, false);
}

void CPU::opcode_asrb(uint16 opcode) {
  uint8 val8 = (uint8) get_destination_value(opcode, true);
  psw.C = (uint8) (val8 & 0001);
  uint8 sign_bit = (uint8) (val8 >> 7);
  val8 = (sign_bit << 7) | (val8 >> 1);
  psw.Z = (uint8) (val8 == 0 ? 1 : 0);
  psw.N = sign_bit;
  psw.V = psw.N ^ psw.C;
  set_destination_value(opcode, val8, true, false);
}

void CPU::opcode_asl(uint16 opcode) {
  uint16 val16 = get_destination_value(opcode);
  psw.C = (uint8) (val16 >> 15);
  val16 = val16 << 1;
  psw.Z = (uint8) (val16 == 0 ? 1 : 0);
  psw.N = (uint8) (val16 < 0 ? 1 : 0);
  psw.V = psw.N ^ psw.C;
  set_destination_value(opcode, val16, false, false);
}

void CPU::opcode_aslb(uint16 opcode) {
  uint8 val8 = (uint8) get_destination_value(opcode, true);
  psw.C = (uint8) (val8 >> 7);
  val8 = val8 << 1;
  psw.Z = (uint8) (val8 == 0 ? 1 : 0);
  psw.N = (uint8) (val8 < 0 ? 1 : 0);
  psw.V = psw.N ^ psw.C;
  set_destination_value(opcode, val8, true, false);
}

void CPU::opcode_ror(uint16 opcode) {
  uint16 val16 = get_destination_value(opcode);
  uint8 sign_bit = psw.C;
  psw.C = (uint8) (val16 & 0001);
  val16 = (((uint16) sign_bit) << 15) | (val16 >> 1);
  psw.Z = (uint8) (val16 == 0 ? 1 : 0);
  psw.N = (uint8) (val16 < 0 ? 1 : 0);
  psw.V = psw.N ^ psw.C;
  set_destination_value(opcode, val16, false, false);
}

void CPU::opcode_rorb(uint16 opcode) {
  uint8 val8 = (uint8) get_destination_value(opcode, true);
  uint8 sign_bit = psw.C;
  psw.C = (uint8) (val8 & 0001);
  val8 = (sign_bit << 7) | (val8 >> 1);
  psw.Z = (uint8) (val8 == 0 ? 1 : 0);
  psw.N = (uint8) (val8 < 0 ? 1 : 0);
  psw.V = psw.N ^ psw.C;
  set_destination_value(opcode, val8, true, false);
}

void CPU::opcode_rol(uint16 opcode) {
  uint16 val16 = get_destination_value(opcode);
  uint8 sign_bit = (uint8) (val16 >> 15);
  val16 = psw.C | (val16 << 1);
  psw.C = sign_bit;
  psw.Z = (uint8) (val16 == 0 ? 1 : 0);
  psw.N = (uint8) (val16 < 0 ? 1 : 0);
  psw.V = psw.N ^ psw.C;
  set_destination_value(opcode, val16, false, false);
}

void CPU::opcode_rolb(uint16 opcode) {
  uint8 val8 = (uint8) get_destination_value(opcode, true);
  uint8 sign_bit = val8 >> 7;
  val8 = psw.C | (val8 << 1);
  psw.C = sign_bit;
  psw.Z = (uint8) (val8 == 0 ? 1 : 0);
  psw.N = (uint8) (val8 < 0 ? 1 : 0);
  psw.V = psw.N ^ psw.C;
  set_destination_value(opcode, val8, true, false);
}

void CPU::opcode_swab(uint16 opcode) {
  uint16 val16 = get_destination_value(opcode);
  uint8 val8 = (uint8) (val16 & 0377);
  uint8 tmp8 = (uint8) ((val16 >> 8) & 0377);
  val16 = (((uint16) val8) << 8) | tmp8;
  psw.V = psw.C = 0;
  psw.N = (uint8) (tmp8 >> 7 == 1);
  psw.Z = (uint8) (tmp8 == 0 ? 1 : 0);
  set_destination_value(opcode, val16, false, false);
}

void CPU::opcode_adc(uint16 opcode) {
  uint16 val16 = get_destination_value(opcode);
  uint8 tmp8 = psw.C;
  psw.C = (uint8) ((val16 == 0177777 && tmp8 == 1) ? 1 : 0);
  psw.V = (uint8) ((val16 == 0077777 && tmp8 == 1) ? 1 : 0);
  val16 = val16 + tmp8;
  psw.N = (uint8) (val16 < 0 ? 1 : 0);
  psw.Z = (uint8) (val16 == 0 ? 1 : 0);
  set_destination_value(opcode, val16, false, false);
}

void CPU::opcode_adcb(uint16 opcode) {
  uint8 val8 = (uint8) get_destination_value(opcode, true);
  uint8 tmp8 = psw.C;
  psw.C = (uint8) ((val8 == 0177777 && tmp8 == 1) ? 1 : 0); // TODO: Maybe compare with 377?
  psw.V = (uint8) ((val8 == 0077777 && tmp8 == 1) ? 1 : 0); // TODO: Maybe compare with 177?
  val8 = val8 + tmp8;
  psw.N = (uint8) (val8 < 0 ? 1 : 0);
  psw.Z = (uint8) (val8 == 0 ? 1 : 0);
  set_destination_value(opcode, val8, true, false);
}

void CPU::opcode_sbc(uint16 opcode) {
  uint16 val16 = get_destination_value(opcode);
  uint8 tmp8 = psw.C;
  psw.C = (uint8) ((val16 == 0000000 && tmp8 == 1) ? 1 : 0);
  psw.V = (uint8) ((val16 == 0100000) ? 1 : 0);
  val16 = val16 - tmp8;
  psw.N = (uint8) (val16 < 0 ? 1 : 0);
  psw.Z = (uint8) (val16 == 0 ? 1 : 0);
  set_destination_value(opcode, val16, false, false);
}

void CPU::opcode_sbcb(uint16 opcode) {
  uint8 val8 = (uint8) get_destination_value(opcode, true);
  uint8 tmp8 = psw.C;
  psw.C = (uint8) ((val8 == 0000000 && tmp8 == 1) ? 1 : 0); // TODO: Maybe compare with 377?
  psw.V = (uint8) ((val8 == 0100000) ? 1 : 0); // TODO: Maybe compare with 177?
  val8 = val8 - tmp8;
  psw.N = (uint8) (val8 < 0 ? 1 : 0);
  psw.Z = (uint8) (val8 == 0 ? 1 : 0);
  set_destination_value(opcode, val8, true, false);
}

void CPU::opcode_sxt(uint16 opcode) {
  uint16 val16 = (uint16) (psw.N == 0 ? 0 : -1);
  psw.Z = (uint8) (psw.N == 0 ? 1 : 0);
  set_destination_value(opcode, val16);
}

void CPU::opcode_mov(uint16 opcode) {
  uint16 val16 = get_source_value(opcode);
  set_destination_value(opcode, val16);
  psw.N = (uint8) (val16 < 0 ? 1 : 0);
  psw.Z = (uint8) (val16 == 0 ? 1 : 0);
  psw.V = 0;
}

void CPU::opcode_movb(uint16 opcode) {
  uint8 val8 = (uint8) get_source_value(opcode, true);
  set_destination_value(opcode, val8, true);
  psw.N = (uint8) (val8 < 0 ? 1 : 0);
  psw.Z = (uint8) (val8 == 0 ? 1 : 0);
  psw.V = 0;
}

void CPU::opcode_cmp(uint16 opcode) {
  uint16 src_val16 = get_source_value(opcode);
  uint16 dst_val16 = get_destination_value(opcode);
  uint16 val16 = src_val16 - dst_val16;
  psw.N = (uint8) (val16 < 0 ? 1 : 0);
  psw.Z = (uint8) (val16 == 0 ? 1 : 0);
  psw.V = (uint8) (signbit(src_val16) != signbit(dst_val16) && signbit(val16) == signbit(dst_val16) ? 1 : 0);
  psw.C = (uint8) ((((uint32) src_val16) - dst_val16) != val16 ? 1 : 0);
}

void CPU::opcode_cmpb(uint16 opcode) {
  uint8 src_val8 = (uint8) get_source_value(opcode, true);
  uint8 dst_val8 = (uint8) get_destination_value(opcode, true);
  uint8 val8 = src_val8 - dst_val8;
  psw.N = (uint8) (val8 < 0 ? 1 : 0);
  psw.Z = (uint8) (val8 == 0 ? 1 : 0);
  psw.V = (uint8) (signbit(src_val8) != signbit(dst_val8) && signbit(val8) == signbit(dst_val8) ? 1 : 0);
  psw.C = (uint8) ((((uint32) src_val8) - dst_val8) != val8 ? 1 : 0);
}

void CPU::opcode_add(uint16 opcode) {
  uint16 src_val16 = get_source_value(opcode);
  uint16 dst_val16 = get_destination_value(opcode);
  uint16 val16 = src_val16 + dst_val16;
  set_destination_value(opcode, val16, false, false);
  psw.N = (uint8) (val16 < 0 ? 1 : 0);
  psw.Z = (uint8) (val16 == 0 ? 1 : 0);
  psw.V = (uint8) (signbit(src_val16) == signbit(dst_val16) && signbit(val16) != signbit(dst_val16) ? 1 : 0);
  psw.C = (uint8) ((((uint32) src_val16) + dst_val16) != val16 ? 1 : 0);
}

void CPU::opcode_sub(uint16 opcode) {
  uint16 src_val16 = get_source_value(opcode);
  uint16 dst_val16 = get_destination_value(opcode);
  uint16 val16 = src_val16 - dst_val16;
  set_destination_value(opcode, val16, false, false);
  psw.N = (uint8) (val16 < 0 ? 1 : 0);
  psw.Z = (uint8) (val16 == 0 ? 1 : 0);
  psw.V = (uint8) (signbit(src_val16) != signbit(dst_val16) && signbit(val16) == signbit(dst_val16) ? 1 : 0);
  psw.C = (uint8) ((((uint32) src_val16) - dst_val16) != val16 ? 1 : 0);
}

void CPU::opcode_bit(uint16 opcode) {
  uint16 src_val16 = get_source_value(opcode);
  uint16 dst_val16 = get_destination_value(opcode);
  uint16 val16 = src_val16 & dst_val16;
  psw.N = (uint8) ((val16 & 0x7000) == 0x7000 ? 1 : 0);
  psw.Z = (uint8) (val16 == 0 ? 1 : 0);
  psw.V = (uint8) 0;
}

void CPU::opcode_bitb(uint16 opcode) {
  uint8 src_val8 = (uint8) get_source_value(opcode, true);
  uint8 dst_val8 = (uint8) get_destination_value(opcode, true);
  uint8 val8 = src_val8 & dst_val8;
  psw.N = (uint8) ((val8 & 0x70) == 0x70 ? 1 : 0);
  psw.Z = (uint8) (val8 == 0 ? 1 : 0);
  psw.V = (uint8) 0;
}

void CPU::opcode_bic(uint16 opcode) {
  uint16 src_val16 = get_source_value(opcode);
  uint16 dst_val16 = get_destination_value(opcode);
  uint16 val16 = ~src_val16 & dst_val16;
  set_destination_value(opcode, val16, false, false);
  psw.N = (uint8) ((val16 & 0x7000) == 0x7000 ? 1 : 0);
  psw.Z = (uint8) (val16 == 0 ? 1 : 0);
  psw.V = (uint8) 0;
}

void CPU::opcode_bicb(uint16 opcode) {
  uint8 src_val8 = (uint8) get_source_value(opcode, true);
  uint8 dst_val8 = (uint8) get_destination_value(opcode, true);
  uint8 val8 = ~src_val8 & dst_val8;
  set_destination_value(opcode, val8, true, false);
  psw.N = (uint8) ((val8 & 0x70) == 0x70 ? 1 : 0);
  psw.Z = (uint8) (val8 == 0 ? 1 : 0);
  psw.V = (uint8) 0;
}

void CPU::opcode_bis(uint16 opcode) {
  uint16 src_val16 = get_source_value(opcode);
  uint16 dst_val16 = get_destination_value(opcode);
  uint16 val16 = src_val16 | dst_val16;
  set_destination_value(opcode, val16, false, false);
  psw.N = (uint8) ((val16 & 0x7000) == 0x7000 ? 1 : 0);
  psw.Z = (uint8) (val16 == 0 ? 1 : 0);
  psw.V = (uint8) 0;
}

void CPU::opcode_bisb(uint16 opcode) {
  uint8 src_val8 = (uint8) get_source_value(opcode, true);
  uint8 dst_val8 = (uint8) get_destination_value(opcode, true);
  uint8 val8 = src_val8 | dst_val8;
  set_destination_value(opcode, val8, true, false);
  psw.N = (uint8) ((val8 & 0x70) == 0x70 ? 1 : 0);
  psw.Z = (uint8) (val8 == 0 ? 1 : 0);
  psw.V = (uint8) 0;
}

void CPU::opcode_xor(uint16 opcode) {
  uint16 src_val16 = (uint16) ((opcode & 0000700) >> 6);
  uint16 dst_val16 = get_destination_value(opcode);
  uint16 val16 = src_val16 ^dst_val16;
  set_destination_value(opcode, val16, false, false);
  psw.N = (uint8) (val16 < 0 ? 1 : 0);
  psw.Z = (uint8) (val16 == 0 ? 1 : 0);
  psw.V = (uint8) 0;
}
