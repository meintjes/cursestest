#include "Color.h"
#include "Cst.h"
#include "Cch.h"

const Cst Color::operator()(const Cst &cst) const {
  return Cst(cst.text, *this);
}

const Cch Color::operator()(const Cch &cch) const {
  return Cch(cch.glyph, *this);
}

bool Color::operator==(const Color &other) const {
  return hue == other.hue && bold == other.bold;
}
