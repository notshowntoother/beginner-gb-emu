#ifndef GB_CYCLE_TABLE_HEADER
#define GB_CYCLE_TABLE_HEADER
#include <iostream>
#include <stdexcept>
#include "mbc.h"
//write instructions
//FDE(fetch, decode, execute)
extern uint16_t PC;
extern uint8_t data[];
inline void LD_H_REG(uint16_t& reggie, uint8_t& value) {
  reggie &= 0x00FF;
  reggie |= (static_cast<uint16_t>(value)) << 8;
}
inline void LD_L_REG(uint16_t& reggie, uint8_t& value) {
  reggie &= 0xFF00;
  reggie |= static_cast<uint16_t>(value);
}
inline uint8_t RD_H_REG(uint16_t reggie) {
  return static_cast<uint8_t>((reggie & 0xFF00)>>8);
}
inline uint8_t RD_L_REG(uint16_t reggie) {
  return static_cast<uint8_t>(reggie & 0x00FF);
}
inline uint8_t fetchCOD() {
  return getData(PC++);
}
//DE(decode, execute)
uint8_t DE(uint8_t opcode) {
  switch(opcode) {
    case 0x00:
      return 0x04;
    break;
    case 0xC3:
      high_byte = fetchCOD();
      low_byte = fetchCOD();
      PC = (high_byte) | (low_byte);
      return 16;
    break
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
