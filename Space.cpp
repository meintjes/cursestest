#include "Space.h"
#include "Archive.h"
#include "generation.h"
#include "Item.h"
#include "Enemy.h"
#include "Cch.h"
#include "Color.h"
#include "functions.h"
#include <cassert>

void Space::serialize(Archive &ar) {
  ar & discovered;
  ar & gasDuration;
  ar & bombDuration;

  serializeUnique(item, ar);
  serializeUnique(enemy, ar);

  if (ar.getType() == Archive::Save) {
    ar << type;
  }
  else {
    int t; //enum types can't be read directly.
    ar >> t;
    type = static_cast<Type>(t);
  }
}

Space::Space() :
  discovered(false),
  type(Wall),
  gasDuration(0),
  bombDuration(0),
  enemy(nullptr),
  item(nullptr)
{}

Space::~Space() {

}

void Space::setType(Space::Type typeIn) {
  type = typeIn;
}

void Space::setItem(SimpleItem * const itemIn) {
  item = std::unique_ptr<SimpleItem>(itemIn);
}

void Space::setItem(std::unique_ptr<SimpleItem> itemIn) {
  item = std::move(itemIn);
}

bool Space::moveEnemy(Space &target) {
  assert(enemy);

  if (target.isPassable() && !target.hasEnemy()) {
    target.setEnemy(std::move(this->enemy));
    return true;
  }
  else {
    return false;
  }
}

void Space::setEnemy(Enemy *enemyIn) {
  assert(isPassable());
  enemy = std::unique_ptr<Enemy>(enemyIn);
}

void Space::setEnemy(std::unique_ptr<Enemy> enemyIn) {
  assert(isPassable());
  enemy = std::move(enemyIn);
}

void Space::removeEnemy() {
  enemy = nullptr;
}

bool Space::act(Map &map, int x, int y) {
  if (enemy) {
    return enemy->act(map, x, y);
  }
  else {
    return false;
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
    bombDuration = 32;
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
  if (isDestructible()) {
    setType(Floor);
  }
  damage(3, map, x, y);
}

bool Space::tick(unsigned int duration, Map &map, int x, int y) {
  decrementDuration(gasDuration, duration);
  
  if (enemy) {
    enemy->tick(duration, map, x, y);
  }

  if (bombDuration > 0) {
    decrementDuration(bombDuration, duration);
    if (bombDuration == 0) {
      return true;
    }
  }
  return false;
}

void Space::addTimeToAct(unsigned int duration) {
  if (enemy) {
    enemy->addTimeToAct(duration);
  }
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
      return DarkGray(getTypeGlyph());
    }
  }

  if (enemy) {
    return enemy->getGlyph();
  }
  if (!isPassable()) {
    return getTypeGlyph();
  }
  
  if (gasDuration > 0) {
    if (gasDuration > 8) {
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
    if (bombDuration > 8) {
      return Orange('*');
    }
    else {
      return Red('*');
    }
  }

  return getTypeGlyph();
}

Cch Space::getTypeGlyph() const {
  switch (type) {
  case Wall:
    return White('#');
  case GlassWall:
    return LightCyan('#');
  case Floor:
    return White('.');
  case StairsUp:
    return White('<');
  case StairsDown:
    return White('>');
  default:
    return Red('X');
  }
}

bool Space::isPassable() const {
  switch (type) {
  case Wall:
  case GlassWall:
    return false;
  default:
    return true;
  }
}

bool Space::isTransparent() const {
  switch (type) {
  case Wall:
    return false;
  default:
    return true;
  }
}

bool Space::isDestructible() const {
  switch (type) {
  case GlassWall:
    return true;
  default:
    return false;
  }
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

bool Space::isStunned() const {
  return (enemy && enemy->isStunned() > 0);
}

void Space::stun(unsigned int duration) {
  if (enemy) {
    enemy->stun(duration);
  }
}

bool Space::typeIs(Space::Type typeIn) const {
  return type == typeIn;
}
