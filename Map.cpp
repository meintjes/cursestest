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
  //if you have an arrow drawn, shoot it
  if ((dx != 0 || dy != 0) && you.shootArrow()) {
    // Decide the glyph for the arrow shot.
    char arrowChar;
    if (dx == 0) {
      arrowChar = '|';
    } else if (dy == 0) {
      arrowChar = '-';
    } else {
      if ((dx < 0 && dy > 0) || (dx > 0 && dy < 0)) {
        arrowChar = '/';
      } else {
        arrowChar = '\\';
      }
    }
    Cch arrowGlyph(Brown(arrowChar));

    for (int i = 0, x = playerX, y = playerY;
	 i < 6 && (space[x][y].isPassable() || space[x][y].hasEnemy());
	 i++) {
      space[x][y].kill(*this, x, y);
      x += dx;
      y += dy;

      if (isVisible(x, y, you.getLOS()) && space[x][y].isPassable()) {
        // Rewrite the actual state of things.
        display();

        // Write the arrow glyph.
        move(y, x);
        addc(arrowGlyph);
        refresh();

        // Pause for a moment to let the user see the animation.
        napms(25);
      }
    }
    return true;
  }
  //otherwise actually move
  else {
    Space *target = &space[playerX + dx][playerY + dy];
    if (target->hasEnemy()) {
      target->kill(*this, playerX + dx, playerY + dy);
      return true;
    }
    else if (target->isPassable()) {
      playerX += dx;
      playerY += dy;
      target->pickup(you);
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
  //try to move in diagonal direction
  Space *target = &space[x + sgn(playerX - x)][y + sgn(playerY - y)];
  if (!space[x][y].moveEnemy(target)) {
    //if it fails, try to move in only one direction
    //note: where dx == dy, moves in y direction first.
    //this is probably not as good as randomizing it
    if (abs(playerX - x) > abs(playerY - y)) { //move in x direction
      target = &space[x + sgn(playerX - x)][y];
      if (!space[x][y].moveEnemy(target)) {
	target = &space[x][y + sgn(playerY - y)];
	space[x][y].moveEnemy(target);
      }
    }
    else { //move in y direction
      target = &space[x][y + sgn(playerY - y)];
      if (!space[x][y].moveEnemy(target)) {
	target = &space[x + sgn(playerX - x)][y];
	space[x][y].moveEnemy(target);
      }
    }
  }
}

Space& Map::getSpace(int x, int y) {
  return space[x][y];
}

void Map::tick() {
  if (!you.tick()) { //don't update game state for free moves
    return;
  }

  bool damagedByGas = false;
  if (space[playerX][playerY].hasGas()) {
    you.damage();
    damagedByGas = true;
  }

  for (int x = 1; x <= MAPWIDTH; x++) {
    for (int y = 1; y <= MAPHEIGHT; y++) {
      //decrement durations of stuff on the space
      if (space[x][y].tick()) {
	explode(x, y, 1); //if a bomb went off
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

      else if (space[x][y].isPassable() && !randTo(1000)
	       && !isVisible(x, y, you.getLOS())) {
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

  //extra-permissive; checks if the space has LOS to you, OR if the space
  //one closer to you in each direction has LOS to you. this makes corridors
  //behave significantly nicer.
  return hasLOS(x, y, playerX, playerY) ||
    (space[x + sgn(playerX - x)][y + sgn(playerY - y)].isTransparent()
     && hasLOS(x + sgn(playerX - x), y + sgn(playerY - y), playerX, playerY));
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

void Map::explode(int x, int y, int radius) {
  assert(radius >= 0);
  for (int x2 = x - radius; x2 <= x + radius; x2++) {
    for (int y2 = y - radius; y2 <= y + radius; y2++) {
      if (isValidX(x) && isValidY(y)) {
	toExplode.push_back(Point{x2, y2});
      }
    }
  }
}

void Map::executeToExplode() {
  if (toExplode.size()) {
    display();
    for ( auto &point : toExplode ) {
      space[point.x][point.y].explode();
      move(point.y, point.x);
      addc(Red('#'));
    }
    refresh();
    napms(150);
    toExplode.clear();
  }
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
		      < distance(rhs.x, rhs.y, playerX, playerY));
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
