#include <iostream>
#include "core/Unibus.h"
#include "core/Memory.h"
#include "core/CPU.h"

int main() {
  Unibus unibus;
  Memory memory(16 * 1024);
  unibus.register_device(&memory, 0, memory.get_memory_size());
  CPU cpu(&unibus);
//  std::cout << "Hello, World!" << Unibus::get_magic() << std::endl;
  return 0;
}