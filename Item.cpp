#include "Item.h"
#include "Player.h"
#include "Cch.h"
#include "Cst.h"
#include "functions.h"

std::unique_ptr<Item> getRandomItem() {
  return std::unique_ptr<Item>(new Health);
}

bool Item::destroyedOnPickup() const {
  return true;
}

Item::~Item() {

}



DestructibleItem::DestructibleItem() :
  durability(100),
  durabilityMin(randRange(-10, 10))
{}

Cst DestructibleItem::getName() const {
  return color()(name());
}

Cch DestructibleItem::getGlyph() const {
  return color()(glyph());
}

Cst DestructibleItem::getDescriptor() const {
  if (durability > 50)
    return LightGray("OK");
  else if (durability > 25)
    return Yellow("damaged");
  else
    return Orange("cracked");
}

bool DestructibleItem::destroyedOnPickup() const {
  return false; //released on pickup, not destroyed;
                //pickup function MUST CREATE A SMART POINTER
}

void DestructibleItem::damage(unsigned int x) {
  durability -= x;
}

bool DestructibleItem::shouldDestroy() const {
  return (durability <= durabilityMin);
}



Cch Health::getGlyph() const {
  return Red('+');
}

bool Health::pickup(Player &you) {
  return you.heal(5);
}
