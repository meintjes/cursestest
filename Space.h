#ifndef __SPACE_H__
#define __SPACE_H__

#include <memory> 
#include "Point.h"
#include "Cch.h"

class Archive;
class Player;
class Enemy;
class Map;
class SimpleItem;

class Space {
 public:
  void serialize(Archive &ar);
   
  Space();
  ~Space();
  
  enum Type {Wall, GlassWall, Floor, StairsUp, StairsDown};

  //change the Space's Type to the given one.
  void setType(Space::Type typeIn);
  
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

  //calls the enemy's act function until it doesn't have the time to act
  //anymore. if the enemy moves, calls its act function with the new space.
  bool act(Map &map, int x, int y);

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

  bool isStunned() const; //returns whether the enemy is stunned.
  void stun(unsigned int duration); //prevents the enemy from acting

  //adds poisonous gas to the space for the given duration.
  void addGas(unsigned int duration);
  
  //if there's a bomb on the space, returns false; otherwise, puts a bomb
  //on the space and returns true.
  bool dropBomb();
  
  //explodes the space. this damages any enemies present and, if the space's
  //type is destructible, replaces it with floor.
  void explode(Map &map, int x, int y);

  //updates the space and any enemies on it. returns true if a bomb went off.
  bool tick(unsigned int duration, Map &map, int x, int y);

  //if the space has an enemy, gives it (duration) time to act
  void addTimeToAct(unsigned int duration);

  //figures out and returns what glyph to display for the space, based on a
  //number of factors (type, visibility, enemy, item, etc.).
  //note: sets the space as discovered the first time it's called. sorry
  Cch getGlyph(bool isVisible) const;

  //returns the glyph of the space's type
  Cch getTypeGlyph() const;

  //get information about the space's type.
  bool isPassable() const;
  bool isTransparent() const;
  bool isDestructible() const;

  //self-explanatory
  bool hasEnemy() const;
  bool hasGas() const;
  bool hasItem() const;

  //returns true if the space's type is the same as the given type.
  bool typeIs(Space::Type typeIn) const;

 private:
  mutable bool discovered;
  Space::Type type;
  int gasDuration;
  int bombDuration;
  std::unique_ptr<Enemy> enemy;
  std::unique_ptr<SimpleItem> item;
};

#endif
