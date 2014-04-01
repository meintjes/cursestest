#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Cch.h"
#include "Point.h"
#include <memory>
#include <list>

class Archive;
class Branch;
class Item;
class Weapon;
class Artifact;
class Map;
class Space;

class Player {
 public:
  enum class Mode {
    Move,
    Run,
    Arrow,
    Hook
  };

  void serialize(Archive &ar);

  Player();
  ~Player(); //necessary for unique_ptr to incomplete type

  //display hud, including the health/stamina bars and the bottom pane
  void display() const;

  //update effect durations, etc. when time passes. returns a value
  //corresponding to how much time should pass: 8 is the norm, lower numbers
  //are 'faster' actions, and higher numbers are 'slower' actions
  unsigned int tick();

  //formats the player's hard drive and frames them for murder
  int getHp() const;

  //returns the current max extent of the player's line of sight
  int getLOS() const;

  //returns a reference to the player's current floor
  Map& getCurrentFloor() const;

  //figures out and returns the player's glyph, based on current mode
  //and free moves
  Cch getGlyph() const;

  //uses the current weapon to attack the space in the given direction.
  //destroys the weapon if it needs to be destroyed. if the player doesn't
  //have a weapon, just does an unarmed attack.
  bool attack(int dx, int dy);

  //evokes the player's current artifact. destroys it if it needs destroyed.
  bool evokeArtifact();

  //get/set the player's current mode (see Player::Mode).
  Mode getMode() const;
  void setMode(Mode modeIn);

  //switches between running/not running (but does nothing if the player
  //has a mode item out)
  void toggleRun();

  //destroy the item that caused the player to enter their current mode
  void destroyModeItem();

  //set/get the direction of the player's last move. if the player doesn't
  //move for a turn (if setLastMoveDirection isn't called), the last moved
  //direction will change to 0, 0. 
  void setLastMoveDirection(const Point &direction);
  Point getLastMoveDirection() const;

  //set and get pointers to current weapon and artifact. DO NOT save the
  //gotten pointers anywhere; ownership is retained by the player.
  const Weapon* const getCurrentWeapon() const;
  void setWeapon(Weapon* const weapon);
  const Artifact* const getCurrentArtifact() const;
  void setArtifact(Artifact* const artifact);

  //damage the player for num damage, resets the internal damage timer.
  void damage(unsigned int num);

  //heals the player for num health / restores num stamina (or max hp/max
  //stamina if lower). returns false if player is already at 100%.
  bool heal(unsigned int num);
  bool restoreStamina(int num);

  //like heal, but only restores up to the current maximum and only if
  //the player hasn't taken damage recently
  bool weakHeal(unsigned int num);

  //deducts num stamina from the player. if the player doesn't have num
  //stamina, instead does nothing and returns false.
  bool removeStamina(int num);

  //return whether hp/stamina is "high" according to a universal standard
  bool hpIsHigh() const;
  bool staminaIsHigh() const;

  //attempts to add the item to the player's inventory. returns false if
  //if the player's inventory is full.
  bool addItem(Item * const item);

  void addOre(int num); //give the player ore.

  //deduct num ore from the player. if the player doesn't have enough, does
  //nothing and returns false.
  bool removeOre(int num);

  //prompts the player for an item to use, then uses it. returns false if
  //the item can't be used for some reason.
  bool useItem();

  //prompts the player for an item to drop, then drops it. returns false if
  //the item can't be dropped (for example, if they're standing on an item).
  bool dropItem();

  //stops time for num turns, meaning the player will be able to act without
  //anything else reacting.
  void stopTime(int num);

  //sets the duration of the player's torch to num turns, overwriting the
  //previous value.
  void setTorchDuration(int num);

  //get the current branch's address
  Branch* getBranch() const;

  //sets the player's current dungeon branch to the given branch.
  void setBranch(Branch *branch);

  //changes the player's depth. if the change would make the player go off
  //the bottom of the branch, does nothing and returns false. if it would make
  //them go above the beginning, it puts them into the parent branch.
  bool changeDepth(int dz);

 private:
  static const int MAX_NUM_ITEMS = 23;
  
  struct InventoryInputResult {
    enum Type {Inventory, CurrentArtifact, CurrentWeapon};
    Type type;
    std::list<std::unique_ptr<Item> >::iterator item;
  };
  Player::InventoryInputResult getInventoryInput();

  //adds an item with no bounds checking
  void addItemUnsafe(Item * const item);

  bool restoreAttribute(int &att, int &attMax, int num);

  std::list<std::unique_ptr<Item> > inventory;
  int ore;
  
  int hpMax;
  int currentHpMax;
  int hp;
  int staminaMax;
  int stamina;
  
  std::unique_ptr<Weapon> currentWeapon;
  std::unique_ptr<Artifact> currentArtifact;
  Mode mode;
  std::list<std::unique_ptr<Item> >::iterator modeItemIterator;
  Point lastMoveDirection;
  bool movedLastTurn; //don't use this, it's set false every turn by tick().
                      //instead compare lastMoveDirection to {0, 0}
  int damageTimer;

  Branch *currentBranch;
  int currentDepth;

  int torchDuration;
  int speedDuration;
  int freeMovesDuration;
};

#endif
