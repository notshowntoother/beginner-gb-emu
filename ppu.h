#ifndef GB_PPU_HEADER
#define GB_PPU_HEADER
#include <cstdint>
extern uint8_t mode;
extern uint8_t scanline;
extern uint32_t ppu_cycles;
extern uint8_t mode3_duration;
extern uint8_t data[];
extern bool VBlank;
extern bool HBlank;
/* so, Sprites in OAM are structured into 4 bytes, the x position, the y position, the index of the tile used, and the flags of the sprite.
the bits of the last byte(called the "flags") determines how the sprite will be drawn, this is the bits and their usage in drawing
bit|usage
7|Priority of render
6|Y flip
5|X flip
4|Palette
3|Bank(CGB ONLY!)
2-0|Palette number(CGB ONLY!)
*/
//sprite rendering x = x - 8
//sprite rendering y = y - 16
/* ok so I feel like i got a revelation
if you want to know what tile you are on vertically and you have the
absolute scanline row number(scanline number + SCY) you can do absolute row/8
if you want to know what part of the tile vertically you do absolute row % 8*/
inline uint16_t decodeTile(uint8_t LSB, uint8_t MSB) {
  uint16_t Row = 0;
  for(int16_t i = 7; i >= 0; --i) {
    uint16_t pixel = static_cast<uint16_t>((((MSB & (1 << i)) >> i) << 1)|((LSB & (1 << i)) >> i));
    Row |= (pixel << (i << 1));
  }
  return Row;
}
inline uint8_t* getTileAddr(uint8_t tileIndex, bool usingSigned) {
  uint8_t* point;
  if(usingSigned) {
    if(tileIndex < 0x80) {
      point = &data[(tileIndex * 16) + 0x9000];
    } else if(tileIndex > 0x7F) {
      point = &data[((tileIndex - 0x80) * 16) + 0x8800];
    }
  } else {
    point = &data[(tileIndex * 16) + 0x8000];
  }
  return point;
}
inline void ppu_event(uint8_t cycles) {
  ppu_cycles += cycles;
  switch(mode){
    case 1:
      if(ppu_cycles >= 4560) {
        mode = 2;
        ppu_cycles -= 4560;
      }
    break;
    case 2:
      if(ppu_cycles >= 80) {
        //scan
        mode = 3;
        ppu_cycles -= 80;
      }
    break;
    case 3:
      if(ppu_cycles >= mode3_duration){
        mode = 0;
        HBlank = 1;
        ++scanline;
        ppu_cycles -= mode3_duration;
      }
    break;
    case 0:
      if(ppu_cycles >= (376 - mode3_duration)) {
        ppu_cycles -= (376 - mode3_duration);
         if(scanline == 144){
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
