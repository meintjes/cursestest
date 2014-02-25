#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "Item.h"
#include "Player.h"

class Map;

class Weapon : public DestructibleItem {
 public:
  using DestructibleItem::DestructibleItem;
  bool pickup(Player &you);
  virtual bool attack(Map *map, int dx, int dy);

 protected:
  virtual std::string name() const = 0;
  virtual const Color& color() const = 0;

 private:
  char glyph() const;
}

#endif
