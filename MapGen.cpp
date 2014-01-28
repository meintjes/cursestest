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
  space[playerX][playerY].setType(&StairsUp);
}

void Map::drawLine(Point a, const Point b, const SpaceType *type) {
  //this is horrific style, but fuck it
  if (randTo(1)) {
    while (a.x - b.x) {
      space[a.x][a.y].setType(type);
      a.x += sgn(b.x - a.x);
      if (!randTo(10)) { //creates wavy lines rather than straight angles
	drawLine(a, b, type);
	return;
      }
    }
    while (a.y - b.y) {
      space[a.x][a.y].setType(type);
      a.y += sgn(b.y - a.y);
    }
  }
  else {
    while (a.y - b.y) {
      space[a.x][a.y].setType(type);
      a.y += sgn(b.y - a.y);
      if (!randTo(10)) { //creates wavy lines rather than straight angles
	drawLine(a, b, type);
	return;
      }
    }
    while (a.x - b.x) {
      space[a.x][a.y].setType(type);
      a.x += sgn(b.x - a.x);
    }
  }
  space[a.x][a.y].setType(type);
}

void Map::drawBox(Point a, Point b, const SpaceType *type) {
  int xMax = std::max(a.x, b.x);
  int yMax = std::max(a.y, b.y);
  for (int x = std::min(a.x, b.x); x <= xMax; x++) {
    for (int y = std::min(a.y, b.y); y <= yMax; y++) {
      assert(x <= MAPWIDTH && y <= MAPHEIGHT);
      space[x][y].setType(type);
    }
  }
}

void Map::generateRoom(Point center, int maxRadius) {
  int xMin = std::max(1, center.x - randRange(1, maxRadius));
  int xMax = std::min(MAPWIDTH, center.x + randRange(1, maxRadius));
  int yMin = std::max(1, center.y - randRange(1, maxRadius));
  int yMax = std::min(MAPHEIGHT, center.y + randRange(1, maxRadius));
  drawBox(Point{xMin, yMin}, Point{xMax, yMax}, &Floor);
}

void Map::generateBoxes(int depth) {
  std::vector<Point> roomLocations;

  for (int i = randRange(8, 10); i > 0; i--) {
    Point center{randRange(1, MAPWIDTH), randRange(1, MAPHEIGHT)};
    generateRoom(center, 4);
    roomLocations.push_back(center);
  }

  for (unsigned int i = 1; i < roomLocations.size(); i++) {
    drawLine(roomLocations.at(i), roomLocations.at(i - 1), &Floor);
  }

  playerX = roomLocations.at(0).x;
  playerY = roomLocations.at(0).y;
  space[roomLocations.at(roomLocations.size() - 1).x]
       [roomLocations.at(roomLocations.size() - 1).y].setType(&StairsDown);
}
