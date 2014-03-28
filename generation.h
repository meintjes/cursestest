#ifndef __GENERATION_H__
#define __GENERATION_H__

#include "Archive.h"
#include <memory>
class SimpleItem;

//helper function for the below.
void* getPointerFromArchive(Archive &ar);

template <typename T>
void serializeUnique(std::unique_ptr<T> &ptr, Archive &ar) {
  if (ar.getType() == Archive::Save) {  
    if (ptr) {
      ar << ptr->getSerializationTag();
      ptr->serialize(ar);
    }
    else {
      ar << "_";
    }
  }
  else {
    ptr.reset(static_cast<T*>(getPointerFromArchive(ar)));
    if (ptr) {
      ptr->serialize(ar);
    }
  }
}

std::unique_ptr<SimpleItem> getRandomItem();
std::unique_ptr<SimpleItem> getRandomArtifact();
std::unique_ptr<SimpleItem> getRandomWeapon();

class Enemy;
std::unique_ptr<Enemy> getRandomEnemy();

#endif
