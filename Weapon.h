#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "Item.h"

class Map;
class Player;

class Weapon : public DestructibleItem {
 public:
  using DestructibleItem::DestructibleItem;

  Item::UseResult use(Map *map);

  virtual void attack(Map *map, int dx, int dy) = 0;

 protected:
  virtual std::string name() const = 0;
  virtual const Color& color() const = 0;

 private:
  char glyph() const;
};

class Axe : public Weapon {
 public:
  using DestructibleItem::DestructibleItem;

  void attack(Map *map, int dx, int dy);
 protected:
  std::string name() const;
  const Color& color() const;
};

class Bludgeon : public Weapon {
 public:
  using DestructibleItem::DestructibleItem;

  void attack(Map *map, int dx, int dy);

 protected:
  std::string name() const;
  const Color& color() const;
};

class Lance : public Weapon {
 public:
  using DestructibleItem::DestructibleItem;

  void attack(Map *map, int dx, int dy);
 protected:
  std::string name() const;
  const Color& color() const;
};

class Spear : public Weapon {
 public:
  using DestructibleItem::DestructibleItem;

  void attack(Map *map, int dx, int dy);
 protected:
  std::string name() const;
  const Color& color() const;
};

#endif
