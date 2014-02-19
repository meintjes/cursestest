#ifndef __ARTIFACT_H__
#define __ARTIFACT_H__

#include "Cst.h"
#include "Cch.h"
#include <string>

class Map;

class Artifact {
 public:
  Artifact();
  virtual ~Artifact();

  Cch getGlyph() const;
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
