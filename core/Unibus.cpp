//
// Created by Aleksandr Parfenov on 01.10.16.
//

#include "Unibus.h"
#include <iomanip>

const uint8 NPR_REQUEST_PRIORITY = 8;
const uint18 CPU_PSW_ADDRESS = 0177776;

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
    if ((*it)->get_base_address() <= address
        && (*it)->get_base_address() + (*it)->get_reserved_space_size() > address) {
      return (*it);
    }
  }
  return nullptr;
}

void Unibus::set_init_line(uint32 ms) {
  // TODO: Time synchronization
  for (auto it = _registered_devices.begin(); it != _registered_devices.end(); ++it) {
    (*it)->get_device()->reset();
  }
}

void Unibus::master_device_execute() {
  pair<uint8, UnibusDeviceConfiguration *> next_master = make_pair(0, (UnibusDeviceConfiguration *) nullptr);
  auto next_master_it = _master_requests_queue.end();

  uint8 cpu_priority_flag = (uint8) (read_word(CPU_PSW_ADDRESS) >> 5); // Read CPU PSW
  for (auto it = _master_requests_queue.begin(); it != _master_requests_queue.end(); ++it) {
    if ((*it).first < cpu_priority_flag)
      it = _master_requests_queue.erase(it);
    else if ((*it).first > next_master.first) {
      next_master = *it;
      next_master_it = it;
    }
  }

  if (next_master.second != nullptr && next_master.first > _master_device.first) {
    _master_requests_queue.erase(next_master_it);
    _master_device = next_master;
  } else {
    if (_master_device.second == nullptr || !_master_device.second->get_device()->is_busy()) {
      _master_device = make_pair(cpu_priority_flag, get_registered_device(CPU_PSW_ADDRESS));
    }
  }

  _master_device.second->get_device()->execute();
}

void Unibus::npr_request(UnibusDevice *device) {
  UnibusDeviceConfiguration *device_configuration = nullptr;
  for (auto it = _registered_devices.begin(); it != _registered_devices.end(); ++it) {
    if ((*it)->get_device() == device) {
      device_configuration = *it;
    }
  }
  if (device_configuration == nullptr)
    throw new runtime_error("Non registered device requests bus-master status");
  _master_requests_queue.push_back(make_pair(NPR_REQUEST_PRIORITY, device_configuration));
}

void Unibus::br_request(UnibusDevice *device, uint8 priority) {
  if (priority > 7 || priority < 4)
    throw new runtime_error("UNIBUS only supports BR7-BR4 requests");

  UnibusDeviceConfiguration *device_configuration = nullptr;
  for (auto it = _registered_devices.begin(); it != _registered_devices.end(); ++it) {
    if ((*it)->get_device() == device) {
      device_configuration = *it;
    }
  }

  if (device_configuration == nullptr)
    throw new runtime_error("Non registered device requests bus-master status");
  _master_requests_queue.push_back(make_pair(priority, device_configuration));
}

void Unibus::cpu_interrupt(uint18 address) {
  UnibusDeviceConfiguration* cpu_device_configuration = get_registered_device(CPU_PSW_ADDRESS);
  cpu_device_configuration->get_device()->interrupt(address);
}
