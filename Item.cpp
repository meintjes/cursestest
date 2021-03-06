#include "Item.h"
#include "Player.h"
#include "Color.h"
#include "Map.h"
#include "functions.h"
#include <stdexcept>

void SimpleItem::serialize(Archive &) {
  //no data to serialize.
}

void DestructibleItem::serialize(Archive &ar) {
  ar & durability;
  ar & durabilityMin;
}



Cch SimpleItem::getGlyph() const {
  return color()(glyph());
}

SimpleItem::~SimpleItem() {

}



Cst Item::getName() const {
  return color()(name());
}

Item::UseResult Item::pickup(Player &you) {
  if (you.addItem(this)) {
    return Item::Release;
  }
  else {
    return Item::Fail;
  }
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

bool DestructibleItem::shouldDestroy() const {
  return (durability <= durabilityMin);
}

void DestructibleItem::damage(unsigned int x) {
  durability -= x;
}



SimpleItem::UseResult Ore::pickup(Player &you) {
  you.addOre(1);
  return SimpleItem::Destroy;
}

char Ore::glyph() const {
  return '$';
}

const Color& Ore::color() const {
  return Brown;
}



Item::UseResult Bomb::use(Map &map) {
  if (map(map.getPlayerX(), map.getPlayerY()).dropBomb()) {
    return Item::Destroy;
  }
  else {
    return Item::Fail;
  }
}

char Bomb::glyph() const {
  return '*';
}

std::string Bomb::name() const {
  return "bomb";
}

const Color& Bomb::color() const {
  return Red;
}



Item::UseResult Torch::use(Map &map) {
  map.you.setTorchDuration(randRange(1000, 1400));
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



Item::UseResult Arrow::use(Map &map) {
  if (map.you.getMode() == Player::Mode::Arrow) {
    map.you.setMode(Player::Mode::Move);
    return Item::None;
  }
  else {
    map.you.setMode(Player::Mode::Arrow);
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



Item::UseResult Hook::use(Map &map) {
  if (map.you.getMode() == Player::Mode::Hook) {
    map.you.setMode(Player::Mode::Move);
    return Item::None;
  }
  else {
    map.you.setMode(Player::Mode::Hook);
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



Item::UseResult HealthPotion::use(Map &map) {
  if (map.you.heal(randRange(8, 12))) {
    return Item::Destroy;
  }
  else {
    return Item::Fail;
  }
}

char HealthPotion::glyph() const {
  return '!';
}

std::string HealthPotion::name() const {
  return "potion of healing";
}

const Color& HealthPotion::color() const {
  return DarkGreen;
}



Item::UseResult StaminaPotion::use(Map &map) {
  if (map.you.restoreStamina(randRange(4, 6))) {
    return Item::Destroy;
  }
  else {
    return Item::Fail;
  }
}

char StaminaPotion::glyph() const {
  return '!';
}

std::string StaminaPotion::name() const {
  return "potion of staminaj";
}

const Color& StaminaPotion::color() const {
  return Brown;
}
