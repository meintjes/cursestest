#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Cch.h"
#include "Point.h"
#include <memory>
#include <list>

struct Branch;
class Item;
class Weapon;
class Artifact;
class Map;
class Space;

class Player {
 public:
  enum class Mode {
    Move,
    Arrow,
    Hook
  };

  Player();
  ~Player(); //necessary for unique_ptr to incomplete type

  void display() const;

  bool tick();

  int getHp() const;
  int getLOS() const;
  Map& getCurrentFloor() const;
  Cch getGlyph() const;

  bool attack(int dx, int dy);
  bool evokeArtifact();
  Mode getMode() const;
  void setMode(Mode modeIn);
  void setLastMoveDirection(const Point &direction);
  Point getLastMoveDirection() const;

  const Weapon* const getCurrentWeapon() const;
  void setWeapon(Weapon* const weapon);
  const Artifact* const getCurrentArtifact() const;
  void setArtifact(Artifact* const artifact);

  void damage(unsigned int num);
  bool heal(unsigned int num);
  bool removeStamina(int num);
  bool restoreStamina(int num);
  bool addItem(Item * const item, bool checkMaxItems);
  void addOre(int num);
  bool removeOre(int num);
  bool useItem(Map &map);
  bool dropItem(Space &space);

  void stopTime(int num);
  void setTorchDuration(int num);
  
  void setBranch(Branch *branch);
  bool changeDepth(int dz);

 private:
  static const int MAX_NUM_ITEMS = 23;
  std::list<std::unique_ptr<Item> > inventory;
  int ore;

  struct InventoryInputResult {
    enum Type {Inventory, CurrentArtifact, CurrentWeapon};
    Type type;
    std::list<std::unique_ptr<Item> >::iterator item;
  };
  Player::InventoryInputResult getInventoryInput();
  
  bool restoreAttribute(int &att, int &attMax, int num);

  int hpMax;
  int hp;
  int staminaMax;
  int stamina;
  std::unique_ptr<Weapon> currentWeapon;
  std::unique_ptr<Artifact> currentArtifact;
  Mode mode;
  Point lastMoveDirection;
  bool movedLastTurn;

  Branch *currentBranch;
  int currentDepth;

  int torchDuration;
  int speedDuration;
  int freeMovesDuration;
};

#endif
