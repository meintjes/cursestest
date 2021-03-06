#ifndef __CCH_H__
#define __CCH_H__

//see Color.h for explanation.

class Cch {
 public:
  constexpr Cch(const char &glyphIn)
    : glyph(glyphIn), hue(8), bold(false) {}

  bool operator>(const Cch &rhs);
  bool operator<(const Cch &rhs);
  bool operator>=(const Cch &rhs);
  bool operator<=(const Cch &rhs);
  bool operator==(const Cch &rhs);

  friend void addc(const Cch &cch);
  friend struct Color;

  bool operator==(const Cch &other) const;
 private:
  //constructor is private. use Color::operator()
  constexpr Cch(const char &glyphIn, const int &hueIn, const bool &boldIn)
    : glyph(glyphIn), hue(hueIn), bold(boldIn) {}

  const char glyph;
  const int hue;
  const bool bold;
};

void addc(int x, int y, const Cch &cch);
void addc(const Cch &cch);

#endif
