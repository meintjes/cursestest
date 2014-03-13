#ifndef __MENU_H__
#define __MENU_H__

#include "Option.h"
#include <vector>

//creates a pretty scrolling menu with a neat scrollbar on the left hand side
//of the screen. see Option.h too. 

class Menu {
 public:
  //pass the menu constructor a bracketed list of options, and it'll create
  //a vector for you. this is probably less tedious than separately adding
  //elements to a vector.
  Menu(std::vector<Option> optionsIn);
  
  //execute the menu (ask the player for input until they choose an option
  //or back out).
  void operator()();

  //Close the menu. This causes it not to prompt for the next choice. Should
  //be called from inside a menu option.
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
