#include "Enemy.h"
#include "Player.h"
#include "Cch.h"
#include "Color.h"
#include "Map.h"
#include "Item.h"
#include "Point.h"
#include "functions.h"
#include <stdexcept>
#include <string>

class Space;

void Enemy::serialize(Archive &ar) {
  ar & hp;
  ar & timeToAct;
  ar & memoryDuration;
  ar & memoryLocation.x;
  ar & memoryLocation.y;
  ar & stunDuration;
}

void Exploder::serialize(Archive &ar) {
  Enemy::serialize(ar);
  ar & isPrimed;
}



Enemy::Enemy(int hpIn) :
  hp(hpIn),
  timeToAct(0),
  memoryDuration(0),
  memoryLocation({0, 0}),
  stunDuration(0)
{}

bool Enemy::act(Map &map, int x, int y) {
  if (timeToAct >= getMoveTime() && map.distance(x, y) > getRange()) {
    timeToAct -= getMoveTime();
    map.moveEnemy(x, y);
    return true;
  }
  else if (timeToAct >= getAttackTime()) {
    timeToAct -= getAttackTime();
    attack(map, x, y);
    return true;
  }
  return false;
}

unsigned int Enemy::getMoveTime() const {
  return 8;
}

unsigned int Enemy::getAttackTime() const {
  return 8;
}

int Enemy::getRange() const {
  return 1;
}

void Enemy::attack(Map &map, int x, int y) {
  map.you.damage(1);
}

void Enemy::damage(int num, Map &map, int x, int y) {
  hp--;
  if (hp <= 0) {
    die(map, x, y);
  }
}

void Enemy::die(Map &map, int x, int y) {
  map(x, y).removeEnemy();
}

Enemy::~Enemy() {

}

void Enemy::renewMemory(Point playerLocation) {
  memoryDuration = 5;
  memoryLocation = playerLocation;
}

bool Enemy::hasMemory() const {
  return (memoryDuration > 0);
}

Point Enemy::getMemory() const {
  return memoryLocation;
}

bool Enemy::isStunned() const {
  return (stunDuration > 0);
}

void Enemy::stun(unsigned int duration) {
  if (stunDuration <= 0) {
    stunDuration++;
  }
  stunDuration += duration;
}

void Enemy::tick(unsigned int duration, Map &map, int x, int y) {
  decrementDuration(memoryDuration, duration);
  decrementDuration(stunDuration, duration);
}

void Enemy::addTimeToAct(unsigned int duration) {
  timeToAct += duration;
}



Zombie::Zombie() :
  Enemy(randRange(1, 3))
{}

Cch Zombie::getGlyph() const {
  switch (hp) {
  case 1:
    return LightGray('z');
  case 2:
    return LightGray('Z');
  default:
    return White('Z');
  }
}



Exploder::Exploder() :
  Enemy(),
  isPrimed(false)
{}

Cch Exploder::getGlyph() const {
  if (isPrimed) {
    return LightGreen('Q');
  }
  else {
    return LightGreen('q');
  }
}

int Exploder::getRange() const {
  if (isPrimed) {
    return 100;
  }
  else {
    return 1;
  }
}

void Exploder::attack(Map &map, int x, int y) {
  if (isPrimed) {
    die(map, x, y);
  }
  else {
    isPrimed = true;
  }
}

void Exploder::die(Map &map, int x, int y) {
  Point p = {x, y};
  for (int x = p.x - 1; x <= p.x + 1; x++) {
    for (int y = p.y - 1; y <= p.y + 1; y++) {
      map(x, y).addGas(32);
    }
  }
  map(x, y).removeEnemy();
}



Cch Reacher::getGlyph() const {
  return Orange('x');
}

int Reacher::getRange() const {
  return 2;
}



SpawnerBoss::SpawnerBoss() :
  Enemy(randRange(7, 9))
{}

Cch SpawnerBoss::getGlyph() const {
  if (hp > 3) {
    return LightGreen('B');
  }
  else {
    return DarkGreen('B');
  }
}

int SpawnerBoss::getRange() const {
  return 3;
}

void SpawnerBoss::attack(Map &map, int x, int y) {
  x += sgn(map.getPlayerX() - x);
  y += sgn(map.getPlayerY() - y);
  if (x == map.getPlayerX() && y == map.getPlayerY()) {
    map.you.damage(1);
  }
  else if (map(x, y).isPassable() && !map(x, y).hasEnemy()) {
    map(x, y).setEnemy(new Exploder);
    stun(2);
  }
}

void SpawnerBoss::die(Map &map, int x, int y) {
  map(x, y).removeEnemy();
  map(x, y).setItem(new Ore);
}



Cch Douser::getGlyph() const {
  return LightBlue('z');
}

void Douser::attack(Map &map, int x, int y) {
  map.you.damage(1);
  map.you.setTorchDuration(0);
}
