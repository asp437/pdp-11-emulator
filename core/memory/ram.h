//
// Created by Aleksandr Parfenov on 01.10.16.
//

#ifndef PDP_11_EMULATOR_MEMORY_H
#define PDP_11_EMULATOR_MEMORY_H

#include "../../common.h"

#include "../unibus/unibus_device.h"
#include "../unibus/unibus.h"

class RAM : public UnibusDevice {
public:
    RAM(uint16 memory_size);
    virtual ~RAM();

    string get_name() override { return "Main Random Access Memory"; }
    void reset() override;
    uint16 read_word(uint18 address, uint18 base_address) override;
    void write_word(uint18 address, uint18 base_address, uint16 value) override;
    uint8 read_byte(uint18 address, uint18 base_address) override;
    void write_byte(uint18 address, uint18 base_address, uint8 value) override;
    uint16 get_memory_size() const;
    bool is_busy() override { return false; }
private:
    uint8 *_memory_array;
    uint16 _memory_size;
    Unibus *_unibus;
};

#endif //PDP_11_EMULATOR_MEMORY_H
