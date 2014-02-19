#include <ncurses.h>
#include "Map.h"
#include "Player.h"
#include "Branch.h"
#include "Command.h"
#include "Menu.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <functional>
#include <cassert>

void playGame();
bool getInput(Player &you, Map *map, const CommandMap cmap);

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

  Option{"Use artifact", std::bind(changeControl, COMMAND_USE_ARTIFACT)},
  Option{"Use bomb", std::bind(changeControl, COMMAND_USE_BOMB)},
  Option{"Use torch", std::bind(changeControl, COMMAND_USE_TORCH)},
  Option{"Use arrow", std::bind(changeControl, COMMAND_USE_ARROW)},
  Option{"Use speed potion", std::bind(changeControl, COMMAND_USE_SPEEDPOTION)},

  Option{"Drop artifact", std::bind(changeControl, COMMAND_DROP_ARTIFACT)},

  Option{"Go up stairs", std::bind(changeControl, COMMAND_INTERACT_STAIRSUP)},
  Option{"Go down stairs", std::bind(changeControl, COMMAND_INTERACT_STAIRSDOWN)},

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

  //remove std::string call once Cst can be constructed from c strings
  Branch dungeon{std::string("Dungeon"), DEPTH_DUNGEON, nullptr, 0};
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
    while (!getInput(you, you.getCurrentFloor(), cmap));  
    you.getCurrentFloor()->tick(); //then update game state
  }

  you.getCurrentFloor()->display();
  you.display();

  getch();
}

bool getInput(Player &you, Map *map, const CommandMap cmap) {
  DirectionalFn dfn;
  if (you.hasArrowMode()) {
    dfn = &Map::shootArrow;
  }
  else {
    dfn = &Map::movePlayer;
  }

  switch (cmap[getch()]) {
  case COMMAND_FIRST: //unbound keys
    return false;
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

  case COMMAND_USE_ARTIFACT:
    return you.useArtifact();
  case COMMAND_USE_BOMB:
    return map->dropBomb();
  case COMMAND_USE_TORCH:
    return you.lightTorch();
  case COMMAND_USE_ARROW:
    return you.drawArrow();
  case COMMAND_USE_SPEEDPOTION:
    return you.quaffSpeedPotion();

  case COMMAND_DROP_ARTIFACT:
    return you.dropArtifact();

  case COMMAND_INTERACT_STAIRSUP:
    return map->changeFloor(-1, StairsUp);
  case COMMAND_INTERACT_STAIRSDOWN:
    return map->changeFloor(+1, StairsDown);

  default: //some command isn't handled by this function
    assert(false);
    return false;
  }
}
