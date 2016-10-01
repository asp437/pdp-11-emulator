//
// Created by Aleksandr Parfenov on 01.10.16.
//

#ifndef PDP_11_EMULATOR_MEMORY_H
#define PDP_11_EMULATOR_MEMORY_H

#include "../common.h"

#include "UnibusDevice.h"

class Memory : public UnibusDevice {
public:
  Memory(uint16 memory_size);

  virtual string get_name();
  virtual uint16 read_word(uint18 address);
  virtual void write_word(uint18 address, uint16 value);
  virtual uint8 read_byte(uint18 address);
  virtual void write_byte(uint18 address, uint8 value);
  uint16 get_memory_size();
private:
  uint8 *_memory_array;
  uint16 _memory_size;
};

#endif //PDP_11_EMULATOR_MEMORY_H
