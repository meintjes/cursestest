#include "Player.h"
#include "functions.h"
#include "Branch.h"
#include "Cst.h"
#include "Cch.h"
#include <string>
#include <ncurses.h>

Player::Player() {
  hp = 10;
  hpMax = 10;
  numBombs = 1;
  numTorches = 3;
  numArrows = 3;
  numSpeedPotions = 0;

  currentBranch = nullptr;
  currentDepth = 0;

  arrowMode = false;
  lightTorch();
  speedDuration = 0;
  freeMovesDuration = 0;
}

void Player::display() const {
  move(23, 0);
  //print hp bar, colored according to your current hp
  const Color &hpColor = ((hp * 10) / hpMax > 1) ? BlackOnGreen : BlackOnRed;
  int i = 0;
  for (; i < (hp * 10) / hpMax; i++) {
    addcs(hpColor("  "));
    addc(DarkGray(' '));
  } //print rest of bar for reference
  for (; i < 10; i++) {
    addcs(DarkGray("[] "));
  }

  //print current floor
  move(23, 31);
  addcs(currentBranch->name + ": ");
  addcs(White(std::to_string(1 + currentDepth)));

  //print item display
  move(23, 79 - MAX_NUM_ITEMS);
  for (int i = numBombs; i > 0; i--) {
    addc(Orange('*'));
  }
  for (int i = numTorches; i > 0; i--) {
    addc(Yellow('^'));
  }
  for (int i = numArrows; i > 0; i--) {
    addc(Brown('|'));
  }
  for (int i = numSpeedPotions; i > 0; i--) {
    addc(LightCyan('!'));
  }
  for (int i = 78 - getNumItems(); i < 79; i++) {
    addch(' ');
  }
}

bool Player::tick() {
  if (torchDuration > 0) {
    torchDuration--;
  }

  if (freeMovesDuration > 0) {
    freeMovesDuration--;
    return false;
  }
  else {
    if (speedDuration > 0) {
      freeMovesDuration++;
      speedDuration--;
    }
    return true;
  }
}

int Player::getHp() const {
  return hp;
}

int Player::getLOS() const {
  return ((torchDuration > 0) ? 6 : 3);
}

Map* Player::getCurrentFloor() const {
  return &currentBranch->floors.at(currentDepth);
}

int Player::getNumItems() const {
  return (numBombs + numTorches + numArrows + numSpeedPotions);
}

Cch Player::getGlyph() const {
  if (arrowMode) {
    if (freeMovesDuration > 0) {
      return BrownOnCyan('@');
    }
    else {
      return BlackOnBrown('@');
    }
  }
  else if (freeMovesDuration > 0) {
    return BlackOnCyan('@');
  }
  else {
    return BlackOnWhite('@');
  }
}

bool Player::lightTorch() {
  if (numTorches > 0) {
    numTorches--;
    torchDuration = randRange(65, 85);
    return true;
  }
  else {
    return false;
  }
}

bool Player::dropBomb() {
  if (numBombs > 0) {
    numBombs--;
    return true;
  }
  else {
    return false;
  }
}

bool Player::drawArrow() {
  if (arrowMode) {
    arrowMode = false;
    return true;
  }
  else if (numArrows > 0) {
    arrowMode = true;
    return true;
  }
  else {
    return false;
  }
}

bool Player::shootArrow() {
  if (arrowMode && numArrows > 0) {
    numArrows--;
    arrowMode = false;
    return true;
  }
  else {
    return false;
  }
}

bool Player::quaffSpeedPotion() {
  if (numSpeedPotions > 0) {
    numSpeedPotions--;
    speedDuration = randRange(5, 6);
    return true;
  }
  else {
    return false;
  }
}

void Player::damage(unsigned int num) {
  hp -= num;
}

bool Player::heal(unsigned int num) {
  if (hp >= hpMax) {
    return false;
  }
  else {
    hp += num;
    if (hp > hpMax) { //don't allow overhealing
      hp = hpMax;
    }
    return true;
  }
}

bool Player::addBombs(int numIn) {
  return (addItem(numBombs, numIn));
}

bool Player::addTorches(int numIn) {
  return (addItem(numTorches, numIn));
}

bool Player::addArrows(int numIn) {
  return (addItem(numArrows, numIn));
}

bool Player::addSpeedPotions(int numIn) {
  return (addItem(numSpeedPotions, numIn));
}

bool Player::addItem(int &item, int numIn) {
  int num;
  for (num = numIn; getNumItems() < MAX_NUM_ITEMS && num > 0; num--) {
    item++;
  }
  return (num != numIn);
}

void Player::setBranch(Branch *branch) {
  currentBranch = branch;
}

bool Player::changeDepth(int dz) {
  currentDepth += dz;

  //if going down from bottom of branch, don't spend a turn
  //TODO: make branch ends not generate down-stairs
  if (currentDepth >= currentBranch->depth) {
    currentDepth = currentBranch->depth - 1;
    return false;
  }

  //if going up from top of branch, go to parent branch and depth
  if (currentDepth < 0) {
    if (currentBranch->parentBranch) { 
      currentDepth = currentBranch->parentDepth;
      currentBranch = currentBranch->parentBranch;
      return true;
    }
    else { //if there's no parent branch, don't spend a turn
      currentDepth -= dz;
      return false;
    }
  }

  //spend a turn in the general case
  return true;
}
