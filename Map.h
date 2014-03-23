#ifndef __MAP_H__
#define __MAP_H__

#include "Space.h"
#include "Point.h"
#include "Enemy.h"
#include <vector>

namespace boost {
  namespace serialization {
    class access;
  }
}

class Player;

//index 0 and index MAPWIDTH + 1 or MAPHEIGHT + 1 are reserved for walls
const int MAPWIDTH = 76;
const int MAPHEIGHT = 20;

class Map {
  //defined in MapGen.cpp
 public:
  Map();
  Map(Player *playerIn, int depth);
 private:
  int depth;
  void drawLine(Point a, Point b, Space::Type type);
  void drawBox(Point a, Point b, Space::Type type);
  void generateRoom(Point center, int maxRadius);
  void generateBoxes(int depth);
  void sanitizeEntry(); //removes enemies within 2 spaces of player

  //defined in Map.cpp
 public:
  //displays the map (and only the map) and refreshes the screen. overwrites
  //the entire map area each turn; for animations, use ncurses directly.
  void display() const;

  //directional functions. return true if the action should spend a turn,
  //or false if it shouldn't spend a turn.
  bool movePlayer(int dx, int dy); //normal movement and attacking
  bool shootArrow(int dx, int dy);
  bool throwHook(int dx, int dy);

  //lights all space within radius spaces of (x, y) for turns turns
  void lightArea(int x, int y, int radius, int turns);

  //causes the enemy at the given coordinates to attempt to move toward the
  //player (or the last place it saw the player). 
  void moveEnemy(int x, int y);

  //get a reference (or const reference) to the space located at coords x, y.
  //causes an assertion failure if the space is beyond those coordinates.
  Space& operator()(int x, int y);
  const Space& operator()(int x, int y) const;

  //main function for updating game state, for everything from the player
  //to the spaces and the enemies contained within.
  void tick();

  //returns a reference to the player
  Player& you();
  const Player& you() const;
  void setPlayer(Player* playerIn);

  //get the player's coordinates on the map
  int getPlayerX() const;
  int getPlayerY() const;

  //verify that the given coordinate isn't beyond the bounds of a map.
  static bool isValidX(int x);
  static bool isValidY(int y);
 
  //causes every space within radius spaces of (x, y) to explode
  void explodeArea(int x, int y, int radius);

 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & space;
    ar & playerX;
    ar & playerY;
    ar & enemyCount;
  }

  Space space[MAPWIDTH + 2][MAPHEIGHT + 2];
  Player *player;
  int playerX, playerY;

  //an approximate count of the number of enemies on the map (one turn ago).
  //don't use for anything serious, just as an approximation.
  int enemyCount;

  bool isVisible(int x, int y) const;
  bool hasLOS(int x1, int y1, int x2, int y2) const;

  //these are cleared every turn after the update loop, so they're not saved
  std::vector<Point> toExplode;
  void executeToExplode();
  std::vector<Point> toAttack;
  void executeToAttack();
  std::vector<Point> toMove;
  void executeToMove();
  
  //returns the distance from the player to the given coordinates
  int distance(int x1, int y1) const;
};

typedef bool (Map::*DirectionalFn)(int x, int y);

#endif
