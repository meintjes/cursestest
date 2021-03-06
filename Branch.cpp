#include "Map.h"
#include "Branch.h"
#include "Archive.h"
#include <cstdio>
#include <cassert>
#include <fstream>

Branch::Branch(const Branch &other) :
  id(other.id),
  name(other.name),
  depthOffset(other.depthOffset),
  cachedDepth(-1),
  cachedMap(nullptr),
  maxDepth(other.maxDepth),
  parentBranch(other.parentBranch),
  parentDepth(other.parentDepth),
  you(other.you)
{}

Branch::Branch(std::string nameIn, int maxDepthIn, int depthOffsetIn,
       Branch *parentBranchIn, int parentDepthIn,
       Player &youIn, unsigned int idIn) :
  id(idIn),
  name(nameIn),
  depthOffset(depthOffsetIn),
  cachedDepth(-1),
  cachedMap(nullptr),
  maxDepth(maxDepthIn),
  parentBranch(parentBranchIn),
  parentDepth(parentDepthIn),
  you(youIn)
{}

Map& Branch::getMap(int mapDepth) {
  //assert(isValidDepth(mapDepth));
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
      cachedMap.reset(new Map(you, depthOffset + mapDepth));
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

std::string Branch::getName() const {
  return name;
}

int Branch::getMaxDepth() const {
  return maxDepth;
}

int Branch::getDepthOffset() const {
  return depthOffset;
}

void Branch::setParentBranch(Branch *parent) {
  parentBranch = parent;
}

Branch* Branch::getParentBranch() const {
  return parentBranch;
}

int Branch::getParentDepth() const {
  return parentDepth;
}

bool Branch::isValidDepth(int depth) const {
  return (depth < maxDepth) && (depth >= 0);
}

void Branch::deleteMapFiles() {
  for (int i = 0; i < maxDepth; i++) {
    remove(getPathFor(i).c_str());
  }
}

std::string Branch::getPathFor(int depth) {
  return "saves/" + std::to_string(id) + name + std::to_string(depth);
}
