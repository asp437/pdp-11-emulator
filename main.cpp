#include <iostream>
#include "core/Unibus.h"
#include "core/Memory.h"
#include "core/CPU.h"

int main() {
  Unibus unibus;
  Memory memory(16 * 1024);
  CPU cpu;
  unibus.register_device(&memory, 0, memory.get_memory_size());
  unibus.register_device(&cpu, CPU::BASE_MEM_MAP_SEGMENT_ADDRESS, CPU::BASE_MEM_MAP_SEGMENT_SIZE);
  
//  std::cout << "Hello, World!" << Unibus::get_magic() << std::endl;
  return 0;
}