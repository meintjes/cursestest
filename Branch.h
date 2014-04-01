#ifndef __BRANCH_H__
#define __BRANCH_H__

#include <string>
#include <memory>

class Archive;
class Map;

//a Branch holds 

class Branch {
 public:
  //copy constructor so that vector<Branch> works. performs a shallow copy,
  //because you don't actually want to make a second Player or anything
  Branch(const Branch &other); 

  Branch(std::string nameIn, int maxDepthIn, int depthOffsetIn,
         Branch *parentBranchIn, int parentDepthIn,
         Player &youIn, unsigned int idIn);

  //get the name of the branch (Dungeon, etc).
  std::string getName() const;

  //get the maximum number of maps within the branch
  int getMaxDepth() const;

  //get the depth offset, i.e. the depth of the first level in the branch
  int getDepthOffset() const;

  //set the parent branch to the pointed-to branch
  void setParentBranch(Branch *parent);

  //get a pointer to the branch that contains this one. returns nullptr if the
  //branch isn't contained by anything.
  Branch* getParentBranch() const;

  //get the depth of the map in the parent branch which contains the stairs to
  //the entrance of this branch
  int getParentDepth() const;

  //replaces the cached map with the map at the requested depth, if it's not
  //the same map, and returns a reference to that map
  Map& getMap(int mapDepth);

  //deallocates the memory used by the map's cache and, if the cached level was
  //valid, saves it to file
  void emptyCache();

  //checks if the given depth might refer to an actual map that could live
  //within the branch
  bool isValidDepth(int depth) const;

  //deletes any saved map files associated with the branch. called whenever a
  //"new" branch is created (the initializing constructor calls it)
  void deleteMapFiles();

 private:
  unsigned int id;
  std::string name;
  int depthOffset;

  //returns a file path for saving/loading the map with the given depth
  std::string getPathFor(int depth);
  
  //Branch keeps its most recently requested map in memory. cachedMap points
  //to this map, cachedDepth indicates the depth of this map within the branch
  int cachedDepth;
  std::unique_ptr<Map> cachedMap;

  int maxDepth;

  Branch *parentBranch;
  int parentDepth;
  Player &you;
};

#endif
