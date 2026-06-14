#include <chrono>
#include <cstdint>
#include <fstream>
#include <thread>
#include <iostream>
#include <cstdlib>
#include "instruction.h"
bool progRuns = false;
uint16_t PC = 0x100;
uint8_t opCycles[0xFF];
uint8_t data[0x10000];
uint8_t header[0x150];
uint8_t* rom = nullptr;
uint8_t mbcType = 0;
size_t rom_size = 0;
bool has_ram = 0;
uint8_t MBCFLAGS = 0x00;
uint8_t rom_bank = 1;
uint8_t bank_size = 0x3FFF;
static const uint32_t CyclesPerFrame = 70224;
int main(int argc, char* argv[]) {
  using namespace std::chrono;
  if(argc < 2) {
    std::cerr<<"son put arguments"<<std::endl;
    return 3;
  }
  progRuns = true;
  std::string filename = argv[1];
  const size_t max_bytes_to_read = 0x150;
  std::ifstream file(filename, std::ios::binary);
  if (!file) {
    // Try in ~/gbemu/roms/
    std::string alt_path = std::string(std::getenv("HOME")) + "/gbemu/roms/" + filename;
    file.open(alt_path, std::ios::binary);
    if (file) {
        filename = alt_path;
    }
  }
  if (!file) {
    std::cerr << "Son what happened to your file" << std::endl;
    return 1;
  }
  file.seekg(0);
  file.read((char*)header, 0x150);
  int rom_size_code = header[0x148];
  rom_size = 32768 * (1 << rom_size_code);
  rom = new uint8_t[rom_size];
  file.seekg(0);                    // go back to start
  file.read((char*)rom, rom_size);  // read entire ROM
  file.close();
  mbcType = header[0x147];
  switch(mbcType) {
    case 0x00:
      std::cout<<"mbcType:none"<<std::endl;
      has_ram = 0;
    break;
    case 0x01:
      std::cout<<"mbcType:1"<<std::endl;
      has_ram = 0;
    break;
    case 0x02:
      std::cout<<"mbcType:2"<<std::endl;
      has_ram = 1;
    break;
    case 0x03:
      std::cout<<"mbcType:3"<<std::endl;
      has_ram = 1;
    break;
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
    cycles = 0;
    while(cycles < CyclesPerFrame) {
      cycles += FDE();
    }
    //execute code here
  }
  delete[] rom;
  return 0;
}
