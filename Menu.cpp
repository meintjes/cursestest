#include "Menu.h"

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
  for (int i = -11; i <= 11; i++) {
    if (currentOption + i >= 0 &&
	currentOption + i < static_cast<int>(options.size())) {
      move(11 + i, 4);
      if (i == 0) {
	//is this good style I don't even know. TODO: implement even more classes,
	//chain together even more member access operators. don't stop until the
	//following line is at least 200 characters long.
	options.at(currentOption + i).getText().shift(BlackOnWhite).add();
      }
      else {
	options.at(currentOption + i).getText().add();
      }
    }
  }
  move(23, 1);
  Cst{"hjkl/numpad to select options", Cyan}.add();
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
