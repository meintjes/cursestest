#include <ncurses.h>
#include "Cch.h"

Cch::Cch(char glyphIn, const Color &colorIn)
  : glyph(glyphIn), color(colorIn) {

}

void Cch::add() const {
  if (color.bold)
    attron(A_BOLD);
  attron(COLOR_PAIR(color.id));

  addch(glyph);

  attroff(COLOR_PAIR(color.id));
  attroff(A_BOLD);
}

Cch Cch::shift(const Color &newcolor) const {
  return Cch{this->glyph, newcolor};
}
