#include "Menu.h"
#include "Cch.h"
#include "Cst.h"
#include <ncurses.h>

Menu::Menu(std::vector<Option> optionsIn) : options(optionsIn) {
  currentOption = 0;
}

void Menu::operator()() {
  do {
    displayMenu();
  }
  while (getMenuInput());
}

void Menu::displayMenu() {
  erase();

  //cool scrollbar
  for (int y = 1; y < 22; y++) {
    move(y, 1);
    DarkGray('|');
  }
  if (options.size() > 1) {
    move(1 + ((20*currentOption) / (options.size() - 1)), 1);
  }
  else {
    move(11, 1);
  }
  BlackOnWhite(' ');

  for (int i = -11; i <= 11; i++) {
    if (currentOption + i >= 0 &&
	currentOption + i < static_cast<int>(options.size())) {
      move(11 + i, 4);
      if (i == 0) {
	addcs(BlackOnWhite(options.at(currentOption + i).getText()));
      }
      else {
	addcs(options.at(currentOption + i).getText());
      }
    }
  }
  move(23, 1);
  addcs(Cyan("hjkl/numpad to select options"));
  refresh();
}

bool Menu::getMenuInput() {
  switch(getch()) {
  case 27:
  case '4':
  case 'h':
    return false;
  case 12:
  case 14:
  case '6':
  case 'l':
    options.at(currentOption)();
    return true;
  case '8':
  case 'k':
    changeChoice(-1);
    return true;
  case '2':
  case 'j':
    changeChoice(+1);
    return true;
  default:
    return true;
  }
}

void Menu::changeChoice(int dx) {
  currentOption += dx;
  if (currentOption < 0) {
    currentOption = options.size() - 1;
  }
  if (currentOption >= static_cast<int>(options.size())) {
    currentOption = 0;
  }
}
