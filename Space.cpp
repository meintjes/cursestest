#include <cassert>
#include "Space.h"
#include "Cch.h"

class Map;

Space::Space() :
  discovered(false),
  lit(false),
  type(&Wall),
  gasDuration(0),
  bombDuration(0),
  enemy(nullptr),
  item(nullptr)
{}

void Space::setType(const SpaceType &typeIn) {
  type = &typeIn;
}

void Space::setItem(SimpleItem * const itemIn) {
  item = std::unique_ptr<SimpleItem>(itemIn);
}

void Space::setItem(std::unique_ptr<SimpleItem> itemIn) {
  item = std::move(itemIn);
}

bool Space::moveEnemy(Space *target) {
  if (target->isPassable() && !target->hasEnemy()) {
    target->setEnemy(std::move(this->enemy));
    return true;
  }
  else {
    return false;
  }
}

void Space::setEnemy(Enemy *enemyIn) {
  enemy = std::unique_ptr<Enemy>(enemyIn);
}

void Space::setEnemy(std::unique_ptr<Enemy> enemyIn) {
  enemy = std::move(enemyIn);
}

void Space::removeEnemy() {
  enemy = nullptr;
}

void Space::attack(Map &map, int x, int y) {
  if (enemy) {
    enemy->attack(map, x, y);
  }
}

void Space::damage(int num, Map &map, int x, int y) {
  if (enemy) {
    //doesn't remove the enemy. the enemy is responsible for calling
    //removeEnemy() if it needs to.
    enemy->damage(num, map, x, y);
  }
}

void Space::addGas(unsigned int duration) {
  gasDuration += duration;
}

bool Space::dropBomb() {
  if (bombDuration <= 0) {
    bombDuration = 4;
    return true;
  }
  else {
    return false;
  }
}

void Space::pickup(Player &you) {
  if (!item) {
    return;
  }
  
  switch (item->pickup(you)) {
  case Item::Release:
    item.release();
    break;
  case Item::Destroy:
    item = nullptr;
    break;
  default:
    break;
  }
}

void Space::explode(Map &map, int x, int y) {
  if (type->destructible) {
    type = &Floor;
  }
  damage(3, map, x, y);
}

void Space::light() {
  lit = 2;
}

bool Space::tick() {
  if (lit > 0) {
    lit--;
  }
  if (gasDuration > 0) {
    gasDuration--;
  }
  if (enemy) {
    enemy->tick();
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
      return item->getGlyph();
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
  
  if (gasDuration > 0) {
    if (gasDuration > 1) {
      return LightGreen('*');
    }
    else {
      return DarkGreen('*');
    }
  }

  if (item) {
    return item->getGlyph();
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

bool Space::hasItem() const {
  return (item != nullptr);
}

bool Space::isLit() const {
  return lit;
}

void Space::renewMemory(Point playerPosition) {
  assert(enemy);
  enemy->renewMemory(playerPosition);
}

bool Space::hasMemory() const {
  assert(enemy);
  return enemy->hasMemory();
}

Point Space::getMemory() const {
  assert(enemy);
  return enemy->getMemory();
}

int Space::getRange() const {
  if (enemy) {
    return enemy->getRange(); 
  }
  else return 0;
}

bool Space::isStunned() const {
  return (enemy && enemy->isStunned() > 0);
}

void Space::stun(unsigned int turns) {
  if (enemy) {
    enemy->stun(turns);
  }
}

bool Space::typeIs(const SpaceType &typeIn) const {
  return *type == typeIn;
}
