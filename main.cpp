#include <chrono>
#include <cstdint>
#include <fstream>
#include <thread>
#include <iostream>
#include "instruction.h"
bool progRuns = false;
uint16_t PC;
uint8_t opCycles[0xFF];
uint8_t data[0x10000];
uint8_t header[0x150];
uint8_t* rom = nullptr;
static const uint32_t CyclesPerFrame = 70224;
int main() {
  using namespace std::chrono;
  progRuns = true;
  const std::string filename = "emulated.gb";
  const size_t max_bytes_to_read = 0x150;
  std::ifstream file(filename, std::ios::binary);
  if (!file) {
    std::cerr << "Son what happened to your file" << std::endl;
    return 1;
  }
  file.seekg(0);
  file.read((char*)header, 0x150);
  int rom_size_code = header[0x148];
  size_t rom_size = 32768 * (1 << rom_size_code);
  rom = new uint8_t[rom_size];
  file.seekg(0);                    // go back to start
  file.read((char*)rom, rom_size);  // read entire ROM
  file.close();
  uint8_t mbcType = header[0x147];
  switch(mbcType) {
    default:
      std::cerr<<"Son we dont have that yet sry";
      return 2;
    break;
  }
  data[0x100] = 0x00;
  //add a jp instruction to 0x150 or wherever your project starts
  steady_clock::time_point acc = steady_clock::now();
  uint32_t cycles = 0;
  while(progRuns) {
    std::this_thread::sleep_until(acc + nanoseconds(16666667));
    acc += nanoseconds(16666667);
    while(cycles < CyclesPerFrame) {
      
    }
    //execute code here
  }
  delete[] rom;
  return 0;
}
