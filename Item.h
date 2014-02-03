#ifndef __ITEM_H__
#define __ITEM_H__

#include <functional>
#include "Player.h"
#include "ColoredText.h"

struct Item {
  Cch glyph;
  std::function<bool(Player&)> pickupFunction;
};

#endif
