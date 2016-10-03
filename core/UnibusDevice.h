//
// Created by Aleksandr Parfenov on 01.10.16.
//

#ifndef PDP_11_EMULATOR_UNIBUSDEVICE_H
#define PDP_11_EMULATOR_UNIBUSDEVICE_H

#include "../common.h"

class Unibus;

class UnibusDevice {
public:
  virtual ~UnibusDevice() {};
  virtual string get_name() = 0;
  virtual void register_unibus(Unibus *unibus) { this->_unibus = unibus; };
  virtual void reset() = 0;
  virtual void interrupt(uint18 address) {}; // Implemented only in CPU
  virtual void execute() {};
  virtual bool is_busy() = 0;
  virtual uint16 read_word(uint18 address, uint18 base_address) = 0;
  virtual void write_word(uint18 address, uint18 base_address, uint16 value) = 0;
  virtual uint8 read_byte(uint18 address, uint18 base_address) = 0;
  virtual void write_byte(uint18 address, uint18 base_address, uint8 value) = 0;
protected:
  Unibus *_unibus;
};

#endif //PDP_11_EMULATOR_UNIBUSDEVICE_H
