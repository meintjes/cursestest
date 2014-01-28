#ifndef __CCH_H__
#define __CCH_H__

#include "Color.h"

class Cch {
 public:
  Cch(char glyphIn, const Color &colorIn);
  void add() const;
  Cch shift(const Color &newcolor) const;
 private:
  char glyph;
  const Color &color;
};

#endif
