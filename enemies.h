#ifndef __ZOMBIES__
#define __ZOMBIES__

#include "Enemy.h"
#include "Space.h"
#include "Map.h"
#include "Color.h"

const Enemy Zombie(LightGray('z'),
1, [](Map &map, int x, int y) {
  map.you.damage();
},
[](Map &map, int x, int y) {
  map.getSpace(x,y).setEnemy();
} );

const Enemy BigZombie(LightGray('Z'),
1, [](Map &map, int x, int y) {
  map.you.damage();
},
[](Map &map, int x, int y) {
  map.getSpace(x,y).setEnemy(Zombie);
} );

const Enemy PrimedExploder(LightGreen('O'),
3, [](Map &map, int x, int y) {
  map.getSpace(x,y).kill(map, x, y);
},
[](Map &map, int x, int y) {
  map.getSpace(x,y).setEnemy();
  for (int dx = -1; dx <= 1; dx++) {
    for (int dy = -1; dy <= 1; dy++) {
      map.getSpace(x + dx, y + dy).addGas(5);
    }
  }
} );

const Enemy Exploder(LightGreen('o'),
1, [](Map &map, int x, int y) {
  map.getSpace(x,y).setEnemy(PrimedExploder);
},
[](Map &map, int x, int y) {
  map.getSpace(x,y).setEnemy();
  map.getSpace(x,y).addGas(5);
} );

const Enemy Reacher(Orange('x'),
2, [](Map &map, int x, int y) {
  map.you.damage();
},
[](Map &map, int x, int y) {
  map.getSpace(x,y).setEnemy();
} );
#endif
