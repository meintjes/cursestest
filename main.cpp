#include <ncurses.h>
#include "Map.h"
#include "Player.h"
#include "Branch.h"
#include "Command.h"
#include "Menu.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cstring>
#include <functional>

void playGame();
bool getInput(Map *map, const CommandMap cmap);
bool getLongPrompt(Map *map);

Menu ControlsMenu({
  Option{"Move up/left", std::bind(changeControl, COMMAND_MOVE_UPLEFT)},
  Option{"Move up", std::bind(changeControl, COMMAND_MOVE_UP)},
  Option{"Move up/right", std::bind(changeControl, COMMAND_MOVE_UPRIGHT)},
  Option{"Move left", std::bind(changeControl, COMMAND_MOVE_LEFT)},
  Option{"Wait", std::bind(changeControl, COMMAND_WAIT)},
  Option{"Move right", std::bind(changeControl, COMMAND_MOVE_RIGHT)},
  Option{"Move down/left", std::bind(changeControl, COMMAND_MOVE_DOWNLEFT)},
  Option{"Move down", std::bind(changeControl, COMMAND_MOVE_DOWN)},
  Option{"Move down/right", std::bind(changeControl, COMMAND_MOVE_DOWNRIGHT)},
  Option{"Use bomb", std::bind(changeControl, COMMAND_USE_BOMB)},
  Option{"Use torch", std::bind(changeControl, COMMAND_USE_TORCH)},
  Option{"Use arrow", std::bind(changeControl, COMMAND_USE_ARROW)},
  Option{"Use speed potion", std::bind(changeControl, COMMAND_USE_SPEEDPOTION)},
  Option{"Go up stairs", std::bind(changeControl, COMMAND_INTERACT_STAIRSUP)},
  Option{"Go down stairs", std::bind(changeControl, COMMAND_INTERACT_STAIRSDOWN)},
  Option{"Enter long command", std::bind(changeControl, COMMAND_LONG_PROMPT)},
  Option{Red("Reset all controls"), resetControls}
});

int main() {
  initscr();
  noecho();
  curs_set(0);
  std::srand(time(0));

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

  Menu MainMenu({
    Option{"Play game", playGame},
    Option{"Change controls", ControlsMenu},
    Option{"Quit", [&MainMenu]() mutable {
      MainMenu.close();
    }}
  });
  MainMenu();

  endwin();
  return 0;
}

void playGame() {
  Player you;

  Branch dungeon{"Dungeon", DEPTH_DUNGEON, nullptr, 0};
  for (int i = 0; i < DEPTH_DUNGEON; i++) {
    dungeon.floors.push_back(Map(you, i));
  }

  you.setBranch(&dungeon);

  CommandMap cmap = {COMMAND_FIRST};
  readControls(cmap);

  while (you.getHp() > 0) {
    you.getCurrentFloor()->display();
    you.display();

    //get input that would cause a turn to pass
    while (!getInput(you.getCurrentFloor(), cmap));  
    you.getCurrentFloor()->tick(); //then update game state
  }

  you.getCurrentFloor()->display();
  you.display();

  getch();
}

bool getInput(Map *map, const CommandMap cmap) {
  DirectionalFn dfn;
  if (map->you.hasArrowMode()) {
    dfn = &Map::shootArrow;
  }
  else {
    dfn = &Map::movePlayer;
  }

  switch (cmap[getch()]) {
  case COMMAND_MOVE_UPLEFT:
    return (map->*dfn)(-1, -1);
  case COMMAND_MOVE_UP:
    return (map->*dfn)(0, -1);
  case COMMAND_MOVE_UPRIGHT:
    return (map->*dfn)(1, -1);
  case COMMAND_MOVE_LEFT:
    return (map->*dfn)(-1, 0);
  case COMMAND_WAIT:
    return true;
  case COMMAND_MOVE_RIGHT:
    return (map->*dfn)(1, 0);
  case COMMAND_MOVE_DOWNLEFT:
    return (map->*dfn)(-1, 1);
  case COMMAND_MOVE_DOWN:
    return (map->*dfn)(0, 1);
  case COMMAND_MOVE_DOWNRIGHT:
    return (map->*dfn)(1, 1);

  case COMMAND_USE_BOMB:
    return map->dropBomb();
  case COMMAND_USE_TORCH:
    return map->you.lightTorch();
  case COMMAND_USE_ARROW:
    return map->you.drawArrow();
  case COMMAND_USE_SPEEDPOTION:
    return map->you.quaffSpeedPotion();

  case COMMAND_INTERACT_STAIRSUP:
    return map->changeFloor(-1, StairsUp);
  case COMMAND_INTERACT_STAIRSDOWN:
    return map->changeFloor(+1, StairsDown);

  case COMMAND_LONG_PROMPT:
    return getLongPrompt(map);

  default:
    return false;
  }
}

bool getLongPrompt(Map *map) {
  char command[79];
  bool didTakeTurn = false;

  erase();
  map->display();
  move(23, 0);
  addc(DarkGray('#'));
  echo();
  getnstr(command, 79);
  noecho();

  if (std::strcmp(command, "die") == 0) {
    map->you.damage(100);
    didTakeTurn = true;
  }

  map->you.display();
  return didTakeTurn;
}
