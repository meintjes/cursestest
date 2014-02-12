#ifndef __CCH_H__
#define __CCH_H__

class Color;

class Cch {
 public:
  Cch(const char &glyphIn);
  friend void addc(const Cch &cch);
  friend class Color;

  bool operator==(const Cch &other) const;
 private:
  //constructor is private. use Color::operator()
  Cch(const char &glyphIn, const Color &colorIn);

  const char glyph;
  const Color &color;
};

#endif
