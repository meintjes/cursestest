#include "Color.h"
#include "Cst.h"
#include "Cch.h"

Cst Color::operator()(const Cst &cst) const {
  return Cst(cst.text, *this);
}


Cch Color::operator()(const char &ch) const {
  return Cch(ch, *this);
}

Cch Color::operator()(const Cch &cch) const {
  return Cch(cch.glyph, *this);
}
