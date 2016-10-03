//
// Created by Aleksandr Parfenov on 01.10.16.
//

#include <cstring>
#include "Memory.h"

Memory::Memory(uint16 memory_size) : _memory_size(memory_size) {
  if ((memory_size & 0x1) == 0x1)
    throw new runtime_error("Wrong memory size");
  _memory_array = (uint8 *) calloc(memory_size, sizeof(uint8));
  Memory::reset();
}

Memory::~Memory()
{
  _memory_size = 0;
  free(_memory_array);
  _memory_array = nullptr;
}

string Memory::get_name() {
  return "Main Random Access Memory";
}

void Memory::register_unibus(Unibus *unibus)
{
  this->_unibus = unibus;
}

void Memory::reset()
{
  memset(_memory_array, 0, sizeof(uint8) * _memory_size);
}

uint16 Memory::read_word(uint18 address, uint18 base_address) {
  if ((address - base_address) >= _memory_size)
    throw new runtime_error("Illegal memory address access");
  if ((address & 0x1) == 0x1)
    throw new runtime_error("Illegal memory address access");
  return _memory_array[address - base_address] | (_memory_array[address - base_address + 1] << 8);
}

void Memory::write_word(uint18 address, uint18 base_address, uint16 value) {
  if ((address - base_address) >= _memory_size)
    throw new runtime_error("Illegal memory address access");
  if ((address & 0x1) == 0x1)
    throw new runtime_error("Illegal memory address access");
  _memory_array[address - base_address] = (uint8) (value & 0xFF);
  _memory_array[address - base_address + 1] = (uint8) ((value & 0xFF00) >> 8);
}

uint8 Memory::read_byte(uint18 address, uint18 base_address) {
  if ((address - base_address) >= _memory_size)
    throw new runtime_error("Illegal memory address access");
  return _memory_array[address - base_address];
}

void Memory::write_byte(uint18 address, uint18 base_address, uint8 value) {
  if ((address - base_address) >= _memory_size)
    throw new runtime_error("Illegal memory address access");
  _memory_array[address - base_address] = value;
}

uint16 Memory::get_memory_size() const {
  return _memory_size;
}
