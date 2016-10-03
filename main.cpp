#include <iostream>
#include "core/Unibus.h"
#include "core/Memory.h"
#include "core/CPU.h"
#include "core/io_devices/PDPKeyboard.h"
#include "core/io_devices/PDPTapeWriter.h"
#include "core/io_devices/PDPDisplayAdapter.h"
#include <unistd.h>

int main() {
  Unibus unibus;
  Memory memory(16 * 1024);
  CPU cpu;
  PDPKeyboard keyboard;
  PDPTapeWriter tape_writer;
  PDPDisplayAdapter display_adapter;

  unibus.register_device(&memory, 0, memory.get_memory_size());
  unibus.register_device(&cpu, CPU::BASE_MEM_MAP_SEGMENT_ADDRESS, CPU::BASE_MEM_MAP_SEGMENT_SIZE);
  unibus.register_device(&keyboard,
                         PDPKeyboard::PDP_KEYBOARD_MEM_REGISTERS_ADDRESS,
                         PDPKeyboard::PDP_KEYBOARD_MEM_REGISTERS_SIZE);
  unibus.register_device(&tape_writer,
                         PDPTapeWriter::PDP_TAPE_WRITER_MEM_REGISTERS_ADDRESS,
                         PDPTapeWriter::PDP_TAPE_WRITER_MEM_REGISTERS_SIZE);
  unibus.register_device(&display_adapter,
                         PDPDisplayAdapter::PDP_VIDEO_ADAPTER_VRAM_ADDRESS,
                         PDPDisplayAdapter::PDP_VIDEO_ADAPTER_VRAM_SIZE);
  unibus.register_device(&display_adapter,
                         PDPDisplayAdapter::PDP_VIDEO_ADAPTER_IO_PALETTE_ADDRESS,
                         PDPDisplayAdapter::PDP_VIDEO_ADAPTER_IO_PALETTE_SIZE);
  unibus.register_device(&display_adapter,
                         PDPDisplayAdapter::PDP_VIDEO_ADAPTER_MODE_ADDRESS,
                         PDPDisplayAdapter::PDP_VIDEO_ADAPTER_IO_SIZE);
  memory.write_word(0001000, 0, 0005000);
  memory.write_word(0001002, 0, 0016060);
  memory.write_word(0001004, 0, 0176000);
  memory.write_word(0001006, 0, 0176100);
  memory.write_word(0001010, 0, 0000127);
  memory.write_word(0001012, 0, 0001000);

  while (true) {
    unibus.master_device_execute();
    usleep(10000);
  }
  return 0;
}