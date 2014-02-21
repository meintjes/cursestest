#ifndef __ENEMY_H__
#define __ENEMY_H__

#include <functional>
#include "Point.h"

class Cch;
class Map;

class Enemy {
 public:
  Enemy();
  virtual Cch getGlyph() const = 0;
  virtual int getRange() const;
  virtual void attack(Map &map, int x, int y) = 0;
  virtual void die(Map &map, int x, int y) = 0;
  virtual ~Enemy();
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

#endif
