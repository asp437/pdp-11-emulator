//
// Created by Aleksandr Parfenov on 06.10.16.
//

#include "pdp_machine.h"
#include "Unibus.h"
#include "Memory.h"
#include "CPU.h"
#include "ROM.h"
#include "io_devices/PDPKeyboard.h"
#include "io_devices/PDPTapeWriter.h"
#include "io_devices/PDPDisplayAdapter.h"

PDPMachine::PDPMachine(string rom_file) {
  _unibus = new Unibus();
  _memory = new Memory(MEMORY_SIZE);
  _rom = new ROM(rom_file);
  _cpu = new CPU();
  _keyboard = new PDPKeyboard();
  _tape_writer = new PDPTapeWriter();
  _display_adapter = new PDPDisplayAdapter();
  _unibus->register_device(_memory, 0, _memory->get_memory_size());
  _unibus->register_device(_cpu, CPU::BASE_MEM_MAP_SEGMENT_ADDRESS, CPU::BASE_MEM_MAP_SEGMENT_SIZE);
  _unibus->register_device(_rom, ROM::ROM_DEFAULT_OFFSET, ROM::ROM_DEFAULT_SIZE);
  _unibus->register_device(_keyboard,
    PDPKeyboard::PDP_KEYBOARD_MEM_REGISTERS_ADDRESS,
    PDPKeyboard::PDP_KEYBOARD_MEM_REGISTERS_SIZE);
  _unibus->register_device(_tape_writer,
    PDPTapeWriter::PDP_TAPE_WRITER_MEM_REGISTERS_ADDRESS,
    PDPTapeWriter::PDP_TAPE_WRITER_MEM_REGISTERS_SIZE);
  _unibus->register_device(_display_adapter,
    PDPDisplayAdapter::PDP_VIDEO_ADAPTER_VRAM_ADDRESS,
    PDPDisplayAdapter::PDP_VIDEO_ADAPTER_VRAM_SIZE);
  _unibus->register_device(_display_adapter,
    PDPDisplayAdapter::PDP_VIDEO_ADAPTER_IO_ADDRESS,
    PDPDisplayAdapter::PDP_VIDEO_ADAPTER_IO_SIZE);
}

PDPMachine::~PDPMachine() {
  delete _unibus;
  delete _display_adapter;
  delete _tape_writer;
  delete _keyboard;
  delete _cpu;
  delete _rom;
  delete _memory;
}

vector<vector<int>> &PDPMachine::get_video_buffer() {
  return _display_adapter->get_video_buffer();
}

void PDPMachine::execute_command() {
  _unibus->master_device_execute();
}

uint16 PDPMachine::get_memory_word(uint18 address) {
  return _unibus->read_word(address);
}

CPUState PDPMachine::get_cpu_state() {
  return CPUState();
}

bool PDPMachine::is_halted() {
  return _cpu->is_halted();
}
