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
  _palette32 = (uint32 *)calloc(_palette_size, sizeof(uint32));
  _mode = PDP_VIDEO_ADAPTER_MODE_4X3_4_BIT | PDP_VIDEO_ADAPTER_MODE_USE_PALETTE;
  reset();
}

PDPDisplayAdapter::~PDPDisplayAdapter() {
  _palette_size = 0;
  _vram_size = 0;
  free(_palette);
  free(_vram);
  free(_palette32);
  _palette = nullptr;
  _vram = nullptr;
  _palette32 = nullptr;
}

void PDPDisplayAdapter::reset() {
  memset(_vram, 0, _vram_size * sizeof(uint8));
  memset(_palette, 0, _palette_size * sizeof(uint8));
  _mode = PDP_VIDEO_ADAPTER_MODE_4X3_4_BIT | PDP_VIDEO_ADAPTER_MODE_USE_PALETTE;
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
    _palette[(address - PDP_VIDEO_ADAPTER_IO_PALETTE_ADDRESS) >> 1] = value;
    int16 color16 = value;
    uint r = (color16 >> 11) & 077,
      g = (color16 >> 6) & 037,
      b = (color16) & 077;
    r = r << 2;
    g = g << 3;
    b = b << 2;
    _palette32[(address - PDP_VIDEO_ADAPTER_IO_PALETTE_ADDRESS) >> 1] = r << 16 | g << 8 | b;
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
    case PDP_VIDEO_ADAPTER_MODE_4X3_4_BIT:
      return 292;
    case PDP_VIDEO_ADAPTER_MODE_4X3_8_BIT:
      return 208;
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
    case PDP_VIDEO_ADAPTER_MODE_4X3_4_BIT:
      return 219;
    case PDP_VIDEO_ADAPTER_MODE_4X3_8_BIT:
      return 156;
    default:
      throw new runtime_error("Unsupported video mode");
  }
}

void PDPDisplayAdapter::set_pixel(uint16 x, uint16 y, uint8 color) {
  uint16 pixel_index = x + y * get_width();
  uint8 color_depth = (uint8)((_mode >> 1) + 1);
  uint16 pixel_vram_offset = (uint16) (pixel_index * color_depth / 8);
  uint16 pixel_vram_internal_offset = pixel_index - (uint16) (pixel_vram_offset * 8 / color_depth);
  pixel_vram_internal_offset *= color_depth;

  uint8 new_color = _vram[pixel_vram_offset];
  uint8 mask = (uint8) (((1 << color_depth) - 1) << pixel_vram_internal_offset);
  color = color << pixel_vram_internal_offset;
  color = color & mask;
  mask = ~mask;
  new_color = new_color & mask; // Reset selected pixel
  new_color = new_color | color;
  _vram[pixel_vram_offset] = new_color;
}

uint PDPDisplayAdapter::get_pixel(uint16 x, uint16 y) {
  uint16 pixel_index = x + y * get_width();
  uint8 color_depth = (uint8)((_mode >> 1) + 1);
  uint16 pixel_vram_offset = (uint16)((pixel_index * color_depth) >> 3);
  uint16 pixel_vram_internal_offset = pixel_index - (uint16)((pixel_vram_offset << 3) / color_depth);
  pixel_vram_internal_offset *= color_depth;
  uint color = 0;

  uint8 mask = (uint8) (((1 << color_depth) - 1) << pixel_vram_internal_offset);
  color = _vram[pixel_vram_offset] & mask;
  color = color >> pixel_vram_internal_offset;

  if ((_mode & 0001) == 0001) { // Palette mode
    color = _palette32[color];
  } else {
    uint r, g, b;
    switch (color_depth) {
      case 1:
        r = g = b = color << 7;
        break;
      case 2:
        r = g = b = color << 6;
        break;
      case 4:
        r = g = b = color << 4;
        break;
      case 8:
        // TODO: Convert to RGB
        r = g = b = color;
        break;
    }
    color = r << 16 | g << 8 | b;
  }
  return color;
}

void PDPDisplayAdapter::print_char(uint16 x, uint16 y, uint8 char_code) {
  // TODO: Implement print_char
}

vector<vector<int>> &PDPDisplayAdapter::get_video_buffer() {
  uint16 height = get_height(), width = get_width();
  if (_buffer.size() != width) {
    _buffer.resize(width);
    for (uint16 x = 0; x < width; x++)
      _buffer[x].resize(height);
  }

  for (uint16 y = 0; y < height; y++) {
    for (uint16 x = 0; x < width; x++) {
      _buffer[x][y] = get_pixel(x, y);
    }
  }

  return _buffer;
}
