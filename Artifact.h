#ifndef __ARTIFACT_H__
#define __ARTIFACT_H__

#include "Cst.h"
#include "Cch.h"
#include "Item.h"
#include <string>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

namespace boost {
  namespace serialization {
    class access;
  }
}

class Map;
class Player;

class Artifact : public DestructibleItem {
 public:
  using DestructibleItem::DestructibleItem;
  
  Item::UseResult use(Map &map);

  virtual bool evoke(Map &map) = 0;

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
BOOST_CLASS_EXPORT(Artifact)



class HealingOrb : public Artifact {
 public:
  using Artifact::Artifact;
  bool evoke(Map &map);
 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<Artifact>(*this);
  }

  std::string name() const;
  const Color& color() const;
};
BOOST_CLASS_EXPORT(HealingOrb)



class TimeStopper : public Artifact {
 public:
  using Artifact::Artifact;
  bool evoke(Map &map);
 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<Artifact>(*this);
  }

  std::string name() const;
  const Color& color() const;
};
BOOST_CLASS_EXPORT(TimeStopper)

#endif
