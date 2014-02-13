#include "Map.h"
#include "items.h"
#include "enemies.h"
#include "functions.h"

#include <assert.h>

//depth arg will eventually be used to scale difficulty. once different branches
//are implemented, there'll probably be another argument controlling monster
//and loot generation
Map::Map(Player &player, int depth) : you(player) {
  switch (randTo(0)) {
  default:
    generateBoxes(depth);
  }

  getSpace(playerX, playerY).setType(StairsUp);
  sanitizeEntry();
}

void Map::drawLine(Point a, const Point b, const SpaceType &type) {
  assert(isValidX(a.x) && isValidX(b.x) &&
	 isValidY(a.y) && isValidY(b.y));
  //this is horrific style, but fuck it
  if (randTo(1)) {
    while (a.x - b.x) {
      getSpace(a.x, a.y).setType(type);
      a.x += sgn(b.x - a.x);
      if (!randTo(10)) { //creates wavy lines rather than straight angles
	drawLine(a, b, type);
	return;
      }
    }
    while (a.y - b.y) {
      getSpace(a.x, a.y).setType(type);
      a.y += sgn(b.y - a.y);
    }
  }
  else {
    while (a.y - b.y) {
      getSpace(a.x, a.y).setType(type);
      a.y += sgn(b.y - a.y);
      if (!randTo(10)) { //creates wavy lines rather than straight angles
	drawLine(a, b, type);
	return;
      }
    }
    while (a.x - b.x) {
      getSpace(a.x, a.y).setType(type);
      a.x += sgn(b.x - a.x);
    }
  }
  getSpace(a.x, a.y).setType(type);
}

void Map::drawBox(Point a, Point b, const SpaceType &type) {
  assert(isValidX(a.x) && isValidX(b.x) &&
	 isValidY(a.y) && isValidY(b.y));
  int xMax = std::max(a.x, b.x);
  int yMax = std::max(a.y, b.y);
  for (int x = std::min(a.x, b.x); x <= xMax; x++) {
    for (int y = std::min(a.y, b.y); y <= yMax; y++) {
      getSpace(x, y).setType(type);
    }
  }
}

void Map::generateRoom(Point center, int maxRadius) {
  int xMin = std::max(1, center.x - randRange(1, maxRadius));
  int xMax = std::min(MAPWIDTH, center.x + randRange(1, maxRadius));
  int yMin = std::max(1, center.y - randRange(1, maxRadius));
  int yMax = std::min(MAPHEIGHT, center.y + randRange(1, maxRadius));
  drawBox(Point{xMin, yMin}, Point{xMax, yMax}, Floor);
}

void Map::generateBoxes(int depth) {
  std::vector<Point> roomLocations;

  for (int i = randRange(8, 10); i > 0; i--) {
    Point center{randRange(1, MAPWIDTH), randRange(1, MAPHEIGHT)};
    generateRoom(center, 4);
    roomLocations.push_back(center);
  }

  for (unsigned int i = 1; i < roomLocations.size(); i++) {
    drawLine(roomLocations.at(i), roomLocations.at(i - 1), Floor);
  }

  playerX = roomLocations.at(0).x;
  playerY = roomLocations.at(0).y;
  space[roomLocations.at(roomLocations.size() - 1).x]
       [roomLocations.at(roomLocations.size() - 1).y].setType(StairsDown);

  for (int x = 1; x <= MAPWIDTH; x++) {
    for (int y = 1; y <= MAPHEIGHT; y++) {
      if (getSpace(x, y).isPassable()) {
	if (!randTo(50 - depth)) {
	  getSpace(x, y).setEnemy(getRandomEnemy());
	}
	else if (!randTo(80 - depth)){
	  getSpace(x, y).setItem(getRandomItem());
	}
      }
    }
  }
}

const Enemy& Map::getRandomEnemy() {
  int num = randTo(99);
  if (num < 50)
    return Zombie;
  if (num < 75)
    return BigZombie;
  if (num < 90)
    return Exploder;
  
  return Reacher;
}

const Item& Map::getRandomItem() {
  int num = randTo(99);
  if (num < 35)
    return Health;
  if (num < 45)
    return BigHealth;
  if (num < 65)
    return TorchPack;
  if (num < 80)
    return BombPack;
  if (num < 90)
    return ArrowPack;

  return SpeedPotionPack;
}

void Map::sanitizeEntry() {
  for (int x = playerX - 2; x <= playerX + 2; x++) {
    for (int y = playerY - 2; y <= playerY + 2; y++) {
      if (isValidX(x) && isValidY(y)) {
	getSpace(x, y).setEnemy();
      }
    }
  }
}
