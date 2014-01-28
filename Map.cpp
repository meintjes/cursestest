#include "Map.h"
#include "Cch.h"
#include "Color.h"
#include <ncurses.h>
#include <algorithm>

#include <assert.h>

void Map::display() const {
  move(0, 0);
  for (int y = 0; y <= MAPHEIGHT + 1; y++) {
    for (int x = 0; x <= MAPWIDTH + 1; x++) {
      if (isVisible(x, y)) {
	if (x == playerX && y == playerY) {
	  Cch{'@', BlackOnWhite}.add();
	}
	else {
	  space[x][y].getGlyph(true).add();
	}
      }
      else {
	space[x][y].getGlyph(false).add();
      }
    }
    addch('\n');
  }
}

int Map::getPlayerX() const {
  return playerX;
}

int Map::getPlayerY() const {
  return playerY;
}

bool Map::getInput() {
  switch (getch()) {
  case '7':
    return this->movePlayer(-1, -1);
  case '8':
    return this->movePlayer(0, -1);
  case '9':
    return this->movePlayer(1, -1);
  case '4':
    return this->movePlayer(-1, 0);
  case '5':
    return this->movePlayer(0, 0);
  case '6':
    return this->movePlayer(1, 0);
  case '1':
    return this->movePlayer(-1, 1);
  case '2':
    return this->movePlayer(0, 1);
  case '3':
    return this->movePlayer(1, 1);

  case 'a':
    return this->dropBomb();
  case 's':
    return you.lightTorch();
  case 'd':
    return this->shootArrow();

  case '<':
    return this->changeFloor(-1, &StairsUp);
  case '>':
    return this->changeFloor(+1, &StairsDown);

  default:
    return false;
  }
}

bool Map::movePlayer(int dx, int dy) {
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

bool Map::dropBomb() {
  if (!space[playerX][playerY].hasBomb() && you.dropBomb()) {
    space[playerX][playerY].dropBomb();
    return true;
  }
  else {
    return false;
  }
}

bool Map::shootArrow() {
  if (you.shootArrow()) {
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
	if (distance(x, y, playerX, playerY) <= space[x][y].getRange()) {
	  toAttack.push_back(Point{x, y});
	}
	//enemies outside range of the player try to move toward him
	else if (distance(x, y, playerX, playerY) <= 7) {
	  toMove.push_back(Point{x, y});
	}
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

bool Map::isVisible(int x, int y) const {
  if (distance(x, y, playerX, playerY) > you.getLOS()) {
    return false;
  }
  else {
    while (x != playerX || y != playerY) {
      std::pair<int, int> result = direction(x, y, playerX, playerY);
      x += result.first;
      y += result.second;
      if (!space[x][y].isTransparent()) {
	return false;
      }
    }
    return true;
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
  int dx = (x2 - x1) ? ((x2 - x1) / abs(x2 - x1)) : 0;
  int dy = (y2 - y1) ? ((y2 - y1) / abs(y2 - y1)) : 0;
  return std::pair<int, int>(dx, dy);
}
