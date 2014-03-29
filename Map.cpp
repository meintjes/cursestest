#include "Archive.h"
#include "Map.h"
#include "Cch.h"
#include "Color.h"
#include "Player.h"
#include "functions.h"
#include "Command.h"
#include <ncurses.h>
#include <algorithm>
#include "generation.h"
#include <assert.h>

//unary constructor that doesn't initialize the map at all. only to be used
//for constructing maps that are about to be serialized.
Map::Map(Player &player) :
  you(player)
{}

//note: the reference to the player isn't (de)serialized. construct the map
//with the unary constructor, then serialize it.
void Map::serialize(Archive &ar) {
  ar & playerX;
  ar & playerY;
  ar & enemyCount;
  for (int x = 0; x <= MAPWIDTH + 1; x++) {
    for (int y = 0; y <= MAPHEIGHT + 1; y++) {
      (*this)(x, y).serialize(ar);
    }
  }
}

void Map::display() const {
  for (int y = 0; y <= MAPHEIGHT + 1; y++) {
    for (int x = 0; x <= MAPWIDTH + 1; x++) {
      if (isVisible(x, y)) {
	if (x == playerX && y == playerY) {
	  addc(x, y, you.getGlyph());
	}
	else {
	  addc(x, y, (*this)(x, y).getGlyph(true));
	}
      }
      else {
	addc(x, y, (*this)(x, y).getGlyph(false));
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
  Space *target = &(*this)(playerX + dx, playerY + dy);
  if (target->hasEnemy()) {
    return you.attack(dx, dy);
  }
  else if (target->isPassable()) {
    playerX += dx;
    playerY += dy;
    you.setLastMoveDirection({dx, dy});
    target->pickup(you);
    return true;
  }
  else {
    return false;
  }
}

bool Map::shootArrow(int dx, int dy) {
  // Decide the glyph for the arrow shot.
  char arrowChar;
  if (dx == 0) {
    arrowChar = '|';
  }
  else if (dy == 0) {
    arrowChar = '-';
  }
  else if ((dx < 0 && dy > 0) || (dx > 0 && dy < 0)) {
    arrowChar = '/';
  }
  else {
    arrowChar = '\\';
  }
  Cch arrowGlyph(Brown(arrowChar));

  for (int i = 0, x = playerX, y = playerY;
       i < 6 && (*this)(x, y).isPassable(); i++) {
    (*this)(x, y).damage(1, *this, x, y);
    x += dx;
    y += dy;
    
    if (isVisible(x, y) && (*this)(x, y).isPassable()) {
      // Rewrite the actual state of things.
      display();

      // Write the arrow glyph.
      addc(x, y, arrowGlyph);
      refresh();

      // Pause for a moment to let the user see the animation.
      napms(25);
    }
  }
  you.destroyModeItem();
  you.setMode(Player::Mode::Move);
  return true;
}

bool Map::throwHook(int dx, int dy) {
  Cch hookGlyph = LightGray('&');
  Cch chainGlyph = LightGray('*');
  for (int i = 0, x = playerX, y = playerY; i < 6; i++) {
    x += dx;
    y += dy;
    bool visible = isVisible(x, y);
    if (visible) {
      addc(x, y, hookGlyph);
    }

    //if the hook hits a wall, pull the player toward it and stop
    if (!(*this)(x, y).isPassable()) {
      playerX = x - dx;
      playerY = y - dy;
      
      you.destroyModeItem();
      you.setMode(Player::Mode::Move);
      return true;
    }
    //if the hook hits an enemy, pull the enemy toward the player and stop
    else if ((*this)(x, y).hasEnemy()) {
      if (i > 0) { //don't stun enemies that are standing directly next to you
        (*this)(x, y).stun(3);
        (*this)(x, y).moveEnemy(&(*this)(playerX + dx, playerY + dy));
      }
      
      you.destroyModeItem();
      you.setMode(Player::Mode::Move);
      return true;
    }
    //if the hook hits nothing, draw the animation
    else {
      refresh();
      napms(80);
      if (visible) {
        addc(x, y, chainGlyph);
      }
    }
  }

  //if the hook never hits anything, don't destroy it, just put it away
  you.setMode(Player::Mode::Move);
  return true;
}

void Map::lightArea(int x, int y, int radius, int turns) {
  for (int x2 = x - radius; x2 <= x + radius; x2++) {
    for (int y2 = y - radius; y2 <= y + radius; y2++) {
      if (isValidX(x2) && isValidY(y2)) {
        (*this)(x2, y2).light(turns);
      }
    }
  }
}

void Map::moveEnemy(int x, int y) {
  //try to move in diagonal direction
  Point target = (*this)(x, y).getMemory();
  Space *dest = &(*this)(x + sgn(target.x - x), y + sgn(target.y - y));
  if (!(*this)(x, y).moveEnemy(dest)) {
    //if it fails, try to move in only one direction
    //note: where dx == dy, moves in y direction first.
    //this is probably not as good as randomizing it
    if (abs(target.x - x) > abs(target.y - y)) { //move in x direction
      dest = &(*this)(x + sgn(target.x - x), y);
      if (!(*this)(x, y).moveEnemy(dest)) { //else move in y direction
	dest = &(*this)(x, y + sgn(target.y - y));
	(*this)(x, y).moveEnemy(dest);
      }
    }
    else { //move in y direction
      dest = &(*this)(x, y + sgn(target.y - y));
      if (!(*this)(x, y).moveEnemy(dest)) { // else move in x direction
	dest = &(*this)(x + sgn(target.x - x), y);
	(*this)(x, y).moveEnemy(dest);
      }
    }
  }
}

const Space& Map::operator()(int x, int y) const {
  assert(isValidX(x) && isValidY(y));
  return space[x][y];
}

Space& Map::operator()(int x, int y) {
  assert(isValidX(x) && isValidY(y));
  return space[x][y];
}

void Map::tick() {
  if (!you.tick()) { //don't update game state for free moves
    return;
  }

  bool damagedByGas = false;
  if ((*this)(playerX, playerY).hasGas()) {
    you.damage(1);
    damagedByGas = true;
  }
 
  int newEnemyCount = 0; 
  for (int x = 0; x <= MAPWIDTH + 1; x++) {
    for (int y = 0; y <= MAPHEIGHT + 1; y++) {
      //decrement durations of stuff on the space
      if ((*this)(x, y).tick(*this, x, y)) {
	explodeArea(x, y, 1); //if a bomb went off
      }

      if ((*this)(x, y).hasEnemy()) {
        if (!(*this)(x, y).isStunned()) {
          //enemies within range attack the player
          if (isVisible(x, y) && (*this)(x, y).getRange() >= distance(x, y)) {
            (*this)(x, y).renewMemory({playerX, playerY});
            toAttack.emplace_back(x, y);
          }
          //enemies outside range of the player try to move toward him
          else if (isVisible(x, y) && distance(x, y) < 7) {
            (*this)(x, y).renewMemory({playerX, playerY});
            toMove.emplace_back(x, y);
          }
          else if ((*this)(x, y).hasMemory()) {
            toMove.emplace_back(x, y);
          }
        }
        //even if the enemy is stunned, count it
        newEnemyCount++;
      }
      //random enemy generation. slows down the more enemies there are
      else if (!randTo(16 * (enemyCount * enemyCount + 32)) &&
               !(*this)(x, y).isLit() &&
               (*this)(x, y).isPassable() &&
               !(*this)(x, y).hasEnemy()) {
        (*this)(x, y).setEnemy(getRandomEnemy());
        newEnemyCount++;
      }
    }
  }

  enemyCount = newEnemyCount;

  executeToMove();
  executeToExplode();
  executeToAttack();

  //kludge to handle gas clouds appearing as part of an attack
  if ((*this)(playerX, playerY).hasGas() && !damagedByGas) {
    you.damage(1);
  }

  assert(!(*this)(playerX, playerY).hasEnemy());
}

bool Map::isValidX(int x) {
  return (x >= 0 && x <= MAPWIDTH + 1);
}

bool Map::isValidY(int y) {
  return (y >= 0 && y <= MAPHEIGHT + 1);
}

bool Map::isVisible(int x, int y) const {
  if (!(*this)(x, y).isLit()) {
    return false;
  }

  //extra-permissive; checks if the space has LOS to you, OR if the space
  //one closer to you in each direction has LOS to you. this makes corridors
  //behave significantly nicer.
  return hasLOS(x, y, playerX, playerY) ||
    ((*this)(x + sgn(playerX - x), y + sgn(playerY - y)).isTransparent()
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
    if (!(*this)(x1, y1).isTransparent()) {
      return false;
    }
  }
}

void Map::explodeArea(int x, int y, int radius) {
  assert(radius >= 0);
  for (int x2 = x - radius; x2 <= x + radius; x2++) {
    for (int y2 = y - radius; y2 <= y + radius; y2++) {
      if (isValidX(x) && isValidY(y)) {
	toExplode.emplace_back(x2, y2);
      }
    }
  }
}

void Map::executeToExplode() {
  if (toExplode.size()) {
    display();
    for ( auto &point : toExplode ) {
      (*this)(point.x, point.y).explode(*this, point.x, point.y);
      addc(point.x, point.y, Red('#'));
    }
    refresh();
    napms(150);
    toExplode.clear();
  }
}

void Map::executeToAttack() {
  for ( auto &point : toAttack ) {
    (*this)(point.x, point.y).attack(*this, point.x, point.y);
  }
  toAttack.clear();
}

void Map::executeToMove() {
  //closest to player move first, to reduce collisions
  std::sort(toMove.begin(), toMove.end(),
	    [&](Point lhs, Point rhs) {
	      return (distance(lhs.x, lhs.y)
		      < distance(rhs.x, rhs.y));
	    }
	    );
  for ( auto &point : toMove ) {
    moveEnemy(point.x, point.y);
  }
  toMove.clear();
}

int Map::distance(int x1, int y1) const {
  return std::max(abs(x1-playerX), abs(y1-playerY));
}
