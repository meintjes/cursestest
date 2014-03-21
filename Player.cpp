#include "Player.h"
#include "functions.h"
#include "Space.h"
#include "Branch.h"
#include "Item.h"
#include "Weapon.h"
#include "Artifact.h"
#include "Color.h"
#include <string>
#include <ncurses.h>
#include <cassert>

Player::Player() :
  ore(0),
  hpMax(20),
  currentHpMax(hpMax),
  hp(currentHpMax),
  staminaMax(10),
  stamina(staminaMax),
  mode(Mode::Move),
  modeItemIterator(inventory.end()),
  lastMoveDirection({0, 0}),
  damageTimer(0),

  currentBranch(nullptr),
  currentDepth(0),

  torchDuration(0),
  speedDuration(0),
  freeMovesDuration(0)

{}

Player::~Player() {}

void Player::display() const {
  //overwrite old information so everything updates correctly
  for (int i = 0; i < 80; i++) {
    addc(i, 22, ' ');
    addc(i, 23, ' ');
  }

  //at low hp, change bar colors
  const Color &hpColor = hpIsHigh() ? BlackOnGreen : BlackOnRed;
  const Color &staminaColor = staminaIsHigh() ? BlackOnBrown : BlackOnRed;
  
  //print hp and stamina bars
  for (int i = 21; i >= 0; i--) {
    addc(78, 21 - i, ((i <= 21 * hp / hpMax) ? hpColor : LightGray)(' '));
    addc(((i <= 21 * stamina / staminaMax) ? staminaColor : LightGray)(' '));
  }
  //display marker for the maximum possible hp regeneration
  if (currentHpMax > hp) {
    addc(78, 21 - (21 * currentHpMax / hpMax), LightGreen('-'));
  }

  //print numerical displays at bottom right corner
  std::string hpDisplay = std::to_string(hp)
                        + '/'
                        + std::to_string(hpMax);
  addcs(80 - hpDisplay.length(), 22, hpColor(hpDisplay));

  std::string staminaDisplay = std::to_string(stamina)
                             + '/'
                             + std::to_string(staminaMax);
  addcs(80 - staminaDisplay.length(), 23, staminaColor(staminaDisplay));

  //print current location
  addcs(0, 23, currentBranch->getName() + ": ");
  addcs(White(std::to_string(1 + currentDepth)));

  //print current weapon and artifact
  if (currentWeapon) {
    addc(20, 22, currentWeapon->getGlyph());
    addcs(22, 22, currentWeapon->getName());
    addcs(" (" + currentWeapon->getDescriptor() + ")");
  }
  if (currentArtifact) {
    addc(20, 23, currentArtifact->getGlyph());
    addcs(22, 23, currentArtifact->getName());
    addcs(" (" + currentArtifact->getDescriptor() + ")");
  }

  //print item display
  move(22, 50);
  for (auto &item : inventory) {
    addc(item->getGlyph());
  }
  addcs(50, 23, Brown(std::to_string(ore) + " x Ore"));
}

bool Player::tick() {
  Map &map = getCurrentFloor();
  map.lightArea(map.getPlayerX(),
                map.getPlayerY(),
                torchDuration > 0 ? 6 : 3,
                1);

  if (!movedLastTurn) {
    if (!damageTimer && stamina >= staminaMax) {
      if (hp < currentHpMax) {
        hp += 1;
      }
    }
    else {
      restoreStamina(1);
    }
  }

  if (movedLastTurn) {
    movedLastTurn = false;
  }
  else {
    lastMoveDirection = {0, 0};
  }

  if (damageTimer > 0) {
    damageTimer--;
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

Map& Player::getCurrentFloor() const {
  return currentBranch->getMap(currentDepth);
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

bool Player::attack(int dx, int dy) {
  Map &currentFloor = getCurrentFloor();
  removeStamina(2);
  if (!currentWeapon) {
    int x = currentFloor.getPlayerX() + dx;
    int y = currentFloor.getPlayerY() + dy;
    currentFloor(x, y).damage(1, currentFloor, x, y);
    return true;
  }
  else {
    bool shouldSpendTurn = currentWeapon->attack(currentFloor, dx, dy);
    if (currentWeapon->shouldDestroy()) {
      currentWeapon = nullptr;
    }
    return shouldSpendTurn;
  }
}
bool Player::hpIsHigh() const {
  return (hp * 3) / hpMax;
}

bool Player::staminaIsHigh() const {
  return (stamina * 3) / staminaMax;
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
  modeItemIterator = inventory.end();
  mode = modeIn;
}

void Player::destroyModeItem() {
  assert(modeItemIterator != inventory.end());
  inventory.erase(modeItemIterator);
  modeItemIterator = inventory.end();
}

void Player::setLastMoveDirection(const Point &direction) {
  lastMoveDirection = direction;
  movedLastTurn = true;
}

Point Player::getLastMoveDirection() const {
  return lastMoveDirection;
}

const Weapon* const Player::getCurrentWeapon() const {
  return currentWeapon.get();
}

void Player::setWeapon(Weapon* const weapon) {
  if (currentWeapon) {
    addItemUnsafe(currentWeapon.release());
  }
  currentWeapon = std::unique_ptr<Weapon>(weapon);
}

const Artifact* const Player::getCurrentArtifact() const {
  return currentArtifact.get();
}

void Player::setArtifact(Artifact * const artifact) {
  if (currentArtifact) {
    addItemUnsafe(currentArtifact.release());
  }
  currentArtifact = std::unique_ptr<Artifact>(artifact);
}

void Player::damage(unsigned int num) {
  hp -= num;
  damageTimer = 3;
  if (!staminaIsHigh()) {
    currentHpMax -= num;
  }
}

bool Player::heal(unsigned int num) {
  if (restoreAttribute(hp, hpMax, num)) {
    restoreAttribute(currentHpMax, hpMax, num);
    return true;
  }
  else {
    return false;
  }
}

bool Player::removeStamina(int num) {
  assert(num > 0);
  if (stamina < num) {
    return false;
  }
  else {
    stamina -= num;
    return true;
  }
}

bool Player::restoreStamina(int num) {
  assert(num > 0);
  return restoreAttribute(stamina, staminaMax, num);
}

bool Player::restoreAttribute(int &att, int &attMax, int num) {
  if (att >= attMax) {
    return false;
  }
  else {
    att += num;
    if (att > attMax) { //don't allow overhealing or whatever
      att = attMax;
    }
    return true;
  }
}

bool Player::addItem(Item *item) {
  if (inventory.size() >= MAX_NUM_ITEMS) {
    return false;
  }
  else {
    addItemUnsafe(item);
    return true;
  }
}

void Player::addItemUnsafe(Item *item) {
  //sort the inventory by glyph and then by name during insertion:
  for (auto it = inventory.begin(); it != inventory.end(); it++) {
    if ((*it)->getGlyph() > item->getGlyph() ||
       ((*it)->getGlyph() == item->getGlyph() &&
        (*it)->getName() > item->getName())) {
      inventory.insert(it, std::unique_ptr<Item>(item));
      return;
    }
  }
  //if nothing comes after it, just insert it at the end:
  inventory.insert(inventory.end(), std::unique_ptr<Item>(item));
}

void Player::addOre(int num) {
  assert(ore >= 0);
  ore += num;
}

bool Player::removeOre(int num) {
  assert (num >= 0);
  if (ore < num) {
    return false;
  }
  else {
    ore -= num;
    return true;
  }
}

bool Player::useItem() {
  //figure out which item the player wants to use
  InventoryInputResult input = getInventoryInput(); 
  //if the player chose an item in their general inventory:
  if (input.type == InventoryInputResult::Inventory) {
    if (input.item == inventory.end()) {
      return false; //if the player didn't choose an item, back out
    }

    //record the player's mode before they used the item
    Mode previousMode = getMode();

    //actually use the item and record the result in result
    Item::UseResult result = (*input.item)->use(getCurrentFloor());
    
    //if using the item caused the player's mode to change, associate
    //the item with the player's mode
    if (previousMode != mode && mode != Mode::Move) {
      modeItemIterator = input.item;
    }

    //then handle the result of using the item:
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
    Item::UseResult result = currentWeapon->use(getCurrentFloor());
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
    Item::UseResult result = currentArtifact->use(getCurrentFloor());
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

bool Player::dropItem() {
  Space &space = getCurrentFloor()(getCurrentFloor().getPlayerX(),
                                   getCurrentFloor().getPlayerY());
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
    if (input.item == modeItemIterator) {
      mode = Mode::Move;
    }
    
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
  if (currentDepth >= currentBranch->getMaxDepth()) {
    currentDepth = currentBranch->getMaxDepth() - 1;
    return false;
  }

  //if going up from top of branch, go to parent branch and depth
  if (currentDepth < 0) {
    if (currentBranch->getParentBranch()) { 
      currentDepth = currentBranch->getParentDepth();
      currentBranch = currentBranch->getParentBranch();
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
  addcs(0, 0, Cyan("Which item? ("));
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
      addc(3, row, Cyan(index));
      addc(5, row, item->getGlyph());
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
