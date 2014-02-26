#include "Weapon.h"

bool Weapon::pickup(Player &you) {
  return true;
}

bool Weapon::attack(Map *map, int dx, int dy) {
  return true;
}

char Weapon::glyph() const {
  return ')';
}
