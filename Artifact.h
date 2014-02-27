#ifndef __ARTIFACT_H__
#define __ARTIFACT_H__

#include "Cst.h"
#include "Cch.h"
#include "Item.h"
#include <string>

class Map;
class Player;

class Artifact : public DestructibleItem {
 public:
  using DestructibleItem::DestructibleItem;
  
  Item::UseResult use(Map *map);

  virtual bool evoke(Map *map) = 0;

 protected:
  virtual std::string name() const = 0;
  virtual const Color& color() const = 0;

 private:
  char glyph() const;
};



class HealingOrb : public Artifact {
 public:
  using Artifact::Artifact;
  bool evoke(Map *map);
 private:
  std::string name() const;
  const Color& color() const;
};



class TimeStopper : public Artifact {
 public:
  using Artifact::Artifact;
  bool evoke(Map *map);
 private:
  std::string name() const;
  const Color& color() const;
};

#endif
