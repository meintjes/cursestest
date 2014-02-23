#ifndef __ITEM_H__
#define __ITEM_H__

#include <memory>

class Cch;
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

class Health : public Item {
  Cch getGlyph() const;
  bool pickup(Player &you);
};

std::unique_ptr<Item> getRandomItem();

#endif
