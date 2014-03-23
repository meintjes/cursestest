#include "Cst.h"
#include <ncurses.h>

const char* Cst::data() {
  return text.data();
}

bool Cst::operator>(const Cst &rhs) {
  return this->text > rhs.text;
}

bool Cst::operator<(const Cst &rhs) {
  return this->text < rhs.text;
}

bool Cst::operator>=(const Cst &rhs) {
  return this->text >= rhs.text;
}

bool Cst::operator<=(const Cst &rhs) {
  return this->text <= rhs.text;
}

bool Cst::operator==(const Cst &rhs) {
  return this->text == rhs.text;
}

void addcs(int x, int y, const Cst &cst) {
  move(y, x);
  addcs(cst);
}

void addcs(const Cst &cst) {
  if (cst.bold) {
    attron(A_BOLD | COLOR_PAIR(cst.hue));
    addstr(cst.text.c_str());
    attroff(A_BOLD | COLOR_PAIR(cst.hue));
  }
  else {
    attron(COLOR_PAIR(cst.hue));
    addstr(cst.text.c_str());
    attroff(COLOR_PAIR(cst.hue));
  }
}

Cst operator+(const Cst &cst, const std::string &str) {
  return Cst(cst.text + str, cst.hue, cst.bold);
}

Cst operator+(const std::string &str, const Cst &cst) {
  return Cst(str + cst.text, cst.hue, cst.bold);
}
