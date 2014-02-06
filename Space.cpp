#include "Space.h"
#include "Cch.h"

class Map;

Space::Space() {
  discovered = false;
  type = &Wall;
  gasDuration = 0;
  bombDuration = 0;
  explosionDuration = 0;
  enemy = nullptr;
  item = nullptr;
}

void Space::setType(const SpaceType &typeIn) {
  type = &typeIn;
}

void Space::setItem(const Item &itemIn) {
  item = &itemIn;
}

void Space::moveEnemy(Space &target) {
  if (target.isPassable()) {
    target.enemy = this->enemy;
    this->enemy = nullptr;
  }
}

void Space::setEnemy(const Enemy &enemyIn) {
  enemy = &enemyIn;
}

void Space::setEnemy() {
  enemy = nullptr;
}

void Space::attack(Map &map, int x, int y) {
  if (enemy) {
    enemy->attackFunction(map, x, y);
  }
}

void Space::kill(Map &map, int x, int y) {
  if (enemy) {
    enemy->deathFunction(map, x, y);
  }
}

void Space::addGas(unsigned int duration) {
  gasDuration += duration;
}

void Space::dropBomb() {
  bombDuration = 4;
}

void Space::pickup(Player &you) {
  if (item && item->pickupFunction(you)) {
    item = nullptr;
  }
}

void Space::explode() {
  if (type->destructible) {
    type = &Floor;
  }
  enemy = nullptr;
  explosionDuration++;
}

bool Space::tick() {
  if (explosionDuration > 0) {
    explosionDuration--;
  }
  if (gasDuration > 0) {
    gasDuration--;
  }
  if (bombDuration > 0) {
    bombDuration--;
    if (bombDuration == 0) {
      return true;
    }
  }
  
  return false;
}

Cch Space::getGlyph(bool isVisible) const {
  if (!discovered) {
    if (!isVisible) {
      return DarkGray(' ');
    }
    else {
      discovered = true;
    }
  }
  else if (!isVisible) {
    if (item) {
      return item->glyph;
    }
    else {
      return DarkGray(type->glyph);
    }  
  }

  if (enemy) {
    return enemy->glyph;
  }
  if (!type->passable) {
    return type->glyph;
  }

  if (item) {
    return item->glyph;
  }

  if (explosionDuration > 0) {
    return Red('#');
  }

  if (gasDuration > 0) {
    if (gasDuration > 1) {
      return LightGreen('*');
    }
    else {
      return DarkGreen('*');
    }
  }

  if (bombDuration > 0) {
    if (bombDuration > 1) {
      return Orange('!');
    }
    else {
      return Red('!');
    }
  }

  return type->glyph;
}

bool Space::isPassable() const {
  return (type->passable && !enemy);
}

bool Space::isTransparent() const {
  return (type->transparent);
}

bool Space::hasEnemy() const {
  return (enemy != nullptr);
}

bool Space::hasGas() const {
  return (gasDuration > 0);
}

bool Space::hasBomb() const {
  return (bombDuration > 0);
}

int Space::getRange() const {
  if (enemy) {
    return enemy->range; 
  }
  else return 0;
}

bool Space::typeIs(const SpaceType *typeIn) const {
  return *type == *typeIn;
}
