#include "Color.h"
#include "Weapon.h"
#include "Player.h"
#include "Map.h"
#include <cassert>

Item::UseResult Weapon::use(Map &map) {
  if (map.you.getCurrentWeapon() == this) {
    if (map.you.addItem(this)) {
      return Item::Release;
    }
    else {
      return Item::Fail;
    }
  }
  else {
    map.you.setWeapon(this);
    return Item::Release;
  }
} 

char Weapon::glyph() const {
  return ')';
}



bool Axe::attack(Map &map, int dx, int dy) {
  Point spaces[3];
  int playerX = map.getPlayerX();
  int playerY = map.getPlayerY();
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
    map(space.x, space.y).damage(1, map, space.x, space.y);
  }
  damage(2);
  return true;
}

std::string Axe::name() const {
  return "axe";
}

const Color& Axe::color() const {
  return DarkGray;
}



bool Bludgeon::attack(Map &map, int dx, int dy) {
  int x = map.getPlayerX() + dx;
  int y = map.getPlayerY() + dy;
  map(x, y).damage(1, map, x, y);
  map(x, y).stun(1);
  damage(2);
  return true;
}

std::string Bludgeon::name() const {
  return "bludgeon";
}

const Color& Bludgeon::color() const {
  return Brown;
}



bool Lance::attack(Map &map, int dx, int dy) {
  int x = map.getPlayerX() + dx;
  int y = map.getPlayerY() + dy;
  bool shouldSpendTurn = (map.you.getLastMoveDirection() != Point{dx, dy});
  map.you.setLastMoveDirection({0, 0});
  if (!shouldSpendTurn) {
    map(x, y).damage(2, map, x, y);
  }
  else {
    map(x, y).damage(1, map, x, y);
  }
  damage(2);
  return shouldSpendTurn;
}

std::string Lance::name() const {
  return "lance";
}

const Color& Lance::color() const {
  return White;
}



bool Spear::attack(Map &map, int dx, int dy) {
  int x = map.getPlayerX() + dx;
  int y = map.getPlayerY() + dy;
  map(x, y).damage(1, map, x, y);
  x += dx;
  y += dy;
  if (Map::isValidX(x) && Map::isValidY(y)) {
    map(x, y).damage(1, map, x, y);
  }
  damage(2);
  return true;
}

std::string Spear::name() const {
  return "spear";
}

const Color& Spear::color() const {
  return Red;
}
