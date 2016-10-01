//
// Created by Aleksandr Parfenov on 01.10.16.
//

#include "CPU.h"
CPU::CPU(Unibus *unibus) : _unibus(unibus) {
  // TODO: Initialize PC and PSW
}

void CPU::execute_command() {
  uint16 opcode = _unibus->read((uint18) this->pc.r);
  switch (opcode) { // TODO: Implement PDP-11 CPU instruction set
  case 0000000: // HALT
    break;
  default:cerr << "Unknown opcode" << endl;
    break;
  }
  this->pc.r++;
}
