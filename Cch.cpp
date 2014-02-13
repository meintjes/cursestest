#include <ncurses.h>
#include "Cch.h"
#include "Color.h"

Cch::Cch(const char &glyphIn)
  : glyph(glyphIn), color(LightGray) {

}

Cch::Cch(const char &glyphIn, const Color &colorIn)
  : glyph(glyphIn), color(colorIn) {

}

bool Cch::operator==(const Cch &other) const {
    return glyph == other.glyph && color == other.color;
}

void addc(const Cch &cch) {
  if (cch.color.bold) {
    attron(A_BOLD);
    attron(COLOR_PAIR(cch.color.hue));
    addch(cch.glyph);
    attroff(COLOR_PAIR(cch.color.hue));
    attroff(A_BOLD);
  }
  else {
    attron(COLOR_PAIR(cch.color.hue));
    addch(cch.glyph);
    attroff(COLOR_PAIR(cch.color.hue));
  }
}
