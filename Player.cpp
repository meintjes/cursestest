#include "Player.h"
#include "functions.h"
#include "Space.h"
#include "Branch.h"
#include "Item.h"
#include "Cst.h"
#include "Cch.h"
#include <string>
#include <ncurses.h>

Player::Player() :
  hp(10),
  hpMax(10),
  currentArtifact(),

  currentBranch(nullptr),
  currentDepth(0),

  arrowMode(false),
  speedDuration(0),
  freeMovesDuration(0)

{
  lightTorch();
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

  //free up space so stuff updates correctly
  move(23, 31);
  for (int i = 31; i < 80; i++) {
    addch(' ');
  }

  //print current floor
  move(23, 31);
  addcs(currentBranch->name + ": ");
  addcs(White(std::to_string(1 + currentDepth)));

  //print current artifact
  if (currentArtifact) {
    move(23, 44);
    addc(currentArtifact->getGlyph());
    addcs(" (" + currentArtifact->getDescriptor() + ")");
  }

  //print item display
  move(23, 79 - inventory.size());
  for (auto &item : inventory) {
    addc(item->getGlyph());
  }
}

bool Player::tick() {
  if (currentArtifact && currentArtifact->shouldDestroy()) {
    currentArtifact = nullptr;
  }

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

bool Player::hasArrowMode() const {
  return arrowMode;
}

bool Player::evokeArtifact() {
  return (currentArtifact &&
	  currentArtifact->evoke(getCurrentFloor()));
}

bool Player::lightTorch() {
  /*if (numTorches > 0) {
    numTorches--;
    torchDuration = randRange(65, 85);
    return true;
  }
  else {
    return false;
  }*/
  return true;
}

bool Player::dropBomb() {
  /*
  if (numBombs > 0) {
    numBombs--;
    return true;
  }
  else {
    return false;
  }
  */
  return true;
}

bool Player::drawArrow() {
  /*
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
  */
  return true;
}

bool Player::shootArrow() {
  /*
  if (arrowMode && numArrows > 0) {
    numArrows--;
    arrowMode = false;
    return true;
  }
  else {
    return false;
  }
  */
  return true;
}

bool Player::quaffSpeedPotion() {
  /*
  if (numSpeedPotions > 0) {
    numSpeedPotions--;
    speedDuration = randRange(5, 6);
    return true;
  }
  else {
    return false;
  }*/
  return true;
}

const Artifact* const Player::getCurrentArtifact() const {
  return currentArtifact.get();
}

void Player::setArtifact(Artifact * const artifact) {
  currentArtifact = std::unique_ptr<Artifact>(artifact);
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

bool Player::addItem(Item *item) {
  if (inventory.size() >= MAX_NUM_ITEMS) {
    return false;
  }

  //sort the inventory alphabetically during insertion:
  for (auto it = inventory.begin(); it != inventory.end(); it++) {
    if ((*it)->getName() > item->getName()) {
      inventory.insert(it, std::unique_ptr<Item>(item));
      return true;
    }
  }
  //if nothing comes after it, just insert it at the end:
  inventory.insert(inventory.end(), std::unique_ptr<Item>(item));
  return true;
}

bool Player::dropItem(Space &space) {
  if (space.hasItem()) {
    return false;
  }
  
  auto item = getInventoryInput();
  if (item == inventory.end()) {
    return false;
  }

  space.setItem(std::move(*item));
  inventory.erase(item);
  return true;
}

void Player::stopTime(int num) {
  freeMovesDuration += num;
}

void Player::extinguishTorch() {
  torchDuration = 0;
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

const std::list<std::unique_ptr<Item> >::iterator
  Player::getInventoryInput() {
  if (inventory.size() == 0) {
    return inventory.end();
  }
  erase();
  move(23, 0);
  addcs(Cyan("Which item? (w: current weapon, a: current artifact, q: quit)"));
  //display inventory, assigning each item a key
  {
    int row = 0;
    char index = 'b';
    for (auto &item : inventory) {
      move(row, 3);
      addc(Cyan(index));
      addcs(" " + item->getName());

      row++; //display next line on next row
      //increment the index and skip keys with special meaning
      do {
        index++;
      }
      while (index == 'w' || index == 'a' || index == 'q');
    }
  }
  //then ask the player which item they want to pick
  while (true) {
    char input = getch();
    if (input == 'q') {
      return inventory.end();
    }
    {
      char index = 'b';
      for (auto it = inventory.begin(); it != inventory.end(); it++) {
        if (input == index) {
          return it;
        } 
        //increment the index and skip keys with special meaning 
        do {
          index++;
        }
        while (index == 'w' || index == 'a' || index == 'q');
      }
    }
  }
}
