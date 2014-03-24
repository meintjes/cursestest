#include "Artifact.h"
#include "Color.h"
#include "functions.h"
#include "Map.h"
#include "Player.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT(Artifact)
BOOST_CLASS_EXPORT(HealingOrb)
BOOST_CLASS_EXPORT(TimeStopper)


Item::UseResult Artifact::use(Map &map) {
  if (map.you().getCurrentArtifact() == this) {
    if (map.you().addItem(this)) {
      return Item::Release;
    }
    else {
      return Item::Fail;
    }
  }
  else {
    map.you().setArtifact(this);
    return Item::Release;
  }
}

char Artifact::glyph() const {
  return 'O';
}



bool HealingOrb::evoke(Map &map) {
  if (map.you().heal(1)) {
    damage(15);
    return true;
  }
  else {
    return false;
  }
}

std::string HealingOrb::name() const {
  return "healing orb";
}

const Color& HealingOrb::color() const {
  return Red;
}



bool TimeStopper::evoke(Map &map) {
  map.you().stopTime(randRange(4, 6));
  damage(50);
  return true;
}

std::string TimeStopper::name() const {
  return "time stopper";
}

const Color& TimeStopper::color() const {
  return LightCyan;
}
