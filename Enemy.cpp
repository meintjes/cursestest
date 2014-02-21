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

Enemy::~Enemy() {

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
}
