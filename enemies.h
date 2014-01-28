#ifndef __ZOMBIES__
#define __ZOMBIES__

#include "Enemy.h"
#include "Space.h"
#include "Map.h"
#include "Color.h"

const Enemy zombie(Cch{'z', LightGray},
1, [](Map &map, int x, int y) {
  map.you.damage();
},
[](Map &map, int x, int y) {
  map.getSpace(x,y).setEnemy(nullptr);
} );

const Enemy bigZombie(Cch{'Z', LightGray},
1, [](Map &map, int x, int y) {
  map.you.damage();
},
[](Map &map, int x, int y) {
  map.getSpace(x,y).setEnemy(&zombie);
} );

const Enemy primedExploder(Cch{'O', LightGreen},
3, [](Map &map, int x, int y) {
  map.getSpace(x,y).kill(map, x, y);
},
[](Map &map, int x, int y) {
  map.getSpace(x,y).setEnemy(nullptr);
  for (int dx = -1; dx <= 1; dx++) {
    for (int dy = -1; dy <= 1; dy++) {
      map.getSpace(x + dx, y + dy).addGas(5);
    }
  }
} );

const Enemy exploder(Cch{'o', LightGreen},
1, [](Map &map, int x, int y) {
  map.getSpace(x,y).setEnemy(&primedExploder);
},
[](Map &map, int x, int y) {
  map.getSpace(x,y).setEnemy(nullptr);
  map.getSpace(x,y).addGas(5);
} );

const Enemy reacher(Cch{'x', Orange},
2, [](Map &map, int x, int y) {
  map.you.damage();
},
[](Map &map, int x, int y) {
  map.getSpace(x,y).setEnemy(nullptr);
} );
#endif
