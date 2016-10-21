//
// Created by Aleksandr Parfenov on 03.10.16.
//

#include "pdp_display_adapter.h"
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
    } else {
        throw new runtime_error("Wrong memory address PDPDisplayAdapter");
    }
}

void PDPDisplayAdapter::write_word(uint18 address, uint18 base_address, uint16 value) {
    if (address >= PDP_VIDEO_ADAPTER_VRAM_ADDRESS
        && address < PDP_VIDEO_ADAPTER_VRAM_ADDRESS + PDP_VIDEO_ADAPTER_VRAM_SIZE) {
        _vram[address - PDP_VIDEO_ADAPTER_VRAM_ADDRESS] = (uint8) value;
        _vram[address - PDP_VIDEO_ADAPTER_VRAM_ADDRESS + 1] = (uint8) (value >> 8);
    } else {
        throw new runtime_error("Wrong memory address PDPDisplayAdapter");
    }
}

uint8 PDPDisplayAdapter::read_byte(uint18 address, uint18 base_address) {
    if (address >= PDP_VIDEO_ADAPTER_VRAM_ADDRESS
        && address < PDP_VIDEO_ADAPTER_VRAM_ADDRESS + PDP_VIDEO_ADAPTER_VRAM_SIZE) {
        uint8 result = _vram[address - PDP_VIDEO_ADAPTER_VRAM_ADDRESS];
        return result;
    } else {
        throw new runtime_error("Wrong memory address PDPDisplayAdapter");
    }
}

void PDPDisplayAdapter::write_byte(uint18 address, uint18 base_address, uint8 value) {
    if (address >= PDP_VIDEO_ADAPTER_VRAM_ADDRESS
        && address < PDP_VIDEO_ADAPTER_VRAM_ADDRESS + PDP_VIDEO_ADAPTER_VRAM_SIZE) {
        _vram[address - PDP_VIDEO_ADAPTER_VRAM_ADDRESS] = value;
    } else {
        throw new runtime_error("Wrong memory address PDPDisplayAdapter");
    }
}

uint PDPDisplayAdapter::get_pixel(uint16 x, uint16 y) {
    uint32 pixel_index = x + y * get_width();
    uint16 pixel_vram_offset = (uint16) (pixel_index >> 2);
    uint16 pixel_vram_internal_offset = (uint16) ((pixel_index % 4) << 1);
    uint8 mask = (uint8) (0x3 << pixel_vram_internal_offset);

    uint color = 0;
    color = _vram[pixel_vram_offset] & mask;
    color = color >> pixel_vram_internal_offset;
    color = color * 85;
    return color << 16 | color << 8 | color;
}
