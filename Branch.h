#ifndef __BRANCH_H__
#define __BRANCH_H__

#include <string>
#include <vector>
#include "Map.h"
#include "Cst.h"

const int DEPTH_DUNGEON = 12;

struct Branch {
  Cst name;
  int depth;

  Branch *parentBranch;
  int parentDepth;

  std::vector<Map> floors;
};

#endif
