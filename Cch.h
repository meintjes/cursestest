#ifndef __CCH_H__
#define __CCH_H__

#include "Color.h"

class Cch {
 public:
  friend void addc(const Cch &cch);
  friend Cch Color::operator()(const char &ch) const;
  friend Cch Color::operator()(const Cch &cch) const;
 private:
  //constructor is private. use Color::operator()
  Cch(char glyphIn, const Color &colorIn);

  char glyph;
  const Color &color;
};

#endif
