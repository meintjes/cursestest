#include "Item.h"
#include "Player.h"
#include "Artifact.h"
#include "Cch.h"
#include "Cst.h"
#include "Map.h"
#include "functions.h"

std::unique_ptr<SimpleItem> getRandomItem() {
  return std::unique_ptr<SimpleItem>(new Bomb);
}



Cch SimpleItem::getGlyph() const {
  return color()(glyph());
}

SimpleItem::~SimpleItem() {

}



Cst Item::getName() const {
  return color()(name());
}

bool Item::pickup(Player &you) {
  return you.addItem(this);
}



DestructibleItem::DestructibleItem() :
  Item(),
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



Item::UseResult Bomb::use(Map *map) {
  if (map->dropBomb()) {
    return Item::Destroy;
  }
  else {
    return Item::Fail;
  }
}

char Bomb::glyph() const {
  return '!';
}

std::string Bomb::name() const {
  return "bomb";
}

const Color& Bomb::color() const {
  return Red;
}



Item::UseResult Torch::use(Map *map) {
  //TODO: make things happen
  return Item::Destroy;
}

char Torch::glyph() const {
  return '^';
}

std::string Torch::name() const {
  return "torch";
}

const Color& Torch::color() const {
  return Yellow;
}



Item::UseResult Arrow::use(Map *map) {
  if (map->you.hasArrowMode()) {
    //TODO: make things happen
    return Item::Destroy;
  }
  else {
    return Item::None;
  }
}

char Arrow::glyph() const {
  return '|';
}

std::string Arrow::name() const {
  return "arrow";
}

const Color& Arrow::color() const {
  return Brown;
}
