//
// Created by Aleksandr Parfenov on 01.10.16.
//

#ifndef PDP_11_EMULATOR_CPU_H
#define PDP_11_EMULATOR_CPU_H

#include "../../common.h"

#include <chrono>

#include "../unibus/unibus.h"

const uint OPCODE_DECODING_LATENCY = 1;
const uint OPERAND_DECODING_LATENCY = 1;
const uint REGISTER_ACCESS_LATENCY = 1;

class CPU;
class PipelinedInstruction;
class CPUCache;

union PSW {
    uint16 ps;
    struct {
        uint8 C : 1, // Carry flag
            V : 1, // Overflow flag
            Z : 1, // Zero flag
            N : 1, // Sign flag
            T : 1, // Trap flag
            I : 3; // Interrupt priority level
        uint8 stub; // Most significant 8 bits not used
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
    bool has_src;
    bool has_dst;
    bool writes_result;
    bool byte_wide;
    uint8 time;
    void (CPU::*opcode_func)(PipelinedInstruction &);
};

struct InstructionOperand {
    uint8 mode;
    uint8 register_addr;
    uint16 index_offset;
    uint16 memory_address;
    bool ready;
    int calculation_time;
};

typedef enum _PIPELINE_STAGE {
    PS_FETCHING = 0,
    PS_DECODING,
    PS_FETCHING_SRC,
    PS_FETCHING_DST,
    PS_EXECUTING,
    PS_WRITE_BACK,
    PS_PIPELINE_LENGTH
} PIPELINE_STAGE;

struct PipelinedInstruction {
    uint64 id;
    uint16 address;
    uint16 opcode;
    uint16 opcode_size;
    bool stage_busy;
    int stage_ticks_left;
    CPUInstruction instruction;
    InstructionOperand src_operand;
    InstructionOperand dst_operand;
    uint16 src_value;
    uint16 dst_value;
    int8 branch_offset;
    uint8 cco_flags;
    uint8 register_index;
    uint total_time;
};

const uint16 SINGLE_OPERAND_INSTRUCTION_MASK = (const uint16) 0177700;
const uint16 DOUBLE_OPERAND_INSTRUCTION_MASK = (const uint16) 0170000;
const uint16 REGISTER_ONLY_INSTRUCTION_MASK = (const uint16) 0177770;
const uint16 REGISTER_OPERAND_INSTRUCTION_MASK = (const uint16) 0177000;
const uint16 NO_OPERANDS_INSTRUCTION_MASK = (const uint16) 0177777;
const uint16 CONDITION_CODE_INSTRUCTION_MASK = (const uint16) 0177740;
const uint16 BRANCHING_OFFSET_INSTRUCTION_MASK = (const uint16) 0177400;
const uint16 BRANCHING_OFFSET_MASK = (const uint16) 0000377;

class CPU : public UnibusDevice {
public:
    CPU();
    virtual ~CPU();
    void register_unibus(Unibus *unibus);

    string get_name() override { return "Central Processing Unit"; }
    void reset() override {}; // Ignore bus-reset, because it's caused by CPU RESET instruction

    uint16 read_word(uint18 address, uint18 base_address) override;
    void write_word(uint18 address, uint18 base_address, uint16 value) override;
    uint8 read_byte(uint18 address, uint18 base_address) override;
    void write_byte(uint18 address, uint18 base_address, uint8 value) override;

    void execute();
    void interrupt(uint18 address, int priority) override;
    bool is_busy() override { return false; }
    bool is_halted() { return _halted; }

    vector<CPUInstruction> get_instruction_set() { return _instruction_set; }
    Register get_register(int i) { return _r[i]; }
    PSW get_psw() { return _psw; }
    uint16 get_current_execution_address() { return _pipeline_stages[PS_EXECUTING].address; }

    void print_statistics();

    static InstructionOperand decode_src_operand(uint16 opcode);
    static InstructionOperand decode_dst_operand(uint16 opcode);

    static const uint18 BASE_MEM_MAP_SEGMENT_ADDRESS = 0177700;
    static const uint18 BASE_MEM_MAP_SEGMENT_SIZE = 077;
private:
    void write_back_stage();
    void execute_stage();
    void fetch_dst_stage();
    void fetch_src_stage();
    void decode_stage();
    void fetch_stage();

    void register_instruction(string mnemonic,
                              uint16 mask,
                              uint16 signature,
                              bool has_src,
                              bool has_dst,
                              bool writes_result,
                              void(CPU::*opcode_f)(PipelinedInstruction &),
                              bool byte_wide = false,
                              uint8 time = 2);

    pair<bool, uint> calculate_operand_address_pipeline(PipelinedInstruction &pipeline_stage, bool src_operand);
    void reset_pipeline_before(int stage);
    bool is_unstable_memory(uint18 address);
    bool is_unstable_register(uint register_index);
    void set_unstable_memory(uint18 address, uint64 instruction_id);
    void set_unstable_register(uint register_index, uint64 instruction_id);
    void release_unstable_memory(uint64 instruction_id);
    void release_unstable_register(uint64 instruction_id);

    pair<uint16, uint> read_memory_word(uint18 address);
    uint write_memory_word(uint18 address, uint16 value);
    pair<uint8, uint> read_memory_byte(uint18 address);
    uint write_memory_byte(uint18 address, uint8 value);

    void stack_push(uint16 value, PipelinedInstruction &instruction);
    uint16 stack_pop(PipelinedInstruction &);

    void immediate_interrupt(uint16 address, uint16 next_address);
    void check_delayed_interrupt();

    uint8 is_negative16(uint16 value) { return (uint8) (value >> 15); }
    uint8 is_negative8(uint8 value) { return (uint8) (value >> 7); }
    uint8 is_zero16(uint16 value) { return (uint8) (value == 0 ? 1 : 0); }
    uint8 is_zero8(uint8 value) { return (uint8) (value == 0 ? 1 : 0); }

    // Single Operand Instructions
    void opcode_clr(PipelinedInstruction &instruction);
    void opcode_clrb(PipelinedInstruction &instruction);
    void opcode_com(PipelinedInstruction &instruction);
    void opcode_comb(PipelinedInstruction &instruction);
    void opcode_inc(PipelinedInstruction &instruction);
    void opcode_incb(PipelinedInstruction &instruction);
    void opcode_dec(PipelinedInstruction &instruction);
    void opcode_decb(PipelinedInstruction &instruction);
    void opcode_neg(PipelinedInstruction &instruction);
    void opcode_negb(PipelinedInstruction &instruction);
    void opcode_tst(PipelinedInstruction &instruction);
    void opcode_tstb(PipelinedInstruction &instruction);
    void opcode_asr(PipelinedInstruction &instruction);
    void opcode_asrb(PipelinedInstruction &instruction);
    void opcode_asl(PipelinedInstruction &instruction);
    void opcode_aslb(PipelinedInstruction &instruction);
    void opcode_ror(PipelinedInstruction &instruction);
    void opcode_rorb(PipelinedInstruction &instruction);
    void opcode_rol(PipelinedInstruction &instruction);
    void opcode_rolb(PipelinedInstruction &instruction);
    void opcode_swab(PipelinedInstruction &instruction);
    void opcode_adc(PipelinedInstruction &instruction);
    void opcode_adcb(PipelinedInstruction &instruction);
    void opcode_sbc(PipelinedInstruction &instruction);
    void opcode_sbcb(PipelinedInstruction &instruction);
    void opcode_sxt(PipelinedInstruction &instruction);

    // Double Operand Instructions
    void opcode_mov(PipelinedInstruction &instruction);
    void opcode_movb(PipelinedInstruction &instruction);
    void opcode_cmp(PipelinedInstruction &instruction);
    void opcode_cmpb(PipelinedInstruction &instruction);
    void opcode_add(PipelinedInstruction &instruction);
    void opcode_sub(PipelinedInstruction &instruction);
    void opcode_bit(PipelinedInstruction &instruction);
    void opcode_bitb(PipelinedInstruction &instruction);
    void opcode_bic(PipelinedInstruction &instruction);
    void opcode_bicb(PipelinedInstruction &instruction);
    void opcode_bis(PipelinedInstruction &instruction);
    void opcode_bisb(PipelinedInstruction &instruction);
    void opcode_xor(PipelinedInstruction &instruction);

    // Program Control Instructions
    void opcode_br(PipelinedInstruction &instruction);
    void opcode_bne(PipelinedInstruction &instruction);
    void opcode_beq(PipelinedInstruction &instruction);
    void opcode_bpl(PipelinedInstruction &instruction);
    void opcode_bmi(PipelinedInstruction &instruction);
    void opcode_bvc(PipelinedInstruction &instruction);
    void opcode_bvs(PipelinedInstruction &instruction);
    void opcode_bcc(PipelinedInstruction &instruction);
    void opcode_bcs(PipelinedInstruction &instruction);
    void opcode_bge(PipelinedInstruction &instruction);
    void opcode_blt(PipelinedInstruction &instruction);
    void opcode_bgt(PipelinedInstruction &instruction);
    void opcode_ble(PipelinedInstruction &instruction);
    void opcode_bhi(PipelinedInstruction &instruction);
    void opcode_blos(PipelinedInstruction &instruction);
    void opcode_bhis(PipelinedInstruction &instruction);
    void opcode_blo(PipelinedInstruction &instruction);
    void opcode_jmp(PipelinedInstruction &instruction);

    // Subroutine Instructions
    void opcode_jsr(PipelinedInstruction &instruction);
    void opcode_rts(PipelinedInstruction &instruction);
    void opcode_mark(PipelinedInstruction &instruction);
    void opcode_sob(PipelinedInstruction &instruction);
    void opcode_emt(PipelinedInstruction &instruction);
    void opcode_trap(PipelinedInstruction &instruction);
    void opcode_bpt(PipelinedInstruction &instruction);
    void opcode_iot(PipelinedInstruction &instruction);
    void opcode_rti(PipelinedInstruction &instruction);
    void opcode_rtt(PipelinedInstruction &instruction);

    // Miscellaneous Instructions
    void opcode_halt(PipelinedInstruction &instruction);
    void opcode_wait(PipelinedInstruction &instruction);
    void opcode_reset(PipelinedInstruction &instruction);
    void opcode_cco(PipelinedInstruction &instruction); // Condition code operators/NOP

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
    bool _waiting;
    bool _halted;

    vector<CPUInstruction> _instruction_set;

    bool _delayed_interrupt;
    uint16 _delayed_interrupt_vector;
    uint16 _delayed_interrupt_next_address;

    uint64 _ticks;
    uint64 _intructions_processed;
    uint64 _pipeline_resets;
    uint64 _pipeline_less_ticks;

    CPUCache *_cache;
    PipelinedInstruction _pipeline_stages[PS_PIPELINE_LENGTH];
    int _memory_block;
    vector<pair<uint18, uint64>> _memory_unstable_addresses;
    vector<pair<uint, uint64>> _registers_unstable_addresses;
};

#endif //PDP_11_EMULATOR_CPU_H
