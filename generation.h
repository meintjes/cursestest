#ifndef __GENERATION_H__
#define __GENERATION_H__

#include <memory>
class SimpleItem;

SimpleItem* getSimpleItemPointerFromTag(std::string tag);

std::unique_ptr<SimpleItem> getRandomItem();
std::unique_ptr<SimpleItem> getRandomArtifact();
std::unique_ptr<SimpleItem> getRandomWeapon();

class Enemy;
std::unique_ptr<Enemy> getRandomEnemy();

#endif
