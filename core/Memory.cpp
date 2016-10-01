//
// Created by Aleksandr Parfenov on 01.10.16.
//

#include "Memory.h"

Memory::Memory(uint16 memory_size) : _memory_size(memory_size) {
  if (memory_size & 0x1 == 0x1) // TODO: Throw exception
    cout << "Worng memory size" << endl;
  _memory_array = (uint8 *) calloc(memory_size, sizeof(uint8));
}

string Memory::get_name() {
  return "Main Memory";
}

uint16 Memory::read(uint18 address) {
  if (address >= _memory_size)
    return 0; // TODO: Throw exception
  if (address & 0x1 == 0x1)
    return 0; // TODO: Throw exception
  return _memory_array[address] | (_memory_array[address + 1] << 8);
}

void Memory::write(uint18 address, uint16 value) {
  if (address >= _memory_size)
    return; // TODO: Throw exception
  if (address & 0x1 == 0x1)
    return; // TODO: Throw exception
  _memory_array[address] = (uint8) (value & 0xFF);
  _memory_array[address + 1] = (uint8) ((value & 0xFF00) >> 8);
}
