#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Artifact.h"
#include "Cch.h"
#include <memory>
#include <list>

struct Branch;
class Item;
class Map;
class Space;

class Player {
 public:
  Player();

  void display() const;

  bool tick();

  int getHp() const;
  int getLOS() const;
  Map* getCurrentFloor() const;
  Cch getGlyph() const;
  bool hasArrowMode() const;

  bool evokeArtifact();
  bool lightTorch();
  bool dropBomb();
  bool drawArrow(); //toggles arrowmode
  bool shootArrow(); //returns whether you had arrowmode, then unsets it
  bool quaffSpeedPotion();

  const Artifact* const getCurrentArtifact() const;
  void setArtifact(Artifact* const artifact);

  void damage(unsigned int num = 1);
  bool heal(unsigned int num);
  bool addItem(Item * const item);
  bool dropItem(Space &space);

  void stopTime(int num);
  void extinguishTorch();
  
  void setBranch(Branch *branch);
  bool changeDepth(int dz);

 private:
  static const int MAX_NUM_ITEMS = 23;
  std::list<std::unique_ptr<Item> > inventory;
  const std::list<std::unique_ptr<Item> >::iterator getInventoryInput();
  
  int hp;
  int hpMax;
  std::unique_ptr<Artifact> currentArtifact;

  Branch *currentBranch;
  int currentDepth;

  bool arrowMode;
  int torchDuration;
  int speedDuration;
  int freeMovesDuration;
};

#endif
