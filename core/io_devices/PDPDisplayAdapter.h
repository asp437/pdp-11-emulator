//
// Created by Aleksandr Parfenov on 03.10.16.
//

#ifndef PDP_11_EMULATOR_PDPDISPLAYADAPTER_H
#define PDP_11_EMULATOR_PDPDISPLAYADAPTER_H

#include "../../common.h"
#include "../UnibusDevice.h"

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

  static const uint8 PDP_VIDEO_ADAPTER_MODE_4X3_1_BIT = 0; // 588x441
  static const uint8 PDP_VIDEO_ADAPTER_MODE_4X3_2_BIT = 2; // 416x312
  static const uint8 PDP_VIDEO_ADAPTER_MODE_4X3_3_BIT = 4; // 340x255
  static const uint8 PDP_VIDEO_ADAPTER_MODE_4X3_4_BIT = 6; // 292x219
  static const uint8 PDP_VIDEO_ADAPTER_MODE_4X3_5_BIT = 8; // 264x198
  static const uint8 PDP_VIDEO_ADAPTER_MODE_4X3_6_BIT = 10; // 240x180
  static const uint8 PDP_VIDEO_ADAPTER_MODE_4X3_7_BIT = 12; // 220x165
  static const uint8 PDP_VIDEO_ADAPTER_MODE_4X3_8_BIT = 14; // 208x156
  static const uint8 PDP_VIDEO_ADAPTER_MODE_16X9_1_BIT = 16; // 672x378
  static const uint8 PDP_VIDEO_ADAPTER_MODE_16X9_2_BIT = 32; // 480x270
  static const uint8 PDP_VIDEO_ADAPTER_MODE_16X9_3_BIT = 48; // 384x216
  static const uint8 PDP_VIDEO_ADAPTER_MODE_16X9_4_BIT = 64; // 336x189
  static const uint8 PDP_VIDEO_ADAPTER_MODE_16X9_5_BIT = 80; // 304x171
  static const uint8 PDP_VIDEO_ADAPTER_MODE_16X9_6_BIT = 96; // 272x153
  static const uint8 PDP_VIDEO_ADAPTER_MODE_16X9_7_BIT = 112; // 256x144
  static const uint8 PDP_VIDEO_ADAPTER_MODE_16X9_8_BIT = 128; // 240x135

  static const uint8 PDP_VIDEO_ADAPTER_MODE_USE_PALETTE = 1; // ORed with another video mode
  static const uint8 PDP_VIDEO_ADAPTER_MODE_NO_PALETTE = 0; // ORed with another video mode

  static const uint16 PDP_VIDEO_ADAPTER_VRAM_ADDRESS = 0040000;
  static const uint16 PDP_VIDEO_ADAPTER_VRAM_SIZE = 0100000;
  static const uint16 PDP_VIDEO_ADAPTER_IO_PALETTE_ADDRESS = 0176200;
  static const uint16 PDP_VIDEO_ADAPTER_IO_PALETTE_SIZE = 0000100;
  static const uint16 PDP_VIDEO_ADAPTER_MODE_ADDRESS = 0176300;
  static const uint16 PDP_VIDEO_ADAPTER_IO_FUNCTION_ADDRESS = 0176302;
  static const uint16 PDP_VIDEO_ADAPTER_IO_ARGUMENT0_ADDRESS = 0176304;
  static const uint16 PDP_VIDEO_ADAPTER_IO_ARGUMENT1_ADDRESS = 0176306;
  static const uint16 PDP_VIDEO_ADAPTER_IO_SIZE = 0000010;
private:
  void execute_function(uint16 opcode);
  uint16 get_width();
  uint16 get_height();
  void set_pixel(uint16 x, uint16 y, uint8 color);
  void print_char(uint16 x, uint16 y, uint8 char_code);

  uint8 *_vram;
  uint16 _vram_size;
  uint16 *_palette;
  uint16 _palette_size;
  uint8 _mode;
  uint16 _arg0;
  uint16 _arg1;
};

#endif //PDP_11_EMULATOR_PDPDISPLAYADAPTER_H
