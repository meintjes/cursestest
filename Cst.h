#ifndef __CST_H__
#define __CST_H__

#include <string>

//see Color.h for explanation.

class Cst {
 public:
  //constructor for implicit conversion from strings literal.
  Cst(const char *textIn)
    : text(textIn), hue(8), bold(false) {}
  Cst(const std::string &textIn)
    : text(textIn), hue(8), bold(false) {}

  bool operator>(const Cst &rhs);
  bool operator<(const Cst &rhs);
  bool operator>=(const Cst &rhs);
  bool operator<=(const Cst &rhs);
  bool operator==(const Cst &rhs);

  friend void addcs(const Cst &cst);
  friend class Color;
  friend Cst operator+(const Cst &cst, const std::string &str);
  friend Cst operator+(const std::string &str, const Cst &cst);
 private:
  //constructor is private. use Color::operator()
  Cst(const std::string &textIn,
      const int &hueIn, const bool &boldIn)
    : text(textIn), hue(hueIn), bold(boldIn) {}

  const std::string text;
  const int hue;
  const bool bold;
};

void addcs(const Cst &cst);
void addcs(int x, int y, const Cst &cst);

#endif
