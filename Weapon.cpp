#include "Weapon.h"
#include "Player.h"
#include "Map.h"

Item::UseResult Weapon::use(Map *map) {
  if (map->you.getCurrentWeapon() == this) {
    if (map->you.addItem(this, true)) {
      return Item::Release;
    }
    else {
      return Item::Fail;
    }
  }
  else {
    map->you.setWeapon(this);
    return Item::Release;
  }
} 

char Weapon::glyph() const {
  return ')';
}



void Axe::attack(Map *map, int dx, int dy) {
  Point spaces[3];
  int playerX = map->getPlayerX();
  int playerY = map->getPlayerY();
  //figure out which three spaces to hit
  if (dx == 0) {
    spaces[0] = {playerX - 1, playerY + dy};
    spaces[1] = {playerX, playerY + dy};
    spaces[2] = {playerX + 1, playerY + dy};
  }
  else if (dy == 0) {
    spaces[0] = {playerX + dx, playerY - 1};
    spaces[1] = {playerX + dx, playerY};
    spaces[2] = {playerX + dx, playerY + 1};
  }
  else {
    spaces[0] = {playerX + dx, playerY + dy};
    spaces[1] = {playerX + dx, playerY};
    spaces[2] = {playerX, playerY + dy};
  }

  //hit each of those spaces
  for (Point &space : spaces) {
    map->getSpace(space.x, space.y).kill(*map, space.x, space.y);
  }
}

std::string Axe::name() const {
  return "axe";
}

const Color& Axe::color() const {
  return DarkGray;
}



void Bludgeon::attack(Map *map, int dx, int dy) {
  int x = map->getPlayerX() + dx;
  int y = map->getPlayerY() + dy;
  map->getSpace(x, y).kill(*map, x, y);
  map->getSpace(x, y).stun(1);
}

std::string Bludgeon::name() const {
  return "bludgeon";
}

const Color& Bludgeon::color() const {
  return Brown;
}



void Lance::attack(Map *map, int dx, int dy) {
  int x = map->getPlayerX() + dx;
  int y = map->getPlayerY() + dy;
  map->getSpace(x, y).kill(*map, x, y);
}

std::string Lance::name() const {
  return "lance";
}

const Color& Lance::color() const {
  return White;
}



void Spear::attack(Map *map, int dx, int dy) {
  int x = map->getPlayerX() + dx;
  int y = map->getPlayerY() + dy;
  map->getSpace(x, y).kill(*map, x, y);
  x += dx;
  y += dy;
  if (Map::isValidX(x) && Map::isValidY(y)) {
    map->getSpace(x, y).kill(*map, x, y);
  }
}

std::string Spear::name() const {
  return "spear";
}

const Color& Spear::color() const {
  return Red;
}
