#ifndef GB_MBC_HEADER
#define GB_MBC_HEADER
#include <cstdint>
extern uint8_t data[];
extern uint8_t mbcType;
extern size_t rom_size;
extern size_t ram_size;
extern uint8_t* rom;
extern uint8_t* ram;
extern bool mbc_mode;
extern uint8_t MBCFLAGS;
extern uint8_t rom_bank;
extern uint8_t ram_bank;
extern uint16_t bank_size; //PLACEHOLDER
extern bool has_ram;
extern bool has_battery;
inline void MBCwrite(uint16_t pointer, uint8_t value){
  if (pointer < 0x2000 && has_ram) {
      // Any write to 0000–1FFF → RAM enable
    MBCFLAGS = value;
    return;
  } else if (pointer < 0x4000) {
      // Any write to 2000–3FFF → ROM bank
    rom_bank &= ~(0x1F);
    //clear before setting
    rom_bank |= value & 0x1F;
    if (rom_bank == 0){rom_bank = 1;}
    return;
  } else if (pointer < 0x6000) {
    if(mbc_mode) {
      ram_bank = value & 0x03;
    } else {
        //upper 2 bits
      rom_bank &= 0x9F;
      //clear before setting
      rom_bank |= (value & 0x03) << 5;
      return;
    }
  } else if(pointer < 0x8000) {
    mbc_mode = value & 0x01;
    return;
  }
}
inline void RAMwrite(uint16_t pointer, const uint8_t value) {
  if(pointer > 0x2001){std::cerr<<"wrong pointer"<<std::endl; return;}
  if(MBCFLAGS == 0x0A && ram_size > 0x2000 && mbc_mode) {
    ram[(0x2000 * (ram_bank)) + pointer % 0x2001] = value;
  } else if (MBCFLAGS == 0x0A){
    ram[pointer] = value;
  }
}
inline void Write(uint16_t pointer ,uint8_t value) {
  if(pointer < 0x8000){MBCwrite(pointer, value); return;}
  if(pointer > 0xDFFF && pointer < 0xFE00){data[pointer - 0x2000] = value; return;}
  if(pointer > 0xFE9F && pointer < 0xFF00){std::cerr<<"Son this is unusable memory Sry"<<std::endl;}
  if(pointer < 0xC000 && pointer > 0x9FFF){RAMwrite(pointer - 0xA000, value); return;}
  data[pointer] = value;
}
inline uint8_t mbc_read(uint16_t addr) {
    switch(mbcType) {
        case 0x00:  // ROM ONLY
            return rom[addr];
        break;
        case 0x01:
        if (addr < 0x4000) {
          return rom[addr];  // Fixed bank 0
        } else {
          return rom[(addr - 0x4000) + (rom_bank * 0x4000)];
        }
        break;
        default: 
            return 0xFF;
        break;
    }
}
inline uint8_t getData(uint16_t pointer) {
  if (pointer < 0x8000) {
    return mbc_read(pointer);
  }
  return data[pointer];
}
#endif
