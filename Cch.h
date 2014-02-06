#ifndef __CCH_H__
#define __CCH_H__

#include "Color.h"

class Cch {
 public:
  friend void addc(const Cch &cch);
  friend class Color;
 private:
  //constructor is private. use Color::operator()
  Cch(char glyphIn, const Color &colorIn);

  const char glyph;
  const Color &color;
};

#endif
