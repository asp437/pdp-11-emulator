#include <iostream>
#include "core/Unibus.h"
#include "core/Memory.h"
#include "core/CPU.h"
#include "core/ROM.h"
#include "core/io_devices/PDPKeyboard.h"
#include "core/io_devices/PDPTapeWriter.h"
#include "core/io_devices/PDPDisplayAdapter.h"
#include "core/pdp_machine.h"
#include "gui/mainwindow.h"
#include <QApplication>

int main(int argc, char **argv) {
  /*
  Unibus unibus;
  Memory memory(16 * 1024);
  ROM rom("video_palette_test.bin");
  CPU cpu;
  PDPKeyboard keyboard;
  PDPTapeWriter tape_writer;
  PDPDisplayAdapter display_adapter;

  unibus.register_device(&memory, 0, memory.get_memory_size());
  unibus.register_device(&cpu, CPU::BASE_MEM_MAP_SEGMENT_ADDRESS, CPU::BASE_MEM_MAP_SEGMENT_SIZE);
  unibus.register_device(&rom, ROM::ROM_DEFAULT_OFFSET, ROM::ROM_DEFAULT_SIZE);
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

  uint32 t = 0;
  while (!cpu.is_halted() && t < 1000000) {
    unibus.master_device_execute();
    t++;
  }
  */

  QApplication a(argc, argv);
  MainWindow w;
  w.set_pdp_machine(new PDPMachine("video_palette_test.bin"));
  // w.renderScreen(display_adapter.get_video_buffer());
  w.show();
  return a.exec();
}