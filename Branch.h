#ifndef __BRANCH_H__
#define __BRANCH_H__

#include <string>
#include <vector>
#include "Map.h"
#include "Cst.h"

const int DEPTH_DUNGEON = 12;

class Branch {
 public:
  Branch(Cst nameIn, unsigned int maxDepthIn,
         Branch *parentBranchIn, unsigned int parentDepthIn,
         Player &youIn);
  Cst getName() const;
  int getMaxDepth() const;
  Branch* getParentBranch() const;
  int getParentDepth() const;
  Map& getMap(unsigned int mapDepth);

 private:
  Cst name;
  std::vector<Map*> floors;
  
  unsigned int maxDepth;
  Branch *parentBranch;
  unsigned int parentDepth;
  Player &you;
};

#endif
