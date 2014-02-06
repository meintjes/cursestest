#include <ncurses.h>
#include "Cch.h"

Cch::Cch(char glyphIn, const Color &colorIn)
  : glyph(glyphIn), color(colorIn) {

}

void addc(const Cch &cch) {
  if (cch.color.bold) {
    attron(A_BOLD);
    attron(COLOR_PAIR(cch.color.id));
    addch(cch.glyph);
    attroff(COLOR_PAIR(cch.color.id));
    attroff(A_BOLD);
  }
  else {
    attron(COLOR_PAIR(cch.color.id));
    addch(cch.glyph);
    attroff(COLOR_PAIR(cch.color.id));
  }
}
