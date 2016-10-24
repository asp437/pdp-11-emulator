//
// Created by Aleksandr Parfenov on 19.10.16.
//

#include "disasm.h"
#include <sstream>
#include <iomanip>

vector <pair<string, uint16>> DisAsm::disasm_code(uint18 base_address, uint18 size) {
    uint18 address = base_address;
    vector <pair<string, uint16>> result;
    while (address < base_address + size) {
        pair <string, uint16> decoded_instruction = disasm_instruction(address);
        result.push_back(decoded_instruction);
        address += decoded_instruction.second;
    }
    return result;
}

pair <string, uint16> DisAsm::get_dst_operand(uint16 opcode, uint18 opcode_address) {
    InstructionOperand operand = CPU::decode_dst_operand(opcode);
    operand.index_offset += 2;
    return get_operand_value(operand, opcode_address);
}

pair <string, uint16> DisAsm::get_src_operand(uint16 opcode, uint18 opcode_address) {
    InstructionOperand operand = CPU::decode_src_operand(opcode);
    operand.index_offset += 2;
    return get_operand_value(operand, opcode_address);
}

pair<string, uint16> DisAsm::get_operand_value(InstructionOperand operand, uint18 opcode_address) {
    stringstream ss;
    ss << oct;
    uint16 val;
    uint16 operand_after_instruction = 0;
    switch (operand.mode) {
        case 0:
            ss << "R" << to_string(operand.register_addr);
            break;
        case 1:
            ss << "@(R" << to_string(operand.register_addr) << ")";
            break;
        case 2:
            if (operand.register_addr == 7) {
                operand_after_instruction += 2;
                ss << oct << setfill('0') << setw(7) << _unibus->read_word(opcode_address + operand.index_offset);
            } else {
                ss << "(R" << to_string(operand.register_addr) << ")+";
            }
            break;
        case 3:
            if (operand.register_addr == 7) {
                operand_after_instruction += 2;
                ss << "@" << oct << setfill('0') << setw(7)
                   << _unibus->read_word(opcode_address + operand.index_offset);
            } else {
                ss << "@(R" << to_string(operand.register_addr) << ")+";
            }
            break;
        case 4:
            ss << "-(R" << to_string(operand.register_addr) << ")";
            break;
        case 5:
            ss << "@-(R" << to_string(operand.register_addr) << ")";
            break;
        case 6:
            val = _unibus->read_word(opcode_address + operand.index_offset);
            operand_after_instruction += 2;
            if (operand.register_addr == 7) {
                ss << oct << setfill('0') << setw(7) << _unibus->read_word(val + opcode_address);
            } else {
                ss << oct << setfill('0') << setw(7) << val << "(R" << to_string(operand.register_addr) << ")";
            }
            break;
        case 7:
            val = _unibus->read_word(opcode_address + operand.index_offset);
            operand_after_instruction += 2;
            if (operand.register_addr == 7) {
                ss << "@(" << oct << setfill('0') << setw(7) << _unibus->read_word(val + opcode_address) << ")";
            } else {
                ss << "@" << oct << setfill('0') << setw(7) << val << "(R" << to_string(operand.register_addr) << ")";
            }
            break;
        default:
            throw new runtime_error("Unsupported memory access mode");
    }
    return make_pair(ss.str(), operand_after_instruction);
}

pair <string, uint16> DisAsm::disasm_instruction(uint18 address) {
    uint16 opcode = _unibus->read_word(address);
    stringstream ss;
    uint16 instruction_size = 2; // In bytes
    for (auto instruction_it = _instruction_set.begin(); instruction_it != _instruction_set.end();
         ++instruction_it) {
        if ((opcode & instruction_it->opcode_mask) == instruction_it->opcode_signature) {
            ss << instruction_it->mnemonic << " ";
            if (instruction_it->opcode_mask == DOUBLE_OPERAND_INSTRUCTION_MASK) {
                pair <string, uint16> operand = get_src_operand(opcode, address);
                ss << operand.first << ", ";
                instruction_size += operand.second;

                operand = get_dst_operand(opcode, address);
                ss << operand.first;
                instruction_size += operand.second;
            } else if (instruction_it->opcode_mask == SINGLE_OPERAND_INSTRUCTION_MASK) {
                pair <string, uint16> operand = get_dst_operand(opcode, address);
                ss << operand.first;
                instruction_size += operand.second;
            } else if (instruction_it->opcode_mask == REGISTER_ONLY_INSTRUCTION_MASK) {
                ss << "R" << (opcode & 07);
            } else if (instruction_it->opcode_mask == REGISTER_OPERAND_INSTRUCTION_MASK) {
                ss << "R" << ((opcode & 0700) >> 6) << ", ";
                pair <string, uint16> operand = get_dst_operand(opcode, address);
                ss << operand.first;
                instruction_size += operand.second;
            } else if (instruction_it->opcode_mask == BRANCHING_OFFSET_INSTRUCTION_MASK) {
                ss << (int16)((int8)(opcode & BRANCHING_OFFSET_MASK));
            } else if (instruction_it->opcode_mask == CONDITION_CODE_INSTRUCTION_MASK) {
                ss << ((((opcode >> 4) & 1) == 1) ? " SET " : " RST ");
                if ((opcode & 0000001) == 0000001)
                    ss << "C ";
                if ((opcode & 0000002) == 0000002)
                    ss << "V ";
                if ((opcode & 0000004) == 0000004)
                    ss << "Z ";
                if ((opcode & 0000010) == 0000010)
                    ss << "N ";
            }
        }
    }
    return make_pair(ss.str(), instruction_size);
}

DisAsm::DisAsm(Unibus *unibus, vector <CPUInstruction> instruction_set)
    : _unibus(unibus), _instruction_set(instruction_set) {
}
