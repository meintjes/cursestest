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

void playGame();
bool getInput(Map *map, const CommandMap cmap);

Menu MainMenu({Option{"Play game", playGame}, Option{"Change controls", createControls}});

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

  CommandMap cmap;
  readControls("controls.txt", cmap);

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
  switch (cmap[getch()]) {
  case COMMAND_MOVE_UPLEFT:
    return map->movePlayer(-1, -1);
  case COMMAND_MOVE_UP:
    return map->movePlayer(0, -1);
  case COMMAND_MOVE_UPRIGHT:
    return map->movePlayer(1, -1);
  case COMMAND_MOVE_LEFT:
    return map->movePlayer(-1, 0);
  case COMMAND_WAIT:
    return map->movePlayer(0, 0);
  case COMMAND_MOVE_RIGHT:
    return map->movePlayer(1, 0);
  case COMMAND_MOVE_DOWNLEFT:
    return map->movePlayer(-1, 1);
  case COMMAND_MOVE_DOWN:
    return map->movePlayer(0, 1);
  case COMMAND_MOVE_DOWNRIGHT:
    return map->movePlayer(1, 1);

  case COMMAND_USE_BOMB:
    return map->dropBomb();
  case COMMAND_USE_TORCH:
    return map->you.lightTorch();
  case COMMAND_USE_ARROW:
    return map->you.drawArrow();

  case COMMAND_INTERACT_STAIRSUP:
    return map->changeFloor(-1, &StairsUp);
  case COMMAND_INTERACT_STAIRSDOWN:
    return map->changeFloor(+1, &StairsDown);

  default:
    return false;
  }
}
