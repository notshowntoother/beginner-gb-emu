#ifndef GB_PPU_HEADER
#define GB_PPU_HEADER
extern uint8_t mode;
extern uint8_t scanline;
static uint8_t mode3_duration = 172;
extern uint8_t data[];
inline uint16_t decodeRow(uint8_t LSB, uint8_t MSB) {
  uint16_t Row = 0;
  for(uint16_t i = 7; i >= 0; --i) {
    uint16_t pixel = static_cast<uint16_t>(((MSB & (1 << i)) >> (i - 1))|((LSB & (1 << i)) >> i));
    Row |= (pixel << (i << 1));
  }
  return Row;
}
inline void ppu_event(uint8_t cycles) {
  ppu_cycles += cycles
  switch(mode){
    case 1:
      if(ppu_cycles >= 4560) {
        mode = 2;
      }
    break;
    case 2:
      if(ppu_cycles >= 80) {
        //scan
        mode = 3;
      }
    break;
    case 3:
      if(ppu_cycles >= mode3_duration){
        mode = 0;
        HBlank = 1;
        ++scanline;
      }
    break;
    case 0:
      if(ppu_cycles >= (376 - mode3_duration)) {
         if(scanline > 144){
           mode = 1;
           VBlank = 1;
         }else{
           mode = 2;
         }
      }
    break;
  }
}
#endif
