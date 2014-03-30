#include "Game.h"
#include "Map.h"
#include "Archive.h"
#include "Color.h"
#include <cstring>
#include <ncurses.h>
#include <cassert>

Game::Game() :
  branches({
      {"Dungeon", 12, nullptr, 0, you}
  })
{
  you.setBranch(&branches.front());
}

Game::Game(unsigned int id) {
  assert(false);
}

void Game::play() {
  CommandMap cmap = {COMMAND_FIRST};
  readControls(cmap);

  //until you die:
  while (you.getHp() > 0) {
    //get input that would cause a turn to pass
    do {
      you.getCurrentFloor().display();
      you.display();
    }
    while (!getInput(cmap));
    you.getCurrentFloor().tick(); //then update game state
  }

  //after the player dies:
  erase();
  you.getCurrentFloor().display();
  addcs(0, 22, Red("You have died..."));
  getch();
  end();
}

void Game::save() {
  for (Branch &b : branches) {
    b.emptyCache();
  }

  exit(0);
}

void Game::end() {
  for (Branch &b : branches) {
    b.deleteMapFiles();
  }

  exit(0);
}

bool Game::getInput(const CommandMap cmap) {
  Map &map = you.getCurrentFloor();

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
    return getLongPrompt();

  default: //some command isn't handled by this function
    assert(false);
    return false;
  }
}

bool Game::getLongPrompt() {
  Map &map = you.getCurrentFloor();

  //get rid of the regular HUD
  erase();
  map.display();

  //prompt for a command
  echo();
  char command[80];
  addc(0, 23, DarkGray('#'));
  getnstr(command, 79);
  noecho();

  if (std::strcmp(command, "die") == 0) {
    map.you.damage(100);
    return true;
  }
  else if (std::strcmp(command, "save") == 0) {
    save();
    return false;
  }

  return false;
}
