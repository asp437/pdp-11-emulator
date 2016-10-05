//
// Created by Aleksandr Parfenov on 01.10.16.
//

#include "ROM.h"
#include <cstring>
#include <fstream>

ROM::ROM(string rom_file) {
  _unibus = nullptr;
  ifstream ifs(rom_file, ios::binary);
  if (!ifs.is_open())
    throw new runtime_error("Can't open ROM file");

  ifs.seekg(0, ios_base::end);
  uint rom_file_size = ifs.tellg();
  ifs.seekg(0, ios_base::beg);
  _rom_size = rom_file_size < ROM_DEFAULT_SIZE ? rom_file_size : ROM_DEFAULT_SIZE;
  _rom_array = (uint8*) calloc(_rom_size, sizeof(uint8));
  for (uint16 i = 0; i < _rom_size; i++) {
    char b;
    ifs.read(&b, 1);
    _rom_array[i] = (uint8) b;
  }
  ifs.close();
  cout << "ROM loading complete. ROM size " << _rom_size << " bytes." << endl;
}

ROM::~ROM() {
  _rom_size = 0;
  free(_rom_array);
  _rom_array = nullptr;
}

void ROM::reset() {
  memset(_rom_array, 0, _rom_size * sizeof(uint8));
}

uint16 ROM::read_word(uint18 address, uint18 base_address) {
  if ((address - base_address) >= _rom_size)
    throw new runtime_error("Illegal ROM address access");
  if ((address & 0x1) == 0x1)
    throw new runtime_error("Illegal ROM address access");
  uint16 val = _rom_array[address - base_address] | (_rom_array[address - base_address + 1] << 8);
  return val;
}

void ROM::write_word(uint18 address, uint18 base_address, uint16 value) {
  throw new runtime_error("ROM doesn't support write operations");
}

uint8 ROM::read_byte(uint18 address, uint18 base_address) {
  if ((address - base_address) >= _rom_size)
    throw new runtime_error("Illegal ROM address access");
  return _rom_array[address - base_address];
}

void ROM::write_byte(uint18 address, uint18 base_address, uint8 value) {
  throw new runtime_error("ROM doesn't support write operations");
}
