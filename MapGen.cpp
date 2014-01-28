#include "Map.h"
#include "items.h"
#include "enemies.h"
#include "functions.h"

#include <assert.h>

Map::Map(Player &player) : you(player) {
  switch (randTo(0)) {
  default:
    generateBoxes();
  }
  space[playerX][playerY].setType(&StairsUp);
}

void Map::drawLine(Point a, Point b, const SpaceType *type) {
  if (randTo(1)) {
    std::swap(a, b);
  }

  while (a.x - b.x) {
    assert(a.x <= MAPWIDTH && a.y <= MAPHEIGHT);
    space[a.x][a.y].setType(&Floor);
    a.x += sgn(b.x - a.x);
  }
  while (b.y - a.y) {
    assert(b.x <= MAPWIDTH && b.y <= MAPHEIGHT);
    space[b.x][b.y].setType(&Floor);
    b.y += sgn(a.y - b.y);
  }
  space[a.x][b.y].setType(&Floor);
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

void Map::generateBoxes() {
  std::vector<Point> roomLocations;

  for (int i = randRange(8, 10); i > 0; i--) {
    Point center{randRange(1, MAPWIDTH), randRange(1, MAPHEIGHT)};
    generateRoom(center, 4);
    roomLocations.push_back(center);
  }

  for (unsigned int i = 0; i < roomLocations.size(); i++) {
    drawLine(roomLocations.at(i),
	     roomLocations.at(randTo(roomLocations.size() - 1)), &Floor);
  }

  playerX = roomLocations.at(0).x;
  playerY = roomLocations.at(0).y;
  space[roomLocations.at(1).x][roomLocations.at(1).y].setType(&StairsDown);
}
