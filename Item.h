#ifndef __ITEM_H__
#define __ITEM_H__

#include <memory>

class Cch;
class Cst;
class Color;
class Player;

class Item {
 public:
  Cst getName() const;
  Cch getGlyph() const;
  virtual ~Item();

  //return true if the item should be picked up, or false if not (e.g., if
  //the player can't use the thing or has a full inventory or something).
  virtual bool pickup(Player &you) = 0;

 protected:
  virtual char glyph() const = 0;
  virtual std::string name() const = 0;
  virtual const Color& color() const = 0;
};

std::unique_ptr<Item> getRandomItem();

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
