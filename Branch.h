#ifndef __BRANCH_H__
#define __BRANCH_H__

#include <string>
#include <vector>
#include "Map.h"

const int DEPTH_DUNGEON = 9;

struct Branch {
  std::string name;
  int depth;

  Branch *parentBranch;
  int parentDepth;

  std::vector<Map> floors;
};

#endif
