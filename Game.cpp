#include "Game.h"
#include "Map.h"
#include "Archive.h"
#include "Color.h"
#include <algorithm>
#include <cstring>
#include <ncurses.h>
#include <cassert>

Game::Game() :
  id(0), //placeholder for finding the next 
  branches({
      {"Dungeon", 12, nullptr, 0, you, id}
  })
{
  you.setBranch(&branches.front());
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
  //explicit scope so that the archive closes before the program exits
  {
    Archive ar(getPath(), Archive::Save);

    //serialize the player
    you.serialize(ar);

    //then the branches; first, the number of branches:
    ar << branches.size();

    //then the bulk of their data:
    for (Branch &b : branches) {
      b.emptyCache(); //(make sure the most recently accessed map is saved too)
      ar << b.getName();
      ar << b.getMaxDepth();
      ar << b.getParentDepth();
    }

    //then serialize the parent branch names after everything else
    for (Branch &b : branches) {
      if (b.getParentBranch()) {
        ar << b.getParentBranch()->getName();
      }
      else {
        ar << '_';
      }
    }
  }

  exit(0);
}

Game::Game(unsigned int idIn) :
  id(idIn)
{
  Archive ar(getPath(), Archive::Load);

  you.serialize(ar);

  int size;
  ar >> size;
  for (int i = 0; i < size; i++) {
    std::string name;
    unsigned int maxDepth, parentDepth;
    ar >> name >> maxDepth >> parentDepth;
    branches.emplace_back(name, maxDepth, nullptr, parentDepth, you, id);
  }

  for (Branch &b : branches) {
    //read in the parent branch name:
    std::string parentName;
    ar >> parentName;
    //find the first branch with the name referred to
    auto parentIterator = std::find_if(branches.begin(),
                                       branches.end(),
                                       [&parentName](const Branch &b) {
                                         return b.getName() == parentName;
                                       });
    if (parentIterator != branches.end()) {
      b.setParentBranch(&*parentIterator);
    }
  }
}

void Game::end() {
  for (Branch &b : branches) {
    b.deleteMapFiles();
  }
  remove(getPath().c_str());
}

std::string Game::getPath() const {
  return "saves/" + std::to_string(id) + "Game";
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
