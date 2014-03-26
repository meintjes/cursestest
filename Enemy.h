#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Archive.h"
#include "Point.h"
#include <memory>
#include <string>

class Cch;
class Map;

class Enemy {
 public:
  //for loading from files. depending on the tag provided, provides an Enemy*
  //pointing at an enemy of the correct type which can then be deserialized. 
  static Enemy* getPointerFromTag(std::string tag);

  //generated automatically by macros. don't manually override.
  virtual std::string getSerializationTag() const = 0;

  //serialization function. enemies that have additional state need to
  //override this. probably call the base class version from the override.
  virtual void serialize(Archive &archive);



  Enemy(int hpIn = 1);

  //returns the glyph for the class. must override.
  virtual Cch getGlyph() const = 0;

  //returns the maximum distance at which the enemy will attack the player
  //rather than move toward them. default value is 1, usually don't override
  virtual int getRange() const;

  //what happens when the enemy attacks. default behavior damages the player
  //by 1. probably should override.
  virtual void attack(Map &map, int x, int y);

  //subtracts hp from the enemy. you usually won't want to override this.
  virtual void damage(int num, Map &map, int x, int y);
  
  virtual ~Enemy();

  void renewMemory(Point playerLocation);
  bool hasMemory() const;
  Point getMemory() const;
  bool isStunned() const;
  void stun(unsigned int turns);

  //decrements timers for various durations. can be overridden for various
  //other reasons, but should call the base class version so that stunning
  //doesn't permanently incapacitate the enemy or anything like that.
  virtual void tick(Map &map, int x, int y);

 protected:
  //what happens when the enemy dies. default behavior just removes it from
  //the map. overrides should probably do that too.
  virtual void die(Map &map, int x, int y);
  int hp;

 private:
  int memoryDuration;
  Point memoryLocation;
  int stunDuration;
};



class Zombie : public Enemy {
 public:
  CREATE_TAG_FOR(Zombie)
  Zombie();
  Cch getGlyph() const;
};

class Exploder : public Enemy {
 public:
  CREATE_TAG_FOR(Exploder)
  Exploder();
  void serialize(Archive &ar);
  Cch getGlyph() const;
  int getRange() const;
  void attack(Map &map, int x, int y);
  void die(Map &map, int x, int y);
 private:
  bool isPrimed;
};

class Reacher : public Enemy {
  CREATE_TAG_FOR(Reacher)
 public:
  using Enemy::Enemy;
  Cch getGlyph() const;
  int getRange() const;
};

class SpawnerBoss : public Enemy {
 public:
  CREATE_TAG_FOR(SpawnerBoss)
  SpawnerBoss();
  Cch getGlyph() const;
  int getRange() const;
  void attack(Map &map, int x, int y);
  void die(Map &map, int x, int y);
};

class Douser : public Enemy {
 public:
  CREATE_TAG_FOR(Douser)
  using Enemy::Enemy;
  Cch getGlyph() const;
  void attack(Map &map, int x, int y);
};

#endif
