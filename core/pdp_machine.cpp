//
// Created by Aleksandr Parfenov on 06.10.16.
//

#include "pdp_machine.h"
#include "unibus/unibus.h"
#include "memory/ram.h"
#include "cpu.h"
#include "memory/rom.h"
#include "utils/disasm.h"
#include "io_devices/pdp_keyboard.h"
#include "io_devices/pdp_display_adapter.h"

PDPMachine::PDPMachine(string rom_file) : _rom_file_name(rom_file) {
    _unibus = new Unibus();
    _memory = new RAM(MEMORY_SIZE);
    _rom = new ROM(rom_file);
    _cpu = new CPU();
    _keyboard = new PDPKeyboard();
    _display_adapter = new PDPDisplayAdapter();
    _unibus->register_device(_memory, 0, _memory->get_memory_size());
    _unibus->register_device(_cpu, CPU::BASE_MEM_MAP_SEGMENT_ADDRESS, CPU::BASE_MEM_MAP_SEGMENT_SIZE);
    _unibus->register_device(_rom, ROM::ROM_DEFAULT_OFFSET, ROM::ROM_DEFAULT_SIZE);
    _unibus->register_device(_keyboard,
                             PDPKeyboard::PDP_KEYBOARD_SYMBOL_REGISTER,
                             PDPKeyboard::PDP_KEYBOARD_REGISTERS_SIZE);
    _unibus->register_device(_display_adapter,
                             PDPDisplayAdapter::PDP_VIDEO_ADAPTER_VRAM_ADDRESS,
                             PDPDisplayAdapter::PDP_VIDEO_ADAPTER_VRAM_SIZE);
    _disasm = new DisAsm(_unibus, _cpu->get_instruction_set());
}

PDPMachine::~PDPMachine() {
    delete _unibus;
    delete _display_adapter;
    delete _keyboard;
    delete _cpu;
    delete _rom;
    delete _memory;
}

void PDPMachine::execute_command() {
    _unibus->master_device_execute();
}

uint16 PDPMachine::get_memory_word(uint18 address) {
    return _unibus->read_word(address);
}

CPUState PDPMachine::get_cpu_state() {
    CPUState state;
    for (int i = 0; i < 8; i++)
        state.r[i] = _cpu->get_register(i).r;
    state.psw = _cpu->get_psw().ps;
    return state;
}

bool PDPMachine::is_halted() {
    return _cpu->is_halted();
}
vector<pair<string, uint16>> PDPMachine::get_disasm(uint18 address, uint18 size) {
    return _disasm->disasm_code(address, size);
}

void PDPMachine::key_pressed(uint keycode, bool key_down) {
    _keyboard->key_pressed(keycode, key_down);
}

void PDPMachine::reset_timer() {
    _cpu->reset_timer();
}
