//
// Created by Aleksandr Parfenov on 06.10.16.
//

#ifndef PDP_11_EMULATOR_PDP_MACHINE_H
#define PDP_11_EMULATOR_PDP_MACHINE_H

#include "../common.h"
#include <vector>

class Unibus;
class Memory;
class ROM;
class CPU;
class PDPKeyboard;
class PDPTapeWriter;
class PDPDisplayAdapter;

struct CPUState {
	uint16 r[8];
	uint16 psw;
};

class PDPMachine {
public:
  PDPMachine(string rom_file);
  ~PDPMachine();

  vector<vector<int>> &get_video_buffer();
  void execute_command();
  uint16 get_memory_word(uint18 address);
  CPUState get_cpu_state();
  PDPDisplayAdapter *get_display_adapter() { return _display_adapter; }
  bool is_halted();
  string get_rom_file_name() { return _rom_file_name; }

  static const uint18 MEMORY_SIZE = 16 * 1024;
private:
  string _rom_file_name;
  Unibus *_unibus;
  Memory *_memory;
  ROM *_rom;
  CPU *_cpu;
  PDPKeyboard *_keyboard;
  PDPTapeWriter *_tape_writer;
  PDPDisplayAdapter *_display_adapter;
};

#endif // PDP_11_EMULATOR_PDP_MACHINE_H