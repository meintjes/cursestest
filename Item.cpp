#include "Item.h"
#include "Player.h"
#include "Artifact.h"
#include "Cch.h"
#include "Cst.h"
#include "Map.h"
#include "functions.h"

std::unique_ptr<SimpleItem> getRandomItem() {
  /*
  int num = randTo(99); 
  if (num < 20) {
    return std::unique_ptr<SimpleItem>(new Bomb);
  }
  else if (num < 40) {
    return std::unique_ptr<SimpleItem>(new Torch);
  }
  else if (num < 60) {
    return std::unique_ptr<SimpleItem>(new Arrow);
  }
  else if (num < 75) {
    return std::unique_ptr<SimpleItem>(new Hook);
  }
  else if (num < 85) {
    return std::unique_ptr<SimpleItem>(new HealingOrb);
  }
  else if (num < 95) {
    return std::unique_ptr<SimpleItem>(new TimeStopper);
  }
  else {
    return std::unique_ptr<SimpleItem>(new Bludgeon);
  }
  */
  return std::unique_ptr<SimpleItem>(new Bludgeon);
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
  return you.addItem(this, true);
}



DestructibleItem::DestructibleItem() :
  Item(),
  durability(100),
  durabilityMin(randRange(-25, 25))
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
  map->you.setTorchDuration(randRange(125, 175));
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
  if (map->you.getMode() == Player::Mode::Arrow) {
    map->you.setMode(Player::Mode::Move);
    return Item::None;
  }
  else {
    map->you.setMode(Player::Mode::Arrow);
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



Item::UseResult Hook::use(Map *map) {
  if (map->you.getMode() == Player::Mode::Hook) {
    map->you.setMode(Player::Mode::Move);
    return Item::None;
  }
  else {
    map->you.setMode(Player::Mode::Hook);
    return Item::None;
  }
}

char Hook::glyph() const {
  return '&';
}

std::string Hook::name() const {
  return "hook";
}

const Color& Hook::color() const {
  return LightGray;
}
