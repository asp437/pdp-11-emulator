//
// Created by Aleksandr Parfenov on 03.10.16.
//

#include "pdp_keyboard.h"
#include "../unibus/unibus.h"

PDPKeyboard::PDPKeyboard() {
    for (uint i = 0; i <= 9; i++)
        _key_conversion_map[0x30 + i] = (uint16) (14 + i); // '0' - '9'
    for (uint i = 0; i < 26; i++)
        _key_conversion_map[0x41 + i] = (uint16) (24 + i); // 'a' - 'z'

    _key_conversion_map[0x60] = 3; // `
    _key_conversion_map[0x7e] = 3; // ~
    _key_conversion_map[0x21] = 15; // !
    _key_conversion_map[0x40] = 16; // @
    _key_conversion_map[0x23] = 17; // #
    _key_conversion_map[0x24] = 18; // $
    _key_conversion_map[0x25] = 19; // %
    _key_conversion_map[0x5e] = 20; // ^
    _key_conversion_map[0x26] = 21; // &
    _key_conversion_map[0x2a] = 22; // *
    _key_conversion_map[0x28] = 23; // (
    _key_conversion_map[0x29] = 14; // )
    _key_conversion_map[0x5f] = 4; // _
    _key_conversion_map[0x2d] = 4; // -
    _key_conversion_map[0x2b] = 5; // +
    _key_conversion_map[0x3d] = 5; // =
    _key_conversion_map[0x5c] = 13; // <back_slash>
    _key_conversion_map[0x7c] = 13; // |

    _key_conversion_map[0x7b] = 6; // {
    _key_conversion_map[0x7d] = 7; // }
    _key_conversion_map[0x5b] = 6; // [
    _key_conversion_map[0x5d] = 7; // ]
    _key_conversion_map[0x3b] = 8; // ;
    _key_conversion_map[0x3a] = 8; // :
    _key_conversion_map[0x27] = 9; // '
    _key_conversion_map[0x22] = 9; // "
    _key_conversion_map[0x2c] = 10; // ,
    _key_conversion_map[0x2e] = 11; // .
    _key_conversion_map[0x2f] = 12; // /
    _key_conversion_map[0x3c] = 10; // <
    _key_conversion_map[0x3e] = 11; // >
    _key_conversion_map[0x3f] = 12; // ?
    _key_conversion_map[0x20] = 2; // <space>
    _key_conversion_map[0x1000003] = 1; // <backspace>
    _key_conversion_map[0x1000004] = 0; // <enter>

    reset();
}

PDPKeyboard::~PDPKeyboard() {
    reset();
}

void PDPKeyboard::reset() {
    _key_code = 0;
    _handler_pc = 0;
    _handler_psw = 0;
    _shift_pressed = false;
    _ctrl_pressed = false;
    _alt_pressed = false;
}

uint16 PDPKeyboard::read_word(uint18 address, uint18 base_address) {
    switch (address) {
        case PDP_KEYBOARD_SYMBOL_REGISTER:
            return _key_code;
        case PDP_KEYBOARD_HANDLER_PC_LOCATION_REGISTER:
            return _handler_pc;
        case PDP_KEYBOARD_HANDLER_PSW_LOCATION_REGISTER:
            return _handler_psw;
        default:
            throw new runtime_error("Wrong memory address PDPKeyboard");
    }
}

uint8 PDPKeyboard::read_byte(uint18 address, uint18 base_address) {
    throw new runtime_error("PDPKeyboard doesn't support byte access");
}

void PDPKeyboard::write_byte(uint18 address, uint18 base_address, uint8 value) {
    throw new runtime_error("PDPKeyboard doesn't support byte access");
}

void PDPKeyboard::write_word(uint18 address, uint18 base_address, uint16 value) {
    switch (address) {
        case PDP_KEYBOARD_HANDLER_PC_LOCATION_REGISTER:
            _handler_pc = value;
            break;
        case PDP_KEYBOARD_HANDLER_PSW_LOCATION_REGISTER:
            _handler_psw = value;
            break;
        default:
            throw new runtime_error("Wrong memory address PDPKeyboard");
    }
}

void PDPKeyboard::key_pressed(int keycode, bool key_down) {
    if (keycode == 0x01000020) { // Qt::Key::Shift
        _shift_pressed = key_down;
    } else if (keycode == 0x01000021) { // Qt::Key::Control
        _ctrl_pressed = key_down;
    } else if (keycode == 0x01000023 || keycode == 0x01001103) { // Qt::Key::Alt(Gr)
        _alt_pressed = key_down;
    } else if (_key_conversion_map.find(keycode) != _key_conversion_map.end()) {
        _key_code = (uint16) _key_conversion_map[keycode];
        _key_code |= _alt_pressed ? (1 << 5) : 0;
        _key_code |= _shift_pressed ? (1 << 6) : 0;
        _key_code |= _ctrl_pressed ? (1 << 7) : 0;
        _key_code |= key_down ? (1 << 9) : 0;
        _unibus->cpu_interrupt(PDP_KEYBOARD_HANDLER_PC_LOCATION_REGISTER, PDP_KEYBOARD_INTERRUPT_PRIORITY);
    }
}
