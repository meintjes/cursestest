#include "Space.h"
#include "Cch.h"

class Map;

Space::Space() {
  discovered = false;
  type = &Wall;
  gasDuration = 0;
  bombDuration = 0;
  enemy = nullptr;
  item = nullptr;
}

void Space::setType(const SpaceType &typeIn) {
  type = &typeIn;
}

void Space::setItem(const Item &itemIn) {
  item = &itemIn;
}

bool Space::moveEnemy(Space *target) {
  if (target->isPassable() && !target->hasEnemy()) {
    target->enemy = std::unique_ptr<Enemy>(std::move(this->enemy));
    return true;
  }
  else {
    return false;
  }
}

void Space::setEnemy() {
  enemy = std::unique_ptr<Enemy>(new Exploder);
}

void Space::removeEnemy() {
  enemy = nullptr;
}

void Space::attack(Map &map, int x, int y) {
  if (enemy) {
    enemy->attack(map, x, y);
  }
}

void Space::kill(Map &map, int x, int y) {
  if (enemy) {
    enemy->die(map, x, y);
    removeEnemy();
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
}

bool Space::tick() {
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
    return enemy->getGlyph();
  }
  if (!type->passable) {
    return type->glyph;
  }
  
  if (item) {
    return item->glyph;
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
      return Orange('*');
    }
    else {
      return Red('*');
    }
  }

  return type->glyph;
}

bool Space::isPassable() const {
  return type->passable;
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
    return enemy->getRange(); 
  }
  else return 0;
}

bool Space::typeIs(const SpaceType *typeIn) const {
  return *type == *typeIn;
}
