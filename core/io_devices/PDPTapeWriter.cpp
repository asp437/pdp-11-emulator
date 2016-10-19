//
// Created by Aleksandr Parfenov on 03.10.16.
//

#include "PDPTapeWriter.h"

PDPTapeWriter::PDPTapeWriter() {
    last_printed_char = 000;
}

PDPTapeWriter::~PDPTapeWriter() {
    last_printed_char = 000;
}

void PDPTapeWriter::reset() {
    last_printed_char = 000;
}

uint16 PDPTapeWriter::read_word(uint18 address, uint18 base_address) {
    if (address == PDP_TAPE_WRITER_MEM_REGISTERS_ADDRESS)
        return last_printed_char;
    else
        throw new runtime_error("Wrong memory address PDPTapeWriter");
}

void PDPTapeWriter::write_word(uint18 address, uint18 base_address, uint16 value) {
    if (address == PDP_TAPE_WRITER_MEM_REGISTERS_ADDRESS) {
        last_printed_char = value;
        if (last_printed_char != '\0') {
            cout << char(last_printed_char);
            cout.flush();
        }
    } else
        throw new runtime_error("Wrong memory address PDPTapeWriter");
}

uint8 PDPTapeWriter::read_byte(uint18 address, uint18 base_address) {
    if (address == PDP_TAPE_WRITER_MEM_REGISTERS_ADDRESS)
        return (uint8) last_printed_char;
    else
        throw new runtime_error("Wrong memory address PDPTapeWriter");
}

void PDPTapeWriter::write_byte(uint18 address, uint18 base_address, uint8 value) {
    if (address == PDP_TAPE_WRITER_MEM_REGISTERS_ADDRESS) {
        last_printed_char = value;
        if (last_printed_char != '\0') {
            cout << char(last_printed_char);
            cout.flush();
        }
    } else
        throw new runtime_error("Wrong memory address PDPTapeWriter");

}
