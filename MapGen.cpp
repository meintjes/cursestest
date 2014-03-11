#include "Map.h"
#include "functions.h"
#include "Item.h"
#include <assert.h>

//depth arg will eventually be used to scale difficulty. once different branches
//are implemented, there'll probably be another argument controlling monster
//and loot generation
Map::Map(Player &player, int depth) : you(player) {
  switch (randTo(0)) {
  default:
    generateBoxes(depth);
  }

  (*this)(playerX, playerY).setType(StairsUp);
  sanitizeEntry();
}

void Map::drawLine(Point a, const Point b, const SpaceType &type) {
  assert(isValidX(a.x) && isValidX(b.x) &&
	 isValidY(a.y) && isValidY(b.y));
  //this is horrific style, but fuck it
  if (randTo(1)) {
    while (a.x - b.x) {
      (*this)(a.x, a.y).setType(type);
      a.x += sgn(b.x - a.x);
      if (!randTo(10)) { //creates wavy lines rather than straight angles
	drawLine(a, b, type);
	return;
      }
    }
    while (a.y - b.y) {
      (*this)(a.x, a.y).setType(type);
      a.y += sgn(b.y - a.y);
    }
  }
  else {
    while (a.y - b.y) {
      (*this)(a.x, a.y).setType(type);
      a.y += sgn(b.y - a.y);
      if (!randTo(10)) { //creates wavy lines rather than straight angles
	drawLine(a, b, type);
	return;
      }
    }
    while (a.x - b.x) {
      (*this)(a.x, a.y).setType(type);
      a.x += sgn(b.x - a.x);
    }
  }
  (*this)(a.x, a.y).setType(type);
}

void Map::drawBox(Point a, Point b, const SpaceType &type) {
  assert(isValidX(a.x) && isValidX(b.x) &&
	 isValidY(a.y) && isValidY(b.y));
  int xMax = std::max(a.x, b.x);
  int yMax = std::max(a.y, b.y);
  for (int x = std::min(a.x, b.x); x <= xMax; x++) {
    for (int y = std::min(a.y, b.y); y <= yMax; y++) {
      (*this)(x, y).setType(type);
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
      if ((*this)(x, y).isPassable()) {
	if (!randTo(50 - depth)) {
	  //(*this)(x, y).setEnemy(getRandomEnemy());
	}
	else if (!randTo(/*80*/ 20 - depth)){
	  (*this)(x, y).setItem(getRandomItem());
	}
      }
    }
  }
}

void Map::sanitizeEntry() {
  for (int x = playerX - 2; x <= playerX + 2; x++) {
    for (int y = playerY - 2; y <= playerY + 2; y++) {
      if (isValidX(x) && isValidY(y)) {
	(*this)(x, y).removeEnemy();
      }
    }
  }
}
