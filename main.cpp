#include "Map.h"
#include <ncurses.h>
#include "Game.h"
#include "Color.h"
#include "Player.h"
#include "Command.h"
#include "Menu.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <functional>

int main() {
  initscr(); //start ncurses
  atexit([](){endwin();}); //end ncurses when program exits
  noecho(); //don't repeat inputted characters to the player
  curs_set(0); //don't show the cursor
  std::srand(time(0)); //seed the RNG

  //initialize color pairs. don't use these, see Color.h
  start_color();
  init_pair(1, COLOR_BLACK, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  init_pair(4, COLOR_YELLOW, COLOR_BLACK);
  init_pair(5, COLOR_BLUE, COLOR_BLACK);
  init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(7, COLOR_CYAN, COLOR_BLACK);
  init_pair(8, COLOR_WHITE, COLOR_BLACK);
  init_pair(9, COLOR_BLACK, COLOR_WHITE);
  init_pair(10, COLOR_BLACK, COLOR_RED);
  init_pair(11, COLOR_BLACK, COLOR_GREEN);
  init_pair(12, COLOR_BLACK, COLOR_YELLOW);
  init_pair(13, COLOR_BLACK, COLOR_CYAN);
  init_pair(14, COLOR_YELLOW, COLOR_CYAN);
  init_pair(15, COLOR_WHITE, COLOR_WHITE);

  //generate the controls menu
  std::vector<Option> controlsMenuOptions;
  for (int i = COMMAND_FIRST + 1; i < COMMAND_LAST; i++) {
    controlsMenuOptions.emplace_back(
      [i]() mutable {
        CommandMap cmap = {COMMAND_FIRST};
        readControls(cmap);
        return (getCommandName(static_cast<Command>(i)) + ": "
                + listKeysForCommand(static_cast<Command>(i), cmap));
      },
      std::bind(changeControl, static_cast<Command>(i))
    );
  }
  controlsMenuOptions.emplace_back(Red("Reset all controls"), resetControls);
  Menu ControlsMenu(controlsMenuOptions);
  
  //generate and execute the main menu
  Menu MainMenu({
    {"Play game", [](){
                    Game game; game.play();
                  }
    },
    {"Change controls", ControlsMenu},
    {"Quit", std::bind(&Menu::close, &MainMenu)}
  });
  MainMenu();
 

  return 0;
}
