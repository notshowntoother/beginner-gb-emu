#ifndef GB_MBC_HEADER
#define GB_MBC_HEADER
#include <cstdint>
extern uint8_t data[];
extern uint8_t mbcType;
extern size_t rom_size;
extern uint8_t* rom;
inline uint8_t mbc_read(uint16_t addr) {
    switch(mbcType) {
        case 0x00:  // ROM ONLY
            return rom[addr];
        default:
            return 0xFF;
    }
}
inline uint8_t getData(uint16_t pointer) {
  if (pointer < 0x8000) {
    return mbc_read(pointer);
  }
  return data[pointer];
}
#endif
