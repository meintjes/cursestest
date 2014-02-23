#include "Item.h"
#include "Player.h"
#include "Cch.h"

std::unique_ptr<Item> getRandomItem() {
  return std::unique_ptr<Item>(new Health);
}

bool Item::destroyedOnPickup() const {
  return true;
}

Item::~Item() {

}

Cch Health::getGlyph() const {
  return Red('+');
}

bool Health::pickup(Player &you) {
  return you.heal(5);
}
