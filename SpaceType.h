#ifndef __SPACETYPE_H__
#define __SPACETYPE_H__

#include "Cch.h"

struct SpaceType {
  bool operator==(const SpaceType &rhs) const;
  Cch glyph;
  bool passable;
  bool transparent;
  bool destructible;
  int id; //shitty kludge to make Space::typeIs() work
};

//                    glyph                  passable  transparent  destructible
const SpaceType Wall {Cch{'#', White},          false,    false,    false, 0};
const SpaceType GlassWall {Cch{'#', LightCyan}, false,    true,     true, 1};
const SpaceType Floor {Cch{'.', White},         true,     true,     false, 2};

const SpaceType StairsUp {Cch{'<', White},      true,     true,     false, 3};
const SpaceType StairsDown {Cch{'>', White},    true,     true,     false, 4};

#endif
