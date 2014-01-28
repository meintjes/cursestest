#include <ncurses.h>
#include "Map.h"
#include "Player.h"
#include "Branch.h"
#include <cstdlib>
#include <ctime>
#include <vector>

int playGame();

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

  if (has_colors()) {
    playGame();
  }  

  endwin();
  return 0;
}

int playGame() {
  Player you;

  Branch dungeon{"Dungeon", DEPTH_DUNGEON, nullptr, 0};
  for (int i = 0; i < DEPTH_DUNGEON; i++) {
    dungeon.floors.push_back(Map(you));
  }

  you.setBranch(&dungeon);

  while (you.getHp() > 0) {
    you.getCurrentFloor()->display();
    you.display();
    while (!you.getCurrentFloor()->getInput());  //get input that would cause a turn to pass
    you.getCurrentFloor()->tick();               //then update game state
  }

  you.getCurrentFloor()->display();
  you.display();

  getch();

  return 0;
}
