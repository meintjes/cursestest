#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "ColoredText.h"

struct Branch;
struct Item;
class Map;

class Player {
 public:
  Player();

  void display() const;

  void tick();

  int getHp() const;
  int getLOS() const;
  Map* getCurrentFloor() const;
  int getNumItems() const;
  Cch getGlyph() const;

  bool lightTorch();
  bool dropBomb();
  bool drawArrow(); //toggles arrowmode
  bool shootArrow(); //returns whether you had arrowmode, then unsets it

  void damage(unsigned int num = 1);
  bool heal(unsigned int num);
  bool addBombs(int numIn);
  bool addTorches(int numIn);
  bool addArrows(int numIn);

  void setBranch(Branch *branch);
  bool changeDepth(int dz);

 private:
  static const int MAX_NUM_ITEMS = 20;
  bool addItem(int &item, int numIn);

  int hp;
  int hpMax;
  int numBombs;
  int numTorches;
  int torchDuration;
  int numArrows;

  Branch *currentBranch;
  int currentDepth;

  bool arrowMode;
};

#endif
