#include <SDL2/SDL.h>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <thread>
#include <iostream>
#include <cstdlib>
#include <string>
#include "instruction.h"
bool progRuns = false;
bool mode = 0;
uint16_t PC = 0x100;
uint8_t opCycles[0xFF];
uint8_t data[0x10000];
uint8_t header[0x150];
uint8_t* rom = nullptr;
uint8_t* ram = nullptr; 
uint16_t AF;
uint16_t BC;
uint16_t DE;
uint16_t HL;
uint8_t mbcType = 0;
size_t rom_size = 0;
size_t ram_size = 0;
bool has_ram = 0;
bool has_battery = 0;
uint8_t MBCFLAGS = 0x00;
uint8_t rom_bank = 1;
uint8_t ram_bank = 0;
uint16_t bank_size = 0x3FFF;
static const uint32_t CyclesPerFrame = 70224;
int main(int argc, char* argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "Son there was a err with ye SDL2/SDL:" << SDL_GetError() << std::endl;
    return 4;
  }
  SDL_Window* window = SDL_CreateWindow("Scaled SDL2 Screen",
                                      SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED,
                                      320, 288,
                                      SDL_WINDOW_SHOWN);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_RenderSetLogicalSize(renderer, 160, 144);
  SDL_RenderSetIntegerScale(renderer, SDL_TRUE);
  using namespace std::chrono;
  if(argc < 2) {
    std::cerr<<"son put arguments"<<std::endl;
    return 3;
  }
  progRuns = true;
  std::string filename = argv[1];
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
  int ram_size_code = header[0x149];
  ram_size = 0;
  switch(ram_size_code) {
    case 0x00: ram_size = 0; break;
    case 0x01: ram_size = 2048; break;    // 2 KB (unused)
    case 0x02: ram_size = 8192; break;    // 8 KB
    case 0x03: ram_size = 32768; break;   // 32 KB
    case 0x04: ram_size = 131072; break;  // 128 KB
    case 0x05: ram_size = 65536; break;   // 64 KB
  }
  ram = new uint8_t[ram_size];
  mbcType = header[0x147];
  switch(mbcType) {
    case 0x00:
      std::cout<<"mbcType:none"<<std::endl;
      has_ram = 0;
      has_battery = 0;
    break;
    case 0x01:
      std::cout<<"mbcType:1"<<std::endl;
      has_ram = 0;
      has_battery = 0;
    break;
    case 0x02:
      std::cout<<"mbcType:2,RAM"<<std::endl;
      has_ram = 1;
      has_battery = 0;
    break;
    case 0x03:
      std::cout<<"mbcType:3,RAM,Battery"<<std::endl;
      has_ram = 1;
      has_battery = 1;
    break;
    default:
      std::cerr<<"Son we dont have that yet sry";
    break;
  }
  data[0x100] = 0x00;
  //add a jp instruction to 0x150 or wherever your project starts
  steady_clock::time_point acc = steady_clock::now();
  //load RAM save before running
  if(has_battery) {
    std::string basename = filename.substr(filename.find_last_of("/") + 1);
    std::ifstream FILE_SAV(std::string(std::getenv("HOME")) + "/gbemu/saves/" + basename + ".sav", std::ios::binary);
    if (FILE_SAV.is_open()) {
      FILE_SAV.read((char*)ram, ram_size);
      FILE_SAV.close();
    }
  }
  uint32_t cycles = 0;
  while(progRuns) {
    std::this_thread::sleep_until(acc + nanoseconds(16666667));
    acc += nanoseconds(16666667);
    cycles = 0;
    while(cycles < CyclesPerFrame) {
      try {
        cycles += FDE();
      } catch(const std::runtime_error& e) {
        std::cerr<<"Son you have a error:"<<e.what()<<std::endl;
        goto end;
      }
    }
    //execute code here
  }
  end:
  //log ram to file here \/
  if(has_battery) {
    std::string basename = filename.substr(filename.find_last_of("/") + 1);
    std::ofstream FILE_SAV(std::string(std::getenv("HOME")) + "/gbemu/saves/" + basename + ".sav", std::ios::binary);
    if(FILE_SAV.is_open()) {
      FILE_SAV.write((const char*)ram, ram_size);
      FILE_SAV.close();
    } else {
      std::cerr<<"Son your saves are probably GONE"<<std::endl;
    }
  }
  if(ram != nullptr) {
    delete[] ram; 
    ram = nullptr;
  }
  if(rom != nullptr) {
    delete[] rom;
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
