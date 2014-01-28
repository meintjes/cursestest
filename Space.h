#ifndef __SPACE_H__
#define __SPACE_H__

#include "Item.h"
#include "Enemy.h"
#include "SpaceType.h"

class Player;
class Map;

class Space {
 public:
  Space();

  void setType(const SpaceType *typeIn);
  void setItem(const Item *itemIn);

  void moveEnemy(Space &target);
  void setEnemy(const Enemy *enemyIn);
  void attack(Map &map, int x, int y);
  void kill(Map &map, int x, int y);

  void addGas(unsigned int duration);
  void dropBomb();
  void pickup(Player &you);
  void explode();
  bool tick();

  //note: sets the space as discovered the first time it's called. sorry
  Cch getGlyph(bool isVisible) const;

  bool isPassable() const;
  bool isTransparent() const;
  bool hasEnemy() const;
  bool hasGas() const;
  bool hasBomb() const;

  int getRange() const;
  bool typeIs(const SpaceType *typeIn) const;

 private:
  mutable bool discovered;
  const SpaceType *type;
  unsigned int gasDuration;
  unsigned int bombDuration;
  unsigned int explosionDuration;
  const Enemy *enemy;
  const Item *item;
};

#endif
