#ifndef __SPACE_H__
#define __SPACE_H__

#include "Item.h"
#include "Enemy.h"
#include "SpaceType.h"
#include <memory> 

class Player;
class Map;

class Space {
 public:
  Space();

  //change the SpaceType to the given one.
  void setType(const SpaceType &typeIn);
  
  //set the item on the space. overwrites the previous item.
  void setItem(SimpleItem * const itemIn);
  void setItem(std::unique_ptr<SimpleItem> itemIn);

  //if the space has an item, calls its pickup function. removes the
  //item if that function returns true; else, leaves it there.
  void pickup(Player &you);
  
  //moves the enemy from this space to the target space.
  //fails if the space is occupied.
  bool moveEnemy(Space *target);

  //sets the enemy on the space. overwrites the previous enemy.
  void setEnemy(Enemy *enemyIn);
  void setEnemy(std::unique_ptr<Enemy> enemyIn);
  
  //sets the space to have no enemy. doesn't call any functions that are
  //supposed to be called when the enemy dies; it just stops existing.
  void removeEnemy();

  //if there is an enemy, has that enemy attack the player. doesn't check
  //the range or anything; you should do that. the x and y are the coordinates
  //of THIS space, and the map is the map this space is on.
  void attack(Map &map, int x, int y);

  //damages the enemy on this space. doesn't remove it if it dies; that's
  //the enemy's responsibility.
  void damage(int num, Map &map, int x, int y);

  //tells the enemy where the player is. if the player goes out of view, the
  //enemy will try to follow them based on the given position.
  void renewMemory(Point playerPosition);

  //checks if the enemy has seen the player recently enough to follow them.
  bool hasMemory() const;

  //returns the location the enemy remembers the player being at.
  Point getMemory() const;

  int getRange() const; //returns the enemy's range.
  bool isStunned() const; //returns whether the enemy is stunned.
  void stun(unsigned int turns); //stuns the enemy.

  //adds poisonous gas to the space for the given duration.
  void addGas(unsigned int duration);
  
  //if there's a bomb on the space, returns false; otherwise, puts a bomb
  //on the space and returns true.
  bool dropBomb();
  
  //explodes the space. this damages any enemies present and, if the space's
  //type is destructible, replaces it with floor.
  void explode(Map &map, int x, int y);

  //lights the space for n turns (if called during the update loop; otherwise,
  //lights for n + 1 turns).
  void light(int turns);

  //updates the space and any enemies on it.
  bool tick();

  //figures out and returns what glyph to display for the space, based on a
  //number of factors (type, visibility, enemy, item, etc.).
  //note: sets the space as discovered the first time it's called. sorry
  Cch getGlyph(bool isVisible) const;

  //get information about the space's type.
  bool isPassable() const;
  bool isTransparent() const;

  //self-explanatory
  bool hasEnemy() const;
  bool hasGas() const;
  bool hasItem() const;
  bool isLit() const;

  //returns true if the space's type is the same as the given type.
  bool typeIs(const SpaceType &typeIn) const;

 private:
  mutable bool discovered;
  int lit;
  const SpaceType *type;
  unsigned int gasDuration;
  unsigned int bombDuration;
  std::unique_ptr<Enemy> enemy;
  std::unique_ptr<SimpleItem> item;
};

#endif
