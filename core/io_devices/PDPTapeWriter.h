//
// Created by Aleksandr Parfenov on 03.10.16.
//

#ifndef PDP_11_EMULATOR_TAPEWRITER_H
#define PDP_11_EMULATOR_TAPEWRITER_H

#include "../../common.h"
#include "../UnibusDevice.h"

class PDPTapeWriter : public UnibusDevice {
public:
    PDPTapeWriter();
    virtual ~PDPTapeWriter();
    string get_name() override { return "PDP-11 Compatible Tape Writer"; }
    void reset() override;
    bool is_busy() override { return false; }
    uint16 read_word(uint18 address, uint18 base_address) override;
    void write_word(uint18 address, uint18 base_address, uint16 value) override;
    uint8 read_byte(uint18 address, uint18 base_address) override;
    void write_byte(uint18 address, uint18 base_address, uint8 value) override;

    static const uint18 PDP_TAPE_WRITER_MEM_REGISTERS_ADDRESS = 0176100;
    static const uint18 PDP_TAPE_WRITER_MEM_REGISTERS_SIZE = 0000002;
private:
    uint16 last_printed_char;
};

#endif //PDP_11_EMULATOR_TAPEWRITER_H
