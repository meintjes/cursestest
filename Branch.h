#ifndef __BRANCH_H__
#define __BRANCH_H__

#include "Map.h"
#include "Cst.h"

#include <memory>

const int DEPTH_DUNGEON = 12;

class Branch {
 public:
  Branch(Cst nameIn, unsigned int maxDepthIn,
         Branch *parentBranchIn, unsigned int parentDepthIn,
         Player *youIn);

  //get the name of the branch (Dungeon, etc).
  Cst getName() const;

  //get the maximum number of maps within the branch 
  int getMaxDepth() const;

  //get a pointer to the branch that contains this one. returns nullptr if the
  //branch isn't contained by anything.
  Branch* getParentBranch() const;

  //get the depth of the map in the parent branch which contains the stairs to
  //the entrance of this branch
  int getParentDepth() const;

  //replaces the cached map with the map at the requested depth, if it's not
  //the same map, and returns a reference to that map
  Map& getMap(unsigned int mapDepth);

  //deallocates the memory used by the map's cache and, if the cached level was
  //valid, saves it to file
  void emptyCache();

  bool isValidDepth(unsigned int depth) const;

 private:
  Cst name;
  
  //Branch keeps its most recently requested map in memory. cachedMap points
  //to this map, cachedDepth indicates the depth of this map within the branch
  unsigned int cachedDepth;
  std::unique_ptr<Map> cachedMap;

  unsigned int maxDepth;

  Branch *parentBranch;
  unsigned int parentDepth;
  Player *you;
};

#endif
