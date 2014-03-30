#ifndef __BRANCH_H__
#define __BRANCH_H__

#include <string>
#include <memory>

class Archive;
class Map;

class Branch {
 public:
  //copy constructor so that vector<Branch> works. performs a shallow copy,
  //because you don't actually want to make a second Player
  Branch(const Branch &other); 
  //non-initializing constructor for deserialization
  Branch(Player &youIn);
  //read/write branch info to/from file (not the player, though)
  void serialize(Archive &ar);

  //initializing constructor for new games. deletes maps.
  Branch(std::string nameIn, unsigned int maxDepthIn,
         Branch *parentBranchIn, unsigned int parentDepthIn,
         Player &youIn);

  //get the name of the branch (Dungeon, etc).
  std::string getName() const;

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

  //checks if the given depth might refer to an actual map that could live
  //within the branch
  bool isValidDepth(unsigned int depth) const;

  //deletes any saved map files associated with the branch. called whenever a
  //"new" branch is created (the initializing constructor calls it)
  void deleteMapFiles();

 private:
  std::string name;

  //returns a file path for saving/loading the map with the given depth
  std::string getPathFor(int depth);
  
  //Branch keeps its most recently requested map in memory. cachedMap points
  //to this map, cachedDepth indicates the depth of this map within the branch
  unsigned int cachedDepth;
  std::unique_ptr<Map> cachedMap;

  unsigned int maxDepth;

  Branch *parentBranch;
  unsigned int parentDepth;
  Player &you;
};

#endif
