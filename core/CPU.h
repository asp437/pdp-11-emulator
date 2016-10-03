//
// Created by Aleksandr Parfenov on 01.10.16.
//

#ifndef PDP_11_EMULATOR_CPU_H
#define PDP_11_EMULATOR_CPU_H

#include "../common.h"
#include "Unibus.h"

class CPU;

union PSW {
  uint16 ps;
  struct {
    uint8 stub; // Most significant 8 bits not used
    uint8 I:3, // Interrupt priority level
        T:1, // Trap flag
        N:1, // Sign flag
        Z:1, // Zero flag
        V:1, // Overflow flag
        C:1; // Carry flag
  };
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

class CPU : public UnibusDevice {
public:
  CPU();
  virtual ~CPU();

  string get_name() override;
  void register_unibus(Unibus *unibus) override;
  void reset() override;
  uint16 read_word(uint18 address, uint18 base_address) override;
  void write_word(uint18 address, uint18 base_address, uint16 value) override;
  uint8 read_byte(uint18 address, uint18 base_address) override;
  void write_byte(uint18 address, uint18 base_address, uint8 value) override;
  void execute();
  void interrupt(uint18 address) override;

  static const uint18 BASE_MEM_MAP_SEGMENT_ADDRESS = 0177700;
  static const uint18 BASE_MEM_MAP_SEGMENT_SIZE = 0177700;
private:
  void register_instruction(string mnemonic, uint16 opcode_mask, uint16 opcode_signature, void(CPU::*opcode_f)(uint16));

  void set_value(uint8 mode, uint8 address, uint16 value, bool byte_wide, bool update_pointers);
  uint16 get_value(uint8 mode, uint8 address, bool byte_wide, bool update_pointers);
  void set_destination_value(uint16 opcode, uint16 value, bool byte_wide = false, bool update_pointers = true);
  uint16 get_destination_value(uint16 opcode, bool byte_wide = false, bool update_pointers = true);
  void set_source_value(uint16 opcode, uint16 value, bool byte_wide = false, bool update_pointers = true);
  uint16 get_source_value(uint16 opcode, bool byte_wide = false, bool update_pointers = true);

  void stack_push(uint16 value);
  uint16 stack_pop();

  uint8 is_negative16(uint16 value) { return (uint8) (value >> 15); }
  uint8 is_negative8(uint8 value) { return (uint8) (value >> 7); }
  uint8 is_zero16(uint16 value) { return (uint8) (value == 0 ? 1 : 0); }
  uint8 is_zero8(uint8 value) { return (uint8) (value == 0 ? 1 : 0); }

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

  // Double Operand Instructions
  void opcode_mov(uint16 opcode);
  void opcode_movb(uint16 opcode);
  void opcode_cmp(uint16 opcode);
  void opcode_cmpb(uint16 opcode);
  void opcode_add(uint16 opcode);
  void opcode_sub(uint16 opcode);
  void opcode_bit(uint16 opcode);
  void opcode_bitb(uint16 opcode);
  void opcode_bic(uint16 opcode);
  void opcode_bicb(uint16 opcode);
  void opcode_bis(uint16 opcode);
  void opcode_bisb(uint16 opcode);
  void opcode_xor(uint16 opcode);

  // Program Control Instructions
  void opcode_br(uint16 opcode);
  void opcode_bne(uint16 opcode);
  void opcode_beq(uint16 opcode);
  void opcode_bpl(uint16 opcode);
  void opcode_bmi(uint16 opcode);
  void opcode_bvc(uint16 opcode);
  void opcode_bvs(uint16 opcode);
  void opcode_bcc(uint16 opcode);
  void opcode_bcs(uint16 opcode);
  void opcode_bge(uint16 opcode);
  void opcode_blt(uint16 opcode);
  void opcode_bgt(uint16 opcode);
  void opcode_ble(uint16 opcode);
  void opcode_bhi(uint16 opcode);
  void opcode_blos(uint16 opcode);
  void opcode_bhis(uint16 opcode);
  void opcode_blo(uint16 opcode);
  void opcode_jmp(uint16 opcode);

  // Subroutine Instructions
  void opcode_jsr(uint16 opcode);
  void opcode_rts(uint16 opcode);
  void opcode_mark(uint16 opcode);
  void opcode_sob(uint16 opcode);
  void opcode_emt(uint16 opcode);
  void opcode_trap(uint16 opcode);
  void opcode_bpt(uint16 opcode);
  void opcode_iot(uint16 opcode);
  void opcode_rti(uint16 opcode);
  void opcode_rtt(uint16 opcode);

  // Miscellaneous Instructions
  void opcode_halt(uint16 opcode);
  void opcode_wait(uint16 opcode);
  void opcode_reset(uint16 opcode);
  void opcode_mfpi(uint16 opcode);
  void opcode_mtpi(uint16 opcode);
  void opcode_cco(uint16 opcode); // Condition code operators/NOP

  union {
    Register _r[8];
    struct {
      Register _r0;
      Register _r1;
      Register _r2;
      Register _r3;
      Register _r4;
      Register _r5;
      Register _sp;
      Register _pc;
    };
  };
  PSW _psw;
  Unibus *_unibus;
  int _pc_step; // In bytes
  bool _waiting;

  vector<CPUInstruction> _instruction_set;
};

#endif //PDP_11_EMULATOR_CPU_H
