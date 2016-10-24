//
// Created by Aleksandr Parfenov on 19.10.16.
//

#ifndef PDP_11_EMULATOR_DISASM_H
#define PDP_11_EMULATOR_DISASM_H

#include "../../common.h"
#include <vector>

#include "../unibus/unibus.h"
#include "../cpu.h"

class DisAsm {
public:
    DisAsm(Unibus *unibus, vector<CPUInstruction> instruction_set);

    vector<pair<string, uint16>> disasm_code(uint18 base_address, uint18 size);
private:
    pair<string, uint16> get_src_operand(uint16 opcode, uint18 opcode_address);
    pair<string, uint16> get_dst_operand(uint16 opcode, uint18 opcode_address);
    pair<string, uint16> get_operand_value(InstructionOperand operand, uint18 opcode_address);
    pair<string, uint16> disasm_instruction(uint18 address);

    Unibus *_unibus;
    vector<CPUInstruction> _instruction_set;
};

#endif //PDP_11_EMULATOR_DISASM_H
