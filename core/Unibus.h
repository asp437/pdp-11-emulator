//
// Created by Aleksandr Parfenov on 01.10.16.
//

#ifndef PDP_11_EMULATOR_UNIBUS_H
#define PDP_11_EMULATOR_UNIBUS_H

#include "../common.h"

#include <vector>

#include "UnibusDevice.h"

class UnibusDeviceConfiguration;

class Unibus {
public:
  bool register_device(UnibusDevice *device, uint18 base_address, uint18 reserve_space_size);
  uint16 read(uint18 address);
  uint16 write(uint18 address, uint16 value);
  static int get_magic();
private:
  UnibusDeviceConfiguration *get_registered_device(uint18 address);

  vector<UnibusDeviceConfiguration *> _registered_devices;
};

#endif //PDP_11_EMULATOR_UNIBUS_H
