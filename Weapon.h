#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "Item.h"
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

namespace boost {
  namespace serialization {
    class access;
  }
}

class Map;
class Player;

class Weapon : public DestructibleItem {
 public:
  using DestructibleItem::DestructibleItem;

  Item::UseResult use(Map &map);

  virtual bool attack(Map &map, int dx, int dy) = 0;

 protected:
  virtual std::string name() const = 0;
  virtual const Color& color() const = 0;

 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<DestructibleItem>(*this);
  }

  char glyph() const;
};
BOOST_CLASS_EXPORT(Weapon)

//axes cleave through enemies in an arc of three spaces
class Axe : public Weapon {
 public:
  using DestructibleItem::DestructibleItem;

  bool attack(Map &map, int dx, int dy);
 protected:
  std::string name() const;
  const Color& color() const;

 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<Weapon>(*this);
  }
};
BOOST_CLASS_EXPORT(Axe)

//bludgeons stun the target for a single turn
class Bludgeon : public Weapon {
 public:
  using DestructibleItem::DestructibleItem;

  bool attack(Map &map, int dx, int dy);

 protected:
  std::string name() const;
  const Color& color() const;
 
 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<Weapon>(*this);
  }
};
BOOST_CLASS_EXPORT(Bludgeon)

//lances get a free double attack if the player moved in the same direction
//last turn
class Lance : public Weapon {
 public:
  using DestructibleItem::DestructibleItem;

  bool attack(Map &map, int dx, int dy);
 protected:
  std::string name() const;
  const Color& color() const;
 
 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<Weapon>(*this);
  }
};
BOOST_CLASS_EXPORT(Lance)

//spears hit the space behind the target as well
class Spear : public Weapon {
 public:
  using DestructibleItem::DestructibleItem;

  bool attack(Map &map, int dx, int dy);
 protected:
  std::string name() const;
  const Color& color() const;
 
 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<Weapon>(*this);
  }
};
BOOST_CLASS_EXPORT(Spear)

#endif
