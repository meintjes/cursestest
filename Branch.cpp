#include "Branch.h"
#include <cassert>

Branch::Branch(Cst nameIn, unsigned int maxDepthIn,
       Branch *parentBranchIn, unsigned int parentDepthIn,
       Player &youIn) :
  name(nameIn),
  maxDepth(maxDepthIn),
  parentBranch(parentBranchIn),
  parentDepth(parentDepthIn),
  you(youIn)
{}

Map& Branch::getMap(unsigned int mapDepth) {
  assert(mapDepth <= maxDepth);

  while (floors.size() <= mapDepth) {
    floors.emplace_back(new Map(you, floors.size()));
  }

  return *floors.at(mapDepth);
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
