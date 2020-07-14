#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <cstdlib>
#include <cstdint>
#include <ctime>
#include <chrono>
#include <thread>
#include <iostream>
#include <iomanip>

#include "UserInterface.hpp"

class Chip8
{
public:
  void ExecuteCycle();
  void Reset();
  void Reset(uint8_t *data, size_t length);
  Chip8();
  Chip8(uint8_t *data, size_t length);

private:
  void TimersTicks();
  void IncreasePC();
  uint8_t GenerateRandomNumber();
  uint8_t MemoryRead(uint16_t memoryAddress);
  void MemoryWrite(uint16_t memoryAddress, uint8_t data);

  uint8_t GetLastOpcodeByte(uint16_t opcode);
  uint16_t GetAdressFromOpcode(uint16_t opcode);
  uint8_t GetOpcodeNibble(uint16_t opcode, unsigned int positionInOpcode);

  void FetchOpcode();
  void DecodeOpcodeAndExecute();

  void ExecuteCLS();
  void ExecuteRET();

  void ExecuteJP(uint16_t addr);
  void ExecuteJP(uint8_t *Vx, uint16_t addr);

  void ExecuteCALL(uint16_t addr);
  void ExecuteSE(uint8_t *Vx, uint8_t byte);
  void ExecuteSE(uint8_t *Vx, uint8_t *Vy);

  void ExecuteSNE(uint8_t *Vx, uint8_t byte);
  void ExecuteSNE(uint8_t *Vx, uint8_t *Vy);

  void ExecuteLD(uint8_t *R1, uint8_t *R2);
  void ExecuteLD(uint8_t *Vx, uint8_t byte);
  void ExecuteLD(uint16_t addr);

  void ExecuteADD(uint8_t *Vx, uint8_t byte);
  void ExecuteADD(uint8_t *Vx, uint8_t *Vy);
  void ExecuteADD(uint8_t *Vx);

  void ExecuteOR(uint8_t *Vx, uint8_t *Vy);
  void ExecuteAND(uint8_t *Vx, uint8_t *Vy);
  void ExecuteXOR(uint8_t *Vx, uint8_t *Vy);
  void ExecuteSUB(uint8_t *Vx, uint8_t *Vy);
  void ExecuteSHR(uint8_t *Vx, uint8_t *Vy);
  void ExecuteSHL(uint8_t *Vx, uint8_t *Vy);
  void ExecuteSUBN(uint8_t *Vx, uint8_t *Vy);
  void ExecuteRND(uint8_t *Vx, uint8_t byte);
  void ExecuteDRW(uint8_t *Vx, uint8_t *Vy, uint8_t nibble);
  void ExecuteSKP(uint8_t *Vx);
  void ExecuteSKNP(uint8_t *Vx);

  void ExecuteLDK(uint8_t *Vx);

  void ExecuteLDS(uint8_t *Vx);
  void ExecuteSTB(uint8_t *Vx);

  void ExecuteLD(uint8_t *Vx);
  void ExecuteST(uint8_t *Vx);

  int cycleCount = 0;
  uint16_t regI;
  uint16_t regPC;
  uint8_t regSP;
  uint8_t regV[16];
  uint16_t stack[16];
  uint8_t delayTimer;
  uint8_t soundTimer;
  uint8_t memory[4096];
  uint8_t keypad[16];
  uint16_t opcode;
  bool flagStopPC;
  UserInterface ui;
  const int cycleDuration = 1851;
  bool display[64][32];
  bool loadedProgram = false;
  uint8_t chip8_fontset[80] =
      {
          0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
          0x20, 0x60, 0x20, 0x20, 0x70, // 1
          0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
          0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
          0x90, 0x90, 0xF0, 0x10, 0x10, // 4
          0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
          0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
          0xF0, 0x10, 0x20, 0x40, 0x40, // 7
          0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
          0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
          0xF0, 0x90, 0xF0, 0x90, 0x90, // A
          0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
          0xF0, 0x80, 0x80, 0x80, 0xF0, // C
          0xE0, 0x90, 0x90, 0x90, 0xE0, // D
          0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
          0xF0, 0x80, 0xF0, 0x80, 0x80  // F
  };
};
#endif