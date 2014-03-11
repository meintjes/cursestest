#include "generation.h"
#include "functions.h"
#include "Item.h"
#include "Weapon.h"
#include "Artifact.h"
#include <cassert>

std::unique_ptr<SimpleItem> getRandomItem() {
  int num = randTo(99); 
  if (num < 20) {
    return std::unique_ptr<SimpleItem>(new Bomb);
  }
  else if (num < 40) {
    return std::unique_ptr<SimpleItem>(new Torch);
  }
  else if (num < 60) {
    return std::unique_ptr<SimpleItem>(new Arrow);
  }
  else if (num < 75) {
    return std::unique_ptr<SimpleItem>(new Hook);
  }
  else if (num < 85) {
    return getRandomArtifact();
  }
  else {
    return getRandomWeapon();
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
