#include "ColoredText.h"
#include <ncurses.h>

template <> void Cst::print() const {
  addstr(text.c_str());
}

template <> void Cch::print() const {
  addch(text);
}
