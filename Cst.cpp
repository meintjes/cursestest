#include "Cst.h"
#include <ncurses.h>

Cst::Cst(const char *textIn)
  : text(textIn), color(LightGray) {

}

Cst::Cst(const std::string &textIn)
  : text(textIn), color(LightGray) {

}

Cst::Cst(const std::string &textIn, const Color &colorIn)
  : text(textIn), color(colorIn) {

}

void addcs(const Cst &cst) {
  if (cst.color.bold) {
    attron(A_BOLD);
    attron(COLOR_PAIR(cst.color.hue));
    addstr(cst.text.c_str());
    attroff(COLOR_PAIR(cst.color.hue));
    attroff(A_BOLD);
  }
  else {
    attron(COLOR_PAIR(cst.color.hue));
    addstr(cst.text.c_str());
    attroff(COLOR_PAIR(cst.color.hue));
  }
}

Cst operator+(const Cst &cst, const std::string &str) {
  return cst.color(cst.text + str);
}

Cst operator+(const std::string &str, const Cst &cst) {
  return cst.color(str + cst.text);
}
