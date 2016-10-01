//
// Created by Aleksandr Parfenov on 01.10.16.
//

#include "CPU.h"
CPU::CPU(Unibus *unibus) : _unibus(unibus) {
  // TODO: Initialize PC and PSW
}

void CPU::execute_command() {
  uint16 opcode = _unibus->read_word((uint18) this->pc.r);
  uint16 val16, tmp16;
  uint8 val8, tmp8;
  uint8 sign_bit;

  switch (opcode) { // TODO: Implement PDP-11 CPU instruction set
  case 0000000: // HALT
    break;
  default: // Unknown one
    cerr << "Unknown opcode" << endl;
    break;
  }

  switch (opcode & 0777700) { // Single Operand Addressing (0xxxxDD)
  case 0005000: // CLR
    psw.N = psw.V = psw.C = 0;
    psw.Z = 1;
    set_single_value(opcode, 0);
    break;
  case 0105000: // CLRB
    psw.N = psw.V = psw.C = 0;
    psw.Z = 1;
    set_single_value(opcode, 0);
    break;
  case 0005100: // COM
    psw.V = 0;
    psw.C = 1;
    val16 = ~get_single_value(opcode);
    psw.Z = (uint8) (val16 == 0 ? 1 : 0);
    psw.N = (uint8) (val16 >> 15);
    set_single_value(opcode, val16);
    break;
  case 0105100: // COMB
    psw.V = 0;
    psw.C = 1;
    val8 = ~((uint8) get_single_value(opcode));
    psw.Z = (uint8) (val8 == 0 ? 1 : 0);
    psw.N = (uint8) (val8 >> 7);
    set_single_value(opcode, val8);
    break;
  case 0005200: // INC
    val16 = get_single_value(opcode);
    psw.V = (uint8) (val16 == 077777 ? 1 : 0);
    val16++;
    psw.Z = (uint8) (val16 == 0 ? 1 : 0);
    psw.N = (uint8) (val16 < 0 ? 1 : 0);
    set_single_value(opcode, val16);
    break;
  case 0105200: // INCB
    val8 = (uint8) get_single_value(opcode);
    psw.V = (uint8) (val8 == 077777 ? 1 : 0); // TODO: Maybe compare with 0177?
    val8++;
    psw.Z = (uint8) (val8 == 0 ? 1 : 0);
    psw.N = (uint8) (val8 < 0 ? 1 : 0);
    set_single_value(opcode, val8);
    break;
  case 0005300: // DEC
    val16 = get_single_value(opcode);
    psw.V = (uint8) (val16 == 0100000 ? 1 : 0);
    val16--;
    psw.Z = (uint8) (val16 == 0 ? 1 : 0);
    psw.N = (uint8) (val16 < 0 ? 1 : 0);
    set_single_value(opcode, val16);
    break;
  case 0105300: // DECB
    val8 = (uint8) get_single_value(opcode);
    psw.V = (uint8) (val8 == 0100000 ? 1 : 0); // TODO: Maybe compare with 0200?
    val8--;
    psw.Z = (uint8) (val8 == 0 ? 1 : 0);
    psw.N = (uint8) (val8 < 0 ? 1 : 0);
    set_single_value(opcode, val8);
    break;
  case 0005400: // NEG
    val16 = get_single_value(opcode);
    val16 = (uint16) ((val16 == 0100000) ? 0100000 : -val16);
    psw.V = (uint8) (val16 == 0100000 ? 1 : 0);
    psw.Z = (uint8) (val16 == 0 ? 1 : 0);
    psw.N = (uint8) (val16 < 0 ? 1 : 0);
    psw.C = (uint8) (val16 == 0 ? 0 : 1);
    set_single_value(opcode, val16);
    break;
  case 0105400: // NEGB
    val8 = (uint8) get_single_value(opcode);
    val8 = (uint8) ((val8 == 0100000) ? 0100000 : -val8);
    psw.V = (uint8) (val8 == 0100000 ? 1 : 0);
    psw.Z = (uint8) (val8 == 0 ? 1 : 0);
    psw.N = (uint8) (val8 < 0 ? 1 : 0);
    psw.C = (uint8) (val8 == 0 ? 0 : 1);
    set_single_value(opcode, val8);
    break;
  case 0005700: // TST
    val16 = get_single_value(opcode);
    psw.V = psw.C = 0;
    psw.Z = (uint8) (val16 == 0 ? 1 : 0);
    psw.N = (uint8) (val16 < 0 ? 1 : 0);
    set_single_value(opcode, val16);
    break;
  case 0105700: // TSTB
    val8 = (uint8) get_single_value(opcode);
    psw.V = psw.C = 0;
    psw.Z = (uint8) (val8 == 0 ? 1 : 0);
    psw.N = (uint8) (val8 < 0 ? 1 : 0);
    set_single_value(opcode, val8);
    break;
  case 0006200: // ASR
    val16 = get_single_value(opcode);
    psw.C = (uint8) (val16 & 0x1);
    sign_bit = (uint8) (val16 >> 15);
    val16 = (((uint16) sign_bit) << 15) | (val16 >> 1);
    psw.Z = (uint8) (val16 == 0 ? 1 : 0);
    psw.N = sign_bit;
    psw.V = psw.N ^ psw.C;
    set_single_value(opcode, val16);
    break;
  case 0106200: // ASRB
    val8 = (uint8) get_single_value(opcode);
    psw.C = (uint8) (val8 & 0001);
    sign_bit = (uint8) (val8 >> 7);
    val8 = (sign_bit << 7) | (val8 >> 1);
    psw.Z = (uint8) (val8 == 0 ? 1 : 0);
    psw.N = sign_bit;
    psw.V = psw.N ^ psw.C;
    set_single_value(opcode, val8);
    break;
  case 0006300: // ASL
    val16 = get_single_value(opcode);
    psw.C = (uint8) (val16 >> 15);
    val16 = val16 << 1;
    psw.Z = (uint8) (val16 == 0 ? 1 : 0);
    psw.N = (uint8) (val16 < 0 ? 1 : 0);
    psw.V = psw.N ^ psw.C;
    set_single_value(opcode, val16);
    break;
  case 0106300: // ASLB
    val8 = (uint8) get_single_value(opcode);
    psw.C = (uint8) (val8 >> 7);
    val8 = val8 << 1;
    psw.Z = (uint8) (val8 == 0 ? 1 : 0);
    psw.N = (uint8) (val8 < 0 ? 1 : 0);
    psw.V = psw.N ^ psw.C;
    set_single_value(opcode, val8);
    break;
  case 0006000: // ROR
    val16 = get_single_value(opcode);
    sign_bit = psw.C;
    psw.C = (uint8) (val16 & 0001);
    val16 = (((uint16) sign_bit) << 15) | (val16 >> 1);
    psw.Z = (uint8) (val16 == 0 ? 1 : 0);
    psw.N = (uint8) (val16 < 0 ? 1 : 0);
    psw.V = psw.N ^ psw.C;
    set_single_value(opcode, val16);
    break;
  case 0106000: // RORB
    val8 = (uint8) get_single_value(opcode);
    sign_bit = psw.C;
    psw.C = (uint8) (val8 & 0001);
    val8 = (sign_bit << 7) | (val8 >> 1);
    psw.Z = (uint8) (val8 == 0 ? 1 : 0);
    psw.N = (uint8) (val8 < 0 ? 1 : 0);
    psw.V = psw.N ^ psw.C;
    set_single_value(opcode, val8);
    break;
  case 0006100: // ROL
    val16 = get_single_value(opcode);
    sign_bit = (uint8) (val16 >> 15);
    val16 = psw.C | (val16 << 1);
    psw.C = sign_bit;
    psw.Z = (uint8) (val16 == 0 ? 1 : 0);
    psw.N = (uint8) (val16 < 0 ? 1 : 0);
    psw.V = psw.N ^ psw.C;
    set_single_value(opcode, val16);
    break;
  case 0106100: // ROLB
    val8 = (uint8) get_single_value(opcode);
    sign_bit = val8 >> 7;
    val8 = psw.C | (val8 << 1);
    psw.C = sign_bit;
    psw.Z = (uint8) (val8 == 0 ? 1 : 0);
    psw.N = (uint8) (val8 < 0 ? 1 : 0);
    psw.V = psw.N ^ psw.C;
    set_single_value(opcode, val8);
    break;
  case 0000300: // SWAB
    val16 = get_single_value(opcode);
    val8 = (uint8) (val16 & 0377);
    tmp8 = (uint8) ((val16 >> 8) & 0377);
    val16 = (((uint16) val8) << 8) | tmp8;
    psw.V = psw.C = 0;
    psw.N = (uint8) (tmp8 >> 7 == 1);
    psw.Z = (uint8) (tmp8 == 0 ? 1 : 0);
    set_single_value(opcode, val16);
    break;
  case 0005500: // ADC
    val16 = get_single_value(opcode);
    tmp8 = psw.C;
    psw.C = (uint8) ((val16 == 0177777 && tmp8 == 1) ? 1 : 0);
    psw.V = (uint8) ((val16 == 0077777 && tmp8 == 1) ? 1 : 0);
    val16 = val16 + tmp8;
    psw.N = (uint8) (val16 < 0 ? 1 : 0);
    psw.Z = (uint8) (val16 == 0 ? 1 : 0);
    set_single_value(opcode, val16);
    break;
  case 0105500: // ADCB
    val8 = (uint8) get_single_value(opcode);
    tmp8 = psw.C;
    psw.C = (uint8) ((val8 == 0177777 && tmp8 == 1) ? 1 : 0); // TODO: Maybe compare with 377?
    psw.V = (uint8) ((val8 == 0077777 && tmp8 == 1) ? 1 : 0); // TODO: Maybe compare with 177?
    val8 = val8 + tmp8;
    psw.N = (uint8) (val8 < 0 ? 1 : 0);
    psw.Z = (uint8) (val8 == 0 ? 1 : 0);
    set_single_value(opcode, val8);
    break;
  case 0005600: // SBC
    val16 = get_single_value(opcode);
    tmp8 = psw.C;
    psw.C = (uint8) ((val16 == 0000000 && tmp8 == 1) ? 1 : 0);
    psw.V = (uint8) ((val16 == 0100000) ? 1 : 0);
    val16 = val16 - tmp8;
    psw.N = (uint8) (val16 < 0 ? 1 : 0);
    psw.Z = (uint8) (val16 == 0 ? 1 : 0);
    set_single_value(opcode, val16);
    break;
  case 0105600: // SBCB
    val8 = (uint8) get_single_value(opcode);
    tmp8 = psw.C;
    psw.C = (uint8) ((val8 == 0000000 && tmp8 == 1) ? 1 : 0); // TODO: Maybe compare with 377?
    psw.V = (uint8) ((val8 == 0100000) ? 1 : 0); // TODO: Maybe compare with 177?
    val8 = val8 - tmp8;
    psw.N = (uint8) (val8 < 0 ? 1 : 0);
    psw.Z = (uint8) (val8 == 0 ? 1 : 0);
    set_single_value(opcode, val8);
    break;
  case 0006700: // SXT
    val16 = (uint16) (psw.N == 0 ? 0 : -1);
    psw.Z = (uint8) (psw.N == 0 ? 1 : 0);
    set_single_value(opcode, val16);
    break;
  }
  this->pc.r += 2;
}

void CPU::set_single_value(uint16 opcode, uint16 value) {
  uint8 mode = (uint8) (opcode & 0000070) >> 3;
  uint8 address = (uint8) (opcode & 0000007);
  bool byte_wide = (bool) ((opcode & 0100000) == 0100000);

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
    // TODO: Implement addressing mode
    break;
  case 3: // Autoincrement Deferred
    // TODO: Implement addressing mode
    break;
  case 4: // Autodecrement
    // TODO: Implement addressing mode
    break;
  case 5: // Autodecrement Deferred
    // TODO: Implement addressing mode
    break;
  case 6: // Index
    // TODO: Implement addressing mode
    break;
  case 7: // Index Deferred
    // TODO: Implement addressing mode
    break;
  }
}

uint16 CPU::get_single_value(uint16 opcode) {
  uint8 mode = (uint8) (opcode & 0000070) >> 3;
  uint8 address = (uint8) (opcode & 0000007);
  bool byte_wide = (bool) ((opcode & 0100000) == 0100000);

  switch (mode) {
  case 0: // Register
    return this->r[address].r;
  case 1: // Register Deferred
    if (byte_wide)
      return _unibus->read_byte(this->r[address].r);
    else
      return _unibus->read_word(this->r[address].r);
  case 2: // Autoincrement
    // TODO: Implement addressing mode
    break;
  case 3: // Autoincrement Deferred
    // TODO: Implement addressing mode
    break;
  case 4: // Autodecrement
    // TODO: Implement addressing mode
    break;
  case 5: // Autodecrement Deferred
    // TODO: Implement addressing mode
    break;
  case 6: // Index
    // TODO: Implement addressing mode
    break;
  case 7: // Index Deferred
    // TODO: Implement addressing mode
    break;
  }
  return 0;
}
