#ifndef __ARTIFACT_H__
#define __ARTIFACT_H__

#include "Cst.h"
#include "Cch.h"
#include "Item.h"
#include <string>

class Map;
class Player;

class Artifact : public Item {
 public:
  Artifact();

  //inherited from Item, do not override:
  bool destroyedOnPickup() const;
  Cch getGlyph() const;
  bool pickup(Player &you);

  Cst getName() const;
  Cst getDescriptor() const;
  void damage(unsigned int x);
  bool shouldDestroy();

  virtual bool use(Map *map) = 0;

 private:
  int durability;

  virtual std::string name() const = 0;
  virtual const Color& color() const = 0;
};



class HealingOrb : public Artifact {
 public:
  using Artifact::Artifact;
  bool use(Map *map);
 private:
  std::string name() const;
  const Color& color() const;
};



class TimeStopper : public Artifact {
 public:
  using Artifact::Artifact;
  bool use(Map *map);
 private:
  std::string name() const;
  const Color& color() const;
};

#endif
