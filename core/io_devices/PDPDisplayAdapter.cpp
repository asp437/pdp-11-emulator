//
// Created by Aleksandr Parfenov on 03.10.16.
//

#include "PDPDisplayAdapter.h"
#include <cstring>

PDPDisplayAdapter::PDPDisplayAdapter() {
    _vram_size = PDP_VIDEO_ADAPTER_VRAM_SIZE;
    _vram = (uint8 *) calloc(_vram_size, sizeof(uint8));
    reset();
}

PDPDisplayAdapter::~PDPDisplayAdapter() {
    _vram_size = 0;
    free(_vram);
    _vram = nullptr;
}

void PDPDisplayAdapter::reset() {
    memset(_vram, 0, _vram_size * sizeof(uint8));
}

uint16 PDPDisplayAdapter::read_word(uint18 address, uint18 base_address) {
    if (address >= PDP_VIDEO_ADAPTER_VRAM_ADDRESS
        && address < PDP_VIDEO_ADAPTER_VRAM_ADDRESS + PDP_VIDEO_ADAPTER_VRAM_SIZE) {
        uint16 result = _vram[address - PDP_VIDEO_ADAPTER_VRAM_ADDRESS + 1];
        result = (result << 8) | (_vram[address - PDP_VIDEO_ADAPTER_VRAM_ADDRESS]);
        return result;
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
        case 0010000: // SET_PIXEL
            set_pixel(_arg0, _arg1, operand12);
            break;
    }
}

uint16 PDPDisplayAdapter::get_width() {
    return 400;
}

uint16 PDPDisplayAdapter::get_height() {
    return 300;
}

void PDPDisplayAdapter::set_pixel(uint16 x, uint16 y, uint8 color) {
    uint32 pixel_index = x + y * get_width();
    uint16 pixel_vram_offset = pixel_index >> 2;
    uint16 pixel_vram_internal_offset = (pixel_index % 4) << 1;
    uint8 mask = (uint8) (0x3 << pixel_vram_internal_offset);

    uint8 new_color = _vram[pixel_vram_offset];
    color = color << pixel_vram_internal_offset;
    color = color & mask;
    mask = ~mask;
    new_color = new_color & mask; // Reset selected pixel
    new_color = new_color | color;
    _vram[pixel_vram_offset] = new_color;
}

uint PDPDisplayAdapter::get_pixel(uint16 x, uint16 y) {
    uint32 pixel_index = x + y * get_width();
    uint16 pixel_vram_offset = pixel_index >> 2;
    uint16 pixel_vram_internal_offset = (pixel_index % 4) << 1;
    uint8 mask = (uint8) (0x3 << pixel_vram_internal_offset);

    uint color = 0;
    color = _vram[pixel_vram_offset] & mask;
    color = color >> pixel_vram_internal_offset;
    color = color * 85;
    return color << 16 | color << 8 | color;
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
