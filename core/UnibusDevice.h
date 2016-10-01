//
// Created by Aleksandr Parfenov on 01.10.16.
//

#ifndef PDP_11_EMULATOR_UNIBUSDEVICE_H
#define PDP_11_EMULATOR_UNIBUSDEVICE_H

#include "../common.h"

class UnibusDevice {
public:
  virtual string get_name() = 0;
  virtual uint16 read(uint18 address) = 0;
  virtual void write(uint18 address, uint16 value) = 0;
};

#endif //PDP_11_EMULATOR_UNIBUSDEVICE_H
