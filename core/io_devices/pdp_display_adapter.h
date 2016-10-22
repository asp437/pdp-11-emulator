//
// Created by Aleksandr Parfenov on 03.10.16.
//

#ifndef PDP_11_EMULATOR_PDPDISPLAYADAPTER_H
#define PDP_11_EMULATOR_PDPDISPLAYADAPTER_H

#include "../../common.h"
#include "../unibus/unibus_device.h"

#include <vector>

class PDPDisplayAdapter : public UnibusDevice {
public:
    PDPDisplayAdapter();
    virtual ~PDPDisplayAdapter();
    string get_name() override { return "PDP-11 Compatible Display Adapter"; }
    void reset() override;
    bool is_busy() override { return false; }
    uint16 read_word(uint18 address, uint18 base_address) override;
    void write_word(uint18 address, uint18 base_address, uint16 value) override;
    uint8 read_byte(uint18 address, uint18 base_address) override;
    void write_byte(uint18 address, uint18 base_address, uint8 value) override;
    uint get_pixel(uint16 x, uint16 y);
    uint16 get_width() { return 400; }
    uint16 get_height() { return 300; }

    static const uint16 PDP_VIDEO_ADAPTER_VRAM_ADDRESS = 0040000;
    static const uint16 PDP_VIDEO_ADAPTER_VRAM_SIZE = 0100000;
private:
    uint8 *_vram;
    uint16 _vram_size;
};

#endif //PDP_11_EMULATOR_PDPDISPLAYADAPTER_H
