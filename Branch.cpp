#include "Branch.h"
#include <string>
#include <cassert>
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

Branch::~Branch() {
  if (cachedMap) {
    delete cachedMap;
  }
}

Map& Branch::getMap(unsigned int mapDepth) {
  assert(mapDepth <= maxDepth);
  assert(mapDepth >= 0);

  if (mapDepth != cachedDepth) {
    //save the cached level to file
    std::string saveFilename = name.data() + std::to_string(cachedDepth);
    std::ofstream saveStream(saveFilename.data());
    if (saveStream) {
      boost::archive::text_oarchive saveArchive(saveStream);
      saveArchive << cachedMap;
    }

    //load the cached level from file
    std::string loadFilename = name.data() + std::to_string(mapDepth);
    std::ifstream loadStream(loadFilename.data());
    if (loadStream) {
      boost::archive::text_iarchive loadArchive(loadStream);
      loadArchive >> cachedMap;
      cachedMap->setPlayer(you);
    }
    else { //if there's no file, just make a new map
      if (cachedMap) {
        delete cachedMap;
      }
      cachedMap = new Map(you, mapDepth);
    }
    cachedDepth = mapDepth;
  }
  
  return *cachedMap;
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
