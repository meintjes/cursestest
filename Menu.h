#ifndef __MENU_H__
#define __MENU_H__

#include "Option.h"
#include <ncurses.h>
#include <vector>

class Menu {
 public:
  Menu(std::vector<Option> optionsIn);
  void operator()();

 private:
  void displayMenu();
  bool getMenuInput();
  void changeChoice(int dx);

  std::vector<Option> options;
  int currentOption;
};

#endif
