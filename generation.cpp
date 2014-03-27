#include "generation.h"
#include "functions.h"
#include "Item.h"
#include "Weapon.h"
#include "Artifact.h"
#include "Enemy.h"
#include <cassert>

//for loading from files. depending on the tag provided, provides a pointer
//pointing at an item of the correct type which can then be deserialized. 
SimpleItem* getSimpleItemPointerFromTag(std::string tag) {
  switch (tagize(tag.c_str())) {
  CREATE_CASE_FOR(Ore)
  CREATE_CASE_FOR(Bomb)
  CREATE_CASE_FOR(Torch)
  CREATE_CASE_FOR(Arrow)
  CREATE_CASE_FOR(Hook)

  CREATE_CASE_FOR(Axe)
  CREATE_CASE_FOR(Bludgeon)
  CREATE_CASE_FOR(Lance)
  CREATE_CASE_FOR(Spear)

  CREATE_CASE_FOR(HealingOrb)
  CREATE_CASE_FOR(TimeStopper)
  
  default:
    throw std::runtime_error("Invalid Item tag: " + tag);
    return nullptr;
  }
}

std::unique_ptr<SimpleItem> getRandomItem() {
  int num = randTo(99); 
  if (num < 20) {
    return std::unique_ptr<SimpleItem>(new Ore);
  }
  else if (num < 36) {
    return std::unique_ptr<SimpleItem>(new Bomb);
  }
  else if (num < 52) {
    return std::unique_ptr<SimpleItem>(new Torch);
  }
  else if (num < 68) {
    return std::unique_ptr<SimpleItem>(new Arrow);
  }
  else if (num < 84) {
    return std::unique_ptr<SimpleItem>(new Hook);
  }
  else if (num < 92) {
    return getRandomWeapon();
  }
  else {
    return getRandomArtifact();
  }
}

std::unique_ptr<SimpleItem> getRandomWeapon() {
  switch(randTo(3)) {
    case 0:
      return std::unique_ptr<SimpleItem>(new Axe);
    case 1:
      return std::unique_ptr<SimpleItem>(new Bludgeon);
    case 2:
      return std::unique_ptr<SimpleItem>(new Lance);
    case 3:
      return std::unique_ptr<SimpleItem>(new Spear);
    default:
      assert(false);
      return std::unique_ptr<SimpleItem>(new Axe);
  }
}

std::unique_ptr<SimpleItem> getRandomArtifact() {
  switch(randTo(1)) {
  case 0:
    return std::unique_ptr<SimpleItem>(new TimeStopper);
  case 1:
    return std::unique_ptr<SimpleItem>(new HealingOrb);
  default:
    assert(false);
    return std::unique_ptr<SimpleItem>(new TimeStopper);
  }
}

std::unique_ptr<Enemy> getRandomEnemy() {
  int num = randTo(99);
  if (num < 60)
    return std::unique_ptr<Enemy>(new Zombie);
  else if (num < 80)
    return std::unique_ptr<Enemy>(new Exploder);
  else if (num < 90)
    return std::unique_ptr<Enemy>(new Douser);
  else if (num < 99)
    return std::unique_ptr<Enemy>(new Reacher);
  else
    return std::unique_ptr<Enemy>(new SpawnerBoss);
}

