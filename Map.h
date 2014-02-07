#ifndef __MAP_H__
#define __MAP_H__

#include "Space.h"
#include "Player.h"
#include "Point.h"
#include "SpaceType.h"
#include <utility>
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
  bool dropBomb();

  void moveEnemy(int x, int y);
  Space& getSpace(int x, int y);
  void tick(int turns = 1);

  Player &you;
  int getPlayerX() const;
  int getPlayerY() const;
  bool changeFloor(int dz, const SpaceType *type);

 private:
  Space space[MAPWIDTH + 2][MAPHEIGHT + 2];
  int playerX, playerY;

  static bool isValidX(int x);
  static bool isValidY(int y);
  bool isVisible(int x, int y, int LOS) const;
  bool hasDirectLOS(int x, int y, int x2, int y2) const;

  std::vector<Point> toExplode;
  void executeToExplode();
  std::vector<Point> toAttack;
  void executeToAttack();
  std::vector<Point> toMove;
  void executeToMove();
  
  static int distance(int x1, int y1, int x2, int y2);
  static std::pair<int, int> direction(int x1, int y1, int x2, int y2);
};

#endif
