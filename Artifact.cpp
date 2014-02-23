#include "Artifact.h"
#include "functions.h"
#include "Map.h"
#include "Player.h"

Artifact::Artifact() : durability(100) {}

bool Artifact::destroyedOnPickup() const {
  return false;
}

bool Artifact::pickup(Player &you) {
  if (!you.hasArtifact()) {
    you.setArtifact(std::unique_ptr<Artifact>(this));
    return true;
  }
  else {
    return false;
  }
}

Cst Artifact::getName() const {
  return color()(name());
}

Cch Artifact::getGlyph() const {
  return color()('O');
}

Cst Artifact::getDescriptor() const {
  if (durability > 50)
    return LightGray("OK");
  else if (durability > 25)
    return Yellow("damaged");
  else
    return Orange("cracked");
}

void Artifact::damage(unsigned int x) {
  durability -= x;
}

bool Artifact::shouldDestroy() {
  return (durability <= -10 + randTo(20));
}



bool HealingOrb::use(Map *map) {
  if (map->you.heal(1)) {
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



bool TimeStopper::use(Map *map) {
  map->you.stopTime(randRange(4, 6));
  damage(50);
  return true;
}

std::string TimeStopper::name() const {
  return "time stopper";
}

const Color& TimeStopper::color() const {
  return LightCyan;
}
