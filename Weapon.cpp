#include "Weapon.h"
#include "Player.h"
#include "Map.h"

Item::UseResult Weapon::use(Map *map) {
  if (map->you.getCurrentWeapon() == this) {
    if (map->you.addItem(this, true)) {
      return Item::Release;
    }
    else {
      return Item::Fail;
    }
  }
  else {
    map->you.setWeapon(this);
    return Item::Release;
  }
} 

char Weapon::glyph() const {
  return ')';
}



void Bludgeon::attack(Map *map, int dx, int dy) {
  int x = map->getPlayerX() + dx;
  int y = map->getPlayerY() + dy;
  map->getSpace(x, y).kill(*map, x, y);
  map->getSpace(x, y).stun(1);
}

std::string Bludgeon::name() const {
  return "bludgeon";
}

const Color& Bludgeon::color() const {
  return Brown;
}
