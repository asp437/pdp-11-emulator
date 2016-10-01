//
// Created by Aleksandr Parfenov on 01.10.16.
//

#ifndef PDP_11_EMULATOR_CPU_H
#define PDP_11_EMULATOR_CPU_H

#include "../common.h"
#include "Unibus.h"

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

class CPU {
public:
  CPU(Unibus *unibus);
  void execute_command();
private:
  void set_single_value(uint16 opcode, uint16 value, bool update_pointers = true);
  uint16 get_single_value(uint16 opcode, bool update_pointers = true);

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
};

#endif //PDP_11_EMULATOR_CPU_H
