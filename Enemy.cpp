#include "Enemy.h"
#include "Map.h"

Enemy::Enemy(Cch glyphIn,
	     int rangeIn,
	     std::function<void(Map&,int,int)> attackFunctionIn,
	     std::function<void(Map&,int,int)> deathFunctionIn)
  : glyph(glyphIn), range(rangeIn),
    attackFunction(attackFunctionIn), deathFunction(deathFunctionIn)
{

}
