#include "Menu.h"
#include "Cch.h"
#include "Cst.h"
#include "Color.h"
#include <ncurses.h>
#include <functional>

Menu::Menu(std::vector<Option> optionsIn) :
  options(optionsIn),
  currentOption(0),
  isDone(false)
{}

void Menu::operator()() {
  if (options.size() == 0) {
    return;
  }

  isDone = false;
  do {
    displayMenu();
    getMenuInput();
  }
  while (!isDone);
}

void Menu::close() {
  isDone = true;
}

void Menu::displayMenu() {
  erase();

  //cool scrollbar
  for (int y = 1; y < 22; y++) {
    addc(1, y, DarkGray('|'));
  }
  if (options.size() > 1) {
    addc(1, 1 + ((20*currentOption) / (options.size() - 1)), BlackOnWhite(' '));
  }
  else {
    addc(1, 11, BlackOnWhite(' '));
  }

  for (int i = -11; i <= 11; i++) {
    if (currentOption + i >= 0 &&
	currentOption + i < static_cast<int>(options.size())) {
      if (i == 0) {
	addcs(4, 11, BlackOnWhite(options.at(currentOption + i).getText()));
      }
      else {
	addcs(4, 11 + i, options.at(currentOption + i).getText());
      }
    }
  }
  addcs(1, 23, Cyan("hjkl/numpad to select options"));
  refresh();
}

void Menu::getMenuInput() {
  switch(getch()) {
  case 27:
  case '4':
  case 'h':
    close();
    break;
  case 12:
  case 14:
  case '6':
  case 'l':
    options.at(currentOption)();
    break;
  case '8':
  case 'k':
    changeChoice(-1);
    break;
  case '2':
  case 'j':
    changeChoice(+1);
    break;
  default:
    break;
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
