#include <ncurses.h>
#include "Cch.h"
#include "Color.h"

bool Cch::operator==(const Cch &other) const {
    return glyph == other.glyph && hue == other.hue && bold == other.bold;
}

void addc(const Cch &cch) {
  if (cch.bold) {
    attron(A_BOLD);
    attron(COLOR_PAIR(cch.hue));
    addch(cch.glyph);
    attroff(COLOR_PAIR(cch.hue));
    attroff(A_BOLD);
  }
  else {
    attron(COLOR_PAIR(cch.hue));
    addch(cch.glyph);
    attroff(COLOR_PAIR(cch.hue));
  }
}
