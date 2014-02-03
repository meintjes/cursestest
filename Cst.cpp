#include "Cst.h"

Cst::Cst(std::string textIn, const Color &colorIn)
  : text(textIn), color(colorIn) {

}

void Cst::add() const {
  if (color.bold) {
    attron(A_BOLD);
    attron(COLOR_PAIR(color.id));
    addstr(text.c_str());
    attroff(COLOR_PAIR(color.id));
    attroff(A_BOLD);
  }
  else {
    attron(COLOR_PAIR(color.id));
    addstr(text.c_str());
    attroff(COLOR_PAIR(color.id));
  }
}

Cst Cst::shift(const Color &newcolor) const {
  return Cst{this->text, newcolor};
}
