//
// Created by Aleksandr Parfenov on 01.10.16.
//

#ifndef PDP_11_EMULATOR_CPU_H
#define PDP_11_EMULATOR_CPU_H

#include "../common.h"
#include "Unibus.h"

class CPU;

struct PSW {
  uint8 I:3, // Interrupt priority level
      T:1, // Trap flag
      N:1, // Sign flag
      Z:1, // Zero flag
      V:1, // Overflow flag
      C:1; // Carry flag
};

union Register {
  uint16 r;
  struct {
    uint8 lo;
    uint8 hi;
  };
};

struct CPUInstruction {
  string mnemonic;
  uint16 opcode_mask;
  uint16 opcode_signature;
  void (CPU::*opcode_func)(uint16);
};

class CPU {
public:
  CPU(Unibus *unibus);
  void execute_command();
private:
  void register_instruction(string mnemonic, uint16 opcode_mask, uint16 opcode_signature, void(CPU::*opcode_f)(uint16));

  void set_destination_value(uint16 opcode, uint16 value, bool byte_wide = false, bool update_pointers = true);
  uint16 get_destination_value(uint16 opcode, bool byte_wide = false, bool update_pointers = true);

  // Single Operand Instructions
  void opcode_clr(uint16 opcode);
  void opcode_clrb(uint16 opcode);
  void opcode_com(uint16 opcode);
  void opcode_comb(uint16 opcode);
  void opcode_inc(uint16 opcode);
  void opcode_incb(uint16 opcode);
  void opcode_dec(uint16 opcode);
  void opcode_decb(uint16 opcode);
  void opcode_neg(uint16 opcode);
  void opcode_negb(uint16 opcode);
  void opcode_tst(uint16 opcode);
  void opcode_tstb(uint16 opcode);
  void opcode_asr(uint16 opcode);
  void opcode_asrb(uint16 opcode);
  void opcode_asl(uint16 opcode);
  void opcode_aslb(uint16 opcode);
  void opcode_ror(uint16 opcode);
  void opcode_rorb(uint16 opcode);
  void opcode_rol(uint16 opcode);
  void opcode_rolb(uint16 opcode);
  void opcode_swab(uint16 opcode);
  void opcode_adc(uint16 opcode);
  void opcode_adcb(uint16 opcode);
  void opcode_sbc(uint16 opcode);
  void opcode_sbcb(uint16 opcode);
  void opcode_sxt(uint16 opcode);

  union {
    Register r[8];
    struct {
      Register r0;
      Register r1;
      Register r2;
      Register r3;
      Register r4;
      Register r5;
      Register sp;
      Register pc;
    };
  };
  PSW psw;
  Unibus *_unibus;

  vector<CPUInstruction> cpu_instruction_set;
};

#endif //PDP_11_EMULATOR_CPU_H
