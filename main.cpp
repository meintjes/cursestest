#include "Map.h"
#include <ncurses.h>
#include "Color.h"
#include "Player.h"
#include "Branch.h"
#include "Command.h"
#include "Menu.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cstring>
#include <functional>
#include <cassert>

void playGame();
bool getInput(Map &map, const CommandMap cmap);
bool getLongPrompt(Map &map);

int main() {
  initscr(); //start ncurses
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
    {"Play game", playGame},
    {"Change controls", ControlsMenu},
    {"Quit", std::bind(&Menu::close, &MainMenu)}
  });
  MainMenu();
  

  //clean up after ncurses
  endwin();
  return 0;
}

void playGame() {
  Player you;

  Branch dungeon("Dungeon", DEPTH_DUNGEON, nullptr, 0, you);

  you.setBranch(&dungeon);

  CommandMap cmap = {COMMAND_FIRST};
  readControls(cmap);

  //until you die:
  while (you.getHp() > 0) {
    //get input that would cause a turn to pass
    do {
      you.getCurrentFloor().display();
      you.display();
    }
    while (!getInput(you.getCurrentFloor(), cmap));
    you.getCurrentFloor().tick(); //then update game state
  }

  //after the player dies:
  dungeon.deleteMapFiles();
  erase();
  you.getCurrentFloor().display();
  addcs(0, 22, Red("You have died..."));
  getch();
}

bool getInput(Map &map, const CommandMap cmap) {
  DirectionalFn dfn;
  switch (map.you.getMode()) {
    case Player::Mode::Move:
      dfn = &Map::movePlayer;
      break;
    case Player::Mode::Arrow:
      dfn = &Map::shootArrow;
      break;
    case Player::Mode::Hook:
      dfn = &Map::throwHook;
      break;
    default:
      assert(false);
  }

  switch (cmap[getch()]) {
  case COMMAND_FIRST: //unbound keys
    return false;
  case COMMAND_MOVE_UPLEFT:
    return (map.*dfn)(-1, -1);
  case COMMAND_MOVE_UP:
    return (map.*dfn)(0, -1);
  case COMMAND_MOVE_UPRIGHT:
    return (map.*dfn)(1, -1);
  case COMMAND_MOVE_LEFT:
    return (map.*dfn)(-1, 0);
  case COMMAND_WAIT:
    return true;
  case COMMAND_MOVE_RIGHT:
    return (map.*dfn)(1, 0);
  case COMMAND_MOVE_DOWNLEFT:
    return (map.*dfn)(-1, 1);
  case COMMAND_MOVE_DOWN:
    return (map.*dfn)(0, 1);
  case COMMAND_MOVE_DOWNRIGHT:
    return (map.*dfn)(1, 1);

  case COMMAND_EVOKE_ARTIFACT:
    return map.you.evokeArtifact();
  case COMMAND_USE_ITEM:
    return map.you.useItem();
  case COMMAND_DROP_ITEM:
    return map.you.dropItem();

  case COMMAND_INTERACT_STAIRSUP:
    return map(map.getPlayerX(), map.getPlayerY()).typeIs(Space::StairsUp)
           && map.you.changeDepth(-1);
  case COMMAND_INTERACT_STAIRSDOWN:
    return map(map.getPlayerX(), map.getPlayerY()).typeIs(Space::StairsDown)
           && map.you.changeDepth(+1);

  case COMMAND_LONG_PROMPT:
    return getLongPrompt(map);

  default: //some command isn't handled by this function
    assert(false);
    return false;
  }
}

bool getLongPrompt(Map &map) {
  char command[80];
  bool didTakeTurn = false;

  erase();
  map.display();
  addc(0, 23, DarkGray('#'));
  echo();
  getnstr(command, 79);
  noecho();

  if (std::strcmp(command, "die") == 0) {
    map.you.damage(100);
    didTakeTurn = true;
  }

  map.you.display();
  return didTakeTurn;
}
