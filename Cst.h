#ifndef __CST_H__
#define __CST_H__

#include "Color.h"
#include <string>

class Cst {
 public:
  Cst(std::string textIn, const Color &colorIn);
  void add() const;
  Cst shift(const Color &newcolor) const;
 private:
  std::string text;
  const Color &color;
};

#endif
