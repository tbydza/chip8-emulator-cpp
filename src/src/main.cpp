#include "Emulator.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    //todo arguments check
    std::cout << "loading: " << argv[1] << std::endl;
    Emulator emulator;
    emulator.LoadProgram(argv[1]);
    emulator.Run();

    return 0;
}