#include "Weapon.h"
#include "Player.h"
#include "Map.h"

Item::UseResult Weapon::use(Map *map) {
  if (map->you.getCurrentWeapon() == this) {
    map->you.addItem(this);
  }
  else {
    map->you.setWeapon(this);
  }
  return Item::Release;
} 

bool Weapon::attack(Map *map, int dx, int dy) {
  return true;
}

char Weapon::glyph() const {
  return ')';
}
