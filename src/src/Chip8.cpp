
#include "Chip8.hpp"

Chip8::Chip8()
{
  srand(time(NULL));
}

uint8_t Chip8::GenerateRandomNumber()
{
  return (uint8_t)(rand() % 256);
}

void Chip8::ExecuteCycle()
{
  if (!loadedProgram)
  {
    throw "No program loaded!";
  }
  FetchOpcode();
  DecodeOpcodeAndExecute();
  cycleCount++;
  if (cycleCount >= 8)
  {
    TimersTicks();
    cycleCount = 0;
  }
  IncreasePC();
  std::this_thread::sleep_for(std::chrono::microseconds(cycleDuration));
}

void Chip8::TimersTicks()
{
  if (delayTimer > 0)
  {
    delayTimer--;
  }
  if (soundTimer > 0)
  {
    soundTimer--;
  }
}

uint8_t Chip8::MemoryRead(uint16_t memoryAddress)
{
  return memory[memoryAddress];
}

void Chip8::MemoryWrite(uint16_t memoryAddress, uint8_t data)
{
  memory[memoryAddress] = data;
}

uint8_t Chip8::GetLastOpcodeByte(uint16_t opcode)
{
  return (uint8_t)(opcode & 0x00FF);
}

uint16_t Chip8::GetAdressFromOpcode(uint16_t opcode)
{
  return opcode & 0x0FFF;
}

uint8_t Chip8::GetOpcodeNibble(uint16_t opcode, unsigned int positionInOpcode)
{
  uint16_t res;
  switch (positionInOpcode)
  {
  case 0:
    res = (opcode & 0xF000) >> 12;
    break;
  case 1:
    res = (opcode & 0x0F00) >> 8;
    break;
  case 2:
    res = (opcode & 0x00F0) >> 4;
    break;
  case 3:
    res = opcode & 0x000F;
    break;
  default:
    res = (opcode & 0x0F00) >> 8;
    break;
  }
  return (uint8_t)res;
}

void Chip8::FetchOpcode()
{
  uint16_t opcodeH = 0;
  uint16_t opcodeL = 0;
  opcode = 0;

  opcodeH = MemoryRead(regPC);
  opcodeL = MemoryRead(regPC + 1);
  opcode = (opcodeH << 8) | opcodeL;
  // std::cerr << std::hex << regPC << " (" << std::dec << (int)regPC - 512 << ") : ";
  // std::cerr << std::hex << opcodeH << " + ";
  // std::cerr << std::hex << opcodeL << " -> ";
  // std::cerr << std::hex << opcode << std::endl;
}

void Chip8::DecodeOpcodeAndExecute()
{
  std::cerr << std::hex << opcode << " ";
  switch (opcode & 0xF000)
  {
  case 0x0000:
    if (opcode == 0x00E0)
    {
      std::cerr << "execute CLS" << std::endl;
      ExecuteCLS();
    }
    else if (opcode == 0x00EE)
    {
      std::cerr << "execute RET" << std::endl;
      ExecuteRET();
    }
    else
    {
      //TODO invalid opcode
      std::cerr << "INVALID 1" << std::endl;
    }
    break;
  case 0x1000:
    std::cerr << "execute JP" << std::endl;
    ExecuteJP(opcode & 0x0FFF);
    break;

  case 0x2000:
    std::cerr << "execute CALL" << std::endl;
    ExecuteCALL(opcode & 0x0FFF);
    break;

  case 0x3000:
    std::cerr << "execute SE1" << std::endl;
    ExecuteSE(&regV[GetOpcodeNibble(opcode, 1)], GetLastOpcodeByte(opcode));
    break;

  case 0x4000:
    std::cerr << "execute SNE1" << std::endl;
    ExecuteSNE(&regV[GetOpcodeNibble(opcode, 1)], GetLastOpcodeByte(opcode));
    break;

  case 0x5000:
    std::cerr << "execute SE2" << std::endl;

    ExecuteSE(&regV[GetOpcodeNibble(opcode, 1)], &regV[GetOpcodeNibble(opcode, 2)]);
    break;

  case 0x6000:
    std::cerr << "execute LD1" << std::endl;

    ExecuteLD(&regV[GetOpcodeNibble(opcode, 1)], GetLastOpcodeByte(opcode));
    break;

  case 0x7000:
    std::cerr << "execute ADD1" << std::endl;

    ExecuteADD(&regV[GetOpcodeNibble(opcode, 1)], GetLastOpcodeByte(opcode));
    break;

  case 0x8000:

    switch (GetOpcodeNibble(opcode, 3))
    {
    case 0x0:
      std::cerr << "execute LD2" << std::endl;

      ExecuteLD(&regV[GetOpcodeNibble(opcode, 1)], &regV[GetOpcodeNibble(opcode, 2)]);
      break;

    case 0x1:
      std::cerr << "execute OR" << std::endl;

      ExecuteOR(&regV[GetOpcodeNibble(opcode, 1)], &regV[GetOpcodeNibble(opcode, 2)]);
      break;

    case 0x2:
      std::cerr << "execute AND" << std::endl;

      ExecuteAND(&regV[GetOpcodeNibble(opcode, 1)], &regV[GetOpcodeNibble(opcode, 2)]);
      break;

    case 0x3:
      std::cerr << "execute XOR" << std::endl;

      ExecuteXOR(&regV[GetOpcodeNibble(opcode, 1)], &regV[GetOpcodeNibble(opcode, 2)]);
      break;

    case 0x4:
      std::cerr << "execute ADD2" << std::endl;

      ExecuteADD(&regV[GetOpcodeNibble(opcode, 1)], &regV[GetOpcodeNibble(opcode, 2)]);
      break;

    case 0x5:
      std::cerr << "execute SUB1" << std::endl;

      ExecuteSUB(&regV[GetOpcodeNibble(opcode, 1)], &regV[GetOpcodeNibble(opcode, 2)]);
      break;

    case 0x6:
      std::cerr << "execute SHR1" << std::endl;

      ExecuteSHR(&regV[GetOpcodeNibble(opcode, 1)], &regV[GetOpcodeNibble(opcode, 2)]);
      break;

    case 0x7:
      std::cerr << "execute SUBN1" << std::endl;

      ExecuteSUBN(&regV[GetOpcodeNibble(opcode, 1)], &regV[GetOpcodeNibble(opcode, 2)]);
      break;

    case 0xE:
      std::cerr << "execute SHL" << std::endl;

      ExecuteSHL(&regV[GetOpcodeNibble(opcode, 1)], &regV[GetOpcodeNibble(opcode, 2)]);
      break;

    default:
      std::cerr << "INVALID 2" << std::endl;

      //TODO
      //invalid opcode
      break;
    }
    break;

  case 0x9000:
    std::cerr << "execute SNE" << std::endl;

    ExecuteSNE(&regV[GetOpcodeNibble(opcode, 1)], &regV[GetOpcodeNibble(opcode, 2)]);
    break;

  case 0xA000:
    std::cerr << "execute LD3" << std::endl;
    ExecuteLD(GetAdressFromOpcode(opcode));
    break;

  case 0xB000:
    std::cerr << "execute JP2" << std::endl;

    ExecuteJP(&regV[0], GetAdressFromOpcode(opcode));
    break;

  case 0xC000:
    std::cerr << "execute RND" << std::endl;

    ExecuteRND(&regV[GetOpcodeNibble(opcode, 1)], GetLastOpcodeByte(opcode));
    break;

  case 0xD000:
    std::cerr << "execute DRW" << std::endl;

    ExecuteDRW(&regV[GetOpcodeNibble(opcode, 1)], &regV[GetOpcodeNibble(opcode, 2)], GetOpcodeNibble(opcode, 3));
    break;

  case 0xE000:
    if (GetLastOpcodeByte(opcode) == 0x9E)
    {
      std::cerr << "execute SKP" << std::endl;

      ExecuteSKP(&regV[GetOpcodeNibble(opcode, 1)]);
    }
    else if (GetLastOpcodeByte(opcode) == 0xA1)
    {
      std::cerr << "execute SKNP" << std::endl;

      ExecuteSKNP(&regV[GetOpcodeNibble(opcode, 1)]);
    }
    else
    {
      //TODO
      //invalid opcode
      std::cerr << "INVALID 3" << std::endl;
    }
    break;

  case 0xF000:

    switch (GetLastOpcodeByte(opcode))
    {
    case 0x07:
      std::cerr << "execute LD4" << std::endl;

      ExecuteLD(&regV[GetOpcodeNibble(opcode, 1)], &delayTimer);
      break;

    case 0x0A:
      std::cerr << "execute LDK" << std::endl;

      ExecuteLDK(&regV[GetOpcodeNibble(opcode, 1)]);
      break;

    case 0x15:
      std::cerr << "execute LD_T1" << std::endl;

      ExecuteLD(&delayTimer, &regV[GetOpcodeNibble(opcode, 1)]);
      break;

    case 0x18:
      std::cerr << "execute LD_T2" << std::endl;

      ExecuteLD(&soundTimer, &regV[GetOpcodeNibble(opcode, 1)]);
      break;

    case 0x1E:
      std::cerr << "execute ADD3" << std::endl;

      ExecuteADD(&regV[GetOpcodeNibble(opcode, 1)]);
      break;

    case 0x29:
      std::cerr << "execute LDS" << std::endl;

      ExecuteLDS(&regV[GetOpcodeNibble(opcode, 1)]);
      break;

    case 0x33:
      std::cerr << "execute STB" << std::endl;

      ExecuteSTB(&regV[GetOpcodeNibble(opcode, 1)]);
      break;

    case 0x55:
      std::cerr << "execute ST" << std::endl;

      ExecuteST(&regV[GetOpcodeNibble(opcode, 1)]);
      break;

    case 0x65:
      std::cerr << "execute LD5" << std::endl;
      std::cerr << "V: " << (int)GetOpcodeNibble(opcode, 1) << std::endl;

      ExecuteLD(&regV[GetOpcodeNibble(opcode, 1)]);
      break;

    default:
      std::cerr << "INVALID 4" << std::endl;

      //TODO
      //invalid opcode
      break;
    }
    break;

  default:
    //TODO
    //invalid opcode
    std::cerr << "INVALID 5" << std::endl;

    break;
  }
}

void Chip8::ExecuteCLS()
{
  ui.Clear();
}

void Chip8::ExecuteRET()
{
  //TODO stackunderflow
  regPC = stack[regSP--];
}

void Chip8::ExecuteJP(uint16_t addr)
{
  regPC = addr;
  flagStopPC = true;
}

void Chip8::ExecuteJP(uint8_t *Vx, uint16_t addr)
{
  regPC = addr + (uint16_t)*Vx;
  flagStopPC = true;
}

void Chip8::ExecuteCALL(uint16_t addr)
{
  //TODO stackoverflow
  stack[++regSP] = regPC;
  regPC = addr;
  flagStopPC = true;
}

void Chip8::ExecuteSE(uint8_t *Vx, uint8_t byte)
{
  if (*Vx == byte)
  {
    IncreasePC();
  }
}

void Chip8::ExecuteSE(uint8_t *Vx, uint8_t *Vy)
{
  if (*Vx == *Vy)
  {
    IncreasePC();
  }
}

void Chip8::ExecuteSNE(uint8_t *Vx, uint8_t byte)
{
  if (*Vx != byte)
  {
    IncreasePC();
  }
}

void Chip8::ExecuteSNE(uint8_t *Vx, uint8_t *Vy)
{
  if (*Vx != *Vy)
  {
    IncreasePC();
  }
}

void Chip8::ExecuteLD(uint8_t *R1, uint8_t *R2)
{
  *R1 = *R2;
}

void Chip8::ExecuteLD(uint8_t *Vx, uint8_t byte)
{
  *Vx = byte;
}

void Chip8::ExecuteLD(uint16_t addr)
{
  regI = addr;
}

void Chip8::ExecuteADD(uint8_t *Vx, uint8_t byte)
{
  *Vx += byte;
}

void Chip8::ExecuteADD(uint8_t *Vx, uint8_t *Vy)
{
  if ((uint16_t)*Vx + (uint16_t)*Vy > 255)
  {
    regV[0xF] = 1;
  }
  else
  {
    regV[0xF] = 0;
  }
  *Vx += *Vy;
}

void Chip8::ExecuteADD(uint8_t *Vx)
{
  regI += (uint16_t)*Vx;
}

void Chip8::ExecuteOR(uint8_t *Vx, uint8_t *Vy)
{
  *Vx |= *Vy;
}

void Chip8::ExecuteAND(uint8_t *Vx, uint8_t *Vy)
{
  *Vx &= *Vy;
}

void Chip8::ExecuteXOR(uint8_t *Vx, uint8_t *Vy)
{
  *Vx ^= *Vy;
}

void Chip8::ExecuteSUB(uint8_t *Vx, uint8_t *Vy)
{
  if (*Vx > *Vy)
  {
    regV[0xF] = 1;
  }
  else
  {
    regV[0xF] = 0;
  }

  *Vx -= *Vy;
}

void Chip8::ExecuteSHR(uint8_t *Vx, uint8_t *Vy)
{
  regV[0xF] = *Vx & 1;
  *Vx >>= 1;
}

void Chip8::ExecuteSHL(uint8_t *Vx, uint8_t *Vy)
{
  regV[0xF] = *Vx & 128;
  *Vx <<= 1;
}

void Chip8::ExecuteSUBN(uint8_t *Vx, uint8_t *Vy)
{
  if (*Vy > *Vx)
  {
    regV[0xF] = 1;
  }
  else
  {
    regV[0xF] = 0;
  }

  *Vx = *Vy - *Vx;
}

void Chip8::ExecuteRND(uint8_t *Vx, uint8_t byte)
{
  *Vx = GenerateRandomNumber() & byte;
}

void Chip8::ExecuteDRW(uint8_t *Vx, uint8_t *Vy, uint8_t nibble)
{
  size_t xPos = *Vx;
  size_t yPos = *Vy;
  bool erased = false;
  std::cerr << "=================================" << std::endl;
  std::cerr << "draw screen" << std::endl;
  std::cerr << std ::dec << "x: " << xPos << " y: " << yPos << std::endl;
  for (uint8_t i = 0; i < nibble; i++)
  {

    uint8_t byte = MemoryRead(regI + i);
    uint8_t maskTemplate = 0x80;
    xPos = *Vx;
    for (size_t i = 0; i < 8; i++)
    {
      uint8_t mask = maskTemplate >> i;
      bool newPixel = ((byte & mask) == mask);
      bool oldPixel = display[xPos][yPos];
      newPixel = newPixel ^ oldPixel;
      display[xPos][yPos] = newPixel;
      if (newPixel == false && oldPixel == true)
      {
        erased = true;
      }
      
      xPos++;
      if (xPos >= 64)
      {
        xPos = 0;
      }
    }
    yPos++;
    if (yPos >= 32)
    {
      yPos = 0;
    }
  }

  if (erased)
  {
    regV[0xF] = 1;
  }
  else
  {
    regV[0xF] = 0;
  }
  // std::cerr << "=================================" << std::endl;
  // std::cerr << "draw screen" << std::endl;
  // std::cerr << std ::dec << "x: " << xPos << " y: " << yPos << std::endl;
  // std::cerr << "=================================" << std::endl;
  ui.DrawScreen(display);
}

void Chip8::ExecuteSKP(uint8_t *Vx)
{
  int key = ui.GetKey(false);
  if (regV + key == Vx)
  {
    IncreasePC();
  }
}

void Chip8::ExecuteSKNP(uint8_t *Vx)
{
  int key = ui.GetKey(false);
  if (regV + key != Vx)
  {
    IncreasePC();
  }
}

void Chip8::ExecuteLDK(uint8_t *Vx)
{
  int key = ui.GetKey(true);
  *Vx = key;
}

//load sprite
void Chip8::ExecuteLDS(uint8_t *Vx)
{
  regI = 5 * *Vx;
}

void Chip8::ExecuteSTB(uint8_t *Vx)
{
  uint8_t data = *Vx;
  MemoryWrite(regI, data / 100);
  MemoryWrite(regI + 1, (data % 100) / 10);
  MemoryWrite(regI + 2, (data % 10));
}

void Chip8::ExecuteLD(uint8_t *Vx)
{
  std::cerr << "load " << regI << std::endl;
  int i = 0;
  for (uint8_t *ip = regV; ip <= Vx; ip++)
  {
    //std::cerr << std::hex << (&regV[0] + i) << std::endl;

    std::cerr << "V" << i << ": " << MemoryRead(regI + i) << std::endl;
    regV[i] = MemoryRead(regI + i);
    i++;
  }
}

void Chip8::ExecuteST(uint8_t *Vx)
{
  for (size_t i = 0; regV + i <= Vx; i++)
  {
    MemoryWrite(regI + i, regV[i]);
  }
}

void Chip8::IncreasePC()
{
  if (!flagStopPC)
  {
    regPC += 2;
  }
  else
  {
    flagStopPC = false;
  }
}

void Chip8::Reset()
{
  cycleCount = 0;
  regI = 0;
  regPC = 512;
  regSP = 0;
  delayTimer = 0;
  soundTimer = 0;
  flagStopPC = false;
  for (size_t i = 0; i < 80; i++)
  {
    memory[i] = chip8_fontset[i];
  }

  for (size_t i = 0; i < 16; i++)
  {
    regV[i] = 0;
  }

  for (size_t i = 0; i < 64; i++)
  {

    for (size_t j = 0; j < 32; j++)
    {
      display[i][j] = false;
    }
  }
  ui.Clear();
}

void Chip8::Reset(uint8_t *data, size_t length)
{
  Reset();
  //std::cerr << "! " << (int)regPC << std::endl;
  for (size_t i = 0; i < length; i++)
  {
    //std::cerr <<std::hex << data[i];
    memory[i + regPC] = data[i];
    loadedProgram = true;
  }
}

Chip8::Chip8(uint8_t *data, size_t length) : Chip8()
{
  Reset(data, length);
}
