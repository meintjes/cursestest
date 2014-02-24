#ifndef __ITEM_H__
#define __ITEM_H__

#include <memory>

class Cch;
class Cst;
class Color;
class Player;

class Item {
 public:
  virtual Cch getGlyph() const = 0;
  virtual ~Item();

  //return true if the item should be picked up, or false if not (e.g., if
  //the player can't use the thing or has a full inventory or something).
  virtual bool pickup(Player &you) = 0;

  //default version returns true. if overridden with 'return false,' the item
  //will be release()'d when picked up, rather than destroyed. a new smart
  //pointer should then be initialized so the thing doesn't get leaked.
  virtual bool destroyedOnPickup() const;
};

std::unique_ptr<Item> getRandomItem();

//base class for artifacts and weapons
class DestructibleItem : public Item {
 public:
  DestructibleItem();
  Cst getName() const;
  Cch getGlyph() const;
  Cst getDescriptor() const;
  bool destroyedOnPickup() const;
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

class Health : public Item {
  Cch getGlyph() const;
  bool pickup(Player &you);
};


#endif
