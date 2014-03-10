#include <ncurses.h>
#include "Cch.h"
#include "Color.h"

bool Cch::operator==(const Cch &other) const {
    return glyph == other.glyph && hue == other.hue && bold == other.bold;
}

void addc(int y, int x, const Cch &cch) {
  move(y, x);
  addc(cch);
}

void addc(const Cch &cch) {
  if (cch.bold) {
    addch(cch.glyph | A_BOLD | COLOR_PAIR(cch.hue));
  }
  else {
    addch(cch.glyph | COLOR_PAIR(cch.hue));
  }
}
