//
// Created by Aleksandr Parfenov on 03.10.16.
//

#include "PDPKeyboard.h"
#include "../Unibus.h"

PDPKeyboard::PDPKeyboard() {
  _buffer = "Hello World";
}

PDPKeyboard::~PDPKeyboard() {
  _buffer = "";
}

void PDPKeyboard::reset() {
  _buffer = "";
}

uint16 PDPKeyboard::read_word(uint18 address, uint18 base_address) {
  uint16 symbol;
  switch (address) {
    case PDP_KEYBOARD_SYMBOL_MEMORY_ADDRESS:
      if (_buffer.size() == 0)
        return '\0';
      symbol = (uint16) _buffer.at(0);
      _buffer.erase(0, 1);
      return symbol;
    case PDP_KEYBOARD_ALT_FLAG_MEMORY_ADDRESS:
      return (uint16) (_ctrl_pressed ? 0000001 : 0000000);
    case PDP_KEYBOARD_SHIFT_FLAG_MEMORY_ADDRESS:
      return (uint16) (_ctrl_pressed ? 0000001 : 0000000);
    case PDP_KEYBOARD_CTRL_FLAG_MEMORY_ADDRESS:
      return (uint16) (_ctrl_pressed ? 0000001 : 0000000);
    default:
      throw new runtime_error("Wrong memory address PDPKeyboard");
  }
}

uint8 PDPKeyboard::read_byte(uint18 address, uint18 base_address) {
  uint16 symbol;
  switch (address) {
    case PDP_KEYBOARD_SYMBOL_MEMORY_ADDRESS:
    case PDP_KEYBOARD_SYMBOL_MEMORY_ADDRESS + 1:
      if (_buffer.size() == 0)
        return '\0';
      symbol = (uint16) _buffer.at(0);
      _buffer.erase(0);
      return symbol;
    case PDP_KEYBOARD_ALT_FLAG_MEMORY_ADDRESS:
    case PDP_KEYBOARD_ALT_FLAG_MEMORY_ADDRESS + 1:
      return (uint8) (_ctrl_pressed ? 0001 : 0000);
    case PDP_KEYBOARD_SHIFT_FLAG_MEMORY_ADDRESS:
    case PDP_KEYBOARD_SHIFT_FLAG_MEMORY_ADDRESS + 1:
      return (uint8) (_ctrl_pressed ? 0001 : 0000);
    case PDP_KEYBOARD_CTRL_FLAG_MEMORY_ADDRESS:
    case PDP_KEYBOARD_CTRL_FLAG_MEMORY_ADDRESS + 1:
      return (uint8) (_ctrl_pressed ? 0001 : 0000);
    default:
      throw new runtime_error("Wrong memory address PDPKeyboard");
  }
}

void PDPKeyboard::write_word(uint18 address, uint18 base_address, uint16 value) {
  throw new runtime_error("PDPKeyboard is read-only device");
}

void PDPKeyboard::write_byte(uint18 address, uint18 base_address, uint8 value) {
  throw new runtime_error("PDPKeyboard is read-only device");
}
