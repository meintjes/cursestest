#ifndef __MENU_H__
#define __MENU_H__

#include "Option.h"
#include <vector>

class Menu {
 public:
  Menu(std::vector<Option> optionsIn);
  void operator()();

  /**
   * Close the menu. This causes it not to prompt for the next choice. Should
   * be called from inside a menu option.
   */
  void close();

 private:
  void displayMenu();
  void getMenuInput();
  void changeChoice(int dx);

  std::vector<Option> options;
  int currentOption;
  bool isDone;
};

#endif
