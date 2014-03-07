#ifndef __MAP_H__
#define __MAP_H__

#include "Space.h"
#include "Player.h"
#include "Point.h"
#include "SpaceType.h"
#include <vector>

//index 0 and index MAPWIDTH + 1 or MAPHEIGHT + 1 are reserved for walls
const int MAPWIDTH = 78;
const int MAPHEIGHT = 21;

class Map {
  //defined in MapGen.cpp
 public:
  Map(Player &player, int depth);
 private:
  int depth;
  void drawLine(Point a, Point b, const SpaceType &type);
  void drawBox(Point a, Point b, const SpaceType &type);
  void generateRoom(Point center, int maxRadius);
  void generateBoxes(int depth);
  void sanitizeEntry(); //removes enemies within 2 spaces of player

  //defined in Map.cpp
 public:
  void display() const;

  bool movePlayer(int dx, int dy);
  bool shootArrow(int dx, int dy);
  bool throwHook(int dx, int dy);

  bool dropBomb();

  void moveEnemy(int x, int y);
  Space& getSpace(int x, int y);
  const Space& getSpace(int x, int y) const;
  void tick();

  Player &you;
  int getPlayerX() const;
  int getPlayerY() const;
  bool changeFloor(int dz, const SpaceType &type);

  static bool isValidX(int x);
  static bool isValidY(int y);
 
 private:
  Space space[MAPWIDTH + 2][MAPHEIGHT + 2];
  int playerX, playerY;

  bool isVisible(int x, int y, int LOS) const;
  bool hasLOS(int x1, int y1, int x2, int y2) const;

  void explode(int x, int y, int radius);
  std::vector<Point> toExplode;
  void executeToExplode();
  std::vector<Point> toAttack;
  void executeToAttack();
  std::vector<Point> toMove;
  void executeToMove();
  
  static int distance(int x1, int y1, int x2, int y2);
};

typedef bool (Map::*DirectionalFn)(int x, int y);

#endif
