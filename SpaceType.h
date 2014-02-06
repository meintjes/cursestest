#ifndef __SPACETYPE_H__
#define __SPACETYPE_H__

#include "Color.h"
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
const SpaceType Wall {White('#'),             false,    false,    false, 0};
const SpaceType GlassWall {LightCyan('#'),    false,    true,     true, 1};
const SpaceType Floor {White('.'),            true,     true,     false, 2};

const SpaceType StairsUp {White('<'),         true,     true,     false, 3};
const SpaceType StairsDown {White('>'),       true,     true,     false, 4};

#endif
