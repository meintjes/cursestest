#include "Player.h"
#include "ColoredText.h"
#include "functions.h"
#include "Branch.h"

Player::Player() {
  hp = 10;
  hpMax = 10;
  numBombs = 2;
  numTorches = 3;
  numArrows = 1;
  torchDuration = 0;

  currentBranch = nullptr;
  currentDepth = 0;

  arrowMode = false;
}

void Player::display() const {
  move(23, 0);
  //print hp bar, colored according to your current hp
  const Color &hpColor = ((hp * 10) / hpMax > 1) ? BlackOnGreen : BlackOnRed;
  int i = 0;
  for (; i < (hp * 10) / hpMax; i++) {
    Cch{' ', hpColor}.add();
    Cch{' ', hpColor}.add();
    Cch{' ', DarkGray}.add();
  } //print rest of bar for reference
  for (; i < 10; i++) {
    Cch{'[', DarkGray}.add();
    Cch{']', DarkGray}.add();
    Cch{' ', DarkGray}.add();
  }

  //print current floor
  move(23, 32);
  addstr(currentBranch->name.c_str());
  addstr(": ");
  addch('1' + currentDepth);

  //print item display
  move(23, 79 - MAX_NUM_ITEMS);
  for (int i = numBombs; i > 0; i--) {
    Cch{'!', Orange}.add();
  }
  for (int i = numTorches; i > 0; i--) {
    Cch{'^', Yellow}.add();
  }
  for (int i = numArrows; i> 0; i--) {
    Cch{'|', Brown}.add();
  }
  for (int i = 78 - getNumItems(); i < 79; i++) {
    addch(' ');
  }
}

void Player::tick() {
  if (torchDuration > 0) {
    torchDuration--;
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
  return (numBombs + numTorches + numArrows);
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
    numArrows++;
    return true;
  }
  else if (numArrows > 0) {
    arrowMode = true;
    numArrows--;
    return true;
  }
  else {
    return false;
  }
}

bool Player::shootArrow() {
  if (arrowMode) {
    arrowMode = false;
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
