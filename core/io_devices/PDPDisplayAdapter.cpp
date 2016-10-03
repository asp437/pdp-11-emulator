//
// Created by Aleksandr Parfenov on 03.10.16.
//

#include "PDPDisplayAdapter.h"
#include <cstring>

PDPDisplayAdapter::PDPDisplayAdapter() {
  _vram_size = PDP_VIDEO_ADAPTER_VRAM_SIZE;
  _vram = (uint8 *) calloc(_vram_size, sizeof(uint8));
  _palette_size = PDP_VIDEO_ADAPTER_IO_PALETTE_SIZE;
  _palette = (uint16 *) calloc(_palette_size, sizeof(uint8));
  reset();
}

PDPDisplayAdapter::~PDPDisplayAdapter() {
  _palette_size = 0;
  _vram_size = 0;
  free(_palette);
  free(_vram);
  _palette = nullptr;
  _vram = nullptr;
}

void PDPDisplayAdapter::reset() {
  memset(_vram, 0, _vram_size * sizeof(uint8));
  memset(_palette, 0, _palette_size * sizeof(uint8));
  _mode = PDP_VIDEO_ADAPTER_MODE_4X3_1_BIT;
}

uint16 PDPDisplayAdapter::read_word(uint18 address, uint18 base_address) {
  if (address >= PDP_VIDEO_ADAPTER_VRAM_ADDRESS
      && address < PDP_VIDEO_ADAPTER_VRAM_ADDRESS + PDP_VIDEO_ADAPTER_VRAM_SIZE) {
    uint16 result = _vram[address - PDP_VIDEO_ADAPTER_VRAM_ADDRESS + 1];
    result = (result << 8) | (_vram[address - PDP_VIDEO_ADAPTER_VRAM_ADDRESS]);
    return result;
  } else if (address >= PDP_VIDEO_ADAPTER_IO_PALETTE_ADDRESS
      && address < PDP_VIDEO_ADAPTER_IO_PALETTE_ADDRESS + PDP_VIDEO_ADAPTER_IO_PALETTE_SIZE) {
    uint16 result = _palette[address - PDP_VIDEO_ADAPTER_IO_PALETTE_ADDRESS];
    return result;
  } else if (address == PDP_VIDEO_ADAPTER_MODE_ADDRESS) {
    return _mode;
  } else if (address == PDP_VIDEO_ADAPTER_IO_FUNCTION_ADDRESS) {
    throw new runtime_error("PDP Video Adapter IO Function register is write-only");
  } else if (address == PDP_VIDEO_ADAPTER_IO_ARGUMENT0_ADDRESS) {
    return _arg0;
  } else if (address == PDP_VIDEO_ADAPTER_IO_ARGUMENT1_ADDRESS) {
    return _arg1;
  } else {
    throw new runtime_error("Wrong memory address PDPDisplayAdapter");
  }
}

void PDPDisplayAdapter::write_word(uint18 address, uint18 base_address, uint16 value) {
  if (address >= PDP_VIDEO_ADAPTER_VRAM_ADDRESS
      && address < PDP_VIDEO_ADAPTER_VRAM_ADDRESS + PDP_VIDEO_ADAPTER_VRAM_SIZE) {
    _vram[address - PDP_VIDEO_ADAPTER_VRAM_ADDRESS] = (uint8) value;
    _vram[address - PDP_VIDEO_ADAPTER_VRAM_ADDRESS + 1] = (uint8) (value >> 8);
  } else if (address >= PDP_VIDEO_ADAPTER_IO_PALETTE_ADDRESS
      && address < PDP_VIDEO_ADAPTER_IO_PALETTE_ADDRESS + PDP_VIDEO_ADAPTER_IO_PALETTE_SIZE) {
    _palette[address - PDP_VIDEO_ADAPTER_IO_PALETTE_ADDRESS] = value;
  } else if (address == PDP_VIDEO_ADAPTER_MODE_ADDRESS) {
    _mode = (uint8) value;
  } else if (address == PDP_VIDEO_ADAPTER_IO_FUNCTION_ADDRESS) {
    execute_function(value);
  } else if (address == PDP_VIDEO_ADAPTER_IO_ARGUMENT0_ADDRESS) {
    _arg0 = value;
  } else if (address == PDP_VIDEO_ADAPTER_IO_ARGUMENT1_ADDRESS) {
    _arg1 = value;
  } else {
    throw new runtime_error("Wrong memory address PDPDisplayAdapter");
  }
}

uint8 PDPDisplayAdapter::read_byte(uint18 address, uint18 base_address) {
  if (address >= PDP_VIDEO_ADAPTER_VRAM_ADDRESS
      && address < PDP_VIDEO_ADAPTER_VRAM_ADDRESS + PDP_VIDEO_ADAPTER_VRAM_SIZE) {
    uint8 result = _vram[address - PDP_VIDEO_ADAPTER_VRAM_ADDRESS];
    return result;
  } else if (address >= PDP_VIDEO_ADAPTER_IO_PALETTE_ADDRESS
      && address < PDP_VIDEO_ADAPTER_IO_PALETTE_ADDRESS + PDP_VIDEO_ADAPTER_IO_PALETTE_SIZE) {
    throw new runtime_error("PDPDisplayAdapter Palette doesn't support byte access");
  } else if (address == PDP_VIDEO_ADAPTER_MODE_ADDRESS) {
    return _mode;
  } else if (address == PDP_VIDEO_ADAPTER_IO_FUNCTION_ADDRESS) {
    throw new runtime_error("PDPDisplayAdapter IO Functions doesn't support byte access");
  } else if (address == PDP_VIDEO_ADAPTER_IO_ARGUMENT0_ADDRESS) {
    throw new runtime_error("PDPDisplayAdapter IO Functions doesn't support byte access");
  } else if (address == PDP_VIDEO_ADAPTER_IO_ARGUMENT1_ADDRESS) {
    throw new runtime_error("PDPDisplayAdapter IO Functions doesn't support byte access");
  } else {
    throw new runtime_error("Wrong memory address PDPDisplayAdapter");
  }
}

void PDPDisplayAdapter::write_byte(uint18 address, uint18 base_address, uint8 value) {
  if (address >= PDP_VIDEO_ADAPTER_VRAM_ADDRESS
      && address < PDP_VIDEO_ADAPTER_VRAM_ADDRESS + PDP_VIDEO_ADAPTER_VRAM_SIZE) {
    _vram[address - PDP_VIDEO_ADAPTER_VRAM_ADDRESS] = value;
  } else if (address >= PDP_VIDEO_ADAPTER_IO_PALETTE_ADDRESS
      && address < PDP_VIDEO_ADAPTER_IO_PALETTE_ADDRESS + PDP_VIDEO_ADAPTER_IO_PALETTE_SIZE) {
    throw new runtime_error("PDPDisplayAdapter Palette doesn't support byte access");
  } else if (address == PDP_VIDEO_ADAPTER_MODE_ADDRESS) {
    _mode = value;
  } else if (address == PDP_VIDEO_ADAPTER_IO_FUNCTION_ADDRESS) {
    throw new runtime_error("PDPDisplayAdapter IO Functions doesn't support byte access");
  } else if (address == PDP_VIDEO_ADAPTER_IO_ARGUMENT0_ADDRESS) {
    throw new runtime_error("PDPDisplayAdapter IO Functions doesn't support byte access");
  } else if (address == PDP_VIDEO_ADAPTER_IO_ARGUMENT1_ADDRESS) {
    throw new runtime_error("PDPDisplayAdapter IO Functions doesn't support byte access");
  } else {
    throw new runtime_error("Wrong memory address PDPDisplayAdapter");
  }
}

void PDPDisplayAdapter::execute_function(uint16 opcode) {
  // TODO: Execute received operation
  uint16 operand12 = (uint16) (opcode & 0007777); // 1.5 byte operand
  uint8 operand8 = (uint8) (opcode & 0000377); // 1 byte operand
  switch (opcode & 0770000) {
    case 0110000: // CLEAR_SCREEN
      memset(_vram, operand8, _vram_size);
      break;
    case 0100000: // PRINT_CHAR
      break;
    case 0010000: // SET_PIXEL
      set_pixel(_arg0, _arg1, operand12);
      break;
  }
}

uint16 PDPDisplayAdapter::get_width() {
  switch (_mode & 0376) {
    case PDP_VIDEO_ADAPTER_MODE_4X3_1_BIT:
      return 588;
    case PDP_VIDEO_ADAPTER_MODE_4X3_2_BIT:
      return 416;
    case PDP_VIDEO_ADAPTER_MODE_4X3_3_BIT:
      return 340;
    case PDP_VIDEO_ADAPTER_MODE_4X3_4_BIT:
      return 292;
    case PDP_VIDEO_ADAPTER_MODE_4X3_5_BIT:
      return 264;
    case PDP_VIDEO_ADAPTER_MODE_4X3_6_BIT:
      return 240;
    case PDP_VIDEO_ADAPTER_MODE_4X3_7_BIT:
      return 220;
    case PDP_VIDEO_ADAPTER_MODE_4X3_8_BIT:
      return 208;
    case PDP_VIDEO_ADAPTER_MODE_16X9_1_BIT:
      return 672;
    case PDP_VIDEO_ADAPTER_MODE_16X9_2_BIT:
      return 480;
    case PDP_VIDEO_ADAPTER_MODE_16X9_3_BIT:
      return 384;
    case PDP_VIDEO_ADAPTER_MODE_16X9_4_BIT:
      return 336;
    case PDP_VIDEO_ADAPTER_MODE_16X9_5_BIT:
      return 304;
    case PDP_VIDEO_ADAPTER_MODE_16X9_6_BIT:
      return 272;
    case PDP_VIDEO_ADAPTER_MODE_16X9_7_BIT:
      return 256;
    case PDP_VIDEO_ADAPTER_MODE_16X9_8_BIT:
      return 240;
    default:
      throw new runtime_error("Unsupported video mode");
  }
}

uint16 PDPDisplayAdapter::get_height() {
  switch (_mode & 0376) {
    case PDP_VIDEO_ADAPTER_MODE_4X3_1_BIT:
      return 441;
    case PDP_VIDEO_ADAPTER_MODE_4X3_2_BIT:
      return 312;
    case PDP_VIDEO_ADAPTER_MODE_4X3_3_BIT:
      return 255;
    case PDP_VIDEO_ADAPTER_MODE_4X3_4_BIT:
      return 219;
    case PDP_VIDEO_ADAPTER_MODE_4X3_5_BIT:
      return 198;
    case PDP_VIDEO_ADAPTER_MODE_4X3_6_BIT:
      return 180;
    case PDP_VIDEO_ADAPTER_MODE_4X3_7_BIT:
      return 165;
    case PDP_VIDEO_ADAPTER_MODE_4X3_8_BIT:
      return 156;
    case PDP_VIDEO_ADAPTER_MODE_16X9_1_BIT:
      return 378;
    case PDP_VIDEO_ADAPTER_MODE_16X9_2_BIT:
      return 270;
    case PDP_VIDEO_ADAPTER_MODE_16X9_3_BIT:
      return 216;
    case PDP_VIDEO_ADAPTER_MODE_16X9_4_BIT:
      return 189;
    case PDP_VIDEO_ADAPTER_MODE_16X9_5_BIT:
      return 171;
    case PDP_VIDEO_ADAPTER_MODE_16X9_6_BIT:
      return 153;
    case PDP_VIDEO_ADAPTER_MODE_16X9_7_BIT:
      return 144;
    case PDP_VIDEO_ADAPTER_MODE_16X9_8_BIT:
      return 135;
    default:
      throw new runtime_error("Unsupported video mode");
  }
}

void PDPDisplayAdapter::set_pixel(uint16 x, uint16 y, uint8 color) {
  uint16 pixel_index = _arg0 + _arg1 * get_width();
  uint8 color_depth = 0;
  if (_mode <= 15) { // 4x3 mode
    color_depth = (uint8) ((_mode >> 1) & 016 + 1);
  } else { // 16x9 mode
    color_depth = (uint8) ((_mode >> 4) & 017);
  }
  uint16 pixel_vram_offset = (uint16) (pixel_index / 8 * color_depth);
  uint16 pixel_vram_internal_offset = pixel_index - (uint16) (pixel_vram_offset * 8 / color_depth);
  if (pixel_vram_internal_offset + color_depth > 8) { // Pixel stored partial in two bytes
    uint8 new_color = _vram[pixel_vram_offset];
    uint8 mask = (uint8) ((2 << color_depth - 1) << pixel_vram_internal_offset);
    uint8 color1 = color << pixel_vram_internal_offset;
    color1 = color1 & mask;
    mask = ~mask;
    new_color = new_color & mask; // Reset selected pixel
    new_color = new_color | color1;
    _vram[pixel_vram_offset] = new_color;

    new_color = _vram[pixel_vram_offset + 1];
    mask = (uint8) ((2 << color_depth - 1) >> (8 - pixel_vram_internal_offset));
    uint8 color2 = color >> (8 - pixel_vram_internal_offset);
    color2 = color2 & mask;
    mask = ~mask;
    new_color = new_color & mask; // Reset selected pixel
    new_color = (new_color | color2);
    _vram[pixel_vram_offset] = new_color;
  } else {
    uint8 new_color = _vram[pixel_vram_offset];
    uint8 mask = (uint8) ((2 << color_depth - 1) << pixel_vram_internal_offset);
    color = color << pixel_vram_internal_offset;
    color = color & mask;
    mask = ~mask;
    new_color = new_color & mask; // Reset selected pixel
    new_color = new_color | color;
    _vram[pixel_vram_offset] = new_color;
  }
}

void PDPDisplayAdapter::print_char(uint16 x, uint16 y, uint8 char_code) {
  // TODO: Implement print_char
}
