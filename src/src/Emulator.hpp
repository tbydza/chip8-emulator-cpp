#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <climits>
#include <vector>
#include <list>
#include <memory>
#include <algorithm>
#include <string>

#include "Chip8.hpp"

class Emulator
{
public:
  Emulator();
  void Run();
  void LoadProgram(std::string filename);

protected:
  bool isRunning;
  Chip8 cpu;
};
#endif