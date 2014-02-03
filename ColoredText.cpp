#include "ColoredText.h"
#include <ncurses.h>
#include <string>

template <> void ColoredText<std::string>::print() const {
  addstr(text.c_str());
}

template <> void ColoredText<char>::print() const {
  addch(text);
}
