#include <iostream>
#include "core/Unibus.h"
#include "core/Memory.h"
#include "core/CPU.h"
#include "core/ROM.h"
#include "core/io_devices/PDPKeyboard.h"
#include "core/io_devices/PDPTapeWriter.h"
#include "core/io_devices/PDPDisplayAdapter.h"
#include "gui/mainwindow.h"
#include <QApplication>

int main(int argc, char **argv) {
  Unibus unibus;
  Memory memory(16 * 1024);
  ROM rom("video_test.bin");
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
//  memory.write_word(0001000, 0, 0005000);
//  memory.write_word(0001002, 0, 0016060);
//  memory.write_word(0001004, 0, 0176000);
//  memory.write_word(0001006, 0, 0176100);
//  memory.write_word(0001010, 0, 0000127);
//  memory.write_word(0001012, 0, 0001000);
  /*
  memory.write_word(0001000, 0, 0005000);
  memory.write_word(0001002, 0, 0005001);
  memory.write_word(0001004, 0, 0005002);
  memory.write_word(0001006, 0, 0005003);
  memory.write_word(0001010, 0, 0005004);
  memory.write_word(0001012, 0, 0016003);
  memory.write_word(0001014, 0, 0001066);
  memory.write_word(0001016, 0, 0016004);
  memory.write_word(0001020, 0, 0001070);
  memory.write_word(0001022, 0, 0010060);
  memory.write_word(0001024, 0, 0176300);
  memory.write_word(0001026, 0, 0010160);
  memory.write_word(0001030, 0, 0176304);
  memory.write_word(0001032, 0, 0010260);
  memory.write_word(0001034, 0, 0176306);
  memory.write_word(0001036, 0, 0016060);
  memory.write_word(0001040, 0, 0001072);
  memory.write_word(0001042, 0, 0176302);
  memory.write_word(0001044, 0, 0005201);
  memory.write_word(0001046, 0, 0003401);
  memory.write_word(0001050, 0, 0000766);
  memory.write_word(0001052, 0, 0005001);
  memory.write_word(0001054, 0, 0005202);
  memory.write_word(0001056, 0, 0020402);
  memory.write_word(0001060, 0, 0003400);
  memory.write_word(0001062, 0, 0000741);
  memory.write_word(0001064, 0, 0000000);
  memory.write_word(0001066, 0, 0001114);
  memory.write_word(0001070, 0, 0000671);
  memory.write_word(0001072, 0, 0010001);
  */

  uint32 t = 0;
  while (!cpu.is_halted() && t < 10000000) {
    unibus.master_device_execute();
    t++;
//    usleep(10000);
  }

  QApplication a(argc, argv);
  MainWindow w;
  w.renderScreen(display_adapter.get_video_buffer());
  w.show();
  return a.exec();
}