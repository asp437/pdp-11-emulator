//
// Created by Aleksandr Parfenov on 01.10.16.
//

#include "Unibus.h"
#include <iomanip>

class UnibusDeviceConfiguration {
public:
  UnibusDeviceConfiguration(UnibusDevice *device, uint18 base_address, uint18 reserve_space_size) :
      _device(device), _base_address(base_address), _reserved_space_size(reserve_space_size) {
    ios_base::fmtflags cout_flags = cout.flags();
    cout << "UnibusDevice <" << device->get_name() << "> was registered at bus with parameters (base_address="
      << oct << setfill('0') << setw(7) << base_address << ", reserve_space_size=" << oct << setfill('0') << setw(7) << reserve_space_size << ")" << endl;

    cout << setfill(' ') << setw(0);
    cout.flags(cout_flags);
  }

  UnibusDevice *get_device() const { return _device; }
  uint18 get_base_address() const { return _base_address; }
  uint18 get_reserved_space_size() const { return _reserved_space_size; }
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
  device_configuration->get_device()->register_unibus(this);
  _registered_devices.push_back(device_configuration);
  return true;
}

uint16 Unibus::read_word(uint18 address) {
  UnibusDeviceConfiguration *configured_device = get_registered_device(address);
  return configured_device == nullptr ? (uint16) 0 : configured_device->get_device()->read_word(address, configured_device->get_base_address());
}

void Unibus::write_word(uint18 address, uint16 value) {
  UnibusDeviceConfiguration *configured_device = get_registered_device(address);
  if (configured_device != nullptr)
    configured_device->get_device()->write_word(address, configured_device->get_base_address(), value);
}

uint8 Unibus::read_byte(uint18 address) {
  UnibusDeviceConfiguration *configured_device = get_registered_device(address);
  return configured_device == nullptr ? (uint8) 0 : configured_device->get_device()->read_byte(address, configured_device->get_base_address());
}

void Unibus::write_byte(uint18 address, uint8 value) {
  UnibusDeviceConfiguration *configured_device = get_registered_device(address);
  if (configured_device != nullptr)
    configured_device->get_device()->write_byte(address, configured_device->get_base_address(), value);
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
  // TODO: Time snchronization
  for (auto it = _registered_devices.begin(); it != _registered_devices.end(); ++it) {
    (*it)->get_device()->reset();
  }
}
