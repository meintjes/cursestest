#include "Enemy.h"
#include "Cch.h"
#include "Color.h"
#include "Map.h"
#include "Point.h"

Enemy::Enemy() {

}

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



Zombie::Zombie() :
  Enemy(),
  hp(3)
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
  if (hp > 0) {
    hp--;
  }
  else {
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
