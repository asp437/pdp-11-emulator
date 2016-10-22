//
// Created by Aleksandr Parfenov on 05.10.16.
//

#ifndef PDP_11_EMULATOR_ROM_H
#define PDP_11_EMULATOR_ROM_H

#include "../../common.h"

#include "../unibus/unibus_device.h"
#include "../unibus/unibus.h"

class ROM : public UnibusDevice {
public:
    ROM(string rom_file);
    virtual ~ROM();

    string get_name() override { return "Main Read Only Memory"; }
    void reset() override;
    uint16 read_word(uint18 address, uint18 base_address) override;
    void write_word(uint18 address, uint18 base_address, uint16 value) override;
    uint8 read_byte(uint18 address, uint18 base_address) override;
    void write_byte(uint18 address, uint18 base_address, uint8 value) override;
    uint16 get_rom_size() const { return _rom_size; }
    bool is_busy() override { return false; }

    static const uint18 ROM_DEFAULT_SIZE = 020000;
    static const uint18 ROM_DEFAULT_OFFSET = 0140000;
private:
    uint8 *_rom_array;
    uint18 _rom_size;
};

#endif //PDP_11_EMULATOR_ROM_H
