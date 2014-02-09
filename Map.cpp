#include "Map.h"
#include "Cch.h"
#include "Color.h"
#include "functions.h"
#include "Command.h"
#include <ncurses.h>
#include <algorithm>

#include <assert.h>

void Map::display() const {
  for (int y = 0; y <= MAPHEIGHT + 1; y++) {
    move(y, 0);
    for (int x = 0; x <= MAPWIDTH + 1; x++) {
      if (isVisible(x, y, you.getLOS())) {
	if (x == playerX && y == playerY) {
	  addc(you.getGlyph());
	}
	else {
	  addc(space[x][y].getGlyph(true));
	}
      }
      else {
	addc(space[x][y].getGlyph(false));
      }
    }
  }
}

int Map::getPlayerX() const {
  return playerX;
}

int Map::getPlayerY() const {
  return playerY;
}

bool Map::movePlayer(int dx, int dy) {
  if ((dx != 0 || dy != 0) && you.shootArrow()) {
    for (int i = 0, x = playerX, y = playerY;
	 i < 6 && (space[x][y].isPassable() || space[x][y].hasEnemy());
	 i++) {
      space[x][y].kill(*this, x, y);
      x += dx;
      y += dy;
    }
    return true;
  }
  else {
    Space *target = &space[playerX + dx][playerY + dy];
    if (target->isPassable()) {
      playerX += dx;
      playerY += dy;
      target->pickup(you);
      return true;
    }
    else if (target->hasEnemy()) {
      target->kill(*this, playerX + dx, playerY + dy);
      return true;
    }
    else {
      return false;
    }
  }
}

bool Map::dropBomb() {
  if (!space[playerX][playerY].hasBomb() && you.dropBomb()) {
    space[playerX][playerY].dropBomb();
    return true;
  }
  else {
    return false;
  }
}

void Map::moveEnemy(int x, int y) {
  std::pair<int, int> dir = direction(x, y, playerX, playerY);
  Space &target = space[x + dir.first][y + dir.second];
  
  space[x][y].moveEnemy(target);
}

Space& Map::getSpace(int x, int y) {
  return space[x][y];
}

void Map::tick(int turns) {
  you.tick();
  bool damagedByGas = false;
  if (space[playerX][playerY].hasGas()) {
    you.damage();
    damagedByGas = true;
  }

  //decrement gas cloud and bomb timer durations
  for (int x = 1; x <= MAPWIDTH; x++) {
    for (int y = 1; y <= MAPHEIGHT; y++) {
      if (space[x][y].tick()) { //if a bomb went off
	for (int x2 = x - 1; x2 <= x + 1; x2++) {
	  for (int y2 = y - 1; y2 <= y + 1; y2++) {
	    toExplode.push_back(Point{x2, y2});
	  }
	}
      }
      if (space[x][y].hasEnemy()) {
	//enemies within range attack the player
	if (isVisible(x, y, space[x][y].getRange())) {
	  toAttack.push_back(Point{x, y});
	}
	//enemies outside range of the player try to move toward him
	else if (isVisible(x, y, 7)) {
	  toMove.push_back(Point{x, y});
	}
      }
      else if (space[x][y].isPassable() && !isVisible(x, y, 7) && !randTo(750)) {
	space[x][y].setEnemy(getRandomEnemy());
      }
    }
  }

  executeToMove();
  executeToExplode();
  executeToAttack();

  //kludge to handle gas clouds appearing as part of an attack
  if (space[playerX][playerY].hasGas() && !damagedByGas) {
    you.damage();
  }

  //handle multi-turn ticks
  if (turns > 1) {
    tick(turns - 1);
  }
}

bool Map::isValidX(int x) {
  return (x >= 0 && x <= MAPWIDTH + 1);
}

bool Map::isValidY(int y) {
  return (y >= 0 && y <= MAPHEIGHT + 1);
}

bool Map::isVisible(int x, int y, int LOS) const {
  if (distance(x, y, playerX, playerY) > LOS) {
    return false;
  }

  if (space[x + sgn(playerX - x)][y + sgn(playerY - y)].isTransparent()) {
    return hasLOS(x, y, playerX, playerY) ||
      hasLOS(x + sgn(playerX - x), y + sgn(playerY - y), playerX, playerY);
  }
  else {
    return hasLOS(x, y, playerX, playerY);
  }
}

bool Map::hasLOS(int x1, int y1, int x2, int y2) const {
  int dx = abs(x2 - x1);
  int dy = abs(y2 - y1);
  int sgnx = sgn(x2 - x1);
  int sgny = sgn(y2 - y1);

  //bresenham's line algorithm, roughly. thanks wikipedia
  int error = dx - dy;
  while(true) {
    if (x1 == x2 && y1 == y2) {
      return true;
    }
    int e2 = 2*error;
    if (e2 > -dy) {
      error -= dy;
      x1 += sgnx;
    }
    if (x1 == x2 && y1 == y2) {
      return true;
    }
    if (e2 < dx) {
      error += dx;
      y1 += sgny;
    }
    if (!space[x1][y1].isTransparent()) {
      return false;
    }
  }
}

bool Map::changeFloor(int dz, const SpaceType *type) {
  if (space[playerX][playerY].typeIs(type)) {
    return you.changeDepth(dz);
  }
  else {
    return false;
  }
}

void Map::executeToExplode() {
  for ( auto &point : toExplode ) {
    space[point.x][point.y].explode();
  }
  toExplode.clear();
}

void Map::executeToAttack() {
  for ( auto &point : toAttack ) {
    space[point.x][point.y].attack(*this, point.x, point.y);
  }
  toAttack.clear();
}

void Map::executeToMove() {
  //closest to player move first, to reduce collisions
  std::sort(toMove.begin(), toMove.end(),
	    [&](Point lhs, Point rhs) {
	      return (distance(lhs.x, lhs.y, playerX, playerY)
		      < distance(lhs.x, lhs.y, playerX, playerY));
	    }
	    );
  for ( auto &point : toMove ) {
    moveEnemy(point.x, point.y);
  }
  toMove.clear();
}

int Map::distance(int x1, int y1, int x2, int y2) {
  return std::max(abs(x1-x2), abs(y1-y2));
}

std::pair<int, int> Map::direction(int x1, int y1, int x2, int y2) {
  return std::pair<int, int>(sgn(x2 - x1), sgn(y2 - y1));
}
