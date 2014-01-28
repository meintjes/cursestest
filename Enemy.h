#ifndef __ENEMY_H__
#define __ENEMY_H__

#include <functional>
#include "Cch.h"

class Map;

struct Enemy {
  Enemy(Cch glyphIn,
	int rangeIn,
        std::function<void(Map&,int,int)> attackFunctionIn,
        std::function<void(Map&,int,int)> deathFunctionIn);

  Cch glyph;
  int range;
  std::function<void(Map&,int,int)> attackFunction;
  std::function<void(Map&,int,int)> deathFunction;
};

#endif
