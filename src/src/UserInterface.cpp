#include "UserInterface.hpp"
#include <iostream>
#include <iomanip>

void UserInterface::DrawScreen(bool display[][32])
{
  // std::cerr << "=================================" << std::endl;
  // std::cerr << "draw screen" << std::endl;
  // std::cerr << "=================================" << std::endl;

  for (size_t i = 0; i < 64; i++)
  {
    for (size_t j = 0; j < 32; j++)
    {
      //std::cerr << std::noboolalpha << display[i][j] << " ";
      if (display[i][j])
      {
        mvaddch(j, i, filledPixel);
      }
      else
      {
        mvaddch(j, i, emptyPixel);
      }
    }
    //std::cerr << std::endl;
  }
  refresh();
  std::cerr << "=================================" << std::endl;
}

int UserInterface::GetKey(bool waitForPress)
{
  //std::cerr << "get key 1" << std::endl;
  if (waitForPress)
  {
    timeout(-1);
  }
  else
  {
    timeout(0);
  }
  //std::cerr << "get key 2" << std::endl;
  int keyCode = getch();
  //std::cerr << "get key 3" << std::endl;
  mvaddch(0, 0, keyCode);
  if (keyCode < 0)
  {
    return -1;
  }

  while (!((keyCode >= 48 && keyCode <= 57) || (keyCode >= 65 && keyCode <= 70) || (keyCode >= 97 && keyCode <= 102)))
  {
    keyCode = getch();
    mvaddch(0, 0, keyCode);
  }

  if (keyCode <= 57)
  {
    return keyCode - 48;
  }
  if (keyCode <= 70)
  {
    return keyCode - 55;
  }
  if (keyCode <= 102)
  {
    return keyCode - 87;
  }
  return -1;
}

void UserInterface::Clear()
{
  //std::cerr << "clear" << std::endl;
  clear();
}

UserInterface::UserInterface()
{
  //std::cerr << "constructor" << std::endl;

  initscr();
  clear();
  raw();
  noecho();
  refresh();
}

UserInterface::~UserInterface()
{
  //std::cerr << "destructor" << std::endl;

  endwin();
}