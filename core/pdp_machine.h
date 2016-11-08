//
// Created by Aleksandr Parfenov on 06.10.16.
//

#ifndef PDP_11_EMULATOR_PDP_MACHINE_H
#define PDP_11_EMULATOR_PDP_MACHINE_H

#include "../common.h"
#include <vector>
#include <chrono>

class Unibus;
class RAM;
class ROM;
class CPU;
class PDPKeyboard;
class PDPDisplayAdapter;
class DisAsm;

const int PDP11_FREQENCY = 5000000;

struct CPUState {
    uint16 r[8];
    uint16 psw;
    uint16 current_operation_address;
};

class PDPMachine {
public:
    PDPMachine(string rom_file);
    ~PDPMachine();

    void execute_command(bool step = false);
    uint16 get_memory_word(uint18 address);
    CPUState get_cpu_state();
    PDPDisplayAdapter *get_display_adapter() { return _display_adapter; }
    bool is_halted();
    string get_rom_file_name() { return _rom_file_name; }
    vector<pair<string, uint16>> get_disasm(uint18 address, uint18 size);
    void key_pressed(uint keycode, bool key_down);
    void reset_timer();

    static const uint18 MEMORY_SIZE = 16 * 1024;
private:
    int _emulated_ticks;
    int _ticks_per_second;
    chrono::high_resolution_clock::time_point _prev_tick_time;

    string _rom_file_name;
    Unibus *_unibus;
    RAM *_memory;
    ROM *_rom;
    CPU *_cpu;
    PDPKeyboard *_keyboard;
    PDPDisplayAdapter *_display_adapter;
    DisAsm *_disasm;
};

#endif // PDP_11_EMULATOR_PDP_MACHINE_H