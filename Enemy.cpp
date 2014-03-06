#include "Enemy.h"
#include "Cch.h"
#include "Color.h"
#include "Map.h"
#include "Point.h"
#include "functions.h"

std::unique_ptr<Enemy> getRandomEnemy() {
  int num = randTo(99);
  if (num < 60)
    return std::unique_ptr<Enemy>(new Zombie);
  else if (num < 80)
    return std::unique_ptr<Enemy>(new Exploder);
  else if (num < 90)
    return std::unique_ptr<Enemy>(new Douser);
  else if (num < 99)
    return std::unique_ptr<Enemy>(new Reacher);
  else
    return std::unique_ptr<Enemy>(new SpawnerBoss);
}

Enemy::Enemy() :
  memoryDuration(0),
  memoryLocation({0, 0}),
  stunDuration(0)
{}

int Enemy::getRange() const {
  return 1;
}

void Enemy::attack(Map &map, int x, int y) {
  map.you.damage();
}

void Enemy::die(Map &map, int x, int y) {
  map.getSpace(x, y).removeEnemy();
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

void Enemy::stun(unsigned int turns) {
  if (stunDuration <= 0) {
    stunDuration++;
  }
  stunDuration += turns;
}

void Enemy::tick() {
  if (memoryDuration > 0) {
    memoryDuration--;
  }
  if (stunDuration > 0) {
    stunDuration--;
  }
}



Zombie::Zombie() :
  Enemy(),
  hp(randRange(1, 3))
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

void Zombie::die(Map &map, int x, int y) {
  hp--;
  if (hp <= 0) {
    map.getSpace(x, y).removeEnemy();
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
    map.getSpace(x, y).kill(map, x, y);
  }
  else {
    isPrimed = true;
  }
}

void Exploder::die(Map &map, int x, int y) {
  Point p = {x, y};
  for (int x = p.x - 1; x <= p.x + 1; x++) {
    for (int y = p.y - 1; y <= p.y + 1; y++) {
      map.getSpace(x, y).addGas(4);
    }
  }
  map.getSpace(x, y).removeEnemy();
}



Cch Reacher::getGlyph() const {
  return Orange('x');
}

int Reacher::getRange() const {
  return 2;
}



SpawnerBoss::SpawnerBoss() :
  Enemy(),
  hp(8)
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
    map.you.damage();
  }
  else if (map.getSpace(x, y).isPassable() && !map.getSpace(x, y).hasEnemy()) {
    map.getSpace(x, y).setEnemy(new Exploder);
  }
}

void SpawnerBoss::die(Map &map, int x, int y) {
  hp--;
  if (hp <= randRange(-2, 2)) {
    map.getSpace(x, y).removeEnemy();
    map.getSpace(x, y).setItem(new TimeStopper);
  }
}



Cch Douser::getGlyph() const {
  return LightBlue('z');
}

void Douser::attack(Map &map, int x, int y) {
  map.you.damage();
  map.you.setTorchDuration(0);
}
