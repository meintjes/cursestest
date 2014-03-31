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
  init_pair(16, COLOR_BLACK, COLOR_BLUE);

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
  Menu controlsMenu(controlsMenuOptions);

  //generate the "load game" menu
  std::vector<Option> loadGameMenuOptions;
  for (unsigned int i = 0; i < ARBITRARY_SAVED_GAMES_LIMIT; i++) {
    if (Game::existsID(i)) {
      loadGameMenuOptions.emplace_back("Game #" + std::to_string(i),
                                      [i]() {
                                        Game game(i);
                                        game.play();
                                      });
    }
  }
  Menu loadGameMenu(loadGameMenuOptions);

  //generate and execute the main menu
  Menu mainMenu({
    {"Start new game", []() {
                         Game game;
                         game.play();
                       }
    },
    {"Load game", loadGameMenu},
    {"Change controls", controlsMenu},
    {"Quit", std::bind(&Menu::close, &mainMenu)}
  });
  mainMenu();
 

  return 0;
}
