#ifndef __CST_H__
#define __CST_H__

#include <string>
#include "Color.h"

class Cst {
 public:
  //constructor for implicit conversion from strings literal.
  Cst(const char *textIn);
  Cst(const std::string &textIn);

  friend void addcs(const Cst &cst);
  friend class Color;
  friend Cst operator+(const Cst &cst, const std::string &str);
  friend Cst operator+(const std::string &str, const Cst &cst);
 private:
  //constructor is private. use Color::operator()
  Cst(const std::string &textIn, const Color &colorIn);

  const std::string text;
  const Color &color;
};

#endif
