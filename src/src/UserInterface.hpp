#ifndef USERINTERFACE_HPP
#define USERINTERFACE_HPP

#include <ncurses.h>
#include <array>

class UserInterface
{
public:
  UserInterface();
  ~UserInterface();
  int GetKey(bool waitForPress);
  void DrawScreen(bool display[][32]);
  void Clear();

protected:
private:
  char filledPixel = '*';
  char emptyPixel = ' ';
};

#endif