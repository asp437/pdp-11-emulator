//
// Created by Aleksandr Parfenov on 03.10.16.
//

#ifndef PDP_11_EMULATOR_PDPKEYBOARD_H
#define PDP_11_EMULATOR_PDPKEYBOARD_H

#include "../../common.h"
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

  static const uint18 PDP_KEYBOARD_SYMBOL_MEMORY_ADDRESS = 0176000;
  static const uint18 PDP_KEYBOARD_ALT_FLAG_MEMORY_ADDRESS = 0176002;
  static const uint18 PDP_KEYBOARD_SHIFT_FLAG_MEMORY_ADDRESS = 0176004;
  static const uint18 PDP_KEYBOARD_CTRL_FLAG_MEMORY_ADDRESS = 0176006;
  static const uint18 PDP_KEYBOARD_MEM_REGISTERS_ADDRESS = 0176000;
  static const uint18 PDP_KEYBOARD_MEM_REGISTERS_SIZE = 0000010;
private:
  string _buffer;
  bool _alt_pressed;
  bool _ctrl_pressed;
  bool _shift_pressed;
};

#endif //PDP_11_EMULATOR_PDPKEYBOARD_H
