#ifndef GB_CYCLE_TABLE_HEADER
#define GB_CYCLE_TABLE_HEADER
#include <iostream>
#include <stdexcept>
#include "mbc.h"
//write instructions
//FDE(fetch, decode, execute)
extern uint16_t PC;
extern uint8_t data[];
uint8_t inline fetchCOD() {
  return getData(PC++);
}
//DE(decode, execute)
uint8_t DE(uint8_t opcode) {
  switch(opcode) {
    case0x00:
      return 0x04;
    break;
    default:
      std::cerr<<"unknown/banned instruction:0x"<<opcode<<std::endl;
      throw std::runtime_error("wrong instruction!");
    break;
  }
}
uint8_t FDE() {
  uint8_t opcode = fetchCOD();
  return DE(opcode);
}

#endif
