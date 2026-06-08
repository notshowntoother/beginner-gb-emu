#include <chrono>
#include <stdint>
bool progRuns = false;
uint16_t PC;
uint8_t opCycles[0xFF];
uint8_t data[0x10000];
static const uint32_t CyclesPerFrame = 70224;
int main() {
  progRuns = true;
  uint8_t mbcType = data[0x147];
  switch(mbcType) {
    
  }
  steady_clock::time_point acc = steady_clock::now();
  uint32_t cycles = 0;
  while(progRuns) {
    std::this_thread::sleep_until(acc + nanoseconds(166667));
    acc += nanoseconds(16666667);
    while(cycles < CyclesPerFrame) {
      
    }
    //execute code here
  }
  return 0;
}
