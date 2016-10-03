//
// Created by Aleksandr Parfenov on 01.10.16.
//

#include "Unibus.h"

class UnibusDeviceConfiguration {
public:
  UnibusDeviceConfiguration(UnibusDevice *device, uint18 base_address, uint18 reserve_space_size) :
      _device(device), _base_address(base_address), _reserved_space_size(reserve_space_size) {
    cout << "UnibusDevice " << device->get_name() << " was registered at bus with parameters (base_address="
         << base_address << ", reserve_space_size=" << reserve_space_size << ")" << endl;
  }

  UnibusDevice *get_device() { return _device; }
  uint18 get_base_address() { return _base_address; }
  uint18 get_reserved_space_size() { return _reserved_space_size; }
private:
  UnibusDevice *_device;
  uint18 _base_address;
  uint18 _reserved_space_size;
};

int Unibus::get_magic() {
  return 42;
}

bool Unibus::register_device(UnibusDevice *device, uint18 base_address, uint18 reserve_space_size) {
  // TODO: Check address spaces intersections
  UnibusDeviceConfiguration
      *device_configuration = new UnibusDeviceConfiguration(device, base_address, reserve_space_size);
  _registered_devices.push_back(device_configuration);
  return true;
}

uint16 Unibus::read_word(uint18 address) {
  // TODO: Send raw or shifted address to device???
  UnibusDeviceConfiguration *configured_device = get_registered_device(address);
  return configured_device == nullptr ? (uint16) 0 : configured_device->get_device()->read_word(address);
}

void Unibus::write_word(uint18 address, uint16 value) {
  // TODO: Send raw or shifted address to device???
  UnibusDeviceConfiguration *configured_device = get_registered_device(address);
  if (configured_device != nullptr)
    configured_device->get_device()->write_word(address, value);
}

uint8 Unibus::read_byte(uint18 address) {
  // TODO: Send raw or shifted address to device???
  UnibusDeviceConfiguration *configured_device = get_registered_device(address);
  return configured_device == nullptr ? (uint8) 0 : configured_device->get_device()->read_byte(address);
}

void Unibus::write_byte(uint18 address, uint8 value) {
  // TODO: Send raw or shifted address to device???
  UnibusDeviceConfiguration *configured_device = get_registered_device(address);
  if (configured_device != nullptr)
    configured_device->get_device()->write_byte(address, value);
}

UnibusDeviceConfiguration *Unibus::get_registered_device(uint18 address) {
  for (auto it = _registered_devices.begin(); it != _registered_devices.end(); ++it) {
    if ((*it)->get_base_address() >= address
        && (*it)->get_base_address() + (*it)->get_reserved_space_size() < address) {
      return (*it);
    }
  }
  return nullptr;
}

void Unibus::set_init_line(uint32 ms) {
  // TODO: Implement set INIT line method
}
