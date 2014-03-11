#ifndef __SPACE_H__
#define __SPACE_H__

#include "Item.h"
#include "Enemy.h"
#include "SpaceType.h"
#include <memory> 

class Player;
class Map;

class Space {
 public:
  Space();

  void setType(const SpaceType &typeIn);
  void setItem(SimpleItem * const itemIn);
  void setItem(std::unique_ptr<SimpleItem> itemIn);

  bool moveEnemy(Space *target);
  void setEnemy(Enemy *enemyIn);
  void setEnemy(std::unique_ptr<Enemy> enemyIn);
  void removeEnemy();
  void attack(Map &map, int x, int y);
  void kill(Map &map, int x, int y);

  void addGas(unsigned int duration);
  void dropBomb();
  void pickup(Player &you);
  void explode(Map &map, int x, int y);
  bool tick();

  //note: sets the space as discovered the first time it's called. sorry
  Cch getGlyph(bool isVisible) const;

  bool isPassable() const;
  bool isTransparent() const;
  bool hasEnemy() const;
  bool hasGas() const;
  bool hasBomb() const;
  bool hasItem() const;

  void renewMemory(Point playerPosition);
  bool hasMemory() const;
  Point getMemory() const;
  int getRange() const;
  bool isStunned() const;
  void stun(unsigned int turns);

  bool typeIs(const SpaceType &typeIn) const;

 private:
  mutable bool discovered;
  const SpaceType *type;
  unsigned int gasDuration;
  unsigned int bombDuration;
  std::unique_ptr<Enemy> enemy;
  std::unique_ptr<SimpleItem> item;
};

#endif
