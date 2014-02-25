#include "Item.h"
#include "Player.h"
#include "Cch.h"
#include "Cst.h"
#include "functions.h"

std::unique_ptr<Item> getRandomItem() {
  return std::unique_ptr<Item>(new TimeStopper);
}

Cst Item::getName() const {
  return color()(name());
}

Cch Item::getGlyph() const {
  return color()(glyph());
}

Item::~Item() {

}

bool Item::pickup(Player &you) {
  return you.addItem(this);
}



DestructibleItem::DestructibleItem() :
  durability(100),
  durabilityMin(randRange(-10, 10))
{}

Cst DestructibleItem::getDescriptor() const {
  if (durability > 50)
    return LightGray("OK");
  else if (durability > 25)
    return Yellow("damaged");
  else
    return Orange("cracked");
}

void DestructibleItem::damage(unsigned int x) {
  durability -= x;
}

bool DestructibleItem::shouldDestroy() const {
  return (durability <= durabilityMin);
}
