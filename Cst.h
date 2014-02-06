#ifndef __CST_H__
#define __CST_H__

#include "Color.h"
#include <string>

class Cst {
 public:
  Cst(const char* textIn);
  Cst(const std::string &textIn);

  friend void addcs(const Cst &cst);
  friend Cst Color::operator()(const Cst &cst) const;
 private:
  //constructor is private. use Color::operator()
  Cst(const std::string &textIn, const Color &colorIn);

  const std::string text;
  const Color &color;
};

#endif
