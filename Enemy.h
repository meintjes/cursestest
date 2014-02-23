#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Point.h"
#include <memory>

class Cch;
class Map;

class Enemy {
 public:
  Enemy();
  virtual Cch getGlyph() const = 0;
  virtual int getRange() const;
  virtual void attack(Map &map, int x, int y);
  virtual void die(Map &map, int x, int y);
  virtual ~Enemy();

  void renewMemory(Point playerLocation);
  bool hasMemory() const;
  Point getMemory() const;

  virtual void tick();

 private:
  int memoryDuration;
  Point memoryLocation;
};

std::unique_ptr<Enemy> getRandomEnemy();



class Zombie : public Enemy {
 public:
  Zombie();
  Cch getGlyph() const;
  void die(Map &map, int x, int y);
 private:
  int hp;
};

class Exploder : public Enemy {
 public:
  Exploder();
  Cch getGlyph() const;
  int getRange() const;
  void attack(Map &map, int x, int y);
  void die(Map &map, int x, int y);
 private:
  bool isPrimed;
};

class Reacher : public Enemy {
 public:
  using Enemy::Enemy;
  Cch getGlyph() const;
  int getRange() const;
};

class SpawnerBoss : public Enemy {
 public:
  SpawnerBoss();
  Cch getGlyph() const;
  int getRange() const;
  void attack(Map &map, int x, int y);
  void die(Map &map, int x, int y);
 private:
  int hp;
};

#endif
