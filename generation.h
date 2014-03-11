#ifndef __GENERATION_H__
#define __GENERATION_H__

#include <memory>
class SimpleItem;

std::unique_ptr<SimpleItem> getRandomItem();

std::unique_ptr<SimpleItem> getRandomArtifact();

std::unique_ptr<SimpleItem> getRandomWeapon();

#endif
