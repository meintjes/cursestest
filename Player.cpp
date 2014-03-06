#include "Player.h"
#include "functions.h"
#include "Space.h"
#include "Branch.h"
#include "Item.h"
#include "Cst.h"
#include "Cch.h"
#include <string>
#include <ncurses.h>
#include <cassert>

Player::Player() :
  hp(10),
  hpMax(10),
  currentArtifact(),
  mode(Mode::Move),

  currentBranch(nullptr),
  currentDepth(0),

  torchDuration(0),
  speedDuration(0),
  freeMovesDuration(0)

{}

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
  if (mode == Mode::Arrow) {
    if (freeMovesDuration > 0) {
      return BrownOnCyan('@');
    }
    else {
      return BlackOnBrown('@');
    }
  }

  else if (mode == Mode::Hook) {
    if (freeMovesDuration > 0) {
      return DarkGrayOnCyan('@');
    }
    else {
      return WhiteOnGray('@');
    }
  }

  else if (freeMovesDuration > 0) {
    return BlackOnCyan('@');
  }
  else {
    return BlackOnWhite('@');
  }
}

void Player::attack(int dx, int dy) {
  Map *currentFloor = getCurrentFloor();
  if (!currentWeapon) {
    int x = currentFloor->getPlayerX() + dx;
    int y = currentFloor->getPlayerY() + dy;
    currentFloor->getSpace(x, y).kill(*currentFloor, x, y);
  }
  else {
    currentWeapon->attack(currentFloor, dx, dy);
    if (currentWeapon->shouldDestroy()) {
      currentWeapon = nullptr;
    }
  }
}

bool Player::evokeArtifact() {
  if (currentArtifact &&
      currentArtifact->evoke(getCurrentFloor())) {
    if (currentArtifact->shouldDestroy()) {
      currentArtifact = nullptr;
    }
    return true;
  }
  else {
    return false;
  }
}

Player::Mode Player::getMode() const {
  return mode;
}

void Player::setMode(Player::Mode modeIn) {
  mode = modeIn;
}

const Weapon* const Player::getCurrentWeapon() const {
  return currentWeapon.get();
}

void Player::setWeapon(Weapon* const weapon) {
  if (currentWeapon) {
    addItem(currentWeapon.release(), false);
  }
  currentWeapon = std::unique_ptr<Weapon>(weapon);
}

const Artifact* const Player::getCurrentArtifact() const {
  return currentArtifact.get();
}

void Player::setArtifact(Artifact * const artifact) {
  if (currentArtifact) {
    addItem(currentArtifact.release(), false);
  }
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

bool Player::addItem(Item *item, bool checkMaxItems) {
  if (checkMaxItems && inventory.size() >= MAX_NUM_ITEMS) {
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

bool Player::useItem(Map *map) {
  //figure out which item the player wants to use
  InventoryInputResult input = getInventoryInput();
  
  //if the player chose an item in their general inventory:
  if (input.type == InventoryInputResult::Inventory) {
    if (input.item == inventory.end()) {
      return false;
    }
    
    Item::UseResult result = (*input.item)->use(map);
    if (result == Item::Fail) {
      return false; //don't take a turn if the item can't be used
    }

    else if (result == Item::Release) {
      input.item->release();
      inventory.erase(input.item);
    }
    else if (result == Item::Destroy) {
      inventory.erase(input.item);
    } 
    return true;
  }

  //if the player instead wanted to use their current weapon or artifact:
  else if (input.type == InventoryInputResult::CurrentWeapon) {
    Item::UseResult result = currentWeapon->use(map);
    if (result == Item::Release) {
      currentWeapon.release();
      return true;
    }
    else if (result == Item::Fail) {
      return false;
    }
    else {
      assert(false);
    }
  }

  else { //CurrentArtifact
    Item::UseResult result = currentArtifact->use(map);
    if (result == Item::Release) {
      currentArtifact.release();
      return true;
    }
    else if (result == Item::Fail) {
      return false;
    }
    else {
      assert(false);
    }
  }
}

bool Player::dropItem(Space &space) {
  if (space.hasItem()) {
    return false;
  }

  InventoryInputResult input = getInventoryInput();
  //dropping normal inventory items:
  if (input.type == InventoryInputResult::Inventory) { 
    if (input.item == inventory.end()) {
      return false;
    }
    space.setItem(std::move(*input.item));
    inventory.erase(input.item);
    //in case the player drops an arrow or something:
    setMode(Mode::Move);
    return true;
  }

  //drop current weapon:
  else if (input.type == InventoryInputResult::CurrentWeapon) {
    space.setItem(std::move(currentWeapon));
    return true;
  }

  else { //drop current artifact:
    space.setItem(std::move(currentArtifact));
    return true;
  }
}

void Player::stopTime(int num) {
  freeMovesDuration += num;
}

void Player::setTorchDuration(int num) {
  torchDuration = num;
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

Player::InventoryInputResult Player::getInventoryInput() {
  assert(inventory.size() <= MAX_NUM_ITEMS);
  InventoryInputResult result {InventoryInputResult::Inventory,
                               inventory.end()};
  //if you don't have any items, don't ask which one you want to pick
  if (inventory.size() == 0 && !currentArtifact && !currentWeapon) {
    return result;
  }
  erase();

  //menu header and current weapon/artifact display
  move(0, 0);
  addcs(Cyan("Which item? ("));
  if (currentWeapon) {
    addcs("w: " + currentWeapon->getName() + ", ");
  }
  if (currentArtifact) {
    addcs("a: " + currentArtifact->getName() + ", ");
  }
  addcs(Cyan("q: quit)"));

  //display regular inventory, assigning each item a key
  {
    int row = 1;
    char index = 'b';
    for (auto &item : inventory) {
      move(row, 3);
      addc(Cyan(index));
      move(row, 5);
      addc(item->getGlyph());
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
      return result;
    }
    else if (input == 'w' && currentWeapon) {
      result.type = InventoryInputResult::CurrentWeapon;
      return result;
    }
    else if (input == 'a' && currentArtifact) {
      result.type = InventoryInputResult::CurrentArtifact;
      return result;
    }
    {
      char index = 'b';
      for (auto it = inventory.begin(); it != inventory.end(); it++) {
        if (input == index) {
          result.item = it;
          return result;
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
