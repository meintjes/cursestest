#include "Menu.h"
#include "Cch.h"
#include "Cst.h"
#include <ncurses.h>

Menu::Menu(std::vector<Option> optionsIn) :
  options(optionsIn),
  currentOption(0),
  isDone(false)
{}

void Menu::operator()() {
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
    move(y, 1);
    addc(DarkGray('|'));
  }
  if (options.size() > 1) {
    move(1 + ((20*currentOption) / (options.size() - 1)), 1);
  }
  else {
    move(11, 1);
  }
  addc(BlackOnWhite(' '));

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
