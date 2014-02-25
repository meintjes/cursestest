#ifndef __ITEM_H__
#define __ITEM_H__

#include <memory>

class Cch;
class Cst;
class Color;
class Player;

//abstract class for items that exist on the ground but don't need names
//because they can't go into the player's inventory.
class SimpleItem {
 public:
  Cch getGlyph() const;
  virtual ~SimpleItem();
  virtual bool pickup(Player &you) = 0;
 protected:
  virtual char glyph() const = 0;
  virtual const Color& color() const = 0;
};

//abstract class for items that have names and go into the player's inventory
class Item : public SimpleItem {
 public:
  Cst getName() const;

  //return true if the item should be picked up, or false if not (e.g., if
  //the player can't use the thing or has a full inventory or something).
  //default behavior puts the item in the player's regular inventory as long
  //as it isn't full.
  virtual bool pickup(Player &you);

 protected:
  virtual std::string name() const = 0;
};

std::unique_ptr<SimpleItem> getRandomItem();

//base class for artifacts and weapons
class DestructibleItem : public Item {
 public:
  DestructibleItem();
  Cst getDescriptor() const;
  void damage(unsigned int x);
  bool shouldDestroy() const;
 
  virtual bool pickup(Player &you) = 0;

 protected:
  virtual char glyph() const = 0;
  virtual std::string name() const = 0;
  virtual const Color& color() const = 0;

 private:
  int durability;
  const int durabilityMin;
};

#endif
