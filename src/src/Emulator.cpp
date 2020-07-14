#include "Emulator.hpp"

Emulator::Emulator()
{
    isRunning = false;
}

void Emulator::Run()
{
    isRunning = true;

    while (isRunning)
    {
        cpu.ExecuteCycle();

        // try
        // {
        //     cpu.ExecuteCycle();
        // }
        // catch (char const *e)
        // {
        //     isRunning = false;
        // }
    }
}

void Emulator::LoadProgram(std::string filename)
{
    std::ifstream infile;
    infile.open(filename, std::ios::binary | std::ios::in);
    infile.seekg(0, infile.end);
    size_t length = infile.tellg();
    infile.seekg(0, infile.beg);
    if (length > 3584)
    {
        length = 3584;
    }

    if (infile.is_open() && infile.good())
    {
        char buffer[4096];
        if (infile.read(buffer, length))
        {

            // for (size_t i = 0; i < length; i++)
            // {
            //     std::cerr << std::hex << buffer[i];
            // }
            // std::cerr << std::flush;
            cpu.Reset((uint8_t *)buffer, length);
        }
        else
        {
            throw "read error";
        }

        infile.close();
    }
}