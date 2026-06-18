#ifndef GB_CYCLE_TABLE_HEADER
#define GB_CYCLE_TABLE_HEADER
#include <iostream>
#include <stdexcept>
#include "mbc.h"
//write instructions
//FDE(fetch, decode, execute)
extern uint16_t PC;
extern uint8_t data[];
extern uint16_t AF;
extern uint16_t BC;
extern uint16_t DE;
extern uint16_t HL;
inline void LD_H_REG();
inline void LD_L_REG();
inline uint8_t fetchCOD();
inline uint8_t RD_H_REG();
inline uint8_t RD_L_REG_();
inline void set_flags();
inline void LD_2_16();
inline void LD_H_REG(uint16_t& reggie, uint8_t value) {
  reggie &= 0x00FF;
  reggie |= (static_cast<uint16_t>(value)) << 8;
}
inline void LD_L_REG(uint16_t& reggie, uint8_t value) {
  reggie &= 0xFF00;
  reggie |= static_cast<uint16_t>(value);
}
inline uint8_t RD_H_REG(uint16_t reggie) {
  return static_cast<uint8_t>((reggie & 0xFF00)>>8);
}
inline uint8_t RD_L_REG(uint16_t reggie) {
  return static_cast<uint8_t>(reggie & 0x00FF);
}
inline void LD_2_16(uint16_t& reggie) {
    uint8_t low = fetchCOD();   // First byte = low byte (little-endian)
    uint8_t high = fetchCOD();  // Second byte = high byte
    reggie = (high << 8) | low;
}
inline void set_flags(bool z, bool n, bool h, bool c) {
  LD_L_REG(AF, static_cast<uint8_t>((z<<7)|(n<<6)|(h<<5)|(c<<4)));
}
inline void INC_H_REG(uint16_t% reggie) {
  if(RD_H_REG(reggie) + 1 == 0){
    AF |= (1 << 7); //set z flag
    LD_H_REG(reggie, 0); //overflow to 0
    return;
  }
  LD_H_REG(reggie, RD_R_REG(reggie) + 1);
  return;
}
inline void DEC_H_REG(uint16_t% reggie) {
  if(RD_H_REG(reggie) + 1 == 0){
    AF |= (1 << 7); //set z flag
    LD_H_REG(reggie, 0); //overflow to 0
    return;
  }
  LD_H_REG(reggie, RD_R_REG(reggie) + 1);
  return;
}
inline uint8_t fetchCOD() {
  return getData(PC++);
}
//DE(decode, execute)
uint8_t DEX(uint8_t opcode) {
  switch(opcode) {
    case 0x00:
      return 0x04;
    break;
    case 0x01:
      LD_2_16(BC);
      return 12;
    break;
    case 0x02:
      Write(BC, RD_L_REG(AF));
      return 8;
    case 0x03:
      ++BC;
      return 8;
    break;
    case 0x04:
      INC_H_REG(BC)
      return 4;
    break;
    case 0x05:
      
    case 0xC3:
      LD_2_16(PC);
      return 16;
    break;
    default:
      std::cerr<<"unknown/banned instruction:0x"<<opcode<<std::endl;
      throw std::runtime_error("wrong instruction!");
    break;
  }
}
uint8_t FDE() {
  uint8_t opcode = fetchCOD();
  return DEX(opcode);
}

#endif
