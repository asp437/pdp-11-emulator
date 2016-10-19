//
// Created by Aleksandr Parfenov on 03.10.16.
//

#ifndef PDP_11_EMULATOR_PDPKEYBOARD_H
#define PDP_11_EMULATOR_PDPKEYBOARD_H

#include "../../common.h"

#include <map>

#include "../UnibusDevice.h"


class PDPKeyboard : public UnibusDevice {
public:
    PDPKeyboard();
    virtual ~PDPKeyboard();
    string get_name() override { return "PDP-11 Compatible Keyboard"; }
    void reset() override;
    bool is_busy() override { return false; }
    uint16 read_word(uint18 address, uint18 base_address) override;
    void write_word(uint18 address, uint18 base_address, uint16 value) override;
    uint8 read_byte(uint18 address, uint18 base_address) override;
    void write_byte(uint18 address, uint18 base_address, uint8 value) override;

    void key_pressed(int keycode, bool key_down);

    static const uint18 PDP_KEYBOARD_SYMBOL_REGISTER = 0176000;
    static const uint18 PDP_KEYBOARD_HANDLER_PC_LOCATION_REGISTER = 0176002;
    static const uint18 PDP_KEYBOARD_HANDLER_PSW_LOCATION_REGISTER = 0176004;
    static const uint18 PDP_KEYBOARD_REGISTERS_SIZE = 000006;
private:
    uint16 _key_code;
    uint16 _handler_pc;
    uint16 _handler_psw;

    map<uint, uint16> _key_conversion_map;

    bool _shift_pressed;
    bool _ctrl_pressed;
    bool _alt_pressed;
};

#endif //PDP_11_EMULATOR_PDPKEYBOARD_H
