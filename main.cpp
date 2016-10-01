#include <iostream>
#include "core/Unibus.h"
#include "core/Memory.h"
#include "core/CPU.h"

int main() {
  Unibus unibus;
  Memory memory(2048);
  unibus.register_device(&memory, 0, 2048);
//  std::cout << "Hello, World!" << Unibus::get_magic() << std::endl;
  return 0;
}