#include "Weapon.h"
#include "Player.h"
#include "Map.h"

Item::UseResult Weapon::use(Map *map) {
  map->you.setWeapon(this);
  return Item::Release;
} 

bool Weapon::attack(Map *map, int dx, int dy) {
  return true;
}

char Weapon::glyph() const {
  return ')';
}
