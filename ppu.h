#ifndef GB_PPU_HEADER
#define GB_PPU_HEADER
extern mode;
inline void ppu_event(uint8_t cycles) {
  ppu_cycles += cycles
  switch(mode){
    case 0:
      if(ppu_cycles >= 4560) {
        VBlank = 0;
        HBlank = 0;
        mode = 2;
      }
    break;
    case 2:
      if(ppu_cycles >= 80) {
        //scan
        mode = 3;
      }
    break;
  }
}
#endif
