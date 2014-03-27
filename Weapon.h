#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "Item.h"

class Map;
class Player;

class Weapon : public DestructibleItem {
 public:
  using DestructibleItem::DestructibleItem;

  Item::UseResult use(Map &map);

  virtual bool attack(Map &map, int dx, int dy) = 0;

 protected:
  virtual std::string name() const = 0;
  virtual const Color& color() const = 0;

 private:
  char glyph() const;
};

//axes cleave through enemies in an arc of three spaces
class Axe : public Weapon {
 public:
  CREATE_TAG_FOR(AXE)
  using DestructibleItem::DestructibleItem;

  bool attack(Map &map, int dx, int dy);
 protected:
  std::string name() const;
  const Color& color() const;
};

//bludgeons stun the target for a single turn
class Bludgeon : public Weapon {
 public:
  CREATE_TAG_FOR(Bludgeon)
  using DestructibleItem::DestructibleItem;

  bool attack(Map &map, int dx, int dy);

 protected:
  std::string name() const;
  const Color& color() const;
};

//lances get a free double attack if the player moved in the same direction
//last turn
class Lance : public Weapon {
 public:
  CREATE_TAG_FOR(Lance)
  using DestructibleItem::DestructibleItem;

  bool attack(Map &map, int dx, int dy);
 protected:
  std::string name() const;
  const Color& color() const;
};

//spears hit the space behind the target as well
class Spear : public Weapon {
 public:
  CREATE_TAG_FOR(Spear)
  using DestructibleItem::DestructibleItem;

  bool attack(Map &map, int dx, int dy);
 protected:
  std::string name() const;
  const Color& color() const;
};

#endif
