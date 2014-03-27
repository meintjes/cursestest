#ifndef __GENERATION_H__
#define __GENERATION_H__

#include <memory>
class SimpleItem;
class Archive;

//helper function for the below.
SimpleItem* getSimpleItemPointerFromArchive(Archive &ar);

//for loading from files. reads a tag from the archive and provides a pointer
//pointing at an item of the correct type which can then be deserialized. 
template <typename T>
T* getItemPointerFromArchive(Archive &ar) {
  return dynamic_cast<T*>(getSimpleItemPointerFromArchive(ar));
}

std::unique_ptr<SimpleItem> getRandomItem();
std::unique_ptr<SimpleItem> getRandomArtifact();
std::unique_ptr<SimpleItem> getRandomWeapon();

class Enemy;
std::unique_ptr<Enemy> getRandomEnemy();

#endif
