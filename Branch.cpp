#include "Branch.h"
#include <string>
#include <cassert>
#include "unique.h"
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

Branch::Branch(Cst nameIn, unsigned int maxDepthIn,
       Branch *parentBranchIn, unsigned int parentDepthIn,
       Player *youIn) :
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
    std::string loadFilename = name.data() + std::to_string(mapDepth);
    std::ifstream loadStream(loadFilename.data());
    if (loadStream) {
      boost::archive::text_iarchive loadArchive(loadStream);
      loadArchive >> cachedMap;
      cachedMap->setPlayer(you);
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
    std::string saveFilename = name.data() + std::to_string(cachedDepth);
    std::ofstream saveStream(saveFilename.data());
    if (saveStream) {
      boost::archive::text_oarchive saveArchive(saveStream);
      saveArchive << cachedMap;
    }
  }
  cachedMap = nullptr;
  cachedDepth = -1;
}

bool Branch::isValidDepth(unsigned int depth) const {
  return (depth < maxDepth) && (depth >= 0);
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
