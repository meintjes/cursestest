#ifndef __ITEM_H__
#define __ITEM_H__

#include <string>
#include <boost/serialization/base_object.hpp>

namespace boost {
  namespace serialization {
    class access;
  }
}

class Cch;
class Cst;
class Color;
class Player;
class Map;

//abstract class for items that exist on the ground but don't need names
//because they can't go into the player's inventory.
class SimpleItem {
 public:
  virtual ~SimpleItem();

  //figures out and returns the item's glyph. note that it's not virtual; you
  //don't want to override this, you want to define glyph() and color().
  Cch getGlyph() const;
  
  //return Release if the item's ownership is going to be transfered somewhere
  //else (e.g. the player's inventory), Destroy if the item is going into
  //the void, or Fail or None if the item can't be picked up for some reason.
  enum UseResult {Fail, None, Release, Destroy};
  virtual SimpleItem::UseResult pickup(Player &you) = 0;

 protected:
  //returns the basic (uncolored) glyph used to signify the item
  virtual char glyph() const = 0;
  //returns the color of the item
  virtual const Color& color() const = 0;

 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {}
};

//abstract class for items that have names and go into the player's inventory
class Item : public SimpleItem {
 public:
  //note: not virtual. don't override this, override name().
  Cst getName() const;
 
  //default behavior puts the item in the player's regular inventory as long
  //as it isn't full, then returns Release if that succeeded and Fail if not
  virtual Item::UseResult pickup(Player &you);
  
  //Item::use executes when the player uses the item from their inventory.
  //the UseResult indicates what should happen as a result of the item's use:
  //Fail means don't spend a turn, None means spend a turn but don't do
  //anything to the item, Release indicates the player should relinquish
  //ownership of the item (for example, so it can be moved from the inventory
  //to the current weapon slot), and Destroy indicates that the item should
  //be destroyed and forgotten.
  virtual Item::UseResult use(Map &map) = 0;

 protected:
  //returns the basic (uncolored) name of the item
  virtual std::string name() const = 0;

 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<SimpleItem>(*this);
  }
};



//base class for artifacts and weapons
class DestructibleItem : public Item {
 public:
  DestructibleItem();

  //returns an adjective describing the current state of the item: "OK",
  //"damaged" etc.
  Cst getDescriptor() const;

  //returns true if the item should be destroyed, or false if not
  bool shouldDestroy() const;
  
  //deducts x from the item's durability.
  void damage(unsigned int x);

 protected:
  virtual char glyph() const = 0;
  virtual std::string name() const = 0;
  virtual const Color& color() const = 0;

 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<Item>(*this);
    ar & durability;
    ar & durabilityMin;
  }

  int durability;
  int durabilityMin;
};



class Ore : public SimpleItem {
 public:
  using SimpleItem::SimpleItem;
  SimpleItem::UseResult pickup(Player &you);
 protected:
  char glyph() const;
  const Color& color() const;
 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<SimpleItem>(*this);
  }
};



class Bomb : public Item {
 public:
  using Item::Item;
  Item::UseResult use(Map &map);
 protected:
  char glyph() const;
  std::string name() const;
  const Color& color() const;
 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<Item>(*this);
  }
};



class Torch : public Item {
 public:
  using Item::Item;
  Item::UseResult use(Map &map);
 protected:
  char glyph() const;
  std::string name() const;
  const Color& color() const;
 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<Item>(*this);
  }
};



class Arrow : public Item {
 public:
  using Item::Item;
  Item::UseResult use(Map &map);
 protected:
  char glyph() const;
  std::string name() const;
  const Color& color() const; 
 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<Item>(*this);
  }
};



class Hook : public DestructibleItem {
 public:
  using DestructibleItem::DestructibleItem;
  Item::UseResult use(Map &map);
 protected:
  char glyph() const;
  std::string name() const;
  const Color& color() const;
 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<DestructibleItem>(*this);
  }
};

#endif
