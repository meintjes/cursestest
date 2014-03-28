#include "Branch.h"
#include <cstdio>
#include <string>
#include <cassert>
#include <fstream>

Branch::Branch(Cst nameIn, unsigned int maxDepthIn,
       Branch *parentBranchIn, unsigned int parentDepthIn,
       Player &youIn) :
  name(nameIn),
  cachedDepth(-1),
  cachedMap(nullptr),
  maxDepth(maxDepthIn),
  parentBranch(parentBranchIn),
  parentDepth(parentDepthIn),
  you(youIn)
{}

Map& Branch::getMap(unsigned int mapDepth) {
  assert(isValidDepth(mapDepth));

  if (mapDepth != cachedDepth) {
    //save the cached level to file
    emptyCache();

    //load the requested level from file
    Archive ar(getPathFor(mapDepth), Archive::Load);
    if (ar) {
      cachedMap.reset(new Map(you));
      cachedMap->serialize(ar);
    }
    else { //if there's no file, just make a new map
      cachedMap.reset(new Map(you, mapDepth));
    }
    cachedDepth = mapDepth;
  }
  
  return *cachedMap;
}

void Branch::emptyCache() {
  if (isValidDepth(cachedDepth)) {
    Archive ar(getPathFor(cachedDepth), Archive::Save);
    cachedMap->serialize(ar);
  }
  cachedMap = nullptr;
  cachedDepth = -1;
}

Cst Branch::getName() const {
  return name;
}

int Branch::getMaxDepth() const {
  return maxDepth;
}

Branch* Branch::getParentBranch() const {
  return parentBranch;
}

int Branch::getParentDepth() const {
  return parentDepth;
}

bool Branch::isValidDepth(unsigned int depth) const {
  return (depth < maxDepth) && (depth >= 0);
}

void Branch::deleteMapFiles() {
  for (unsigned int i = 0; i < maxDepth; i++) {
    remove(getPathFor(i).data());
  }
}

std::string Branch::getPathFor(int depth) {
  return "saves/" + name.data() + std::to_string(depth);
}
