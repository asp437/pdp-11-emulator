//
// Created by Aleksandr Parfenov on 01.10.16.
//

#include "cpu.h"
#include "cpu_cache.h"

#include <iomanip>
#include <cstring>

CPU::CPU() {
    // Single Operand Instructions
    register_instruction("CLR", SINGLE_OPERAND_INSTRUCTION_MASK, 0005000, false, true, true, &CPU::opcode_clr, false, 1);
    register_instruction("CLRB", SINGLE_OPERAND_INSTRUCTION_MASK, 0105000, false, true, true, &CPU::opcode_clrb, true, 1);
    register_instruction("COM", SINGLE_OPERAND_INSTRUCTION_MASK, 0005100, false, true, true, &CPU::opcode_com, false, 1);
    register_instruction("COMB", SINGLE_OPERAND_INSTRUCTION_MASK, 0105100, false, true, true, &CPU::opcode_comb, true, 1);
    register_instruction("INC", SINGLE_OPERAND_INSTRUCTION_MASK, 0005200, false, true, true, &CPU::opcode_inc, false, 1);
    register_instruction("INCB", SINGLE_OPERAND_INSTRUCTION_MASK, 0105200, false, true, true, &CPU::opcode_incb, true, 1);
    register_instruction("DEC", SINGLE_OPERAND_INSTRUCTION_MASK, 0005300, false, true, true, &CPU::opcode_dec, false, 1);
    register_instruction("DECB", SINGLE_OPERAND_INSTRUCTION_MASK, 0105300, false, true, true, &CPU::opcode_decb, true, 1);
    register_instruction("NEG", SINGLE_OPERAND_INSTRUCTION_MASK, 0005400, false, true, true, &CPU::opcode_neg, false, 1);
    register_instruction("NEGB", SINGLE_OPERAND_INSTRUCTION_MASK, 0105400, false, true, true, &CPU::opcode_negb, true, 1);
    register_instruction("TST", SINGLE_OPERAND_INSTRUCTION_MASK, 0005700, false, true, false, &CPU::opcode_tst, false, 1);
    register_instruction("TSTB", SINGLE_OPERAND_INSTRUCTION_MASK, 0105700, false, true, false, &CPU::opcode_tstb, true, 1);
    register_instruction("ASR", SINGLE_OPERAND_INSTRUCTION_MASK, 0006200, false, true, true, &CPU::opcode_asr, false, 2);
    register_instruction("ASRB", SINGLE_OPERAND_INSTRUCTION_MASK, 0106200, false, true, true, &CPU::opcode_asrb, true, 2);
    register_instruction("ASL", SINGLE_OPERAND_INSTRUCTION_MASK, 0006300, false, true, true, &CPU::opcode_asl, false, 2);
    register_instruction("ASLB", SINGLE_OPERAND_INSTRUCTION_MASK, 0106300, false, true, true, &CPU::opcode_aslb, true, 2);
    register_instruction("ROR", SINGLE_OPERAND_INSTRUCTION_MASK, 0006000, false, true, true, &CPU::opcode_ror, false, 2);
    register_instruction("RORB", SINGLE_OPERAND_INSTRUCTION_MASK, 0106000, false, true, true, &CPU::opcode_rorb, true, 2);
    register_instruction("ROL", SINGLE_OPERAND_INSTRUCTION_MASK, 0006100, false, true, true, &CPU::opcode_rol, false, 2);
    register_instruction("ROLB", SINGLE_OPERAND_INSTRUCTION_MASK, 0106100, false, true, true, &CPU::opcode_rolb, true, 2);
    register_instruction("SWAB", SINGLE_OPERAND_INSTRUCTION_MASK, 0000300, false, true, true, &CPU::opcode_swab, false, 2);
    register_instruction("ADC", SINGLE_OPERAND_INSTRUCTION_MASK, 0005500, false, true, true, &CPU::opcode_adc, false, 2);
    register_instruction("ADCB", SINGLE_OPERAND_INSTRUCTION_MASK, 0105500, false, true, true, &CPU::opcode_adcb, true, 2);
    register_instruction("SBC", SINGLE_OPERAND_INSTRUCTION_MASK, 0005600, false, true, true, &CPU::opcode_sbc, false, 2);
    register_instruction("SBCB", SINGLE_OPERAND_INSTRUCTION_MASK, 0105600, false, true, true, &CPU::opcode_sbcb, true, 2);
    register_instruction("SXT", SINGLE_OPERAND_INSTRUCTION_MASK, 0006700, false, true, true, &CPU::opcode_sxt, false, 2);

    // Double Operand Instructions
    register_instruction("MOV", DOUBLE_OPERAND_INSTRUCTION_MASK, 0010000, true, true, true, &CPU::opcode_mov, false, 2);
    register_instruction("MOVB", DOUBLE_OPERAND_INSTRUCTION_MASK, 0110000, true, true, true, &CPU::opcode_movb, true, 2);
    register_instruction("CMP", DOUBLE_OPERAND_INSTRUCTION_MASK, 0020000, true, true, false, &CPU::opcode_cmp, false, 2);
    register_instruction("CMPB", DOUBLE_OPERAND_INSTRUCTION_MASK, 0120000, true, true, false, &CPU::opcode_cmpb, true, 2);
    register_instruction("ADD", DOUBLE_OPERAND_INSTRUCTION_MASK, 0060000, true, true, true, &CPU::opcode_add, false, 3);
    register_instruction("SUB", DOUBLE_OPERAND_INSTRUCTION_MASK, 0160000, true, true, true, &CPU::opcode_sub, false, 3);
    register_instruction("BIT", DOUBLE_OPERAND_INSTRUCTION_MASK, 0030000, true, true, false, &CPU::opcode_bit, false, 2);
    register_instruction("BITB", DOUBLE_OPERAND_INSTRUCTION_MASK, 0130000, true, true, false, &CPU::opcode_bitb, true, 2);
    register_instruction("BIC", DOUBLE_OPERAND_INSTRUCTION_MASK, 0040000, true, true, true, &CPU::opcode_bic, false, 3);
    register_instruction("BICB", DOUBLE_OPERAND_INSTRUCTION_MASK, 0140000, true, true, true, &CPU::opcode_bicb, true, 3);
    register_instruction("BIS", DOUBLE_OPERAND_INSTRUCTION_MASK, 0050000, true, true, true, &CPU::opcode_bis, false, 3);
    register_instruction("BISB", DOUBLE_OPERAND_INSTRUCTION_MASK, 0150000, true, true, true, &CPU::opcode_bisb, true, 3);
    register_instruction("XOR", REGISTER_OPERAND_INSTRUCTION_MASK, 0074000, false, true, true, &CPU::opcode_xor, false, 3);

    // Program Control Instructions
    register_instruction("BR", BRANCHING_OFFSET_INSTRUCTION_MASK, 0000400, false, false, false, &CPU::opcode_br, false, 2);
    register_instruction("BNE", BRANCHING_OFFSET_INSTRUCTION_MASK, 0001000, false, false, false, &CPU::opcode_bne, false, 1);
    register_instruction("BEQ", BRANCHING_OFFSET_INSTRUCTION_MASK, 0001400, false, false, false, &CPU::opcode_beq, false, 1);
    register_instruction("BPL", BRANCHING_OFFSET_INSTRUCTION_MASK, 0100000, false, false, false, &CPU::opcode_bpl, false, 1);
    register_instruction("BMI", BRANCHING_OFFSET_INSTRUCTION_MASK, 0100400, false, false, false, &CPU::opcode_bmi, false, 1);
    register_instruction("BVC", BRANCHING_OFFSET_INSTRUCTION_MASK, 0102000, false, false, false, &CPU::opcode_bvc, false, 1);
    register_instruction("BVS", BRANCHING_OFFSET_INSTRUCTION_MASK, 0102400, false, false, false, &CPU::opcode_bvs, false, 1);
    register_instruction("BCC", BRANCHING_OFFSET_INSTRUCTION_MASK, 0103000, false, false, false, &CPU::opcode_bcc, false, 1);
    register_instruction("BCS", BRANCHING_OFFSET_INSTRUCTION_MASK, 0103400, false, false, false, &CPU::opcode_bcs, false, 1);
    register_instruction("BGE", BRANCHING_OFFSET_INSTRUCTION_MASK, 0002000, false, false, false, &CPU::opcode_bge, false, 1);
    register_instruction("BLT", BRANCHING_OFFSET_INSTRUCTION_MASK, 0002400, false, false, false, &CPU::opcode_blt, false, 1);
    register_instruction("BGT", BRANCHING_OFFSET_INSTRUCTION_MASK, 0003000, false, false, false, &CPU::opcode_bgt, false, 1);
    register_instruction("BLE", BRANCHING_OFFSET_INSTRUCTION_MASK, 0003400, false, false, false, &CPU::opcode_ble, false, 1);
    register_instruction("BHI", BRANCHING_OFFSET_INSTRUCTION_MASK, 0101000, false, false, false, &CPU::opcode_bhi, false, 1);
    register_instruction("BLOS", BRANCHING_OFFSET_INSTRUCTION_MASK, 0101400, false, false, false, &CPU::opcode_blos, false, 1);
    register_instruction("BHIS", BRANCHING_OFFSET_INSTRUCTION_MASK, 0103000, false, false, false, &CPU::opcode_bhis, false, 1);
    register_instruction("BLO", BRANCHING_OFFSET_INSTRUCTION_MASK, 0103400, false, false, false, &CPU::opcode_blo, false, 1);
    register_instruction("JMP", SINGLE_OPERAND_INSTRUCTION_MASK, 0000100, false, true, false, &CPU::opcode_jmp, false, 1);

    // Subroutine Instructions
    register_instruction("JSR", REGISTER_OPERAND_INSTRUCTION_MASK, 0004000, false, true, false, &CPU::opcode_jsr, false, 3);
    register_instruction("RTS", REGISTER_ONLY_INSTRUCTION_MASK, 0000200, false, false, false, &CPU::opcode_rts, false, 4);
//    register_instruction("MARK", SINGLE_OPERAND_INSTRUCTION_MASK, 0006400, false, false, false, &CPU::opcode_mark, false, 2);
    register_instruction("SOB", REGISTER_OPERAND_INSTRUCTION_MASK, 0077000, false, false, false, &CPU::opcode_sob, false, 3);
    register_instruction("EMT", BRANCHING_OFFSET_INSTRUCTION_MASK, 0104000, false, false, false, &CPU::opcode_emt, false, 3);
    register_instruction("TRAP", BRANCHING_OFFSET_INSTRUCTION_MASK, 0104400, false, false, false, &CPU::opcode_trap, false, 3);
    register_instruction("BPT", NO_OPERANDS_INSTRUCTION_MASK, 0000003, false, false, false, &CPU::opcode_bpt, false, 3);
    register_instruction("IOT", NO_OPERANDS_INSTRUCTION_MASK, 0000004, false, false, false, &CPU::opcode_iot, false, 3);
    register_instruction("RTI", NO_OPERANDS_INSTRUCTION_MASK, 0000002, false, false, false, &CPU::opcode_rti, false, 5);
    register_instruction("RTT", NO_OPERANDS_INSTRUCTION_MASK, 0000006, false, false, false, &CPU::opcode_rtt, false, 5);

    // Miscellaneous Instructions
    register_instruction("HALT", NO_OPERANDS_INSTRUCTION_MASK, 0000000, false, false, false, &CPU::opcode_halt, false, 1);
    register_instruction("WAIT", NO_OPERANDS_INSTRUCTION_MASK, 0000001, false, false, false, &CPU::opcode_wait, false, 1);
    register_instruction("RESET", NO_OPERANDS_INSTRUCTION_MASK, 0000005, false, false, false, &CPU::opcode_reset, false, 1);
    register_instruction("CCO", CONDITION_CODE_INSTRUCTION_MASK, 0000240, false, false, false, &CPU::opcode_cco, false, 2);

    _waiting = false;
    _halted = false;
    memset(_r, 0, sizeof(_r));
    _psw.ps = 0;
    _r[6].r = 16 * 1024;
    _pc.r = 0140000;

    _delayed_interrupt = false;
    _delayed_interrupt_vector = 0;

    _ticks = 0;
    _intructions_processed = 0;
    _pipeline_resets = 0;
    _pipeline_less_ticks = 0;

    _cache = new CPUCache();
    _memory_block = PS_PIPELINE_LENGTH;
    for (int i = 0; i < PS_PIPELINE_LENGTH; i++) {
        _pipeline_stages[i].stage_ticks_left = 0;
        _pipeline_stages[i].stage_busy = false;
    }
    _pipeline_stages[PS_FETCHING].stage_busy = true;

    cout << "CPU initialization complete. Totally registered " << _instruction_set.size() << " instructions." << endl;
}

CPU::~CPU() {
    _instruction_set.clear();
    _unibus = nullptr;
    memset(_r, 0, sizeof(_r));
    delete _cache;
}

void CPU::register_unibus(Unibus *unibus) {
    this->_cache->register_unibus(unibus);
    this->_unibus = unibus;
};

uint16 CPU::read_word(uint18 address, uint18 base_address) {
    if (address >= BASE_MEM_MAP_SEGMENT_ADDRESS && address < BASE_MEM_MAP_SEGMENT_ADDRESS + 020) {
        uint8 reg_n = (uint8) ((address - 0177700) >> 1);
        return _r[reg_n].r;
    } else if (address == 0177776)
        return _psw.ps;
    else
        throw new runtime_error("Wrong memory address inside CPU Memory Mapping segment");
}

void CPU::write_word(uint18 address, uint18 base_address, uint16 value) {
    if (address >= 0177700 && address < 0177720) {
        uint8 reg_n = (uint8) ((address - 0177700) >> 1);
        _r[reg_n].r = value;
    } else if (address == 0177776)
        _psw.ps = value;
    else
        throw new runtime_error("Wrong memory address inside CPU Memory Mapping segment");
}

uint8 CPU::read_byte(uint18 address, uint18 base_address) {
    throw new runtime_error("CPU doesn't support byte reading");
}

void CPU::write_byte(uint18 address, uint18 base_address, uint8 value) {
    throw new runtime_error("CPU doesn't support byte writing");
}

void CPU::register_instruction(string mnemonic,
                               uint16 mask,
                               uint16 signature,
                               bool has_src,
                               bool has_dst,
                               bool writes_result,
                               void (CPU::*opcode_f)(PipelinedInstruction &),
                               bool byte_wide,
                               uint8 time) {
    CPUInstruction instruction;
    instruction.mnemonic = mnemonic;
    instruction.opcode_mask = mask;
    instruction.opcode_signature = signature;
    instruction.opcode_func = opcode_f;
    instruction.time = time;
    instruction.has_src = has_src;
    instruction.has_dst = has_dst;
    instruction.writes_result = writes_result;
    instruction.byte_wide = byte_wide;
    _instruction_set.push_back(instruction);

    ios_base::fmtflags cout_flags = cout.flags();
    cout << "CPU Instruction " << setw(5) << mnemonic << " (OPCODE SIGNATURE " << oct << setfill('0') << setw(7)
         << signature << ") registered." << endl;
    cout << setfill(' ') << setw(0);
    cout.flags(cout_flags);
}

InstructionOperand CPU::decode_src_operand(uint16 opcode) {
    InstructionOperand operand;
    operand.mode = (uint8) ((opcode & 0007000) >> 9);
    operand.register_addr = (uint8) ((opcode & 0000700) >> 6);
    operand.index_offset = 2;
    operand.ready = false;
    return operand;
}

InstructionOperand CPU::decode_dst_operand(uint16 opcode) {
    InstructionOperand operand;
    operand.mode = ((uint8) (opcode & 0000070)) >> 3;
    operand.register_addr = (uint8) (opcode & 0000007);

    uint8 src_mode = (uint8) ((opcode & 0007000) >> 9);
    bool src_follow_instr = ((opcode & 0070000) != 0 && src_mode >= 6);
    src_follow_instr |= ((opcode & 0000700) == 0000700) & (src_mode == 2 || src_mode == 3);

    operand.index_offset = (uint16) (src_follow_instr ? 4 : 2); // Double Operand Instructions check
    operand.ready = false;
    return operand;
}

pair<uint16, uint> CPU::read_memory_word(uint18 address) {
    return _cache->read_word(address);
}

uint CPU::write_memory_word(uint18 address, uint16 value) {
    return _cache->write_word(address, value);
}

pair<uint8, uint> CPU::read_memory_byte(uint18 address) {
    return _cache->read_byte(address);
}

uint CPU::write_memory_byte(uint18 address, uint8 value) {
    return _cache->write_byte(address, value);
}

void CPU::stack_push(uint16 value, PipelinedInstruction &instruction) {
    // TODO: Check race condition
    _sp.r -= 2;
    instruction.stage_ticks_left += write_memory_word(_sp.r, value);
}

uint16 CPU::stack_pop(PipelinedInstruction &instruction) {
    // TODO: Check race condition
    pair<uint16, uint> mem_read_result = read_memory_word(_sp.r);
    instruction.stage_ticks_left += mem_read_result.second;
    _sp.r += 2;
    return mem_read_result.first;
}

void CPU::execute() {
    if (_halted)
        return;

    write_back_stage();
    execute_stage();

    if (_waiting)
        return;

    fetch_dst_stage();
    fetch_src_stage();
    decode_stage();
    fetch_stage();
    check_delayed_interrupt();
    _ticks++;
}

void CPU::interrupt(uint18 address, int priority) {
    if (_psw.I > priority)
        return;

    // Immediately handle interrupt if CPU is waiting, and all previous operations completed
    if (_waiting && !_pipeline_stages[PS_EXECUTING].stage_busy && !_pipeline_stages[PS_WRITE_BACK].stage_busy) {
        _waiting = false;
        // Waiting state possible only after WAIT instruction
        // WAIT instruction blocks executing of next instruction
        // Therefore, PS_EXECUTING stores information about last executed instruction, the WAIT instruction
        immediate_interrupt((uint16) address,
                            _pipeline_stages[PS_EXECUTING].address + _pipeline_stages[PS_EXECUTING].opcode_size);
    } else {
        _delayed_interrupt = true;
        _delayed_interrupt_vector = (uint16) address;
        _delayed_interrupt_next_address = _pipeline_stages[PS_EXECUTING].address + _pipeline_stages[PS_EXECUTING].opcode_size;

        // Reset and temporary disable all stages, except PS_EXECUTING and PS_WRITE_BACK
        for (int stage = PS_FETCHING; stage < PS_EXECUTING; stage++)
            _pipeline_stages[stage].stage_busy = false;
    }
}

void CPU::immediate_interrupt(uint16 address, uint16 next_address) {
    // Method called only if all stages are disabled. There is no need to manually reset pipeline.
    _sp.r -= 2;
    write_memory_word(_sp.r, _psw.ps);
    _sp.r -= 2;
    write_memory_word(_sp.r, next_address);
    reset_pipeline_before(PS_EXECUTING);
    _pc.r = read_memory_word(address).first;
    _psw.ps = (uint8) read_memory_word(address + 2).first;
    _pipeline_resets++;
}

void CPU::check_delayed_interrupt() {
    if (_delayed_interrupt && !_pipeline_stages[PS_EXECUTING].stage_busy && !_pipeline_stages[PS_WRITE_BACK].stage_busy) {
        immediate_interrupt(_delayed_interrupt_vector, _delayed_interrupt_next_address);
        _delayed_interrupt = false;
        _pipeline_stages[PS_FETCHING].stage_busy = true; // Enable instruction fetching
    }
}

void CPU::opcode_clr(PipelinedInstruction &instruction) {
    _psw.N = _psw.V = _psw.C = 0;
    _psw.Z = 1;
    instruction.dst_value = 0;
}

void CPU::opcode_clrb(PipelinedInstruction &instruction) {
    _psw.N = _psw.V = _psw.C = 0;
    _psw.Z = 1;
    instruction.dst_value = 0;
}

void CPU::opcode_com(PipelinedInstruction &instruction) {
    _psw.V = 0;
    _psw.C = 1;
    instruction.dst_value = ~instruction.dst_value;
    _psw.Z = is_zero16(instruction.dst_value);
    _psw.N = is_negative16(instruction.dst_value);
}

void CPU::opcode_comb(PipelinedInstruction &instruction) {
    _psw.V = 0;
    _psw.C = 1;
    instruction.dst_value = (uint8) ~instruction.dst_value;
    _psw.Z = is_zero8((uint8) instruction.dst_value);
    _psw.N = is_negative8((uint8) instruction.dst_value);
}

void CPU::opcode_inc(PipelinedInstruction &instruction) {
    _psw.V = (uint8) (instruction.dst_value == 077777 ? 1 : 0);
    instruction.dst_value++;
    _psw.Z = is_zero16(instruction.dst_value);
    _psw.N = is_negative16(instruction.dst_value);
}

void CPU::opcode_incb(PipelinedInstruction &instruction) {
    uint8 val8 = (uint8) instruction.dst_value;
    _psw.V = (uint8) (val8 == 0177 ? 1 : 0);
    val8++;
    _psw.Z = is_zero8(val8);
    _psw.N = is_negative8(val8);
    instruction.dst_value = val8;
}

void CPU::opcode_dec(PipelinedInstruction &instruction) {
    _psw.V = (uint8) (instruction.dst_value == 0100000 ? 1 : 0);
    instruction.dst_value--;
    _psw.Z = is_zero16(instruction.dst_value);
    _psw.N = is_negative16(instruction.dst_value);
}

void CPU::opcode_decb(PipelinedInstruction &instruction) {
    uint8 val8 = (uint8) instruction.dst_value;
    _psw.V = (uint8) (val8 == 0200 ? 1 : 0);
    val8--;
    _psw.Z = is_zero8(val8);
    _psw.N = is_negative8(val8);
    instruction.dst_value = val8;
}

void CPU::opcode_neg(PipelinedInstruction &instruction) {
    instruction.dst_value = (uint16) ((instruction.dst_value == 0100000) ? 0100000 : -instruction.dst_value);
    _psw.V = (uint8) (instruction.dst_value == 0100000 ? 1 : 0);
    _psw.Z = is_zero16(instruction.dst_value);
    _psw.N = is_negative16(instruction.dst_value);
    _psw.C = (uint8) (instruction.dst_value == 0 ? 0 : 1);
}

void CPU::opcode_negb(PipelinedInstruction &instruction) {
    uint8 val8 = (uint8) instruction.dst_value;
    val8 = (uint8) ((val8 == 0100000) ? 0100000 : -val8);
    _psw.V = (uint8) (val8 == 0100000 ? 1 : 0);
    _psw.Z = is_zero8(val8);
    _psw.N = is_negative8(val8);
    _psw.C = (uint8) (val8 == 0 ? 0 : 1);
    instruction.dst_value = val8;
}

void CPU::opcode_tst(PipelinedInstruction &instruction) {
    _psw.V = _psw.C = 0;
    _psw.Z = is_zero16(instruction.dst_value);
    _psw.N = is_negative16(instruction.dst_value);
}

void CPU::opcode_tstb(PipelinedInstruction &instruction) {
    uint8 val8 = (uint8) instruction.dst_value;
    _psw.V = _psw.C = 0;
    _psw.Z = is_zero8(val8);
    _psw.N = is_negative8(val8);
}

void CPU::opcode_asr(PipelinedInstruction &instruction) {
    _psw.C = (uint8) (instruction.dst_value & 0x1);
    uint8 sign_bit = (uint8) (instruction.dst_value >> 15);
    instruction.dst_value = (((uint16) sign_bit) << 15) | (instruction.dst_value >> 1);
    _psw.Z = is_zero16(instruction.dst_value);
    _psw.N = sign_bit;
    _psw.V = _psw.N ^ _psw.C;
}

void CPU::opcode_asrb(PipelinedInstruction &instruction) {
    uint8 val8 = (uint8) instruction.dst_value;
    _psw.C = (uint8) (val8 & 0001);
    uint8 sign_bit = (uint8) (val8 >> 7);
    val8 = (sign_bit << 7) | (val8 >> 1);
    _psw.Z = is_zero8(val8);
    _psw.N = sign_bit;
    _psw.V = _psw.N ^ _psw.C;
    instruction.dst_value = val8;
}

void CPU::opcode_asl(PipelinedInstruction &instruction) {
    _psw.C = (uint8) (instruction.dst_value >> 15);
    instruction.dst_value = instruction.dst_value << 1;
    _psw.Z = is_zero16(instruction.dst_value);
    _psw.N = is_negative16(instruction.dst_value);
    _psw.V = _psw.N ^ _psw.C;
}

void CPU::opcode_aslb(PipelinedInstruction &instruction) {
    uint8 val8 = (uint8) instruction.dst_value;
    _psw.C = (uint8) (val8 >> 7);
    val8 = val8 << 1;
    _psw.Z = is_zero8(val8);
    _psw.N = is_negative8(val8);
    _psw.V = _psw.N ^ _psw.C;
    instruction.dst_value = val8;
}

void CPU::opcode_ror(PipelinedInstruction &instruction) {
    uint8 sign_bit = _psw.C;
    _psw.C = (uint8) (instruction.dst_value & 0001);
    instruction.dst_value = (((uint16) sign_bit) << 15) | (instruction.dst_value >> 1);
    _psw.Z = is_zero16(instruction.dst_value);
    _psw.N = is_negative16(instruction.dst_value);
    _psw.V = _psw.N ^ _psw.C;
}

void CPU::opcode_rorb(PipelinedInstruction &instruction) {
    uint8 val8 = (uint8) instruction.dst_value;
    uint8 sign_bit = _psw.C;
    _psw.C = (uint8) (val8 & 0001);
    val8 = (sign_bit << 7) | (val8 >> 1);
    _psw.Z = is_zero8(val8);
    _psw.N = is_negative8(val8);
    _psw.V = _psw.N ^ _psw.C;
    instruction.dst_value = val8;
}

void CPU::opcode_rol(PipelinedInstruction &instruction) {
    uint8 sign_bit = (uint8) (instruction.dst_value >> 15);
    instruction.dst_value = _psw.C | (instruction.dst_value << 1);
    _psw.C = sign_bit;
    _psw.Z = is_zero16(instruction.dst_value);
    _psw.N = is_negative16(instruction.dst_value);
    _psw.V = _psw.N ^ _psw.C;
}

void CPU::opcode_rolb(PipelinedInstruction &instruction) {
    uint8 val8 = (uint8) instruction.dst_value;
    uint8 sign_bit = val8 >> 7;
    val8 = _psw.C | (val8 << 1);
    _psw.C = sign_bit;
    _psw.Z = is_zero8(val8);
    _psw.N = is_negative8(val8);
    _psw.V = _psw.N ^ _psw.C;
    instruction.dst_value = val8;
}

void CPU::opcode_swab(PipelinedInstruction &instruction) {
    uint8 val8 = (uint8) (instruction.dst_value & 0377);
    uint8 tmp8 = (uint8) ((instruction.dst_value >> 8) & 0377);
    instruction.dst_value = (((uint16) val8) << 8) | tmp8;
    _psw.V = _psw.C = 0;
    _psw.N = is_negative8(tmp8);
    _psw.Z = is_zero8(tmp8);
}

void CPU::opcode_adc(PipelinedInstruction &instruction) {
    uint8 tmp8 = _psw.C;
    _psw.C = (uint8) ((instruction.dst_value == 0177777 && tmp8 == 1) ? 1 : 0);
    _psw.V = (uint8) ((instruction.dst_value == 0077777 && tmp8 == 1) ? 1 : 0);
    instruction.dst_value = instruction.dst_value + tmp8;
    _psw.N = is_negative16(instruction.dst_value);
    _psw.Z = is_zero16(instruction.dst_value);
}

void CPU::opcode_adcb(PipelinedInstruction &instruction) {
    uint8 val8 = (uint8) instruction.dst_value;
    uint8 tmp8 = _psw.C;
    _psw.C = (uint8) ((val8 == 0377 && tmp8 == 1) ? 1 : 0);
    _psw.V = (uint8) ((val8 == 0177 && tmp8 == 1) ? 1 : 0);
    val8 = val8 + tmp8;
    _psw.N = is_negative8(val8);
    _psw.Z = is_zero8(val8);
    instruction.dst_value = val8;
}

void CPU::opcode_sbc(PipelinedInstruction &instruction) {
    uint8 tmp8 = _psw.C;
    _psw.C = (uint8) ((instruction.dst_value == 0000000 && tmp8 == 1) ? 1 : 0);
    _psw.V = (uint8) ((instruction.dst_value == 0100000) ? 1 : 0);
    instruction.dst_value = instruction.dst_value - tmp8;
    _psw.N = is_negative16(instruction.dst_value);
    _psw.Z = is_zero16(instruction.dst_value);
}

void CPU::opcode_sbcb(PipelinedInstruction &instruction) {
    uint8 val8 = (uint8) instruction.dst_value;
    uint8 tmp8 = _psw.C;
    _psw.C = (uint8) ((val8 == 0377 && tmp8 == 1) ? 1 : 0);
    _psw.V = (uint8) ((val8 == 0177) ? 1 : 0);
    val8 = val8 - tmp8;
    _psw.N = is_negative8(val8);
    _psw.Z = is_zero8(val8);
    instruction.dst_value = val8;
}

void CPU::opcode_sxt(PipelinedInstruction &instruction) {
    instruction.dst_value = (uint16) (_psw.N == 0 ? 0 : -1);
    _psw.Z = is_zero8(_psw.N);
}

void CPU::opcode_mov(PipelinedInstruction &instruction) {
    instruction.dst_value = instruction.src_value;
    _psw.N = is_negative16(instruction.dst_value);
    _psw.Z = is_zero16(instruction.dst_value);
    _psw.V = 0;
}

void CPU::opcode_movb(PipelinedInstruction &instruction) {
    instruction.dst_value = (uint8) instruction.src_value;
    _psw.N = is_negative8((uint8) instruction.dst_value);
    _psw.Z = is_zero8((uint8) instruction.dst_value);
    _psw.V = 0;
}

void CPU::opcode_cmp(PipelinedInstruction &instruction) {
    uint16 val16 = instruction.src_value - instruction.dst_value;
    _psw.N = is_negative16(val16);
    _psw.Z = is_zero16(val16);
    _psw.V = (uint8) (is_negative16(instruction.src_value) != is_negative16(instruction.dst_value)
                          && is_negative16(val16) == is_negative16(instruction.dst_value) ? 1 : 0);
    _psw.C = (uint8) ((((uint32) instruction.src_value) - instruction.dst_value) != val16 ? 1 : 0);
}

void CPU::opcode_cmpb(PipelinedInstruction &instruction) {
    uint8 src_val8 = (uint8) instruction.src_value;
    uint8 dst_val8 = (uint8) instruction.dst_value;
    uint8 val8 = src_val8 - dst_val8;
    _psw.N = is_negative8(val8);
    _psw.Z = is_zero8(val8);
    _psw.V = (uint8) (is_negative8(src_val8) != is_negative8(dst_val8) && is_negative8(val8) == is_negative8(dst_val8) ? 1 : 0);
    _psw.C = (uint8) ((((uint32) src_val8) - dst_val8) != val8 ? 1 : 0);
}

void CPU::opcode_add(PipelinedInstruction &instruction) {
    uint16 src_val16 = instruction.src_value;
    uint16 dst_val16 = instruction.dst_value;
    instruction.dst_value = src_val16 + dst_val16;
    _psw.N = is_negative16(instruction.dst_value);
    _psw.Z = is_zero16(instruction.dst_value);
    _psw.V = (uint8) (is_negative16(src_val16) == is_negative16(dst_val16)
                          && is_negative16(instruction.dst_value) != is_negative16(dst_val16) ? 1 : 0);
    _psw.C = (uint8) ((((uint32) src_val16) + dst_val16) != instruction.dst_value ? 1 : 0);
}

void CPU::opcode_sub(PipelinedInstruction &instruction) {
    uint16 src_val16 = instruction.src_value;
    uint16 dst_val16 = instruction.dst_value;
    instruction.dst_value = src_val16 - dst_val16;
    _psw.N = is_negative16(instruction.dst_value);
    _psw.Z = is_zero16(instruction.dst_value);
    _psw.V =
        (uint8) (is_negative16(src_val16) != is_negative16(dst_val16)
                     && is_negative16(instruction.dst_value) == is_negative16(dst_val16)
                 ? 1 : 0);
    _psw.C = (uint8) ((((uint32) src_val16) - dst_val16) != instruction.dst_value ? 1 : 0);
}

void CPU::opcode_bit(PipelinedInstruction &instruction) {
    instruction.dst_value = instruction.src_value & instruction.dst_value;
    _psw.N = is_negative16(instruction.dst_value);
    _psw.Z = is_zero16(instruction.dst_value);
    _psw.V = (uint8) 0;
}

void CPU::opcode_bitb(PipelinedInstruction &instruction) {
    uint8 src_val8 = (uint8) instruction.src_value;
    uint8 dst_val8 = (uint8) instruction.dst_value;
    instruction.dst_value = src_val8 & dst_val8;
    _psw.N = is_negative8((uint8) instruction.dst_value);
    _psw.Z = is_zero8((uint8) instruction.dst_value);
    _psw.V = (uint8) 0;
}

void CPU::opcode_bic(PipelinedInstruction &instruction) {
    instruction.dst_value = ~instruction.src_value & instruction.dst_value;
    _psw.N = is_negative16(instruction.dst_value);
    _psw.Z = is_zero16(instruction.dst_value);
    _psw.V = (uint8) 0;
}

void CPU::opcode_bicb(PipelinedInstruction &instruction) {
    uint8 src_val8 = (uint8) instruction.src_value;
    uint8 dst_val8 = (uint8) instruction.dst_value;
    uint8 val8 = ~src_val8 & dst_val8;
    instruction.dst_value = val8;
    _psw.N = is_negative8(val8);
    _psw.Z = is_zero8(val8);
    _psw.V = (uint8) 0;
}

void CPU::opcode_bis(PipelinedInstruction &instruction) {
    instruction.dst_value = instruction.src_value | instruction.dst_value;
    _psw.N = is_negative16(instruction.dst_value);
    _psw.Z = is_zero16(instruction.dst_value);
    _psw.V = (uint8) 0;
}

void CPU::opcode_bisb(PipelinedInstruction &instruction) {
    uint8 src_val8 = (uint8) instruction.src_value;
    uint8 dst_val8 = (uint8) instruction.dst_value;
    uint8 val8 = src_val8 | dst_val8;
    instruction.dst_value = val8;
    _psw.N = is_negative8(val8);
    _psw.Z = is_zero8(val8);
    _psw.V = (uint8) 0;
}

void CPU::opcode_xor(PipelinedInstruction &instruction) {
    instruction.dst_value = _r[instruction.register_index].r ^ instruction.dst_value;
    _psw.N = is_negative16(instruction.dst_value);
    _psw.Z = is_zero16(instruction.dst_value);
    _psw.V = (uint8) 0;
}

void CPU::opcode_br(PipelinedInstruction &instruction) {
    reset_pipeline_before(PS_EXECUTING);
    _pc.r = (uint16) (instruction.address + 2 + (instruction.branch_offset << 1));
}

void CPU::opcode_bne(PipelinedInstruction &instruction) {
    if (_psw.Z == 0) {
        reset_pipeline_before(PS_EXECUTING);
        _pc.r = (uint16) (instruction.address + 2 + (instruction.branch_offset << 1));
        instruction.instruction.time *= 2;
    }
}

void CPU::opcode_beq(PipelinedInstruction &instruction) {
    if (_psw.Z == 1) {
        reset_pipeline_before(PS_EXECUTING);
        _pc.r = (uint16) (instruction.address + 2 + (instruction.branch_offset << 1));
        instruction.instruction.time *= 2;
    }
}

void CPU::opcode_bpl(PipelinedInstruction &instruction) {
    if (_psw.N == 0) {
        reset_pipeline_before(PS_EXECUTING);
        _pc.r = (uint16) (instruction.address + 2 + (instruction.branch_offset << 1));
        instruction.instruction.time *= 2;
    }
}

void CPU::opcode_bmi(PipelinedInstruction &instruction) {
    if (_psw.N == 1) {
        reset_pipeline_before(PS_EXECUTING);
        _pc.r = (uint16) (instruction.address + 2 + (instruction.branch_offset << 1));
        instruction.instruction.time *= 2;
    }
}

void CPU::opcode_bvc(PipelinedInstruction &instruction) {
    if (_psw.V == 0) {
        reset_pipeline_before(PS_EXECUTING);
        _pc.r = (uint16) (instruction.address + 2 + (instruction.branch_offset << 1));
        instruction.instruction.time *= 2;
    }
}

void CPU::opcode_bvs(PipelinedInstruction &instruction) {
    if (_psw.V == 1) {
        reset_pipeline_before(PS_EXECUTING);
        _pc.r = (uint16) (instruction.address + 2 + (instruction.branch_offset << 1));
        instruction.instruction.time *= 2;
    }
}

void CPU::opcode_bcc(PipelinedInstruction &instruction) {
    if (_psw.C == 0) {
        reset_pipeline_before(PS_EXECUTING);
        _pc.r = (uint16) (instruction.address + 2 + (instruction.branch_offset << 1));
        instruction.instruction.time *= 2;
    }
}

void CPU::opcode_bcs(PipelinedInstruction &instruction) {
    if (_psw.C == 1) {
        reset_pipeline_before(PS_EXECUTING);
        _pc.r = (uint16) (instruction.address + 2 + (instruction.branch_offset << 1));
        instruction.instruction.time *= 2;
    }
}

void CPU::opcode_bge(PipelinedInstruction &instruction) {
    if (_psw.V == _psw.N) {
        reset_pipeline_before(PS_EXECUTING);
        _pc.r = (uint16) (instruction.address + 2 + (instruction.branch_offset << 1));
        instruction.instruction.time *= 2;
    }
}

void CPU::opcode_blt(PipelinedInstruction &instruction) {
    if ((_psw.V ^ _psw.N) == 1) {
        reset_pipeline_before(PS_EXECUTING);
        _pc.r = (uint16) (instruction.address + 2 + (instruction.branch_offset << 1));
        instruction.instruction.time *= 2;
    }
}

void CPU::opcode_bgt(PipelinedInstruction &instruction) {
    if (_psw.V == _psw.N && _psw.Z == 0) {
        reset_pipeline_before(PS_EXECUTING);
        _pc.r = (uint16) (instruction.address + 2 + (instruction.branch_offset << 1));
        instruction.instruction.time *= 2;
    }
}

void CPU::opcode_ble(PipelinedInstruction &instruction) {
    if ((_psw.V ^ _psw.N) == 1 && _psw.Z == 0) {
        reset_pipeline_before(PS_EXECUTING);
        _pc.r = (uint16) (instruction.address + 2 + (instruction.branch_offset << 1));
        instruction.instruction.time *= 2;
    }
}

void CPU::opcode_bhi(PipelinedInstruction &instruction) {
    if (_psw.C == 0 && _psw.Z == 0) {
        reset_pipeline_before(PS_EXECUTING);
        _pc.r = (uint16) (instruction.address + 2 + (instruction.branch_offset << 1));
        instruction.instruction.time *= 2;
    }
}

void CPU::opcode_blos(PipelinedInstruction &instruction) {
    if (_psw.C == 1 || _psw.Z == 1) {
        reset_pipeline_before(PS_EXECUTING);
        _pc.r = (uint16) (instruction.address + 2 + (instruction.branch_offset << 1));
        instruction.instruction.time *= 2;
    }
}

void CPU::opcode_bhis(PipelinedInstruction &instruction) {
    if (_psw.C == 0) {
        reset_pipeline_before(PS_EXECUTING);
        _pc.r = (uint16) (instruction.address + 2 + (instruction.branch_offset << 1));
        instruction.instruction.time *= 2;
    }
}

void CPU::opcode_blo(PipelinedInstruction &instruction) {
    if (_psw.C == 1) {
        reset_pipeline_before(PS_EXECUTING);
        _pc.r = (uint16) (instruction.address + 2 + (instruction.branch_offset << 1));
        instruction.instruction.time *= 2;
    }
}

void CPU::opcode_jmp(PipelinedInstruction &instruction) {
    _pc.r = instruction.dst_value;
}

void CPU::opcode_jsr(PipelinedInstruction &instruction) {
    _r[instruction.register_index].r = _pipeline_stages[PS_EXECUTING].address + _pipeline_stages[PS_EXECUTING].opcode_size;
    stack_push(_r[instruction.register_index].r, instruction);
    uint16 new_pc = instruction.dst_value;
    reset_pipeline_before(PS_EXECUTING);
    _pc.r = new_pc;
}

void CPU::opcode_rts(PipelinedInstruction &instruction) {
    reset_pipeline_before(PS_EXECUTING);
    _pc.r = _r[instruction.register_index].r;
    _r[instruction.register_index].r = stack_pop(instruction);
}

void CPU::opcode_mark(PipelinedInstruction &instruction) {
    // TODO: Implement MARK instruction?
//    uint16 nn = (uint16) (opcode & 077);
//    _sp.r = _sp.r + (nn << 1);
//    _pc.r = _r[5].r;
//    _r[5].r = read_memory_word(_sp.r).first;
}

void CPU::opcode_sob(PipelinedInstruction &instruction) {
    _r[instruction.register_index].r -= 1;
    if (_r[instruction.register_index].r != 0) {
        reset_pipeline_before(PS_EXECUTING);
        _pc.r = _pc.r - (instruction.branch_offset << 1);
    }
}

void CPU::opcode_emt(PipelinedInstruction &instruction) {
    interrupt(030, 8);
}

void CPU::opcode_trap(PipelinedInstruction &instruction) {
    interrupt(034, 8);
}

void CPU::opcode_bpt(PipelinedInstruction &instruction) {
    interrupt(014, 8);
}

void CPU::opcode_iot(PipelinedInstruction &instruction) {
    interrupt(020, 8);
}

void CPU::opcode_rti(PipelinedInstruction &instruction) {
    reset_pipeline_before(PS_EXECUTING);
    _pc.r = stack_pop(instruction);
    _psw.ps = (uint8) stack_pop(instruction);
}

void CPU::opcode_rtt(PipelinedInstruction &instruction) {
    reset_pipeline_before(PS_EXECUTING);
    _pc.r = stack_pop(instruction);
    _psw.ps = (uint8) stack_pop(instruction);
}

void CPU::opcode_halt(PipelinedInstruction &instruction) {
    _halted = true;
    print_statistics();
}

void CPU::opcode_wait(PipelinedInstruction &instruction) {
    _waiting = true;
    reset_pipeline_before(PS_EXECUTING);
}

void CPU::opcode_reset(PipelinedInstruction &instruction) {
    _unibus->set_init_line();
}

void CPU::opcode_cco(PipelinedInstruction &instruction) {
    uint8 val = (uint8) ((instruction.cco_flags >> 4) & 1);
    if ((instruction.cco_flags & 0000001) == 0000001)
        _psw.C = val;
    if ((instruction.cco_flags & 0000002) == 0000002)
        _psw.V = val;
    if ((instruction.cco_flags & 0000004) == 0000004)
        _psw.Z = val;
    if ((instruction.cco_flags & 0000010) == 0000010)
        _psw.N = val;
}

void CPU::write_back_stage() {
    // Skip this stage for instruction, which doesn't store result, or skip tick if there is no instruction on this stage
    if (_memory_block != PS_PIPELINE_LENGTH && _memory_block != PS_WRITE_BACK) {
        return;
    } else if (!_pipeline_stages[PS_WRITE_BACK].instruction.writes_result || !_pipeline_stages[PS_WRITE_BACK].stage_busy) {
        _pipeline_stages[PS_WRITE_BACK].stage_busy = false;
    } else if (_pipeline_stages[PS_WRITE_BACK].instruction.writes_result
        && _pipeline_stages[PS_WRITE_BACK].stage_ticks_left <= 0) {
        if (_pipeline_stages[PS_WRITE_BACK].dst_operand.mode == 0) {
            _r[_pipeline_stages[PS_WRITE_BACK].dst_operand.register_addr].r = _pipeline_stages[PS_WRITE_BACK].dst_value;
            _pipeline_stages[PS_WRITE_BACK].stage_ticks_left = REGISTER_ACCESS_LATENCY;
        } else {
            _memory_block = PS_WRITE_BACK;
            uint latency;
            if (_pipeline_stages[PS_WRITE_BACK].instruction.byte_wide) {
                latency = write_memory_byte(_pipeline_stages[PS_WRITE_BACK].dst_operand.memory_address,
                                            (uint8) _pipeline_stages[PS_WRITE_BACK].dst_value);
            } else {
                latency = write_memory_word(_pipeline_stages[PS_WRITE_BACK].dst_operand.memory_address,
                                            _pipeline_stages[PS_WRITE_BACK].dst_value);
            }
            _pipeline_stages[PS_WRITE_BACK].stage_ticks_left = latency;
        }
        _pipeline_stages[PS_WRITE_BACK].total_time += _pipeline_stages[PS_WRITE_BACK].stage_ticks_left;
    }
    _pipeline_stages[PS_WRITE_BACK].stage_ticks_left--;
    if (_pipeline_stages[PS_WRITE_BACK].stage_ticks_left == 0) {
        _memory_block = PS_PIPELINE_LENGTH;
        if (_pipeline_stages[PS_WRITE_BACK].dst_operand.mode == 0) {
            release_unstable_register(_pipeline_stages[PS_WRITE_BACK].id);
        } else {
            release_unstable_memory(_pipeline_stages[PS_WRITE_BACK].id);
        }
        _pipeline_stages[PS_WRITE_BACK].stage_busy = false;
        _pipeline_less_ticks += _pipeline_stages[PS_WRITE_BACK].total_time;
    }
}

void CPU::execute_stage() {
    if (!_pipeline_stages[PS_EXECUTING].stage_busy) { // Skip the tick in there is no operation on this stage
        return;
    } else if (_pipeline_stages[PS_EXECUTING].stage_ticks_left <= 0) { // First tick of the operation execution
        (this
            ->*(_pipeline_stages[PS_EXECUTING].instruction.opcode_func))(_pipeline_stages[PS_EXECUTING]);
        _pipeline_stages[PS_EXECUTING].stage_ticks_left = _pipeline_stages[PS_EXECUTING].instruction.time;
        _pipeline_stages[PS_EXECUTING].total_time += _pipeline_stages[PS_EXECUTING].stage_ticks_left;
    }
    _pipeline_stages[PS_EXECUTING].stage_ticks_left--;
    if (_pipeline_stages[PS_EXECUTING].stage_ticks_left == 0) {
        if (!_pipeline_stages[PS_WRITE_BACK].stage_busy) {
            _intructions_processed++;
            _pipeline_stages[PS_WRITE_BACK] = _pipeline_stages[PS_EXECUTING];
            _pipeline_stages[PS_EXECUTING].stage_busy = false;
            _pipeline_stages[PS_WRITE_BACK].stage_busy = true;
        } else {
            _pipeline_stages[PS_EXECUTING].stage_ticks_left = 1;
        }
    }
}

void CPU::fetch_dst_stage() {
    // Skip this stage for instruction, which doesn't store result, or skip tick if there is no instruction on this stage
    if (_pipeline_stages[PS_FETCHING_DST].stage_busy && !_pipeline_stages[PS_FETCHING_DST].instruction.has_dst) {
        _pipeline_stages[PS_FETCHING_DST].stage_ticks_left = 0;
        if (!_pipeline_stages[PS_EXECUTING].stage_busy) {
            _pipeline_stages[PS_EXECUTING] = _pipeline_stages[PS_FETCHING_DST];
            _pipeline_stages[PS_FETCHING_DST].stage_busy = false;
            _pipeline_stages[PS_EXECUTING].stage_busy = true;
        }
    } else if (!_pipeline_stages[PS_FETCHING_DST].stage_busy
        || (_memory_block != PS_PIPELINE_LENGTH && _memory_block != PS_FETCHING_DST)) {
        // Skip the tick in there is no operation on this stage
        return;
    } else if (_pipeline_stages[PS_FETCHING_DST].stage_ticks_left <= 0) { // First tick of the fetching dst operand
        _memory_block = PS_FETCHING_DST;
        pair<bool, uint> calculation_result = calculate_operand_address_pipeline(_pipeline_stages[PS_FETCHING_DST], false);
        if (!calculation_result.first) {
            _memory_block = PS_PIPELINE_LENGTH;
            return;
        }
        if (_pipeline_stages[PS_FETCHING_DST].dst_operand.mode == 0) {
            if (is_unstable_register(_pipeline_stages[PS_FETCHING_DST].dst_operand.register_addr)) {
                _memory_block = PS_PIPELINE_LENGTH;
                return;
            }
            _pipeline_stages[PS_FETCHING_DST].dst_value = _r[_pipeline_stages[PS_FETCHING_DST].dst_operand.register_addr].r;
            _pipeline_stages[PS_FETCHING_DST].stage_ticks_left = REGISTER_ACCESS_LATENCY;
            if (_pipeline_stages[PS_FETCHING_DST].instruction.writes_result)
                set_unstable_register(_pipeline_stages[PS_FETCHING_DST].dst_operand.register_addr,
                                      _pipeline_stages[PS_FETCHING_DST].id);
        } else {
            uint18 address = _pipeline_stages[PS_FETCHING_DST].dst_operand.memory_address;
            if (is_unstable_memory(address)) {
                _memory_block = PS_PIPELINE_LENGTH;
                return;
            }
            pair<uint16, uint> result;
            if (_pipeline_stages[PS_FETCHING_DST].instruction.byte_wide) {
                result = read_memory_byte(address);
            } else {
                result = read_memory_word(address);
            }
            _pipeline_stages[PS_FETCHING_DST].dst_value = result.first;
            _pipeline_stages[PS_FETCHING_DST].stage_ticks_left = result.second;
            if (_pipeline_stages[PS_FETCHING_DST].instruction.writes_result)
                set_unstable_memory(address, _pipeline_stages[PS_FETCHING_DST].id);
        }
        _pipeline_stages[PS_FETCHING_DST].stage_ticks_left += calculation_result.second;
        _pipeline_stages[PS_FETCHING_DST].total_time += _pipeline_stages[PS_FETCHING_DST].stage_ticks_left;
    }
    _pipeline_stages[PS_FETCHING_DST].stage_ticks_left--;
    if (_pipeline_stages[PS_FETCHING_DST].stage_ticks_left == 0) {
        _memory_block = PS_PIPELINE_LENGTH;
        if (!_pipeline_stages[PS_EXECUTING].stage_busy) {
            _pipeline_stages[PS_EXECUTING] = _pipeline_stages[PS_FETCHING_DST];
            _pipeline_stages[PS_FETCHING_DST].stage_busy = false;
            _pipeline_stages[PS_EXECUTING].stage_busy = true;
        } else {
            _pipeline_stages[PS_FETCHING_DST].stage_ticks_left = 1;
        }
    }
}

void CPU::fetch_src_stage() {
    // Skip this stage for instruction, which doesn't store result, or skip tick if there is no instruction on this stage
    if (_pipeline_stages[PS_FETCHING_SRC].stage_busy && !_pipeline_stages[PS_FETCHING_SRC].instruction.has_src) {
        _pipeline_stages[PS_FETCHING_SRC].stage_ticks_left = 0;
        if (!_pipeline_stages[PS_FETCHING_DST].stage_busy) {
            _pipeline_stages[PS_FETCHING_DST] = _pipeline_stages[PS_FETCHING_SRC];
            _pipeline_stages[PS_FETCHING_SRC].stage_busy = false;
            _pipeline_stages[PS_FETCHING_DST].stage_busy = true;
        }
    } else if (!_pipeline_stages[PS_FETCHING_SRC].stage_busy
        || (_memory_block != PS_PIPELINE_LENGTH && _memory_block != PS_FETCHING_SRC)) {
        // Skip the tick in there is no operation on this stage or memory bus busy
        return;
    } else if (_pipeline_stages[PS_FETCHING_SRC].stage_ticks_left <= 0) { // First tick of the fetching dst operand
        _memory_block = PS_FETCHING_SRC;
        pair<bool, uint> calculation_result = calculate_operand_address_pipeline(_pipeline_stages[PS_FETCHING_SRC], true);
        if (!calculation_result.first) {
            _memory_block = PS_PIPELINE_LENGTH;
            return;
        }
        if (_pipeline_stages[PS_FETCHING_SRC].src_operand.mode == 0) {
            if (is_unstable_register(_pipeline_stages[PS_FETCHING_SRC].src_operand.register_addr)) {
                _memory_block = PS_PIPELINE_LENGTH;
                return;
            }
            _pipeline_stages[PS_FETCHING_SRC].src_value = _r[_pipeline_stages[PS_FETCHING_SRC].src_operand.register_addr].r;
            _pipeline_stages[PS_FETCHING_SRC].stage_ticks_left = REGISTER_ACCESS_LATENCY;
        } else {
            uint18 address = _pipeline_stages[PS_FETCHING_SRC].src_operand.memory_address;
            if (is_unstable_memory(address)) {
                _memory_block = PS_PIPELINE_LENGTH;
                return;
            }
            pair<uint16, uint> result;
            if (_pipeline_stages[PS_FETCHING_SRC].instruction.byte_wide) {
                result = read_memory_byte(address);
            } else {
                result = read_memory_word(address);
            }
            _pipeline_stages[PS_FETCHING_SRC].src_value = result.first;
            _pipeline_stages[PS_FETCHING_SRC].stage_ticks_left = result.second;
        }
        _pipeline_stages[PS_FETCHING_SRC].stage_ticks_left += calculation_result.second;
        _pipeline_stages[PS_FETCHING_SRC].total_time += _pipeline_stages[PS_FETCHING_SRC].stage_ticks_left;
    }
    _pipeline_stages[PS_FETCHING_SRC].stage_ticks_left--;
    if (_pipeline_stages[PS_FETCHING_SRC].stage_ticks_left == 0) {
        _memory_block = PS_PIPELINE_LENGTH;
        if (!_pipeline_stages[PS_FETCHING_DST].stage_busy) {
            _pipeline_stages[PS_FETCHING_DST] = _pipeline_stages[PS_FETCHING_SRC];
            _pipeline_stages[PS_FETCHING_SRC].stage_busy = false;
            _pipeline_stages[PS_FETCHING_DST].stage_busy = true;
        } else {
            _pipeline_stages[PS_FETCHING_SRC].stage_ticks_left = 1;
        }
    }
}

void CPU::decode_stage() {
    if (!_pipeline_stages[PS_DECODING].stage_busy) { // Skip the tick in there is no operation on this stage
        return;
    } else if (_pipeline_stages[PS_DECODING].stage_ticks_left <= 0) { // First tick of the fetching dst operand
        for (auto instruction_it = _instruction_set.begin(); instruction_it != _instruction_set.end();
             ++instruction_it) {
            if ((_pipeline_stages[PS_DECODING].opcode & instruction_it->opcode_mask) == instruction_it->opcode_signature) {
                _pipeline_stages[PS_DECODING].instruction = *instruction_it;
                _pipeline_stages[PS_DECODING].stage_ticks_left = OPCODE_DECODING_LATENCY;
                break;
            }
        }
        if (_pipeline_stages[PS_DECODING].instruction.has_src) {
            InstructionOperand operand = decode_src_operand(_pipeline_stages[PS_DECODING].opcode);
            _pipeline_stages[PS_DECODING].src_operand = operand;
            _pipeline_stages[PS_DECODING].stage_ticks_left += OPERAND_DECODING_LATENCY;
        }
        if (_pipeline_stages[PS_DECODING].instruction.has_dst) {
            InstructionOperand operand = decode_dst_operand(_pipeline_stages[PS_DECODING].opcode);
            _pipeline_stages[PS_DECODING].dst_operand = operand;
            _pipeline_stages[PS_DECODING].stage_ticks_left += OPERAND_DECODING_LATENCY;
        }
        if (_pipeline_stages[PS_DECODING].instruction.opcode_mask == CONDITION_CODE_INSTRUCTION_MASK) {
            _pipeline_stages[PS_DECODING].cco_flags = (uint8) (_pipeline_stages[PS_DECODING].opcode & 0x1F);
            _pipeline_stages[PS_DECODING].stage_ticks_left += OPERAND_DECODING_LATENCY;
        }
        if (_pipeline_stages[PS_DECODING].instruction.opcode_mask == BRANCHING_OFFSET_INSTRUCTION_MASK) {
            _pipeline_stages[PS_DECODING].branch_offset = (int8) (_pipeline_stages[PS_DECODING].opcode & BRANCHING_OFFSET_MASK);
            _pipeline_stages[PS_DECODING].stage_ticks_left += OPERAND_DECODING_LATENCY;
        }
        if (_pipeline_stages[PS_DECODING].instruction.opcode_mask == REGISTER_OPERAND_INSTRUCTION_MASK) {
            // Decode XOR, JSR, SOB register-operand
            _pipeline_stages[PS_DECODING].register_index = (uint8) ((_pipeline_stages[PS_DECODING].opcode & 0000700) >> 6);
            _pipeline_stages[PS_DECODING].stage_ticks_left += OPERAND_DECODING_LATENCY;
        }
        if (_pipeline_stages[PS_DECODING].instruction.opcode_mask == REGISTER_ONLY_INSTRUCTION_MASK) {
            // Decode RTS register-operand
            _pipeline_stages[PS_DECODING].register_index = (uint8) ((_pipeline_stages[PS_DECODING].opcode & 0000007));
            _pipeline_stages[PS_DECODING].stage_ticks_left += OPERAND_DECODING_LATENCY;
        }
        _pipeline_stages[PS_DECODING].total_time += _pipeline_stages[PS_DECODING].stage_ticks_left;
    }
    _pipeline_stages[PS_DECODING].stage_ticks_left--;
    if (_pipeline_stages[PS_DECODING].stage_ticks_left == 0) {
        if (!_pipeline_stages[PS_FETCHING_SRC].stage_busy) {
            _pipeline_stages[PS_FETCHING_SRC] = _pipeline_stages[PS_DECODING];
            _pipeline_stages[PS_DECODING].stage_busy = false;
            _pipeline_stages[PS_FETCHING_SRC].stage_busy = true;
        } else {
            _pipeline_stages[PS_DECODING].stage_ticks_left = 1;
        }
    }
}

void CPU::fetch_stage() {
    // Skip the tick in there is no operation on this stage or bus blocked
    if (!_pipeline_stages[PS_FETCHING].stage_busy || (_memory_block != PS_PIPELINE_LENGTH && _memory_block != PS_FETCHING)) {
        return;
    } else if (_pipeline_stages[PS_FETCHING].stage_ticks_left <= 0) { // First tick of the fetching dst operand
        uint18 address = _pc.r;
        pair<uint16, uint> result = read_memory_word(address);
        _pipeline_stages[PS_FETCHING].opcode = result.first;
        _pipeline_stages[PS_FETCHING].stage_ticks_left = result.second;
        _pipeline_stages[PS_FETCHING].src_value = 0;
        _pipeline_stages[PS_FETCHING].dst_value = 0;
        _pipeline_stages[PS_FETCHING].cco_flags = 0;
        _pipeline_stages[PS_FETCHING].register_index = 0;
        _pipeline_stages[PS_FETCHING].address = _pc.r;
        _pipeline_stages[PS_FETCHING].opcode_size = 2;
        _pipeline_stages[PS_FETCHING].id = _ticks;
        _pipeline_stages[PS_FETCHING].total_time = (uint) _pipeline_stages[PS_FETCHING].stage_ticks_left;
        _memory_block = PS_FETCHING;
    }
    _pipeline_stages[PS_FETCHING].stage_ticks_left--;
    if (_pipeline_stages[PS_FETCHING].stage_ticks_left == 0) {
        _memory_block = PS_PIPELINE_LENGTH;
        if (!_pipeline_stages[PS_DECODING].stage_busy) {
            _pipeline_stages[PS_DECODING] = _pipeline_stages[PS_FETCHING];
            _pipeline_stages[PS_FETCHING].stage_busy = true; // Don't stop instruction fetching mechanism
            _pipeline_stages[PS_DECODING].stage_busy = true;
            _pc.r += 2;
        } else {
            _pipeline_stages[PS_FETCHING].stage_ticks_left = 1;
        }
    }
}

pair<bool, uint> CPU::calculate_operand_address_pipeline(PipelinedInstruction &pipeline_stage, bool src_operand) {
    uint16 index;
    uint latency = 0;
    pair<uint16, uint> mem_read_result;
    InstructionOperand &operand = src_operand ? pipeline_stage.src_operand : pipeline_stage.dst_operand;
    if (operand.ready)
        return make_pair(true, operand.calculation_time);
    switch (operand.mode) {
        case 0:
            operand.memory_address = 0;
            break;
        case 1: // Register Deferred
            if (is_unstable_register(operand.register_addr))
                return make_pair(false, 0);
            operand.memory_address = _r[operand.register_addr].r;
            latency = REGISTER_ACCESS_LATENCY;
            break;
        case 2: // Autoincrement
            if (is_unstable_register(operand.register_addr))
                return make_pair(false, 0);
            if (operand.register_addr == 7) {
                operand.memory_address = pipeline_stage.address + operand.index_offset;
            } else {
                operand.memory_address = _r[operand.register_addr].r;
            }
            _r[operand.register_addr].r += pipeline_stage.instruction.byte_wide ? 1 : 2;
            latency = REGISTER_ACCESS_LATENCY + 1; // 1 - Increment time

            if (operand.register_addr == 07) {
                pipeline_stage.opcode_size += 2;
                if (src_operand) {
                    reset_pipeline_before(PS_FETCHING_SRC);
                } else {
                    reset_pipeline_before(PS_FETCHING_DST);
                }
            }
            break;
        case 3: // Autoincrement Deferred
            if (is_unstable_register(operand.register_addr) || is_unstable_memory(_r[operand.register_addr].r))
                return make_pair(false, 0);

            if (operand.register_addr == 7) {
                mem_read_result = read_memory_word(pipeline_stage.address + operand.index_offset);
            } else {
                mem_read_result = read_memory_word(_r[operand.register_addr].r);
            }
            operand.memory_address = mem_read_result.first;
            latency = mem_read_result.second;
            _r[operand.register_addr].r += pipeline_stage.instruction.byte_wide ? 1 : 2;
            latency += REGISTER_ACCESS_LATENCY + 1; // 1 - Increment time

            if (operand.register_addr == 07) {
                pipeline_stage.opcode_size += 2;
                if (src_operand) {
                    reset_pipeline_before(PS_FETCHING_SRC);
                } else {
                    reset_pipeline_before(PS_FETCHING_DST);
                }
            }
            break;
        case 4: // Autodecrement
            if (is_unstable_register(operand.register_addr))
                return make_pair(false, 0);
            _r[operand.register_addr].r -= pipeline_stage.instruction.byte_wide ? 1 : 2;
            operand.memory_address = this->_r[operand.register_addr].r;
            latency = REGISTER_ACCESS_LATENCY + 1; // 1 - Decrement time
            break;
        case 5: // Autodecrement Deferred
            if (is_unstable_register(operand.register_addr) || is_unstable_memory(_r[operand.register_addr].r))
                return make_pair(false, 0);
            _r[operand.register_addr].r -= pipeline_stage.instruction.byte_wide ? 1 : 2;
            mem_read_result = read_memory_word(this->_r[operand.register_addr].r);
            latency = mem_read_result.second + 1; // 1 - Decrement time
            operand.memory_address = mem_read_result.first;
            latency += REGISTER_ACCESS_LATENCY;
            break;
        case 6: // Index
            if (is_unstable_register(operand.register_addr) || is_unstable_memory(pipeline_stage.address + operand.index_offset))
                return make_pair(false, 0);
            mem_read_result = read_memory_word(pipeline_stage.address + operand.index_offset);
            latency = mem_read_result.second + REGISTER_ACCESS_LATENCY;
            latency += 2; // Addition time
            index = mem_read_result.first;
            operand.memory_address = _r[operand.register_addr].r + index;
            pipeline_stage.opcode_size += 2;

            if (src_operand) {
                reset_pipeline_before(PS_FETCHING_SRC);
            } else {
                reset_pipeline_before(PS_FETCHING_DST);
            }
            break;
        case 7: // Index Deferred
            if (is_unstable_register(operand.register_addr) || is_unstable_memory(pipeline_stage.address + operand.index_offset))
                return make_pair(false, 0);
            mem_read_result = read_memory_word(pipeline_stage.address + operand.index_offset);
            latency = mem_read_result.second + REGISTER_ACCESS_LATENCY;
            index = mem_read_result.first;
            if (is_unstable_memory(_r[operand.register_addr].r + index))
                return make_pair(false, 0);
            mem_read_result = read_memory_word(_r[operand.register_addr].r + index);
            latency += mem_read_result.second;
            latency += 2; // Addition time
            operand.memory_address = mem_read_result.first;
            pipeline_stage.opcode_size += 2;
            if (src_operand) {
                reset_pipeline_before(PS_FETCHING_SRC);
            } else {
                reset_pipeline_before(PS_FETCHING_DST);
            }
            break;
        default:
            throw new runtime_error("Wrong addressing mode");
    }
    operand.ready = true;
    operand.calculation_time = latency;
    return make_pair(operand.ready, latency);
}

bool CPU::is_unstable_memory(uint18 address) {
    for (auto it = _memory_unstable_addresses.begin(); it != _memory_unstable_addresses.end(); ++it)
        if (it->first == (address - (address % 2)))
            return true;
    return false;
}

bool CPU::is_unstable_register(uint register_index) {
    for (auto it = _registers_unstable_addresses.begin(); it != _registers_unstable_addresses.end(); ++it)
        if (it->first == register_index)
            return true;
    return false;
}

void CPU::set_unstable_memory(uint18 address, uint64 instruction_id) {
    _memory_unstable_addresses.push_back(make_pair(address - (address % 2), instruction_id));
}

void CPU::set_unstable_register(uint register_index, uint64 instruction_id) {
    _registers_unstable_addresses.push_back(make_pair(register_index, instruction_id));
}

void CPU::release_unstable_memory(uint64 instruction_id) {
    for (auto it = _memory_unstable_addresses.begin(); it != _memory_unstable_addresses.end();) {
        if (it->second == instruction_id)
            it = _memory_unstable_addresses.erase(it);
        else
            ++it;
    }
}

void CPU::release_unstable_register(uint64 instruction_id) {
    for (auto it = _registers_unstable_addresses.begin(); it != _registers_unstable_addresses.end();) {
        if (it->second == instruction_id)
            it = _registers_unstable_addresses.erase(it);
        else
            ++it;
    }
}

void CPU::reset_pipeline_before(int stage) {
    for (int i = 0; i < stage; i++) {
        _pipeline_stages[i].stage_busy = false;
        _pipeline_stages[i].stage_ticks_left = 0;
        release_unstable_register(_pipeline_stages[i].id);
        release_unstable_memory(_pipeline_stages[i].id);
        revert_addressing_register_changes(_pipeline_stages[i]);
    }
    if (_memory_block < stage)
        _memory_block = PS_PIPELINE_LENGTH;
    _pipeline_stages[PS_FETCHING].stage_busy = true;
    _pc.r = _pipeline_stages[stage].address + _pipeline_stages[stage].opcode_size;
    _pipeline_resets++;
}

void CPU::print_statistics() {
    cout << "Total ticks: " << _ticks << ", instructions processed: " << _intructions_processed << endl;
    cout << "Efficiency: " << 1.0f * _intructions_processed / _ticks << " instructions/tick, ";
    cout << 1.0f * _ticks / _intructions_processed << " tick's per instruction" << endl;

    cout << "Estimated ticks without pipeline: " << _pipeline_less_ticks << endl;
    cout << "Estimated efficiency without pipeline: " << 1.0f * _intructions_processed / _pipeline_less_ticks
         << " instructions/tick, ";
    cout << 1.0f * _pipeline_less_ticks / _intructions_processed << " tick's per instruction" << endl;

    cout << "Pipeline resets: " << _pipeline_resets << ", " << 100.0f * _pipeline_resets / _ticks << "% of ticks" << endl;
    double pipeline_speedup = (1.0f * _pipeline_less_ticks) / (1.0f * _ticks);
    if (pipeline_speedup > 1.0f) {
        cout << "Pipeline is " << (pipeline_speedup - 1.0f) * 100.0f << "% faster than CPU without pipeline" << endl;
    } else {
        cout << "Pipeline is " << (1.0f - pipeline_speedup) * 100.0f << "% slower than CPU without pipeline" << endl;
    }
}

void CPU::revert_addressing_register_changes(PipelinedInstruction &instruction) {
    if (instruction.instruction.has_dst && instruction.dst_operand.ready) {
        // Modes 2 & 3 - Autoincrement
        if (instruction.dst_operand.mode == 0x2 || instruction.dst_operand.mode == 0x3) {
            _r[instruction.dst_operand.register_addr].r -= instruction.instruction.byte_wide ? 1 : 2;
        }
        // Modes 4 & 5 - Autodecrement
        if (instruction.dst_operand.mode == 0x4 || instruction.dst_operand.mode == 0x5) {
            _r[instruction.dst_operand.register_addr].r += instruction.instruction.byte_wide ? 1 : 2;
        }
    }
    if (instruction.instruction.has_src && instruction.src_operand.ready) {
        // Modes 2 & 3 - Autoincrement
        if (instruction.src_operand.mode == 0x2 || instruction.src_operand.mode == 0x3) {
            _r[instruction.src_operand.register_addr].r -= instruction.instruction.byte_wide ? 1 : 2;
        }
        // Modes 4 & 5 - Autodecrement
        if (instruction.src_operand.mode == 0x4 || instruction.src_operand.mode == 0x5) {
            _r[instruction.src_operand.register_addr].r += instruction.instruction.byte_wide ? 1 : 2;
        }
    }
}
