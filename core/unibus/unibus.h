//
// Created by Aleksandr Parfenov on 01.10.16.
//

#ifndef PDP_11_EMULATOR_UNIBUS_H
#define PDP_11_EMULATOR_UNIBUS_H

#include "../../common.h"

#include <vector>

#include "unibus_device.h"

class UnibusDeviceConfiguration;

class Unibus {
public:
    bool register_device(UnibusDevice *device, uint18 base_address, uint18 reserve_space_size);
    uint16 read_word(uint18 address);
    void write_word(uint18 address, uint16 value);
    uint8 read_byte(uint18 address);
    void write_byte(uint18 address, uint8 value);
    void npr_request(UnibusDevice *device);
    void br_request(UnibusDevice *device, uint8 priority);
    void cpu_interrupt(uint18 address);

    void set_init_line();
    void master_device_execute();
private:
    UnibusDeviceConfiguration *get_registered_device(uint18 address);

    vector<UnibusDeviceConfiguration *> _registered_devices;
    pair<uint8, UnibusDeviceConfiguration *> _master_device;
    vector<pair<uint8, UnibusDeviceConfiguration *> > _master_requests_queue;
};

#endif //PDP_11_EMULATOR_UNIBUS_H
